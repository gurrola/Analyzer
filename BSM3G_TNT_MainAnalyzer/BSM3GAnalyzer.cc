////////////////////////////////////////////////////////////////////////////// 
// Authors:                Alfredo Gurrola                                  //
// contact:                                                                 //
//   Alfredo.Gurrola@cern.ch       (Vanderbilt University)                  // 
//////////////////////////////////////////////////////////////////////////////

#include "BSM3GAnalyzer.h"

//---main function
int main (int argc, char *argv[]) {

  TApplication app("App",&argc, argv);
  TFile * fs = new TFile("analysis.root", "RECREATE");
  BSM3GAnalyzer BSM3GAnalyzer_(fs);

}

BSM3GAnalyzer::BSM3GAnalyzer(TFile* theFile) {

  //---obtain the configurable inputs
  getInputs();
  //std::cout << "Finished grabbing configurable inputs" << std::endl;

  //---open input root file and read in the tree information
  TFile *f = new TFile ("OutTree_9_1_dab.root");
  f->cd("TNT");
  TTree* BOOM = (TTree*)f->Get("TNT/BOOM");
  int nentries = (int) BOOM->GetEntries();
  setBranchAddress(BOOM);
  //std::cout << "Finished opening the input root file and reading in the tree information" << std::endl;

  //---initialize event counters and maps
  beginJob();
  //std::cout << "Finished initializing event counters and maps" << std::endl;

  //---loop over events and perform the analysis
  //for (int entries = 0; entries < 1000; ++entries) {
  for(int entries = 0; entries < nentries; ++entries) {
    if((entries+1) % 100 == 0) { std::cout << "Analyzing Event #" << entries+1 << std::endl; }
    BOOM->GetEntry(entries);
    analyze(theFile);
  }
  //std::cout << "Finished looping over events" << std::endl;

  //---write out histograms and printout cut flow efficiency table
  endJob(theFile);
  //std::cout << "Finished writing out histograms" << std::endl;

}

