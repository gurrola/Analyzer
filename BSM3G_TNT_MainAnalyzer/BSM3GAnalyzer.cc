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

  //std::cout << "BSM3GAnalyzer Message: Grabbing user defined input parameters" << std::endl;

  //---obtain the configurable inputs
  getInputs();
  //std::cout << "BSM3GAnalyzer Message: Finished grabbing configurable inputs. Attempting to open the Ntuple root file ..." << std::endl;

  //---open input root file and read in the tree information
  //TFile *f = new TFile ("OutTree_9_1_dab.root");
  //TFile *f = new TFile (fname);
  TFile *f = TFile::Open(fname);
  f->cd("TNT");
  TTree* BOOM = (TTree*)f->Get("TNT/BOOM");
  int nentries = (int) BOOM->GetEntries();
  setBranchAddress(BOOM);
  //std::cout << "BSM3GAnalyzer Message: Successfully opened the input root file and reading in the tree information. The event counters and maps are being initialized ..." << std::endl;

  //---initialize event counters and maps
  beginJob();
  //std::cout << "BSM3GAnalyzer Message: Finished initializing event counters and maps. Looping over the events and performing the analysis." << std::endl;

  //---loop over events and perform the analysis
  //for (int entries = 0; entries < 1000; ++entries) { // for testing purposes --> loop over only 1k events
  for(int entries = 0; entries < nentries; ++entries) {
    if((entries+1) % 100 == 0) { std::cout << "BSM3GAnalyzer Message: Analyzing Event #" << entries+1 << std::endl; }
    BOOM->GetEntry(entries);
    analyze(theFile);
  }
  //std::cout << "BSM3GAnalyzer Message: Finished looping over events and performing the analysis (i.e. cuts). Attempting to fill and writeout histograms." << std::endl;

  //---write out histograms and printout cut flow efficiency table
  endJob(theFile);
  //std::cout << "BSM3GAnalyzer Message: Finished writing out histograms" << std::endl;

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
  _RecoTriggers1Nmin = params->GetValue ("RecoTriggers1Nmin", 1);
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
  _RecoCentralJetNmin                = params->GetValue ("RecoCentralJetNmin", 1);
  _RecoCentralJetNmax                = params->GetValue ("RecoCentralJetNmax", 1);
  _RecoFirstLeadingJetNmin           = params->GetValue ("RecoFirstLeadingJetNmin", 1);
  _RecoSecondLeadingJetNmin          = params->GetValue ("RecoSecondLeadingJetNmin", 1);
  _RecoBJetNmin                      = params->GetValue ("RecoBJetNmin", 1);
  _RecoBJetNmax                      = params->GetValue ("RecoBJetNmax", 1);
  _SusyCombinationsNmin              = params->GetValue ("SusyCombinationsNmin", 1);
  _RecoMuon1MetTopologyNmin          = params->GetValue ("RecoMuon1MetTopologyNmin", 1);
  _RecoMuon1MetTopologyNmax          = params->GetValue ("RecoMuon1MetTopologyNmax", 1);
  _RecoMuon2MetTopologyNmin          = params->GetValue ("RecoMuon2MetTopologyNmin", 1);
  _RecoMuon2MetTopologyNmax          = params->GetValue ("RecoMuon2MetTopologyNmax", 1);
  _RecoElectron1MetTopologyNmin          = params->GetValue ("RecoElectron1MetTopologyNmin", 1);
  _RecoElectron1MetTopologyNmax          = params->GetValue ("RecoElectron1MetTopologyNmax", 1);
  _RecoElectron2MetTopologyNmin          = params->GetValue ("RecoElectron2MetTopologyNmin", 1);
  _RecoElectron2MetTopologyNmax          = params->GetValue ("RecoElectron2MetTopologyNmax", 1);
  _RecoTau1MetTopologyNmin          = params->GetValue ("RecoTau1MetTopologyNmin", 1);
  _RecoTau1MetTopologyNmax          = params->GetValue ("RecoTau1MetTopologyNmax", 1);
  _RecoTau2MetTopologyNmin          = params->GetValue ("RecoTau2MetTopologyNmin", 1);
  _RecoTau2MetTopologyNmax          = params->GetValue ("RecoTau2MetTopologyNmax", 1);
  _DiMuonCombinationsNmin           = params->GetValue ("DiMuonCombinationsNmin", 1);
  _DiMuonCombinationsNmax           = params->GetValue ("DiMuonCombinationsNmax", 1);
  _DiElectronCombinationsNmin       = params->GetValue ("DiElectronCombinationsNmin", 1);
  _DiElectronCombinationsNmax       = params->GetValue ("DiElectronCombinationsNmax", 1);
  _DiTauCombinationsNmin            = params->GetValue ("DiTauCombinationsNmin", 1);
  _DiTauCombinationsNmax            = params->GetValue ("DiTauCombinationsNmax", 1);
  _DiJetCombinationsNmin            = params->GetValue ("DiJetCombinationsNmin", 1);
  _DiJetCombinationsNmax            = params->GetValue ("DiJetCombinationsNmax", 1);
  _Muon1Tau1CombinationsNmin        = params->GetValue ("Muon1Tau1CombinationsNmin", 1);
  _Muon1Tau1CombinationsNmax        = params->GetValue ("Muon1Tau1CombinationsNmax", 1);
  _Muon1Tau2CombinationsNmin        = params->GetValue ("Muon1Tau2CombinationsNmin", 1);
  _Muon1Tau2CombinationsNmax        = params->GetValue ("Muon1Tau2CombinationsNmax", 1);
  _Muon2Tau1CombinationsNmin        = params->GetValue ("Muon2Tau1CombinationsNmin", 1);
  _Muon2Tau1CombinationsNmax        = params->GetValue ("Muon2Tau1CombinationsNmax", 1);
  _Muon2Tau2CombinationsNmin        = params->GetValue ("Muon2Tau2CombinationsNmin", 1);
  _Muon2Tau2CombinationsNmax        = params->GetValue ("Muon2Tau2CombinationsNmax", 1);
  _Electron1Tau1CombinationsNmin    = params->GetValue ("Electron1Tau1CombinationsNmin", 1);
  _Electron1Tau1CombinationsNmax    = params->GetValue ("Electron1Tau1CombinationsNmax", 1);
  _Electron1Tau2CombinationsNmin    = params->GetValue ("Electron1Tau2CombinationsNmin", 1);
  _Electron1Tau2CombinationsNmax    = params->GetValue ("Electron1Tau2CombinationsNmax", 1);
  _Electron2Tau1CombinationsNmin    = params->GetValue ("Electron2Tau1CombinationsNmin", 1);
  _Electron2Tau1CombinationsNmax    = params->GetValue ("Electron2Tau1CombinationsNmax", 1);
  _Electron2Tau2CombinationsNmin    = params->GetValue ("Electron2Tau2CombinationsNmin", 1);
  _Electron2Tau2CombinationsNmax    = params->GetValue ("Electron2Tau2CombinationsNmax", 1);
  _RecoTriggers2Nmin                = params->GetValue ("RecoTriggers2Nmin", 1);
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
  _JetPtForMhtAndHt                  = params->GetValue ("JetPtForMhtAndHt", 1.0);
  _JetEtaForMhtAndHt                 = params->GetValue ("JetEtaForMhtAndHt", 1.0);
  _ApplyJetLooseIDforMhtAndHt        = params->GetValue ("ApplyJetLooseIDforMhtAndHt", "string");
  _ApplyCentralJetLooseID            = params->GetValue ("ApplyCentralJetLooseID", "string");
  _RecoCentralJetPtCut               = params->GetValue ("RecoCentralJetPtCut", 1.0);
  _RemoveCentralJetOverlapWithMuon1s = params->GetValue ("RemoveCentralJetOverlapWithMuon1s", "string");
  _RemoveCentralJetOverlapWithMuon2s = params->GetValue ("RemoveCentralJetOverlapWithMuon2s", "string");
  _RemoveCentralJetOverlapWithElectron1s = params->GetValue ("RemoveCentralJetOverlapWithElectron1s", "string");
  _RemoveCentralJetOverlapWithElectron2s = params->GetValue ("RemoveCentralJetOverlapWithElectron2s", "string");
  _RemoveCentralJetOverlapWithTau1s = params->GetValue ("RemoveCentralJetOverlapWithTau1s", "string");
  _RemoveCentralJetOverlapWithTau2s = params->GetValue ("RemoveCentralJetOverlapWithTau2s", "string");
  _CentralJetMuon1MatchingDeltaR             = params->GetValue ("CentralJetMuon1MatchingDeltaR", 1.0);
  _CentralJetMuon2MatchingDeltaR             = params->GetValue ("CentralJetMuon2MatchingDeltaR", 1.0);
  _CentralJetElectron1MatchingDeltaR         = params->GetValue ("CentralJetElectron1MatchingDeltaR", 1.0);
  _CentralJetElectron2MatchingDeltaR         = params->GetValue ("CentralJetElectron2MatchingDeltaR", 1.0);
  _CentralJetTau1MatchingDeltaR              = params->GetValue ("CentralJetTau1MatchingDeltaR", 1.0);
  _CentralJetTau2MatchingDeltaR              = params->GetValue ("CentralJetTau2MatchingDeltaR", 1.0);
  _ApplyLeadingJetsLooseID           = params->GetValue ("ApplyLeadingJetsLooseID", "string");
  _DoDiscrByFirstLeadingJet          = params->GetValue ("DoDiscrByFirstLeadingJet", "string");
  _RecoFirstLeadingJetEtaMaxCut      = params->GetValue ("RecoFirstLeadingJetEtaMaxCut", 1.0);
  _RecoFirstLeadingJetEtaMinCut      = params->GetValue ("RecoFirstLeadingJetEtaMinCut", 1.0);
  _RecoFirstLeadingJetPt             = params->GetValue ("RecoFirstLeadingJetPt", 1.0);
  _RemoveFirstLeadingJetOverlapWithMuon1s = params->GetValue ("RemoveFirstLeadingJetOverlapWithMuon1s", "string");
  _RemoveFirstLeadingJetOverlapWithMuon2s = params->GetValue ("RemoveFirstLeadingJetOverlapWithMuon2s", "string");
  _RemoveFirstLeadingJetOverlapWithElectron1s = params->GetValue ("RemoveFirstLeadingJetOverlapWithElectron1s", "string");
  _RemoveFirstLeadingJetOverlapWithElectron2s = params->GetValue ("RemoveFirstLeadingJetOverlapWithElectron2s", "string");
  _RemoveFirstLeadingJetOverlapWithTau1s = params->GetValue ("RemoveFirstLeadingJetOverlapWithTau1s", "string");
  _RemoveFirstLeadingJetOverlapWithTau2s = params->GetValue ("RemoveFirstLeadingJetOverlapWithTau2s", "string");
  _FirstLeadingJetMuon1MatchingDeltaR             = params->GetValue ("FirstLeadingJetMuon1MatchingDeltaR", 1.0);
  _FirstLeadingJetMuon2MatchingDeltaR             = params->GetValue ("FirstLeadingJetMuon2MatchingDeltaR", 1.0);
  _FirstLeadingJetElectron1MatchingDeltaR         = params->GetValue ("FirstLeadingJetElectron1MatchingDeltaR", 1.0);
  _FirstLeadingJetElectron2MatchingDeltaR         = params->GetValue ("FirstLeadingJetElectron2MatchingDeltaR", 1.0);
  _FirstLeadingJetTau1MatchingDeltaR              = params->GetValue ("FirstLeadingJetTau1MatchingDeltaR", 1.0);
  _FirstLeadingJetTau2MatchingDeltaR              = params->GetValue ("FirstLeadingJetTau2MatchingDeltaR", 1.0);
  _DoDiscrBySecondLeadingJet          = params->GetValue ("DoDiscrBySecondLeadingJet", "string");
  _RecoSecondLeadingJetEtaMaxCut      = params->GetValue ("RecoSecondLeadingJetEtaMaxCut", 1.0);
  _RecoSecondLeadingJetEtaMinCut      = params->GetValue ("RecoSecondLeadingJetEtaMinCut", 1.0);
  _RecoSecondLeadingJetPt             = params->GetValue ("RecoSecondLeadingJetPt", 1.0);
  _RemoveSecondLeadingJetOverlapWithMuon1s = params->GetValue ("RemoveSecondLeadingJetOverlapWithMuon1s", "string");
  _RemoveSecondLeadingJetOverlapWithMuon2s = params->GetValue ("RemoveSecondLeadingJetOverlapWithMuon2s", "string");
  _RemoveSecondLeadingJetOverlapWithElectron1s = params->GetValue ("RemoveSecondLeadingJetOverlapWithElectron1s", "string");
  _RemoveSecondLeadingJetOverlapWithElectron2s = params->GetValue ("RemoveSecondLeadingJetOverlapWithElectron2s", "string");
  _RemoveSecondLeadingJetOverlapWithTau1s = params->GetValue ("RemoveSecondLeadingJetOverlapWithTau1s", "string");
  _RemoveSecondLeadingJetOverlapWithTau2s = params->GetValue ("RemoveSecondLeadingJetOverlapWithTau2s", "string");
  _SecondLeadingJetMuon1MatchingDeltaR             = params->GetValue ("SecondLeadingJetMuon1MatchingDeltaR", 1.0);
  _SecondLeadingJetMuon2MatchingDeltaR             = params->GetValue ("SecondLeadingJetMuon2MatchingDeltaR", 1.0);
  _SecondLeadingJetElectron1MatchingDeltaR         = params->GetValue ("SecondLeadingJetElectron1MatchingDeltaR", 1.0);
  _SecondLeadingJetElectron2MatchingDeltaR         = params->GetValue ("SecondLeadingJetElectron2MatchingDeltaR", 1.0);
  _SecondLeadingJetTau1MatchingDeltaR              = params->GetValue ("SecondLeadingJetTau1MatchingDeltaR", 1.0);
  _SecondLeadingJetTau2MatchingDeltaR              = params->GetValue ("SecondLeadingJetTau2MatchingDeltaR", 1.0);
  _RecoBJetEtaMaxCut                 = params->GetValue ("RecoBJetEtaMaxCut", 1.0);
  _RecoBJetEtaMinCut                 = params->GetValue ("RecoBJetEtaMinCut", 1.0);
  _RecoBJetPtCut                     = params->GetValue ("RecoBJetPtCut", 1.0);
  _RemoveBJetOverlapWithMuon1s = params->GetValue ("RemoveBJetOverlapWithMuon1s", "string");
  _RemoveBJetOverlapWithMuon2s = params->GetValue ("RemoveBJetOverlapWithMuon2s", "string");
  _RemoveBJetOverlapWithElectron1s = params->GetValue ("RemoveBJetOverlapWithElectron1s", "string");
  _RemoveBJetOverlapWithElectron2s = params->GetValue ("RemoveBJetOverlapWithElectron2s", "string");
  _RemoveBJetOverlapWithTau1s = params->GetValue ("RemoveBJetOverlapWithTau1s", "string");
  _RemoveBJetOverlapWithTau2s = params->GetValue ("RemoveBJetOverlapWithTau2s", "string");
  _BJetMuon1MatchingDeltaR             = params->GetValue ("BJetMuon1MatchingDeltaR", 1.0);
  _BJetMuon2MatchingDeltaR             = params->GetValue ("BJetMuon2MatchingDeltaR", 1.0);
  _BJetElectron1MatchingDeltaR         = params->GetValue ("BJetElectron1MatchingDeltaR", 1.0);
  _BJetElectron2MatchingDeltaR         = params->GetValue ("BJetElectron2MatchingDeltaR", 1.0);
  _BJetTau1MatchingDeltaR              = params->GetValue ("BJetTau1MatchingDeltaR", 1.0);
  _BJetTau2MatchingDeltaR              = params->GetValue ("BJetTau2MatchingDeltaR", 1.0);
  _ApplyJetBTagging                    = params->GetValue ("ApplyJetBTagging", "string");
  _JetBTaggingCut                      = params->GetValue ("JetBTaggingCut", 1.0);
  _DoSUSYDiscrByLeadDiJetMass        = params->GetValue ("DoSUSYDiscrByLeadDiJetMass", "string");
  _LeadDiJetMinMassCut               = params->GetValue ("LeadDiJetMinMassCut", 1.0);
  _LeadDiJetMaxMassCut               = params->GetValue ("LeadDiJetMaxMassCut", 1.0);
  _DoSUSYDiscrByLeadDiJetPt          = params->GetValue ("DoSUSYDiscrByLeadDiJetPt", "string");
  _LeadDiJetMinPtCut                 = params->GetValue ("LeadDiJetMinPtCut", 1.0);
  _LeadDiJetMaxPtCut                 = params->GetValue ("LeadDiJetMaxPtCut", 1.0);
  _DoSUSYDiscrByLeadDiJetDeltaEta    = params->GetValue ("DoSUSYDiscrByLeadDiJetDeltaEta", "string");
  _LeadDiJetMinDeltaEtaCut           = params->GetValue ("LeadDiJetMinDeltaEtaCut", 1.0);
  _LeadDiJetMaxDeltaEtaCut           = params->GetValue ("LeadDiJetMaxDeltaEtaCut", 1.0);
  _DoSUSYDiscrByLeadDiJetDeltaPhi    = params->GetValue ("DoSUSYDiscrByLeadDiJetDeltaPhi", "string");
  _LeadDiJetMinDeltaPhiCut           = params->GetValue ("LeadDiJetMinDeltaPhiCut", 1.0);
  _LeadDiJetMaxDeltaPhiCut           = params->GetValue ("LeadDiJetMaxDeltaPhiCut", 1.0);
  _DoSUSYDiscrByLeadDiJetOSEta       = params->GetValue ("DoSUSYDiscrByLeadDiJetOSEta", "string");
  _DoDiscrByMet                      = params->GetValue ("DoDiscrByMet", "string");
  _RecoMetMinCut                     = params->GetValue ("RecoMetMinCut", 1.0);
  _RecoMetMaxCut                     = params->GetValue ("RecoMetMaxCut", 1.0);
  _DoSUSYDiscrByMHT                  = params->GetValue ("DoSUSYDiscrByMHT", "string");
  _MhtCut                            = params->GetValue ("MhtCut", 1.0);
  _DoSUSYDiscrByHT                   = params->GetValue ("DoSUSYDiscrByHT", "string");
  _HtCut                             = params->GetValue ("HtCut", 1.0);
  _DoSUSYDiscrByR1                   = params->GetValue ("DoSUSYDiscrByR1", "string");
  _R1MinCut                          = params->GetValue ("R1MinCut", 1.0);
  _R1MaxCut                          = params->GetValue ("R1MaxCut", 1.0);
  _DoSUSYDiscrByR2                   = params->GetValue ("DoSUSYDiscrByR2", "string");
  _R2MinCut                          = params->GetValue ("R2MinCut", 1.0);
  _R2MaxCut                          = params->GetValue ("R2MaxCut", 1.0);
  _DoSUSYDiscrByAlpha                = params->GetValue ("DoSUSYDiscrByAlpha", "string");
  _AlphaMinCut                       = params->GetValue ("AlphaMinCut", 1.0);
  _AlphaMaxCut                       = params->GetValue ("AlphaMaxCut", 1.0);
  _DoSUSYDiscrByDphi1                = params->GetValue ("DoSUSYDiscrByDphi1", "string");
  _Dphi1MinCut                       = params->GetValue ("Dphi1MinCut", 1.0);
  _Dphi1MaxCut                       = params->GetValue ("Dphi1MaxCut", 1.0);
  _DoSUSYDiscrByDphi2                = params->GetValue ("DoSUSYDiscrByDphi2", "string");
  _Dphi2MinCut                       = params->GetValue ("Dphi2MinCut", 1.0);
  _Dphi2MaxCut                       = params->GetValue ("Dphi2MaxCut", 1.0);
  _DoMuon1DiscrByIsZllCut            = params->GetValue ("DoMuon1DiscrByIsZllCut", "string");
  _DoDiscrByMuon1MetDphi             = params->GetValue ("DoDiscrByMuon1MetDphi", "string");
  _Muon1MetDphiMaxCut                = params->GetValue ("Muon1MetDphiMaxCut", 1.0);
  _Muon1MetDphiMinCut                = params->GetValue ("Muon1MetDphiMinCut", 1.0);
  _DoDiscrByMuon1MetMt               = params->GetValue ("DoDiscrByMuon1MetMt", "string");
  _Muon1MetMtMinCut                  = params->GetValue ("Muon1MetMtMinCut", 1.0);
  _Muon1MetMtMaxCut                  = params->GetValue ("Muon1MetMtMaxCut", 1.0);
  _DoMuon2DiscrByIsZllCut            = params->GetValue ("DoMuon2DiscrByIsZllCut", "string");
  _DoDiscrByMuon2MetDphi             = params->GetValue ("DoDiscrByMuon2MetDphi", "string");
  _Muon2MetDphiMaxCut                = params->GetValue ("Muon2MetDphiMaxCut", 1.0);
  _Muon2MetDphiMinCut                = params->GetValue ("Muon2MetDphiMinCut", 1.0);
  _DoDiscrByMuon2MetMt               = params->GetValue ("DoDiscrByMuon2MetMt", "string");
  _Muon2MetMtMinCut                  = params->GetValue ("Muon2MetMtMinCut", 1.0);
  _Muon2MetMtMaxCut                  = params->GetValue ("Muon2MetMtMaxCut", 1.0);
  _DoElectron1DiscrByIsZllCut        = params->GetValue ("DoElectron1DiscrByIsZllCut", "string");
  _DoDiscrByElectron1MetDphi         = params->GetValue ("DoDiscrByElectron1MetDphi", "string");
  _Electron1MetDphiMaxCut            = params->GetValue ("Electron1MetDphiMaxCut", 1.0);
  _Electron1MetDphiMinCut            = params->GetValue ("Electron1MetDphiMinCut", 1.0);
  _DoDiscrByElectron1MetMt           = params->GetValue ("DoDiscrByElectron1MetMt", "string");
  _Electron1MetMtMinCut              = params->GetValue ("Electron1MetMtMinCut", 1.0);
  _Electron1MetMtMaxCut              = params->GetValue ("Electron1MetMtMaxCut", 1.0);
  _DoElectron2DiscrByIsZllCut        = params->GetValue ("DoElectron2DiscrByIsZllCut", "string");
  _DoDiscrByElectron2MetDphi         = params->GetValue ("DoDiscrByElectron2MetDphi", "string");
  _Electron2MetDphiMaxCut            = params->GetValue ("Electron2MetDphiMaxCut", 1.0);
  _Electron2MetDphiMinCut            = params->GetValue ("Electron2MetDphiMinCut", 1.0);
  _DoDiscrByElectron2MetMt           = params->GetValue ("DoDiscrByElectron2MetMt", "string");
  _Electron2MetMtMinCut              = params->GetValue ("Electron2MetMtMinCut", 1.0);
  _Electron2MetMtMaxCut              = params->GetValue ("Electron2MetMtMaxCut", 1.0);
  _DoDiscrByTau1MetDphi             = params->GetValue ("DoDiscrByTau1MetDphi", "string");
  _Tau1MetDphiMaxCut                = params->GetValue ("Tau1MetDphiMaxCut", 1.0);
  _Tau1MetDphiMinCut                = params->GetValue ("Tau1MetDphiMinCut", 1.0);
  _DoDiscrByTau1MetMt               = params->GetValue ("DoDiscrByTau1MetMt", "string");
  _Tau1MetMtMinCut                  = params->GetValue ("Tau1MetMtMinCut", 1.0);
  _Tau1MetMtMaxCut                  = params->GetValue ("Tau1MetMtMaxCut", 1.0);
  _DoDiscrByTau2MetDphi             = params->GetValue ("DoDiscrByTau2MetDphi", "string");
  _Tau2MetDphiMaxCut                = params->GetValue ("Tau2MetDphiMaxCut", 1.0);
  _Tau2MetDphiMinCut                = params->GetValue ("Tau2MetDphiMinCut", 1.0);
  _DoDiscrByTau2MetMt               = params->GetValue ("DoDiscrByTau2MetMt", "string");
  _Tau2MetMtMinCut                  = params->GetValue ("Tau2MetMtMinCut", 1.0);
  _Tau2MetMtMaxCut                  = params->GetValue ("Tau2MetMtMaxCut", 1.0);
  _DoDiTauDiscrByDeltaR             = params->GetValue ("DoDiTauDiscrByDeltaR", "string");
  _DiTauDeltaRCut                   = params->GetValue ("DiTauDeltaRCut", 1.0);
  _DiTauDiscrByOSLSType             = params->GetValue ("DiTauDiscrByOSLSType", "string");
  _DoDiTauDiscrByCosDphi            = params->GetValue ("DoDiTauDiscrByCosDphi", "string");
  _DiTauCosDphiMaxCut               = params->GetValue ("DiTauCosDphiMaxCut", 1.0);
  _DiTauCosDphiMinCut               = params->GetValue ("DiTauCosDphiMinCut", 1.0);
  _DoDiscrByDiTauMassReco           = params->GetValue ("DoDiscrByDiTauMassReco", "string");
  _UseVectorSumOfDiTauProductsAndMetMassReco = params->GetValue ("UseVectorSumOfDiTauProductsAndMetMassReco", "string");
  _UseCollinerApproxDiTauMassReco   = params->GetValue ("UseCollinerApproxDiTauMassReco", "string");
  _DiTauMassMinCut                  = params->GetValue ("DiTauMassMinCut", 1.0);
  _DiTauMassMaxCut                  = params->GetValue ("DiTauMassMaxCut", 1.0);
  _DoDiTauDiscrByCDFzeta2D          = params->GetValue ("DoDiTauDiscrByCDFzeta2D", "string");
  _DiTauPZetaCutCoefficient         = params->GetValue ("DiTauPZetaCutCoefficient", 1.0);
  _DiTauPZetaVisCutCoefficient      = params->GetValue ("DiTauPZetaVisCutCoefficient", 1.0);
  _DiTauCDFzeta2DMinCutValue        = params->GetValue ("DiTauCDFzeta2DMinCutValue", 1.0);
  _DiTauCDFzeta2DMaxCutValue        = params->GetValue ("DiTauCDFzeta2DMaxCutValue", 1.0);
  _DoDiTauDiscrByDeltaPtDivSumPt    = params->GetValue ("DoDiTauDiscrByDeltaPtDivSumPt", "string");
  _DiTauDeltaPtDivSumPtMinCutValue  = params->GetValue ("DiTauDeltaPtDivSumPtMinCutValue", 1.0);
  _DiTauDeltaPtDivSumPtMaxCutValue  = params->GetValue ("DiTauDeltaPtDivSumPtMaxCutValue", 1.0);
  _DoDiTauDiscrByDeltaPt            = params->GetValue ("DoDiTauDiscrByDeltaPt", "string");
  _DiTauDeltaPtMinCutValue          = params->GetValue ("DiTauDeltaPtMinCutValue", 1.0);
  _DiTauDeltaPtMaxCutValue          = params->GetValue ("DiTauDeltaPtMaxCutValue", 1.0);
  _DoDiMuonDiscrByDeltaR             = params->GetValue ("DoDiMuonDiscrByDeltaR", "string");
  _DiMuonDeltaRCut                   = params->GetValue ("DiMuonDeltaRCut", 1.0);
  _DiMuonDiscrByOSLSType             = params->GetValue ("DiMuonDiscrByOSLSType", "string");
  _DoDiMuonDiscrByCosDphi            = params->GetValue ("DoDiMuonDiscrByCosDphi", "string");
  _DiMuonCosDphiMaxCut               = params->GetValue ("DiMuonCosDphiMaxCut", 1.0);
  _DiMuonCosDphiMinCut               = params->GetValue ("DiMuonCosDphiMinCut", 1.0);
  _DoDiscrByDiMuonMassReco           = params->GetValue ("DoDiscrByDiMuonMassReco", "string");
  _UseVectorSumOfDiMuonProductsAndMetMassReco = params->GetValue ("UseVectorSumOfDiMuonProductsAndMetMassReco", "string");
  _UseCollinerApproxDiMuonMassReco   = params->GetValue ("UseCollinerApproxDiMuonMassReco", "string");
  _DiMuonMassMinCut                  = params->GetValue ("DiMuonMassMinCut", 1.0);
  _DiMuonMassMaxCut                  = params->GetValue ("DiMuonMassMaxCut", 1.0);
  _DoDiMuonDiscrByCDFzeta2D          = params->GetValue ("DoDiMuonDiscrByCDFzeta2D", "string");
  _DiMuonPZetaCutCoefficient         = params->GetValue ("DiMuonPZetaCutCoefficient", 1.0);
  _DiMuonPZetaVisCutCoefficient      = params->GetValue ("DiMuonPZetaVisCutCoefficient", 1.0);
  _DiMuonCDFzeta2DMinCutValue        = params->GetValue ("DiMuonCDFzeta2DMinCutValue", 1.0);
  _DiMuonCDFzeta2DMaxCutValue        = params->GetValue ("DiMuonCDFzeta2DMaxCutValue", 1.0);
  _DoDiMuonDiscrByDeltaPtDivSumPt    = params->GetValue ("DoDiMuonDiscrByDeltaPtDivSumPt", "string");
  _DiMuonDeltaPtDivSumPtMinCutValue  = params->GetValue ("DiMuonDeltaPtDivSumPtMinCutValue", 1.0);
  _DiMuonDeltaPtDivSumPtMaxCutValue  = params->GetValue ("DiMuonDeltaPtDivSumPtMaxCutValue", 1.0);
  _DoDiMuonDiscrByDeltaPt            = params->GetValue ("DoDiMuonDiscrByDeltaPt", "string");
  _DiMuonDeltaPtMinCutValue          = params->GetValue ("DiMuonDeltaPtMinCutValue", 1.0);
  _DiMuonDeltaPtMaxCutValue          = params->GetValue ("DiMuonDeltaPtMaxCutValue", 1.0);
  _DoDiElectronDiscrByDeltaR             = params->GetValue ("DoDiElectronDiscrByDeltaR", "string");
  _DiElectronDeltaRCut                   = params->GetValue ("DiElectronDeltaRCut", 1.0);
  _DiElectronDiscrByOSLSType             = params->GetValue ("DiElectronDiscrByOSLSType", "string");
  _DoDiElectronDiscrByCosDphi            = params->GetValue ("DoDiElectronDiscrByCosDphi", "string");
  _DiElectronCosDphiMaxCut               = params->GetValue ("DiElectronCosDphiMaxCut", 1.0);
  _DiElectronCosDphiMinCut               = params->GetValue ("DiElectronCosDphiMinCut", 1.0);
  _DoDiscrByDiElectronMassReco           = params->GetValue ("DoDiscrByDiElectronMassReco", "string");
  _UseVectorSumOfDiElectronProductsAndMetMassReco = params->GetValue ("UseVectorSumOfDiElectronProductsAndMetMassReco", "string");
  _UseCollinerApproxDiElectronMassReco   = params->GetValue ("UseCollinerApproxDiElectronMassReco", "string");
  _DiElectronMassMinCut                  = params->GetValue ("DiElectronMassMinCut", 1.0);
  _DiElectronMassMaxCut                  = params->GetValue ("DiElectronMassMaxCut", 1.0);
  _DoDiElectronDiscrByCDFzeta2D          = params->GetValue ("DoDiElectronDiscrByCDFzeta2D", "string");
  _DiElectronPZetaCutCoefficient         = params->GetValue ("DiElectronPZetaCutCoefficient", 1.0);
  _DiElectronPZetaVisCutCoefficient      = params->GetValue ("DiElectronPZetaVisCutCoefficient", 1.0);
  _DiElectronCDFzeta2DMinCutValue        = params->GetValue ("DiElectronCDFzeta2DMinCutValue", 1.0);
  _DiElectronCDFzeta2DMaxCutValue        = params->GetValue ("DiElectronCDFzeta2DMaxCutValue", 1.0);
  _DoDiElectronDiscrByDeltaPtDivSumPt    = params->GetValue ("DoDiElectronDiscrByDeltaPtDivSumPt", "string");
  _DiElectronDeltaPtDivSumPtMinCutValue  = params->GetValue ("DiElectronDeltaPtDivSumPtMinCutValue", 1.0);
  _DiElectronDeltaPtDivSumPtMaxCutValue  = params->GetValue ("DiElectronDeltaPtDivSumPtMaxCutValue", 1.0);
  _DoDiElectronDiscrByDeltaPt            = params->GetValue ("DoDiElectronDiscrByDeltaPt", "string");
  _DiElectronDeltaPtMinCutValue          = params->GetValue ("DiElectronDeltaPtMinCutValue", 1.0);
  _DiElectronDeltaPtMaxCutValue          = params->GetValue ("DiElectronDeltaPtMaxCutValue", 1.0);
  _DoDiJetDiscrByDeltaR                  = params->GetValue ("DoDiJetDiscrByDeltaR", "string");
  _DiJetDeltaRCut                        = params->GetValue ("DiJetDeltaRCut", 1.0);
  _DoDiJetDiscrByDeltaEta                = params->GetValue ("DoDiJetDiscrByDeltaEta", "string");
  _DiJetMinDeltaEtaCut                   = params->GetValue ("DiJetMinDeltaEtaCut", 1.0);
  _DiJetMaxDeltaEtaCut                   = params->GetValue ("DiJetMaxDeltaEtaCut", 1.0);
  _DoDiJetDiscrByDeltaPhi                = params->GetValue ("DoDiJetDiscrByDeltaPhi", "string");
  _DiJetMinDeltaPhiCut                   = params->GetValue ("DiJetMinDeltaPhiCut", 1.0);
  _DiJetMaxDeltaPhiCut                   = params->GetValue ("DiJetMaxDeltaPhiCut", 1.0);
  _DoDiJetDiscrByOSEta                   = params->GetValue ("DoDiJetDiscrByOSEta", "string");
  _DoDiJetDiscrByCosDphi                 = params->GetValue ("DoDiJetDiscrByCosDphi", "string");
  _DiJetCosDphiMaxCut                    = params->GetValue ("DiJetCosDphiMaxCut", 1.0);
  _DiJetCosDphiMinCut                    = params->GetValue ("DiJetCosDphiMinCut", 1.0);
  _DoDiscrByDiJetMassReco                = params->GetValue ("DoDiscrByDiJetMassReco", "string");
  _DiJetMassMinCut                       = params->GetValue ("DiJetMassMinCut", 1.0);
  _DiJetMassMaxCut                       = params->GetValue ("DiJetMassMaxCut", 1.0);
  _DoMuon1Tau1DiscrByDeltaR         = params->GetValue ("DoMuon1Tau1DiscrByDeltaR", "string");
  _Muon1Tau1DeltaRCut               = params->GetValue ("Muon1Tau1DeltaRCut", 1.0);
  _Muon1Tau1DiscrByOSLSType         = params->GetValue ("Muon1Tau1DiscrByOSLSType", "string");
  _DoMuon1Tau1DiscrByCosDphi        = params->GetValue ("DoMuon1Tau1DiscrByCosDphi", "string");
  _Muon1Tau1CosDphiMaxCut           = params->GetValue ("Muon1Tau1CosDphiMaxCut", 1.0);
  _Muon1Tau1CosDphiMinCut           = params->GetValue ("Muon1Tau1CosDphiMinCut", 1.0);
  _DoDiscrByMuon1Tau1MassReco       = params->GetValue ("DoDiscrByMuon1Tau1MassReco", "string");
  _UseVectorSumOfMuon1Tau1ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfMuon1Tau1ProductsAndMetMassReco", "string");
  _UseCollinerApproxMuon1Tau1MassReco   = params->GetValue ("UseCollinerApproxMuon1Tau1MassReco", "string");
  _Muon1Tau1MassMinCut              = params->GetValue ("Muon1Tau1MassMinCut", 1.0);
  _Muon1Tau1MassMaxCut              = params->GetValue ("Muon1Tau1MassMaxCut", 1.0);
  _DoMuon1Tau1DiscrByCDFzeta2D      = params->GetValue ("DoMuon1Tau1DiscrByCDFzeta2D", "string");
  _Muon1Tau1PZetaCutCoefficient     = params->GetValue ("Muon1Tau1PZetaCutCoefficient", 1.0);
  _Muon1Tau1PZetaVisCutCoefficient  = params->GetValue ("Muon1Tau1PZetaVisCutCoefficient", 1.0);
  _Muon1Tau1CDFzeta2DMinCutValue    = params->GetValue ("Muon1Tau1CDFzeta2DMinCutValue", 1.0);
  _Muon1Tau1CDFzeta2DMaxCutValue    = params->GetValue ("Muon1Tau1CDFzeta2DMaxCutValue", 1.0);
  _DoMuon1Tau1DiscrByDeltaPtDivSumPt = params->GetValue ("DoMuon1Tau1DiscrByDeltaPtDivSumPt", "string");
  _Muon1Tau1DeltaPtDivSumPtMinCutValue  = params->GetValue ("Muon1Tau1DeltaPtDivSumPtMinCutValue", 1.0);
  _Muon1Tau1DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Muon1Tau1DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoMuon1Tau1DiscrByDeltaPt        = params->GetValue ("DoMuon1Tau1DiscrByDeltaPt", "string");
  _Muon1Tau1DeltaPtMinCutValue      = params->GetValue ("Muon1Tau1DeltaPtMinCutValue", 1.0);
  _Muon1Tau1DeltaPtMaxCutValue      = params->GetValue ("Muon1Tau1DeltaPtMaxCutValue", 1.0);
  _DoMuon1Tau2DiscrByDeltaR         = params->GetValue ("DoMuon1Tau2DiscrByDeltaR", "string");
  _Muon1Tau2DeltaRCut               = params->GetValue ("Muon1Tau2DeltaRCut", 1.0);
  _Muon1Tau2DiscrByOSLSType         = params->GetValue ("Muon1Tau2DiscrByOSLSType", "string");
  _DoMuon1Tau2DiscrByCosDphi        = params->GetValue ("DoMuon1Tau2DiscrByCosDphi", "string");
  _Muon1Tau2CosDphiMaxCut           = params->GetValue ("Muon1Tau2CosDphiMaxCut", 1.0);
  _Muon1Tau2CosDphiMinCut           = params->GetValue ("Muon1Tau2CosDphiMinCut", 1.0);
  _DoDiscrByMuon1Tau2MassReco       = params->GetValue ("DoDiscrByMuon1Tau2MassReco", "string");
  _UseVectorSumOfMuon1Tau2ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfMuon1Tau2ProductsAndMetMassReco", "string");
  _UseCollinerApproxMuon1Tau2MassReco   = params->GetValue ("UseCollinerApproxMuon1Tau2MassReco", "string");
  _Muon1Tau2MassMinCut              = params->GetValue ("Muon1Tau2MassMinCut", 1.0);
  _Muon1Tau2MassMaxCut              = params->GetValue ("Muon1Tau2MassMaxCut", 1.0);
  _DoMuon1Tau2DiscrByCDFzeta2D      = params->GetValue ("DoMuon1Tau2DiscrByCDFzeta2D", "string");
  _Muon1Tau2PZetaCutCoefficient     = params->GetValue ("Muon1Tau2PZetaCutCoefficient", 1.0);
  _Muon1Tau2PZetaVisCutCoefficient  = params->GetValue ("Muon1Tau2PZetaVisCutCoefficient", 1.0);
  _Muon1Tau2CDFzeta2DMinCutValue    = params->GetValue ("Muon1Tau2CDFzeta2DMinCutValue", 1.0);
  _Muon1Tau2CDFzeta2DMaxCutValue    = params->GetValue ("Muon1Tau2CDFzeta2DMaxCutValue", 1.0);
  _DoMuon1Tau2DiscrByDeltaPtDivSumPt = params->GetValue ("DoMuon1Tau2DiscrByDeltaPtDivSumPt", "string");
  _Muon1Tau2DeltaPtDivSumPtMinCutValue  = params->GetValue ("Muon1Tau2DeltaPtDivSumPtMinCutValue", 1.0);
  _Muon1Tau2DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Muon1Tau2DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoMuon1Tau2DiscrByDeltaPt        = params->GetValue ("DoMuon1Tau2DiscrByDeltaPt", "string");
  _Muon1Tau2DeltaPtMinCutValue      = params->GetValue ("Muon1Tau2DeltaPtMinCutValue", 1.0);
  _Muon1Tau2DeltaPtMaxCutValue      = params->GetValue ("Muon1Tau2DeltaPtMaxCutValue", 1.0);
  _DoMuon2Tau1DiscrByDeltaR         = params->GetValue ("DoMuon2Tau1DiscrByDeltaR", "string");
  _Muon2Tau1DeltaRCut               = params->GetValue ("Muon2Tau1DeltaRCut", 1.0);
  _Muon2Tau1DiscrByOSLSType         = params->GetValue ("Muon2Tau1DiscrByOSLSType", "string");
  _DoMuon2Tau1DiscrByCosDphi        = params->GetValue ("DoMuon2Tau1DiscrByCosDphi", "string");
  _Muon2Tau1CosDphiMaxCut           = params->GetValue ("Muon2Tau1CosDphiMaxCut", 1.0);
  _Muon2Tau1CosDphiMinCut           = params->GetValue ("Muon2Tau1CosDphiMinCut", 1.0);
  _DoDiscrByMuon2Tau1MassReco       = params->GetValue ("DoDiscrByMuon2Tau1MassReco", "string");
  _UseVectorSumOfMuon2Tau1ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfMuon2Tau1ProductsAndMetMassReco", "string");
  _UseCollinerApproxMuon2Tau1MassReco   = params->GetValue ("UseCollinerApproxMuon2Tau1MassReco", "string");
  _Muon2Tau1MassMinCut              = params->GetValue ("Muon2Tau1MassMinCut", 1.0);
  _Muon2Tau1MassMaxCut              = params->GetValue ("Muon2Tau1MassMaxCut", 1.0);
  _DoMuon2Tau1DiscrByCDFzeta2D      = params->GetValue ("DoMuon2Tau1DiscrByCDFzeta2D", "string");
  _Muon2Tau1PZetaCutCoefficient     = params->GetValue ("Muon2Tau1PZetaCutCoefficient", 1.0);
  _Muon2Tau1PZetaVisCutCoefficient  = params->GetValue ("Muon2Tau1PZetaVisCutCoefficient", 1.0);
  _Muon2Tau1CDFzeta2DMinCutValue    = params->GetValue ("Muon2Tau1CDFzeta2DMinCutValue", 1.0);
  _Muon2Tau1CDFzeta2DMaxCutValue    = params->GetValue ("Muon2Tau1CDFzeta2DMaxCutValue", 1.0);
  _DoMuon2Tau1DiscrByDeltaPtDivSumPt = params->GetValue ("DoMuon2Tau1DiscrByDeltaPtDivSumPt", "string");
  _Muon2Tau1DeltaPtDivSumPtMinCutValue  = params->GetValue ("Muon2Tau1DeltaPtDivSumPtMinCutValue", 1.0);
  _Muon2Tau1DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Muon2Tau1DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoMuon2Tau1DiscrByDeltaPt        = params->GetValue ("DoMuon2Tau1DiscrByDeltaPt", "string");
  _Muon2Tau1DeltaPtMinCutValue      = params->GetValue ("Muon2Tau1DeltaPtMinCutValue", 1.0);
  _Muon2Tau1DeltaPtMaxCutValue      = params->GetValue ("Muon2Tau1DeltaPtMaxCutValue", 1.0);
  _DoMuon2Tau2DiscrByDeltaR         = params->GetValue ("DoMuon2Tau2DiscrByDeltaR", "string");
  _Muon2Tau2DeltaRCut               = params->GetValue ("Muon2Tau2DeltaRCut", 1.0);
  _Muon2Tau2DiscrByOSLSType         = params->GetValue ("Muon2Tau2DiscrByOSLSType", "string");
  _DoMuon2Tau2DiscrByCosDphi        = params->GetValue ("DoMuon2Tau2DiscrByCosDphi", "string");
  _Muon2Tau2CosDphiMaxCut           = params->GetValue ("Muon2Tau2CosDphiMaxCut", 1.0);
  _Muon2Tau2CosDphiMinCut           = params->GetValue ("Muon2Tau2CosDphiMinCut", 1.0);
  _DoDiscrByMuon2Tau2MassReco       = params->GetValue ("DoDiscrByMuon2Tau2MassReco", "string");
  _UseVectorSumOfMuon2Tau2ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfMuon2Tau2ProductsAndMetMassReco", "string");
  _UseCollinerApproxMuon2Tau2MassReco   = params->GetValue ("UseCollinerApproxMuon2Tau2MassReco", "string");
  _Muon2Tau2MassMinCut              = params->GetValue ("Muon2Tau2MassMinCut", 1.0);
  _Muon2Tau2MassMaxCut              = params->GetValue ("Muon2Tau2MassMaxCut", 1.0);
  _DoMuon2Tau2DiscrByCDFzeta2D      = params->GetValue ("DoMuon2Tau2DiscrByCDFzeta2D", "string");
  _Muon2Tau2PZetaCutCoefficient     = params->GetValue ("Muon2Tau2PZetaCutCoefficient", 1.0);
  _Muon2Tau2PZetaVisCutCoefficient  = params->GetValue ("Muon2Tau2PZetaVisCutCoefficient", 1.0);
  _Muon2Tau2CDFzeta2DMinCutValue    = params->GetValue ("Muon2Tau2CDFzeta2DMinCutValue", 1.0);
  _Muon2Tau2CDFzeta2DMaxCutValue    = params->GetValue ("Muon2Tau2CDFzeta2DMaxCutValue", 1.0);
  _DoMuon2Tau2DiscrByDeltaPtDivSumPt = params->GetValue ("DoMuon2Tau2DiscrByDeltaPtDivSumPt", "string");
  _Muon2Tau2DeltaPtDivSumPtMinCutValue  = params->GetValue ("Muon2Tau2DeltaPtDivSumPtMinCutValue", 1.0);
  _Muon2Tau2DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Muon2Tau2DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoMuon2Tau2DiscrByDeltaPt        = params->GetValue ("DoMuon2Tau2DiscrByDeltaPt", "string");
  _Muon2Tau2DeltaPtMinCutValue      = params->GetValue ("Muon2Tau2DeltaPtMinCutValue", 1.0);
  _Muon2Tau2DeltaPtMaxCutValue      = params->GetValue ("Muon2Tau2DeltaPtMaxCutValue", 1.0);
  _DoElectron1Tau1DiscrByDeltaR         = params->GetValue ("DoElectron1Tau1DiscrByDeltaR", "string");
  _Electron1Tau1DeltaRCut               = params->GetValue ("Electron1Tau1DeltaRCut", 1.0);
  _Electron1Tau1DiscrByOSLSType         = params->GetValue ("Electron1Tau1DiscrByOSLSType", "string");
  _DoElectron1Tau1DiscrByCosDphi        = params->GetValue ("DoElectron1Tau1DiscrByCosDphi", "string");
  _Electron1Tau1CosDphiMaxCut           = params->GetValue ("Electron1Tau1CosDphiMaxCut", 1.0);
  _Electron1Tau1CosDphiMinCut           = params->GetValue ("Electron1Tau1CosDphiMinCut", 1.0);
  _DoDiscrByElectron1Tau1MassReco       = params->GetValue ("DoDiscrByElectron1Tau1MassReco", "string");
  _UseVectorSumOfElectron1Tau1ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfElectron1Tau1ProductsAndMetMassReco", "string");
  _UseCollinerApproxElectron1Tau1MassReco   = params->GetValue ("UseCollinerApproxElectron1Tau1MassReco", "string");
  _Electron1Tau1MassMinCut              = params->GetValue ("Electron1Tau1MassMinCut", 1.0);
  _Electron1Tau1MassMaxCut              = params->GetValue ("Electron1Tau1MassMaxCut", 1.0);
  _DoElectron1Tau1DiscrByCDFzeta2D      = params->GetValue ("DoElectron1Tau1DiscrByCDFzeta2D", "string");
  _Electron1Tau1PZetaCutCoefficient     = params->GetValue ("Electron1Tau1PZetaCutCoefficient", 1.0);
  _Electron1Tau1PZetaVisCutCoefficient  = params->GetValue ("Electron1Tau1PZetaVisCutCoefficient", 1.0);
  _Electron1Tau1CDFzeta2DMinCutValue    = params->GetValue ("Electron1Tau1CDFzeta2DMinCutValue", 1.0);
  _Electron1Tau1CDFzeta2DMaxCutValue    = params->GetValue ("Electron1Tau1CDFzeta2DMaxCutValue", 1.0);
  _DoElectron1Tau1DiscrByDeltaPtDivSumPt = params->GetValue ("DoElectron1Tau1DiscrByDeltaPtDivSumPt", "string");
  _Electron1Tau1DeltaPtDivSumPtMinCutValue  = params->GetValue ("Electron1Tau1DeltaPtDivSumPtMinCutValue", 1.0);
  _Electron1Tau1DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Electron1Tau1DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoElectron1Tau1DiscrByDeltaPt        = params->GetValue ("DoElectron1Tau1DiscrByDeltaPt", "string");
  _Electron1Tau1DeltaPtMinCutValue      = params->GetValue ("Electron1Tau1DeltaPtMinCutValue", 1.0);
  _Electron1Tau1DeltaPtMaxCutValue      = params->GetValue ("Electron1Tau1DeltaPtMaxCutValue", 1.0);
  _DoElectron1Tau2DiscrByDeltaR         = params->GetValue ("DoElectron1Tau2DiscrByDeltaR", "string");
  _Electron1Tau2DeltaRCut               = params->GetValue ("Electron1Tau2DeltaRCut", 1.0);
  _Electron1Tau2DiscrByOSLSType         = params->GetValue ("Electron1Tau2DiscrByOSLSType", "string");
  _DoElectron1Tau2DiscrByCosDphi        = params->GetValue ("DoElectron1Tau2DiscrByCosDphi", "string");
  _Electron1Tau2CosDphiMaxCut           = params->GetValue ("Electron1Tau2CosDphiMaxCut", 1.0);
  _Electron1Tau2CosDphiMinCut           = params->GetValue ("Electron1Tau2CosDphiMinCut", 1.0);
  _DoDiscrByElectron1Tau2MassReco       = params->GetValue ("DoDiscrByElectron1Tau2MassReco", "string");
  _UseVectorSumOfElectron1Tau2ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfElectron1Tau2ProductsAndMetMassReco", "string");
  _UseCollinerApproxElectron1Tau2MassReco   = params->GetValue ("UseCollinerApproxElectron1Tau2MassReco", "string");
  _Electron1Tau2MassMinCut              = params->GetValue ("Electron1Tau2MassMinCut", 1.0);
  _Electron1Tau2MassMaxCut              = params->GetValue ("Electron1Tau2MassMaxCut", 1.0);
  _DoElectron1Tau2DiscrByCDFzeta2D      = params->GetValue ("DoElectron1Tau2DiscrByCDFzeta2D", "string");
  _Electron1Tau2PZetaCutCoefficient     = params->GetValue ("Electron1Tau2PZetaCutCoefficient", 1.0);
  _Electron1Tau2PZetaVisCutCoefficient  = params->GetValue ("Electron1Tau2PZetaVisCutCoefficient", 1.0);
  _Electron1Tau2CDFzeta2DMinCutValue    = params->GetValue ("Electron1Tau2CDFzeta2DMinCutValue", 1.0);
  _Electron1Tau2CDFzeta2DMaxCutValue    = params->GetValue ("Electron1Tau2CDFzeta2DMaxCutValue", 1.0);
  _DoElectron1Tau2DiscrByDeltaPtDivSumPt = params->GetValue ("DoElectron1Tau2DiscrByDeltaPtDivSumPt", "string");
  _Electron1Tau2DeltaPtDivSumPtMinCutValue  = params->GetValue ("Electron1Tau2DeltaPtDivSumPtMinCutValue", 1.0);
  _Electron1Tau2DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Electron1Tau2DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoElectron1Tau2DiscrByDeltaPt        = params->GetValue ("DoElectron1Tau2DiscrByDeltaPt", "string");
  _Electron1Tau2DeltaPtMinCutValue      = params->GetValue ("Electron1Tau2DeltaPtMinCutValue", 1.0);
  _Electron1Tau2DeltaPtMaxCutValue      = params->GetValue ("Electron1Tau2DeltaPtMaxCutValue", 1.0);
  _DoElectron2Tau1DiscrByDeltaR         = params->GetValue ("DoElectron2Tau1DiscrByDeltaR", "string");
  _Electron2Tau1DeltaRCut               = params->GetValue ("Electron2Tau1DeltaRCut", 1.0);
  _Electron2Tau1DiscrByOSLSType         = params->GetValue ("Electron2Tau1DiscrByOSLSType", "string");
  _DoElectron2Tau1DiscrByCosDphi        = params->GetValue ("DoElectron2Tau1DiscrByCosDphi", "string");
  _Electron2Tau1CosDphiMaxCut           = params->GetValue ("Electron2Tau1CosDphiMaxCut", 1.0);
  _Electron2Tau1CosDphiMinCut           = params->GetValue ("Electron2Tau1CosDphiMinCut", 1.0);
  _DoDiscrByElectron2Tau1MassReco       = params->GetValue ("DoDiscrByElectron2Tau1MassReco", "string");
  _UseVectorSumOfElectron2Tau1ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfElectron2Tau1ProductsAndMetMassReco", "string");
  _UseCollinerApproxElectron2Tau1MassReco   = params->GetValue ("UseCollinerApproxElectron2Tau1MassReco", "string");
  _Electron2Tau1MassMinCut              = params->GetValue ("Electron2Tau1MassMinCut", 1.0);
  _Electron2Tau1MassMaxCut              = params->GetValue ("Electron2Tau1MassMaxCut", 1.0);
  _DoElectron2Tau1DiscrByCDFzeta2D      = params->GetValue ("DoElectron2Tau1DiscrByCDFzeta2D", "string");
  _Electron2Tau1PZetaCutCoefficient     = params->GetValue ("Electron2Tau1PZetaCutCoefficient", 1.0);
  _Electron2Tau1PZetaVisCutCoefficient  = params->GetValue ("Electron2Tau1PZetaVisCutCoefficient", 1.0);
  _Electron2Tau1CDFzeta2DMinCutValue    = params->GetValue ("Electron2Tau1CDFzeta2DMinCutValue", 1.0);
  _Electron2Tau1CDFzeta2DMaxCutValue    = params->GetValue ("Electron2Tau1CDFzeta2DMaxCutValue", 1.0);
  _DoElectron2Tau1DiscrByDeltaPtDivSumPt = params->GetValue ("DoElectron2Tau1DiscrByDeltaPtDivSumPt", "string");
  _Electron2Tau1DeltaPtDivSumPtMinCutValue  = params->GetValue ("Electron2Tau1DeltaPtDivSumPtMinCutValue", 1.0);
  _Electron2Tau1DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Electron2Tau1DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoElectron2Tau1DiscrByDeltaPt        = params->GetValue ("DoElectron2Tau1DiscrByDeltaPt", "string");
  _Electron2Tau1DeltaPtMinCutValue      = params->GetValue ("Electron2Tau1DeltaPtMinCutValue", 1.0);
  _Electron2Tau1DeltaPtMaxCutValue      = params->GetValue ("Electron2Tau1DeltaPtMaxCutValue", 1.0);
  _DoElectron2Tau2DiscrByDeltaR         = params->GetValue ("DoElectron2Tau2DiscrByDeltaR", "string");
  _Electron2Tau2DeltaRCut               = params->GetValue ("Electron2Tau2DeltaRCut", 1.0);
  _Electron2Tau2DiscrByOSLSType         = params->GetValue ("Electron2Tau2DiscrByOSLSType", "string");
  _DoElectron2Tau2DiscrByCosDphi        = params->GetValue ("DoElectron2Tau2DiscrByCosDphi", "string");
  _Electron2Tau2CosDphiMaxCut           = params->GetValue ("Electron2Tau2CosDphiMaxCut", 1.0);
  _Electron2Tau2CosDphiMinCut           = params->GetValue ("Electron2Tau2CosDphiMinCut", 1.0);
  _DoDiscrByElectron2Tau2MassReco       = params->GetValue ("DoDiscrByElectron2Tau2MassReco", "string");
  _UseVectorSumOfElectron2Tau2ProductsAndMetMassReco = params->GetValue ("UseVectorSumOfElectron2Tau2ProductsAndMetMassReco", "string");
  _UseCollinerApproxElectron2Tau2MassReco   = params->GetValue ("UseCollinerApproxElectron2Tau2MassReco", "string");
  _Electron2Tau2MassMinCut              = params->GetValue ("Electron2Tau2MassMinCut", 1.0);
  _Electron2Tau2MassMaxCut              = params->GetValue ("Electron2Tau2MassMaxCut", 1.0);
  _DoElectron2Tau2DiscrByCDFzeta2D      = params->GetValue ("DoElectron2Tau2DiscrByCDFzeta2D", "string");
  _Electron2Tau2PZetaCutCoefficient     = params->GetValue ("Electron2Tau2PZetaCutCoefficient", 1.0);
  _Electron2Tau2PZetaVisCutCoefficient  = params->GetValue ("Electron2Tau2PZetaVisCutCoefficient", 1.0);
  _Electron2Tau2CDFzeta2DMinCutValue    = params->GetValue ("Electron2Tau2CDFzeta2DMinCutValue", 1.0);
  _Electron2Tau2CDFzeta2DMaxCutValue    = params->GetValue ("Electron2Tau2CDFzeta2DMaxCutValue", 1.0);
  _DoElectron2Tau2DiscrByDeltaPtDivSumPt = params->GetValue ("DoElectron2Tau2DiscrByDeltaPtDivSumPt", "string");
  _Electron2Tau2DeltaPtDivSumPtMinCutValue  = params->GetValue ("Electron2Tau2DeltaPtDivSumPtMinCutValue", 1.0);
  _Electron2Tau2DeltaPtDivSumPtMaxCutValue  = params->GetValue ("Electron2Tau2DeltaPtDivSumPtMaxCutValue", 1.0);
  _DoElectron2Tau2DiscrByDeltaPt        = params->GetValue ("DoElectron2Tau2DiscrByDeltaPt", "string");
  _Electron2Tau2DeltaPtMinCutValue      = params->GetValue ("Electron2Tau2DeltaPtMinCutValue", 1.0);
  _Electron2Tau2DeltaPtMaxCutValue      = params->GetValue ("Electron2Tau2DeltaPtMaxCutValue", 1.0);
  _Trigger1FirstRequirement             = params->GetValue ("Trigger1FirstRequirement", "string");
  _Trigger1SecondRequirement             = params->GetValue ("Trigger1SecondRequirement", "string");
  _Trigger2FirstRequirement             = params->GetValue ("Trigger2FirstRequirement", "string");
  _Trigger2SecondRequirement             = params->GetValue ("Trigger2SecondRequirement", "string");
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
  _JetEnergyScaleOffset              = params->GetValue ("JetEnergyScaleOffset", 1.0);
  _MatchBToGen                       = params->GetValue ("MatchBToGen", "string");
  _DataHistos                        = params->GetValue ("DataHistos", "string");
  isData                             = params->GetValue ("isData", "string");
  _MCHistos                          = params->GetValue ("MCHistos", "string"); 
  _FillRecoVertexHists               = params->GetValue ("FillRecoVertexHists", "string");
  _FillGenHists                      = params->GetValue ("FillGenHists", "string");
  _FillRecoTauHists                  = params->GetValue ("FillRecoTauHists", "string");
  _FillRecoMuonHists                 = params->GetValue ("FillRecoMuonHists", "string");
  _FillRecoJetHists                  = params->GetValue ("FillRecoJetHists", "string");
  _FillTopologyHists                 = params->GetValue ("FillTopologyHists", "string");

  string inputString;
  string inputType;

  //---open theinput file containing all event level selection requirements
  ifstream inEventRequirementsFile;
  char inputEventRequirementsFilename[] = "BSM3GAnalyzer_EventRequirements.in";
  inEventRequirementsFile.open(inputEventRequirementsFilename, ios::in);
  //---if can't open input file, then exit
  if (!inEventRequirementsFile) {
    cerr << "BSM3GAnalyzer Error: Can't open the input configuration file " << inputEventRequirementsFilename << endl;
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
    cerr << "BSM3GAnalyzer Message: Can't open the input configuration file " << inputCutFoldersFilename << endl;
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
  initializePileupInfo();
}

