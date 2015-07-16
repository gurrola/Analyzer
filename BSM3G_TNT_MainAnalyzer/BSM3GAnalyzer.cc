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

  TEnv *params = new TEnv ("config_file");
  params->ReadFile ("BSM3GAnalyzer_CutParameters.in", kEnvChange);
  _GenTauPtMinCut    = params->GetValue ("GenTauPtMinCut", 1.0);
  _GenTauPtMaxCut    = params->GetValue ("GenTauPtMaxCut", 1.0);
  _GenTauEtaMaxCut   = params->GetValue ("GenTauEtaMaxCut", 1.0);
  _GenTauNmin        = params->GetValue ("GenTauNmin", 1);
  _GenTauNmax        = params->GetValue ("GenTauNmax", 1);
  _GenTopNmin        = params->GetValue ("GenTopNmin", 1);
  _GenTopNmax        = params->GetValue ("GenTopNmax", 1);
  _GenElectronNmin   = params->GetValue ("GenElectronNmin", 1);
  _GenElectronNmax   = params->GetValue ("GenElectronNmax", 1);
  _GenMuonNmin       = params->GetValue ("GenMuonNmin", 1);
  _GenMuonNmax       = params->GetValue ("GenMuonNmax", 1);
  _GenZNmin          = params->GetValue ("GenZNmin", 1);
  _GenZNmax          = params->GetValue ("GenZNmax", 1);
  _GenWNmin          = params->GetValue ("GenWNmin", 1);
  _GenWNmax          = params->GetValue ("GenWNmax", 1);
  _GenSMHiggsNmin    = params->GetValue ("GenSMHiggsNmin", 1);
  _GenSMHiggsNmax    = params->GetValue ("GenSMHiggsNmax", 1);
  _RecoVertexNmin    = params->GetValue ("RecoVertexNmin", 1);
  _RecoVertexNmax    = params->GetValue ("RecoVertexNmax", 1);
  _RecoMuon1Nmin     = params->GetValue ("RecoMuon1Nmin", 1);
  _RecoMuon1Nmax     = params->GetValue ("RecoMuon1Nmax", 1);
  _RecoMuon2Nmin     = params->GetValue ("RecoMuon2Nmin", 1);
  _RecoMuon2Nmax     = params->GetValue ("RecoMuon2Nmax", 1);
  _RecoElectron1Nmin = params->GetValue ("RecoElectron1Nmin", 1);
  _RecoElectron1Nmax = params->GetValue ("RecoElectron1Nmax", 1);
  _RecoElectron2Nmin = params->GetValue ("RecoElectron2Nmin", 1);
  _RecoElectron2Nmax = params->GetValue ("RecoElectron2Nmax", 1);
  _RecoTau1Nmin      = params->GetValue ("RecoTau1Nmin", 1);
  _RecoTau1Nmax      = params->GetValue ("RecoTau1Nmax", 1);
  _RecoTau2Nmin      = params->GetValue ("RecoTau2Nmin", 1);
  _RecoTau2Nmax      = params->GetValue ("RecoTau2Nmax", 1);
  _RecoJet1Nmin      = params->GetValue ("RecoJet1Nmin", 1);
  _RecoJet1Nmax      = params->GetValue ("RecoJet1Nmax", 1);
  _RecoJet2Nmin      = params->GetValue ("RecoJet2Nmin", 1);
  _RecoJet2Nmax                      = params->GetValue ("RecoJet2Nmax", 1);
  _DoRecoMuon1DiscrByTightID         = params->GetValue ("DoRecoMuon1DiscrByTightID", "string");
  _DoRecoMuon1DiscrBySoftID          = params->GetValue ("DoRecoMuon1DiscrBySoftID", "string");
  _DoRecoMuon1DiscrByIsolation       = params->GetValue ("DoRecoMuon1DiscrByIsolation", "string");
  _RecoMuon1EtaCut                   = params->GetValue ("RecoMuon1EtaCut", 1.0);
  _RecoMuon1PtMinCut                 = params->GetValue ("RecoMuon1PtMinCut", 1.0);
  _RecoMuon1PtMaxCut                 = params->GetValue ("RecoMuon1PtMaxCut", 1.0);
  _RecoMuon1IsoSumPtMaxCutValue      = params->GetValue ("RecoMuon1IsoSumPtMaxCutValue", 1.0);
  _RecoMuon1IsoSumPtMinCutValue      = params->GetValue ("RecoMuon1IsoSumPtMinCutValue", 1.0);
  _DoRecoMuon2DiscrByTightID         = params->GetValue ("DoRecoMuon2DiscrByTightID", "string");
  _DoRecoMuon2DiscrBySoftID          = params->GetValue ("DoRecoMuon2DiscrBySoftID", "string");
  _DoRecoMuon2DiscrByIsolation       = params->GetValue ("DoRecoMuon2DiscrByIsolation", "string");
  _RecoMuon2EtaCut                   = params->GetValue ("RecoMuon2EtaCut", 1.0);
  _RecoMuon2PtMinCut                 = params->GetValue ("RecoMuon2PtMinCut", 1.0);
  _RecoMuon2PtMaxCut                 = params->GetValue ("RecoMuon2PtMaxCut", 1.0);
  _RecoMuon2IsoSumPtMaxCutValue      = params->GetValue ("RecoMuon2IsoSumPtMaxCutValue", 1.0);
  _RecoMuon2IsoSumPtMinCutValue      = params->GetValue ("RecoMuon2IsoSumPtMinCutValue", 1.0);
  _TreatMuonsAsNeutrinos             = params->GetValue ("TreatMuonsAsNeutrinos", "string");
  _RecoElectron1EtaCut               = params->GetValue ("RecoElectron1EtaCut", 1.0); 
  _RecoElectron1PtMinCut             = params->GetValue ("RecoElectron1PtMinCut", 1.0);
  _RecoElectron1PtMaxCut             = params->GetValue ("RecoElectron1PtMaxCut", 1.0);
  _DoRecoElectron1DiscrByIsolation   = params->GetValue ("DoRecoElectron1DiscrByIsolation", "string");
  _RecoElectron1IsoSumPtMaxCutValue  = params->GetValue ("RecoElectron1IsoSumPtMaxCutValue", 1.0);
  _RecoElectron1IsoSumPtMinCutValue  = params->GetValue ("RecoElectron1IsoSumPtMinCutValue", 1.0);
  _DoRecoElectron1DiscrByVetoID      = params->GetValue ("DoRecoElectron1DiscrByVetoID", "string");
  _DoRecoElectron1DiscrByLooseID     = params->GetValue ("DoRecoElectron1DiscrByLooseID", "string");
  _DoRecoElectron1DiscrByMediumID    = params->GetValue ("DoRecoElectron1DiscrByMediumID", "string");
  _DoRecoElectron1DiscrByTightID     = params->GetValue ("DoRecoElectron1DiscrByTightID", "string");
  _DoRecoElectron1DiscrByHEEPID      = params->GetValue ("DoRecoElectron1DiscrByHEEPID", "string");
  _RecoElectron2EtaCut               = params->GetValue ("RecoElectron2EtaCut", 1.0);
  _RecoElectron2PtMinCut             = params->GetValue ("RecoElectron2PtMinCut", 1.0); 
  _RecoElectron2PtMaxCut             = params->GetValue ("RecoElectron2PtMaxCut", 1.0);
  _DoRecoElectron2DiscrByIsolation   = params->GetValue ("DoRecoElectron2DiscrByIsolation", "string");
  _RecoElectron2IsoSumPtMaxCutValue  = params->GetValue ("RecoElectron2IsoSumPtMaxCutValue", 1.0);
  _RecoElectron2IsoSumPtMinCutValue  = params->GetValue ("RecoElectron2IsoSumPtMinCutValue", 1.0);
  _DoRecoElectron2DiscrByVetoID      = params->GetValue ("DoRecoElectron2DiscrByVetoID", "string");
  _DoRecoElectron2DiscrByLooseID     = params->GetValue ("DoRecoElectron2DiscrByLooseID", "string");
  _DoRecoElectron2DiscrByMediumID    = params->GetValue ("DoRecoElectron2DiscrByMediumID", "string");
  _DoRecoElectron2DiscrByTightID     = params->GetValue ("DoRecoElectron2DiscrByTightID", "string");
  _DoRecoElectron2DiscrByHEEPID      = params->GetValue ("DoRecoElectron2DiscrByHEEPID", "string");
  _RecoTau1EtaCut                    = params->GetValue ("RecoTau1EtaCut", 1.0);
  _RecoTau1PtMinCut                  = params->GetValue ("RecoTau1PtMinCut", 1.0);
  _RecoTau1PtMaxCut                  = params->GetValue ("RecoTau1PtMaxCut", 1.0);
  _DoRecoTau1DiscrByLeadTrack        = params->GetValue ("DoRecoTau1DiscrByLeadTrack", "string");
  _RecoTau1LeadTrackThreshold        = params->GetValue ("RecoTau1LeadTrackThreshold", 1.0);
  _DoRecoTau1DiscrByIsolation        = params->GetValue ("DoRecoTau1DiscrByIsolation", "string");
  _RecoTau1DiscrByMaxIsolation       = params->GetValue ("RecoTau1DiscrByMaxIsolation", "string");
  _RecoTau1DiscrByMinIsolation       = params->GetValue ("RecoTau1DiscrByMinIsolation", "string");
  _RecoTau1DiscrByProngType          = params->GetValue ("RecoTau1DiscrByProngType", "string");
  _DoRecoTau1DiscrAgainstElectron    = params->GetValue ("DoRecoTau1DiscrAgainstElectron", "string");
  _RecoTau1DiscrAgainstElectron      = params->GetValue ("RecoTau1DiscrAgainstElectron", "string");
  _SelectTau1sThatAreElectrons       = params->GetValue ("SelectTau1sThatAreElectrons", "string");
  _DoRecoTau1DiscrAgainstMuon        = params->GetValue ("DoRecoTau1DiscrAgainstMuon", "string");
  _RecoTau1DiscrAgainstMuon          = params->GetValue ("RecoTau1DiscrAgainstMuon", "string");
  _SelectTau1sThatAreMuons           = params->GetValue ("SelectTau1sThatAreMuons", "string");
  _DoRecoTau1DiscrByCrackCut         = params->GetValue ("DoRecoTau1DiscrByCrackCut", "string");
  _RemoveTau1OverlapWithMuon1s       = params->GetValue ("RemoveTau1OverlapWithMuon1s", "string");
  _RemoveTau1OverlapWithMuon2s       = params->GetValue ("RemoveTau1OverlapWithMuon2s", "string");
  _Tau1Muon1MatchingDeltaR           = params->GetValue ("Tau1Muon1MatchingDeltaR", 1.0);
  _Tau1Muon2MatchingDeltaR           = params->GetValue ("Tau1Muon2MatchingDeltaR", 1.0);
  _RemoveTau1OverlapWithElectron1s   = params->GetValue ("RemoveTau1OverlapWithElectron1s", "string");
  _RemoveTau1OverlapWithElectron2s   = params->GetValue ("RemoveTau1OverlapWithElectron2s", "string");
  _Tau1Electron1MatchingDeltaR       = params->GetValue ("Tau1Electron1MatchingDeltaR", 1.0);
  _Tau1Electron2MatchingDeltaR       = params->GetValue ("Tau1Electron2MatchingDeltaR", 1.0);
  _RecoTau2EtaCut                    = params->GetValue ("RecoTau2EtaCut", 1.0);
  _RecoTau2PtMinCut                  = params->GetValue ("RecoTau2PtMinCut", 1.0);
  _RecoTau2PtMaxCut                  = params->GetValue ("RecoTau2PtMaxCut", 1.0);
  _DoRecoTau2DiscrByLeadTrack        = params->GetValue ("DoRecoTau2DiscrByLeadTrack", "string");
  _RecoTau2LeadTrackThreshold        = params->GetValue ("RecoTau2LeadTrackThreshold", 1.0);
  _DoRecoTau2DiscrByIsolation        = params->GetValue ("DoRecoTau2DiscrByIsolation", "string");
  _RecoTau2DiscrByMaxIsolation       = params->GetValue ("RecoTau2DiscrByMaxIsolation", "string");
  _RecoTau2DiscrByMinIsolation       = params->GetValue ("RecoTau2DiscrByMinIsolation", "string");
  _RecoTau2DiscrByProngType          = params->GetValue ("RecoTau2DiscrByProngType", "string");
  _DoRecoTau2DiscrAgainstElectron    = params->GetValue ("DoRecoTau2DiscrAgainstElectron", "string");
  _RecoTau2DiscrAgainstElectron      = params->GetValue ("RecoTau2DiscrAgainstElectron", "string");
  _SelectTau2sThatAreElectrons       = params->GetValue ("SelectTau2sThatAreElectrons", "string");
  _DoRecoTau2DiscrAgainstMuon        = params->GetValue ("DoRecoTau2DiscrAgainstMuon", "string");
  _RecoTau2DiscrAgainstMuon          = params->GetValue ("RecoTau2DiscrAgainstMuon", "string");
  _SelectTau2sThatAreMuons           = params->GetValue ("SelectTau2sThatAreMuons", "string");
  _DoRecoTau2DiscrByCrackCut         = params->GetValue ("DoRecoTau2DiscrByCrackCut", "string");
  _RemoveTau2OverlapWithMuon1s       = params->GetValue ("RemoveTau2OverlapWithMuon1s", "string");
  _RemoveTau2OverlapWithMuon2s       = params->GetValue ("RemoveTau2OverlapWithMuon2s", "string");
  _Tau2Muon1MatchingDeltaR           = params->GetValue ("Tau2Muon1MatchingDeltaR", 1.0);
  _Tau2Muon2MatchingDeltaR           = params->GetValue ("Tau2Muon2MatchingDeltaR", 1.0);
  _RemoveTau2OverlapWithElectron1s   = params->GetValue ("RemoveTau2OverlapWithElectron1s", "string");
  _RemoveTau2OverlapWithElectron2s   = params->GetValue ("RemoveTau2OverlapWithElectron2s", "string");
  _Tau2Electron1MatchingDeltaR       = params->GetValue ("Tau2Electron1MatchingDeltaR", 1.0);
  _Tau2Electron2MatchingDeltaR       = params->GetValue ("Tau2Electron2MatchingDeltaR", 1.0);
  _RecoJet1EtaMaxCut                 = params->GetValue ("RecoJet1EtaMaxCut", 1.0);
  _RecoJet1EtaMinCut                 = params->GetValue ("RecoJet1EtaMinCut", 1.0);
  _RecoJet1PtCut                     = params->GetValue ("RecoJet1PtCut", 1.0);
  _ApplyJet1LooseID                  = params->GetValue ("ApplyJet1LooseID", "string");
  _RemoveJet1OverlapWithMuon1s       = params->GetValue ("RemoveJet1OverlapWithMuon1s", "string");
  _Jet1Muon1MatchingDeltaR           = params->GetValue ("Jet1Muon1MatchingDeltaR", 1.0);
  _RemoveJet1OverlapWithMuon2s       = params->GetValue ("RemoveJet1OverlapWithMuon2s", "string");
  _Jet1Muon2MatchingDeltaR           = params->GetValue ("Jet1Muon2MatchingDeltaR", 1.0);
  _RemoveJet1OverlapWithElectron1s   = params->GetValue ("RemoveJet1OverlapWithElectron1s", "string");
  _Jet1Electron1MatchingDeltaR       = params->GetValue ("Jet1Electron1MatchingDeltaR", 1.0);
  _RemoveJet1OverlapWithElectron2s   = params->GetValue ("RemoveJet1OverlapWithElectron2s", "string");
  _Jet1Electron2MatchingDeltaR       = params->GetValue ("Jet1Electron2MatchingDeltaR", 1.0);
  _RemoveJet1OverlapWithTau1s        = params->GetValue ("RemoveJet1OverlapWithTau1s", "string");
  _Jet1Tau1MatchingDeltaR            = params->GetValue ("Jet1Tau1MatchingDeltaR", 1.0);
  _RemoveJet1OverlapWithTau2s        = params->GetValue ("RemoveJet1OverlapWithTau2s", "string");
  _Jet1Tau2MatchingDeltaR            = params->GetValue ("Jet1Tau2MatchingDeltaR", 1.0);
  _RecoJet2EtaMaxCut                 = params->GetValue ("RecoJet2EtaMaxCut", 1.0);
  _RecoJet2EtaMinCut                 = params->GetValue ("RecoJet2EtaMinCut", 1.0);
  _RecoJet2PtCut                     = params->GetValue ("RecoJet2PtCut", 1.0);
  _ApplyJet2LooseID                  = params->GetValue ("ApplyJet2LooseID", "string");
  _RemoveJet2OverlapWithMuon1s       = params->GetValue ("RemoveJet2OverlapWithMuon1s", "string");
  _Jet2Muon1MatchingDeltaR           = params->GetValue ("Jet2Muon1MatchingDeltaR", 1.0);
  _RemoveJet2OverlapWithMuon2s       = params->GetValue ("RemoveJet2OverlapWithMuon2s", "string");
  _Jet2Muon2MatchingDeltaR           = params->GetValue ("Jet2Muon2MatchingDeltaR", 1.0);
  _RemoveJet2OverlapWithElectron1s   = params->GetValue ("RemoveJet2OverlapWithElectron1s", "string");
  _Jet2Electron1MatchingDeltaR       = params->GetValue ("Jet2Electron1MatchingDeltaR", 1.0);
  _RemoveJet2OverlapWithElectron2s   = params->GetValue ("RemoveJet2OverlapWithElectron2s", "string");
  _Jet2Electron2MatchingDeltaR       = params->GetValue ("Jet2Electron2MatchingDeltaR", 1.0);
  _RemoveJet2OverlapWithTau1s        = params->GetValue ("RemoveJet2OverlapWithTau1s", "string");
  _Jet2Tau1MatchingDeltaR            = params->GetValue ("Jet2Tau1MatchingDeltaR", 1.0);
  _RemoveJet2OverlapWithTau2s        = params->GetValue ("RemoveJet2OverlapWithTau2s", "string");
  _Jet2Tau2MatchingDeltaR            = params->GetValue ("Jet2Tau2MatchingDeltaR", 1.0);
  _CalculatePUSystematics            = params->GetValue ("CalculatePUSystematics", "string");
  _SmearTheMuon                      = params->GetValue ("SmearTheMuon", "string");
  _MatchMuonToGen                    = params->GetValue ("MatchMuonToGen", "string");
  _UseMuonMotherId                   = params->GetValue ("UseMuonMotherId", "string");
  _MuonMotherId                      = params->GetValue ("MuonMotherId", 1);
  _MuonToGenMatchingDeltaR           = params->GetValue ("MuonToGenMatchingDeltaR", 1.0);
  _MuonPtScaleOffset                 = params->GetValue ("MuonPtScaleOffset", 1.0);
  _MuonPtSigmaOffset                 = params->GetValue ("MuonPtSigmaOffset", 1.0);
  _MuonEtaScaleOffset                = params->GetValue ("MuonEtaScaleOffset", 1.0);
  _MuonEtaSigmaOffset                = params->GetValue ("MuonEtaSigmaOffset", 1.0);
  _MuonPhiScaleOffset                = params->GetValue ("MuonPhiScaleOffset", 1.0);
  _MuonPhiSigmaOffset                = params->GetValue ("MuonPhiSigmaOffset", 1.0);
  _MuonEnergyScaleOffset             = params->GetValue ("MuonEnergyScaleOffset", 1.0);
  _MuonEnergySigmaOffset             = params->GetValue ("MuonEnergySigmaOffset", 1.0);
  _SmearTheTau                       = params->GetValue ("SmearTheTau", "string");
  _MatchTauToGen                     = params->GetValue ("MatchTauToGen", "string");
  _TauToGenMatchingDeltaR            = params->GetValue ("TauToGenMatchingDeltaR", 1.0);
  _TauPtScaleOffset                  = params->GetValue ("TauPtScaleOffset", 1.0);
  _TauPtSigmaOffset                  = params->GetValue ("TauPtSigmaOffset", 1.0);
  _TauEtaScaleOffset                 = params->GetValue ("TauEtaScaleOffset", 1.0);
  _TauEtaSigmaOffset                 = params->GetValue ("TauEtaSigmaOffset", 1.0);
  _TauPhiScaleOffset                 = params->GetValue ("TauPhiScaleOffset", 1.0);
  _TauPhiSigmaOffset                 = params->GetValue ("TauPhiSigmaOffset", 1.0);
  _TauEnergyScaleOffset              = params->GetValue ("TauEnergyScaleOffset", 1.0);
  _TauEnergySigmaOffset              = params->GetValue ("TauEnergySigmaOffset", 1.0);
  _SmearTheElectron                  = params->GetValue ("SmearTheElectron", "string");
  _MatchElectronToGen                = params->GetValue ("MatchElectronToGen", "string");
  _UseElectronMotherId               = params->GetValue ("UseElectronMotherId", "string");
  _ElectronMotherId                  = params->GetValue ("ElectronMotherId", 1);
  _ElectronToGenMatchingDeltaR       = params->GetValue ("ElectronToGenMatchingDeltaR", 1.0);
  _ElectronPtScaleOffset             = params->GetValue ("ElectronPtScaleOffset", 1.0);
  _ElectronPtSigmaOffset             = params->GetValue ("ElectronPtSigmaOffset", 1.0);
  _ElectronEtaScaleOffset            = params->GetValue ("ElectronEtaScaleOffset", 1.0);
  _ElectronEtaSigmaOffset            = params->GetValue ("ElectronEtaSigmaOffset", 1.0);
  _ElectronPhiScaleOffset            = params->GetValue ("ElectronPhiScaleOffset", 1.0);
  _ElectronPhiSigmaOffset            = params->GetValue ("ElectronPhiSigmaOffset", 1.0);
  _ElectronEnergyScaleOffset         = params->GetValue ("ElectronEnergyScaleOffset", 1.0);
  _ElectronEnergySigmaOffset         = params->GetValue ("ElectronEnergySigmaOffset", 1.0);
  _SmearTheJet                       = params->GetValue ("SmearTheJet", "string");
  _CentralJetMuon1MatchingDeltaR     = params->GetValue ("CentralJetMuon1MatchingDeltaR", 1.0);
  _CentralJetMuon2MatchingDeltaR     = params->GetValue ("CentralJetMuon2MatchingDeltaR", 1.0);
  _CentralJetElectron1MatchingDeltaR = params->GetValue ("CentralJetElectron1MatchingDeltaR", 1.0);
  _CentralJetElectron2MatchingDeltaR = params->GetValue ("CentralJetElectron2MatchingDeltaR", 1.0);
  _CentralJetTau1MatchingDeltaR      = params->GetValue ("CentralJetTau1MatchingDeltaR", 1.0);
  _CentralJetTau2MatchingDeltaR      = params->GetValue ("CentralJetTau2MatchingDeltaR", 1.0);
  _JetEnergyScaleOffset              = params->GetValue ("JetEnergyScaleOffset", 1.0);
  _DataHistos                        = params->GetValue ("DataHistos", "string");
  isData                             = params->GetValue ("isData", "string");
  _MCHistos                          = params->GetValue ("MCHistos", "string"); 
  _FillGenHists                      = params->GetValue ("FillGenHists", "string");

  string inputString;
  string inputType;

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
      _hNGenTau[i][NpdfCounter]                         = new TH1F(("NGenTau_"+j.str()).c_str(),                       ("NGenTau_"+j.str()).c_str(),     20, 0., 20.);
      _hGenTauEnergy[i][NpdfCounter]                    = new TH1F(("GenTauEnergy_"+j.str()).c_str(),                  ("GenTauEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hGenTauPt[i][NpdfCounter]                        = new TH1F(("GenTauPt_"+j.str()).c_str(),                      ("GenTauPt_"+j.str()).c_str(),    200, 0., 500.);
      _hGenTauEta[i][NpdfCounter]                       = new TH1F(("GenTauEta_"+j.str()).c_str(),                     ("GenTauEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenTauPhi[i][NpdfCounter]                       = new TH1F(("GenTauPhi_"+j.str()).c_str(),                     ("GenTauPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hNGenMuon[i][NpdfCounter]                         = new TH1F(("NGenMuon_"+j.str()).c_str(),                       ("NGenMuon_"+j.str()).c_str(),     20, 0., 20.);
      _hGenMuonEnergy[i][NpdfCounter]                    = new TH1F(("GenMuonEnergy_"+j.str()).c_str(),                  ("GenMuonEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hGenMuonPt[i][NpdfCounter]                        = new TH1F(("GenMuonPt_"+j.str()).c_str(),                      ("GenMuonPt_"+j.str()).c_str(),    200, 0., 500.);
      _hGenMuonEta[i][NpdfCounter]                       = new TH1F(("GenMuonEta_"+j.str()).c_str(),                     ("GenMuonEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenMuonPhi[i][NpdfCounter]                       = new TH1F(("GenMuonPhi_"+j.str()).c_str(),                     ("GenMuonPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
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