//---function used to obtain the configurable inputs from the .in files
void BSM3GAnalyzer::getInputs() {

  string inputString;
  string inputType;

  //---open input file containing all configurable parameters
  ifstream inCutParametersFile;
  char inputCutParametersFilename[] = "BSM3GAnalyzer_CutParameters.in";
  inCutParametersFile.open(inputCutParametersFilename, ios::in);
  //---if can't open input file, then exit
  if (!inCutParametersFile) {
    cerr << "Can't open the input configuration file " << inputCutParametersFilename << endl;
    exit(1);
  }

  //---grab all relevant object level cut information from the input file
  while (inCutParametersFile >> inputType >> inputString) {
    if(inputType == "GenTauPtMinCut") {
      _GenTauPtMinCut = atof(inputString.c_str()); // minimum tau pt requirement for gen vis had taus
    } else if(inputType == "GenTauPtMaxCut") {
      _GenTauPtMaxCut = atof(inputString.c_str()); // maximum tau pt requirement for gen vis had taus
    } else if(inputType == "GenTauEtaMaxCut") {
      _GenTauEtaMaxCut = atof(inputString.c_str()); // maximum eta requirement for gen vis had taus
    } else if(inputType == "GenTauNmin") {
      _GenTauNmin = (int)(atof(inputString.c_str())); // minimum number of gen vis had taus passing cuts
    } else if(inputType == "GenTauNmax") {
      _GenTauNmax = (int)(atof(inputString.c_str())); // maximum number of gen vis had taus passing cuts
    } else if(inputType == "GenTopNmin") {
      _GenTopNmin = (int)(atof(inputString.c_str())); // minimum number of gen top quarks passing cuts
    } else if(inputType == "GenTopNmax") {
      _GenTopNmax = (int)(atof(inputString.c_str())); // maximum number of gen top quarks passing cuts
    } else if(inputType == "GenElectronNmin") {
      _GenElectronNmin = (int)(atof(inputString.c_str())); // minimum number of gen electrons passing cuts
    } else if(inputType == "GenElectronNmax") {
      _GenElectronNmax = (int)(atof(inputString.c_str())); // maximum number of gen electrons passing cuts
    } else if(inputType == "GenMuonNmin") {
      _GenMuonNmin = (int)(atof(inputString.c_str())); // minimum number of gen muons passing cuts
    } else if(inputType == "GenMuonNmax") {
      _GenMuonNmax = (int)(atof(inputString.c_str())); // maximum number of gen muons passing cuts
    } else if(inputType == "GenZNmin") {
      _GenZNmin = (int)(atof(inputString.c_str())); // minimum number of gen Zs passing cuts
    } else if(inputType == "GenZNmax") {
      _GenZNmax = (int)(atof(inputString.c_str())); // maximum number of gen Zs passing cuts
    } else if(inputType == "GenWNmin") {
      _GenWNmin = (int)(atof(inputString.c_str())); // minimum number of gen Ws passing cuts
    } else if(inputType == "GenWNmax") {
      _GenWNmax = (int)(atof(inputString.c_str())); // maximum number of gen Ws passing cuts
    } else if(inputType == "GenSMHiggsNmin") {
      _GenSMHiggsNmin = (int)(atof(inputString.c_str())); // minimum number of gen SMHiggs passing cuts
    } else if(inputType == "GenSMHiggsNmax") {
      _GenSMHiggsNmax = (int)(atof(inputString.c_str())); // maximum number of gen SMHiggs passing cuts
    } else if(inputType == "RecoVertexNmin") {
      _RecoVertexNmin = (int)(atof(inputString.c_str())); // minimum number of primary vertices passing cuts
    } else if(inputType == "RecoVertexNmax") {
      _RecoVertexNmax = (int)(atof(inputString.c_str())); // maximum number of primary vertices passing cuts
    } else if(inputType == "RecoMuon1Nmin") {
      _RecoMuon1Nmin = (int)(atof(inputString.c_str())); // minimum number of reco muon1 objects passing cuts
    } else if(inputType == "RecoMuon1Nmax") {
      _RecoMuon1Nmax = (int)(atof(inputString.c_str())); // maximum number of reco muon1 objects passing cuts
    } else if(inputType == "RecoMuon2Nmin") {
      _RecoMuon2Nmin = (int)(atof(inputString.c_str())); // minimum number of reco muon2 objects passing cuts
    } else if(inputType == "RecoMuon2Nmax") {
      _RecoMuon2Nmax = (int)(atof(inputString.c_str())); // maximum number of reco muon2 objects passing cuts
    } else if(inputType == "DoRecoMuon1DiscrByTightID") {
      _DoRecoMuon1DiscrByTightID = inputString; // "1" to require the reco muon1 to pass "tight" ID; "0" to disable the "tight" ID cut
    } else if(inputType == "DoRecoMuon1DiscrBySoftID") {
      _DoRecoMuon1DiscrBySoftID = inputString; // "1" to require the reco muon1 to pass "soft" ID; "0" to disable the "soft" ID cut
    } else if(inputType == "DoRecoMuon1DiscrByIsolation") {
      _DoRecoMuon1DiscrByIsolation = inputString; // "1" to require the reco muon1 to pass isolation; "0" to disable the isolation cut
    } else if(inputType == "RecoMuon1EtaCut") {
      _RecoMuon1EtaCut = atof(inputString.c_str()); // maximum reco muon1 eta requirement
    } else if(inputType == "RecoMuon1PtMinCut") {
      _RecoMuon1PtMinCut = atof(inputString.c_str()); // minimum reco muon1 pt requirement
    } else if(inputType == "RecoMuon1PtMaxCut") {
      _RecoMuon1PtMaxCut = atof(inputString.c_str()); // maximum reco muon1 pt requirement
    } else if(inputType == "RecoMuon1IsoSumPtMaxCutValue") {
      _RecoMuon1IsoSumPtMaxCutValue = atof(inputString.c_str()); // maximum reco muon1 iso requirement
    } else if(inputType == "RecoMuon1IsoSumPtMinCutValue") {
      _RecoMuon1IsoSumPtMinCutValue = atof(inputString.c_str()); // minimum reco muon1 iso requirement
    } else if(inputType == "DoRecoMuon2DiscrByTightID") {
      _DoRecoMuon2DiscrByTightID = inputString; // "1" to require the reco muon2 to pass "tight" ID; "0" to disable the "tight" ID cut
    } else if(inputType == "DoRecoMuon2DiscrBySoftID") {
      _DoRecoMuon2DiscrBySoftID = inputString; // "1" to require the reco muon2 to pass "soft" ID; "0" to disable the "soft" ID cut
    } else if(inputType == "DoRecoMuon2DiscrByIsolation") {
      _DoRecoMuon2DiscrByIsolation = inputString; // "1" to require the reco muon2 to pass isolation; "0" to disable the isolation cut
    } else if(inputType == "RecoMuon2EtaCut") {
      _RecoMuon2EtaCut = atof(inputString.c_str()); // maximum reco muon2 eta requirement
    } else if(inputType == "RecoMuon2PtMinCut") {
      _RecoMuon2PtMinCut = atof(inputString.c_str()); // minimum reco muon2 pt requirement
    } else if(inputType == "RecoMuon2PtMaxCut") {
      _RecoMuon2PtMaxCut = atof(inputString.c_str()); // maximum reco muon2 pt requirement
    } else if(inputType == "RecoMuon2IsoSumPtMaxCutValue") {
      _RecoMuon2IsoSumPtMaxCutValue = atof(inputString.c_str()); // maximum reco muon2 iso requirement
    } else if(inputType == "RecoMuon2IsoSumPtMinCutValue") {
      _RecoMuon2IsoSumPtMinCutValue = atof(inputString.c_str()); // minimum reco muon2 iso requirement
    } else if(inputType == "TreatMuonsAsNeutrinos") {
      _TreatMuonsAsNeutrinos = inputString; // "1" to treat muons as neutrinos and recalculate MET; "0" to disable this functionality
    } else if(inputType == "CalculatePUSystematics") {
      _CalculatePUSystematics = inputString; // "1" to allow pileup re-weighting; "0" to disable pileup weights
    } else if(inputType == "SmearTheMuon") {
      _SmearTheMuon = inputString; // "1" to smear the muon momentum; "0" to disable muon momentum smearing
    } else if(inputType == "MatchMuonToGen") {
      _MatchMuonToGen = inputString; // "1" to match the reco muon to a gen muon; "0" to disable muon matching
    } else if(inputType == "UseMuonMotherId") {
      _UseMuonMotherId = inputString; // "1" to use mother information (pdgID) for muon gen matching; "0" to disable it
    } else if(inputType == "MuonMotherId") {
      _MuonMotherId = (int)(atof(inputString.c_str())); // muon mother pdg id
    } else if(inputType == "MuonToGenMatchingDeltaR") {
      _MuonToGenMatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used to match reco muons to gen muons
    } else if(inputType == "MuonPtScaleOffset") {
      _MuonPtScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the muon pt scale
    } else if(inputType == "MuonPtSigmaOffset") {
      _MuonPtSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the muon pt resolution
    } else if(inputType == "MuonEtaScaleOffset") {
      _MuonEtaScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the muon eta scale
    } else if(inputType == "MuonEtaSigmaOffset") {
      _MuonEtaSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the muon eta resolution
    } else if(inputType == "MuonPhiScaleOffset") {
      _MuonPhiScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the muon phi scale
    } else if(inputType == "MuonPhiSigmaOffset") {
      _MuonPhiSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the muon phi resolution
    } else if(inputType == "MuonEnergyScaleOffset") {
      _MuonEnergyScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the muon energy scale
    } else if(inputType == "MuonEnergySigmaOffset") {
      _MuonEnergySigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the muon energy resolution
    } else if(inputType == "SmearTheTau") {
      _SmearTheTau = inputString; // "1" to smear the tau momentum; "0" to disable tau momentum smearing
    } else if(inputType == "TauToGenMatchingDeltaR") {
      _TauToGenMatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used to match reco taus to gen taus
    } else if(inputType == "TauPtScaleOffset") {
      _TauPtScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the tau pt scale
    } else if(inputType == "TauPtSigmaOffset") {
      _TauPtSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the tau pt resolution
    } else if(inputType == "TauEtaScaleOffset") {
      _TauEtaScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the tau eta scale
    } else if(inputType == "TauEtaSigmaOffset") {
      _TauEtaSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the tau eta resolution
    } else if(inputType == "TauPhiScaleOffset") {
      _TauPhiScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the tau phi scale
    } else if(inputType == "TauPhiSigmaOffset") {
      _TauPhiSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the tau phi resolution
    } else if(inputType == "TauEnergyScaleOffset") {
      _TauEnergyScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the tau energy scale
    } else if(inputType == "TauEnergySigmaOffset") {
      _TauEnergySigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the tau energy resolution
    } else if(inputType == "DataHistos") {
      _DataHistos = inputString; // pileup root file name for data
    } else if(inputType == "MCHistos") {
      _MCHistos = inputString; // pileup root file name for MC
    } else if(inputType == "isData") {
      isData = inputString; // are you running over data? "1" for "yes", "0" for "no"
    } else if(inputType == "FillGenHists") {
      _FillGenHists = inputString; // do you want to fill the gen tau histograms? "1" for "yes", "0" for "no"
    } else {
      cerr << "Incorrect input type " << inputType << endl; // exit code if unwanted input is specified
      exit(1);
    }
  }

  //---close the .in file
  inCutParametersFile.close();

  //---open theinput file containing all event level selection requirements
  ifstream inEventRequirementsFile;
  char inputEventRequirementsFilename[] = "BSM3GAnalyzer_EventRequirements.in";
  inEventRequirementsFile.open(inputEventRequirementsFilename, ios::in);
  //---if can't open input file, then exit
  if (!inEventRequirementsFile) {
    cerr << "Can't open the input configuration file " << inputEventRequirementsFilename << endl;
    exit(1);
  }

  //---grab the event level requirements
  while (inEventRequirementsFile >> inputString) {
    if(inputString != "EventSelectionSequence") {_EventSelectionSequence.push_back(inputString);} 
  }

  //---close the .in file
  inEventRequirementsFile.close();

  //---open the input file containing all configurable parameters
  ifstream inCutFoldersFile;
  char inputCutFoldersFilename[] = "BSM3GAnalyzer_CutFolders.in";
  inCutFoldersFile.open(inputCutFoldersFilename, ios::in);
  //---if can't open input file, exit the code
  if (!inCutFoldersFile) {
    cerr << "Can't open the input configuration file " << inputCutFoldersFilename << endl;
    exit(1);
  }

  //---grab the names of the cut directories
  while (inCutFoldersFile >> inputString) {
    if(inputString != "TopologicalSelectionSequence") {_TopologicalSelectionSequence.push_back(inputString);} 
  }

  //---close the .in file
  inCutFoldersFile.close();

}