//---this function is called once for each event. It performs the main analysis (cuts, filling of histograms, etc.)
void BSM3GAnalyzer::analyze(TFile *theFile) {
  
  //---Number of events analyzed (denominator)
  _totalEvents++;
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished incrementing event counter" << std::endl;

  //---Get weights for the calculation of pdf systematic uncertainties for the denominator
  pdfWeightVector.clear();
  pdfWeightVector.push_back(1); // set the weight to 1 for the moment ...
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished initializing the pdf weight vector" << std::endl;
  
  //---This is where the pileup weight is calculated
  if((_CalculatePUSystematics == "1") && (isData == "0") ) { pu_weight = getPileupWeight(nTruePUInteractions); }
  else { pu_weight = 1.0; }
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished calculating pu weights" << std::endl;

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

  //---recalculate MET (if the muons are treated as neutrinos/taus or if the objects are being 'smeared')
  double temppx = Met_type1PF_px + deltaForMEx;
  double temppy = Met_type1PF_py + deltaForMEy;
  double temppz = Met_type1PF_pz;
  double temppt = TMath::Sqrt((temppx*temppx) + (temppy*temppy));
  TLorentzVector theTempMETVector;
  theTempMETVector.SetPxPyPzE(temppx,temppy,temppz,temppt);
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
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished booking histograms" << std::endl;
  
  //---this is a placeholder for the moment ... eventually this will be a product of all the weights
  isrgluon_weight = pu_weight;
  
  //---Get the event flags (did the event pass the cuts?)
  getEventFlags();
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished getting event flags" << std::endl;

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
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished initializing the pdf weight vector" << std::endl;
  
  //---Number of events passing cuts (numerator)
  if(passEventSelectionSequence()) {
    _totalEventsPassingCuts++;
/*
    for(int j = 0; j < Gen_pt->size(); j++) {
      if(Gen_BmotherIndex->at(j) > -1) {
        if( (abs(Gen_pdg_id->at(j)) == 15) || (abs(Gen_pdg_id->at(j)) == 9900024) || (abs(Gen_pdg_id->at(j)) == 9900016) || ((int)(abs(Gen_pdg_id->at(Gen_BmotherIndex->at(j)))) == 9900024) || ((int)(abs(Gen_pdg_id->at(Gen_BmotherIndex->at(j)))) == 9900016) ) {
          std::cout << "Gen #" << j << ": pdgID = " << (int)(abs(Gen_pdg_id->at(j))) << ", pt = " << Gen_pt->at(j) << ", eta = " << Gen_eta->at(j) << ", status = " << 
          Gen_status->at(j) << ", mother index = " << Gen_BmotherIndex->at(j) << ", mother pdgID = " << (int)(abs(Gen_pdg_id->at(Gen_BmotherIndex->at(j)))) << std::endl;
        }
      }
    }
*/
  }
  //std::cout << "          BSM3GAnalyzer 'analyze' Function: Finished incrementing the passing events counter" << std::endl;
  
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

  // ------Does the event pass the first trigger requirements?
  int nTriggersSatisfied = 0;
  if(passRecoTrigger1Cuts()) {nTriggersSatisfied++;}
  if (nTriggersSatisfied>=_RecoTriggers1Nmin) _EventFlag[_mapSelectionAlgoID["RecoTriggers1Nmin"]] = true;

  //---deltas for recalculation of MET (used when studying systematics or treating muons as neutrinos/taus)
  deltaForMEx = 0;
  deltaForMEy = 0;
  sumpxForMht = 0.0;
  sumpyForMht = 0.0;
  sumptForHt  = 0.0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if( (smearedJetMomentumVector.at(j).Pt() > _JetPtForMhtAndHt) && (fabs(smearedJetMomentumVector.at(j).Eta()) < _JetEtaForMhtAndHt) ) {
      if(_ApplyJetLooseIDforMhtAndHt == "1") {
        if(passedLooseJetID(j)) {
          sumpxForMht = sumpxForMht - smearedJetMomentumVector.at(j).Px();
          sumpyForMht = sumpyForMht - smearedJetMomentumVector.at(j).Py();
          sumptForHt  = sumptForHt  + smearedJetMomentumVector.at(j).Pt();
        }
      } else {
        sumpxForMht = sumpxForMht - smearedJetMomentumVector.at(j).Px();
        sumpyForMht = sumpyForMht - smearedJetMomentumVector.at(j).Py();
        sumptForHt  = sumptForHt  + smearedJetMomentumVector.at(j).Pt();
      }
    }
  }
  if (sumpxForMht >= 0) {phiForMht = atan(sumpyForMht/sumpxForMht);}
  if (sumpxForMht < 0 && sumpyForMht >= 0) {phiForMht = atan(sumpyForMht/sumpxForMht) + TMath::Pi();}
  if (sumpxForMht < 0 && sumpyForMht < 0) {phiForMht = atan(sumpyForMht/sumpxForMht) - TMath::Pi();}

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
  TLorentzVector theTempMETVector;
  theTempMETVector.SetPxPyPzE(temppx,temppy,temppz,temppt);
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

  nGoodJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoCentralJetCuts(j)) continue;
    nGoodJets++;
  }
  if (nGoodJets>=_RecoCentralJetNmin) _EventFlag[_mapSelectionAlgoID["RecoCentralJetNmin"]] = true;
  if (nGoodJets<=_RecoCentralJetNmax) _EventFlag[_mapSelectionAlgoID["RecoCentralJetNmax"]] = true;

  // ------Number of Good Leading Jets
  leadingjetpt = 0;
  leadingjeteta = -100;
  theLeadingJetIndex = -1;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoFirstLeadingJetCuts(j)) continue;
    if(smearedJetMomentumVector.at(j).Pt() > leadingjetpt) {
      leadingjetpt = smearedJetMomentumVector.at(j).Pt();
      leadingjeteta = smearedJetMomentumVector.at(j).Eta();
      theLeadingJetIndex = j;
    }
  }
  int nGoodFirstLeadingJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (j != theLeadingJetIndex) continue;
    nGoodFirstLeadingJets++;
  }
  if (nGoodFirstLeadingJets>=_RecoFirstLeadingJetNmin) _EventFlag[_mapSelectionAlgoID["RecoFirstLeadingJetNmin"]] = true;

  // ------Number of Good Second Leading Jets
  secondleadingjetpt = 0;
  secondleadingjeteta = -100;
  theSecondLeadingJetIndex = -1;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoSecondLeadingJetCuts(j)) continue;
    if((smearedJetMomentumVector.at(j).Pt() > secondleadingjetpt) && (j != theLeadingJetIndex)) {
      secondleadingjetpt = smearedJetMomentumVector.at(j).Pt();
      secondleadingjeteta = smearedJetMomentumVector.at(j).Eta();
      theSecondLeadingJetIndex = j;
    }
  }
  int nGoodSecondLeadingJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (j != theSecondLeadingJetIndex) continue;
    nGoodSecondLeadingJets++;
  }
  if (nGoodSecondLeadingJets>=_RecoSecondLeadingJetNmin) _EventFlag[_mapSelectionAlgoID["RecoSecondLeadingJetNmin"]] = true;

  // ------Number of Good b-Jets
  int nGoodBJets = 0;
  for(int j = 0; j < Jet_pt->size(); j++) {
    if (!passRecoBJetCuts(j)) continue;
    nGoodBJets++;
  }
  if (nGoodBJets>=_RecoBJetNmin) _EventFlag[_mapSelectionAlgoID["RecoBJetNmin"]] = true;
  if (nGoodBJets<=_RecoBJetNmax) _EventFlag[_mapSelectionAlgoID["RecoBJetNmax"]] = true;

  // ------Number of Good Susy Combinations (jet1+jet2+met combinations)
  int nGoodSusyCombinations = 0;
  if(passSusyTopologyCuts(theLeadingJetIndex,theSecondLeadingJetIndex)) {nGoodSusyCombinations++;}
  if (nGoodSusyCombinations>=_SusyCombinationsNmin) _EventFlag[_mapSelectionAlgoID["SusyCombinationsNmin"]] = true;

  // ------lepton+Met topology cuts
  nGoodCandidatesMuon1 = 0;
  nGoodCandidatesElectron1 = 0;
  nGoodCandidatesTau1 = 0;
  nGoodCandidatesMuon2 = 0;
  nGoodCandidatesElectron2 = 0;
  nGoodCandidatesTau2 = 0;
  for(int j = 0; j < Muon_pt->size(); j++) {
    if (!passRecoMuon1Cuts(j)) continue;
    if (!passRecoMuon1MetTopologyCuts(j)) continue;
    nGoodCandidatesMuon1++;
  }
  for(int j = 0; j < Muon_pt->size(); j++) {
    if (!passRecoMuon2Cuts(j)) continue;
    if (!passRecoMuon2MetTopologyCuts(j)) continue;
    nGoodCandidatesMuon2++;
  }
  for(int j = 0; j < patElectron_pt->size(); j++) {
    if (!passRecoElectron1Cuts(j)) continue;
    if (!passRecoElectron1MetTopologyCuts(j)) continue;
    nGoodCandidatesElectron1++;
  }
  for(int j = 0; j < patElectron_pt->size(); j++) {
    if (!passRecoElectron2Cuts(j)) continue;
    if (!passRecoElectron2MetTopologyCuts(j)) continue;
    nGoodCandidatesElectron2++;
  }
  for(int j = 0; j < Tau_pt->size(); j++) {
    if (!passRecoTau1Cuts(j)) continue;
    if (!passRecoTau1MetTopologyCuts(j)) continue;
    nGoodCandidatesTau1++;
  }
  for(int j = 0; j < Tau_pt->size(); j++) {
    if (!passRecoTau2Cuts(j)) continue;
    if (!passRecoTau2MetTopologyCuts(j)) continue;
    nGoodCandidatesTau2++;
  }
  if (nGoodCandidatesMuon1>=_RecoMuon1MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoMuon1MetTopologyNmin"]] = true;
  if (nGoodCandidatesMuon1<=_RecoMuon1MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoMuon1MetTopologyNmax"]] = true;
  if (nGoodCandidatesMuon2>=_RecoMuon2MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoMuon2MetTopologyNmin"]] = true;
  if (nGoodCandidatesMuon2<=_RecoMuon2MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoMuon2MetTopologyNmax"]] = true;
  if (nGoodCandidatesElectron1>=_RecoElectron1MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoElectron1MetTopologyNmin"]] = true;
  if (nGoodCandidatesElectron1<=_RecoElectron1MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoElectron1MetTopologyNmax"]] = true;
  if (nGoodCandidatesElectron2>=_RecoElectron2MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoElectron2MetTopologyNmin"]] = true;
  if (nGoodCandidatesElectron2<=_RecoElectron2MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoElectron2MetTopologyNmax"]] = true;
  if (nGoodCandidatesTau1>=_RecoTau1MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoTau1MetTopologyNmin"]] = true;
  if (nGoodCandidatesTau1<=_RecoTau1MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoTau1MetTopologyNmax"]] = true;
  if (nGoodCandidatesTau2>=_RecoTau2MetTopologyNmin) _EventFlag[_mapSelectionAlgoID["RecoTau2MetTopologyNmin"]] = true;
  if (nGoodCandidatesTau2<=_RecoTau2MetTopologyNmax) _EventFlag[_mapSelectionAlgoID["RecoTau2MetTopologyNmax"]] = true;

  // ------Number of Good Combinations
  int nGoodDiTauCombinations = 0;
  for(int t1j = 0; t1j < Tau_pt->size(); t1j++) {
    for(int t2j = 0; t2j < Tau_pt->size(); t2j++) {
      if ((passRecoTau1Cuts(t1j)) && (passRecoTau2Cuts(t2j)) && (passDiTauTopologyCuts(t1j,t2j)) ) {
        nGoodDiTauCombinations++;
      }
    }
  }
  int nGoodDiMuonCombinations = 0;
  for(int m1j = 0; m1j < Muon_pt->size(); m1j++) {
    for(int m2j = 0; m2j < Muon_pt->size(); m2j++) {
      if ((passRecoMuon1Cuts(m1j)) && (passRecoMuon2Cuts(m2j)) && (passDiMuonTopologyCuts(m1j,m2j)) ) {
        nGoodDiMuonCombinations++;
      }
    }
  }
  int nGoodDiElectronCombinations = 0;
  for(int e1j = 0; e1j < patElectron_pt->size(); e1j++) {
    for(int e2j = 0; e2j < patElectron_pt->size(); e2j++) {
      if ((passRecoElectron1Cuts(e1j)) && (passRecoElectron2Cuts(e2j)) && (passDiElectronTopologyCuts(e1j,e2j)) ) {
        nGoodDiElectronCombinations++;
      }
    }
  }
  int nGoodDiJetCombinations = 0;
  for(int j1j = 0; j1j < Jet_pt->size(); j1j++) {
    for(int j2j = 0; j2j < Jet_pt->size(); j2j++) {
      if ((passRecoJet1Cuts(j1j)) && (passRecoJet2Cuts(j2j)) && (passDiJetTopologyCuts(j1j,j2j)) ) {
        nGoodDiJetCombinations++;
      }
    }
  }
  int nGoodMuon1Tau1Combinations = 0;
  for(int mj = 0; mj < Muon_pt->size(); mj++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau1Cuts(tj)) && (passRecoMuon1Cuts(mj)) && (passMuon1Tau1TopologyCuts(tj,mj))) {
        nGoodMuon1Tau1Combinations++;
      }
    }
  }
  int nGoodMuon1Tau2Combinations = 0;
  for(int mj = 0; mj < Muon_pt->size(); mj++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau2Cuts(tj)) && (passRecoMuon1Cuts(mj)) && (passMuon1Tau2TopologyCuts(tj,mj))) {
        nGoodMuon1Tau2Combinations++;
      }
    }
  }
  int nGoodMuon2Tau1Combinations = 0;
  for(int mj = 0; mj < Muon_pt->size(); mj++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau1Cuts(tj)) && (passRecoMuon2Cuts(mj)) && (passMuon2Tau1TopologyCuts(tj,mj))) {
        nGoodMuon2Tau1Combinations++;
      }
    }
  }
  int nGoodMuon2Tau2Combinations = 0;
  for(int mj = 0; mj < Muon_pt->size(); mj++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau2Cuts(tj)) && (passRecoMuon2Cuts(mj)) && (passMuon2Tau2TopologyCuts(tj,mj))) {
        nGoodMuon2Tau2Combinations++;
      }
    }
  }
  int nGoodElectron1Tau1Combinations = 0;
  for(int ej = 0; ej < patElectron_pt->size(); ej++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau1Cuts(tj)) && (passRecoElectron1Cuts(ej)) && (passElectron1Tau1TopologyCuts(tj,ej))) {
        nGoodElectron1Tau1Combinations++;
      }
    }
  }
  int nGoodElectron1Tau2Combinations = 0;
  for(int ej = 0; ej < patElectron_pt->size(); ej++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau2Cuts(tj)) && (passRecoElectron1Cuts(ej)) && (passElectron1Tau2TopologyCuts(tj,ej))) {
        nGoodElectron1Tau2Combinations++;
      }
    }
  }
  int nGoodElectron2Tau1Combinations = 0;
  for(int ej = 0; ej < patElectron_pt->size(); ej++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau1Cuts(tj)) && (passRecoElectron2Cuts(ej)) && (passElectron2Tau1TopologyCuts(tj,ej))) {
        nGoodElectron2Tau1Combinations++;
      }
    }
  }
  int nGoodElectron2Tau2Combinations = 0;
  for(int ej = 0; ej < patElectron_pt->size(); ej++) {
    for(int tj = 0; tj < Tau_pt->size(); tj++) {
      if ((passRecoTau2Cuts(tj)) && (passRecoElectron2Cuts(ej)) && (passElectron2Tau2TopologyCuts(tj,ej))) {
        nGoodElectron2Tau2Combinations++;
      }
    }
  }

  if(nGoodDiMuonCombinations > 1000) {nGoodDiMuonCombinations = 999;}
  if(nGoodDiElectronCombinations > 1000) {nGoodDiElectronCombinations = 999;}
  if(nGoodDiTauCombinations > 1000) {nGoodDiTauCombinations = 999;}
  if(nGoodDiJetCombinations > 1000) {nGoodDiJetCombinations = 999;}
  if(nGoodMuon1Tau1Combinations > 1000) {nGoodMuon1Tau1Combinations = 999;}
  if(nGoodMuon1Tau2Combinations > 1000) {nGoodMuon1Tau2Combinations = 999;}
  if(nGoodMuon2Tau1Combinations > 1000) {nGoodMuon2Tau1Combinations = 999;}
  if(nGoodMuon2Tau2Combinations > 1000) {nGoodMuon2Tau2Combinations = 999;}
  if(nGoodElectron1Tau1Combinations > 1000) {nGoodElectron1Tau1Combinations = 999;}
  if(nGoodElectron1Tau2Combinations > 1000) {nGoodElectron1Tau2Combinations = 999;}
  if(nGoodElectron2Tau1Combinations > 1000) {nGoodElectron2Tau1Combinations = 999;}
  if(nGoodElectron2Tau2Combinations > 1000) {nGoodElectron2Tau2Combinations = 999;}
  if(nGoodDiMuonCombinations>=_DiMuonCombinationsNmin) _EventFlag[_mapSelectionAlgoID["DiMuonCombinationsNmin"]] = true;
  if(nGoodDiMuonCombinations<=_DiMuonCombinationsNmax) _EventFlag[_mapSelectionAlgoID["DiMuonCombinationsNmax"]] = true;
  if(nGoodDiElectronCombinations>=_DiElectronCombinationsNmin) _EventFlag[_mapSelectionAlgoID["DiElectronCombinationsNmin"]] = true;
  if(nGoodDiElectronCombinations<=_DiElectronCombinationsNmax) _EventFlag[_mapSelectionAlgoID["DiElectronCombinationsNmax"]] = true;
  if(nGoodDiTauCombinations>=_DiTauCombinationsNmin) _EventFlag[_mapSelectionAlgoID["DiTauCombinationsNmin"]] = true;
  if(nGoodDiTauCombinations<=_DiTauCombinationsNmax) _EventFlag[_mapSelectionAlgoID["DiTauCombinationsNmax"]] = true;
  if(nGoodDiJetCombinations>=_DiJetCombinationsNmin) _EventFlag[_mapSelectionAlgoID["DiJetCombinationsNmin"]] = true;
  if(nGoodDiJetCombinations<=_DiJetCombinationsNmax) _EventFlag[_mapSelectionAlgoID["DiJetCombinationsNmax"]] = true;
  if(nGoodMuon1Tau1Combinations>=_Muon1Tau1CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Muon1Tau1CombinationsNmin"]] = true;
  if(nGoodMuon1Tau1Combinations<=_Muon1Tau1CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Muon1Tau1CombinationsNmax"]] = true;
  if(nGoodMuon1Tau2Combinations>=_Muon1Tau2CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Muon1Tau2CombinationsNmin"]] = true;
  if(nGoodMuon1Tau2Combinations<=_Muon1Tau2CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Muon1Tau2CombinationsNmax"]] = true;
  if(nGoodMuon2Tau1Combinations>=_Muon2Tau1CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Muon2Tau1CombinationsNmin"]] = true;
  if(nGoodMuon2Tau1Combinations<=_Muon2Tau1CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Muon2Tau1CombinationsNmax"]] = true;
  if(nGoodMuon2Tau2Combinations>=_Muon2Tau2CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Muon2Tau2CombinationsNmin"]] = true;
  if(nGoodMuon2Tau2Combinations<=_Muon2Tau2CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Muon2Tau2CombinationsNmax"]] = true;
  if(nGoodElectron1Tau1Combinations>=_Electron1Tau1CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Electron1Tau1CombinationsNmin"]] = true;
  if(nGoodElectron1Tau1Combinations<=_Electron1Tau1CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Electron1Tau1CombinationsNmax"]] = true;
  if(nGoodElectron1Tau2Combinations>=_Electron1Tau2CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Electron1Tau2CombinationsNmin"]] = true;
  if(nGoodElectron1Tau2Combinations<=_Electron1Tau2CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Electron1Tau2CombinationsNmax"]] = true;
  if(nGoodElectron2Tau1Combinations>=_Electron2Tau1CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Electron2Tau1CombinationsNmin"]] = true;
  if(nGoodElectron2Tau1Combinations<=_Electron2Tau1CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Electron2Tau1CombinationsNmax"]] = true;
  if(nGoodElectron2Tau2Combinations>=_Electron2Tau2CombinationsNmin) _EventFlag[_mapSelectionAlgoID["Electron2Tau2CombinationsNmin"]] = true;
  if(nGoodElectron2Tau2Combinations<=_Electron2Tau2CombinationsNmax) _EventFlag[_mapSelectionAlgoID["Electron2Tau2CombinationsNmax"]] = true;

  // ------Does the event pass trigger requirements?
  nTriggersSatisfied = 0;
  if(passRecoTrigger2Cuts()) {nTriggersSatisfied++;}
  if(nTriggersSatisfied>=_RecoTriggers2Nmin) _EventFlag[_mapSelectionAlgoID["RecoTriggers2Nmin"]] = true;

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

    // ------Primary Vertices
    if (_FillRecoVertexHists == "1") {
      _hNPVertices[i][NpdfID]->Fill(bestVertices,isrgluon_weight * pdfWeightVector.at(NpdfID));
    }

    // ------Gen level plots
    if ( (_FillGenHists == "1") && (isData == "0") ) {

      // ------Generated Taus
      int nGenTaus = 0;
      for(int j = 0; j < Gen_pt->size(); j++) {
        if(abs(Gen_pdg_id->at(j)) == 15) {
          //std::cout << "GenTau #" << j << ":  pt = " << Gen_pt->at(j) << ", eta = " << Gen_eta->at(j) << ", phi = " << Gen_phi->at(j) << ", status = " << Gen_status->at(j) << std::endl;
          _hGenTauStatusCode[i][NpdfID]->Fill(Gen_status->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        }
        if((abs(Gen_pdg_id->at(j)) == 15) && (Gen_status->at(j) == 2)) {
          _hGenTauEnergy[i][NpdfID]->Fill(Gen_energy->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauPt[i][NpdfID]->Fill(Gen_pt->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauEta[i][NpdfID]->Fill(Gen_eta->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          _hGenTauPhi[i][NpdfID]->Fill(Gen_phi->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
          nGenTaus++;
	}
      }
      //std::cout << "NGenTaus = " << nGenTaus << std::endl;
      _hNGenTau[i][NpdfID]->Fill(nGenTaus,isrgluon_weight * pdfWeightVector.at(NpdfID));

      int nGenHadTaus = 0;
      bool IsItAHadronicDecay; // boolean used to specify whether a gen tau lepton decays hadronically
      TLorentzVector theGenObject(0,0,0,0); // initialize the 4-momentum vector of the vis gen tau matched to the reco tau
      TLorentzVector theNeutrinoObject(0,0,0,0); // initialize the 4-momentum vector of the tau neutrino from the tau-lepton decay
      vector<bool> IsItAHadronicDecayVector; // vector of booleans which contain the information about whether a gen tau lepton decays hadronically
      IsItAHadronicDecayVector.clear(); // clear any previous declaration from memory
      vector<int> tempTauIndexVector; // vector which contains the index (in the gen collection) of the gen level taus (before decaying)
      tempTauIndexVector.clear(); // clear any previous declaration from memory
      vector<TLorentzVector> tempNeutrinoMomentumVector; // vector of lorentz 4-momentum vectors for each tau neutrino from the tau-lepton decay
      tempNeutrinoMomentumVector.clear(); // clear any previous declaration from memory
      //---Loop over gen particles to find the tau neutrinos and then store the index of each tau neutrino's mother (a tau).
      //---Also store the tau neutrino's 4-momentum vector in order to calculate the visible tau 4-momentum at a later point.
      for(int j = 0; j < Gen_pt->size(); j++) {
        if( (abs(Gen_pdg_id->at(j)) == 16) && (abs(Gen_pdg_id->at(Gen_BmotherIndex->at(j))) == 15) && (Gen_status->at(Gen_BmotherIndex->at(j)) == 2) ) {
          tempTauIndexVector.push_back(Gen_BmotherIndex->at(j));
          theNeutrinoObject.SetPtEtaPhiE(Gen_pt->at(j), Gen_eta->at(j), Gen_phi->at(j), Gen_energy->at(j));
          tempNeutrinoMomentumVector.push_back(theNeutrinoObject);
        }
      }
      //---if there is at least one gen tau in the event
      if(tempTauIndexVector.size() > 0) {
        //---Loop over the gen taus and determine whether it decays hadronically.
        for(int jj = 0; jj < tempTauIndexVector.size(); jj++) {
          IsItAHadronicDecay = true;
          for(int j = 0; j < Gen_pt->size(); j++) {
            if( ((abs(Gen_pdg_id->at(j)) == 12) || (abs(Gen_pdg_id->at(j)) == 14)) && (Gen_BmotherIndex->at(j) == tempTauIndexVector.at(jj)) ) {
              IsItAHadronicDecay = false; // it is not a hadronic tau decay since it decayed to a electron/muon neutrino
            }
          }
          IsItAHadronicDecayVector.push_back(IsItAHadronicDecay);
        }
        //---Loop over the gen taus and calculate the 4-momentum of the visible products (i.e. subtract the 4-momentum of the tau neutrino)
        for(int jj = 0; jj < tempTauIndexVector.size(); jj++) {
          for(int j = 0; j < Gen_pt->size(); j++) {
            if(j == tempTauIndexVector.at(jj)) {
              theGenObject.SetPtEtaPhiE(Gen_pt->at(j), Gen_eta->at(j), Gen_phi->at(j), Gen_energy->at(j)); // 4-momentum of the gen tau
              theGenObject = theGenObject - tempNeutrinoMomentumVector.at(jj); // subtract the 4-momentum of the tau neutrino (visible tau)
              if( (IsItAHadronicDecayVector.at(jj)) ) {
                nGenHadTaus++;
                _hGenHadTauPt[i][NpdfID]->Fill(theGenObject.Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
                _hGenHadTauEta[i][NpdfID]->Fill(theGenObject.Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
              }
            }
          }
        }
      }
      _hNGenHadTau[i][NpdfID]->Fill(nGenHadTaus,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Generated Muons
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
      _hNGenMuon[i][NpdfID]->Fill(nGenMuons,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Generated Zprimes
      for(int j = 0; j < Gen_pt->size(); j++) {
        if(abs(Gen_pdg_id->at(j)) == 32) {
          //std::cout << "GenZprime #" << j << ":  pt = " << Gen_pt->at(j) << ", eta = " << Gen_eta->at(j) << ", phi = " << Gen_phi->at(j) << ", status = " << Gen_status->at(j) << std::endl;
          _hGenZprimeStatusCode[i][NpdfID]->Fill(Gen_status->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        }
        if((abs(Gen_pdg_id->at(j)) == 32) && (Gen_status->at(j) != 22)) {
          TLorentzVector genObjt1;
          genObjt1.SetPtEtaPhiE(Gen_pt->at(j), Gen_eta->at(j), Gen_phi->at(j), Gen_energy->at(j));
          _hGenZprimeMass[i][NpdfID]->Fill(genObjt1.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        }
      }

      // ------Gen level ditau mass (before taus decay)
      for(int j = 0; j < Gen_pt->size(); j++) {
        for(int jj = 0; jj < Gen_pt->size(); jj++) {
          if((abs(Gen_pdg_id->at(j)) == 15) && (Gen_status->at(j) == 2) && (abs(Gen_pdg_id->at(jj)) == 15) && (Gen_status->at(jj) == 2) && (jj > j)) {
            TLorentzVector genObjt1;
            genObjt1.SetPtEtaPhiE(Gen_pt->at(j), Gen_eta->at(j), Gen_phi->at(j), Gen_energy->at(j));
            TLorentzVector genObjt2;
            genObjt2.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj));
            _hGenDiTauMass[i][NpdfID]->Fill(CalculateTheDiJet4Momentum(genObjt1,genObjt2).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));
          }
        }
      }

    }
    // ------Reco Tau Histograms
    if (_FillRecoTauHists == "1") {
      int nTaus = 0;
      double leadingtaupt = 0;
      double leadingtaueta = 0;
      for(int j = 0; j < Tau_pt->size(); j++) {
	if (!passRecoTau1Cuts(j)) continue;
        if(smearedTauMomentumVector.at(j).Pt() >= leadingtaupt) {
          leadingtaupt = smearedTauMomentumVector.at(j).Pt();
          leadingtaueta = smearedTauMomentumVector.at(j).Eta();
        }
	_hTauJet1Energy[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hTauJet1Pt[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hTauJet1Eta[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hTauJet1Phi[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet1NumSignalTracks[i][NpdfID]->Fill(Tau_nProngs->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hTauJet1Charge[i][NpdfID]->Fill(Tau_charge->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet1SeedTrackPt[i][NpdfID]->Fill(Tau_leadChargedCandPt->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        nTaus++;
      }
      _hNTau1[i][NpdfID]->Fill(nTaus,isrgluon_weight * pdfWeightVector.at(NpdfID));
      if(nTaus > 0) {
        _hFirstLeadingTauJet1Pt[i][NpdfID]->Fill(leadingtaupt,isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hFirstLeadingTauJet1Eta[i][NpdfID]->Fill(leadingtaueta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      }
      nTaus = 0;
      leadingtaupt = 0;
      leadingtaueta = 0;
      for(int j = 0; j < Tau_pt->size(); j++) {
        if (!passRecoTau2Cuts(j)) continue;
        if(smearedTauMomentumVector.at(j).Pt() >= leadingtaupt) {
          leadingtaupt = smearedTauMomentumVector.at(j).Pt();
          leadingtaueta = smearedTauMomentumVector.at(j).Eta();
        }
        _hTauJet2Energy[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2Pt[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2Eta[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2Phi[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2NumSignalTracks[i][NpdfID]->Fill(Tau_nProngs->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2Charge[i][NpdfID]->Fill(Tau_charge->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hTauJet2SeedTrackPt[i][NpdfID]->Fill(Tau_leadChargedCandPt->at(j),isrgluon_weight * pdfWeightVector.at(NpdfID));
        nTaus++;
      }
      _hNTau2[i][NpdfID]->Fill(nTaus,isrgluon_weight * pdfWeightVector.at(NpdfID));
      if(nTaus > 0) {
        _hFirstLeadingTauJet2Pt[i][NpdfID]->Fill(leadingtaupt,isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hFirstLeadingTauJet2Eta[i][NpdfID]->Fill(leadingtaueta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      }
    }

    // ------Reco Muon Histograms
    if (_FillRecoMuonHists == "1") {
      int nMuons = 0;
      double leadingmuonpt = 0;
      double leadingmuoneta = 0;
      for(int j = 0; j < Muon_pt->size(); j++) {
	if (!passRecoMuon1Cuts(j)) continue;
        if(smearedMuonMomentumVector.at(j).Pt() >= leadingmuonpt) {
          leadingmuonpt = smearedMuonMomentumVector.at(j).Pt();
          leadingmuoneta = smearedMuonMomentumVector.at(j).Eta();
        }
        _hMuon1Energy[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hMuon1Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hMuon1Eta[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hMuon1Phi[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hMuon1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(j)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	nMuons++;
      }
      _hNMuon1[i][NpdfID]->Fill(nMuons,isrgluon_weight * pdfWeightVector.at(NpdfID));
      if(nMuons > 0) {
        _hFirstLeadingMuon1Pt[i][NpdfID]->Fill(leadingmuonpt,isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hFirstLeadingMuon1Eta[i][NpdfID]->Fill(leadingmuoneta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      }
      nMuons = 0;
      leadingmuonpt = 0;
      leadingmuoneta = 0;
      for(int j = 0; j < Muon_pt->size(); j++) {
        if (!passRecoMuon2Cuts(j)) continue;
        if(smearedMuonMomentumVector.at(j).Pt() >= leadingmuonpt) {
          leadingmuonpt = smearedMuonMomentumVector.at(j).Pt();
          leadingmuoneta = smearedMuonMomentumVector.at(j).Eta();
        }
        _hMuon2Energy[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hMuon2Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hMuon2Eta[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hMuon2Phi[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hMuon2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(j)),isrgluon_weight * pdfWeightVector.at(NpdfID));
        nMuons++;
      }
      _hNMuon2[i][NpdfID]->Fill(nMuons,isrgluon_weight * pdfWeightVector.at(NpdfID));
      if(nMuons > 0) {
        _hFirstLeadingMuon2Pt[i][NpdfID]->Fill(leadingmuonpt,isrgluon_weight * pdfWeightVector.at(NpdfID));
        _hFirstLeadingMuon2Eta[i][NpdfID]->Fill(leadingmuoneta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      }
    }

    // ------Reco Jet Histograms
    if (_FillRecoJetHists == "1") {

      // ------Reco B-Jet Histograms
      int NbJets = 0;
      for(int j = 0; j < Jet_pt->size(); j++) {
	if (!passRecoBJetCuts(j)) continue;
        _hBJetEnergy[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hBJetPt[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hBJetEta[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hBJetPhi[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
        NbJets++;
      }
      _hNBJet[i][NpdfID]->Fill(NbJets,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Jet1 Histograms
      int nJets = 0;
      for(int j = 0; j < Jet_pt->size(); j++) {
	if (!passRecoJet1Cuts(j)) continue;
        _hJet1Energy[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet1Pt[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet1Eta[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet1Phi[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	nJets++;
      }
      _hNJet1[i][NpdfID]->Fill(nJets,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Jet2 Histograms
      nJets = 0;
      for(int j = 0; j < Jet_pt->size(); j++) {
	if (!passRecoJet2Cuts(j)) continue;
        _hJet2Energy[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Energy(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet2Pt[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet2Eta[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hJet2Phi[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Phi(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	nJets++;
      }
      _hNJet2[i][NpdfID]->Fill(nJets,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Central Jet Histograms
      int nCJets = 0;
      for(int j = 0; j < Jet_pt->size(); j++) {
	if (!passRecoCentralJetCuts(j)) continue;
	_hCentralJetPt[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	_hCentralJetEta[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).Eta(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	nCJets++;
      }
      _hNCentralJet[i][NpdfID]->Fill(nCJets,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------DiJet Histograms and histograms of highest mass dijet pair
      double leaddijetmass = 0;
      double leaddijetpt = 0;
      double leaddijetdeltaR = 0;
      double leaddijetdeltaEta = 0;
      double etaproduct = -100;
      for(int j = 0; j < Jet_pt->size(); j++) {
        for(int jj = 0; jj < Jet_pt->size(); jj++) {
          if ((passRecoJet1Cuts(j)) && 
              (passRecoJet2Cuts(jj)) && 
	      (passDiJetTopologyCuts(j,jj)) &&
              (jj > j)) {
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.M() > leaddijetmass) {leaddijetmass = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.M();etaproduct = smearedJetMomentumVector.at(j).Eta() * smearedJetMomentumVector.at(jj).Eta();}
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.Pt() > leaddijetpt) {leaddijetpt = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.Pt();}
            if(fabs(smearedJetMomentumVector.at(j).Eta() - smearedJetMomentumVector.at(jj).Eta()) > leaddijetdeltaEta) {leaddijetdeltaEta = fabs(smearedJetMomentumVector.at(j).Eta() - smearedJetMomentumVector.at(jj).Eta());}
            if(smearedJetMomentumVector.at(j).DeltaR(smearedJetMomentumVector.at(jj)) > leaddijetdeltaR) {leaddijetdeltaR = smearedJetMomentumVector.at(j).DeltaR(smearedJetMomentumVector.at(jj));}
            _hDiJetMass[i][NpdfID]->Fill(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hDiJetPt[i][NpdfID]->Fill(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second.Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hDiJetDeltaEta[i][NpdfID]->Fill(fabs(smearedJetMomentumVector.at(j).Eta() - smearedJetMomentumVector.at(jj).Eta()),isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hDiJetDeltaPhi[i][NpdfID]->Fill(fabs(normalizedPhi(smearedJetMomentumVector.at(j).Phi() - smearedJetMomentumVector.at(jj).Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hDiJetDeltaR[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).DeltaR(smearedJetMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
          }
        }
      }
      _hLeadDiJetMass[i][NpdfID]->Fill(leaddijetmass,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hLeadDiJetPt[i][NpdfID]->Fill(leaddijetpt,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hLeadDiJetDeltaEta[i][NpdfID]->Fill(leaddijetdeltaEta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hLeadDiJetDeltaR[i][NpdfID]->Fill(leaddijetdeltaR,isrgluon_weight * pdfWeightVector.at(NpdfID));
      if((etaproduct < 0) && (leaddijetmass != 0)) {etaproduct = -1;}
      if((etaproduct > 0) && (leaddijetmass != 0)) {etaproduct = +1;}
      _hLeadDiJetEtaProduct[i][NpdfID]->Fill(etaproduct,isrgluon_weight * pdfWeightVector.at(NpdfID));

      // ------Leading+SubLeading jet Histograms; "Susy" histograms --> r1, r2, Alpha, etc.
      for(int j = 0; j < Jet_pt->size(); j++) {
        for(int jj = 0; jj < Jet_pt->size(); jj++) {
          if ((j == theLeadingJetIndex) && (jj == theSecondLeadingJetIndex)) {
            if( (passRecoFirstLeadingJetCuts(j)) && (passRecoSecondLeadingJetCuts(jj)) && (passSusyTopologyCuts(j,jj)) ) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(j),smearedJetMomentumVector.at(jj)).second;
              _hLeadingJetsMass[i][NpdfID]->Fill(TheLeadDiJetVect.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hLeadingJetsPt[i][NpdfID]->Fill(TheLeadDiJetVect.Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hLeadingJetsDeltaEta[i][NpdfID]->Fill(fabs(smearedJetMomentumVector.at(j).Eta() - smearedJetMomentumVector.at(jj).Eta()),isrgluon_weight * pdfWeightVector.at(NpdfID));
              double dphiDijets = abs(normalizedPhi(smearedJetMomentumVector.at(j).Phi() - smearedJetMomentumVector.at(jj).Phi()));
              _hLeadSublDijetDphi[i][NpdfID]->Fill(dphiDijets,isrgluon_weight * pdfWeightVector.at(NpdfID)); 
              _hMetVsDiJetDeltaPhiLeadSubl[i][NpdfID]->Fill(theMETVector.Pt(),dphiDijets, isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDeltaEtaVsDeltaPhiLeadSubl[i][NpdfID]->Fill(fabs(smearedJetMomentumVector.at(j).Eta() - smearedJetMomentumVector.at(jj).Eta()), dphiDijets, isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hLeadingJetsDeltaR[i][NpdfID]->Fill(smearedJetMomentumVector.at(j).DeltaR(smearedJetMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
              double dphi1MHT = normalizedPhi(smearedJetMomentumVector.at(j).Phi() - phiForMht);
              double dphi2MHT = normalizedPhi(smearedJetMomentumVector.at(jj).Phi() - phiForMht);
              double dphi1 = normalizedPhi(smearedJetMomentumVector.at(j).Phi() - theMETVector.Phi());
              double dphi2 = normalizedPhi(smearedJetMomentumVector.at(jj).Phi() - theMETVector.Phi());
              double r1;
              double r2;
              double alpha;
              r1 = sqrt( pow(dphi1,2.0) + pow((TMath::Pi() - dphi2),2.0) );
              r2 = sqrt( pow(dphi2,2.0) + pow((TMath::Pi() - dphi1),2.0) );
              double px = smearedJetMomentumVector.at(j).Px() + smearedJetMomentumVector.at(jj).Px();
              double py = smearedJetMomentumVector.at(j).Py() + smearedJetMomentumVector.at(jj).Py();
              double pz = smearedJetMomentumVector.at(j).Pz() + smearedJetMomentumVector.at(jj).Pz();
              double e = smearedJetMomentumVector.at(j).Energy() + smearedJetMomentumVector.at(jj).Energy();
              TLorentzVector Susy_LorentzVect;
              Susy_LorentzVect.SetPxPyPzE(px, py, pz, e);
              if(Susy_LorentzVect.M() > 0) {alpha = smearedJetMomentumVector.at(jj).Pt() / Susy_LorentzVect.M();}
              else {alpha = 999999999.0;}
              _hR1[i][NpdfID]->Fill(r1,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hR2[i][NpdfID]->Fill(r2,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDphi1MHT[i][NpdfID]->Fill(dphi1MHT,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDphi2MHT[i][NpdfID]->Fill(dphi2MHT,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDphi1[i][NpdfID]->Fill(dphi1,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDphi2[i][NpdfID]->Fill(dphi2,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hDphi1VsDphi2[i][NpdfID]->Fill(dphi1,dphi2,isrgluon_weight * pdfWeightVector.at(NpdfID));
              _hAlpha[i][NpdfID]->Fill(alpha,isrgluon_weight * pdfWeightVector.at(NpdfID));
            }
          }
        }
      }
      _hFirstLeadingJetPt[i][NpdfID]->Fill(leadingjetpt,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hSecondLeadingJetPt[i][NpdfID]->Fill(secondleadingjetpt,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hFirstLeadingJetEta[i][NpdfID]->Fill(leadingjeteta,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hSecondLeadingJetEta[i][NpdfID]->Fill(secondleadingjeteta,isrgluon_weight * pdfWeightVector.at(NpdfID));
    }

    // ------Topology Histograms
    if (_FillTopologyHists == "1") {
      _hMHT[i][NpdfID]->Fill(sqrt((sumpxForMht * sumpxForMht) + (sumpyForMht * sumpyForMht)),isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hHT[i][NpdfID]->Fill(sumptForHt,isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hMeff[i][NpdfID]->Fill(sumptForHt + sqrt((sumpxForMht * sumpxForMht) + (sumpyForMht * sumpyForMht)),isrgluon_weight * pdfWeightVector.at(NpdfID));
      _hMet[i][NpdfID]->Fill(theMETVector.Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
      if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
        TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
        _hMetDiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(theMETVector.Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
      }

      for(int j = 0; j < Muon_pt->size(); j++) {
        for(int jj = 0; jj < Muon_pt->size(); jj++) {
	  if ((passRecoMuon1Cuts(j)) && (passRecoMuon2Cuts(jj)) && (passDiMuonTopologyCuts(j,jj)) && (jj > j)) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hMuon1Muon2_Muon1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(j).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
	      _hMuon1Muon2_Muon2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(jj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
            }
            _hMuon1Muon2_Muon1IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(j)).first,isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hMuon1Muon2_Muon2IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(jj)).first,isrgluon_weight * pdfWeightVector.at(NpdfID));
            _hMuon1PtVsMuon2Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).Pt(),smearedMuonMomentumVector.at(jj).Pt(),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Muon2DeltaR[i][NpdfID]->Fill(smearedMuonMomentumVector.at(j).DeltaR(smearedMuonMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Muon2DeltaPtDivSumPt[i][NpdfID]->Fill((smearedMuonMomentumVector.at(j).Pt() - smearedMuonMomentumVector.at(jj).Pt()) / (smearedMuonMomentumVector.at(j).Pt() + smearedMuonMomentumVector.at(jj).Pt()),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Muon2DeltaPt[i][NpdfID]->Fill((smearedMuonMomentumVector.at(j).Pt() - smearedMuonMomentumVector.at(jj).Pt()),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Muon2CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(j).Phi() - smearedMuonMomentumVector.at(jj).Phi()))),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuon_Muon1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(j).Phi() - theMETVector.Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuon_Muon2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(jj).Phi() - theMETVector.Phi())),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1MetDeltaPhiVsMuon1Muon2CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(j).Phi() - theMETVector.Phi())), cos(abs(normalizedPhi(smearedMuonMomentumVector.at(j).Phi() - smearedMuonMomentumVector.at(jj).Phi()))),isrgluon_weight * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfDiMuonProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxDiMuonMassReco;
	    if(CalculateTheDiTau4Momentum(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)).first) {_hDiMuonReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    else {_hDiMuonNotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    _hDiMuon_Muon1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(j)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuon_Muon2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Muon2OSLS[i][NpdfID]->Fill(Muon_charge->at(j) * Muon_charge->at(jj),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuonPZeta[i][NpdfID]->Fill(CalculatePZeta(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuonPZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuonZeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)),CalculatePZeta(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hDiMuonZeta1D[i][NpdfID]->Fill((_DiMuonPZetaCutCoefficient * CalculatePZeta(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj))) + (_DiMuonPZetaVisCutCoefficient * CalculatePZetaVis(smearedMuonMomentumVector.at(j),smearedMuonMomentumVector.at(jj))),isrgluon_weight * pdfWeightVector.at(NpdfID));
	  }
	}
      }

      for(int j = 0; j < Tau_pt->size(); j++) {
        for(int jj = 0; jj < Tau_pt->size(); jj++) {
	  if ((passRecoTau1Cuts(j)) && (passRecoTau2Cuts(jj)) && (passDiTauTopologyCuts(j,jj)) && (jj > j)) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hTau1Tau2_Tau1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(j).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	      _hTau1Tau2_Tau2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(jj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
              _UseVectorSumOfVisProductsAndMetMassReco = "1";
              _UseCollinerApproxMassReco = _UseCollinerApproxDiTauMassReco;
	      _hDiTauDiJetReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(TheLeadDiJetVect,CalculateTheDiJet4Momentum(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)).second).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            }
	    _hTau1PtVsTau2Pt[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).Pt(),smearedTauMomentumVector.at(jj).Pt(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hTau1Tau2DeltaR[i][NpdfID]->Fill(smearedTauMomentumVector.at(j).DeltaR(smearedTauMomentumVector.at(jj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hTau1Tau2DeltaPtDivSumPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(j).Pt() - smearedTauMomentumVector.at(jj).Pt()) / (smearedTauMomentumVector.at(j).Pt() + smearedTauMomentumVector.at(jj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hTau1Tau2DeltaPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(j).Pt() - smearedTauMomentumVector.at(jj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _hTau1Tau2OSLS[i][NpdfID]->Fill(Tau_charge->at(j) * Tau_charge->at(jj),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hTau1Tau2CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedTauMomentumVector.at(j).Phi() - smearedTauMomentumVector.at(jj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTau_Tau1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(j).Phi() -  theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTau_Tau2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(jj).Phi() -  theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hTau1MetDeltaPhiVsTau1Tau2CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(j).Phi() -  theMETVector.Phi())), cos(abs(normalizedPhi(smearedTauMomentumVector.at(j).Phi() - smearedTauMomentumVector.at(jj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfDiTauProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxDiTauMassReco;
	    if(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)).first) {_hDiTauReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));}
	    else {_hDiTauNotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));}
	    _hDiTau_Tau1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(j)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTau_Tau2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(jj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTauPZeta[i][NpdfID]->Fill(CalculatePZeta(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTauPZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTauZeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)),CalculatePZeta(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hDiTauZeta1D[i][NpdfID]->Fill((_DiTauPZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj))) + (_DiTauPZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(j),smearedTauMomentumVector.at(jj))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	  }
	}
      }

      for(int mj = 0; mj < Muon_pt->size(); mj++) {
        for(int tj = 0; tj < Tau_pt->size(); tj++) {
	  if ((passRecoTau1Cuts(tj)) && (passRecoMuon1Cuts(mj)) && (passMuon1Tau1TopologyCuts(tj,mj))) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hMuon1Tau1_Tau1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	      _hMuon1Tau1_Muon1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
              _UseVectorSumOfVisProductsAndMetMassReco = "1";
              _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau1MassReco;
	      _hMuon1Tau1DiJetReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(TheLeadDiJetVect,CalculateTheDiJet4Momentum(smearedMuonMomentumVector.at(mj),smearedTauMomentumVector.at(tj)).second).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            }
            _hMuon1Tau1_Muon1IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(mj)).first,isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1PtVsTau1Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(mj).Pt(),smearedTauMomentumVector.at(tj).Pt(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1DeltaR[i][NpdfID]->Fill(smearedTauMomentumVector.at(tj).DeltaR(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1DeltaPtDivSumPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()) / (smearedTauMomentumVector.at(tj).Pt() + smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1DeltaPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1_Muon1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1MetDeltaPhiVsMuon1Tau1CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())), cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1_Tau1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon1Tau1ProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau1MassReco;
	    if(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).first) {_hMuon1Tau1ReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    else {_hMuon1Tau1NotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    _hMuon1Tau1_Muon1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1_Tau1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(tj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _hMuon1Tau1OSLS[i][NpdfID]->Fill(Muon_charge->at(mj) * Tau_charge->at(tj),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1PZeta[i][NpdfID]->Fill(CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1PZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1Zeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau1Zeta1D[i][NpdfID]->Fill((_Muon1Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))) + 
					       (_Muon1Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	  }
	  if ((passRecoTau2Cuts(tj)) && (passRecoMuon1Cuts(mj)) && (passMuon1Tau2TopologyCuts(tj,mj))) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hMuon1Tau2_Tau2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	      _hMuon1Tau2_Muon1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
              _UseVectorSumOfVisProductsAndMetMassReco = "1";
              _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau2MassReco;
	      _hMuon1Tau2DiJetReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(TheLeadDiJetVect,CalculateTheDiJet4Momentum(smearedMuonMomentumVector.at(mj),smearedTauMomentumVector.at(tj)).second).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            }
            _hMuon1Tau2_Muon1IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(mj)).first,isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1PtVsTau2Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(mj).Pt(),smearedTauMomentumVector.at(tj).Pt(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2DeltaR[i][NpdfID]->Fill(smearedTauMomentumVector.at(tj).DeltaR(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2DeltaPtDivSumPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()) / (smearedTauMomentumVector.at(tj).Pt() + smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2DeltaPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2_Muon1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1MetDeltaPhiVsMuon1Tau2CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())), cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2_Tau2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon1Tau2ProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau2MassReco;
	    if(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).first) {_hMuon1Tau2ReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    else {_hMuon1Tau2NotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    _hMuon1Tau2_Muon1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2_Tau2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(tj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _hMuon1Tau2OSLS[i][NpdfID]->Fill(Muon_charge->at(mj) * Tau_charge->at(tj),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2PZeta[i][NpdfID]->Fill(CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2PZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2Zeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon1Tau2Zeta1D[i][NpdfID]->Fill((_Muon1Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))) + 
					       (_Muon1Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	  }
	  if ((passRecoTau1Cuts(tj)) && (passRecoMuon2Cuts(mj)) && (passMuon2Tau1TopologyCuts(tj,mj))) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hMuon2Tau1_Tau1DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	      _hMuon2Tau1_Muon2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
              _UseVectorSumOfVisProductsAndMetMassReco = "1";
              _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau1MassReco;
	      _hMuon2Tau1DiJetReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(TheLeadDiJetVect,CalculateTheDiJet4Momentum(smearedMuonMomentumVector.at(mj),smearedTauMomentumVector.at(tj)).second).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            }
            _hMuon2Tau1_Muon2IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(mj)).first,isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2PtVsTau1Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(mj).Pt(),smearedTauMomentumVector.at(tj).Pt(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1DeltaR[i][NpdfID]->Fill(smearedTauMomentumVector.at(tj).DeltaR(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1DeltaPtDivSumPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()) / (smearedTauMomentumVector.at(tj).Pt() + smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1DeltaPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1_Muon2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2MetDeltaPhiVsMuon2Tau1CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())), cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1_Tau1MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon2Tau1ProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau1MassReco;
	    if(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).first) {_hMuon2Tau1ReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    else {_hMuon2Tau1NotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    _hMuon2Tau1_Muon2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1_Tau1MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(tj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _hMuon2Tau1OSLS[i][NpdfID]->Fill(Muon_charge->at(mj) * Tau_charge->at(tj),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1PZeta[i][NpdfID]->Fill(CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1PZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1Zeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau1Zeta1D[i][NpdfID]->Fill((_Muon2Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))) + 
					       (_Muon2Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	  }
	  if ((passRecoTau2Cuts(tj)) && (passRecoMuon2Cuts(mj)) && (passMuon2Tau2TopologyCuts(tj,mj))) {
            if ((theLeadingJetIndex >= 0) && (theSecondLeadingJetIndex >= 0)) {
              TheLeadDiJetVect = CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(theLeadingJetIndex),smearedJetMomentumVector.at(theSecondLeadingJetIndex)).second;
	      _hMuon2Tau2_Tau2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	      _hMuon2Tau2_Muon2DiJetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - TheLeadDiJetVect.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
              _UseVectorSumOfVisProductsAndMetMassReco = "1";
              _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau2MassReco;
	      _hMuon2Tau2DiJetReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(TheLeadDiJetVect,CalculateTheDiJet4Momentum(smearedMuonMomentumVector.at(mj),smearedTauMomentumVector.at(tj)).second).second.M(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            }
            _hMuon2Tau2_Muon2IsZmm[i][NpdfID]->Fill(isZmm(smearedMuonMomentumVector.at(mj)).first,isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2PtVsTau2Pt[i][NpdfID]->Fill(smearedMuonMomentumVector.at(mj).Pt(),smearedTauMomentumVector.at(tj).Pt(),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2DeltaR[i][NpdfID]->Fill(smearedTauMomentumVector.at(tj).DeltaR(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2DeltaPtDivSumPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()) / (smearedTauMomentumVector.at(tj).Pt() + smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2DeltaPt[i][NpdfID]->Fill((smearedTauMomentumVector.at(tj).Pt() - smearedMuonMomentumVector.at(mj).Pt()),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2CosDphi[i][NpdfID]->Fill(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2_Muon2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2MetDeltaPhiVsMuon2Tau2CosDphi[i][NpdfID]->Fill(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - theMETVector.Phi())), cos(abs(normalizedPhi(smearedMuonMomentumVector.at(mj).Phi() - smearedTauMomentumVector.at(tj).Phi()))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2_Tau2MetDeltaPhi[i][NpdfID]->Fill(abs(normalizedPhi(smearedTauMomentumVector.at(tj).Phi() - theMETVector.Phi())),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon2Tau2ProductsAndMetMassReco;
            _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau2MassReco;
	    if(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).first) {_hMuon2Tau2ReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    else {_hMuon2Tau2NotReconstructableMass[i][NpdfID]->Fill(CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)).second.M(),isrgluon_weight * pdfWeightVector.at(NpdfID));}
	    _hMuon2Tau2_Muon2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedMuonMomentumVector.at(mj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2_Tau2MetMt[i][NpdfID]->Fill(CalculateLeptonMetMt(smearedTauMomentumVector.at(tj)),isrgluon_weight  * pdfWeightVector.at(NpdfID));
            _hMuon2Tau2OSLS[i][NpdfID]->Fill(Muon_charge->at(mj) * Tau_charge->at(tj),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2PZeta[i][NpdfID]->Fill(CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2PZetaVis[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2Zeta2D[i][NpdfID]->Fill(CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj)),isrgluon_weight * pdfWeightVector.at(NpdfID));
	    _hMuon2Tau2Zeta1D[i][NpdfID]->Fill((_Muon2Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))) + 
					       (_Muon2Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(tj),smearedMuonMomentumVector.at(mj))),isrgluon_weight  * pdfWeightVector.at(NpdfID));
	  }
	}
      }


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

    if (_FillRecoVertexHists == "1") {
      _hNPVertices[i][NpdfCounter]      = new TH1F(("NPVertices_"+j.str()).c_str(),      ("NPVertices_"+j.str()).c_str(),       100, 0., 100.);
    }
    
    //--- book generator level histograms
    if (_FillGenHists == "1") {
      _hNGenTau[i][NpdfCounter]                         = new TH1F(("NGenTau_"+j.str()).c_str(),                       ("NGenTau_"+j.str()).c_str(),     20, 0., 20.);
      _hNGenHadTau[i][NpdfCounter]                      = new TH1F(("NGenHadTau_"+j.str()).c_str(),                    ("NGenHadTau_"+j.str()).c_str(),     20, 0., 20.);
      _hGenTauEnergy[i][NpdfCounter]                    = new TH1F(("GenTauEnergy_"+j.str()).c_str(),                  ("GenTauEnergy_"+j.str()).c_str(), 500, 0., 5000.);
      _hGenTauPt[i][NpdfCounter]                        = new TH1F(("GenTauPt_"+j.str()).c_str(),                      ("GenTauPt_"+j.str()).c_str(),    500, 0., 5000.);
      _hGenTauEta[i][NpdfCounter]                       = new TH1F(("GenTauEta_"+j.str()).c_str(),                     ("GenTauEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenHadTauPt[i][NpdfCounter]                     = new TH1F(("GenHadTauPt_"+j.str()).c_str(),                   ("GenHadTauPt_"+j.str()).c_str(),    500, 0., 5000.);
      _hGenHadTauEta[i][NpdfCounter]                    = new TH1F(("GenHadTauEta_"+j.str()).c_str(),                  ("GenHadTauEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenTauPhi[i][NpdfCounter]                       = new TH1F(("GenTauPhi_"+j.str()).c_str(),                     ("GenTauPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hNGenMuon[i][NpdfCounter]                         = new TH1F(("NGenMuon_"+j.str()).c_str(),                       ("NGenMuon_"+j.str()).c_str(),     20, 0., 20.);
      _hGenMuonEnergy[i][NpdfCounter]                    = new TH1F(("GenMuonEnergy_"+j.str()).c_str(),                  ("GenMuonEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hGenMuonPt[i][NpdfCounter]                        = new TH1F(("GenMuonPt_"+j.str()).c_str(),                      ("GenMuonPt_"+j.str()).c_str(),    200, 0., 500.);
      _hGenMuonEta[i][NpdfCounter]                       = new TH1F(("GenMuonEta_"+j.str()).c_str(),                     ("GenMuonEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hGenMuonPhi[i][NpdfCounter]                       = new TH1F(("GenMuonPhi_"+j.str()).c_str(),                     ("GenMuonPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hGenDiTauMass[i][NpdfCounter]                     = new TH1F(("GenDiTauMass_"+j.str()).c_str(),                   ("GenDiTauMass_"+j.str()).c_str(),    100, 0., 10000.);
      _hGenZprimeMass[i][NpdfCounter]                    = new TH1F(("GenZprimeMass_"+j.str()).c_str(),                  ("GenZprimeMass_"+j.str()).c_str(),    100, 0., 10000.);
      _hGenZprimeStatusCode[i][NpdfCounter]              = new TH1F(("GenZprimeStatusCode_"+j.str()).c_str(),            ("GenZprimeStatusCode_"+j.str()).c_str(),     100, 0., 100.);
      _hGenTauStatusCode[i][NpdfCounter]                 = new TH1F(("GenTauStatusCode_"+j.str()).c_str(),               ("GenTauStatusCode_"+j.str()).c_str(),     100, 0., 100.);
    }

    if (_FillRecoTauHists == "1") {
      _hNTau1[i][NpdfCounter]                                    = new TH1F(("NTau1_"+j.str()).c_str(),                                    ("NTau1_"+j.str()).c_str(),         20, 0., 20.);
      _hTauJet1Energy[i][NpdfCounter]                            = new TH1F(("TauJet1Energy_"+j.str()).c_str(),                            ("TauJet1Energy_"+j.str()).c_str(), 2000, 0., 5000.);
      _hTauJet1Pt[i][NpdfCounter]                                = new TH1F(("TauJet1Pt_"+j.str()).c_str(),                                ("TauJet1Pt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hTauJet1Eta[i][NpdfCounter]                               = new TH1F(("TauJet1Eta_"+j.str()).c_str(),                               ("TauJet1Eta_"+j.str()).c_str(),    100, -5.0, +5.0);
      _hFirstLeadingTauJet1Pt[i][NpdfCounter]                    = new TH1F(("FirstLeadingTauJet1Pt_"+j.str()).c_str(),                    ("FirstLeadingTauJet1Pt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hFirstLeadingTauJet1Eta[i][NpdfCounter]                   = new TH1F(("FirstLeadingTauJet1Eta_"+j.str()).c_str(),                   ("FirstLeadingTauJet1Eta_"+j.str()).c_str(),    144, -7.2, +7.2);
      _hTauJet1Phi[i][NpdfCounter]                               = new TH1F(("TauJet1Phi_"+j.str()).c_str(),                               ("TauJet1Phi_"+j.str()).c_str(),    36, -TMath::Pi(), +TMath::Pi());
      _hTauJet1NumSignalTracks[i][NpdfCounter]                   = new TH1F(("TauJet1NumSignalTracks_"+j.str()).c_str(),                   ("TauJet1NumSignalTracks_"+j.str()).c_str(), 10, 0, 10);
      _hTauJet1SeedTrackPt[i][NpdfCounter]                       = new TH1F(("TauJet1SeedTrackPt_"+j.str()).c_str(),                       ("TauJet1SeedTrackPt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hTauJet1Charge[i][NpdfCounter]                            = new TH1F(("TauJet1Charge_"+j.str()).c_str(),                            ("TauJet1Charge_"+j.str()).c_str(), 10, -5., 5.);
      _hNTau2[i][NpdfCounter]                                    = new TH1F(("NTau2_"+j.str()).c_str(),                                    ("NTau2_"+j.str()).c_str(),         20, 0., 20.);
      _hTauJet2Energy[i][NpdfCounter]                            = new TH1F(("TauJet2Energy_"+j.str()).c_str(),                            ("TauJet2Energy_"+j.str()).c_str(), 2000, 0., 5000.);
      _hTauJet2Pt[i][NpdfCounter]                                = new TH1F(("TauJet2Pt_"+j.str()).c_str(),                                ("TauJet2Pt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hTauJet2Eta[i][NpdfCounter]                               = new TH1F(("TauJet2Eta_"+j.str()).c_str(),                               ("TauJet2Eta_"+j.str()).c_str(),    100, -5.0, +5.0);
      _hFirstLeadingTauJet2Pt[i][NpdfCounter]                    = new TH1F(("FirstLeadingTauJet2Pt_"+j.str()).c_str(),                    ("FirstLeadingTauJet2Pt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hFirstLeadingTauJet2Eta[i][NpdfCounter]                   = new TH1F(("FirstLeadingTauJet2Eta_"+j.str()).c_str(),                   ("FirstLeadingTauJet2Eta_"+j.str()).c_str(),    144, -7.2, +7.2);
      _hTauJet2Phi[i][NpdfCounter]                               = new TH1F(("TauJet2Phi_"+j.str()).c_str(),                               ("TauJet2Phi_"+j.str()).c_str(),    36, -TMath::Pi(), +TMath::Pi());
      _hTauJet2NumSignalTracks[i][NpdfCounter]                   = new TH1F(("TauJet2NumSignalTracks_"+j.str()).c_str(),                   ("TauJet2NumSignalTracks_"+j.str()).c_str(), 10, 0, 10);
      _hTauJet2SeedTrackPt[i][NpdfCounter]                       = new TH1F(("TauJet2SeedTrackPt_"+j.str()).c_str(),                       ("TauJet2SeedTrackPt_"+j.str()).c_str(),     2000, 0., 5000.);
      _hTauJet2Charge[i][NpdfCounter]                            = new TH1F(("TauJet2Charge_"+j.str()).c_str(),                            ("TauJet2Charge_"+j.str()).c_str(), 10, -5., 5.);
    }

    if (_FillRecoMuonHists == "1") {
      _hNMuon1[i][NpdfCounter]                       = new TH1F(("NMuon1_"+j.str()).c_str(),                   ("NMuon1_"+j.str()).c_str(), 20, 0., 20.);
      _hMuon1Energy[i][NpdfCounter]                  = new TH1F(("Muon1Energy_"+j.str()).c_str(),              ("Muon1Energy_"+j.str()).c_str(), 2000, 0., 5000.);
      _hMuon1Pt[i][NpdfCounter]                      = new TH1F(("Muon1Pt_"+j.str()).c_str(),                  ("Muon1Pt_"+j.str()).c_str(),  2000, 0., 5000.);
      _hMuon1Eta[i][NpdfCounter]                     = new TH1F(("Muon1Eta_"+j.str()).c_str(),                 ("Muon1Eta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hFirstLeadingMuon1Pt[i][NpdfCounter]          = new TH1F(("FirstLeadingMuon1Pt_"+j.str()).c_str(),      ("FirstLeadingMuon1Pt_"+j.str()).c_str(),  400, 0., 1000.);
      _hFirstLeadingMuon1Eta[i][NpdfCounter]         = new TH1F(("FirstLeadingMuon1Eta_"+j.str()).c_str(),     ("FirstLeadingMuon1Eta_"+j.str()).c_str(), 144, -7.2, +7.2);
      _hMuon1Phi[i][NpdfCounter]                     = new TH1F(("Muon1Phi_"+j.str()).c_str(),                 ("Muon1Phi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hMuon1MetMt[i][NpdfCounter]                   = new TH1F(("Muon1MetMt_"+j.str()).c_str(),       ("Muon1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hNMuon2[i][NpdfCounter]                       = new TH1F(("NMuon2_"+j.str()).c_str(),                   ("NMuon2_"+j.str()).c_str(), 20, 0., 20.);
      _hMuon2Energy[i][NpdfCounter]                  = new TH1F(("Muon2Energy_"+j.str()).c_str(),              ("Muon2Energy_"+j.str()).c_str(), 2000, 0., 5000.);
      _hMuon2Pt[i][NpdfCounter]                      = new TH1F(("Muon2Pt_"+j.str()).c_str(),                  ("Muon2Pt_"+j.str()).c_str(),  2000, 0., 5000.);
      _hMuon2Eta[i][NpdfCounter]                     = new TH1F(("Muon2Eta_"+j.str()).c_str(),                 ("Muon2Eta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hFirstLeadingMuon2Pt[i][NpdfCounter]          = new TH1F(("FirstLeadingMuon2Pt_"+j.str()).c_str(),      ("FirstLeadingMuon2Pt_"+j.str()).c_str(),  400, 0., 1000.);
      _hFirstLeadingMuon2Eta[i][NpdfCounter]         = new TH1F(("FirstLeadingMuon2Eta_"+j.str()).c_str(),     ("FirstLeadingMuon2Eta_"+j.str()).c_str(), 144, -7.2, +7.2);
      _hMuon2Phi[i][NpdfCounter]                     = new TH1F(("Muon2Phi_"+j.str()).c_str(),                 ("Muon2Phi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hMuon2MetMt[i][NpdfCounter]                   = new TH1F(("Muon2MetMt_"+j.str()).c_str(),       ("Muon2MetMt_"+j.str()).c_str(), 100, 0, 500);
    }

    if (_FillRecoJetHists == "1") {
      _hNJet1[i][NpdfCounter]            = new TH1F(("NJet1_"+j.str()).c_str(),           ("NJet1_"+j.str()).c_str(), 20, 0., 20.);
      _hNJet2[i][NpdfCounter]            = new TH1F(("NJet2_"+j.str()).c_str(),           ("NJet2_"+j.str()).c_str(), 20, 0., 20.);
      _hNCentralJet[i][NpdfCounter]      = new TH1F(("NCentralJet_"+j.str()).c_str(),     ("NCentralJet_"+j.str()).c_str(), 20, 0., 20.);
      _hNBJet[i][NpdfCounter]            = new TH1F(("NBJet_"+j.str()).c_str(),           ("NBJet_"+j.str()).c_str(), 20, 0., 20.);
      _hJet1Energy[i][NpdfCounter]       = new TH1F(("Jet1Energy_"+j.str()).c_str(),      ("Jet1Energy_"+j.str()).c_str(), 200, 0., 500.);
      _hJet1Pt[i][NpdfCounter]           = new TH1F(("Jet1Pt_"+j.str()).c_str(),          ("Jet1Pt_"+j.str()).c_str(), 200, 0., 500.);
      _hJet2Energy[i][NpdfCounter]       = new TH1F(("Jet2Energy_"+j.str()).c_str(),      ("Jet2Energy_"+j.str()).c_str(), 200, 0., 500.);
      _hJet2Pt[i][NpdfCounter]           = new TH1F(("Jet2Pt_"+j.str()).c_str(),          ("Jet2Pt_"+j.str()).c_str(), 200, 0., 500.);
      _hCentralJetPt[i][NpdfCounter]     = new TH1F(("CentralJetPt_"+j.str()).c_str(),    ("CentralJetPt_"+j.str()).c_str(), 200, 0., 500.);
      _hCentralJetEta[i][NpdfCounter]    = new TH1F(("CentralJetEta_"+j.str()).c_str(),   ("CentralJetEta_"+j.str()).c_str(), 100, -5.0, +5.0);
      _hJet1Eta[i][NpdfCounter]          = new TH1F(("Jet1Eta_"+j.str()).c_str(),         ("Jet1Eta_"+j.str()).c_str(), 100, -5.0, +5.0);
      _hJet1Phi[i][NpdfCounter]          = new TH1F(("Jet1Phi_"+j.str()).c_str(),         ("Jet1Phi_"+j.str()).c_str(), 144, -2. * TMath::Pi(), +2. * TMath::Pi());
      _hJet2Eta[i][NpdfCounter]          = new TH1F(("Jet2Eta_"+j.str()).c_str(),         ("Jet2Eta_"+j.str()).c_str(), 100, -5.0, +5.0);
      _hJet2Phi[i][NpdfCounter]          = new TH1F(("Jet2Phi_"+j.str()).c_str(),         ("Jet2Phi_"+j.str()).c_str(), 144, -2. * TMath::Pi(), +2. * TMath::Pi());
      _hBJetEnergy[i][NpdfCounter]       = new TH1F(("BJetEnergy_"+j.str()).c_str(),      ("BJetEnergy_"+j.str()).c_str(), 200, 0., 500.);
      _hBJetPt[i][NpdfCounter]           = new TH1F(("BJetPt_"+j.str()).c_str(),          ("BJetPt_"+j.str()).c_str(), 200, 0., 500.);
      _hBJetEta[i][NpdfCounter]          = new TH1F(("BJetEta_"+j.str()).c_str(),         ("BJetEta_"+j.str()).c_str(), 72, -3.6, +3.6);
      _hBJetPhi[i][NpdfCounter]          = new TH1F(("BJetPhi_"+j.str()).c_str(),         ("BJetPhi_"+j.str()).c_str(), 36, -TMath::Pi(), +TMath::Pi());
      _hFirstLeadingJetPt[i][NpdfCounter]          = new TH1F(("FirstLeadingJetPt_"+j.str()).c_str(),         ("FirstLeadingJetPt_"+j.str()).c_str(), 200, 0., 1000.);
      _hSecondLeadingJetPt[i][NpdfCounter]         = new TH1F(("SecondLeadingJetPt_"+j.str()).c_str(),        ("SecondLeadingJetPt_"+j.str()).c_str(), 200, 0., 1000.);
      _hFirstLeadingJetEta[i][NpdfCounter]         = new TH1F(("FirstLeadingJetEta_"+j.str()).c_str(),        ("FirstLeadingJetEta_"+j.str()).c_str(), 100, -5., 5.);
      _hSecondLeadingJetEta[i][NpdfCounter]        = new TH1F(("SecondLeadingJetEta_"+j.str()).c_str(),       ("SecondLeadingJetEta_"+j.str()).c_str(), 100, -5., 5.);
      _hMHT[i][NpdfCounter]                        = new TH1F(("MHT_"+j.str()).c_str(),                       ("MHT_"+j.str()).c_str(), 500, 0, 5000);
      _hHT[i][NpdfCounter]                         = new TH1F(("HT_"+j.str()).c_str(),                        ("HT_"+j.str()).c_str(), 500, 0, 5000);
      _hMeff[i][NpdfCounter]                       = new TH1F(("Meff_"+j.str()).c_str(),                      ("Meff_"+j.str()).c_str(), 500, 0, 5000);
      _hLeadDiJetMass[i][NpdfCounter]              = new TH1F(("LeadDiJetMass_"+j.str()).c_str(),             ("LeadDiJetMass_"+j.str()).c_str(), 1000, 0, 5000);
      _hLeadDiJetPt[i][NpdfCounter]                = new TH1F(("LeadDiJetPt_"+j.str()).c_str(),               ("LeadDiJetPt_"+j.str()).c_str(), 1000, 0, 5000);
      _hLeadDiJetEtaProduct[i][NpdfCounter]        = new TH1F(("LeadDiJetEtaProduct_"+j.str()).c_str(),       ("LeadDiJetEtaProduct_"+j.str()).c_str(), 4, -2, 2);
      _hDiJetMass[i][NpdfCounter]                  = new TH1F(("DiJetMass_"+j.str()).c_str(),                 ("DiJetMass_"+j.str()).c_str(), 1000, 0, 5000);
      _hDiJetPt[i][NpdfCounter]                    = new TH1F(("DiJetPt_"+j.str()).c_str(),                   ("DiJetPt_"+j.str()).c_str(), 1000, 0, 5000);
      _hLeadingJetsMass[i][NpdfCounter]            = new TH1F(("LeadingJetsMass_"+j.str()).c_str(),           ("LeadingJetsMass_"+j.str()).c_str(), 1000, 0, 5000);
      _hLeadSublDijetDphi[i][NpdfCounter]          = new TH1F(("LeadSublDijetDphi_"+j.str()).c_str(),         ("LeadSublDijetDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMetVsDiJetDeltaPhiLeadSubl[i][NpdfCounter] = new TH2F(("MetVsDiJetDeltaPhiLeadSubl_"+j.str()).c_str(),("MetVsDiJetDeltaPhiLeadSubl_"+j.str()).c_str(), 100, 0, 1000., 72, 0, +TMath::Pi());
      _hDeltaEtaVsDeltaPhiLeadSubl[i][NpdfCounter] = new TH2F(("DeltaEtaVsDeltaPhiLeadSubl_"+j.str()).c_str(),("DeltaEtaVsDeltaPhiLeadSubl_"+j.str()).c_str(), 200, 0, 10., 72, 0, +TMath::Pi());
      _hLeadingJetsPt[i][NpdfCounter]              = new TH1F(("LeadingJetsPt_"+j.str()).c_str(),             ("LeadingJetsPt_"+j.str()).c_str(), 1000, 0, 5000);
      _hDiJetDeltaR[i][NpdfCounter]                = new TH1F(("DiJetDeltaR_"+j.str()).c_str(),               ("DiJetDeltaR_"+j.str()).c_str(), 200, 0, 10.);
      _hDiJetDeltaEta[i][NpdfCounter]              = new TH1F(("DiJetDeltaEta_"+j.str()).c_str(),             ("DiJetDeltaEta_"+j.str()).c_str(), 200, 0, 10.);
      _hDiJetDeltaPhi[i][NpdfCounter]              = new TH1F(("DiJetDeltaPhi_"+j.str()).c_str(),             ("DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hLeadDiJetDeltaR[i][NpdfCounter]            = new TH1F(("LeadDiJetDeltaR_"+j.str()).c_str(),           ("LeadDiJetDeltaR_"+j.str()).c_str(), 200, 0, 10.);
      _hLeadDiJetDeltaEta[i][NpdfCounter]          = new TH1F(("LeadDiJetDeltaEta_"+j.str()).c_str(),         ("LeadDiJetDeltaEta_"+j.str()).c_str(), 200, 0, 10.);
      _hLeadDiJetDeltaPhi[i][NpdfCounter]          = new TH1F(("LeadDiJetDeltaPhi_"+j.str()).c_str(),         ("LeadDiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hLeadingJetsDeltaR[i][NpdfCounter]          = new TH1F(("LeadingJetsDeltaR_"+j.str()).c_str(),         ("LeadingJetsDeltaR_"+j.str()).c_str(), 200, 0, 10.);
      _hLeadingJetsDeltaEta[i][NpdfCounter]        = new TH1F(("LeadingJetsDeltaEta_"+j.str()).c_str(),       ("LeadingJetsDeltaEta_"+j.str()).c_str(), 200, 0, 10.);
      _hR1[i][NpdfCounter] 		       = new TH1F(("R1_"+j.str()).c_str(),                    ("R1_"+j.str()).c_str(), 60, 0, 6);
      _hR2[i][NpdfCounter]                     = new TH1F(("R2_"+j.str()).c_str(),                    ("R2_"+j.str()).c_str(), 60, 0, 6);
      _hDphi1MHT[i][NpdfCounter]               = new TH1F(("Dphi1_"+j.str()).c_str(),                 ("Dphi1_"+j.str()).c_str(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi());
      _hDphi2MHT[i][NpdfCounter]               = new TH1F(("Dphi2MHT_"+j.str()).c_str(),              ("Dphi2MHT_"+j.str()).c_str(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi());
      _hDphi1[i][NpdfCounter]                  = new TH1F(("Dphi1MHT_"+j.str()).c_str(),              ("Dphi1MHT_"+j.str()).c_str(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi());
      _hDphi2[i][NpdfCounter]                  = new TH1F(("Dphi2_"+j.str()).c_str(),                 ("Dphi2_"+j.str()).c_str(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi());
      _hDphi1VsDphi2[i][NpdfCounter]           = new TH2F(("Dphi1VsDphi2_"+j.str()).c_str(),          ("Dphi1VsDphi2_"+j.str()).c_str(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi(), 72, -2.0 * TMath::Pi(), +2.0 * TMath::Pi());
      _hAlpha[i][NpdfCounter]                  = new TH1F(("Alpha_"+j.str()).c_str(),                 ("Alpha_"+j.str()).c_str(), 50, 0, 2);
    }

    if (_FillTopologyHists == "1") {
      _hMet[i][NpdfCounter]                    = new TH1F(("Met_"+j.str()).c_str(),                   ("Met_"+j.str()).c_str(), 100, 0, 1000);
      _hMetDiJetDeltaPhi[i][NpdfCounter]                     = new TH1F(("MetDiJetDeltaPhi_"+j.str()).c_str(),                     ("MetDiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Muon2_Muon1DiJetDeltaPhi[i][NpdfCounter]        = new TH1F(("Muon1Muon2_Muon1DiJetDeltaPhi_"+j.str()).c_str(),        ("Muon1Muon2_Muon1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Muon2_Muon2DiJetDeltaPhi[i][NpdfCounter]        = new TH1F(("Muon1Muon2_Muon2DiJetDeltaPhi_"+j.str()).c_str(),        ("Muon1Muon2_Muon2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi()); 
      _hMuon1PtVsMuon2Pt[i][NpdfCounter]               = new TH2F(("Muon1PtVsMuon2Pt_"+j.str()).c_str(),                   ("Muon1PtVsMuon2Pt_"+j.str()).c_str(), 100, 0, 500, 100, 0, 500);
      _hMuon1Muon2_Muon1IsZmm[i][NpdfCounter]          = new TH1F(("Muon1Muon2_Muon1IsZmm_"+j.str()).c_str(),              ("Muon1Muon2_Muon1IsZmm_"+j.str()).c_str(), 2, 0, 2);
      _hMuon1Muon2_Muon2IsZmm[i][NpdfCounter]          = new TH1F(("Muon1Muon2_Muon2IsZmm_"+j.str()).c_str(),              ("Muon1Muon2_Muon2IsZmm_"+j.str()).c_str(), 2, 0, 2);
      _hMuon1Muon2DeltaR[i][NpdfCounter]               = new TH1F(("Muon1Muon2DeltaR_"+j.str()).c_str(),                   ("Muon1Muon2DeltaR_"+j.str()).c_str(), 100, 0, 5.);
      _hMuon1Muon2DeltaPtDivSumPt[i][NpdfCounter]      = new TH1F(("Muon1Muon2DeltaPtDivSumPt_"+j.str()).c_str(),          ("Muon1Muon2DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hMuon1Muon2DeltaPt[i][NpdfCounter]              = new TH1F(("Muon1Muon2DeltaPt_"+j.str()).c_str(),                  ("Muon1Muon2DeltaPt_"+j.str()).c_str(), 100, 0, 1000);
      _hDiMuon_Muon1MetMt[i][NpdfCounter]              = new TH1F(("DiMuon_Muon1MetMt_"+j.str()).c_str(),                  ("DiMuon_Muon1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hDiMuon_Muon2MetMt[i][NpdfCounter]              = new TH1F(("DiMuon_Muon2MetMt_"+j.str()).c_str(),                  ("DiMuon_Muon2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon1Muon2OSLS[i][NpdfCounter]                 = new TH1F(("Muon1Muon2OSLS_"+j.str()).c_str(),                     ("Muon1Muon2OSLS_"+j.str()).c_str(), 20, -10, 10);
      _hMuon1Muon2CosDphi[i][NpdfCounter]              = new TH1F(("Muon1Muon2CosDphi_"+j.str()).c_str(),                  ("Muon1Muon2CosDphi_"+j.str()).c_str(), 220, -1.1, 1.1);
      _hDiMuon_Muon1MetDeltaPhi[i][NpdfCounter]        = new TH1F(("DiMuon_Muon1MetDeltaPhi_"+j.str()).c_str(),            ("DiMuon_Muon1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hDiMuon_Muon2MetDeltaPhi[i][NpdfCounter]        = new TH1F(("DiMuon_Muon2MetDeltaPhi_"+j.str()).c_str(),            ("DiMuon_Muon2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1MetDeltaPhiVsMuon1Muon2CosDphi[i][NpdfCounter]       = new TH2F(("Muon1MetDeltaPhiVsMuon1Muon2CosDphi_"+j.str()).c_str(), ("Muon1MetDeltaPhiVsMuon1Muon2CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hDiMuonPZeta[i][NpdfCounter]            = new TH1F(("DiMuonPZeta_"+j.str()).c_str(),           ("DiMuonPZeta_"+j.str()).c_str(), 200, -100, 100);
      _hDiMuonPZetaVis[i][NpdfCounter]         = new TH1F(("DiMuonPZetaVis_"+j.str()).c_str(),        ("DiMuonPZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hDiMuonZeta2D[i][NpdfCounter]           = new TH2F(("DiMuonZeta2D_"+j.str()).c_str(),          ("DiMuonZeta2D_"+j.str()).c_str(), 100, 0, 100, 200, -100, 100);
      _hDiMuonZeta1D[i][NpdfCounter]           = new TH1F(("DiMuonZeta1D_"+j.str()).c_str(),          ("DiMuonZeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hDiMuonNotReconstructableMass[i][NpdfCounter]   = new TH1F(("DiMuonNotReconstructableMass_"+j.str()).c_str(),          ("DiMuonNotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hDiMuonReconstructableMass[i][NpdfCounter]      = new TH1F(("DiMuonReconstructableMass_"+j.str()).c_str(),             ("DiMuonReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hTau1Tau2_Tau1DiJetDeltaPhi[i][NpdfCounter]                = new TH1F(("Tau1Tau2_Tau1DiJetDeltaPhi_"+j.str()).c_str(),                ("Tau1Tau2_Tau1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi()); 
      _hTau1Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]                = new TH1F(("Tau1Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(),                 ("Tau1Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hTau1PtVsTau2Pt[i][NpdfCounter]                 = new TH2F(("Tau1PtVsTau2Pt_"+j.str()).c_str(),                     ("Tau1PtVsTau2Pt_"+j.str()).c_str(), 100, 0, 500, 100, 0, 500);
      _hTau1Tau2DeltaR[i][NpdfCounter]                 = new TH1F(("Tau1Tau2DeltaR_"+j.str()).c_str(),                     ("Tau1Tau2DeltaR_"+j.str()).c_str(), 100, 0, 5.);
      _hTau1Tau2DeltaPtDivSumPt[i][NpdfCounter]        = new TH1F(("Tau1Tau2DeltaPtDivSumPt_"+j.str()).c_str(),            ("Tau1Tau2DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hTau1Tau2DeltaPt[i][NpdfCounter]                = new TH1F(("Tau1Tau2DeltaPt_"+j.str()).c_str(),                    ("Tau1Tau2DeltaPt_"+j.str()).c_str(), 100, 0, 1000);
      _hDiTau_Tau1MetMt[i][NpdfCounter]                = new TH1F(("DiTau_Tau1MetMt_"+j.str()).c_str(),                    ("DiTau_Tau1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hDiTau_Tau2MetMt[i][NpdfCounter]                = new TH1F(("DiTau_Tau2MetMt_"+j.str()).c_str(),                    ("DiTau_Tau2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hTau1Tau2OSLS[i][NpdfCounter]                   = new TH1F(("Tau1Tau2OSLS_"+j.str()).c_str(),                       ("Tau1Tau2OSLS_"+j.str()).c_str(), 20, -10, 10);
      _hTau1Tau2CosDphi[i][NpdfCounter]                = new TH1F(("Tau1Tau2CosDphi_"+j.str()).c_str(),                    ("Tau1Tau2CosDphi_"+j.str()).c_str(), 220, -1.1, 1.1);
      _hDiTau_Tau1MetDeltaPhi[i][NpdfCounter]          = new TH1F(("DiTau_Tau1MetDeltaPhi_"+j.str()).c_str(),              ("DiTau_Tau1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hDiTau_Tau2MetDeltaPhi[i][NpdfCounter]          = new TH1F(("DiTau_Tau2MetDeltaPhi_"+j.str()).c_str(),              ("DiTau_Tau2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hTau1MetDeltaPhiVsTau1Tau2CosDphi[i][NpdfCounter] = new TH2F(("Tau1MetDeltaPhiVsTau1Tau2CosDphi_"+j.str()).c_str(), ("Tau1MetDeltaPhiVsTau1Tau2CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hDiTauPZeta[i][NpdfCounter]             = new TH1F(("DiTauPZeta_"+j.str()).c_str(),            ("DiTauPZeta_"+j.str()).c_str(), 200, -100, 100);
      _hDiTauPZetaVis[i][NpdfCounter]          = new TH1F(("DiTauPZetaVis_"+j.str()).c_str(),         ("DiTauPZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hDiTauZeta2D[i][NpdfCounter]            = new TH2F(("DiTauZeta2D_"+j.str()).c_str(),           ("DiTauZeta2D_"+j.str()).c_str(), 100, 0, 100, 200, -100, 100);
      _hDiTauZeta1D[i][NpdfCounter]            = new TH1F(("DiTauZeta1D_"+j.str()).c_str(),           ("DiTauZeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hDiTauNotReconstructableMass[i][NpdfCounter]           = new TH1F(("DiTauNotReconstructableMass_"+j.str()).c_str(),           ("DiTauNotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hDiTauReconstructableMass[i][NpdfCounter]              = new TH1F(("DiTauReconstructableMass_"+j.str()).c_str(),              ("DiTauReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hDiTauDiJetReconstructableMass[i][NpdfCounter]         = new TH1F(("DiTauDiJetReconstructableMass_"+j.str()).c_str(),              ("DiTauDiJetReconstructableMass_"+j.str()).c_str(), 100, 0, 5000);
      _hMuon1Tau1_Tau1DiJetDeltaPhi[i][NpdfCounter]          = new TH1F(("Muon1Tau1_Tau1DiJetDeltaPhi_"+j.str()).c_str(),          ("Muon1Tau1_Tau1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau1_Muon1DiJetDeltaPhi[i][NpdfCounter]         = new TH1F(("Muon1Tau1_Muon1DiJetDeltaPhi_"+j.str()).c_str(),         ("Muon1Tau1_Muon1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau1_Muon1IsZmm[i][NpdfCounter]           = new TH1F(("Muon1Tau1_Muon1IsZmm_"+j.str()).c_str(),               ("Muon1Tau1_Muon1IsZmm_"+j.str()).c_str(), 2, 0., 2.);
      _hMuon1PtVsTau1Pt[i][NpdfCounter]                = new TH2F(("Muon1PtVsTau1Pt_"+j.str()).c_str(),                    ("Muon1PtVsTau1Pt_"+j.str()).c_str(), 100, 0, 500, 100, 0, 500);
      _hMuon1Tau1DeltaR[i][NpdfCounter]                = new TH1F(("Muon1Tau1DeltaR_"+j.str()).c_str(),                    ("Muon1Tau1DeltaR_"+j.str()).c_str(), 100, 0, 5.);
      _hMuon1Tau1DeltaPtDivSumPt[i][NpdfCounter]       = new TH1F(("Muon1Tau1DeltaPtDivSumPt_"+j.str()).c_str(),           ("Muon1Tau1DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hMuon1Tau1DeltaPt[i][NpdfCounter]               = new TH1F(("Muon1Tau1DeltaPt_"+j.str()).c_str(),                   ("Muon1Tau1DeltaPt_"+j.str()).c_str(), 100, 0, 1000);
      _hMuon1Tau1_Muon1MetMt[i][NpdfCounter]           = new TH1F(("Muon1Tau1_Muon1MetMt_"+j.str()).c_str(),               ("Muon1Tau1_Muon1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon1Tau1_Tau1MetMt[i][NpdfCounter]            = new TH1F(("Muon1Tau1_Tau1MetMt_"+j.str()).c_str(),                ("Muon1Tau1_Tau1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon1Tau1OSLS[i][NpdfCounter]                  = new TH1F(("Muon1Tau1OSLS_"+j.str()).c_str(),                      ("Muon1Tau1OSLS_"+j.str()).c_str(), 20, -10, 10);
      _hMuon1Tau1CosDphi[i][NpdfCounter]               = new TH1F(("Muon1Tau1CosDphi_"+j.str()).c_str(),                   ("Muon1Tau1CosDphi_"+j.str()).c_str(), 220, -1.1, 1.1);
      _hMuon1Tau1_Muon1MetDeltaPhi[i][NpdfCounter]     = new TH1F(("Muon1Tau1_Muon1MetDeltaPhi_"+j.str()).c_str(),         ("Muon1Tau1_Muon1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau1_Tau1MetDeltaPhi[i][NpdfCounter]      = new TH1F(("Muon1Tau1_Tau1MetDeltaPhi_"+j.str()).c_str(),          ("Muon1Tau1_Tau1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1MetDeltaPhiVsMuon1Tau1CosDphi[i][NpdfCounter] = new TH2F(("Muon1MetDeltaPhiVsMuon1Tau1CosDphi_"+j.str()).c_str(), ("Muon1MetDeltaPhiVsMuon1Tau1CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hMuon1Tau1NotReconstructableMass[i][NpdfCounter]       = new TH1F(("Muon1Tau1NotReconstructableMass_"+j.str()).c_str(),       ("Muon1Tau1NotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon1Tau1ReconstructableMass[i][NpdfCounter]          = new TH1F(("Muon1Tau1ReconstructableMass_"+j.str()).c_str(),          ("Muon1Tau1ReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon1Tau1PZeta[i][NpdfCounter]         = new TH1F(("Muon1Tau1PZeta_"+j.str()).c_str(),        ("Muon1Tau1PZeta_"+j.str()).c_str(), 200, -100, 100);
      _hMuon1Tau1PZetaVis[i][NpdfCounter]      = new TH1F(("Muon1Tau1PZetaVis_"+j.str()).c_str(),     ("Muon1Tau1PZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hMuon1Tau1Zeta2D[i][NpdfCounter]        = new TH2F(("Muon1Tau1Zeta2D_"+j.str()).c_str(),       ("Muon1Tau1Zeta2D_"+j.str()).c_str(), 100, 0, 100, 200, -100, 100);
      _hMuon1Tau1Zeta1D[i][NpdfCounter]        = new TH1F(("Muon1Tau1Zeta1D_"+j.str()).c_str(),       ("Muon1Tau1Zeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hMuon1Tau1DiJetReconstructableMass[i][NpdfCounter]         = new TH1F(("Muon1Tau1DiJetReconstructableMass_"+j.str()).c_str(),              ("Muon1Tau1DiJetReconstructableMass_"+j.str()).c_str(), 100, 0, 5000);
      _hMuon1Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]          = new TH1F(("Muon1Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(),("Muon1Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau2_Muon1DiJetDeltaPhi[i][NpdfCounter]         = new TH1F(("Muon1Tau2_Muon1DiJetDeltaPhi_"+j.str()).c_str(),("Muon1Tau2_Muon1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau2_Muon1IsZmm[i][NpdfCounter]           = new TH1F(("Muon1Tau2_Muon1IsZmm_"+j.str()).c_str(),("Muon1Tau2_Muon1IsZmm_"+j.str()).c_str(), 2, 0., 2.);
      _hMuon1PtVsTau2Pt[i][NpdfCounter]                = new TH2F(("Muon1PtVsTau2Pt_"+j.str()).c_str(),                    ("Muon1PtVsTau2Pt_"+j.str()).c_str(),100, 0, 500, 100, 0, 500);
      _hMuon1Tau2DeltaR[i][NpdfCounter]                = new TH1F(("Muon1Tau2DeltaR_"+j.str()).c_str(),                    ("Muon1Tau2DeltaR_"+j.str()).c_str(),100, 0, 5.);
      _hMuon1Tau2DeltaPtDivSumPt[i][NpdfCounter]       = new TH1F(("Muon1Tau2DeltaPtDivSumPt_"+j.str()).c_str(),("Muon1Tau2DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hMuon1Tau2DeltaPt[i][NpdfCounter]               = new TH1F(("Muon1Tau2DeltaPt_"+j.str()).c_str(),                   ("Muon1Tau2DeltaPt_"+j.str()).c_str(),100, 0, 1000);
      _hMuon1Tau2_Muon1MetMt[i][NpdfCounter]           = new TH1F(("Muon1Tau2_Muon1MetMt_"+j.str()).c_str(),("Muon1Tau2_Muon1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon1Tau2_Tau2MetMt[i][NpdfCounter]            = new TH1F(("Muon1Tau2_Tau2MetMt_"+j.str()).c_str(),("Muon1Tau2_Tau2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon1Tau2OSLS[i][NpdfCounter]                  = new TH1F(("Muon1Tau2OSLS_"+j.str()).c_str(),                      ("Muon1Tau2OSLS_"+j.str()).c_str(), 20,-10, 10);
      _hMuon1Tau2CosDphi[i][NpdfCounter]               = new TH1F(("Muon1Tau2CosDphi_"+j.str()).c_str(),                   ("Muon1Tau2CosDphi_"+j.str()).c_str(),220, -1.1, 1.1);
      _hMuon1Tau2_Muon1MetDeltaPhi[i][NpdfCounter]     = new TH1F(("Muon1Tau2_Muon1MetDeltaPhi_"+j.str()).c_str(),("Muon1Tau2_Muon1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1Tau2_Tau2MetDeltaPhi[i][NpdfCounter]      = new TH1F(("Muon1Tau2_Tau2MetDeltaPhi_"+j.str()).c_str(),("Muon1Tau2_Tau2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon1MetDeltaPhiVsMuon1Tau2CosDphi[i][NpdfCounter] = new TH2F(("Muon1MetDeltaPhiVsMuon1Tau2CosDphi_"+j.str()).c_str(),("Muon1MetDeltaPhiVsMuon1Tau2CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hMuon1Tau2NotReconstructableMass[i][NpdfCounter]       = new TH1F(("Muon1Tau2NotReconstructableMass_"+j.str()).c_str(),("Muon1Tau2NotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon1Tau2ReconstructableMass[i][NpdfCounter]          = new TH1F(("Muon1Tau2ReconstructableMass_"+j.str()).c_str(),("Muon1Tau2ReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon1Tau2PZeta[i][NpdfCounter]         = new TH1F(("Muon1Tau2PZeta_"+j.str()).c_str(),        ("Muon1Tau2PZeta_"+j.str()).c_str(), 200, -100, 100);
      _hMuon1Tau2PZetaVis[i][NpdfCounter]      = new TH1F(("Muon1Tau2PZetaVis_"+j.str()).c_str(),     ("Muon1Tau2PZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hMuon1Tau2Zeta2D[i][NpdfCounter]        = new TH2F(("Muon1Tau2Zeta2D_"+j.str()).c_str(),       ("Muon1Tau2Zeta2D_"+j.str()).c_str(), 100, 0, 100, 200,-100, 100);
      _hMuon1Tau2Zeta1D[i][NpdfCounter]        = new TH1F(("Muon1Tau2Zeta1D_"+j.str()).c_str(),       ("Muon1Tau2Zeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hMuon1Tau2DiJetReconstructableMass[i][NpdfCounter]         = new TH1F(("Muon1Tau2DiJetReconstructableMass_"+j.str()).c_str(),("Muon1Tau2DiJetReconstructableMass_"+j.str()).c_str(), 100, 0, 5000);
      _hMuon2Tau1_Tau1DiJetDeltaPhi[i][NpdfCounter]          = new TH1F(("Muon2Tau1_Tau1DiJetDeltaPhi_"+j.str()).c_str(),("Muon2Tau1_Tau1DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau1_Muon2DiJetDeltaPhi[i][NpdfCounter]         = new TH1F(("Muon2Tau1_Muon2DiJetDeltaPhi_"+j.str()).c_str(),("Muon2Tau1_Muon2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau1_Muon2IsZmm[i][NpdfCounter]           = new TH1F(("Muon2Tau1_Muon2IsZmm_"+j.str()).c_str(),("Muon2Tau1_Muon2IsZmm_"+j.str()).c_str(), 2, 0., 2.);
      _hMuon2PtVsTau1Pt[i][NpdfCounter]                = new TH2F(("Muon2PtVsTau1Pt_"+j.str()).c_str(),                    ("Muon2PtVsTau1Pt_"+j.str()).c_str(),100, 0, 500, 100, 0, 500);
      _hMuon2Tau1DeltaR[i][NpdfCounter]                = new TH1F(("Muon2Tau1DeltaR_"+j.str()).c_str(),                    ("Muon2Tau1DeltaR_"+j.str()).c_str(),100, 0, 5.);
      _hMuon2Tau1DeltaPtDivSumPt[i][NpdfCounter]       = new TH1F(("Muon2Tau1DeltaPtDivSumPt_"+j.str()).c_str(),("Muon2Tau1DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hMuon2Tau1DeltaPt[i][NpdfCounter]               = new TH1F(("Muon2Tau1DeltaPt_"+j.str()).c_str(),                   ("Muon2Tau1DeltaPt_"+j.str()).c_str(),100, 0, 1000);
      _hMuon2Tau1_Muon2MetMt[i][NpdfCounter]           = new TH1F(("Muon2Tau1_Muon2MetMt_"+j.str()).c_str(),("Muon2Tau1_Muon2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon2Tau1_Tau1MetMt[i][NpdfCounter]            = new TH1F(("Muon2Tau1_Tau1MetMt_"+j.str()).c_str(),("Muon2Tau1_Tau1MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon2Tau1OSLS[i][NpdfCounter]                  = new TH1F(("Muon2Tau1OSLS_"+j.str()).c_str(),                      ("Muon2Tau1OSLS_"+j.str()).c_str(), 20,-10, 10);
      _hMuon2Tau1CosDphi[i][NpdfCounter]               = new TH1F(("Muon2Tau1CosDphi_"+j.str()).c_str(),                   ("Muon2Tau1CosDphi_"+j.str()).c_str(),220, -1.1, 1.1);
      _hMuon2Tau1_Muon2MetDeltaPhi[i][NpdfCounter]     = new TH1F(("Muon2Tau1_Muon2MetDeltaPhi_"+j.str()).c_str(),("Muon2Tau1_Muon2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau1_Tau1MetDeltaPhi[i][NpdfCounter]      = new TH1F(("Muon2Tau1_Tau1MetDeltaPhi_"+j.str()).c_str(),("Muon2Tau1_Tau1MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2MetDeltaPhiVsMuon2Tau1CosDphi[i][NpdfCounter] = new TH2F(("Muon2MetDeltaPhiVsMuon2Tau1CosDphi_"+j.str()).c_str(),("Muon2MetDeltaPhiVsMuon2Tau1CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hMuon2Tau1NotReconstructableMass[i][NpdfCounter]       = new TH1F(("Muon2Tau1NotReconstructableMass_"+j.str()).c_str(),("Muon2Tau1NotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon2Tau1ReconstructableMass[i][NpdfCounter]          = new TH1F(("Muon2Tau1ReconstructableMass_"+j.str()).c_str(),("Muon2Tau1ReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon2Tau1PZeta[i][NpdfCounter]         = new TH1F(("Muon2Tau1PZeta_"+j.str()).c_str(),        ("Muon2Tau1PZeta_"+j.str()).c_str(), 200, -100, 100);
      _hMuon2Tau1PZetaVis[i][NpdfCounter]      = new TH1F(("Muon2Tau1PZetaVis_"+j.str()).c_str(),     ("Muon2Tau1PZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hMuon2Tau1Zeta2D[i][NpdfCounter]        = new TH2F(("Muon2Tau1Zeta2D_"+j.str()).c_str(),       ("Muon2Tau1Zeta2D_"+j.str()).c_str(), 100, 0, 100, 200,-100, 100);
      _hMuon2Tau1Zeta1D[i][NpdfCounter]        = new TH1F(("Muon2Tau1Zeta1D_"+j.str()).c_str(),       ("Muon2Tau1Zeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hMuon2Tau1DiJetReconstructableMass[i][NpdfCounter]         = new TH1F(("Muon2Tau1DiJetReconstructableMass_"+j.str()).c_str(),("Muon2Tau1DiJetReconstructableMass_"+j.str()).c_str(), 100, 0, 5000);
      _hMuon2Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]          = new TH1F(("Muon2Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(),("Muon2Tau2_Tau2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau2_Muon2DiJetDeltaPhi[i][NpdfCounter]         = new TH1F(("Muon2Tau2_Muon2DiJetDeltaPhi_"+j.str()).c_str(),("Muon2Tau2_Muon2DiJetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau2_Muon2IsZmm[i][NpdfCounter]           = new TH1F(("Muon2Tau2_Muon2IsZmm_"+j.str()).c_str(),("Muon2Tau2_Muon2IsZmm_"+j.str()).c_str(), 2, 0., 2.);
      _hMuon2PtVsTau2Pt[i][NpdfCounter]                = new TH2F(("Muon2PtVsTau2Pt_"+j.str()).c_str(),                    ("Muon2PtVsTau2Pt_"+j.str()).c_str(),100, 0, 500, 100, 0, 500);
      _hMuon2Tau2DeltaR[i][NpdfCounter]                = new TH1F(("Muon2Tau2DeltaR_"+j.str()).c_str(),                    ("Muon2Tau2DeltaR_"+j.str()).c_str(),100, 0, 5.);
      _hMuon2Tau2DeltaPtDivSumPt[i][NpdfCounter]       = new TH1F(("Muon2Tau2DeltaPtDivSumPt_"+j.str()).c_str(),("Muon2Tau2DeltaPtDivSumPt_"+j.str()).c_str(), 100, -5, 5.);
      _hMuon2Tau2DeltaPt[i][NpdfCounter]               = new TH1F(("Muon2Tau2DeltaPt_"+j.str()).c_str(),                   ("Muon2Tau2DeltaPt_"+j.str()).c_str(),100, 0, 1000);
      _hMuon2Tau2_Muon2MetMt[i][NpdfCounter]           = new TH1F(("Muon2Tau2_Muon2MetMt_"+j.str()).c_str(),("Muon2Tau2_Muon2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon2Tau2_Tau2MetMt[i][NpdfCounter]            = new TH1F(("Muon2Tau2_Tau2MetMt_"+j.str()).c_str(),("Muon2Tau2_Tau2MetMt_"+j.str()).c_str(), 100, 0, 500);
      _hMuon2Tau2OSLS[i][NpdfCounter]                  = new TH1F(("Muon2Tau2OSLS_"+j.str()).c_str(),                      ("Muon2Tau2OSLS_"+j.str()).c_str(), 20,-10, 10);
      _hMuon2Tau2CosDphi[i][NpdfCounter]               = new TH1F(("Muon2Tau2CosDphi_"+j.str()).c_str(),                   ("Muon2Tau2CosDphi_"+j.str()).c_str(),220, -1.1, 1.1);
      _hMuon2Tau2_Muon2MetDeltaPhi[i][NpdfCounter]     = new TH1F(("Muon2Tau2_Muon2MetDeltaPhi_"+j.str()).c_str(),("Muon2Tau2_Muon2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2Tau2_Tau2MetDeltaPhi[i][NpdfCounter]      = new TH1F(("Muon2Tau2_Tau2MetDeltaPhi_"+j.str()).c_str(),("Muon2Tau2_Tau2MetDeltaPhi_"+j.str()).c_str(), 72, 0, +TMath::Pi());
      _hMuon2MetDeltaPhiVsMuon2Tau2CosDphi[i][NpdfCounter] = new TH2F(("Muon2MetDeltaPhiVsMuon2Tau2CosDphi_"+j.str()).c_str(),("Muon2MetDeltaPhiVsMuon2Tau2CosDphi_"+j.str()).c_str(), 72, 0, +TMath::Pi(), 220, -1.1, 1.1);
      _hMuon2Tau2NotReconstructableMass[i][NpdfCounter]       = new TH1F(("Muon2Tau2NotReconstructableMass_"+j.str()).c_str(),("Muon2Tau2NotReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon2Tau2ReconstructableMass[i][NpdfCounter]          = new TH1F(("Muon2Tau2ReconstructableMass_"+j.str()).c_str(),("Muon2Tau2ReconstructableMass_"+j.str()).c_str(), 600, 0, 1500);
      _hMuon2Tau2PZeta[i][NpdfCounter]         = new TH1F(("Muon2Tau2PZeta_"+j.str()).c_str(),        ("Muon2Tau2PZeta_"+j.str()).c_str(), 200, -100, 100);
      _hMuon2Tau2PZetaVis[i][NpdfCounter]      = new TH1F(("Muon2Tau2PZetaVis_"+j.str()).c_str(),     ("Muon2Tau2PZetaVis_"+j.str()).c_str(), 100, 0, 100);
      _hMuon2Tau2Zeta2D[i][NpdfCounter]        = new TH2F(("Muon2Tau2Zeta2D_"+j.str()).c_str(),       ("Muon2Tau2Zeta2D_"+j.str()).c_str(), 100, 0, 100, 200,-100, 100);
      _hMuon2Tau2Zeta1D[i][NpdfCounter]        = new TH1F(("Muon2Tau2Zeta1D_"+j.str()).c_str(),       ("Muon2Tau2Zeta1D_"+j.str()).c_str(), 150, -300, 300);
      _hMuon2Tau2DiJetReconstructableMass[i][NpdfCounter]         = new TH1F(("Muon2Tau2DiJetReconstructableMass_"+j.str()).c_str(),("Muon2Tau2DiJetReconstructableMass_"+j.str()).c_str(), 100, 0, 5000);
    }

    j.str("");
  }
}

// ---------------
void BSM3GAnalyzer::writeHistograms(TFile *theOutFile, string mydirectory , unsigned int i) {

  theOutFile->cd( mydirectory.c_str() );
  for(unsigned int NpdfCounter = 0; NpdfCounter < pdfWeightVector.size();  NpdfCounter++) {
    _hEvents[i][NpdfCounter]->Write();

    if (_FillRecoVertexHists == "1") {
      _hNPVertices[i][NpdfCounter]->Write();
    }

    //--- book generator level histograms
    if (_FillGenHists == "1") {
      _hNGenTau[i][NpdfCounter]->Write();
      _hNGenHadTau[i][NpdfCounter]->Write();
      _hGenTauEnergy[i][NpdfCounter]->Write();
      _hGenTauPt[i][NpdfCounter]->Write();
      _hGenTauEta[i][NpdfCounter]->Write();
      _hGenHadTauPt[i][NpdfCounter]->Write();
      _hGenHadTauEta[i][NpdfCounter]->Write();
      _hGenTauPhi[i][NpdfCounter]->Write();
      _hNGenMuon[i][NpdfCounter]->Write();
      _hGenMuonEnergy[i][NpdfCounter]->Write();
      _hGenMuonPt[i][NpdfCounter]->Write();
      _hGenMuonEta[i][NpdfCounter]->Write();
      _hGenMuonPhi[i][NpdfCounter]->Write();
      _hGenDiTauMass[i][NpdfCounter]->Write();
      _hGenZprimeMass[i][NpdfCounter]->Write();
      _hGenZprimeStatusCode[i][NpdfCounter]->Write();
      _hGenTauStatusCode[i][NpdfCounter]->Write();
    }

    if (_FillRecoTauHists == "1") {
      _hNTau1[i][NpdfCounter]->Write();
      _hTauJet1Energy[i][NpdfCounter]->Write();
      _hTauJet1Pt[i][NpdfCounter]->Write();    
      _hTauJet1Eta[i][NpdfCounter]->Write();   
      _hFirstLeadingTauJet1Pt[i][NpdfCounter]->Write();
      _hFirstLeadingTauJet1Eta[i][NpdfCounter]->Write();
      _hTauJet1Phi[i][NpdfCounter]->Write();            
      _hTauJet1NumSignalTracks[i][NpdfCounter]->Write();
      _hTauJet1SeedTrackPt[i][NpdfCounter]->Write();    
      _hTauJet1Charge[i][NpdfCounter]->Write();         
      _hNTau2[i][NpdfCounter]->Write();
      _hTauJet2Energy[i][NpdfCounter]->Write();
      _hTauJet2Pt[i][NpdfCounter]->Write();    
      _hTauJet2Eta[i][NpdfCounter]->Write();   
      _hFirstLeadingTauJet2Pt[i][NpdfCounter]->Write();
      _hFirstLeadingTauJet2Eta[i][NpdfCounter]->Write();
      _hTauJet2Phi[i][NpdfCounter]->Write();            
      _hTauJet2NumSignalTracks[i][NpdfCounter]->Write();
      _hTauJet2SeedTrackPt[i][NpdfCounter]->Write();    
      _hTauJet2Charge[i][NpdfCounter]->Write();         
    }

    if (_FillRecoMuonHists == "1") {
      _hNMuon1[i][NpdfCounter]->Write();     
      _hMuon1Energy[i][NpdfCounter]->Write();
      _hMuon1Pt[i][NpdfCounter]->Write();    
      _hMuon1Eta[i][NpdfCounter]->Write();   
      _hFirstLeadingMuon1Pt[i][NpdfCounter]->Write();
      _hFirstLeadingMuon1Eta[i][NpdfCounter]->Write();
      _hMuon1Phi[i][NpdfCounter]->Write();            
      _hMuon1MetMt[i][NpdfCounter]->Write();          
      _hNMuon2[i][NpdfCounter]->Write();     
      _hMuon2Energy[i][NpdfCounter]->Write();
      _hMuon2Pt[i][NpdfCounter]->Write();    
      _hMuon2Eta[i][NpdfCounter]->Write();   
      _hFirstLeadingMuon2Pt[i][NpdfCounter]->Write();
      _hFirstLeadingMuon2Eta[i][NpdfCounter]->Write();
      _hMuon2Phi[i][NpdfCounter]->Write();            
      _hMuon2MetMt[i][NpdfCounter]->Write();          
    }

    if (_FillRecoJetHists == "1") {
      _hNJet1[i][NpdfCounter]->Write();     
      _hNJet2[i][NpdfCounter]->Write();     
      _hNCentralJet[i][NpdfCounter]->Write(); 
      _hNBJet[i][NpdfCounter]->Write();       
      _hJet1Energy[i][NpdfCounter]->Write();  
      _hJet1Pt[i][NpdfCounter]->Write();      
      _hJet2Energy[i][NpdfCounter]->Write();  
      _hJet2Pt[i][NpdfCounter]->Write();      
      _hCentralJetPt[i][NpdfCounter]->Write();
      _hCentralJetEta[i][NpdfCounter]->Write();
      _hJet1Eta[i][NpdfCounter]->Write();      
      _hJet1Phi[i][NpdfCounter]->Write();      
      _hJet2Eta[i][NpdfCounter]->Write();      
      _hJet2Phi[i][NpdfCounter]->Write();      
      _hBJetEnergy[i][NpdfCounter]->Write();   
      _hBJetPt[i][NpdfCounter]->Write();       
      _hBJetEta[i][NpdfCounter]->Write();      
      _hBJetPhi[i][NpdfCounter]->Write();      
      _hFirstLeadingJetPt[i][NpdfCounter]->Write();
      _hSecondLeadingJetPt[i][NpdfCounter]->Write();
      _hFirstLeadingJetEta[i][NpdfCounter]->Write();
      _hSecondLeadingJetEta[i][NpdfCounter]->Write(); 
      _hMHT[i][NpdfCounter]->Write();                 
      _hHT[i][NpdfCounter]->Write();                  
      _hMeff[i][NpdfCounter]->Write();                
      _hLeadDiJetMass[i][NpdfCounter]->Write();       
      _hLeadDiJetPt[i][NpdfCounter]->Write();         
      _hLeadDiJetEtaProduct[i][NpdfCounter]->Write(); 
      _hDiJetMass[i][NpdfCounter]->Write();           
      _hDiJetPt[i][NpdfCounter]->Write();             
      _hLeadingJetsMass[i][NpdfCounter]->Write();     
      _hLeadSublDijetDphi[i][NpdfCounter]->Write();   
      _hMetVsDiJetDeltaPhiLeadSubl[i][NpdfCounter]->Write();
      _hDeltaEtaVsDeltaPhiLeadSubl[i][NpdfCounter]->Write();
      _hLeadingJetsPt[i][NpdfCounter]->Write();             
      _hDiJetDeltaR[i][NpdfCounter]->Write();               
      _hDiJetDeltaEta[i][NpdfCounter]->Write();             
      _hDiJetDeltaPhi[i][NpdfCounter]->Write();             
      _hLeadDiJetDeltaR[i][NpdfCounter]->Write();           
      _hLeadDiJetDeltaEta[i][NpdfCounter]->Write();         
      _hLeadDiJetDeltaPhi[i][NpdfCounter]->Write();         
      _hLeadingJetsDeltaR[i][NpdfCounter]->Write();         
      _hLeadingJetsDeltaEta[i][NpdfCounter]->Write();       
      _hR1[i][NpdfCounter]->Write(); 		     
      _hR2[i][NpdfCounter]->Write();                   
      _hDphi1MHT[i][NpdfCounter]->Write();             
      _hDphi2MHT[i][NpdfCounter]->Write();             
      _hDphi1[i][NpdfCounter]->Write();                
      _hDphi2[i][NpdfCounter]->Write();                
      _hDphi1VsDphi2[i][NpdfCounter]->Write();         
      _hAlpha[i][NpdfCounter]->Write();                
    }

    if (_FillTopologyHists == "1") {
      _hMetDiJetDeltaPhi[i][NpdfCounter]->Write();        
      _hMuon1Muon2_Muon1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Muon2_Muon2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1PtVsMuon2Pt[i][NpdfCounter]->Write();        
      _hMuon1Muon2_Muon1IsZmm[i][NpdfCounter]->Write();   
      _hMuon1Muon2_Muon2IsZmm[i][NpdfCounter]->Write();   
      _hMuon1Muon2DeltaR[i][NpdfCounter]->Write();        
      _hMuon1Muon2DeltaPtDivSumPt[i][NpdfCounter]->Write();
      _hMuon1Muon2DeltaPt[i][NpdfCounter]->Write();        
      _hDiMuon_Muon1MetMt[i][NpdfCounter]->Write();        
      _hDiMuon_Muon2MetMt[i][NpdfCounter]->Write();        
      _hMuon1Muon2OSLS[i][NpdfCounter]->Write();           
      _hMuon1Muon2CosDphi[i][NpdfCounter]->Write();        
      _hDiMuon_Muon1MetDeltaPhi[i][NpdfCounter]->Write();  
      _hDiMuon_Muon2MetDeltaPhi[i][NpdfCounter]->Write();  
      _hMuon1MetDeltaPhiVsMuon1Muon2CosDphi[i][NpdfCounter]->Write();
      _hDiMuonPZeta[i][NpdfCounter]->Write();        
      _hDiMuonPZetaVis[i][NpdfCounter]->Write();     
      _hDiMuonZeta2D[i][NpdfCounter]->Write();       
      _hDiMuonZeta1D[i][NpdfCounter]->Write();       
      _hDiMuonNotReconstructableMass[i][NpdfCounter]->Write();
      _hDiMuonReconstructableMass[i][NpdfCounter]->Write();
      _hMet[i][NpdfCounter]->Write();                
      _hTau1Tau2_Tau1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hTau1Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hTau1PtVsTau2Pt[i][NpdfCounter]->Write();            
      _hTau1Tau2DeltaR[i][NpdfCounter]->Write();            
      _hTau1Tau2DeltaPtDivSumPt[i][NpdfCounter]->Write();   
      _hTau1Tau2DeltaPt[i][NpdfCounter]->Write();           
      _hDiTau_Tau1MetMt[i][NpdfCounter]->Write();           
      _hDiTau_Tau2MetMt[i][NpdfCounter]->Write();           
      _hTau1Tau2OSLS[i][NpdfCounter]->Write();              
      _hTau1Tau2CosDphi[i][NpdfCounter]->Write();           
      _hDiTau_Tau1MetDeltaPhi[i][NpdfCounter]->Write();     
      _hDiTau_Tau2MetDeltaPhi[i][NpdfCounter]->Write();     
      _hTau1MetDeltaPhiVsTau1Tau2CosDphi[i][NpdfCounter]->Write();
      _hDiTauPZeta[i][NpdfCounter]->Write();          
      _hDiTauPZetaVis[i][NpdfCounter]->Write();       
      _hDiTauZeta2D[i][NpdfCounter]->Write();         
      _hDiTauZeta1D[i][NpdfCounter]->Write();         
      _hDiTauNotReconstructableMass[i][NpdfCounter]->Write();
      _hDiTauReconstructableMass[i][NpdfCounter]->Write();   
      _hDiTauDiJetReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau1_Tau1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Tau1_Muon1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Tau1_Muon1IsZmm[i][NpdfCounter]->Write();
      _hMuon1PtVsTau1Pt[i][NpdfCounter]->Write();
      _hMuon1Tau1DeltaR[i][NpdfCounter]->Write();   
      _hMuon1Tau1DeltaPtDivSumPt[i][NpdfCounter]->Write();    
      _hMuon1Tau1DeltaPt[i][NpdfCounter]->Write();
      _hMuon1Tau1_Muon1MetMt[i][NpdfCounter]->Write();
      _hMuon1Tau1_Tau1MetMt[i][NpdfCounter]->Write();
      _hMuon1Tau1OSLS[i][NpdfCounter]->Write();
      _hMuon1Tau1CosDphi[i][NpdfCounter]->Write();
      _hMuon1Tau1_Muon1MetDeltaPhi[i][NpdfCounter]->Write();  
      _hMuon1Tau1_Tau1MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1MetDeltaPhiVsMuon1Tau1CosDphi[i][NpdfCounter]->Write();
      _hMuon1Tau1NotReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau1ReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau1PZeta[i][NpdfCounter]->Write();
      _hMuon1Tau1PZetaVis[i][NpdfCounter]->Write();
      _hMuon1Tau1Zeta2D[i][NpdfCounter]->Write();
      _hMuon1Tau1Zeta1D[i][NpdfCounter]->Write();
      _hMuon1Tau1DiJetReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Tau2_Muon1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Tau2_Muon1IsZmm[i][NpdfCounter]->Write();
      _hMuon1PtVsTau2Pt[i][NpdfCounter]->Write();
      _hMuon1Tau2DeltaR[i][NpdfCounter]->Write();
      _hMuon1Tau2DeltaPtDivSumPt[i][NpdfCounter]->Write();
      _hMuon1Tau2DeltaPt[i][NpdfCounter]->Write();
      _hMuon1Tau2_Muon1MetMt[i][NpdfCounter]->Write();
      _hMuon1Tau2_Tau2MetMt[i][NpdfCounter]->Write();
      _hMuon1Tau2OSLS[i][NpdfCounter]->Write();
      _hMuon1Tau2CosDphi[i][NpdfCounter]->Write();
      _hMuon1Tau2_Muon1MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1Tau2_Tau2MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon1MetDeltaPhiVsMuon1Tau2CosDphi[i][NpdfCounter]->Write();
      _hMuon1Tau2NotReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau2ReconstructableMass[i][NpdfCounter]->Write();
      _hMuon1Tau2PZeta[i][NpdfCounter]->Write();
      _hMuon1Tau2PZetaVis[i][NpdfCounter]->Write();
      _hMuon1Tau2Zeta2D[i][NpdfCounter]->Write();
      _hMuon1Tau2Zeta1D[i][NpdfCounter]->Write();
      _hMuon1Tau2DiJetReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau1_Tau1DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau1_Muon2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau1_Muon2IsZmm[i][NpdfCounter]->Write();
      _hMuon2PtVsTau1Pt[i][NpdfCounter]->Write();
      _hMuon2Tau1DeltaR[i][NpdfCounter]->Write();
      _hMuon2Tau1DeltaPtDivSumPt[i][NpdfCounter]->Write();
      _hMuon2Tau1DeltaPt[i][NpdfCounter]->Write();
      _hMuon2Tau1_Muon2MetMt[i][NpdfCounter]->Write();
      _hMuon2Tau1_Tau1MetMt[i][NpdfCounter]->Write();
      _hMuon2Tau1OSLS[i][NpdfCounter]->Write();
      _hMuon2Tau1CosDphi[i][NpdfCounter]->Write();
      _hMuon2Tau1_Muon2MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau1_Tau1MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2MetDeltaPhiVsMuon2Tau1CosDphi[i][NpdfCounter]->Write();
      _hMuon2Tau1NotReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau1ReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau1PZeta[i][NpdfCounter]->Write();
      _hMuon2Tau1PZetaVis[i][NpdfCounter]->Write();
      _hMuon2Tau1Zeta2D[i][NpdfCounter]->Write();
      _hMuon2Tau1Zeta1D[i][NpdfCounter]->Write();
      _hMuon2Tau1DiJetReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau2_Tau2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau2_Muon2DiJetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau2_Muon2IsZmm[i][NpdfCounter]->Write();
      _hMuon2PtVsTau2Pt[i][NpdfCounter]->Write();
      _hMuon2Tau2DeltaR[i][NpdfCounter]->Write();
      _hMuon2Tau2DeltaPtDivSumPt[i][NpdfCounter]->Write();
      _hMuon2Tau2DeltaPt[i][NpdfCounter]->Write();
      _hMuon2Tau2_Muon2MetMt[i][NpdfCounter]->Write();
      _hMuon2Tau2_Tau2MetMt[i][NpdfCounter]->Write();
      _hMuon2Tau2OSLS[i][NpdfCounter]->Write();
      _hMuon2Tau2CosDphi[i][NpdfCounter]->Write();
      _hMuon2Tau2_Muon2MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2Tau2_Tau2MetDeltaPhi[i][NpdfCounter]->Write();
      _hMuon2MetDeltaPhiVsMuon2Tau2CosDphi[i][NpdfCounter]->Write();
      _hMuon2Tau2NotReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau2ReconstructableMass[i][NpdfCounter]->Write();
      _hMuon2Tau2PZeta[i][NpdfCounter]->Write();
      _hMuon2Tau2PZetaVis[i][NpdfCounter]->Write();
      _hMuon2Tau2Zeta2D[i][NpdfCounter]->Write();
      _hMuon2Tau2Zeta1D[i][NpdfCounter]->Write();
      _hMuon2Tau2DiJetReconstructableMass[i][NpdfCounter]->Write();
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

void BSM3GAnalyzer::initializePileupInfo() {
  char * cstr1;
  char * cstr2;

  // Filenames must be c_strings below. Here is the conversion from strings to c_strings
  // As you can see above cstr1 corresponds to MC and cstr2 corresponds to data.
  cstr1 = new char [_MCHistos.size()+1];
  strcpy (cstr1, _MCHistos.c_str());
  cstr2 = new char [_DataHistos.size()+1];
  strcpy (cstr2, _DataHistos.c_str());

  TFile *file1 = TFile::Open(cstr1);
  TH1* histmc = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!histmc) {throw std::runtime_error("failed to extract histogram");}
  for(int bin=0; bin<=(histmc->GetXaxis()->GetNbins() + 1); bin++) {
    hPUmc->SetBinContent(bin,histmc->GetBinContent(bin));
  }
  file1->Close();

  TFile *file2 = TFile::Open(cstr2);
  TH1* histdata = dynamic_cast<TH1*>(file2->Get("analyzeHiMassTau/NVertices_0"));
  if(!histdata) {throw std::runtime_error("failed to extract histogram");}
  for(int bin=0; bin<=(histdata->GetXaxis()->GetNbins() + 1); bin++) {
    hPUdata->SetBinContent(bin,histdata->GetBinContent(bin));
  }
  file2->Close();
}

double BSM3GAnalyzer::getPileupWeight(float ntruePUInt) {
  int bin;
  double MCintegral;
  double MCvalue;
  double Dataintegral;
  double Datavalue;

  // The probability that data (or MC) has N pileup interactions is value / integral
  // The ratio of the data and MC probability density functions gives us our pileup weights

  //std::cout << "Grabbing pileup info. " << std::endl;
  bin = hPUmc->GetBin(ntruePUInt+1);
  MCvalue = hPUmc->GetBinContent(bin);
  MCintegral = hPUmc->Integral();
  Datavalue = hPUdata->GetBinContent(bin);
  Dataintegral = hPUdata->Integral();

  // printouts for debugging
  //std::cout << "Number of true pileup interactions = " << ntruePUInt << std::endl;
  //std::cout << "Histogram bin, given the number of true pileup interactions = " << bin << std::endl;
  //std::cout << "MC PU probability density, given the number of true pileup interactions = " << MCvalue << std::endl;
  //std::cout << "Data PU probability density, given the number of true pileup interactions = " << Datavalue << std::endl;

  //std::cout << "Grabbing pileup weight. " << std::endl;
  //Ratio of normalized histograms in given bin
  if((MCvalue * Dataintegral) != 0) {pu_weight = (Datavalue * MCintegral) / (MCvalue * Dataintegral);}
  else {pu_weight = 1.0;}

  // printouts for debugging
  //std::cout << "PU weight, given the number of true pileup interactions = " << pu_weight << std::endl;

  return pu_weight;
}

// Gen level tau requirements
int BSM3GAnalyzer::ExtractNumberOfGoodGenTaus() {
  int nGenTaus = 0;
  if(isData == "0") {
    for(int j = 0; j < Gen_pt->size(); j++) {
      if((abs(Gen_pdg_id->at(j)) == 15) && (Gen_status->at(j) == 2)) {
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

// -------------Apply Trigger Requirements
bool BSM3GAnalyzer::passRecoTrigger1Cuts() {
  for(int nTrig = 0; nTrig < Trigger_names->size(); ++nTrig) {
    //std::cout << "Trigger Name = " << Trigger_names->at(nTrig) << ", Decision = " << Trigger_decision->at(nTrig) << std::endl;
    string trigName = Trigger_names->at(nTrig);
    if( ( (trigName.find(_Trigger1FirstRequirement) != string::npos) && (Trigger_decision->at(nTrig) == 1) ) || 
        ( (trigName.find(_Trigger1SecondRequirement) != string::npos) && (Trigger_decision->at(nTrig) == 1) ) ) {return true;}
  }
  return false;
}

// -------------Apply Trigger Requirements
bool BSM3GAnalyzer::passRecoTrigger2Cuts() {
  for(int nTrig = 0; nTrig < Trigger_names->size(); ++nTrig) {
    string trigName = Trigger_names->at(nTrig);
    if( ( (trigName.find(_Trigger2FirstRequirement) != string::npos) && (Trigger_decision->at(nTrig) == 1) ) || 
        ( (trigName.find(_Trigger2SecondRequirement) != string::npos) && (Trigger_decision->at(nTrig) == 1) ) ) {return true;}
  }
  return false;
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
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byLooseIsolationMva3oldDMwLT") {
      if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byMediumIsolationMva3oldDMwLT") {
      if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrByMaxIsolation == "byTightIsolationMva3oldDMwLT") {
      if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
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
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byLooseIsolationMva3oldDMwLT") {
        if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byMediumIsolationMva3oldDMwLT") {
        if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau1DiscrByMinIsolation == "byTightIsolationMva3oldDMwLT") {
        if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
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
    if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau1DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau1sThatAreMuons == "1") {
    if(_RecoTau1DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) > 0.5) {return false;}
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
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
      if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byLooseIsolationMva3oldDMwLT") {
      if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byMediumIsolationMva3oldDMwLT") {
      if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrByMaxIsolation == "byTightIsolationMva3oldDMwLT") {
      if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) < 0.5) {return false;}
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
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightCombinedIsolationDeltaBetaCorr3Hits") {
        if (Tau_byTightCombinedIsolationDeltaBetaCorr3Hits->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byLooseIsolationMva3oldDMwLT") {
        if (Tau_byLooseIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byMediumIsolationMva3oldDMwLT") {
        if (Tau_byMediumIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
      } else if(_RecoTau2DiscrByMinIsolation == "byTightIsolationMva3oldDMwLT") {
        if (Tau_byTightIsolationMva3oldDMwLT->at(nobj) > 0.5) {return false;}
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
    if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) < 0.5) {return false;}
    } else if(_RecoTau2DiscrAgainstMuon == "againstMuonTight3") {
      if (Tau_againstMuonTight3->at(nobj) < 0.5) {return false;}
    } else { }
  }
  if (_SelectTau2sThatAreMuons == "1") {
    if(_RecoTau2DiscrAgainstMuon == "againstMuonLoose3") {
      if (Tau_againstMuonLoose3->at(nobj) > 0.5) {return false;}
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

bool BSM3GAnalyzer::passRecoCentralJetCuts(int nobj) {
  // ----Acceptance cuts
  if (fabs(smearedJetMomentumVector.at(nobj).Eta()) > 2.5) {return false;}
  if (smearedJetMomentumVector.at(nobj).Pt()<_RecoCentralJetPtCut) {return false;}
  if (_ApplyCentralJetLooseID == "1") {
    if (!passedLooseJetID(nobj)) {return false;}
  }
  // ----anti-overlap requirements
  if (_RemoveCentralJetOverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _CentralJetMuon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveCentralJetOverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _CentralJetMuon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveCentralJetOverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _CentralJetElectron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveCentralJetOverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _CentralJetElectron2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveCentralJetOverlapWithTau1s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _CentralJetTau1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveCentralJetOverlapWithTau2s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _CentralJetTau2MatchingDeltaR) {return false;}
      }
    }
  }
  return true;
}

//--------------Apply Leading Jet Cuts
bool BSM3GAnalyzer::passRecoFirstLeadingJetCuts(int nobj) {
  if(_DoDiscrByFirstLeadingJet == "1") {
    // ----Acceptance cuts
    if (fabs(smearedJetMomentumVector.at(nobj).Eta())>_RecoFirstLeadingJetEtaMaxCut) {return false;}
    if (fabs(smearedJetMomentumVector.at(nobj).Eta())<_RecoFirstLeadingJetEtaMinCut) {return false;}
    if (smearedJetMomentumVector.at(nobj).Pt()<_RecoFirstLeadingJetPt) {return false;}
    if (_ApplyLeadingJetsLooseID == "1") {
      if (!passedLooseJetID(nobj)) {return false;}
    }
    if (_RemoveFirstLeadingJetOverlapWithMuon1s == "1") {
      for(int jj = 0; jj < Muon_pt->size(); jj++) {
        if (passRecoMuon1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _FirstLeadingJetMuon1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveFirstLeadingJetOverlapWithMuon2s == "1") {
      for(int jj = 0; jj < Muon_pt->size(); jj++) {
        if (passRecoMuon2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _FirstLeadingJetMuon2MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveFirstLeadingJetOverlapWithElectron1s == "1") {
      for(int jj = 0; jj < patElectron_pt->size(); jj++) {
        if (passRecoElectron1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _FirstLeadingJetElectron1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveFirstLeadingJetOverlapWithElectron2s == "1") {
      for(int jj = 0; jj < patElectron_pt->size(); jj++) {
        if (passRecoElectron2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _FirstLeadingJetElectron2MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveFirstLeadingJetOverlapWithTau1s == "1") {
      for(int jj = 0; jj < Tau_pt->size(); jj++) {
        if (passRecoTau1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _FirstLeadingJetTau1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveFirstLeadingJetOverlapWithTau2s == "1") {
      for(int jj = 0; jj < Tau_pt->size(); jj++) {
        if (passRecoTau2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _FirstLeadingJetTau2MatchingDeltaR) {return false;}
        }
      }
    }
  }
  return true;
}

//--------------Apply Second Leading Jet Cuts
bool BSM3GAnalyzer::passRecoSecondLeadingJetCuts(int nobj) {
  if(_DoDiscrBySecondLeadingJet == "1") {
    // ----Acceptance cuts
    if (fabs(smearedJetMomentumVector.at(nobj).Eta())>_RecoSecondLeadingJetEtaMaxCut) {return false;}
    if (fabs(smearedJetMomentumVector.at(nobj).Eta())<_RecoSecondLeadingJetEtaMinCut) {return false;}
    if (smearedJetMomentumVector.at(nobj).Pt()<_RecoSecondLeadingJetPt) {return false;}
    if (_ApplyLeadingJetsLooseID == "1") {
      if (!passedLooseJetID(nobj)) {return false;}
    }
    if (_RemoveSecondLeadingJetOverlapWithMuon1s == "1") {
      for(int jj = 0; jj < Muon_pt->size(); jj++) {
        if (passRecoMuon1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _SecondLeadingJetMuon1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveSecondLeadingJetOverlapWithMuon2s == "1") {
      for(int jj = 0; jj < Muon_pt->size(); jj++) {
        if (passRecoMuon2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _SecondLeadingJetMuon2MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveSecondLeadingJetOverlapWithElectron1s == "1") {
      for(int jj = 0; jj < patElectron_pt->size(); jj++) {
        if (passRecoElectron1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _SecondLeadingJetElectron1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveSecondLeadingJetOverlapWithElectron2s == "1") {
      for(int jj = 0; jj < patElectron_pt->size(); jj++) {
        if (passRecoElectron2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _SecondLeadingJetElectron2MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveSecondLeadingJetOverlapWithTau1s == "1") {
      for(int jj = 0; jj < Tau_pt->size(); jj++) {
        if (passRecoTau1Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _SecondLeadingJetTau1MatchingDeltaR) {return false;}
        }
      }
    }
    if (_RemoveSecondLeadingJetOverlapWithTau2s == "1") {
      for(int jj = 0; jj < Tau_pt->size(); jj++) {
        if (passRecoTau2Cuts(jj)) {
          if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _SecondLeadingJetTau2MatchingDeltaR) {return false;}
        }
      }
    }
  }
  return true;
}

//--------------Apply b-Jet Cuts
bool BSM3GAnalyzer::passRecoBJetCuts(int nobj) {
  // ----Matching
  if((_MatchBToGen == "1") && (isData == "0")) {
    if(abs(Jet_partonFlavour->at(nobj)) != 5) {return false;}
  }

  // ----Acceptance cuts
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())>_RecoBJetEtaMaxCut) {return false;}
  if (fabs(smearedJetMomentumVector.at(nobj).Eta())<_RecoBJetEtaMinCut) {return false;}
  if (smearedJetMomentumVector.at(nobj).Pt()<_RecoBJetPtCut) {return false;}

  // ----anti-overlap requirements
  if (_RemoveBJetOverlapWithMuon1s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _BJetMuon1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveBJetOverlapWithMuon2s == "1") {
    for(int jj = 0; jj < Muon_pt->size(); jj++) {
      if (passRecoMuon2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedMuonMomentumVector.at(jj)) < _BJetMuon2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveBJetOverlapWithElectron1s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _BJetElectron1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveBJetOverlapWithElectron2s == "1") {
    for(int jj = 0; jj < patElectron_pt->size(); jj++) {
      if (passRecoElectron2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedElectronMomentumVector.at(jj)) < _BJetElectron2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveBJetOverlapWithTau1s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau1Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _BJetTau1MatchingDeltaR) {return false;}
      }
    }
  }
  if (_RemoveBJetOverlapWithTau2s == "1") {
    for(int jj = 0; jj < Tau_pt->size(); jj++) {
      if (passRecoTau2Cuts(jj)) {
        if(smearedJetMomentumVector.at(nobj).DeltaR(smearedTauMomentumVector.at(jj)) < _BJetTau2MatchingDeltaR) {return false;}
      }
    }
  }
  if (_ApplyJetBTagging == "1") {if(Jet_bDiscriminator->at(nobj) <= _JetBTaggingCut) {return false;}}
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

bool BSM3GAnalyzer::passSusyTopologyCuts(int nobj1, int nobj2) {
  if(_DoSUSYDiscrByLeadDiJetMass == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      for(int jj = 0; jj < Jet_pt->size(); jj++) {
        for(int jjj = 0; jjj < Jet_pt->size(); jjj++) {
          if ((jj == nobj1) && (jjj == nobj2)) {
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(jj),smearedJetMomentumVector.at(jjj)).second.M() < _LeadDiJetMinMassCut) {return false;}
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(jj),smearedJetMomentumVector.at(jjj)).second.M() > _LeadDiJetMaxMassCut) {return false;}
          }
        }
      }
    }
  }
  if(_DoSUSYDiscrByLeadDiJetPt == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      for(int jj = 0; jj < Jet_pt->size(); jj++) {
        for(int jjj = 0; jjj < Jet_pt->size(); jjj++) {
          if ((jj == nobj1) && (jjj == nobj2)) {
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(jj),smearedJetMomentumVector.at(jjj)).second.Pt() < _LeadDiJetMinPtCut) {return false;}
            if(CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(jj),smearedJetMomentumVector.at(jjj)).second.Pt() > _LeadDiJetMaxPtCut) {return false;}
          }
        }
      }
    }
  }
  if(_DoSUSYDiscrByLeadDiJetDeltaEta == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      for(int jj = 0; jj < Jet_pt->size(); jj++) {
        for(int jjj = 0; jjj < Jet_pt->size(); jjj++) {
          if ((jj == nobj1) && (jjj == nobj2)) {
            if(fabs(smearedJetMomentumVector.at(nobj1).Eta() - smearedJetMomentumVector.at(nobj2).Eta()) < _LeadDiJetMinDeltaEtaCut) {return false;}
            if(fabs(smearedJetMomentumVector.at(nobj1).Eta() - smearedJetMomentumVector.at(nobj2).Eta()) > _LeadDiJetMaxDeltaEtaCut) {return false;}
          }
        }
      }
    }
  }
  if(_DoSUSYDiscrByLeadDiJetDeltaPhi == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      for(int jj = 0; jj < Jet_pt->size(); jj++) {
        for(int jjj = 0; jjj < Jet_pt->size(); jjj++) {
          if ((jj == nobj1) && (jjj == nobj2)) {
            if(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi())) < _LeadDiJetMinDeltaPhiCut) {return false;}
            if(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi())) > _LeadDiJetMaxDeltaPhiCut) {return false;}
          }
        }
      }
    }
  }
  if(_DoSUSYDiscrByLeadDiJetOSEta == "1") {
    if((smearedJetMomentumVector.at(nobj1).Eta() * smearedJetMomentumVector.at(nobj2).Eta()) >= 0) {return false;}
  }
  if(_DoDiscrByMet == "1") {
    if(theMETVector.Pt() < _RecoMetMinCut) {return false;}
    if(theMETVector.Pt() > _RecoMetMaxCut) {return false;}
  }
  if(_DoSUSYDiscrByMHT == "1") { if(sqrt((sumpxForMht * sumpxForMht) + (sumpyForMht * sumpyForMht)) < _MhtCut) {return false;} }
  if(_DoSUSYDiscrByHT == "1") { if(sumptForHt < _HtCut) {return false;} }
  double dphi1;
  double dphi2;
  double r1;
  double r2;
  double alpha;
  if(_DoSUSYDiscrByR1 == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      dphi1 = normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - theMETVector.Phi());
      dphi2 = normalizedPhi(smearedJetMomentumVector.at(nobj2).Phi() - theMETVector.Phi());
      r1 = sqrt( pow(dphi1,2.0) + pow((TMath::Pi() - dphi2),2.0) );
      if(r1 < _R1MinCut) {return false;}
      if(r1 > _R1MaxCut) {return false;}
    }
  }
  if(_DoSUSYDiscrByR2 == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      dphi1 = normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - theMETVector.Phi());
      dphi2 = normalizedPhi(smearedJetMomentumVector.at(nobj2).Phi() - theMETVector.Phi());
      r2 = sqrt( pow(dphi2,2.0) + pow((TMath::Pi() - dphi1),2.0) );
      if(r2 < _R2MinCut) {return false;}
      if(r2 > _R2MaxCut) {return false;}
    }
  }
  if(_DoSUSYDiscrByAlpha == "1") {
    if((nobj1 < 0) || (nobj2 < 0)) {return false;}
    else {
      double px = smearedJetMomentumVector.at(nobj1).Px() + smearedJetMomentumVector.at(nobj2).Px();
      double py = smearedJetMomentumVector.at(nobj1).Py() + smearedJetMomentumVector.at(nobj2).Py();
      double pz = smearedJetMomentumVector.at(nobj1).Pz() + smearedJetMomentumVector.at(nobj2).Pz();
      double e = smearedJetMomentumVector.at(nobj1).Energy() + smearedJetMomentumVector.at(nobj2).Energy();
      TLorentzVector Susy_LorentzVect;
      Susy_LorentzVect.SetPxPyPzE(px, py, pz, e);
      if(Susy_LorentzVect.M() > 0) {alpha = smearedJetMomentumVector.at(nobj2).Pt() / Susy_LorentzVect.M();}
      else {alpha = 999999999.0;}
      if(alpha < _AlphaMinCut) {return false;}
      if(alpha > _AlphaMaxCut) {return false;}
    }
  }
  if(_DoSUSYDiscrByDphi1 == "1") {
    if(nobj1 < 0) {return false;}
    else {
      dphi1 = normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - theMETVector.Phi());
      if(abs(dphi1) < _Dphi1MinCut) {return false;}
      if(abs(dphi1) > _Dphi1MaxCut) {return false;}
    }
  }
  if(_DoSUSYDiscrByDphi2 == "1") {
    if(nobj2 < 0) {return false;}
    else {
      dphi2 = normalizedPhi(smearedJetMomentumVector.at(nobj2).Phi() - theMETVector.Phi());
      if(abs(dphi2) < _Dphi2MinCut) {return false;}
      if(abs(dphi2) > _Dphi2MaxCut) {return false;}
    }
  }
  return true;
}

pair<bool, TLorentzVector> BSM3GAnalyzer::CalculateTheDiJet4Momentum(TLorentzVector Tobj1, TLorentzVector Tobj2) {
  TLorentzVector The_LorentzVect = Tobj1 + Tobj2;
  pair<bool, TLorentzVector> MassRecoInformation(true,The_LorentzVect);
  return MassRecoInformation;
}

pair<bool, TLorentzVector> BSM3GAnalyzer::CalculateTheDiTau4Momentum(TLorentzVector Tobj1, TLorentzVector Tobj2) {
  if(_UseVectorSumOfVisProductsAndMetMassReco == "1") {
    double px = Tobj1.Px() + Tobj2.Px() + theMETVector.Px();
    double py = Tobj1.Py() + Tobj2.Py() + theMETVector.Py();
    double pz = Tobj1.Pz() + Tobj2.Pz();
    double e  = Tobj1.Energy() + Tobj2.Energy() + TMath::Sqrt((theMETVector.Px() * theMETVector.Px()) + (theMETVector.Py() * theMETVector.Py()));
    TLorentzVector The_LorentzVect;
    The_LorentzVect.SetPxPyPzE(px, py, pz, e);
    pair<bool, TLorentzVector> MassRecoInformation(true,The_LorentzVect);
    return MassRecoInformation;
  } else if(_UseCollinerApproxMassReco == "1") {
    double x1_numerator = (Tobj1.Px() * Tobj2.Py()) - (Tobj2.Px() * Tobj1.Py());
    double x1_denominator = (Tobj2.Py() * (Tobj1.Px() + theMETVector.Px())) - (Tobj2.Px() * (Tobj1.Py() + theMETVector.Py()));
    double x1 = ( x1_denominator != 0. ) ? x1_numerator/x1_denominator : -1.;
    double x2_numerator = x1_numerator;
    double x2_denominator = (Tobj1.Px() * (Tobj2.Py() + theMETVector.Py())) - (Tobj1.Py() * (Tobj2.Px() + theMETVector.Px()));
    double x2 = ( x2_denominator != 0. ) ? x2_numerator/x2_denominator : -1.;
    if ( (x1 > 0. && x1 < 1.) && (x2 > 0. && x2 < 1.) ) {
      TLorentzVector The_LorentzVect;
      The_LorentzVect.SetPxPyPzE( (Tobj1.Px() / x1) + (Tobj2.Px() / x2), (Tobj1.Py() / x1) + (Tobj2.Py() / x2), (Tobj1.Pz() / x1) + (Tobj2.Pz() / x2), (Tobj1.Energy() / x1) + (Tobj2.Energy() / x2) );
      pair<bool, TLorentzVector> MassRecoInformation(true,The_LorentzVect);
      return MassRecoInformation;
    } else {
      double px = Tobj1.Px() + Tobj2.Px() + theMETVector.Px();
      double py = Tobj1.Py() + Tobj2.Py() + theMETVector.Py();
      double pz = Tobj1.Pz() + Tobj2.Pz();
      double e  = Tobj1.Energy() + Tobj2.Energy() + TMath::Sqrt((theMETVector.Px() * theMETVector.Px()) + (theMETVector.Py() * theMETVector.Py()));
      TLorentzVector The_LorentzVect;
      The_LorentzVect.SetPxPyPzE(px, py, pz, e);
      pair<bool, TLorentzVector> MassRecoInformation(false,The_LorentzVect);
      return MassRecoInformation;
    }
  } else {
    TLorentzVector The_LorentzVect = Tobj1 + Tobj2;
    pair<bool, TLorentzVector> MassRecoInformation(true,The_LorentzVect);
    return MassRecoInformation;
  }
}

//-----Calculate lepton+met transverse mass
double BSM3GAnalyzer::CalculateLeptonMetMt(TLorentzVector Tobj) {
  double px = Tobj.Px() + theMETVector.Px();
  double py = Tobj.Py() + theMETVector.Py();
  double et = Tobj.Et() + TMath::Sqrt((theMETVector.Px() * theMETVector.Px()) + (theMETVector.Py() * theMETVector.Py()));
  double mt2 = et*et - (px*px + py*py);
  if ( mt2 < 0 ) { return -1.; }
  else { return sqrt(mt2); }
}

double BSM3GAnalyzer::CalculatePZeta(TLorentzVector Tobj1, TLorentzVector Tobj2) {
  double zetaX = cos(Tobj1.Phi()) + cos(Tobj2.Phi());
  double zetaY = sin(Tobj1.Phi()) + sin(Tobj2.Phi());
  double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
  if ( zetaR > 0. ) { zetaX /= zetaR; zetaY /= zetaR; }
  double visPx = Tobj1.Px() + Tobj2.Px();
  double visPy = Tobj1.Py() + Tobj2.Py();
  double px = visPx + theMETVector.Px();
  double py = visPy + theMETVector.Py();
  double pZeta = px*zetaX + py*zetaY;
  return pZeta;
}

double BSM3GAnalyzer::CalculatePZetaVis(TLorentzVector Tobj1, TLorentzVector Tobj2) {
  double zetaX = cos(Tobj1.Phi()) + cos(Tobj2.Phi());
  double zetaY = sin(Tobj1.Phi()) + sin(Tobj2.Phi());
  double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
  if ( zetaR > 0. ) { zetaX /= zetaR; zetaY /= zetaR; }
  double visPx = Tobj1.Px() + Tobj2.Px();
  double visPy = Tobj1.Py() + Tobj2.Py();
  double pZetaVis = visPx*zetaX + visPy*zetaY;
  return pZetaVis;
}

double BSM3GAnalyzer::normalizedPhi(double phi) {
  static double const TWO_PI = TMath::Pi() * 2;
  while ( phi < -TMath::Pi() ) phi += TWO_PI;
  while ( phi >  TMath::Pi() ) phi -= TWO_PI;
  return phi;
}

bool BSM3GAnalyzer::passRecoMuon1MetTopologyCuts(int nobjM) {
  if (_DoMuon1DiscrByIsZllCut == "1") { if((isZmm(smearedMuonMomentumVector.at(nobjM)).first)) {return false;} }
  if (_DoDiscrByMuon1MetDphi == "1") {
    if(abs(normalizedPhi(smearedMuonMomentumVector.at(nobjM).Phi() - theMETVector.Phi())) > _Muon1MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedMuonMomentumVector.at(nobjM).Phi() - theMETVector.Phi())) < _Muon1MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByMuon1MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedMuonMomentumVector.at(nobjM))<_Muon1MetMtMinCut) || (CalculateLeptonMetMt(smearedMuonMomentumVector.at(nobjM))>_Muon1MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passRecoMuon2MetTopologyCuts(int nobjM) {
  if (_DoMuon2DiscrByIsZllCut == "1") { if((isZmm(smearedMuonMomentumVector.at(nobjM)).first)) {return false;} }
  if (_DoDiscrByMuon2MetDphi == "1") {
    if(abs(normalizedPhi(smearedMuonMomentumVector.at(nobjM).Phi() - theMETVector.Phi())) > _Muon2MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedMuonMomentumVector.at(nobjM).Phi() - theMETVector.Phi())) < _Muon2MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByMuon2MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedMuonMomentumVector.at(nobjM))<_Muon2MetMtMinCut) || (CalculateLeptonMetMt(smearedMuonMomentumVector.at(nobjM))>_Muon2MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passRecoElectron1MetTopologyCuts(int nobjE) {
  if (_DoElectron1DiscrByIsZllCut == "1") { if((isZee(smearedElectronMomentumVector.at(nobjE)).first)) {return false;} }
  if (_DoDiscrByElectron1MetDphi == "1") {
    if(abs(normalizedPhi(smearedElectronMomentumVector.at(nobjE).Phi() - theMETVector.Phi())) > _Electron1MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedElectronMomentumVector.at(nobjE).Phi() - theMETVector.Phi())) < _Electron1MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByElectron1MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedElectronMomentumVector.at(nobjE))<_Electron1MetMtMinCut) || (CalculateLeptonMetMt(smearedElectronMomentumVector.at(nobjE))>_Electron1MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passRecoElectron2MetTopologyCuts(int nobjE) {
  if (_DoElectron2DiscrByIsZllCut == "1") { if((isZee(smearedElectronMomentumVector.at(nobjE)).first)) {return false;} }
  if (_DoDiscrByElectron2MetDphi == "1") {
    if(abs(normalizedPhi(smearedElectronMomentumVector.at(nobjE).Phi() - theMETVector.Phi())) > _Electron2MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedElectronMomentumVector.at(nobjE).Phi() - theMETVector.Phi())) < _Electron2MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByElectron2MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedElectronMomentumVector.at(nobjE))<_Electron2MetMtMinCut) || (CalculateLeptonMetMt(smearedElectronMomentumVector.at(nobjE))>_Electron2MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passRecoTau1MetTopologyCuts(int nobjT) {
  if (_DoDiscrByTau1MetDphi == "1") {
    if(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - theMETVector.Phi())) > _Tau1MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - theMETVector.Phi())) < _Tau1MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByTau1MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedTauMomentumVector.at(nobjT))<_Tau1MetMtMinCut) || (CalculateLeptonMetMt(smearedTauMomentumVector.at(nobjT))>_Tau1MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passRecoTau2MetTopologyCuts(int nobjT) {
  if (_DoDiscrByTau2MetDphi == "1") {
    if(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - theMETVector.Phi())) > _Tau2MetDphiMaxCut) {return false;}
    if(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - theMETVector.Phi())) < _Tau2MetDphiMinCut) {return false;}
  }
  if (_DoDiscrByTau2MetMt == "1") {
    if( (CalculateLeptonMetMt(smearedTauMomentumVector.at(nobjT))<_Tau2MetMtMinCut) || (CalculateLeptonMetMt(smearedTauMomentumVector.at(nobjT))>_Tau2MetMtMaxCut) ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passDiTauTopologyCuts(int nobj1, int nobj2) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoDiTauDiscrByDeltaR == "1") {if(smearedTauMomentumVector.at(nobj1).DeltaR(smearedTauMomentumVector.at(nobj2)) < _DiTauDeltaRCut) {return false;}}
  // ----Opposite sign - Like sign requirement
  if (_DiTauDiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobj1) * Tau_charge->at(nobj2)) >= 0) {return false;}
  } else if (_DiTauDiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobj1) * Tau_charge->at(nobj2)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoDiTauDiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobj1).Phi() - smearedTauMomentumVector.at(nobj2).Phi()))) > _DiTauCosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobj1).Phi() - smearedTauMomentumVector.at(nobj2).Phi()))) < _DiTauCosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByDiTauMassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfDiTauProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxDiTauMassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2)).second.M() < _DiTauMassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2)).second.M() > _DiTauMassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoDiTauDiscrByCDFzeta2D == "1") {
    if( ((_DiTauPZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2))) +
         (_DiTauPZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2)))) < _DiTauCDFzeta2DMinCutValue )
      {return false;}
    if( ((_DiTauPZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2))) +
         (_DiTauPZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobj1),smearedTauMomentumVector.at(nobj2)))) > _DiTauCDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoDiTauDiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobj1).Pt() - smearedTauMomentumVector.at(nobj2).Pt()) / (smearedTauMomentumVector.at(nobj1).Pt() + smearedTauMomentumVector.at(nobj2).Pt())) < _DiTauDeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobj1).Pt() - smearedTauMomentumVector.at(nobj2).Pt()) / (smearedTauMomentumVector.at(nobj1).Pt() + smearedTauMomentumVector.at(nobj2).Pt())) > _DiTauDeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoDiTauDiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobj1).Pt() - smearedTauMomentumVector.at(nobj2).Pt())) < _DiTauDeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobj1).Pt() - smearedTauMomentumVector.at(nobj2).Pt())) > _DiTauDeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passDiMuonTopologyCuts(int nobj1, int nobj2) {
  // ----Separation cut between muon legs (remove double counting)
  if (_DoDiMuonDiscrByDeltaR == "1") { if(smearedMuonMomentumVector.at(nobj1).DeltaR(smearedMuonMomentumVector.at(nobj2)) < _DiMuonDeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_DiMuonDiscrByOSLSType == "OS") {
    if((Muon_charge->at(nobj1) * Muon_charge->at(nobj2)) >= 0) {return false;}
  } else if (_DiMuonDiscrByOSLSType == "LS") {
    if((Muon_charge->at(nobj1) * Muon_charge->at(nobj2)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoDiMuonDiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(nobj1).Phi() - smearedMuonMomentumVector.at(nobj2).Phi()))) > _DiMuonCosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedMuonMomentumVector.at(nobj1).Phi() - smearedMuonMomentumVector.at(nobj2).Phi()))) < _DiMuonCosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByDiMuonMassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfDiMuonProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxDiMuonMassReco;
    if( (CalculateTheDiTau4Momentum(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2)).second.M() < _DiMuonMassMinCut) || (CalculateTheDiTau4Momentum(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2)).second.M() > _DiMuonMassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoDiMuonDiscrByCDFzeta2D == "1") {
    if( ((_DiMuonPZetaCutCoefficient * CalculatePZeta(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2))) +
         (_DiMuonPZetaVisCutCoefficient * CalculatePZetaVis(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2)))) < _DiMuonCDFzeta2DMinCutValue )
      {return false;}
    if( ((_DiMuonPZetaCutCoefficient * CalculatePZeta(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2))) +
         (_DiMuonPZetaVisCutCoefficient * CalculatePZetaVis(smearedMuonMomentumVector.at(nobj1),smearedMuonMomentumVector.at(nobj2)))) > _DiMuonCDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoDiMuonDiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedMuonMomentumVector.at(nobj1).Pt() - smearedMuonMomentumVector.at(nobj2).Pt()) / (smearedMuonMomentumVector.at(nobj1).Pt() + smearedMuonMomentumVector.at(nobj2).Pt())) < _DiMuonDeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedMuonMomentumVector.at(nobj1).Pt() - smearedMuonMomentumVector.at(nobj2).Pt()) / (smearedMuonMomentumVector.at(nobj1).Pt() + smearedMuonMomentumVector.at(nobj2).Pt())) > _DiMuonDeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoDiMuonDiscrByDeltaPt == "1") {
    if ( ((smearedMuonMomentumVector.at(nobj1).Pt() - smearedMuonMomentumVector.at(nobj2).Pt())) < _DiMuonDeltaPtMinCutValue ) {return false;}
    if ( ((smearedMuonMomentumVector.at(nobj1).Pt() - smearedMuonMomentumVector.at(nobj2).Pt())) > _DiMuonDeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passDiElectronTopologyCuts(int nobj1, int nobj2) {
  // ----Separation cut between Electron legs (remove double counting)
  if (_DoDiElectronDiscrByDeltaR == "1") { if(smearedElectronMomentumVector.at(nobj1).DeltaR(smearedElectronMomentumVector.at(nobj2)) < _DiElectronDeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_DiElectronDiscrByOSLSType == "OS") {
    if((patElectron_charge->at(nobj1) * patElectron_charge->at(nobj2)) >= 0) {return false;}
  } else if (_DiElectronDiscrByOSLSType == "LS") {
    if((patElectron_charge->at(nobj1) * patElectron_charge->at(nobj2)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoDiElectronDiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedElectronMomentumVector.at(nobj1).Phi() - smearedElectronMomentumVector.at(nobj2).Phi()))) > _DiElectronCosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedElectronMomentumVector.at(nobj1).Phi() - smearedElectronMomentumVector.at(nobj2).Phi()))) < _DiElectronCosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByDiElectronMassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfDiElectronProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxDiElectronMassReco;
    if( (CalculateTheDiTau4Momentum(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2)).second.M() < _DiElectronMassMinCut) || (CalculateTheDiTau4Momentum(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2)).second.M() > _DiElectronMassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoDiElectronDiscrByCDFzeta2D == "1") {
    if( ((_DiElectronPZetaCutCoefficient * CalculatePZeta(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2))) +
         (_DiElectronPZetaVisCutCoefficient * CalculatePZetaVis(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2)))) < _DiElectronCDFzeta2DMinCutValue )
      {return false;}
    if( ((_DiElectronPZetaCutCoefficient * CalculatePZeta(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2))) +
         (_DiElectronPZetaVisCutCoefficient * CalculatePZetaVis(smearedElectronMomentumVector.at(nobj1),smearedElectronMomentumVector.at(nobj2)))) > _DiElectronCDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoDiElectronDiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedElectronMomentumVector.at(nobj1).Pt() - smearedElectronMomentumVector.at(nobj2).Pt()) / (smearedElectronMomentumVector.at(nobj1).Pt() + smearedElectronMomentumVector.at(nobj2).Pt())) < _DiElectronDeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedElectronMomentumVector.at(nobj1).Pt() - smearedElectronMomentumVector.at(nobj2).Pt()) / (smearedElectronMomentumVector.at(nobj1).Pt() + smearedElectronMomentumVector.at(nobj2).Pt())) > _DiElectronDeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoDiElectronDiscrByDeltaPt == "1") {
    if ( ((smearedElectronMomentumVector.at(nobj1).Pt() - smearedElectronMomentumVector.at(nobj2).Pt())) < _DiElectronDeltaPtMinCutValue ) {return false;}
    if ( ((smearedElectronMomentumVector.at(nobj1).Pt() - smearedElectronMomentumVector.at(nobj2).Pt())) > _DiElectronDeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passDiJetTopologyCuts(int nobj1, int nobj2) {
  // ----Separation cut between jets (remove overlaps)
  if (_DoDiJetDiscrByDeltaR == "1") {if(smearedJetMomentumVector.at(nobj1).DeltaR(smearedJetMomentumVector.at(nobj2)) < _DiJetDeltaRCut) {return false;}}
  if (_DoDiJetDiscrByDeltaEta == "1") {
    if(fabs(smearedJetMomentumVector.at(nobj1).Eta() - smearedJetMomentumVector.at(nobj2).Eta()) < _DiJetMinDeltaEtaCut) {return false;}
    if(fabs(smearedJetMomentumVector.at(nobj1).Eta() - smearedJetMomentumVector.at(nobj2).Eta()) > _DiJetMaxDeltaEtaCut) {return false;}
  }
  if (_DoDiJetDiscrByDeltaPhi == "1") {
    if(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi())) < _DiJetMinDeltaPhiCut) {return false;}
    if(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi())) > _DiJetMaxDeltaPhiCut) {return false;}
  } 
  if (_DoDiJetDiscrByOSEta == "1") {
    if((smearedJetMomentumVector.at(nobj1).Eta() * smearedJetMomentumVector.at(nobj2).Eta()) >= 0) {return false;}
  }
  // ----Require both legs to be almost back-to-back in phi
  if (_DoDiJetDiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi()))) > _DiJetCosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedJetMomentumVector.at(nobj1).Phi() - smearedJetMomentumVector.at(nobj2).Phi()))) < _DiJetCosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByDiJetMassReco == "1") {
    if( (CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(nobj1),smearedJetMomentumVector.at(nobj2)).second.M() < _DiJetMassMinCut) || (CalculateTheDiJet4Momentum(smearedJetMomentumVector.at(nobj1),smearedJetMomentumVector.at(nobj2)).second.M() > _DiJetMassMaxCut) ) {return false;}
  }
  return true;
}

// ---------------Apply Topology Cuts
bool BSM3GAnalyzer::passMuon1Tau1TopologyCuts(int nobjT, int nobjM) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoMuon1Tau1DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedMuonMomentumVector.at(nobjM)) < _Muon1Tau1DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Muon1Tau1DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) >= 0) {return false;}
  } else if (_Muon1Tau1DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoMuon1Tau1DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) > _Muon1Tau1CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) < _Muon1Tau1CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByMuon1Tau1MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon1Tau1ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau1MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() < _Muon1Tau1MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() > _Muon1Tau1MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoMuon1Tau1DiscrByCDFzeta2D == "1") {
    if( ((_Muon1Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon1Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) < _Muon1Tau1CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Muon1Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon1Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) > _Muon1Tau1CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoMuon1Tau1DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon1Tau1DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon1Tau1DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoMuon1Tau1DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon1Tau1DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon1Tau1DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

// ---------------Apply Topology Cuts
bool BSM3GAnalyzer::passMuon1Tau2TopologyCuts(int nobjT, int nobjM) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoMuon1Tau2DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedMuonMomentumVector.at(nobjM)) < _Muon1Tau2DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Muon1Tau2DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) >= 0) {return false;}
  } else if (_Muon1Tau2DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoMuon1Tau2DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) > _Muon1Tau2CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) < _Muon1Tau2CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByMuon1Tau2MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon1Tau2ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxMuon1Tau2MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() < _Muon1Tau2MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() > _Muon1Tau2MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoMuon1Tau2DiscrByCDFzeta2D == "1") {
    if( ((_Muon1Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon1Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) < _Muon1Tau2CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Muon1Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon1Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) > _Muon1Tau2CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoMuon1Tau2DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon1Tau2DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon1Tau2DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoMuon1Tau2DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon1Tau2DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon1Tau2DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

// ---------------Apply Topology Cuts
bool BSM3GAnalyzer::passMuon2Tau1TopologyCuts(int nobjT, int nobjM) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoMuon2Tau1DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedMuonMomentumVector.at(nobjM)) < _Muon2Tau1DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Muon2Tau1DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) >= 0) {return false;}
  } else if (_Muon2Tau1DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoMuon2Tau1DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) > _Muon2Tau1CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) < _Muon2Tau1CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByMuon2Tau1MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon2Tau1ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau1MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() < _Muon2Tau1MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() > _Muon2Tau1MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoMuon2Tau1DiscrByCDFzeta2D == "1") {
    if( ((_Muon2Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon2Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) < _Muon2Tau1CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Muon2Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon2Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) > _Muon2Tau1CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoMuon2Tau1DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon2Tau1DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon2Tau1DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoMuon2Tau1DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon2Tau1DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon2Tau1DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

// ---------------Apply Topology Cuts
bool BSM3GAnalyzer::passMuon2Tau2TopologyCuts(int nobjT, int nobjM) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoMuon2Tau2DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedMuonMomentumVector.at(nobjM)) < _Muon2Tau2DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Muon2Tau2DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) >= 0) {return false;}
  } else if (_Muon2Tau2DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * Muon_charge->at(nobjM)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoMuon2Tau2DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) > _Muon2Tau2CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedMuonMomentumVector.at(nobjM).Phi()))) < _Muon2Tau2CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByMuon2Tau2MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfMuon2Tau2ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxMuon2Tau2MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() < _Muon2Tau2MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)).second.M() > _Muon2Tau2MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoMuon2Tau2DiscrByCDFzeta2D == "1") {
    if( ((_Muon2Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon2Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) < _Muon2Tau2CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Muon2Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM))) +
         (_Muon2Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedMuonMomentumVector.at(nobjM)))) > _Muon2Tau2CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoMuon2Tau2DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon2Tau2DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon2Tau2DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoMuon2Tau2DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) < _Muon2Tau2DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedMuonMomentumVector.at(nobjM).Pt())) > _Muon2Tau2DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passElectron1Tau1TopologyCuts(int nobjT, int nobjE) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoElectron1Tau1DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedElectronMomentumVector.at(nobjE)) < _Electron1Tau1DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Electron1Tau1DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) >= 0) {return false;}
  } else if (_Electron1Tau1DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoElectron1Tau1DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) > _Electron1Tau1CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) < _Electron1Tau1CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByElectron1Tau1MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfElectron1Tau1ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxElectron1Tau1MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() < _Electron1Tau1MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() > _Electron1Tau1MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoElectron1Tau1DiscrByCDFzeta2D == "1") {
    if( ((_Electron1Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron1Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) < _Electron1Tau1CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Electron1Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron1Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) > _Electron1Tau1CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoElectron1Tau1DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron1Tau1DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron1Tau1DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoElectron1Tau1DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron1Tau1DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron1Tau1DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passElectron1Tau2TopologyCuts(int nobjT, int nobjE) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoElectron1Tau2DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedElectronMomentumVector.at(nobjE)) < _Electron1Tau2DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Electron1Tau2DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) >= 0) {return false;}
  } else if (_Electron1Tau2DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoElectron1Tau2DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) > _Electron1Tau2CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) < _Electron1Tau2CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByElectron1Tau2MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfElectron1Tau2ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxElectron1Tau2MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() < _Electron1Tau2MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() > _Electron1Tau2MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoElectron1Tau2DiscrByCDFzeta2D == "1") {
    if( ((_Electron1Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron1Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) < _Electron1Tau2CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Electron1Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron1Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) > _Electron1Tau2CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoElectron1Tau2DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron1Tau2DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron1Tau2DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoElectron1Tau2DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron1Tau2DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron1Tau2DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passElectron2Tau1TopologyCuts(int nobjT, int nobjE) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoElectron2Tau1DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedElectronMomentumVector.at(nobjE)) < _Electron2Tau1DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Electron2Tau1DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) >= 0) {return false;}
  } else if (_Electron2Tau1DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoElectron2Tau1DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) > _Electron2Tau1CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) < _Electron2Tau1CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByElectron2Tau1MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfElectron2Tau1ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxElectron2Tau1MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() < _Electron2Tau1MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() > _Electron2Tau1MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoElectron2Tau1DiscrByCDFzeta2D == "1") {
    if( ((_Electron2Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron2Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) < _Electron2Tau1CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Electron2Tau1PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron2Tau1PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) > _Electron2Tau1CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoElectron2Tau1DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron2Tau1DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron2Tau1DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoElectron2Tau1DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron2Tau1DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron2Tau1DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

bool BSM3GAnalyzer::passElectron2Tau2TopologyCuts(int nobjT, int nobjE) {
  // ----Separation cut between lepton and tau jet (remove overlaps)
  if (_DoElectron2Tau2DiscrByDeltaR == "1") { if(smearedTauMomentumVector.at(nobjT).DeltaR(smearedElectronMomentumVector.at(nobjE)) < _Electron2Tau2DeltaRCut) {return false;} }
  // ----Opposite sign - Like sign requirement
  if (_Electron2Tau2DiscrByOSLSType == "OS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) >= 0) {return false;}
  } else if (_Electron2Tau2DiscrByOSLSType == "LS") {
    if((Tau_charge->at(nobjT) * patElectron_charge->at(nobjE)) <= 0) {return false;}
  } else {}
  // ----Require both legs to be almost back-to-back in phi
  if (_DoElectron2Tau2DiscrByCosDphi == "1") {
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) > _Electron2Tau2CosDphiMaxCut) {return false;}
    if(cos(abs(normalizedPhi(smearedTauMomentumVector.at(nobjT).Phi() - smearedElectronMomentumVector.at(nobjE).Phi()))) < _Electron2Tau2CosDphiMinCut) {return false;}
  }
  // ----Mass window requirement
  if (_DoDiscrByElectron2Tau2MassReco == "1") {
    _UseVectorSumOfVisProductsAndMetMassReco = _UseVectorSumOfElectron2Tau2ProductsAndMetMassReco;
    _UseCollinerApproxMassReco = _UseCollinerApproxElectron2Tau2MassReco;
    if( (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() < _Electron2Tau2MassMinCut) || (CalculateTheDiTau4Momentum(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)).second.M() > _Electron2Tau2MassMaxCut) ) {return false;}
  }
  // ----Zeta requirement
  if (_DoElectron2Tau2DiscrByCDFzeta2D == "1") {
    if( ((_Electron2Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron2Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) < _Electron2Tau2CDFzeta2DMinCutValue )
      {return false;}
    if( ((_Electron2Tau2PZetaCutCoefficient * CalculatePZeta(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE))) + 
         (_Electron2Tau2PZetaVisCutCoefficient * CalculatePZetaVis(smearedTauMomentumVector.at(nobjT),smearedElectronMomentumVector.at(nobjE)))) > _Electron2Tau2CDFzeta2DMaxCutValue )
      {return false;}
  }
  // ----Missing transverse energy requirement
  if (_DoElectron2Tau2DiscrByDeltaPtDivSumPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron2Tau2DeltaPtDivSumPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt()) / (smearedTauMomentumVector.at(nobjT).Pt() + smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron2Tau2DeltaPtDivSumPtMaxCutValue ) {return false;}
  }
  if (_DoElectron2Tau2DiscrByDeltaPt == "1") {
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) < _Electron2Tau2DeltaPtMinCutValue ) {return false;}
    if ( ((smearedTauMomentumVector.at(nobjT).Pt() - smearedElectronMomentumVector.at(nobjE).Pt())) > _Electron2Tau2DeltaPtMaxCutValue ) {return false;}
  }
  return true;
}

pair<bool, pair<float, float> > BSM3GAnalyzer::isZmm(TLorentzVector theObject) {
  pair<bool, pair<float, float> > theOutPutPair;
  bool eventIsZmm = false;
  bool massWindow = false;
  bool ptAsymmWindow = false;
  const float zmmMass = 90.1876;
  const float zmmWidht = 2.4952;
  float zmmPtAsymmetry = -10.;
  pair<float, float> theMassPtAsymmPair;
  // if mass is within 3 sigmas of z or pt asymmetry is small set to true.
  for(int jmuon = 0; jmuon < Muon_pt->size(); jmuon++) {
    if(theObject.DeltaR(smearedMuonMomentumVector.at(jmuon)) < 0.3) continue;
    if(theObject == smearedMuonMomentumVector.at(jmuon)) continue;
    TLorentzVector The_LorentzVect = theObject + smearedMuonMomentumVector.at(jmuon);
    zmmPtAsymmetry = (theObject.Pt() - smearedMuonMomentumVector.at(jmuon).Pt())/(theObject.Pt() + smearedMuonMomentumVector.at(jmuon).Pt());
    theMassPtAsymmPair = std::make_pair<float, float>(The_LorentzVect.M(), float(zmmPtAsymmetry));

    if((The_LorentzVect.M() < (zmmMass + (3.*zmmWidht))) && (The_LorentzVect.M() > (zmmMass - (3.*zmmWidht)))) massWindow = true;
    if(fabs(zmmPtAsymmetry) < 0.20) ptAsymmWindow = true;
    if(massWindow || ptAsymmWindow){
      eventIsZmm = true;
      break;
    }
  }
  theOutPutPair = std::make_pair<bool, pair<float, float> >(bool(eventIsZmm), pair<float, float>(theMassPtAsymmPair));
  return theOutPutPair;
}

pair<bool, pair<float, float> > BSM3GAnalyzer::isZee(TLorentzVector theObject) {
  pair<bool, pair<float, float> > theOutPutPair;
  bool eventIsZee = false;
  bool massWindow = false;
  bool ptAsymmWindow = false;
  const float zeeMass = 90.1876;
  const float zeeWidht = 2.4952;
  float zeePtAsymmetry = -10.;
  pair<float, float> theMassPtAsymmPair;
  // if mass is within 3 sigmas of z or pt asymmetry is small set to true.
  for(int je = 0; je < patElectron_pt->size(); je++) {
    if(theObject.DeltaR(smearedElectronMomentumVector.at(je)) < 0.3) continue;
    if(theObject == smearedElectronMomentumVector.at(je)) continue;
    TLorentzVector The_LorentzVect = theObject + smearedElectronMomentumVector.at(je);
    zeePtAsymmetry = (theObject.Pt() - smearedElectronMomentumVector.at(je).Pt())/(theObject.Pt() + smearedElectronMomentumVector.at(je).Pt());
    theMassPtAsymmPair = std::make_pair<float, float>(The_LorentzVect.M(), float(zeePtAsymmetry));

    if((The_LorentzVect.M() < (zeeMass + (3.*zeeWidht))) && (The_LorentzVect.M() > (zeeMass - (3.*zeeWidht)))) massWindow = true;
    if(fabs(zeePtAsymmetry) < 0.20) ptAsymmWindow = true;
    if(massWindow || ptAsymmWindow){
      eventIsZee = true;
      break;
    }
  }
  theOutPutPair = std::make_pair<bool, pair<float, float> >(bool(eventIsZee), pair<float, float>(theMassPtAsymmPair));
  return theOutPutPair;
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
  IsItAHadronicDecayVector.clear(); // clear any previous declaration from memory
  vector<int> tempTauIndexVector; // vector which contains the index (in the gen collection) of the gen level taus (before decaying)
  tempTauIndexVector.clear(); // clear any previous declaration from memory
  vector<TLorentzVector> tempNeutrinoMomentumVector; // vector of lorentz 4-momentum vectors for each tau neutrino from the tau decay
  tempNeutrinoMomentumVector.clear(); // clear any previous declaration from memory

  //---Loop over gen particles to find the tau neutrinos and then store the index of each tau neutrino's mother (a tau).
  //---Also store the tau neutrino's 4-momentum vector in order to calculate the visible tau 4-momentum at a later point.
  for(int jj = 0; jj < Gen_pt->size(); jj++) {
    if( (abs(Gen_pdg_id->at(jj)) == 16) && (abs(Gen_pdg_id->at(Gen_BmotherIndex->at(jj))) == 15) && (Gen_status->at(Gen_BmotherIndex->at(jj)) == 2) ) {
      tempTauIndexVector.push_back(Gen_BmotherIndex->at(jj));
      theNeutrinoObject.SetPtEtaPhiE(Gen_pt->at(jj), Gen_eta->at(jj), Gen_phi->at(jj), Gen_energy->at(jj));
      tempNeutrinoMomentumVector.push_back(theNeutrinoObject);
    }
  }

  //---Perform matching only if there is at least one gen tau in the event
  if(tempTauIndexVector.size() > 0) {
    //---Loop over the gen taus and determine whether it decays hadronically.
    for(int jjj = 0; jjj < tempTauIndexVector.size(); jjj++) {
      IsItAHadronicDecay = true;
      for(int jj = 0; jj < Gen_pt->size(); jj++) {
        if( ((abs(Gen_pdg_id->at(jj)) == 12) || (abs(Gen_pdg_id->at(jj)) == 14)) && (Gen_BmotherIndex->at(jj) == tempTauIndexVector.at(jjj)) ) {
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
  //TFile *file1 = new TFile (cstr);
  TFile *file1 = TFile::Open(cstr);
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
  //TFile *file1 = new TFile (cstr);
  TFile *file1 = TFile::Open(cstr);
  TH1 *hist = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!hist) {throw std::runtime_error("failed to extract histogram");}
  
  double result = hist->GetBinContent(bin);
  file1->Close();
  return result;
}

double BSM3GAnalyzer::getintegral(char * cstr, int bin) {
  //TFile *file1 = new TFile (cstr);
  TFile *file1 = TFile::Open(cstr);
  TH1 *hist = dynamic_cast<TH1*>(file1->Get("analyzeHiMassTau/NVertices_0"));
  if(!hist) {throw std::runtime_error("failed to extract histogram");}
  
  double result = hist->Integral();
  file1->Close();
  return result;
}

BSM3GAnalyzer::~BSM3GAnalyzer() { }

void BSM3GAnalyzer::setBranchAddress(TTree* BOOM) {

  //---set object pointer
  Trigger_decision = 0;
  Trigger_names = 0;
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
  Muon_charge = 0;
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
  patElectron_charge = 0;
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
  Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byTightCombinedIsolationDeltaBetaCorr3Hits = 0;
  Tau_byLooseIsolationMVA3newDMwLT = 0;
  //Tau_byLooseIsolationMVA3newDMwoLT = 0;
  Tau_byLooseIsolationMva3oldDMwLT = 0;
  //Tau_byLooseIsolationMVA3oldDMwoLT = 0;
  Tau_byMediumIsolationMVA3newDMwLT = 0;
  //Tau_byMediumIsolationMVA3newDMwoLT = 0;
  Tau_byMediumIsolationMva3oldDMwLT = 0;
  //Tau_byMediumIsolationMVA3oldDMwoLT = 0;
  Tau_byTightIsolationMVA3newDMwLT = 0;
  //Tau_byTightIsolationMVA3newDMwoLT = 0;
  Tau_byTightIsolationMva3oldDMwLT = 0;
  //Tau_byTightIsolationMVA3oldDMwoLT = 0;
  //Tau_againstMuonLoose2 = 0;
  Tau_againstMuonLoose3 = 0;
  //Tau_againstMuonTight2 = 0;
  Tau_againstElectronMVALooseMVA5 = 0;
  Tau_againstElectronMVAMediumMVA5 = 0;
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
  Jet_partonFlavour = 0;
  Jet_bDiscriminator = 0;
  Gen_eta = 0;
  Gen_phi = 0;
  Gen_pt = 0;
  Gen_energy = 0;
  Gen_pdg_id = 0;
  Gen_motherpdg_id = 0;
  Gen_status = 0;
  Gen_BmotherIndex = 0;
  nTruePUInteractions = 0;
  bestVertices = 0;

  //---Set branch addresses and branch pointers
  if(!BOOM) return;
  BOOM->SetBranchAddress("Trigger_decision", &Trigger_decision, &b_Trigger_decision);
  BOOM->SetBranchAddress("Trigger_names", &Trigger_names, &b_Trigger_names);
  BOOM->SetBranchAddress("Gen_pt", &Gen_pt, &b_Gen_pt);
  BOOM->SetBranchAddress("Gen_eta", &Gen_eta, &b_Gen_eta);
  BOOM->SetBranchAddress("Gen_phi", &Gen_phi, &b_Gen_phi);
  BOOM->SetBranchAddress("Gen_energy", &Gen_energy, &b_Gen_energy);
  BOOM->SetBranchAddress("Gen_pdg_id", &Gen_pdg_id, &b_Gen_pdg_id);
  BOOM->SetBranchAddress("Gen_motherpdg_id", &Gen_motherpdg_id, &b_Gen_motherpdg_id);
  BOOM->SetBranchAddress("Gen_status", &Gen_status, &b_Gen_status);
  BOOM->SetBranchAddress("Gen_BmotherIndex", &Gen_BmotherIndex, &b_Gen_BmotherIndex);
  BOOM->SetBranchAddress("nTruePUInteractions", &nTruePUInteractions, &b_nTruePUInteractions);
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
  BOOM->SetBranchAddress("Muon_charge", &Muon_charge, &b_Muon_charge);
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
  BOOM->SetBranchAddress("patElectron_charge", &patElectron_charge, &b_patElectron_charge);
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
  BOOM->SetBranchAddress("Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits", &Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", &Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byTightCombinedIsolationDeltaBetaCorr3Hits", &Tau_byTightCombinedIsolationDeltaBetaCorr3Hits, &b_Tau_byTightCombinedIsolationDeltaBetaCorr3Hits);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3newDMwLT", &Tau_byLooseIsolationMVA3newDMwLT, &b_Tau_byLooseIsolationMVA3newDMwLT);
  //BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3newDMwoLT", &Tau_byLooseIsolationMVA3newDMwoLT, &b_Tau_byLooseIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byLooseIsolationMva3oldDMwLT", &Tau_byLooseIsolationMva3oldDMwLT, &b_Tau_byLooseIsolationMva3oldDMwLT);
  //BOOM->SetBranchAddress("Tau_byLooseIsolationMVA3oldDMwoLT", &Tau_byLooseIsolationMVA3oldDMwoLT, &b_Tau_byLooseIsolationMVA3oldDMwoLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3newDMwLT", &Tau_byMediumIsolationMVA3newDMwLT, &b_Tau_byMediumIsolationMVA3newDMwLT);
  //BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3newDMwoLT", &Tau_byMediumIsolationMVA3newDMwoLT, &b_Tau_byMediumIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byMediumIsolationMva3oldDMwLT", &Tau_byMediumIsolationMva3oldDMwLT, &b_Tau_byMediumIsolationMva3oldDMwLT);
  //BOOM->SetBranchAddress("Tau_byMediumIsolationMVA3oldDMwoLT", &Tau_byMediumIsolationMVA3oldDMwoLT, &b_Tau_byMediumIsolationMVA3oldDMwoLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMVA3newDMwLT", &Tau_byTightIsolationMVA3newDMwLT, &b_Tau_byTightIsolationMVA3newDMwLT);
  //BOOM->SetBranchAddress("Tau_byTightIsolationMVA3newDMwoLT", &Tau_byTightIsolationMVA3newDMwoLT, &b_Tau_byTightIsolationMVA3newDMwoLT);
  BOOM->SetBranchAddress("Tau_byTightIsolationMva3oldDMwLT", &Tau_byTightIsolationMva3oldDMwLT, &b_Tau_byTightIsolationMva3oldDMwLT);
  //BOOM->SetBranchAddress("Tau_byTightIsolationMVA3oldDMwoLT", &Tau_byTightIsolationMVA3oldDMwoLT, &b_Tau_byTightIsolationMVA3oldDMwoLT);
  //BOOM->SetBranchAddress("Tau_againstMuonLoose2", &Tau_againstMuonLoose2, &b_Tau_againstMuonLoose2);
  BOOM->SetBranchAddress("Tau_againstMuonLoose3", &Tau_againstMuonLoose3, &b_Tau_againstMuonLoose3);
  //BOOM->SetBranchAddress("Tau_againstMuonTight2", &Tau_againstMuonTight2, &b_Tau_againstMuonTight2);
  BOOM->SetBranchAddress("Tau_againstElectronMVALooseMVA5", &Tau_againstElectronMVALooseMVA5, &b_Tau_againstElectronMVALooseMVA5);
  BOOM->SetBranchAddress("Tau_againstElectronMVAMediumMVA5", &Tau_againstElectronMVAMediumMVA5, &b_Tau_againstElectronMVAMediumMVA5);
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
  BOOM->SetBranchAddress("Jet_partonFlavour", &Jet_partonFlavour, &b_Jet_partonFlavour);
  BOOM->SetBranchAddress("Jet_bDiscriminator", &Jet_bDiscriminator, &b_Jet_bDiscriminator);
  BOOM->SetBranchAddress("Met_type1PF_px", &Met_type1PF_px, &b_Met_type1PF_px);
  BOOM->SetBranchAddress("Met_type1PF_py", &Met_type1PF_py, &b_Met_type1PF_py);
  BOOM->SetBranchAddress("Met_type1PF_pz", &Met_type1PF_pz, &b_Met_type1PF_pz);

};

