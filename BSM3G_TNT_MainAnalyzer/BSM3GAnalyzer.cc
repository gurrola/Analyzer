////////////////////////////////////////////////////////////////////////////// 
// Authors:	Alfredo Gurrola, Andres Florez                              //
// contact:                                                                 //
//   Alfredo.Gurrola@cern.ch       (Vanderbilt University)                  // 
//   Andres.Florez@cern.ch         (Los Andes University)                   //
//////////////////////////////////////////////////////////////////////////////

#include "BSM3GAnalyzer.h"

//---main function
int main (int argc, char *argv[]) {

  //TApplication app("App",&argc, argv);
  TFile * fs = new TFile(argv[2], "RECREATE");
  BSM3GAnalyzer BSM3GAnalyzer_(fs, argv[1]);

}

BSM3GAnalyzer::BSM3GAnalyzer(TFile* theFile, char* fname) {

  //---obtain the configurable inputs
  getInputs();
  //std::cout << "Finished grabbing configurable inputs" << std::endl;

  //---open input root file and read in the tree information
//  TFile *f = new TFile ("OutTree_9_1_dab.root");
  TFile *f = new TFile (fname);
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
    } else if(inputType == "RecoElectron1Nmin") {
      _RecoElectron1Nmin = (int)(atof(inputString.c_str())); // minimum number of reco Electron1 objects passing cuts
    } else if(inputType == "RecoElectron1Nmax") {
      _RecoElectron1Nmax = (int)(atof(inputString.c_str())); // maximum number of reco Electron1 objects passing cuts
    } else if(inputType == "RecoElectron2Nmin") {
      _RecoElectron2Nmin = (int)(atof(inputString.c_str())); // minimum number of reco Electron2 objects passing cuts
    } else if(inputType == "RecoElectron2Nmax") {
      _RecoElectron2Nmax = (int)(atof(inputString.c_str())); // maximum number of reco Electron2 objects passing cuts
    } else if(inputType == "RecoTau1Nmin") {
      _RecoTau1Nmin = (int)(atof(inputString.c_str())); // minimum number of reco tau1 objects passing cuts
    } else if(inputType == "RecoTau1Nmax") {
      _RecoTau1Nmax = (int)(atof(inputString.c_str())); // maximum number of reco tau1 objects passing cuts
    } else if(inputType == "RecoTau2Nmin") {
      _RecoTau2Nmin = (int)(atof(inputString.c_str())); // minimum number of reco tau2 objects passing cuts
    } else if(inputType == "RecoTau2Nmax") {
      _RecoTau2Nmax = (int)(atof(inputString.c_str())); // maximum number of reco tau2 objects passing cuts
    } else if(inputType == "RecoJet1Nmin") {
      _RecoJet1Nmin = (int)(atof(inputString.c_str())); // minimum number of reco jet1 objects passing cuts
    } else if(inputType == "RecoJet1Nmax") {
      _RecoJet1Nmax = (int)(atof(inputString.c_str())); // maximum number of reco jet1 objects passing cuts
    } else if(inputType == "RecoJet2Nmin") {
      _RecoJet2Nmin = (int)(atof(inputString.c_str())); // minimum number of reco jet2 objects passing cuts
    } else if(inputType == "RecoJet2Nmax") {
      _RecoJet2Nmax = (int)(atof(inputString.c_str())); // maximum number of reco jet2 objects passing cuts
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
    } else if(inputType == "RecoElectron1EtaCut") {
      _RecoElectron1EtaCut = atof(inputString.c_str()); // maximum reco Electron1 eta requirement
    } else if(inputType == "RecoElectron1PtMinCut") {
      _RecoElectron1PtMinCut = atof(inputString.c_str()); // minimum reco Electron1 pt requirement
    } else if(inputType == "RecoElectron1PtMaxCut") {
      _RecoElectron1PtMaxCut = atof(inputString.c_str()); // maximum reco Electron1 pt requirement
    } else if(inputType == "DoRecoElectron1DiscrByIsolation") {
      _DoRecoElectron1DiscrByIsolation = inputString; // "1" to require the reco electron1 to pass isolation; "0" to disable the isolation cut
    } else if(inputType == "RecoElectron1IsoSumPtMaxCutValue") {
      _RecoElectron1IsoSumPtMaxCutValue = atof(inputString.c_str()); // maximum reco Electron1 iso requirement
    } else if(inputType == "RecoElectron1IsoSumPtMinCutValue") {
      _RecoElectron1IsoSumPtMinCutValue = atof(inputString.c_str()); // minimum reco Electron1 iso requirement
    } else if(inputType == "DoRecoElectron1DiscrByVetoID") {
      _DoRecoElectron1DiscrByVetoID = inputString; // "1" to require the reco electron1 to pass "veto" ID; "0" to disable the "veto" ID cut
    } else if(inputType == "DoRecoElectron1DiscrByLooseID") {
      _DoRecoElectron1DiscrByLooseID = inputString; // "1" to require the reco electron1 to pass "loose" ID; "0" to disable the "loose" ID cut
    } else if(inputType == "DoRecoElectron1DiscrByMediumID") {
      _DoRecoElectron1DiscrByMediumID = inputString; // "1" to require the reco electron1 to pass "medium" ID; "0" to disable the "medium" ID cut
    } else if(inputType == "DoRecoElectron1DiscrByTightID") {
      _DoRecoElectron1DiscrByTightID = inputString; // "1" to require the reco electron1 to pass "tight" ID; "0" to disable the "tight" ID cut
    } else if(inputType == "DoRecoElectron1DiscrByHEEPID") {
      _DoRecoElectron1DiscrByHEEPID = inputString; // "1" to require the reco electron1 to pass "HEEP" ID; "0" to disable the "HEEP" ID cut
    } else if(inputType == "RecoElectron2EtaCut") {
      _RecoElectron2EtaCut = atof(inputString.c_str()); // maximum reco Electron2 eta requirement
    } else if(inputType == "RecoElectron2PtMinCut") {
      _RecoElectron2PtMinCut = atof(inputString.c_str()); // minimum reco Electron2 pt requirement
    } else if(inputType == "RecoElectron2PtMaxCut") {
      _RecoElectron2PtMaxCut = atof(inputString.c_str()); // maximum reco Electron2 pt requirement
    } else if(inputType == "DoRecoElectron2DiscrByIsolation") {
      _DoRecoElectron2DiscrByIsolation = inputString; // "1" to require the reco electron2 to pass isolation; "0" to disable the isolation cut
    } else if(inputType == "RecoElectron2IsoSumPtMaxCutValue") {
      _RecoElectron2IsoSumPtMaxCutValue = atof(inputString.c_str()); // maximum reco Electron2 iso requirement
    } else if(inputType == "RecoElectron2IsoSumPtMinCutValue") {
      _RecoElectron2IsoSumPtMinCutValue = atof(inputString.c_str()); // minimum reco Electron2 iso requirement
    } else if(inputType == "DoRecoElectron2DiscrByVetoID") {
      _DoRecoElectron2DiscrByVetoID = inputString; // "1" to require the reco electron2 to pass "veto" ID; "0" to disable the "veto" ID cut
    } else if(inputType == "DoRecoElectron2DiscrByLooseID") {
      _DoRecoElectron2DiscrByLooseID = inputString; // "1" to require the reco electron2 to pass "loose" ID; "0" to disable the "loose" ID cut
    } else if(inputType == "DoRecoElectron2DiscrByMediumID") {
      _DoRecoElectron2DiscrByMediumID = inputString; // "1" to require the reco electron2 to pass "medium" ID; "0" to disable the "medium" ID cut
    } else if(inputType == "DoRecoElectron2DiscrByTightID") {
      _DoRecoElectron2DiscrByTightID = inputString; // "1" to require the reco electron2 to pass "tight" ID; "0" to disable the "tight" ID cut
    } else if(inputType == "DoRecoElectron2DiscrByHEEPID") {
      _DoRecoElectron2DiscrByHEEPID = inputString; // "1" to require the reco electron2 to pass "HEEP" ID; "0" to disable the "HEEP" ID cut
    } else if(inputType == "RecoTau1EtaCut") {
      _RecoTau1EtaCut = atof(inputString.c_str()); // maximum reco tau1 eta requirement
    } else if(inputType == "RecoTau1PtMinCut") {
      _RecoTau1PtMinCut = atof(inputString.c_str()); // minimum reco tau1 pt requirement
    } else if(inputType == "RecoTau1PtMaxCut") {
      _RecoTau1PtMaxCut = atof(inputString.c_str()); // maximum reco tau1 pt requirement
    } else if(inputType == "DoRecoTau1DiscrByLeadTrack") {
      _DoRecoTau1DiscrByLeadTrack = inputString; // "1" to require the reco tau1 to pass a lead track pt requirement; "0" to disable it
    } else if(inputType == "RecoTau1LeadTrackThreshold") {
      _RecoTau1LeadTrackThreshold = atof(inputString.c_str()); // minimum reco tau1 lead track pt requirement
    } else if(inputType == "DoRecoTau1DiscrByIsolation") {
      _DoRecoTau1DiscrByIsolation = inputString; // "1" to require the reco tau1 to pass isolation; "0" to disable it
    } else if(inputType == "RecoTau1DiscrByMaxIsolation") {
      _RecoTau1DiscrByMaxIsolation = inputString; // maximum reco tau1 isolation requirement
    } else if(inputType == "RecoTau1DiscrByMinIsolation") {
      _RecoTau1DiscrByMinIsolation = inputString; // minimum reco tau1 isolation requirement
    } else if(inputType == "RecoTau1DiscrByProngType") {
      _RecoTau1DiscrByProngType = inputString; // "1" 
    } else if(inputType == "DoRecoTau1DiscrAgainstElectron") {
      _DoRecoTau1DiscrAgainstElectron = inputString; // "1" to require the reco tau1 to pass the e-veto; "0" to disable it
    } else if(inputType == "RecoTau1DiscrAgainstElectron") {
      _RecoTau1DiscrAgainstElectron = inputString; // name of the e-veto discriminator
    } else if(inputType == "SelectTau1sThatAreElectrons") {
      _SelectTau1sThatAreElectrons = inputString; // "1" to require the reco tau1 to pass the inverted e-veto (i.e. select "e-like"); "0" to disable it
    } else if(inputType == "DoRecoTau1DiscrAgainstMuon") {
      _DoRecoTau1DiscrAgainstMuon = inputString; // "1" to require the reco tau1 to pass the muon-veto; "0" to disable it
    } else if(inputType == "RecoTau1DiscrAgainstMuon") {
      _RecoTau1DiscrAgainstMuon = inputString; // name of the muon-veto discriminator
    } else if(inputType == "SelectTau1sThatAreMuons") {
      _SelectTau1sThatAreMuons = inputString; // "1" to require the reco tau1 to pass the inverted muon-veto (i.e. select "muon-like"); "0" to disable it
    } else if(inputType == "DoRecoTau1DiscrByCrackCut") {
      _DoRecoTau1DiscrByCrackCut = inputString; // "1" to require the reco tau1 to pass the detector crack cut; "0" to disable it
    } else if(inputType == "RemoveTau1OverlapWithMuon1s") {
      _RemoveTau1OverlapWithMuon1s = inputString; // "1" to require the reco tau1 to NOT overlap a "good muon1"; "0" to disable this cut
    } else if(inputType == "RemoveTau1OverlapWithMuon2s") {
      _RemoveTau1OverlapWithMuon2s = inputString; // "1" to require the reco tau1 to NOT overlap a "good muon2"; "0" to disable this cut
    } else if(inputType == "Tau1Muon1MatchingDeltaR") {
      _Tau1Muon1MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau1-muon1 overlap removal cut
    } else if(inputType == "Tau1Muon2MatchingDeltaR") {
      _Tau1Muon2MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau1-muon2 overlap removal cut
    } else if(inputType == "RemoveTau1OverlapWithElectron1s") {
      _RemoveTau1OverlapWithElectron1s = inputString; // "1" to require the reco tau1 to NOT overlap a "good Electron1"; "0" to disable this cut
    } else if(inputType == "RemoveTau1OverlapWithElectron2s") {
      _RemoveTau1OverlapWithElectron2s = inputString; // "1" to require the reco tau1 to NOT overlap a "good Electron2"; "0" to disable this cut
    } else if(inputType == "Tau1Electron1MatchingDeltaR") {
      _Tau1Electron1MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau1-Electron1 overlap removal cut
    } else if(inputType == "Tau1Electron2MatchingDeltaR") {
      _Tau1Electron2MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau1-Electron2 overlap removal cut
    } else if(inputType == "RecoTau2EtaCut") {
      _RecoTau2EtaCut = atof(inputString.c_str()); // maximum reco tau2 eta requirement
    } else if(inputType == "RecoTau2PtMinCut") {
      _RecoTau2PtMinCut = atof(inputString.c_str()); // minimum reco tau2 pt requirement
    } else if(inputType == "RecoTau2PtMaxCut") {
      _RecoTau2PtMaxCut = atof(inputString.c_str()); // maximum reco Tau2 pt requirement
    } else if(inputType == "DoRecoTau2DiscrByLeadTrack") {
      _DoRecoTau2DiscrByLeadTrack = inputString; // "1" to require the reco Tau2 to pass a lead track pt requirement; "0" to disable it
    } else if(inputType == "RecoTau2LeadTrackThreshold") {
      _RecoTau2LeadTrackThreshold = atof(inputString.c_str()); // minimum reco Tau2 lead track pt requirement
    } else if(inputType == "DoRecoTau2DiscrByIsolation") {
      _DoRecoTau2DiscrByIsolation = inputString; // "1" to require the reco Tau2 to pass isolation; "0" to disable it
    } else if(inputType == "RecoTau2DiscrByMaxIsolation") {
      _RecoTau2DiscrByMaxIsolation = inputString; // maximum reco Tau2 isolation requirement
    } else if(inputType == "RecoTau2DiscrByMinIsolation") {
      _RecoTau2DiscrByMinIsolation = inputString; // minimum reco Tau2 isolation requirement
    } else if(inputType == "RecoTau2DiscrByProngType") {
      _RecoTau2DiscrByProngType = inputString; // "1"
    } else if(inputType == "DoRecoTau2DiscrAgainstElectron") {
      _DoRecoTau2DiscrAgainstElectron = inputString; // "1" to require the reco Tau2 to pass the e-veto; "0" to disable it
    } else if(inputType == "RecoTau2DiscrAgainstElectron") {
      _RecoTau2DiscrAgainstElectron = inputString; // name of the e-veto discriminator
    } else if(inputType == "SelectTau2sThatAreElectrons") {
      _SelectTau2sThatAreElectrons = inputString; // "1" to require the reco Tau2 to pass the inverted e-veto (i.e. select "e-like"); "0" to disable it
    } else if(inputType == "DoRecoTau2DiscrAgainstMuon") {
      _DoRecoTau2DiscrAgainstMuon = inputString; // "1" to require the reco Tau2 to pass the muon-veto; "0" to disable it
    } else if(inputType == "RecoTau2DiscrAgainstMuon") {
      _RecoTau2DiscrAgainstMuon = inputString; // name of the muon-veto discriminator
    } else if(inputType == "SelectTau2sThatAreMuons") {
      _SelectTau2sThatAreMuons = inputString; // "1" to require the reco Tau2 to pass the inverted muon-veto (i.e. select "muon-like"); "0" to disable it
    } else if(inputType == "DoRecoTau2DiscrByCrackCut") {
      _DoRecoTau2DiscrByCrackCut = inputString; // "1" to require the reco Tau2 to pass the detector crack cut; "0" to disable it
    } else if(inputType == "RemoveTau2OverlapWithMuon1s") {
      _RemoveTau2OverlapWithMuon1s = inputString; // "1" to require the reco Tau2 to NOT overlap a "good muon1"; "0" to disable this cut
    } else if(inputType == "RemoveTau2OverlapWithMuon2s") {
      _RemoveTau2OverlapWithMuon2s = inputString; // "1" to require the reco Tau2 to NOT overlap a "good muon2"; "0" to disable this cut
    } else if(inputType == "Tau2Muon1MatchingDeltaR") {
      _Tau2Muon1MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the Tau2-muon1 overlap removal cut
    } else if(inputType == "Tau2Muon2MatchingDeltaR") {
      _Tau2Muon2MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the Tau2-muon2 overlap removal cut
    } else if(inputType == "RemoveTau2OverlapWithElectron1s") {
      _RemoveTau2OverlapWithElectron1s = inputString; // "1" to require the reco tau2 to NOT overlap a "good Electron1"; "0" to disable this cut
    } else if(inputType == "RemoveTau2OverlapWithElectron2s") {
      _RemoveTau2OverlapWithElectron2s = inputString; // "1" to require the reco tau2 to NOT overlap a "good Electron2"; "0" to disable this cut
    } else if(inputType == "Tau2Electron1MatchingDeltaR") {
      _Tau2Electron1MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau2-Electron1 overlap removal cut
    } else if(inputType == "Tau2Electron2MatchingDeltaR") {
      _Tau2Electron2MatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used for the tau2-Electron2 overlap removal cut
    } else if(inputType == "RecoJet1EtaMaxCut") {
      _RecoJet1EtaMaxCut = atof(inputString.c_str()); // maximum reco jet1 eta requirement
    } else if(inputType == "RecoJet1EtaMinCut") {
      _RecoJet1EtaMinCut = atof(inputString.c_str()); // minimum reco jet1 eta requirement
    } else if(inputType == "RecoJet1PtCut") {
      _RecoJet1PtCut = atof(inputString.c_str()); // minimum reco jet1 pt requirement
    } else if(inputType == "ApplyJet1LooseID") {
      _ApplyJet1LooseID = inputString; // "1" to require the reco jet1 to pass "loose" jet ID; "0" to disable it
    } else if(inputType == "RemoveJet1OverlapWithMuon1s") {
      _RemoveJet1OverlapWithMuon1s = inputString; // "1" to require the reco jet1 to NOT overlap with a muon1; "0" to disable it
    } else if(inputType == "Jet1Muon1MatchingDeltaR") {
      _Jet1Muon1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-muon1 overlap removal cut
    } else if(inputType == "RemoveJet1OverlapWithMuon2s") {
      _RemoveJet1OverlapWithMuon2s = inputString; // "1" to require the reco jet1 to NOT overlap with a muon2; "0" to disable it
    } else if(inputType == "Jet1Muon2MatchingDeltaR") {
      _Jet1Muon2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-muon2 overlap removal cut
    } else if(inputType == "RemoveJet1OverlapWithElectron1s") {
      _RemoveJet1OverlapWithElectron1s = inputString; // "1" to require the reco jet1 to NOT overlap with a Electron1; "0" to disable it
    } else if(inputType == "Jet1Electron1MatchingDeltaR") {
      _Jet1Electron1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-Electron1 overlap removal cut
    } else if(inputType == "RemoveJet1OverlapWithElectron2s") {
      _RemoveJet1OverlapWithElectron2s = inputString; // "1" to require the reco jet1 to NOT overlap with a Electron2; "0" to disable it
    } else if(inputType == "Jet1Electron2MatchingDeltaR") {
      _Jet1Electron2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-Electron2 overlap removal cut
    } else if(inputType == "RemoveJet1OverlapWithTau1s") {
      _RemoveJet1OverlapWithTau1s = inputString; // "1" to require the reco jet1 to NOT overlap with a Tau1; "0" to disable it
    } else if(inputType == "Jet1Tau1MatchingDeltaR") {
      _Jet1Tau1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-Tau1 overlap removal cut
    } else if(inputType == "RemoveJet1OverlapWithTau2s") {
      _RemoveJet1OverlapWithTau2s = inputString; // "1" to require the reco jet1 to NOT overlap with a Tau2; "0" to disable it
    } else if(inputType == "Jet1Tau2MatchingDeltaR") {
      _Jet1Tau2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet1-Tau2 overlap removal cut
    } else if(inputType == "RecoJet2EtaMaxCut") {
      _RecoJet2EtaMaxCut = atof(inputString.c_str()); // maximum reco jet2 eta requirement
    } else if(inputType == "RecoJet2EtaMinCut") {
      _RecoJet2EtaMinCut = atof(inputString.c_str()); // minimum reco jet2 eta requirement
    } else if(inputType == "RecoJet2PtCut") {
      _RecoJet2PtCut = atof(inputString.c_str()); // minimum reco jet2 pt requirement
    } else if(inputType == "ApplyJet2LooseID") {
      _ApplyJet2LooseID = inputString; // "1" to require the reco jet2 to pass "loose" jet ID; "0" to disable it
    } else if(inputType == "RemoveJet2OverlapWithMuon1s") {
      _RemoveJet2OverlapWithMuon1s = inputString; // "1" to require the reco jet2 to NOT overlap with a muon1; "0" to disable it
    } else if(inputType == "Jet2Muon1MatchingDeltaR") {
      _Jet2Muon1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-muon1 overlap removal cut
    } else if(inputType == "RemoveJet2OverlapWithMuon2s") {
      _RemoveJet2OverlapWithMuon2s = inputString; // "1" to require the reco jet2 to NOT overlap with a muon2; "0" to disable it
    } else if(inputType == "Jet2Muon2MatchingDeltaR") {
      _Jet2Muon2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-muon2 overlap removal cut
    } else if(inputType == "RemoveJet2OverlapWithElectron1s") {
      _RemoveJet2OverlapWithElectron1s = inputString; // "1" to require the reco jet2 to NOT overlap with a Electron1; "0" to disable it
    } else if(inputType == "Jet2Electron1MatchingDeltaR") {
      _Jet2Electron1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-Electron1 overlap removal cut
    } else if(inputType == "RemoveJet2OverlapWithElectron2s") {
      _RemoveJet2OverlapWithElectron2s = inputString; // "1" to require the reco jet2 to NOT overlap with a Electron2; "0" to disable it
    } else if(inputType == "Jet2Electron2MatchingDeltaR") {
      _Jet2Electron2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-Electron2 overlap removal cut
    } else if(inputType == "RemoveJet2OverlapWithTau1s") {
      _RemoveJet2OverlapWithTau1s = inputString; // "1" to require the reco jet2 to NOT overlap with a Tau1; "0" to disable it
    } else if(inputType == "Jet2Tau1MatchingDeltaR") {
      _Jet2Tau1MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-Tau1 overlap removal cut
    } else if(inputType == "RemoveJet2OverlapWithTau2s") {
      _RemoveJet2OverlapWithTau2s = inputString; // "1" to require the reco jet2 to NOT overlap with a Tau2; "0" to disable it
    } else if(inputType == "Jet2Tau2MatchingDeltaR") {
      _Jet2Tau2MatchingDeltaR = atof(inputString.c_str()); // deltaR used for the jet2-Tau2 overlap removal cut
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
    } else if(inputType == "MatchTauToGen") {
      _MatchTauToGen = inputString; // "1" to match the reco had tau to a gen had tau; "0" to disable had tau matching
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
    } else if(inputType == "SmearTheElectron") {
      _SmearTheElectron = inputString; // "1" to smear the electron momentum; "0" to disable electron momentum smearing
    } else if(inputType == "MatchElectronToGen") {
      _MatchElectronToGen = inputString; // "1" to match the reco electron to a gen electron; "0" to disable muon matching
    } else if(inputType == "UseElectronMotherId") {
      _UseElectronMotherId = inputString; // "1" to use mother information (pdgID) for Electron gen matching; "0" to disable it
    } else if(inputType == "ElectronMotherId") {
      _ElectronMotherId = (int)(atof(inputString.c_str())); // Electron mother pdg id
    } else if(inputType == "ElectronToGenMatchingDeltaR") {
      _ElectronToGenMatchingDeltaR = (double)(atof(inputString.c_str())); // deltaR used to match reco Electrons to gen Electrons
    } else if(inputType == "ElectronPtScaleOffset") {
      _ElectronPtScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the Electron pt scale
    } else if(inputType == "ElectronPtSigmaOffset") {
      _ElectronPtSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the Electron pt resolution
    } else if(inputType == "ElectronEtaScaleOffset") {
      _ElectronEtaScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the Electron eta scale
    } else if(inputType == "ElectronEtaSigmaOffset") {
      _ElectronEtaSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the Electron eta resolution
    } else if(inputType == "ElectronPhiScaleOffset") {
      _ElectronPhiScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the Electron phi scale
    } else if(inputType == "ElectronPhiSigmaOffset") {
      _ElectronPhiSigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the Electron phi resolution
    } else if(inputType == "ElectronEnergyScaleOffset") {
      _ElectronEnergyScaleOffset = (double)(atof(inputString.c_str())); // factor used to shift the Electron energy scale
    } else if(inputType == "ElectronEnergySigmaOffset") {
      _ElectronEnergySigmaOffset = (double)(atof(inputString.c_str())); // factor used to broaden the Electron energy resolution
    } else if(inputType == "SmearTheJet") {
      _SmearTheJet = inputString; // "1" to smear the jet momentum; "0" to disable jet momentum smearing
    } else if(inputType == "CentralJetMuon1MatchingDeltaR") {
      _CentralJetMuon1MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with muon1s
    } else if(inputType == "CentralJetMuon2MatchingDeltaR") {
      _CentralJetMuon2MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with muon2s
    } else if(inputType == "CentralJetElectron1MatchingDeltaR") {
      _CentralJetElectron1MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with electron1s
    } else if(inputType == "CentralJetElectron2MatchingDeltaR") {
      _CentralJetElectron2MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with electron2s
    } else if(inputType == "CentralJetTau1MatchingDeltaR") {
      _CentralJetTau1MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with tau1s
    } else if(inputType == "CentralJetTau2MatchingDeltaR") {
      _CentralJetTau2MatchingDeltaR = atof(inputString.c_str()); // deltaR used to remove central jet overlaps with tau2s
    } else if(inputType == "JetEnergyScaleOffset") {
      _JetEnergyScaleOffset = atof(inputString.c_str()); // k-factor used to smear the jet 4-momentum: smeared = k * default
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
  smearedElectronMomentumVector.clear();
  if(_SmearTheElectron == "1") {
    for(int j = 0; j < patElectron_pt->size(); j++) {
      TLorentzVector unsmearedElectronMomentum;
      unsmearedElectronMomentum.SetPtEtaPhiE(patElectron_pt->at(j), patElectron_eta->at(j), patElectron_phi->at(j), patElectron_energy->at(j));
      smearedElectronMomentumVector.push_back(SmearElectron(j));
      deltaForMEx = deltaForMEx + unsmearedElectronMomentum.Px() - SmearElectron(j).Px();
      deltaForMEy = deltaForMEy + unsmearedElectronMomentum.Py() - SmearElectron(j).Py();
    }
  } else {
    for(int j = 0; j < patElectron_pt->size(); j++) {
      TLorentzVector unsmearedElectronMomentum;
      unsmearedElectronMomentum.SetPtEtaPhiE(patElectron_pt->at(j), patElectron_eta->at(j), patElectron_phi->at(j), patElectron_energy->at(j));
      smearedElectronMomentumVector.push_back(unsmearedElectronMomentum);
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
  smearedJetMomentumVector.clear();
  if(_SmearTheJet == "1") {
    for(int j = 0; j < Jet_pt->size(); j++) {
      TLorentzVector unsmearedJetMomentum;
      unsmearedJetMomentum.SetPtEtaPhiE(Jet_pt->at(j), Jet_eta->at(j), Jet_phi->at(j), Jet_energy->at(j));
      smearedJetMomentumVector.push_back(SmearJet(j));
      deltaForMEx = deltaForMEx + unsmearedJetMomentum.Px() - SmearJet(j).Px();
      deltaForMEy = deltaForMEy + unsmearedJetMomentum.Py() - SmearJet(j).Py();
    }
  } else {
    for(int j = 0; j < Jet_pt->size(); j++) {
      TLorentzVector unsmearedJetMomentum;
      unsmearedJetMomentum.SetPtEtaPhiE(Jet_pt->at(j), Jet_eta->at(j), Jet_phi->at(j), Jet_energy->at(j));
      smearedJetMomentumVector.push_back(unsmearedJetMomentum);
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

  //---Reco level electron1 requirements
  int nGoodCandidatesElectron1 = 0;
  for(int j = 0; j < patElectron_pt->size(); j++) {
    if (!passRecoElectron1Cuts(j)) continue;
    nGoodCandidatesElectron1++;
  }
  if (nGoodCandidatesElectron1>=_RecoElectron1Nmin) _EventFlag[_mapSelectionAlgoID["RecoElectron1Nmin"]] = true;
  if (nGoodCandidatesElectron1<=_RecoElectron1Nmax) _EventFlag[_mapSelectionAlgoID["RecoElectron1Nmax"]] = true;

  //---Reco level electron2 requirements
  int nGoodCandidatesElectron2 = 0;
  for(int j = 0; j < patElectron_pt->size(); j++) {
    if (!passRecoElectron2Cuts(j)) continue;
    nGoodCandidatesElectron2++;
  }
  if (nGoodCandidatesElectron2>=_RecoElectron2Nmin) _EventFlag[_mapSelectionAlgoID["RecoElectron2Nmin"]] = true;
  if (nGoodCandidatesElectron2<=_RecoElectron2Nmax) _EventFlag[_mapSelectionAlgoID["RecoElectron2Nmax"]] = true;

  //---Reco level tau1 requirements
  int nGoodCandidatesTau1 = 0;
  for(int j = 0; j < Tau_pt->size(); j++) {
    if (!passRecoTau1Cuts(j)) continue;
    nGoodCandidatesTau1++;
  }
  if (nGoodCandidatesTau1>=_RecoTau1Nmin) _EventFlag[_mapSelectionAlgoID["RecoTau1Nmin"]] = true;
  if (nGoodCandidatesTau1<=_RecoTau1Nmax) _EventFlag[_mapSelectionAlgoID["RecoTau1Nmax"]] = true;

  //---Reco level tau2 requirements
  int nGoodCandidatesTau2 = 0;
  for(int j = 0; j < Tau_pt->size(); j++) {
    if (!passRecoTau2Cuts(j)) continue;
    nGoodCandidatesTau2++;
  }
  if (nGoodCandidatesTau2>=_RecoTau2Nmin) _EventFlag[_mapSelectionAlgoID["RecoTau2Nmin"]] = true;
  if (nGoodCandidatesTau2<=_RecoTau2Nmax) _EventFlag[_mapSelectionAlgoID["RecoTau2Nmax"]] = true;

  //---recalculate MET
  double temppx = theMETVector.Px() + deltaForMEx;
  double temppy = theMETVector.Py() + deltaForMEy;
  double temppz = theMETVector.Pz();
  double temppt = TMath::Sqrt((temppx*temppx) + (temppy*temppy));
  TLorentzVector theTempMETVector(temppx,temppy,temppz,temppt);
  theMETVector = theTempMETVector;

  // ------Number of Good Jets
  int nGoodJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoJet1Cuts(j)) continue;
    nGoodJets++;
  }
  if (nGoodJets>=_RecoJet1Nmin) _EventFlag[_mapSelectionAlgoID["RecoJet1Nmin"]] = true;
  if (nGoodJets<=_RecoJet1Nmax) _EventFlag[_mapSelectionAlgoID["RecoJet1Nmax"]] = true;

  nGoodJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoJet2Cuts(j)) continue;
    nGoodJets++;
  }
  if (nGoodJets>=_RecoJet2Nmin) _EventFlag[_mapSelectionAlgoID["RecoJet2Nmin"]] = true;
  if (nGoodJets<=_RecoJet2Nmax) _EventFlag[_mapSelectionAlgoID["RecoJet2Nmax"]] = true;

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

// Reco level electron1 requirements
bool BSM3GAnalyzer::passRecoElectron1Cuts(int nobj) {
  // ----Matching to gen
  if((_MatchElectronToGen == "1") && (isData == "0")) {
    if(!(matchElectronToGen(smearedElectronMomentumVector.at(nobj)).first)) {return false;}
  }

  // ----Acceptance cuts
  if (fabs(smearedElectronMomentumVector.at(nobj).Eta())>_RecoElectron1EtaCut) {return false;}
  if (smearedElectronMomentumVector.at(nobj).Pt()<_RecoElectron1PtMinCut) {return false;}
  if (smearedElectronMomentumVector.at(nobj).Pt()>_RecoElectron1PtMaxCut) {return false;}

  // ----Isolation requirement
  if (_DoRecoElectron1DiscrByIsolation == "1") {
    float relIsoWithDBeta = patElectron_isoChargedHadrons->at(nobj) + std::max(0.0 , patElectron_isoNeutralHadrons->at(nobj) + patElectron_isoPhotons->at(nobj) - 0.5 * patElectron_isoPU->at(nobj) );
    relIsoWithDBeta = relIsoWithDBeta / smearedElectronMomentumVector.at(nobj).Pt();
    if(relIsoWithDBeta >= _RecoElectron1IsoSumPtMaxCutValue) {return false;}
    if(relIsoWithDBeta < _RecoElectron1IsoSumPtMinCutValue) {return false;}
  }

  //----Require electron to pass ID discriminators
  if (_DoRecoElectron1DiscrByVetoID == "1") {if (patElectron_isPassVeto->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron1DiscrByLooseID == "1") {if (patElectron_isPassLoose->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron1DiscrByMediumID == "1") {if (patElectron_isPassMedium->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron1DiscrByTightID == "1") {if (patElectron_isPassTight->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron1DiscrByHEEPID == "1") {if (patElectron_isPassHEEPId->at(nobj) == 0) {return false;}}

  return true;
}

// Reco level electron2 requirements
bool BSM3GAnalyzer::passRecoElectron2Cuts(int nobj) {
  // ----Matching to gen
  if((_MatchElectronToGen == "1") && (isData == "0")) {
    if(!(matchElectronToGen(smearedElectronMomentumVector.at(nobj)).first)) {return false;}
  }

  // ----Acceptance cuts
  if (fabs(smearedElectronMomentumVector.at(nobj).Eta())>_RecoElectron2EtaCut) {return false;}
  if (smearedElectronMomentumVector.at(nobj).Pt()<_RecoElectron2PtMinCut) {return false;}
  if (smearedElectronMomentumVector.at(nobj).Pt()>_RecoElectron2PtMaxCut) {return false;}

  // ----Isolation requirement
  if (_DoRecoElectron2DiscrByIsolation == "1") {
    float relIsoWithDBeta = patElectron_isoChargedHadrons->at(nobj) + std::max(0.0 , patElectron_isoNeutralHadrons->at(nobj) + patElectron_isoPhotons->at(nobj) - 0.5 * patElectron_isoPU->at(nobj) );
    relIsoWithDBeta = relIsoWithDBeta / smearedElectronMomentumVector.at(nobj).Pt();
    if(relIsoWithDBeta >= _RecoElectron2IsoSumPtMaxCutValue) {return false;}
    if(relIsoWithDBeta < _RecoElectron2IsoSumPtMinCutValue) {return false;}
  }

  //----Require electron to pass ID discriminators
  if (_DoRecoElectron2DiscrByVetoID == "1") {if (patElectron_isPassVeto->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron2DiscrByLooseID == "1") {if (patElectron_isPassLoose->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron2DiscrByMediumID == "1") {if (patElectron_isPassMedium->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron2DiscrByTightID == "1") {if (patElectron_isPassTight->at(nobj) == 0) {return false;}}
  if (_DoRecoElectron2DiscrByHEEPID == "1") {if (patElectron_isPassHEEPId->at(nobj) == 0) {return false;}}

  return true;
}

// Reco level tau1 requirements
bool BSM3GAnalyzer::passRecoTau1Cuts(int nobj) {
  //----Matching to gen
  if((_MatchTauToGen == "1") && (isData == "0")) { // if the user wants to apply matching to gen had taus and this is simulation
    if(!(matchTauToGen(smearedTauMomentumVector.at(nobj)).first)) {return false;} // if there's no match, reco tau fails the matching cut
  }

  // ----Acceptance cuts
  if (fabs(smearedTauMomentumVector.at(nobj).Eta())>_RecoTau1EtaCut) {return false;}
  if (smearedTauMomentumVector.at(nobj).Pt()<_RecoTau1PtMinCut) {return false;}
  if (smearedTauMomentumVector.at(nobj).Pt()>_RecoTau1PtMaxCut) {return false;}

  // ----Lead track requirement
  if (_DoRecoTau1DiscrByLeadTrack == "1") {
    if(Tau_leadChargedCandPt->at(nobj) < _RecoTau1LeadTrackThreshold) {return false;}
  }

  // ----Isolation requirement
  if (_DoRecoTau1DiscrByIsolation == "1") {

    //--- max isolation requirement
    if(_RecoTau1DiscrByMaxIsolation == "byLooseIsolationMVA3newDMwLT") {
      if (Tau_byLooseIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumIsolationMVA3newDMwLT") {
      if (Tau_byMediumIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightIsolationMVA3newDMwLT") {
      if (Tau_byTightIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseIsolationMVA3newDMwoLT") {
      if (Tau_byLooseIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumIsolationMVA3newDMwoLT") {
      if (Tau_byMediumIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightIsolationMVA3newDMwoLT") {
      if (Tau_byTightIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseIsolationMva3oldDMwLT") {
      if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumIsolationMva3oldDMwLT") {
      if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightIsolationMva3oldDMwLT") {
      if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseIsolationMVA3oldDMwoLT") {
      if (Tau_byLooseIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumIsolationMVA3oldDMwoLT") {
      if (Tau_byMediumIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightIsolationMVA3oldDMwoLT") {
      if (Tau_byTightIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byVLooseCombinedIsolationDeltaBetaCorr") {
      if (Tau_byVLooseCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else { }

    //--- min isolation requirement
    if(_RecoTau1DiscrByMinIsolation == "ZERO") { }
    else {
      if(_RecoTau1DiscrByMinIsolation == "byLooseIsolationMVA3newDMwLT") {
        if (Tau_byLooseIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumIsolationMVA3newDMwLT") {
        if (Tau_byMediumIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightIsolationMVA3newDMwLT") {
        if (Tau_byTightIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseIsolationMVA3newDMwoLT") {
        if (Tau_byLooseIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumIsolationMVA3newDMwoLT") {
        if (Tau_byMediumIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightIsolationMVA3newDMwoLT") {
        if (Tau_byTightIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseIsolationMva3oldDMwLT") {
        if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumIsolationMva3oldDMwLT") {
        if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightIsolationMva3oldDMwLT") {
        if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseIsolationMVA3oldDMwoLT") {
        if (Tau_byLooseIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumIsolationMVA3oldDMwoLT") {
        if (Tau_byMediumIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightIsolationMVA3oldDMwoLT") {
        if (Tau_byTightIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byVLooseCombinedIsolationDeltaBetaCorr") {
        if (Tau_byVLooseCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else { }
    }

  }

  // ----Require 1 or 3 prongs
  if (_RecoTau1DiscrByProngType == "1or3") {
    if((Tau_nProngs->at(nobj) == 1) ||(Tau_nProngs->at(nobj) == 3)) {}
    else {return false;}
  } else if (_RecoTau1DiscrByProngType == "1") {
    if(Tau_nProngs->at(nobj) == 1) {}
    else {return false;}
  } else if (_RecoTau1DiscrByProngType == "3") {
    if(Tau_nProngs->at(nobj) == 3) {}
    else {return false;}
  } else if (_RecoTau1DiscrByProngType == "1or2or3hps") {
    if (Tau_decayModeFindingNewDMs->at(nobj)  < 0.5) {return false;}
  } else if (_RecoTau1DiscrByProngType == "1hps") {
    if( (Tau_nProngs->at(nobj) == 1) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else if (_RecoTau1DiscrByProngType == "2hps") {
    if( (Tau_nProngs->at(nobj) == 2) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else if (_RecoTau1DiscrByProngType == "3hps") {
    if( (Tau_nProngs->at(nobj) == 3) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else {}

  // ----Electron and Muon vetos
  if ((_DoRecoTau1DiscrAgainstElectron == "1") && (_SelectTau1sThatAreElectrons == "0")) {
    if(_RecoTau1DiscrAgainstElectron == "againstElectronMVALooseMVA5") {
      if (Tau_againstElectronMVALooseMVA5->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstElectron == "againstElectronMVAMediumMVA5") {
      if (Tau_againstElectronMVAMediumMVA5->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstElectron == "againstElectronMVATightMVA5") {
      if (Tau_againstElectronMVATightMVA5->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau1sThatAreElectrons == "1") {
    if(_RecoTau1DiscrAgainstElectron == "againstElectronMVALooseMVA5") {
      if (Tau_againstElectronMVALooseMVA5->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstElectron == "againstElectronMVAMediumMVA5") {
      if (Tau_againstElectronMVAMediumMVA5->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstElectron == "againstElectronMVATightMVA5") {
      if (Tau_againstElectronMVATightMVA5->at(nobj) > 0.5) {return false;}
    } else { }
  }
  if (_DoRecoTau1DiscrByCrackCut == "1") { if(isInTheCracks(smearedTauMomentumVector.at(nobj).Eta())) {return false;} }
  if ((_DoRecoTau1DiscrAgainstMuon == "1") && (_SelectTau1sThatAreMuons == "0")) {
    if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose2") {
      if (Tau_againstMuonLoose2->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonTight2") {
      if (Tau_againstMuonTight2->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau1sThatAreMuons == "1") {
    if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose2") {
      if (Tau_againstMuonLoose2->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonTight2") {
      if (Tau_againstMuonTight2->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) > 0.5) {return false;}
    } else { }
  }

  // ----anti-overlap requirements
  if (_RemoveTau1OverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Tau1Muon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau1OverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Tau1Muon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau1OverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Tau1Electron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau1OverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Tau1Electron2MatchingDeltaR) {return false;}
      }
    }
  }

  return true;
}

// Reco level Tau2 requirements
bool BSM3GAnalyzer::passRecoTau2Cuts(int nobj) {
  //----Matching to gen
  if((_MatchTauToGen == "1") && (isData == "0")) { // if the user wants to apply matching to gen had taus and this is simulation
    if(!(matchTauToGen(smearedTauMomentumVector.at(nobj)).first)) {return false;} // if there's no match, reco tau fails the matching cut
  }

  // ----Acceptance cuts
  if (fabs(smearedTauMomentumVector.at(nobj).Eta())>_RecoTau2EtaCut) {return false;}
  if (smearedTauMomentumVector.at(nobj).Pt()<_RecoTau2PtMinCut) {return false;}
  if (smearedTauMomentumVector.at(nobj).Pt()>_RecoTau2PtMaxCut) {return false;}

  // ----Lead track requirement
  if (_DoRecoTau2DiscrByLeadTrack == "1") {
    if(Tau_leadChargedCandPt->at(nobj) < _RecoTau2LeadTrackThreshold) {return false;}
  }

  // ----Isolation requirement
  if (_DoRecoTau2DiscrByIsolation == "1") {

    //--- max isolation requirement
    if(_RecoTau2DiscrByMaxIsolation == "byLooseIsolationMVA3newDMwLT") {
      if (Tau_byLooseIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumIsolationMVA3newDMwLT") {
      if (Tau_byMediumIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightIsolationMVA3newDMwLT") {
      if (Tau_byTightIsolationMVA3newDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseIsolationMVA3newDMwoLT") {
      if (Tau_byLooseIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumIsolationMVA3newDMwoLT") {
      if (Tau_byMediumIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightIsolationMVA3newDMwoLT") {
      if (Tau_byTightIsolationMVA3newDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseIsolationMva3oldDMwLT") {
      if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumIsolationMva3oldDMwLT") {
      if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightIsolationMva3oldDMwLT") {
      if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseIsolationMVA3oldDMwoLT") {
      if (Tau_byLooseIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumIsolationMVA3oldDMwoLT") {
      if (Tau_byMediumIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightIsolationMVA3oldDMwoLT") {
      if (Tau_byTightIsolationMVA3oldDMwoLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byVLooseCombinedIsolationDeltaBetaCorr") {
      if (Tau_byVLooseCombinedIsolationDeltaBetaCorr->at(nobj) < 0.5) {return false;}
    } else { }

    //--- min isolation requirement
    if(_RecoTau2DiscrByMinIsolation == "ZERO") { }
    else {
      if(_RecoTau2DiscrByMinIsolation == "byLooseIsolationMVA3newDMwLT") {
        if (Tau_byLooseIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumIsolationMVA3newDMwLT") {
        if (Tau_byMediumIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightIsolationMVA3newDMwLT") {
        if (Tau_byTightIsolationMVA3newDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseIsolationMVA3newDMwoLT") {
        if (Tau_byLooseIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumIsolationMVA3newDMwoLT") {
        if (Tau_byMediumIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightIsolationMVA3newDMwoLT") {
        if (Tau_byTightIsolationMVA3newDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseIsolationMva3oldDMwLT") {
        if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumIsolationMva3oldDMwLT") {
        if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightIsolationMva3oldDMwLT") {
        if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseIsolationMVA3oldDMwoLT") {
        if (Tau_byLooseIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumIsolationMVA3oldDMwoLT") {
        if (Tau_byMediumIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightIsolationMVA3oldDMwoLT") {
        if (Tau_byTightIsolationMVA3oldDMwoLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byVLooseCombinedIsolationDeltaBetaCorr") {
        if (Tau_byVLooseCombinedIsolationDeltaBetaCorr->at(nobj) > 0.5) {return false;}
      } else { }
    }

  }

  // ----Require 1 or 3 prongs
  if (_RecoTau2DiscrByProngType == "1or3") {
    if((Tau_nProngs->at(nobj) == 1) ||(Tau_nProngs->at(nobj) == 3)) {}
    else {return false;}
  } else if (_RecoTau2DiscrByProngType == "1") {
    if(Tau_nProngs->at(nobj) == 1) {}
    else {return false;}
  } else if (_RecoTau2DiscrByProngType == "3") {
    if(Tau_nProngs->at(nobj) == 3) {}
    else {return false;}
  } else if (_RecoTau2DiscrByProngType == "1or2or3hps") {
    if (Tau_decayModeFindingNewDMs->at(nobj)  < 0.5) {return false;}
  } else if (_RecoTau2DiscrByProngType == "1hps") {
    if( (Tau_nProngs->at(nobj) == 1) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else if (_RecoTau2DiscrByProngType == "2hps") {
    if( (Tau_nProngs->at(nobj) == 2) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else if (_RecoTau2DiscrByProngType == "3hps") {
    if( (Tau_nProngs->at(nobj) == 3) && (Tau_decayModeFindingNewDMs->at(nobj) > 0.5) ) {}
    else {return false;}
  } else {}

  // ----Electron and Muon vetos
  if ((_DoRecoTau2DiscrAgainstElectron == "1") && (_SelectTau2sThatAreElectrons == "0")) {
    if(_RecoTau2DiscrAgainstElectron == "againstElectronMVALooseMVA5") {
      if (Tau_againstElectronMVALooseMVA5->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstElectron == "againstElectronMVAMediumMVA5") {
      if (Tau_againstElectronMVAMediumMVA5->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstElectron == "againstElectronMVATightMVA5") {
      if (Tau_againstElectronMVATightMVA5->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau2sThatAreElectrons == "1") {
    if(_RecoTau2DiscrAgainstElectron == "againstElectronMVALooseMVA5") {
      if (Tau_againstElectronMVALooseMVA5->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstElectron == "againstElectronMVAMediumMVA5") {
      if (Tau_againstElectronMVAMediumMVA5->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstElectron == "againstElectronMVATightMVA5") {
      if (Tau_againstElectronMVATightMVA5->at(nobj) > 0.5) {return false;}
    } else { }
  }
  if (_DoRecoTau2DiscrByCrackCut == "1") { if(isInTheCracks(smearedTauMomentumVector.at(nobj).Eta())) {return false;} }
  if ((_DoRecoTau2DiscrAgainstMuon == "1") && (_SelectTau2sThatAreMuons == "0")) {
    if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose2") {
      if (Tau_againstMuonLoose2->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonTight2") {
      if (Tau_againstMuonTight2->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau2sThatAreMuons == "1") {
    if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose2") {
      if (Tau_againstMuonLoose2->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonTight2") {
      if (Tau_againstMuonTight2->at(nobj) > 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) > 0.5) {return false;}
    } else { }
  }

  // ----anti-overlap requirements
  if (_RemoveTau2OverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Tau2Muon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau2OverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Tau2Muon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau2OverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Tau2Electron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveTau2OverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedTauMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Tau2Electron2MatchingDeltaR) {return false;}
      }
    }
  }

  return true;
}

// Reco level jet1 requirements
bool BSM3GAnalyzer::passRecoJet1Cuts(int nobj) {
  // ----Acceptance cuts
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())>_RecoJet1EtaMaxCut) {return false;}
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())<_RecoJet1EtaMinCut) {return false;}
  if (smearedJetMomentumVector.at(nobj).Pt()<_RecoJet1PtCut) {return false;}
  if (_ApplyJet1LooseID == "1") {
    if (!passedLooseJetID(nobj)) {return false;}
  }

  // ----anti-overlap requirements
  if (_RemoveJet1OverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Jet1Muon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet1OverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Jet1Muon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet1OverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Jet1Electron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet1OverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Jet1Electron2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet1OverlapWithTau1s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _Jet1Tau1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet1OverlapWithTau2s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _Jet1Tau2MatchingDeltaR) {return false;}
      }
    }
  }
  return true;
}

// Reco level jet2 requirements
bool BSM3GAnalyzer::passRecoJet2Cuts(int nobj) {
  // ----Acceptance cuts
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())>_RecoJet2EtaMaxCut) {return false;}
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())<_RecoJet2EtaMinCut) {return false;}
  if (smearedJetMomentumVector.at(nobj).Pt()<_RecoJet2PtCut) {return false;}
  if (_ApplyJet2LooseID == "1") {
    if (!passedLooseJetID(nobj)) {return false;}
  }

  // ----anti-overlap requirements
  if (_RemoveJet2OverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Jet2Muon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet2OverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _Jet2Muon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet2OverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Jet2Electron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet2OverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _Jet2Electron2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet2OverlapWithTau1s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _Jet2Tau1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveJet2OverlapWithTau2s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _Jet2Tau2MatchingDeltaR) {return false;}
      }
    }
  }
  return true;
}

bool BSM3GAnalyzer::passedLooseJetID(int nobj) {
  if (Jet_neutralHadEnergyFraction->at(nobj) >= 0.99) {return false;}
  if (Jet_neutralEmEmEnergyFraction->at(nobj) >= 0.99) {return false;}
  if (Jet_numberOfConstituents->at(nobj) <= 1) {return false;}
  if (Jet_muonEnergyFraction->at(nobj) >= 0.80) {return false;}
  if ( (fabs(smearedJetMomentumVector.at(nobj).Eta()) < 2.4) && (Jet_chargedHadronEnergyFraction->at(nobj) <= 0.0) ) {return false;}
  if ( (fabs(smearedJetMomentumVector.at(nobj).Eta()) < 2.4) && (Jet_chargedMultiplicity->at(nobj) <= 0.0) ) {return false;}
  if ( (fabs(smearedJetMomentumVector.at(nobj).Eta()) < 2.4) && (Jet_chargedEmEnergyFraction->at(nobj) >= 0.99) ) {return false;}
  return true;
}

TLorentzVector BSM3GAnalyzer::SmearMuon(int index) {
  TLorentzVector smearedMomentum;
  smearedMomentum.SetPtEtaPhiE(Muon_pt->at(index), Muon_eta->at(index), Muon_phi->at(index), Muon_energy->at(index));
  double smearedPt;
  double smearedEta;
  double smearedPhi;
  double smearedEnergy;
  if(isData == "0") {
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

TLorentzVector BSM3GAnalyzer::SmearElectron(int index) {
  TLorentzVector smearedMomentum;
  smearedMomentum.SetPtEtaPhiE(patElectron_pt->at(index), patElectron_eta->at(index), patElectron_phi->at(index), patElectron_energy->at(index));
  double smearedPt;
  double smearedEta;
  double smearedPhi;
  double smearedEnergy;
  if(isData == "0") {
    if(matchElectronToGen(smearedMomentum).first) {
      TLorentzVector unsmearedMomentum = matchElectronToGen(smearedMomentum).second;
      if(_SmearTheElectron == "1") {
        smearedPt = (unsmearedMomentum.Pt() * _ElectronPtScaleOffset) + ((smearedMomentum.Pt() -  unsmearedMomentum.Pt()) * _ElectronPtSigmaOffset);
        smearedEta = (unsmearedMomentum.Eta() * _ElectronEtaScaleOffset) + ((smearedMomentum.Eta() -  unsmearedMomentum.Eta()) * _ElectronEtaSigmaOffset);
        smearedPhi = (unsmearedMomentum.Phi() * _ElectronPhiScaleOffset) + ((smearedMomentum.Phi() -  unsmearedMomentum.Phi()) * _ElectronPhiSigmaOffset);
        smearedEnergy = (unsmearedMomentum.Energy() * _ElectronEnergyScaleOffset) + ((smearedMomentum.Energy() -  unsmearedMomentum.Energy()) * _ElectronEnergySigmaOffset);
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
    return smearedMomentum;
  }
}

//---Matching Electrons to generator level objects
pair<bool, TLorentzVector> BSM3GAnalyzer::matchElectronToGen(TLorentzVector p4Vector) {
  bool isGenMatched = false;
  TLorentzVector theGenObject(0,0,0,0);
  for(int jj = 0; jj < Gen_pt->size(); jj++) {
    if((abs(Gen_pdg_id->at(jj)) == 11) && (Gen_status->at(jj) == 1)) {
      theGenObject.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj));
      if(p4Vector.DeltaR(theGenObject) <= _ElectronToGenMatchingDeltaR) {
        if(_UseElectronMotherId == "1") {
          if(abs(Gen_motherpdg_id->at(jj)) == _ElectronMotherId) { isGenMatched = true; }
        } else { isGenMatched = true; }
      }
    }
  }
  pair<bool, TLorentzVector> GenMatchedInformation(isGenMatched,theGenObject);
  return GenMatchedInformation;
}

TLorentzVector BSM3GAnalyzer::SmearTau(int index) {
  TLorentzVector smearedMomentum;
  smearedMomentum.SetPtEtaPhiE(Tau_pt->at(index), Tau_eta->at(index), Tau_phi->at(index), Tau_energy->at(index)); // reco level had tau momentum
  double smearedPt;
  double smearedEta;
  double smearedPhi;
  double smearedEnergy;
  if(isData == "0") { // it's not real collision data, it's simulation
    if(matchTauToGen(smearedMomentum).first) { // it's a real had tau
      TLorentzVector unsmearedMomentum = matchTauToGen(smearedMomentum).second; // gen level vis had tau momentum
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
  } else { // it's real collision data
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

TLorentzVector BSM3GAnalyzer::SmearJet(int index) {
  bool isRealJet = true;
  TLorentzVector tempJetVector(Jet_pt->at(index), Jet_eta->at(index), Jet_phi->at(index), Jet_energy->at(index));
  TLorentzVector tempVector;
  if(isData == "0") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      tempVector.SetPtEtaPhiE(Muon_pt->at(jj), Muon_eta->at(jj), Muon_phi->at(jj), Muon_energy->at(jj));
      if( ((tempJetVector.DeltaR(tempVector) < _CentralJetMuon1MatchingDeltaR) ||
           (tempJetVector.DeltaR(tempVector) < _CentralJetMuon2MatchingDeltaR))
          && (matchMuonToGen(tempVector).first) ) {isRealJet = false;}
    }
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      tempVector.SetPtEtaPhiE(patElectron_pt->at(jj), patElectron_eta->at(jj), patElectron_phi->at(jj), patElectron_energy->at(jj));
      if( ((tempJetVector.DeltaR(tempVector) < _CentralJetElectron1MatchingDeltaR) ||
           (tempJetVector.DeltaR(tempVector) < _CentralJetElectron2MatchingDeltaR))
          && (matchElectronToGen(tempVector).first) ) {isRealJet = false;}
    }
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      tempVector.SetPtEtaPhiE(Tau_pt->at(jj), Tau_eta->at(jj), Tau_phi->at(jj), Tau_energy->at(jj));
      if( ((tempJetVector.DeltaR(tempVector) < _CentralJetTau1MatchingDeltaR) ||
           (tempJetVector.DeltaR(tempVector) < _CentralJetTau2MatchingDeltaR))
          && (matchTauToGen(tempVector).first) ) {isRealJet = false;}
    }
    if(isRealJet) {
      TLorentzVector smearedMomentum = _JetEnergyScaleOffset * tempJetVector;
      return smearedMomentum;
    } else {
      TLorentzVector smearedMomentum = tempJetVector;
      return smearedMomentum;
    }
  } else {
    TLorentzVector smearedMomentum = tempJetVector;
    return smearedMomentum;
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

bool BSM3GAnalyzer::isInTheCracks(float etaValue){
  return (fabs(etaValue) < 0.018 ||
          (fabs(etaValue)>0.423 && fabs(etaValue)<0.461) ||
          (fabs(etaValue)>0.770 && fabs(etaValue)<0.806) ||
          (fabs(etaValue)>1.127 && fabs(etaValue)<1.163) ||
          (fabs(etaValue)>1.460 && fabs(etaValue)<1.558));
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
  patElectron_eta = 0;
  patElectron_phi = 0;
  patElectron_pt = 0;
  patElectron_energy = 0;
  patElectron_isPassVeto = 0;
  patElectron_isPassLoose = 0;
  patElectron_isPassMedium = 0;
  patElectron_isPassTight = 0;
  patElectron_isPassHEEPId = 0;
  patElectron_isoChargedHadrons = 0;
  patElectron_isoNeutralHadrons = 0;
  patElectron_isoPhotons = 0;
  patElectron_isoPU = 0;
  Tau_eta = 0;
  Tau_phi = 0;
  Tau_pt = 0;
  Tau_energy = 0;
  Tau_charge = 0;
  Tau_decayModeFinding = 0;
  Tau_decayModeFindingNewDMs = 0;
  Tau_chargedIsoPtSum = 0;
  Tau_neutralIsoPtSum = 0;
  Tau_againstMuonTight3 = 0;
  Tau_againstElectronMVATightMVA5 = 0;
  Tau_nProngs = 0;
  Tau_puCorrPtSum = 0;
  Tau_byLooseCombinedIsolationDeltaBetaCorr = 0;
  Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byMediumCombinedIsolationDeltaBetaCorr = 0;
  Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byTightCombinedIsolationDeltaBetaCorr = 0;
  Tau_byTightCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byLooseIsolationMVA3newDMwLT = 0;
  Tau_byLooseIsolationMVA3newDMwoLT = 0;
  Tau_byLooseIsolationMva3oldDMwLT = 0;
  Tau_byLooseIsolationMVA3oldDMwoLT = 0;
  Tau_byMediumIsolationMVA3newDMwLT = 0;
  Tau_byMediumIsolationMVA3newDMwoLT = 0;
  Tau_byMediumIsolationMva3oldDMwLT = 0;
  Tau_byMediumIsolationMVA3oldDMwoLT = 0;
  Tau_byTightIsolationMVA3newDMwLT = 0;
  Tau_byTightIsolationMVA3newDMwoLT = 0;
  Tau_byTightIsolationMva3oldDMwLT = 0;
  Tau_byTightIsolationMVA3oldDMwoLT = 0;
  Tau_againstMuonLoose2 = 0;
  Tau_againstMuonLoose3 = 0;
  Tau_againstMuonTight2 = 0;
  Tau_againstElectronMVALooseMVA5 = 0;
  Tau_againstElectronMVAMediumMVA5 = 0;
  Tau_byVLooseCombinedIsolationDeltaBetaCorr = 0;
  Tau_leadChargedCandPt = 0;
  Tau_leadChargedCandCharge = 0;
  Tau_leadChargedCandEta = 0;
  Tau_leadChargedCandPhi = 0;
  Jet_pt = 0;
  Jet_eta = 0;
  Jet_phi = 0;
  Jet_energy = 0;
  Jet_neutralHadEnergyFraction = 0;
  Jet_neutralEmEmEnergyFraction = 0;
  Jet_numberOfConstituents = 0;
  Jet_muonEnergyFraction = 0;
  Jet_chargedHadronEnergyFraction = 0;
  Jet_chargedMultiplicity = 0;
  Jet_chargedEmEnergyFraction = 0;
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
  BOOM->SetBranchAddress("patElectron_eta", &patElectron_eta, &b_patElectron_eta);
  BOOM->SetBranchAddress("patElectron_phi", &patElectron_phi, &b_patElectron_phi);
  BOOM->SetBranchAddress("patElectron_pt", &patElectron_pt, &b_patElectron_pt);
  BOOM->SetBranchAddress("patElectron_energy", &patElectron_energy, &b_patElectron_energy);
  BOOM->SetBranchAddress("patElectron_isPassVeto", &patElectron_isPassVeto, &b_patElectron_isPassVeto);
  BOOM->SetBranchAddress("patElectron_isPassLoose", &patElectron_isPassLoose, &b_patElectron_isPassLoose);
  BOOM->SetBranchAddress("patElectron_isPassMedium", &patElectron_isPassMedium, &b_patElectron_isPassMedium);
  BOOM->SetBranchAddress("patElectron_isPassTight", &patElectron_isPassTight, &b_patElectron_isPassTight);
  BOOM->SetBranchAddress("patElectron_isPassHEEPId", &patElectron_isPassHEEPId, &b_patElectron_isPassHEEPId);
  BOOM->SetBranchAddress("patElectron_isoChargedHadrons", &patElectron_isoChargedHadrons, &b_patElectron_isoChargedHadrons);
  BOOM->SetBranchAddress("patElectron_isoNeutralHadrons", &patElectron_isoNeutralHadrons, &b_patElectron_isoNeutralHadrons);
  BOOM->SetBranchAddress("patElectron_isoPhotons", &patElectron_isoPhotons, &b_patElectron_isoPhotons);
  BOOM->SetBranchAddress("patElectron_isoPU", &patElectron_isoPU, &b_patElectron_isoPU);
  BOOM->SetBranchAddress("Tau_eta", &Tau_eta, &b_Tau_eta);
  BOOM->SetBranchAddress("Tau_phi", &Tau_phi, &b_Tau_phi);
  BOOM->SetBranchAddress("Tau_pt", &Tau_pt, &b_Tau_pt);
  BOOM->SetBranchAddress("Tau_energy", &Tau_energy, &b_Tau_energy);
  BOOM->SetBranchAddress("Tau_charge", &Tau_charge, &b_Tau_charge);
  BOOM->SetBranchAddress("Tau_decayModeFinding", &Tau_decayModeFinding, &b_Tau_decayModeFinding);
  BOOM->SetBranchAddress("Tau_decayModeFindingNewDMs", &Tau_decayModeFindingNewDMs, &b_Tau_decayModeFindingNewDMs);
  BOOM->SetBranchAddress("Tau_chargedIsoPtSum", &Tau_chargedIsoPtSum, &b_Tau_chargedIsoPtSum);
  BOOM->SetBranchAddress("Tau_neutralIsoPtSum", &Tau_neutralIsoPtSum, &b_Tau_neutralIsoPtSum);
  BOOM->SetBranchAddress("Tau_againstMuonTight3", &Tau_againstMuonTight3, &b_Tau_againstMuonTight3);
  BOOM->SetBranchAddress("Tau_againstElectronMVATightMVA5", &Tau_againstElectronMVATightMVA5, &b_Tau_againstElectronMVATightMVA5);
  BOOM->SetBranchAddress("Tau_nProngs", &Tau_nProngs, &b_Tau_nProngs);
  BOOM->SetBranchAddress("Tau_puCorrPtSum", &Tau_puCorrPtSum, &b_Tau_puCorrPtSum);
  BOOM->SetBranchAddress("Tau_byLooseCombinedIsolationDeltaBetaCorr", &Tau_byLooseCombinedIsolationDeltaBetaCorr, &b_Tau_byLooseCombinedIsolationDeltaBetaCorr);
  BOOM->SetBranchAddress("Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits", &Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byMediumCombinedIsolationDeltaBetaCorr", &Tau_byMediumCombinedIsolationDeltaBetaCorr, &b_Tau_byMediumCombinedIsolationDeltaBetaCorr);
  BOOM->SetBranchAddress("Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", &Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byTightCombinedIsolationDeltaBetaCorr", &Tau_byTightCombinedIsolationDeltaBetaCorr, &b_Tau_byTightCombinedIsolationDeltaBetaCorr);
  BOOM->SetBranchAddress("Tau_byTightCombinedIsolationDeltaBetaCorr3Hits", &Tau_byTightCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byTightCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3newDMwLT", &Tau_byLooseIsolationMVA3newDMwLT, &b_Tau_byLooseIsolationMVA3newDMwLT);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3newDMwoLT", &Tau_byLooseIsolationMVA3newDMwoLT, &b_Tau_byLooseIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMva3oldDMwLT", &Tau_byLooseIsolationMva3oldDMwLT, &b_Tau_byLooseIsolationMva3oldDMwLT);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3oldDMwoLT", &Tau_byLooseIsolationMVA3oldDMwoLT, &b_Tau_byLooseIsolationMVA3oldDMwoLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3newDMwLT", &Tau_byMediumIsolationMVA3newDMwLT, &b_Tau_byMediumIsolationMVA3newDMwLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3newDMwoLT", &Tau_byMediumIsolationMVA3newDMwoLT, &b_Tau_byMediumIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMva3oldDMwLT", &Tau_byMediumIsolationMva3oldDMwLT, &b_Tau_byMediumIsolationMva3oldDMwLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3oldDMwoLT", &Tau_byMediumIsolationMVA3oldDMwoLT, &b_Tau_byMediumIsolationMVA3oldDMwoLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMVA3newDMwLT", &Tau_byTightIsolationMVA3newDMwLT, &b_Tau_byTightIsolationMVA3newDMwLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMVA3newDMwoLT", &Tau_byTightIsolationMVA3newDMwoLT, &b_Tau_byTightIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMva3oldDMwLT", &Tau_byTightIsolationMva3oldDMwLT, &b_Tau_byTightIsolationMva3oldDMwLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMVA3oldDMwoLT", &Tau_byTightIsolationMVA3oldDMwoLT, &b_Tau_byTightIsolationMVA3oldDMwoLT);
  BOOM->SetBranchAddress("Tau_againstMuonLoose2", &Tau_againstMuonLoose2, &b_Tau_againstMuonLoose2);
  BOOM->SetBranchAddress("Tau_againstMuonLoose3", &Tau_againstMuonLoose3, &b_Tau_againstMuonLoose3);
  BOOM->SetBranchAddress("Tau_againstMuonTight2", &Tau_againstMuonTight2, &b_Tau_againstMuonTight2);
  BOOM->SetBranchAddress("Tau_againstElectronMVALooseMVA5", &Tau_againstElectronMVALooseMVA5, &b_Tau_againstElectronMVALooseMVA5);
  BOOM->SetBranchAddress("Tau_againstElectronMVAMediumMVA5", &Tau_againstElectronMVAMediumMVA5, &b_Tau_againstElectronMVAMediumMVA5);
  BOOM->SetBranchAddress("Tau_byVLooseCombinedIsolationDeltaBetaCorr", &Tau_byVLooseCombinedIsolationDeltaBetaCorr, &b_Tau_byVLooseCombinedIsolationDeltaBetaCorr);
  BOOM->SetBranchAddress("Tau_leadChargedCandPt", &Tau_leadChargedCandPt, &b_Tau_leadChargedCandPt);
  BOOM->SetBranchAddress("Tau_leadChargedCandCharge", &Tau_leadChargedCandCharge, &b_Tau_leadChargedCandCharge);
  BOOM->SetBranchAddress("Tau_leadChargedCandEta", &Tau_leadChargedCandEta, &b_Tau_leadChargedCandEta);
  BOOM->SetBranchAddress("Tau_leadChargedCandPhi", &Tau_leadChargedCandPhi, &b_Tau_leadChargedCandPhi);
  BOOM->SetBranchAddress("Jet_pt", &Jet_pt, &b_Jet_pt);
  BOOM->SetBranchAddress("Jet_eta", &Jet_eta, &b_Jet_eta);
  BOOM->SetBranchAddress("Jet_phi", &Jet_phi, &b_Jet_phi);
  BOOM->SetBranchAddress("Jet_energy", &Jet_energy, &b_Jet_energy);
  BOOM->SetBranchAddress("Jet_neutralHadEnergyFraction", &Jet_neutralHadEnergyFraction, &b_Jet_neutralHadEnergyFraction);
  BOOM->SetBranchAddress("Jet_neutralEmEmEnergyFraction", &Jet_neutralEmEmEnergyFraction, &b_Jet_neutralEmEmEnergyFraction);
  BOOM->SetBranchAddress("Jet_numberOfConstituents", &Jet_numberOfConstituents, &b_Jet_numberOfConstituents);
  BOOM->SetBranchAddress("Jet_muonEnergyFraction", &Jet_muonEnergyFraction, &b_Jet_muonEnergyFraction);
  BOOM->SetBranchAddress("Jet_chargedHadronEnergyFraction", &Jet_chargedHadronEnergyFraction, &b_Jet_chargedHadronEnergyFraction);
  BOOM->SetBranchAddress("Jet_chargedMultiplicity", &Jet_chargedMultiplicity, &b_Jet_chargedMultiplicity);
  BOOM->SetBranchAddress("Jet_chargedEmEnergyFraction", &Jet_chargedEmEnergyFraction, &b_Jet_chargedEmEnergyFraction);
  BOOM->SetBranchAddress("Met_px", &Met_px, &b_Met_px);
  BOOM->SetBranchAddress("Met_py", &Met_py, &b_Met_py);
  BOOM->SetBranchAddress("Met_pz", &Met_pz, &b_Met_pz);

};