//---function called once just before starting the event loop. It initiates the event counters and maps.
void BSM3GAnalyzer::beginJob() {
  _totalEvents = 0;  
  _totalEventsPassingCuts = 0;  
  setMapSelectionAlgoIDs();
  initMapSelectionCounters();
}

//---this function is called once for each event. It performs the main analysis (cuts, filling of histograms, etc.)
void BSM3GAnalyzer::analyze(TFile *theFile) {
  
  //---Number of events analyzed (denominator)
  _totalEvents++;
  //std::cout << "Finished incrementing event counter" << std::endl;

  //---Get weights for the calculation of pdf systematic uncertainties for the denominator
  pdfWeightVector.clear();
  pdfWeightVector.push_back(1); // set the weight to 1 for the moment ...
  //std::cout << "Finished initializing the pdf weight vector" << std::endl;
  
  //---This is where the pileup weight is calculated
  if((_CalculatePUSystematics == "1") && (isData == "0") ) { pu_weight = getPileupWeight(trueInteractions); }
  else { pu_weight = 1.0; }
  //std::cout << "Finished calculating pu weights" << std::endl;

  //---deltas for recalculation of MET (used when studying systematics or treating muons as neutrinos/taus)
  deltaForMEx = 0;
  deltaForMEy = 0;

  //---If requested by the user, smear the momentum and position for systematic uncertanties and calculation of MET deltas
  smearedMuonMomentumVector.clear();
  if(_SmearTheMuon == "1") {
    for(int j = 0; j < Muon_pt->size(); j++) {
      TLorentzVector unsmearedMuonMomentum;
      unsmearedMuonMomentum.SetPtEtaPhiE(Muon_pt->at(j), Muon_eta->at(j), Muon_phi->at(j), Muon_energy->at(j));
      smearedMuonMomentumVector.push_back(SmearMuon(j));
      deltaForMEx = deltaForMEx + unsmearedMuonMomentum.Px() - SmearMuon(j).Px();
      deltaForMEy = deltaForMEy + unsmearedMuonMomentum.Py() - SmearMuon(j).Py();
    }
  } else {
    for(int j = 0; j < Muon_pt->size(); j++) {
      TLorentzVector unsmearedMuonMomentum;
      unsmearedMuonMomentum.SetPtEtaPhiE(Muon_pt->at(j), Muon_eta->at(j), Muon_phi->at(j), Muon_energy->at(j));
      smearedMuonMomentumVector.push_back(unsmearedMuonMomentum);
    }
  }
  smearedTauMomentumVector.clear();
  if(_SmearTheTau == "1") {
    for(int j = 0; j < Tau_pt->size(); j++) {
      TLorentzVector unsmearedTauMomentum;
      unsmearedTauMomentum.SetPtEtaPhiE(Tau_pt->at(j), Tau_eta->at(j), Tau_phi->at(j), Tau_energy->at(j));
      smearedTauMomentumVector.push_back(SmearTau(j));
      deltaForMEx = deltaForMEx + unsmearedTauMomentum.Px() - SmearTau(j).Px();
      deltaForMEy = deltaForMEy + unsmearedTauMomentum.Py() - SmearTau(j).Py();
    }
  } else {
    for(int j = 0; j < Tau_pt->size(); j++) {
      TLorentzVector unsmearedTauMomentum;
      unsmearedTauMomentum.SetPtEtaPhiE(Tau_pt->at(j), Tau_eta->at(j), Tau_phi->at(j), Tau_energy->at(j));
      smearedTauMomentumVector.push_back(unsmearedTauMomentum);
    }
  }

  //---recalculate MET
  double temppx = Met_px + deltaForMEx;
  double temppy = Met_py + deltaForMEy;
  double temppz = Met_pz;
  double temppt = TMath::Sqrt((temppx*temppx) + (temppy*temppy));
  TLorentzVector theTempMETVector(temppx,temppy,temppz,temppt);
  theMETVector = theTempMETVector;

  //---book histograms at the beginning of the events loop
  if(_totalEvents == 1) {
    //std::cout << "Event number " << _totalEvents << std::endl;
    for(unsigned int i = 0 ; i < _TopologicalSelectionSequence.size(); i++) {
      string theDirectory = _TopologicalSelectionSequence[i];
      //std::cout << "Directory = " << theDirectory << std::endl;
      bookHistograms(theFile, theDirectory.c_str(), i);
    }
  }
  //std::cout << "Finished booking histograms" << std::endl;
  
  //---this is a placeholder for the moment ... eventually this will be a product of all the weights
  isrgluon_weight = pu_weight;
  
  //---Get the event flags (did the event pass the cuts?)
  getEventFlags();
  //std::cout << "Finished getting event flags" << std::endl;

  //---loop over the map of event flags and fill histograms 
  for(unsigned int i = 0 ; i < _EventSelectionSequence.size(); i++) {
    string theCut = _EventSelectionSequence[i];
    bool passedSelection = pass_i_EventSelectionSequence(i);
    for(unsigned int j = 0; j < _TopologicalSelectionSequence.size(); j++) {
      if(_TopologicalSelectionSequence[j] == theCut.c_str()) {
        for(unsigned int NpdfID = 0; NpdfID < pdfWeightVector.size();  NpdfID++) {
          _hEvents[j][NpdfID]->Fill(0.0,isrgluon_weight * pdfWeightVector.at(NpdfID));
        }
        if(passedSelection == true) {
          //------Number of events passing cuts (numerator)
          for(unsigned int NpdfID = 0; NpdfID < pdfWeightVector.size();  NpdfID++) {
            _hEvents[j][NpdfID]->Fill(1.0,isrgluon_weight * pdfWeightVector.at(NpdfID));
          }
          fillHistograms(j);
        }
      }
    }
  }
  //std::cout << "Finished initializing the pdf weight vector" << std::endl;
  
  //---Number of events passing cuts (numerator)
  if(passEventSelectionSequence()) {_totalEventsPassingCuts++;}
  //std::cout << "Finished incrementing passing events counter" << std::endl;
  
}

