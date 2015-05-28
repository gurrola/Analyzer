//////////////////////////////////////////////////////////////////////////////
// Authors:     Alfredo Gurrola, Andres Florez                              //
// contact:                                                                 //
//   Alfredo.Gurrola@cern.ch       (Vanderbilt University)                  //
//   Andres.Florez@cern.ch         (Los Andes University)                   //
//////////////////////////////////////////////////////////////////////////////

#ifndef BSM3GAnalyzer_h
#define BSM3GAnalyzer_h

// system include files
#include <memory>

// user include files
#include <Math/VectorUtil.h>
#include <fstream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <TRandom3.h>
#include <TMath.h>
#include <iostream>
#include <iomanip>
#include <utility>
#include <TROOT.h>
#include <TBranch.h>
#include <TApplication.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TLorentzVector.h>

using namespace std;

class BSM3GAnalyzer {
public:
  BSM3GAnalyzer(TFile*);
  ~BSM3GAnalyzer();

private:

  virtual void beginJob() ;
  virtual void analyze(TFile*);
  virtual void endJob(TFile*);
  virtual void getInputs();

  void fillHistograms(unsigned int);
  void getEventFlags();
  bool passEventSelectionSequence();
  bool pass_i_EventSelectionSequence(unsigned int);
  void bookHistograms(TFile*, string, unsigned int);
  void writeHistograms(TFile*, string, unsigned int);
  void setMapSelectionAlgoIDs();
  void initMapSelectionCounters();
  void printEfficiency();
  int getBin(char * cstr, int nVertices);
  int ExtractNumberOfGoodGenTaus();
  int ExtractNumberOfGoodGenTops();
  int ExtractNumberOfGoodGenElectrons();
  int ExtractNumberOfGoodGenMuons();
  int ExtractNumberOfGoodGenZs();
  int ExtractNumberOfGoodGenWs();
  int ExtractNumberOfGoodGenSMHiggs();
  bool passRecoMuon1Cuts(int);
  bool passRecoMuon2Cuts(int);
  TLorentzVector SmearMuon(int);
  TLorentzVector SmearTau(int);
  pair<bool, TLorentzVector> matchMuonToGen(TLorentzVector);
  pair<bool, TLorentzVector> matchTauToGen(TLorentzVector);
  double getvalue(char * cstr, int bin);
  double getintegral(char * cstr, int bin);
  double getPileupWeight(float);

  // ----------member data ---------------------------

  //-----Generator level Inputs 
  float _GenTauPtMinCut;
  float _GenTauPtMaxCut;
  float _GenTauEtaMaxCut;
  double _MuonToGenMatchingDeltaR;
  double _TauToGenMatchingDeltaR;

  string _DataHistos;
  string _MCHistos;

  //-----Fill Histograms?
  string _FillGenHists;

  //-----histogram that keeps track of the number of analyzed events & the number of
  //-----events passing the user defined cuts
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hEvents;

  //-----generator level histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNGenTau;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauEnergy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNGenMuon;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonEnergy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonPhi;

  //-----Event Sequence inputTags
  int _GenTauNmin;
  int _GenTauNmax;
  int _GenTopNmin;
  int _GenTopNmax;
  int _GenElectronNmin;
  int _GenElectronNmax;
  int _GenMuonNmin;
  int _GenMuonNmax;
  int _GenZNmin;
  int _GenZNmax;
  int _GenWNmin;
  int _GenWNmax;
  int _GenSMHiggsNmin;
  int _GenSMHiggsNmax;
  int _RecoVertexNmin;
  int _RecoVertexNmax;
  int _RecoMuon1Nmin;
  int _RecoMuon1Nmax;
  int _RecoMuon2Nmin;
  int _RecoMuon2Nmax;
  vector<string> _EventSelectionSequence;
  vector<string> _TopologicalSelectionSequence;

  //-----string to determine if the sample is real data or simulation
  string isData;
  string _MatchMuonToGen;
  string _UseMuonMotherId;
  int _MuonMotherId;

  //-----Event flags
  vector<bool> _EventFlag;

  //-----Event counters
  int _totalEvents;
  int _totalEventsPassingCuts;

  //-----Mapping of cut names and cut id
  std::map<string,int> _mapSelectionAlgoID;
  std::map<string,int> _mapSelectionCounter;
  std::map<string,int> _mapSelectionCounterCumul;