void BSM3GAnalyzer::initMapSelectionCounters() {
  for (unsigned int i=0;i<_EventSelectionSequence.size();i++) {
    _mapSelectionCounter[_EventSelectionSequence[i]] = 0;
    _mapSelectionCounterCumul[_EventSelectionSequence[i]] = 0;
  }
}

void BSM3GAnalyzer::printEfficiency() {
  cout.setf(ios::floatfield,ios::fixed);
  cout<<setprecision(3);
  cout << "\n";
  cout << "Selection Efficiency " << "\n";
  cout << "Total events: " << _totalEvents << "\n";
  cout << "         Name                     Indiv.      Cumulative\n";
  cout << "---------------------------------------------------------------------------\n";
  for (unsigned int i=0;i<_EventSelectionSequence.size();i++) {
    cout<<setw(24)<<_EventSelectionSequence[i]<<" "
	<<setw(6)<<_mapSelectionCounter[_EventSelectionSequence[i]]<<" ("
	<<setw(8)<<(float)_mapSelectionCounter[_EventSelectionSequence[i]]/(float)_totalEvents<<") "
	<<setw(6)<<_mapSelectionCounterCumul[_EventSelectionSequence[i]]<<"( "
	<<setw(8)<<(float)_mapSelectionCounterCumul[_EventSelectionSequence[i]]/(float)_totalEvents<<") "
	<<endl;
  }
  cout << "---------------------------------------------------------------------------\n";  
}

void BSM3GAnalyzer::setMapSelectionAlgoIDs() {
  for (unsigned int i=0;i<_EventSelectionSequence.size();i++) { _mapSelectionAlgoID[_EventSelectionSequence[i]] = i; }
}

void BSM3GAnalyzer::getEventFlags() {

  //---initiate event flags
  _EventFlag.clear();
  for (unsigned int i=0; i<_EventSelectionSequence.size(); i++) { _EventFlag.push_back(false); }

  //---Gen level tau requirements
  int nGenTaus = ExtractNumberOfGoodGenTaus();
  if (nGenTaus>=_GenTauNmin) _EventFlag[_mapSelectionAlgoID["GenTauNmin"]] = true;
  if (nGenTaus<=_GenTauNmax) _EventFlag[_mapSelectionAlgoID["GenTauNmax"]] = true;

  //---Gen level top requirements
  int nGenTop = ExtractNumberOfGoodGenTops();
  if (nGenTop>=_GenTopNmin) _EventFlag[_mapSelectionAlgoID["GenTopNmin"]] = true;
  if (nGenTop<=_GenTopNmax) _EventFlag[_mapSelectionAlgoID["GenTopNmax"]] = true;

  //---Gen level electron requirements
  int nGenElectrons = ExtractNumberOfGoodGenElectrons();
  if (nGenElectrons>=_GenElectronNmin) _EventFlag[_mapSelectionAlgoID["GenElectronNmin"]] = true;
  if (nGenElectrons<=_GenElectronNmax) _EventFlag[_mapSelectionAlgoID["GenElectronNmax"]] = true;

  //---Gen level muon requirements
  int nGenMuons = ExtractNumberOfGoodGenMuons();
  if (nGenMuons>=_GenMuonNmin) _EventFlag[_mapSelectionAlgoID["GenMuonNmin"]] = true;
  if (nGenMuons<=_GenMuonNmax) _EventFlag[_mapSelectionAlgoID["GenMuonNmax"]] = true;

  //---Gen level Z requirements
  int nGenZ = ExtractNumberOfGoodGenZs();
  if (nGenZ>=_GenZNmin) _EventFlag[_mapSelectionAlgoID["GenZNmin"]] = true;
  if (nGenZ<=_GenZNmax) _EventFlag[_mapSelectionAlgoID["GenZNmax"]] = true;

  //---Gen level W requirements
  int nGenW = ExtractNumberOfGoodGenWs();
  if (nGenW>=_GenWNmin) _EventFlag[_mapSelectionAlgoID["GenWNmin"]] = true;
  if (nGenW<=_GenWNmax) _EventFlag[_mapSelectionAlgoID["GenWNmax"]] = true;

  //---Gen level SM Higgs requirements
  int nGenSMHiggs = ExtractNumberOfGoodGenSMHiggs();
  if (nGenSMHiggs>=_GenSMHiggsNmin) _EventFlag[_mapSelectionAlgoID["GenSMHiggsNmin"]] = true;
  if (nGenSMHiggs<=_GenSMHiggsNmax) _EventFlag[_mapSelectionAlgoID["GenSMHiggsNmax"]] = true;

  //---Number of Good Vertices
  if (bestVertices>=_RecoVertexNmin) _EventFlag[_mapSelectionAlgoID["RecoVertexNmin"]] = true;
  if (bestVertices<=_RecoVertexNmax) _EventFlag[_mapSelectionAlgoID["RecoVertexNmax"]] = true;

  //---deltas for recalculation of MET (used when studying systematics or treating muons as neutrinos/taus)
  deltaForMEx = 0;
  deltaForMEy = 0;

  //---Reco level muon1 requirements
  int nGoodCandidatesMuon1 = 0;
  for(int j = 0; j < Muon_pt->size(); j++) {
    if( ((passRecoMuon1Cuts(j)) || (passRecoMuon2Cuts(j))) && (_TreatMuonsAsNeutrinos == "1") ) {
      deltaForMEx += smearedMuonMomentumVector.at(j).Px();
      deltaForMEy += smearedMuonMomentumVector.at(j).Py();
    }
    if (!passRecoMuon1Cuts(j)) continue;
    nGoodCandidatesMuon1++;
  }
  if (nGoodCandidatesMuon1>=_RecoMuon1Nmin) _EventFlag[_mapSelectionAlgoID["RecoMuon1Nmin"]] = true;
  if (nGoodCandidatesMuon1<=_RecoMuon1Nmax) _EventFlag[_mapSelectionAlgoID["RecoMuon1Nmax"]] = true;

  //---Reco level muon2 requirements
  int nGoodCandidatesMuon2 = 0;
  for(int j = 0; j < Muon_pt->size(); j++) {
    if (!passRecoMuon2Cuts(j)) continue;
    nGoodCandidatesMuon2++;
  }
  if (nGoodCandidatesMuon2>=_RecoMuon2Nmin) _EventFlag[_mapSelectionAlgoID["RecoMuon2Nmin"]] = true;
  if (nGoodCandidatesMuon2<=_RecoMuon2Nmax) _EventFlag[_mapSelectionAlgoID["RecoMuon2Nmax"]] = true;

  //---recalculate MET
  double temppx = theMETVector.Px() + deltaForMEx;
  double temppy = theMETVector.Py() + deltaForMEy;
  double temppz = theMETVector.Pz();
  double temppt = TMath::Sqrt((temppx*temppx) + (temppy*temppy));
  TLorentzVector theTempMETVector(temppx,temppy,temppz,temppt);
  theMETVector = theTempMETVector;

}

// --------Count the number of events passing the selection criteria
bool BSM3GAnalyzer::passEventSelectionSequence() {
  bool cumulDecision = true;
  for (unsigned int i=0;i<_EventSelectionSequence.size();i++) {
    cumulDecision = cumulDecision && _EventFlag[i]; 
    if (_EventFlag[i]) { (_mapSelectionCounter[_EventSelectionSequence[i]])++; }
    if (cumulDecision) { (_mapSelectionCounterCumul[_EventSelectionSequence[i]])++; }
  }  
  return cumulDecision;
}

bool BSM3GAnalyzer::pass_i_EventSelectionSequence(unsigned cut) {
  bool cumulDecision = true;
  if(cut == 0) {
    cumulDecision = cumulDecision && _EventFlag[cut];
  } else {
    for(unsigned int i=0;i<= cut ;i++) {
      cumulDecision = cumulDecision && _EventFlag[i];
    }
  }
  return cumulDecision;
}

// ---------------Fill Histograms
void BSM3GAnalyzer::fillHistograms(unsigned int i) {
  
  for(unsigned int NpdfID = 0; NpdfID < pdfWeightVector.size();  NpdfID++){
    
    // ------Generated Taus
    if ( (_FillGenHists == "1") && (isData == "0") ) {
      int nGenTaus = 0;
      for(int j = 0; j < Gen_pt->size(); j++) {
        if((abs(Gen_pdg_id->at(j)) == 15) && (Gen_status->at(j) != 3)) {
          _hGenTauEnergy[i][NpdfID]->Fill(Gen_energy->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauPt[i][NpdfID]->Fill(Gen_pt->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauEta[i][NpdfID]->Fill(Gen_eta->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauPhi[i][NpdfID]->Fill(Gen_phi->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          nGenTaus++;
	}
      }
      int nGenMuons = 0;
      for(int j = 0; j < Gen_pt->size(); j++) {
        if((abs(Gen_pdg_id->at(j)) == 13) && (Gen_status->at(j) == 1)) {
          _hGenMuonEnergy[i][NpdfID]->Fill(Gen_energy->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenMuonPt[i][NpdfID]->Fill(Gen_pt->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenMuonEta[i][NpdfID]->Fill(Gen_eta->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenMuonPhi[i][NpdfID]->Fill(Gen_phi->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          nGenMuons++;
	}
      }
      _hNGenTau[i][NpdfID]->Fill(nGenTaus,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hNGenMuon[i][NpdfID]->Fill(nGenMuons,isrgluon_weight * pdfWeightVector.at(NpdfID));
    }
    
  }
  
}

// ---------------
void BSM3GAnalyzer::bookHistograms(TFile * theOutFile, string mydirectory , unsigned int i) {

  //std::cout << "Creating directory" << std::endl;
  theOutFile->cd();
  TDirectory *subDir = theOutFile->mkdir( mydirectory.c_str() );
  //std::cout << "Created directory" << std::endl;

  theOutFile->cd( mydirectory.c_str() );
  //std::cout << "Created directory and cd'd into it" << std::endl;
  
  // Initialize stringstream used to name histograms for each PDF weight
  std::stringstream j;
  j.str("");
  
  // The loop below is used to create a different histogram for each event weighting factor (for systematic studies or renomarlization w/ respect to data).
  // If reweighting booleans are set to false, then event weight will be set to 1 and only 1 histogram per variable will be created.
  for(unsigned int NpdfCounter = 0; NpdfCounter < pdfWeightVector.size();  NpdfCounter++) {
    j << NpdfCounter;

    //std::cout << "Looping over NpdfCounter " << NpdfCounter << std::endl;
    //--- histogram containing the number of events analyzed and number passing specificied cuts
    _hEvents[i][NpdfCounter] = new TH1F(("Events_"+j.str()).c_str(), ("Events_"+j.str()).c_str(), 2, 0., 2.);
    
    //--- book generator level histograms
    if (_FillGenHists == "1") {
      _hNGenTau[i][NpdfCounter]                         = new TH1F(("NGenTau_"+j.str()).c_str(),                        ("NGenTau_"+j.str()).c_str(),      20, 0., 20.);
      _hGenTauEnergy[i][NpdfCounter]                    = new TH1F(("GenTauEnergy_"+j.str()).c_str(),                   ("GenTauEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hGenTauPt[i][NpdfCounter]                        = new TH1F(("GenTauPt_"+j.str()).c_str(),                       ("GenTauPt_"+j.str()).c_str(),     200, 0., 500.);
      _hGenTauEta[i][NpdfCounter]                       = new TH1F(("GenTauEta_"+j.str()).c_str(),                      ("GenTauEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenTauPhi[i][NpdfCounter]                       = new TH1F(("GenTauPhi_"+j.str()).c_str(),                      ("GenTauPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hNGenMuon[i][NpdfCounter]                         = new TH1F(("NGenMuon_"+j.str()).c_str(),                        ("NGenMuon_"+j.str()).c_str(),      20, 0., 20.);
      _hGenMuonEnergy[i][NpdfCounter]                    = new TH1F(("GenMuonEnergy_"+j.str()).c_str(),                   ("GenMuonEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hGenMuonPt[i][NpdfCounter]                        = new TH1F(("GenMuonPt_"+j.str()).c_str(),                       ("GenMuonPt_"+j.str()).c_str(),     200, 0., 500.);
      _hGenMuonEta[i][NpdfCounter]                       = new TH1F(("GenMuonEta_"+j.str()).c_str(),                      ("GenMuonEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenMuonPhi[i][NpdfCounter]                       = new TH1F(("GenMuonPhi_"+j.str()).c_str(),                      ("GenMuonPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
    }
    
    j.str("");
  }
}

// ---------------
void BSM3GAnalyzer::writeHistograms(TFile *theOutFile, string mydirectory , unsigned int i) {

  theOutFile->cd( mydirectory.c_str() );
  for(unsigned int NpdfCounter = 0; NpdfCounter < pdfWeightVector.size();  NpdfCounter++) {
    _hEvents[i][NpdfCounter]->Write();

    //--- book generator level histograms
    if (_FillGenHists == "1") {
      _hNGenTau[i][NpdfCounter]->Write();
      _hGenTauEnergy[i][NpdfCounter]->Write();
      _hGenTauPt[i][NpdfCounter]->Write();
      _hGenTauEta[i][NpdfCounter]->Write();
      _hGenTauPhi[i][NpdfCounter]->Write();
      _hNGenMuon[i][NpdfCounter]->Write();
      _hGenMuonEnergy[i][NpdfCounter]->Write();
      _hGenMuonPt[i][NpdfCounter]->Write();
      _hGenMuonEta[i][NpdfCounter]->Write();
      _hGenMuonPhi[i][NpdfCounter]->Write();
    }

  }
}

// ------------ method called once each job just after ending the event loop  ------------
void BSM3GAnalyzer::endJob(TFile *theOutFile) {

  printEfficiency();  

  theOutFile->cd();
  for(unsigned int i = 0 ; i < _TopologicalSelectionSequence.size(); i++) {
    string theDirectory = _TopologicalSelectionSequence[i];
    writeHistograms(theOutFile, theDirectory.c_str(), i);
  }
  theOutFile->Close();

}

double BSM3GAnalyzer::getPileupWeight(float ntruePUInt) {

  int bin;
  double MCintegral;
  double MCvalue;
  double Dataintegral;
  double Datavalue;
  char * cstr1;
  char * cstr2;
    
  // Filenames must be c_strings below. Here is the conversion from strings to c_strings
  cstr1 = new char [_MCHistos.size()+1];
  strcpy (cstr1, _MCHistos.c_str());
  cstr2 = new char [_DataHistos.size()+1];
  strcpy (cstr2, _DataHistos.c_str());

  // As you can see above cstr1 corresponds to MC and cstr2 corresponds to data.
  // The probability that data (or MC) has N pileup interactions is value / integral
  // The ratio of the data and MC probability density functions gives us our pileup weights
    
  bin = getBin(cstr1,ntruePUInt);
  MCvalue = getvalue(cstr1,bin);
  MCintegral = getintegral(cstr1,bin);
    
  Datavalue = getvalue(cstr2,bin);
  Dataintegral = getintegral(cstr2,bin);
    
  //Ratio of normalized histograms in given bin
  if((MCvalue * Dataintegral) != 0) {pu_weight = (Datavalue * MCintegral) / (MCvalue * Dataintegral);}
  else {pu_weight = 1.0;}

  // printout statements for debugging    
  // cout << "pu_weight: " << pu_weight << " nVertices: " << nVertices << std::endl;
  // cout << "MCvalue: " << MCvalue << " MCintegral: " << MCintegral << " Datavalue: " << Datavalue << " Dataintegral: " << Dataintegral << std::endl;

  return pu_weight;
}

// Gen level tau requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenTaus() {
  int nGenTaus = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 15) && (Gen_status->at(j) != 3)) {
        if( (Gen_pt->at(j) >= _GenTauPtMinCut) && (abs(Gen_eta->at(j)) <= _GenTauEtaMaxCut) ) {
          nGenTaus++;
        }
      }
    }
  }
  return nGenTaus;
}

// Gen level top requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenTops() {
  int nGenTop = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 6) && (Gen_status->at(j) == 2)) {nGenTop++;}
    }
  }
  return nGenTop;
}

// Gen level electron requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenElectrons() {
  int nGenElectrons = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 11) && (Gen_status->at(j) == 1)) {nGenElectrons++;}
    }
  }
  return nGenElectrons;
}

// Gen level muon requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenMuons() {
  int nGenMuons = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 13) && (Gen_status->at(j) == 1)) {nGenMuons++;}
    }
  }
  return nGenMuons;
}