  //-----Inputs for systematic uncertainties
  double isrgluon_weight;
  double pu_weight;
  string _CalculatePUSystematics;
  std::vector<TLorentzVector> smearedMuonMomentumVector;
  std::vector<TLorentzVector> smearedTauMomentumVector;
  TLorentzVector theMETVector;
  double deltaForMEx;
  double deltaForMEy;
  string _SmearTheMuon;
  string _SmearTheTau;
  double _MuonPtScaleOffset;
  double _MuonPtSigmaOffset;
  double _MuonEtaScaleOffset;
  double _MuonEtaSigmaOffset;
  double _MuonPhiScaleOffset;
  double _MuonPhiSigmaOffset;
  double _MuonEnergyScaleOffset;
  double _MuonEnergySigmaOffset;
  double _TauPtScaleOffset;
  double _TauPtSigmaOffset;
  double _TauEtaScaleOffset;
  double _TauEtaSigmaOffset;
  double _TauPhiScaleOffset;
  double _TauPhiSigmaOffset;
  double _TauEnergyScaleOffset;
  double _TauEnergySigmaOffset;

  string _TreatMuonsAsNeutrinos;
  string _DoRecoMuon1DiscrByTightID;
  string _DoRecoMuon1DiscrBySoftID;
  string _DoRecoMuon1DiscrByIsolation;
  float _RecoMuon1PtMinCut;
  float _RecoMuon1PtMaxCut;
  float _RecoMuon1EtaCut;
  float _RecoMuon1IsoSumPtMinCutValue;
  float _RecoMuon1IsoSumPtMaxCutValue;
  string _DoRecoMuon2DiscrByTightID;
  string _DoRecoMuon2DiscrBySoftID;
  string _DoRecoMuon2DiscrByIsolation;
  float _RecoMuon2PtMinCut;
  float _RecoMuon2PtMaxCut;
  float _RecoMuon2EtaCut;
  float _RecoMuon2IsoSumPtMinCutValue;
  float _RecoMuon2IsoSumPtMaxCutValue;

  //-----For PDF weights
  vector<double> pdfWeightVector;

  // Define Branches
  void setBranchAddress(TTree* BOOM);
  vector<double>  *Muon_pt;
  vector<double>  *Muon_eta;
  vector<double>  *Muon_phi;
  vector<double>  *Muon_energy;
  vector<bool>    *Muon_tight;
  vector<bool>    *Muon_soft;
  vector<double>  *Muon_isoCharged;
  vector<double>  *Muon_isoNeutralHadron;
  vector<double>  *Muon_isoPhoton;
  vector<double>  *Muon_isoPU;
  vector<double>  *Tau_eta;
  vector<double>  *Tau_phi;
  vector<double>  *Tau_pt;
  vector<double>  *Tau_energy;
  vector<double>  *Gen_pt;
  vector<double>  *Gen_eta;
  vector<double>  *Gen_phi;
  vector<double>  *Gen_energy;
  vector<double>  *Gen_pdg_id;
  vector<double>  *Gen_motherpdg_id;
  vector<double>  *Gen_status;
  vector<int>  *Gen_BmotherIndices;
  Float_t         trueInteractions;
  Int_t           bestVertices;
  Double_t        Met_px;
  Double_t        Met_py;
  Double_t        Met_pz;

  // List of branches
  TBranch        *b_Gen_pt;   //!
  TBranch        *b_Gen_eta;   //!
  TBranch        *b_Gen_phi;   //!
  TBranch        *b_Gen_energy;   //!
  TBranch        *b_Gen_pdg_id;   //!
  TBranch        *b_Gen_motherpdg_id;   //!
  TBranch        *b_Gen_status;   //!
  TBranch        *b_Gen_BmotherIndices;   //!
  TBranch        *b_trueInteractions;   //!
  TBranch        *b_bestVertices;   //!
  TBranch        *b_Muon_pt;   //!
  TBranch        *b_Muon_eta;   //!
  TBranch        *b_Muon_phi;   //!
  TBranch        *b_Muon_energy;   //!
  TBranch        *b_Muon_tight;   //!
  TBranch        *b_Muon_soft;   //!
  TBranch        *b_Muon_isoCharged;   //!
  TBranch        *b_Muon_isoNeutralHadron;   //!
  TBranch        *b_Muon_isoPhoton;   //!
  TBranch        *b_Muon_isoPU;   //!
  TBranch        *b_Tau_eta;   //!
  TBranch        *b_Tau_phi;   //!
  TBranch        *b_Tau_pt;   //!
  TBranch        *b_Tau_energy;   //!
  TBranch        *b_Met_px;   //!
  TBranch        *b_Met_py;   //!
  TBranch        *b_Met_pz;   //!

};
#endif