// Gen level Z requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenZs() {
  int nGenZ = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 23) && (Gen_status->at(j) == 2)) {nGenZ++;}
    }
  }
  return nGenZ;
}

// Gen level W requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenWs() {
  int nGenW = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 24) && (Gen_status->at(j) == 2)) {nGenW++;}
    }
  }
  return nGenW;
}

// Gen level SM Higgs requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenSMHiggs() {
  int nGenSMHiggs = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 25) && (Gen_status->at(j) == 2)) {nGenSMHiggs++;}
    }
  }
  return nGenSMHiggs;
}

// Reco level muon1 requirements
bool BSM3GAnalyzer::passRecoMuon1Cuts(int nobj) {
  //----Matching to gen
  if((_MatchMuonToGen == "1") && (isData == "0")) {
    if(!(matchMuonToGen(smearedMuonMomentumVector.at(nobj)).first)) {return false;}
  }

  //----Require muon to pass ID discriminators
  if (_DoRecoMuon1DiscrByTightID == "1") {if (Muon_tight->at(nobj) == 0) {return false;}}
  if (_DoRecoMuon1DiscrBySoftID == "1") {if (Muon_soft->at(nobj) == 0) {return false;}}

  //----Acceptance cuts
  if (fabs(smearedMuonMomentumVector.at(nobj).Eta())>_RecoMuon1EtaCut) {return false;}
  if (smearedMuonMomentumVector.at(nobj).Pt()<_RecoMuon1PtMinCut) {return false;}
  if (smearedMuonMomentumVector.at(nobj).Pt()>_RecoMuon1PtMaxCut) {return false;}

  //----Isolation requirement
  if (_DoRecoMuon1DiscrByIsolation == "1") {
    if( ((Muon_isoCharged->at(nobj) + max(0.,Muon_isoNeutralHadron->at(nobj) + Muon_isoPhoton->at(nobj) - (0.5 * Muon_isoPU->at(nobj)))) / smearedMuonMomentumVector.at(nobj).Pt()) >= _RecoMuon1IsoSumPtMaxCutValue  ) {return false;}
    if( ((Muon_isoCharged->at(nobj) + max(0.,Muon_isoNeutralHadron->at(nobj) + Muon_isoPhoton->at(nobj) - (0.5 * Muon_isoPU->at(nobj)))) / smearedMuonMomentumVector.at(nobj).Pt()) <  _RecoMuon1IsoSumPtMinCutValue  ) {return false;}
  }

  return true;
}

// Reco level muon2 requirements
bool BSM3GAnalyzer::passRecoMuon2Cuts(int nobj) {
  // ----Matching to gen
  if((_MatchMuonToGen == "1") && (isData == "0")) {
    if(!(matchMuonToGen(smearedMuonMomentumVector.at(nobj)).first)) {return false;}
  }

  // ----Require muon to pass ID discriminators
  if (_DoRecoMuon2DiscrByTightID == "1") {if (Muon_tight->at(nobj) == 0) {return false;}}
  if (_DoRecoMuon2DiscrBySoftID == "1") {if (Muon_soft->at(nobj) == 0) {return false;}}

  // ----Acceptance cuts
  if (fabs(smearedMuonMomentumVector.at(nobj).Eta())>_RecoMuon2EtaCut) {return false;}
  if (smearedMuonMomentumVector.at(nobj).Pt()<_RecoMuon2PtMinCut) {return false;}
  if (smearedMuonMomentumVector.at(nobj).Pt()>_RecoMuon2PtMaxCut) {return false;}

  // ----Isolation requirement
  if (_DoRecoMuon2DiscrByIsolation == "1") {
    if( ((Muon_isoCharged->at(nobj) + max(0.,Muon_isoNeutralHadron->at(nobj) + Muon_isoPhoton->at(nobj) - (0.5 * Muon_isoPU->at(nobj)))) / smearedMuonMomentumVector.at(nobj).Pt()) >= _RecoMuon2IsoSumPtMaxCutValue  ) {return false;}
    if( ((Muon_isoCharged->at(nobj) + max(0.,Muon_isoNeutralHadron->at(nobj) + Muon_isoPhoton->at(nobj) - (0.5 * Muon_isoPU->at(nobj)))) / smearedMuonMomentumVector.at(nobj).Pt()) <  _RecoMuon2IsoSumPtMinCutValue  ) {return false;}
  }

  return true;
}

TLorentzVector BSM3GAnalyzer::SmearMuon(int index) {
  double smearedPt;
  double smearedEta;
  double smearedPhi;
  double smearedEnergy;
  if(isData == "0") {
    TLorentzVector smearedMomentum;
    smearedMomentum.SetPtEtaPhiE(Muon_pt->at(index), Muon_eta->at(index), Muon_phi->at(index), Muon_energy->at(index));
    if(matchMuonToGen(smearedMomentum).first) {
      TLorentzVector unsmearedMomentum = matchMuonToGen(smearedMomentum).second;
      if(_SmearTheMuon == "1") {
        smearedPt = (unsmearedMomentum.Pt() * _MuonPtScaleOffset) + ((smearedMomentum.Pt() -  unsmearedMomentum.Pt()) * _MuonPtSigmaOffset);
        smearedEta = (unsmearedMomentum.Eta() * _MuonEtaScaleOffset) + ((smearedMomentum.Eta() -  unsmearedMomentum.Eta()) * _MuonEtaSigmaOffset);
        smearedPhi = (unsmearedMomentum.Phi() * _MuonPhiScaleOffset) + ((smearedMomentum.Phi() -  unsmearedMomentum.Phi()) * _MuonPhiSigmaOffset);
        smearedEnergy = (unsmearedMomentum.Energy() * _MuonEnergyScaleOffset) + ((smearedMomentum.Energy() -  unsmearedMomentum.Energy()) * _MuonEnergySigmaOffset);
      } else {
        smearedPt  = smearedMomentum.Pt();
        smearedEta  = smearedMomentum.Eta();
        smearedPhi  = smearedMomentum.Phi();
        smearedEnergy  = smearedMomentum.Energy();
      }
      smearedMomentum.SetPtEtaPhiE(smearedPt, smearedEta, smearedPhi, smearedEnergy);
      return smearedMomentum;
    } else {
      return smearedMomentum;
    }
  } else {
    TLorentzVector smearedMomentum;
    smearedMomentum.SetPtEtaPhiE(Muon_pt->at(index), Muon_eta->at(index), Muon_phi->at(index), Muon_energy->at(index));
    return smearedMomentum;
  }
}

//---Matching Muons to generator level objects
pair<bool, TLorentzVector> BSM3GAnalyzer::matchMuonToGen(TLorentzVector p4Vector) {
  bool isGenMatched = false;
  TLorentzVector theGenObject(0,0,0,0);
  for(int jj = 0; jj < Gen_pt->size(); jj++) {
    if((abs(Gen_pdg_id->at(jj)) == 13) && (Gen_status->at(jj) == 1)) {
      theGenObject.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj));
      if(p4Vector.DeltaR(theGenObject) <= _MuonToGenMatchingDeltaR) {
        if(_UseMuonMotherId == "1") {
          if(abs(Gen_motherpdg_id->at(jj)) == _MuonMotherId) { isGenMatched = true; }
        } else { isGenMatched = true; }
      }
    }
  }
  pair<bool, TLorentzVector> GenMatchedInformation(isGenMatched,theGenObject);
  return GenMatchedInformation;
}

TLorentzVector BSM3GAnalyzer::SmearTau(int index) {
  double smearedPt;
  double smearedEta;
  double smearedPhi;
  double smearedEnergy;
  if(isData == "0") {
    TLorentzVector smearedMomentum;
    smearedMomentum.SetPtEtaPhiE(Tau_pt->at(index), Tau_eta->at(index), Tau_phi->at(index), Tau_energy->at(index));
    if(matchTauToGen(smearedMomentum).first) {
      TLorentzVector unsmearedMomentum = matchTauToGen(smearedMomentum).second;
      if(_SmearTheTau == "1") {
        smearedPt = (unsmearedMomentum.Pt() * _TauPtScaleOffset) + ((smearedMomentum.Pt() -  unsmearedMomentum.Pt()) * _TauPtSigmaOffset);
        smearedEta = (unsmearedMomentum.Eta() * _TauEtaScaleOffset) + ((smearedMomentum.Eta() -  unsmearedMomentum.Eta()) * _TauEtaSigmaOffset);
        smearedPhi = (unsmearedMomentum.Phi() * _TauPhiScaleOffset) + ((smearedMomentum.Phi() -  unsmearedMomentum.Phi()) * _TauPhiSigmaOffset);
        smearedEnergy = (unsmearedMomentum.Energy() * _TauEnergyScaleOffset) + ((smearedMomentum.Energy() -  unsmearedMomentum.Energy()) * _TauEnergySigmaOffset);
      } else {
        smearedPt  = smearedMomentum.Pt();
        smearedEta  = smearedMomentum.Eta();
        smearedPhi  = smearedMomentum.Phi();
        smearedEnergy  = smearedMomentum.Energy();
      }
      smearedMomentum.SetPtEtaPhiE(smearedPt, smearedEta, smearedPhi, smearedEnergy);
      return smearedMomentum;
    } else {
      return smearedMomentum;
    }
  } else {
    TLorentzVector smearedMomentum;
    smearedMomentum.SetPtEtaPhiE(Tau_pt->at(index), Tau_eta->at(index), Tau_phi->at(index), Tau_energy->at(index));
    return smearedMomentum;
  }
}

//---Matching Muons to generator level objects
pair<bool, TLorentzVector> BSM3GAnalyzer::matchTauToGen(TLorentzVector p4Vector) {
  bool isGenMatched = false; // by default, the reco-gen tau matching boolean is set to false
  bool IsItAHadronicDecay; // boolean used to specify whether a gen tau lepton decays hadronically
  TLorentzVector theGenObject(0,0,0,0); // initialize the 4-momentum vector of the vis gen tau matched to the reco tau
  TLorentzVector theNeutrinoObject(0,0,0,0); // initialize the 4-momentum vector of the tau neutrino from the tau decay
  vector<bool> IsItAHadronicDecayVector; // vector of booleans which contain the information about whether a gen tau lepton decays hadronically
  vector<int> tempTauIndexVector; // vector which contains the index (in the gen collection) of the gen level taus (before decaying)
  vector<TLorentzVector> tempNeutrinoMomentumVector; // vector of lorentz 4-momentum vectors for each tau neutrino from the tau decay
  tempTauIndexVector.clear(); // clear any previous declaration from memory
  tempNeutrinoMomentumVector.clear(); // clear any previous declaration from memory

  //---Loop over gen particles to find the tau neutrinos and then store the index of each tau neutrino's mother (a tau).
  //---Also store the tau neutrino's 4-momentum vector in order to calculate the visible tau 4-momentum at a later point.
  for(int jj = 0; jj < Gen_pt->size(); jj++) {
    if(abs(Gen_pdg_id->at(jj)) == 16) {
      tempTauIndexVector.push_back(Gen_BmotherIndices->at(jj));
      theNeutrinoObject.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj));
      tempNeutrinoMomentumVector.push_back(theNeutrinoObject);
    }
  }

  //---Perform matching only if their is at least one gen tau in the event
  if(tempTauIndexVector.size() > 0) {
    //---Loop over the gen taus and determine whether it decays hadronically.
    for(int jjj = 0; jjj < tempTauIndexVector.size(); jjj++) {
      IsItAHadronicDecay = true;
      for(int jj = 0; jj < Gen_pt->size(); jj++) {
        if( ((abs(Gen_pdg_id->at(jj)) == 12) || (abs(Gen_pdg_id->at(jj)) == 14)) && (Gen_BmotherIndices->at(jj) == tempTauIndexVector.at(jjj)) ) {
          IsItAHadronicDecay = false; // it is not a hadronic tau decay since it decayed to a electron/muon neutrino
        }
      }
      IsItAHadronicDecayVector.push_back(IsItAHadronicDecay);
    }

    //---Loop over the gen taus and calculate the 4-momentum of the visible products (i.e. subtract the 4-momentum of the tau neutrino)
    for(int jjj = 0; jjj < tempTauIndexVector.size(); jjj++) {
      for(int jj = 0; jj < Gen_pt->size(); jj++) {
        if(jj == tempTauIndexVector.at(jjj)) {
          theGenObject.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj)); // 4-momentum of the gen tau
          theGenObject = theGenObject - tempNeutrinoMomentumVector.at(jjj); // subtract the 4-momentum of the tau neutrino (visible tau)
          if( (IsItAHadronicDecayVector.at(jjj)) && (p4Vector.DeltaR(theGenObject) <= _TauToGenMatchingDeltaR) ) {isGenMatched = true;}
        }
      }
    }
    pair<bool, TLorentzVector> GenMatchedInformation(isGenMatched,theGenObject);
    return GenMatchedInformation;
  } else {
    pair<bool, TLorentzVector> GenMatchedInformation(isGenMatched,theGenObject);
    return GenMatchedInformation;
  }
}

int BSM3GAnalyzer::getBin(char * cstr, int nVertices) {
  TFile *file1 = new TFile (cstr);
  TH1 *hist = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!hist) {throw std::runtime_error("failed to extract histogram");}
    
  int result = hist->GetBin(nVertices+1);
  file1->Close();
  return result;
}

double BSM3GAnalyzer::getvalue(char * cstr, int bin) {
  TFile *file1 = new TFile (cstr);
  TH1 *hist = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!hist) {throw std::runtime_error("failed to extract histogram");}
  
  double result = hist->GetBinContent(bin);
  file1->Close();
  return result;
}

double BSM3GAnalyzer::getintegral(char * cstr, int bin) {
  TFile *file1 = new TFile (cstr);
  TH1 *hist = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!hist) {throw std::runtime_error("failed to extract histogram");}
  
  double result = hist->Integral();
  file1->Close();
  return result;
}

BSM3GAnalyzer::~BSM3GAnalyzer() { }

void BSM3GAnalyzer::setBranchAddress(TTree* BOOM) {

  //---set object pointer
  Muon_pt = 0;
  Muon_eta = 0;
  Muon_phi = 0;
  Muon_energy = 0;
  Muon_tight = 0;
  Muon_soft = 0;
  Muon_isoCharged = 0;
  Muon_isoNeutralHadron = 0;
  Muon_isoPhoton = 0;
  Muon_isoPU = 0;
  Tau_eta = 0;
  Tau_phi = 0;
  Tau_pt = 0;
  Tau_energy = 0;
  Gen_eta = 0;
  Gen_phi = 0;
  Gen_pt = 0;
  Gen_energy = 0;
  Gen_pdg_id = 0;
  Gen_motherpdg_id = 0;
  Gen_status = 0;
  Gen_BmotherIndices = 0;
  trueInteractions = 0;
  bestVertices = 0;

  //---Set branch addresses and branch pointers
  if(!BOOM) return;
  BOOM->SetBranchAddress("Gen_pt", &Gen_pt, &b_Gen_pt);
  BOOM->SetBranchAddress("Gen_eta", &Gen_eta, &b_Gen_eta);
  BOOM->SetBranchAddress("Gen_phi", &Gen_phi, &b_Gen_phi);
  BOOM->SetBranchAddress("Gen_energy", &Gen_energy, &b_Gen_energy);
  BOOM->SetBranchAddress("Gen_pdg_id", &Gen_pdg_id, &b_Gen_pdg_id);
  BOOM->SetBranchAddress("Gen_motherpdg_id", &Gen_motherpdg_id, &b_Gen_motherpdg_id);
  BOOM->SetBranchAddress("Gen_status", &Gen_status, &b_Gen_status);
  BOOM->SetBranchAddress("Gen_BmotherIndices", &Gen_BmotherIndices, &b_Gen_BmotherIndices);
  BOOM->SetBranchAddress("trueInteractions", &trueInteractions, &b_trueInteractions);
  BOOM->SetBranchAddress("bestVertices", &bestVertices, &b_bestVertices);
  BOOM->SetBranchAddress("Muon_pt", &Muon_pt, &b_Muon_pt);
  BOOM->SetBranchAddress("Muon_eta", &Muon_eta, &b_Muon_eta);
  BOOM->SetBranchAddress("Muon_phi", &Muon_phi, &b_Muon_phi);
  BOOM->SetBranchAddress("Muon_energy", &Muon_energy, &b_Muon_energy);
  BOOM->SetBranchAddress("Muon_tight", &Muon_tight, &b_Muon_tight);
  BOOM->SetBranchAddress("Muon_soft", &Muon_soft, &b_Muon_soft);
  BOOM->SetBranchAddress("Muon_isoCharged", &Muon_isoCharged, &b_Muon_isoCharged);
  BOOM->SetBranchAddress("Muon_isoNeutralHadron", &Muon_isoNeutralHadron, &b_Muon_isoNeutralHadron);
  BOOM->SetBranchAddress("Muon_isoPhoton", &Muon_isoPhoton, &b_Muon_isoPhoton);
  BOOM->SetBranchAddress("Muon_isoPU", &Muon_isoPU, &b_Muon_isoPU);
  BOOM->SetBranchAddress("Tau_eta", &Tau_eta, &b_Tau_eta);
  BOOM->SetBranchAddress("Tau_phi", &Tau_phi, &b_Tau_phi);
  BOOM->SetBranchAddress("Tau_pt", &Tau_pt, &b_Tau_pt);
  BOOM->SetBranchAddress("Tau_energy", &Tau_energy, &b_Tau_energy);
  BOOM->SetBranchAddress("Met_px", &Met_px, &b_Met_px);
  BOOM->SetBranchAddress("Met_py", &Met_py, &b_Met_py);
  BOOM->SetBranchAddress("Met_pz", &Met_pz, &b_Met_pz);

};
