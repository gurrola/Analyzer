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
#include <TEnv.h>
using namespace std;

class BSM3GAnalyzer {
public:
  BSM3GAnalyzer(TFile*, char*);
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
  void initializePileupInfo();
  void printEfficiency();
  int getBin(char * cstr, int nVertices);
  int ExtractNumberOfGoodGenTaus();
  int ExtractNumberOfGoodGenTops();
  int ExtractNumberOfGoodGenElectrons();
  int ExtractNumberOfGoodGenMuons();
  int ExtractNumberOfGoodGenZs();
  int ExtractNumberOfGoodGenWs();
  int ExtractNumberOfGoodGenSMHiggs();
  bool passRecoTrigger1Cuts();
  bool passRecoTrigger2Cuts();
  bool passRecoMuon1Cuts(int);
  bool passRecoMuon2Cuts(int);
  bool passRecoElectron1Cuts(int);
  bool passRecoElectron2Cuts(int);
  bool passRecoTau1Cuts(int);
  bool passRecoTau2Cuts(int);
  bool passRecoJet1Cuts(int);
  bool passRecoJet2Cuts(int);
  bool passRecoCentralJetCuts(int);
  bool passRecoFirstLeadingJetCuts(int);
  bool passRecoSecondLeadingJetCuts(int);
  bool passRecoBJetCuts(int);
  bool passedLooseJetID(int);
  bool passSusyTopologyCuts(int, int);
  bool passRecoMuon1MetTopologyCuts(int);
  bool passRecoMuon2MetTopologyCuts(int);
  bool passRecoElectron1MetTopologyCuts(int);
  bool passRecoElectron2MetTopologyCuts(int);
  bool passRecoTau1MetTopologyCuts(int);
  bool passRecoTau2MetTopologyCuts(int);
  bool passDiTauTopologyCuts(int, int);
  bool passDiMuonTopologyCuts(int, int);
  bool passDiElectronTopologyCuts(int, int);
  bool passDiJetTopologyCuts(int, int);
  bool passMuon1Tau1TopologyCuts(int, int);
  bool passMuon1Tau2TopologyCuts(int, int);
  bool passMuon2Tau1TopologyCuts(int, int);
  bool passMuon2Tau2TopologyCuts(int, int);
  bool passElectron1Tau1TopologyCuts(int, int);
  bool passElectron1Tau2TopologyCuts(int, int);
  bool passElectron2Tau1TopologyCuts(int, int);
  bool passElectron2Tau2TopologyCuts(int, int);
  std::pair<bool, TLorentzVector> CalculateTheDiJet4Momentum(TLorentzVector, TLorentzVector);
  std::pair<bool, TLorentzVector> CalculateTheDiTau4Momentum(TLorentzVector, TLorentzVector);
  std::pair<bool, pair<float, float> > isZmm(TLorentzVector);
  std::pair<bool, pair<float, float> > isZee(TLorentzVector);
  double CalculateLeptonMetMt(TLorentzVector);
  double CalculatePZeta(TLorentzVector, TLorentzVector);
  double CalculatePZetaVis(TLorentzVector, TLorentzVector);
  TLorentzVector SmearMuon(int);
  TLorentzVector SmearTau(int);
  TLorentzVector SmearElectron(int);
  TLorentzVector SmearJet(int);
  bool isInTheCracks(float);
  pair<bool, TLorentzVector> matchMuonToGen(TLorentzVector);
  pair<bool, TLorentzVector> matchElectronToGen(TLorentzVector);
  pair<bool, TLorentzVector> matchTauToGen(TLorentzVector);
  double getvalue(char * cstr, int bin);
  double getintegral(char * cstr, int bin);
  double getPileupWeight(float);
  double normalizedPhi(double);

  // ----------member data ---------------------------

  //-----Generator level Inputs 
  float _GenTauPtMinCut;
  float _GenTauPtMaxCut;
  float _GenTauEtaMaxCut;
  double _MuonToGenMatchingDeltaR;
  double _ElectronToGenMatchingDeltaR;
  double _TauToGenMatchingDeltaR;

  string _DataHistos;
  string _MCHistos;

  TH1F *hPUmc = new TH1F("hPUmc", "hPUmc", 100, 0, 100);
  TH1F *hPUdata = new TH1F("hPUdata", "hPUdata", 100, 0, 100);

  //-----Fill Histograms?
  string _FillRecoVertexHists;
  string _FillGenHists;
  string _FillRecoTauHists;      
  string _FillRecoMuonHists;     
  string _FillRecoJetHists;    
  string _FillTopologyHists;     

  //-----histogram that keeps track of the number of analyzed events & the number of
  //-----events passing the user defined cuts
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hEvents;

  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNPVertices;

  //-----generator level histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNGenTau;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNGenHadTau;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauEnergy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenHadTauPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenHadTauEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNGenMuon;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonEnergy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenMuonPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenDiTauMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenZprimeMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenZprimeStatusCode;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hGenTauStatusCode;

  //-----reconstruction level tau histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNTau1;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingTauJet1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingTauJet1Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1NumSignalTracks;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1SeedTrackPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet1Charge;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNTau2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingTauJet2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingTauJet2Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2NumSignalTracks;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2SeedTrackPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTauJet2Charge;

  //-----reconstruction level muon histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNMuon1;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingMuon1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingMuon1Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNMuon2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingMuon2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingMuon2Eta;

  //-----reconstruction level jet histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNJet1;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNJet2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNCentralJet;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hNBJet;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hBJetEnergy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hBJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hBJetEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hBJetPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet1Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet2Energy;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hCentralJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hCentralJetEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet1Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet1Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet2Eta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hJet2Phi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hSecondLeadingJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hFirstLeadingJetEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hSecondLeadingJetEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMHT;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hHT;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMeff;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadingJetsMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadSublDijetDphi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMetVsDiJetDeltaPhiLeadSubl;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hDeltaEtaVsDeltaPhiLeadSubl;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadingJetsPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2_Muon1IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2_Muon2IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMetDiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetDeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetDeltaEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiJetMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiJetDeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiJetDeltaEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadingJetsDeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadingJetsDeltaEta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hLeadDiJetEtaProduct;

  //-----reconstruction level topology histograms
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2_Muon1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2_Muon2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuon_Muon1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuon_Muon2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuon_Muon1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuon_Muon2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1PtVsMuon2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Muon2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1MetDeltaPhiVsMuon1Muon2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonNotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonNotReconstructableMassOS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonReconstructableMassOS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonNotReconstructableMassLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonReconstructableMassLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonPZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonPZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hDiMuonZeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiMuonZeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2_Tau1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2_Tau2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hTau1PtVsTau2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTau_Tau1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTau_Tau2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hTau1Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTau_Tau1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTau_Tau2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hTau1MetDeltaPhiVsTau1Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauPZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauPZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hDiTauZeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauZeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauNotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDiTauDiJetReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMet;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hR1;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hR2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDphi1;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDphi2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDphi1MHT;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hDphi2MHT;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hAlpha;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hDphi1VsDphi2;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Tau1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Muon1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Muon1IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1PtVsTau1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Muon1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Tau1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Muon1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1_Tau1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1MetDeltaPhiVsMuon1Tau1CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1NotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1ReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1PZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1PZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1Tau1Zeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1Zeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau1DiJetReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Tau2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Muon1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Muon1IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1PtVsTau2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Muon1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Tau2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Muon1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2_Tau2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1MetDeltaPhiVsMuon1Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2NotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2ReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2PZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2PZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon1Tau2Zeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2Zeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon1Tau2DiJetReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Tau1DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Muon2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Muon2IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2PtVsTau1Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Muon2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Tau1MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Muon2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1_Tau1MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2MetDeltaPhiVsMuon2Tau1CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1NotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1ReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1PZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1PZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2Tau1Zeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1Zeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau1DiJetReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Tau2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Muon2DiJetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Muon2IsZmm;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2PtVsTau2Pt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2DeltaR;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2DeltaPtDivSumPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2DeltaPt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Muon2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Tau2MetMt;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2OSLS;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Muon2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2_Tau2MetDeltaPhi;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2MetDeltaPhiVsMuon2Tau2CosDphi;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2NotReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2ReconstructableMass;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2PZeta;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2PZetaVis;
  std::map< unsigned int, std::map<unsigned int, TH2*>> _hMuon2Tau2Zeta2D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2Zeta1D;
  std::map< unsigned int, std::map<unsigned int, TH1*>> _hMuon2Tau2DiJetReconstructableMass;

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
  int _RecoElectron1Nmin;
  int _RecoElectron1Nmax;
  int _RecoElectron2Nmin;
  int _RecoElectron2Nmax;
  int _RecoTau1Nmin;
  int _RecoTau1Nmax;
  int _RecoTau2Nmin;
  int _RecoTau2Nmax;
  int _RecoJet1Nmin;
  int _RecoJet1Nmax;
  int _RecoJet2Nmin;
  int _RecoJet2Nmax;
  int _RecoCentralJetNmin;
  int _RecoCentralJetNmax;
  int _RecoFirstLeadingJetNmin;
  int _RecoSecondLeadingJetNmin;
  int _RecoBJetNmin;
  int _RecoBJetNmax;
  int _SusyCombinationsNmin;
  int _RecoMuon1MetTopologyNmin;
  int _RecoMuon1MetTopologyNmax;
  int _RecoMuon2MetTopologyNmin;
  int _RecoMuon2MetTopologyNmax;
  int _RecoElectron1MetTopologyNmin;
  int _RecoElectron1MetTopologyNmax;
  int _RecoElectron2MetTopologyNmin;
  int _RecoElectron2MetTopologyNmax;
  int _RecoTau1MetTopologyNmin;
  int _RecoTau1MetTopologyNmax;
  int _RecoTau2MetTopologyNmin;
  int _RecoTau2MetTopologyNmax;
  int _DiMuonCombinationsNmin;
  int _DiMuonCombinationsNmax;
  int _DiElectronCombinationsNmin;
  int _DiElectronCombinationsNmax;
  int _DiTauCombinationsNmin;
  int _DiTauCombinationsNmax;
  int _DiJetCombinationsNmin;
  int _DiJetCombinationsNmax;
  int _Muon1Tau1CombinationsNmin;
  int _Muon1Tau1CombinationsNmax;
  int _Muon1Tau2CombinationsNmin;
  int _Muon1Tau2CombinationsNmax;
  int _Muon2Tau1CombinationsNmin;
  int _Muon2Tau1CombinationsNmax;
  int _Muon2Tau2CombinationsNmin;
  int _Muon2Tau2CombinationsNmax;
  int _Electron1Tau1CombinationsNmin;
  int _Electron1Tau1CombinationsNmax;
  int _Electron1Tau2CombinationsNmin;
  int _Electron1Tau2CombinationsNmax;
  int _Electron2Tau1CombinationsNmin;
  int _Electron2Tau1CombinationsNmax;
  int _Electron2Tau2CombinationsNmin;
  int _Electron2Tau2CombinationsNmax;
  int _RecoTriggers2Nmin;
  int _RecoTriggers1Nmin;
  vector<string> _EventSelectionSequence;
  vector<string> _TopologicalSelectionSequence;

  //-----string to determine if the sample is real data or simulation
  string isData;
  string _MatchMuonToGen;
  string _MatchElectronToGen;
  string _MatchTauToGen;
  string _UseMuonMotherId;
  int _MuonMotherId;
  string _UseElectronMotherId;
  int _ElectronMotherId;
  string _MatchBToGen;

  //-----Event flags
  vector<bool> _EventFlag;

  //-----Event counters
  int _totalEvents;
  int _totalEventsPassingCuts;

  //-----Mapping of cut names and cut id
  std::map<string,int> _mapSelectionAlgoID;
  std::map<string,int> _mapSelectionCounter;
  std::map<string,int> _mapSelectionCounterCumul;

  TLorentzVector TheLeadDiJetVect;

  //-----Inputs for systematic uncertainties
  double isrgluon_weight;
  double pu_weight;
  string _CalculatePUSystematics;
  std::vector<TLorentzVector> smearedMuonMomentumVector;
  std::vector<TLorentzVector> smearedTauMomentumVector;
  std::vector<TLorentzVector> smearedElectronMomentumVector;
  std::vector<TLorentzVector> smearedJetMomentumVector;
  TLorentzVector theMETVector;
  double deltaForMEx;
  double deltaForMEy;
  double sumpxForMht;
  double sumpyForMht;
  double sumptForHt;
  double phiForMht;
  double leadingjetpt;
  double leadingjeteta;
  int theLeadingJetIndex;
  double secondleadingjetpt;
  double secondleadingjeteta;
  int theSecondLeadingJetIndex;
  string _SmearTheMuon;
  string _SmearTheTau;
  string _SmearTheElectron;
  string _SmearTheJet;
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
  double _ElectronPtScaleOffset;
  double _ElectronPtSigmaOffset;
  double _ElectronEtaScaleOffset;
  double _ElectronEtaSigmaOffset;
  double _ElectronPhiScaleOffset;
  double _ElectronPhiSigmaOffset;
  double _ElectronEnergyScaleOffset;
  double _ElectronEnergySigmaOffset;

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

  float _RecoElectron1EtaCut;
  float _RecoElectron1PtMinCut;
  float _RecoElectron1PtMaxCut;
  float _RecoElectron1IsoSumPtMinCutValue;
  float _RecoElectron1IsoSumPtMaxCutValue;
  string _DoRecoElectron1DiscrByIsolation;
  string _DoRecoElectron1DiscrByVetoID;
  string _DoRecoElectron1DiscrByLooseID;
  string _DoRecoElectron1DiscrByMediumID;
  string _DoRecoElectron1DiscrByTightID;
  string _DoRecoElectron1DiscrByHEEPID;
  float _RecoElectron2EtaCut;
  float _RecoElectron2PtMinCut;
  float _RecoElectron2PtMaxCut;
  float _RecoElectron2IsoSumPtMinCutValue;
  float _RecoElectron2IsoSumPtMaxCutValue;
  string _DoRecoElectron2DiscrByIsolation;
  string _DoRecoElectron2DiscrByVetoID;
  string _DoRecoElectron2DiscrByLooseID;
  string _DoRecoElectron2DiscrByMediumID;
  string _DoRecoElectron2DiscrByTightID;
  string _DoRecoElectron2DiscrByHEEPID;

  float _RecoTau1PtMinCut;
  float _RecoTau1PtMaxCut;
  float _RecoTau1EtaCut;
  string _DoRecoTau1DiscrByLeadTrack;
  float _RecoTau1LeadTrackThreshold;
  string _DoRecoTau1DiscrByIsolation;
  string _RecoTau1DiscrByMaxIsolation;
  string _RecoTau1DiscrByMinIsolation;
  string _RecoTau1DiscrByProngType;
  string _DoRecoTau1DiscrAgainstElectron;
  string _RecoTau1DiscrAgainstElectron;
  string _SelectTau1sThatAreElectrons;
  string _DoRecoTau1DiscrAgainstMuon;
  string _RecoTau1DiscrAgainstMuon;
  string _SelectTau1sThatAreMuons;
  string _DoRecoTau1DiscrByCrackCut;
  string _RemoveTau1OverlapWithMuon1s;
  string _RemoveTau1OverlapWithMuon2s;
  double _Tau1Muon1MatchingDeltaR;
  double _Tau1Muon2MatchingDeltaR;
  string _RemoveTau1OverlapWithElectron1s;
  string _RemoveTau1OverlapWithElectron2s;
  double _Tau1Electron1MatchingDeltaR;
  double _Tau1Electron2MatchingDeltaR;
  float _RecoTau2PtMinCut;
  float _RecoTau2PtMaxCut;
  float _RecoTau2EtaCut;
  string _DoRecoTau2DiscrByLeadTrack;
  float _RecoTau2LeadTrackThreshold;
  string _DoRecoTau2DiscrByIsolation;
  string _RecoTau2DiscrByMaxIsolation;
  string _RecoTau2DiscrByMinIsolation;
  string _RecoTau2DiscrByProngType;
  string _DoRecoTau2DiscrAgainstElectron;
  string _RecoTau2DiscrAgainstElectron;
  string _SelectTau2sThatAreElectrons;
  string _DoRecoTau2DiscrAgainstMuon;
  string _RecoTau2DiscrAgainstMuon;
  string _SelectTau2sThatAreMuons;
  string _DoRecoTau2DiscrByCrackCut;
  string _RemoveTau2OverlapWithMuon1s;
  string _RemoveTau2OverlapWithMuon2s;
  double _Tau2Muon1MatchingDeltaR;
  double _Tau2Muon2MatchingDeltaR;
  string _RemoveTau2OverlapWithElectron1s;
  string _RemoveTau2OverlapWithElectron2s;
  double _Tau2Electron1MatchingDeltaR;
  double _Tau2Electron2MatchingDeltaR;
  float _RecoJet1EtaMaxCut;
  float _RecoJet1EtaMinCut;
  float _RecoJet1PtCut;
  string _ApplyJet1LooseID;
  string _RemoveJet1OverlapWithMuon1s;
  float _Jet1Muon1MatchingDeltaR;
  string _RemoveJet1OverlapWithMuon2s;
  float _Jet1Muon2MatchingDeltaR;
  string _RemoveJet1OverlapWithElectron1s;
  float _Jet1Electron1MatchingDeltaR;
  string _RemoveJet1OverlapWithElectron2s;
  float _Jet1Electron2MatchingDeltaR;
  string _RemoveJet1OverlapWithTau1s;
  float _Jet1Tau1MatchingDeltaR;
  string _RemoveJet1OverlapWithTau2s;
  float _Jet1Tau2MatchingDeltaR;
  float _RecoJet2EtaMaxCut;
  float _RecoJet2EtaMinCut;
  float _RecoJet2PtCut;
  string _ApplyJet2LooseID;
  string _RemoveJet2OverlapWithMuon1s;
  float _Jet2Muon1MatchingDeltaR;
  string _RemoveJet2OverlapWithMuon2s;
  float _Jet2Muon2MatchingDeltaR;
  string _RemoveJet2OverlapWithElectron1s;
  float _Jet2Electron1MatchingDeltaR;
  string _RemoveJet2OverlapWithElectron2s;
  float _Jet2Electron2MatchingDeltaR;
  string _RemoveJet2OverlapWithTau1s;
  float _Jet2Tau1MatchingDeltaR;
  string _RemoveJet2OverlapWithTau2s;
  float _Jet2Tau2MatchingDeltaR;
  float _RecoCentralJetPtCut;
  float _CentralJetMuon1MatchingDeltaR;
  float _CentralJetMuon2MatchingDeltaR;
  float _CentralJetElectron1MatchingDeltaR;
  float _CentralJetElectron2MatchingDeltaR;
  float _CentralJetTau1MatchingDeltaR;
  float _CentralJetTau2MatchingDeltaR;

  string _ApplyLeadingJetsLooseID;
  float _FirstLeadingJetMuon1MatchingDeltaR;
  float _FirstLeadingJetElectron1MatchingDeltaR;
  float _FirstLeadingJetTau1MatchingDeltaR;
  float _FirstLeadingJetMuon2MatchingDeltaR;
  float _FirstLeadingJetElectron2MatchingDeltaR;
  float _FirstLeadingJetTau2MatchingDeltaR;
  float _RecoFirstLeadingJetPt;
  float _RecoFirstLeadingJetEtaMinCut;
  float _RecoFirstLeadingJetEtaMaxCut;
  string _DoDiscrByFirstLeadingJet;
  string _RemoveFirstLeadingJetOverlapWithMuon1s;
  string _RemoveFirstLeadingJetOverlapWithMuon2s;
  string _RemoveFirstLeadingJetOverlapWithElectron1s;
  string _RemoveFirstLeadingJetOverlapWithElectron2s;
  string _RemoveFirstLeadingJetOverlapWithTau1s;
  string _RemoveFirstLeadingJetOverlapWithTau2s;
  float _SecondLeadingJetMuon1MatchingDeltaR;
  float _SecondLeadingJetElectron1MatchingDeltaR;
  float _SecondLeadingJetTau1MatchingDeltaR;
  float _SecondLeadingJetMuon2MatchingDeltaR;
  float _SecondLeadingJetElectron2MatchingDeltaR;
  float _SecondLeadingJetTau2MatchingDeltaR;
  float _RecoSecondLeadingJetPt;
  float _RecoSecondLeadingJetEtaMinCut;
  float _RecoSecondLeadingJetEtaMaxCut;
  string _DoDiscrBySecondLeadingJet;
  string _RemoveSecondLeadingJetOverlapWithMuon1s;
  string _RemoveSecondLeadingJetOverlapWithMuon2s;
  string _RemoveSecondLeadingJetOverlapWithElectron1s;
  string _RemoveSecondLeadingJetOverlapWithElectron2s;
  string _RemoveSecondLeadingJetOverlapWithTau1s;
  string _RemoveSecondLeadingJetOverlapWithTau2s;
  string _ApplyCentralJetLooseID;
  string _RemoveCentralJetOverlapWithMuon1s;
  string _RemoveCentralJetOverlapWithMuon2s;
  string _RemoveCentralJetOverlapWithElectron1s;
  string _RemoveCentralJetOverlapWithElectron2s;
  string _RemoveCentralJetOverlapWithTau1s;
  string _RemoveCentralJetOverlapWithTau2s;
  float _JetEnergyScaleOffset;
  float _RecoBJetEtaMaxCut;
  float _RecoBJetEtaMinCut;
  float _RecoBJetPtCut;
  float _BJetMuon1MatchingDeltaR;
  float _BJetMuon2MatchingDeltaR;
  float _BJetElectron1MatchingDeltaR;
  float _BJetElectron2MatchingDeltaR;
  float _BJetTau1MatchingDeltaR;
  float _BJetTau2MatchingDeltaR;
  string _RemoveBJetOverlapWithMuon1s;
  string _RemoveBJetOverlapWithMuon2s;
  string _RemoveBJetOverlapWithElectron1s;
  string _RemoveBJetOverlapWithElectron2s;
  string _RemoveBJetOverlapWithTau1s;
  string _RemoveBJetOverlapWithTau2s;
  string _ApplyJetBTagging;
  float _JetBTaggingCut;

  string _ApplyJetLooseIDforMhtAndHt;
  float _JetPtForMhtAndHt;
  float _JetEtaForMhtAndHt;
  string _DoSUSYDiscrByLeadDiJetMass;
  float _LeadDiJetMinMassCut;
  float _LeadDiJetMaxMassCut;
  string _DoSUSYDiscrByLeadDiJetPt;
  float _LeadDiJetMinPtCut;
  float _LeadDiJetMaxPtCut;
  string _DoSUSYDiscrByLeadDiJetDeltaEta;
  float _LeadDiJetMinDeltaEtaCut;
  float _LeadDiJetMaxDeltaEtaCut;
  string _DoSUSYDiscrByLeadDiJetDeltaPhi;
  float _LeadDiJetMinDeltaPhiCut;
  float _LeadDiJetMaxDeltaPhiCut;
  string _DoSUSYDiscrByLeadDiJetOSEta;
  string _DoDiscrByMet;
  float _RecoMetMinCut;
  float _RecoMetMaxCut;
  string _DoSUSYDiscrByMHT;
  float _MhtCut;
  string _DoSUSYDiscrByHT;
  float _HtCut;
  string _DoSUSYDiscrByR1;
  float _R1MinCut;
  float _R1MaxCut;
  string _DoSUSYDiscrByR2;
  float _R2MinCut;
  float _R2MaxCut;
  string _DoSUSYDiscrByAlpha;
  float _AlphaMinCut;
  float _AlphaMaxCut;
  string _DoSUSYDiscrByDphi1;
  float _Dphi1MinCut;
  float _Dphi1MaxCut;
  string _DoSUSYDiscrByDphi2;
  float _Dphi2MinCut;
  float _Dphi2MaxCut;

  string _DoMuon1DiscrByIsZllCut;
  string _DoDiscrByMuon1MetDphi;
  float _Muon1MetDphiMaxCut;
  float _Muon1MetDphiMinCut;
  string _DoDiscrByMuon1MetMt;
  float _Muon1MetMtMinCut;
  float _Muon1MetMtMaxCut;
  string _DoMuon2DiscrByIsZllCut;
  string _DoDiscrByMuon2MetDphi;
  float _Muon2MetDphiMaxCut;
  float _Muon2MetDphiMinCut;
  string _DoDiscrByMuon2MetMt;
  float _Muon2MetMtMinCut;
  float _Muon2MetMtMaxCut;

  string _DoElectron1DiscrByIsZllCut;
  string _DoDiscrByElectron1MetDphi;
  float _Electron1MetDphiMaxCut;
  float _Electron1MetDphiMinCut;
  string _DoDiscrByElectron1MetMt;
  float _Electron1MetMtMinCut;
  float _Electron1MetMtMaxCut;
  string _DoElectron2DiscrByIsZllCut;
  string _DoDiscrByElectron2MetDphi;
  float _Electron2MetDphiMaxCut;
  float _Electron2MetDphiMinCut;
  string _DoDiscrByElectron2MetMt;
  float _Electron2MetMtMinCut;
  float _Electron2MetMtMaxCut;

  string _DoDiscrByTau1MetDphi;
  float _Tau1MetDphiMaxCut;
  float _Tau1MetDphiMinCut;
  string _DoDiscrByTau1MetMt;
  float _Tau1MetMtMinCut;
  float _Tau1MetMtMaxCut;
  string _DoDiscrByTau2MetDphi;
  float _Tau2MetDphiMaxCut;
  float _Tau2MetDphiMinCut;
  string _DoDiscrByTau2MetMt;
  float _Tau2MetMtMinCut;
  float _Tau2MetMtMaxCut;

  string _DoDiTauDiscrByDeltaR;
  float _DiTauDeltaRCut;
  string _DiTauDiscrByOSLSType;
  string _DoDiTauDiscrByCosDphi;
  float _DiTauCosDphiMaxCut;
  float _DiTauCosDphiMinCut;
  string _DoDiscrByDiTauMassReco;
  string _UseVectorSumOfDiTauProductsAndMetMassReco;
  string _UseCollinerApproxDiTauMassReco;
  float _DiTauMassMinCut;
  float _DiTauMassMaxCut;
  string _DoDiTauDiscrByCDFzeta2D;
  float _DiTauPZetaCutCoefficient;
  float _DiTauPZetaVisCutCoefficient;
  float _DiTauCDFzeta2DMinCutValue;
  float _DiTauCDFzeta2DMaxCutValue;
  string _DoDiTauDiscrByDeltaPtDivSumPt;
  float _DiTauDeltaPtDivSumPtMinCutValue;
  float _DiTauDeltaPtDivSumPtMaxCutValue;
  string _DoDiTauDiscrByDeltaPt;
  float _DiTauDeltaPtMinCutValue;
  float _DiTauDeltaPtMaxCutValue;

  string _DoDiMuonDiscrByDeltaR;
  float _DiMuonDeltaRCut;
  string _DiMuonDiscrByOSLSType;
  string _DoDiMuonDiscrByCosDphi;
  float _DiMuonCosDphiMaxCut;
  float _DiMuonCosDphiMinCut;
  string _DoDiscrByDiMuonMassReco;
  string _UseVectorSumOfDiMuonProductsAndMetMassReco;
  string _UseCollinerApproxDiMuonMassReco;
  float _DiMuonMassMinCut;
  float _DiMuonMassMaxCut;
  string _DoDiMuonDiscrByCDFzeta2D;
  float _DiMuonPZetaCutCoefficient;
  float _DiMuonPZetaVisCutCoefficient;
  float _DiMuonCDFzeta2DMinCutValue;
  float _DiMuonCDFzeta2DMaxCutValue;
  string _DoDiMuonDiscrByDeltaPtDivSumPt;
  float _DiMuonDeltaPtDivSumPtMinCutValue;
  float _DiMuonDeltaPtDivSumPtMaxCutValue;
  string _DoDiMuonDiscrByDeltaPt;
  float _DiMuonDeltaPtMinCutValue;
  float _DiMuonDeltaPtMaxCutValue;

  string _DoDiElectronDiscrByDeltaR;
  float _DiElectronDeltaRCut;
  string _DiElectronDiscrByOSLSType;
  string _DoDiElectronDiscrByCosDphi;
  float _DiElectronCosDphiMaxCut;
  float _DiElectronCosDphiMinCut;
  string _DoDiscrByDiElectronMassReco;
  string _UseVectorSumOfDiElectronProductsAndMetMassReco;
  string _UseCollinerApproxDiElectronMassReco;
  float _DiElectronMassMinCut;
  float _DiElectronMassMaxCut;
  string _DoDiElectronDiscrByCDFzeta2D;
  float _DiElectronPZetaCutCoefficient;
  float _DiElectronPZetaVisCutCoefficient;
  float _DiElectronCDFzeta2DMinCutValue;
  float _DiElectronCDFzeta2DMaxCutValue;
  string _DoDiElectronDiscrByDeltaPtDivSumPt;
  float _DiElectronDeltaPtDivSumPtMinCutValue;
  float _DiElectronDeltaPtDivSumPtMaxCutValue;
  string _DoDiElectronDiscrByDeltaPt;
  float _DiElectronDeltaPtMinCutValue;
  float _DiElectronDeltaPtMaxCutValue;

  string _UseVectorSumOfVisProductsAndMetMassReco;
  string _UseCollinerApproxMassReco;

  string _DoDiJetDiscrByDeltaR;
  float _DiJetDeltaRCut;
  string _DoDiJetDiscrByDeltaEta;
  float _DiJetMinDeltaEtaCut;
  float _DiJetMaxDeltaEtaCut;
  string _DoDiJetDiscrByDeltaPhi;
  float _DiJetMinDeltaPhiCut;
  float _DiJetMaxDeltaPhiCut;
  string _DoDiJetDiscrByOSEta;
  string _DoDiJetDiscrByCosDphi;
  float _DiJetCosDphiMaxCut;
  float _DiJetCosDphiMinCut;
  string _DoDiscrByDiJetMassReco;
  float _DiJetMassMinCut;
  float _DiJetMassMaxCut;

  float _Muon1Tau1DeltaRCut;
  float _Muon1Tau1CosDphiMinCut;
  float _Muon1Tau1CosDphiMaxCut;
  float _Muon1Tau1MassMinCut;
  float _Muon1Tau1MassMaxCut;
  float _Muon1Tau1PZetaCutCoefficient;
  float _Muon1Tau1PZetaVisCutCoefficient;
  float _Muon1Tau1CDFzeta2DMinCutValue;
  float _Muon1Tau1CDFzeta2DMaxCutValue;
  float _Muon1Tau1DeltaPtDivSumPtMinCutValue;
  float _Muon1Tau1DeltaPtDivSumPtMaxCutValue;
  float _Muon1Tau1DeltaPtMinCutValue;
  float _Muon1Tau1DeltaPtMaxCutValue;
  string _DoMuon1Tau1DiscrByDeltaR;
  string _DoMuon1Tau1DiscrByCosDphi;
  string _DoDiscrByMuon1Tau1MassReco;
  string _DoMuon1Tau1DiscrByCDFzeta2D;
  string _DoMuon1Tau1DiscrByDeltaPtDivSumPt;
  string _DoMuon1Tau1DiscrByDeltaPt;
  string _UseVectorSumOfMuon1Tau1ProductsAndMetMassReco;
  string _UseCollinerApproxMuon1Tau1MassReco;
  string _Muon1Tau1DiscrByOSLSType;

  float _Muon1Tau2DeltaRCut;
  float _Muon1Tau2CosDphiMinCut;
  float _Muon1Tau2CosDphiMaxCut;
  float _Muon1Tau2MassMinCut;
  float _Muon1Tau2MassMaxCut;
  float _Muon1Tau2PZetaCutCoefficient;
  float _Muon1Tau2PZetaVisCutCoefficient;
  float _Muon1Tau2CDFzeta2DMinCutValue;
  float _Muon1Tau2CDFzeta2DMaxCutValue;
  float _Muon1Tau2DeltaPtDivSumPtMinCutValue;
  float _Muon1Tau2DeltaPtDivSumPtMaxCutValue;
  float _Muon1Tau2DeltaPtMinCutValue;
  float _Muon1Tau2DeltaPtMaxCutValue;
  string _DoMuon1Tau2DiscrByDeltaR;
  string _DoMuon1Tau2DiscrByCosDphi;
  string _DoDiscrByMuon1Tau2MassReco;
  string _DoMuon1Tau2DiscrByCDFzeta2D;
  string _DoMuon1Tau2DiscrByDeltaPtDivSumPt;
  string _DoMuon1Tau2DiscrByDeltaPt;
  string _UseVectorSumOfMuon1Tau2ProductsAndMetMassReco;
  string _UseCollinerApproxMuon1Tau2MassReco;
  string _Muon1Tau2DiscrByOSLSType;

  float _Muon2Tau1DeltaRCut;
  float _Muon2Tau1CosDphiMinCut;
  float _Muon2Tau1CosDphiMaxCut;
  float _Muon2Tau1MassMinCut;
  float _Muon2Tau1MassMaxCut;
  float _Muon2Tau1PZetaCutCoefficient;
  float _Muon2Tau1PZetaVisCutCoefficient;
  float _Muon2Tau1CDFzeta2DMinCutValue;
  float _Muon2Tau1CDFzeta2DMaxCutValue;
  float _Muon2Tau1DeltaPtDivSumPtMinCutValue;
  float _Muon2Tau1DeltaPtDivSumPtMaxCutValue;
  float _Muon2Tau1DeltaPtMinCutValue;
  float _Muon2Tau1DeltaPtMaxCutValue;
  string _DoMuon2Tau1DiscrByDeltaR;
  string _DoMuon2Tau1DiscrByCosDphi;
  string _DoDiscrByMuon2Tau1MassReco;
  string _DoMuon2Tau1DiscrByCDFzeta2D;
  string _DoMuon2Tau1DiscrByDeltaPtDivSumPt;
  string _DoMuon2Tau1DiscrByDeltaPt;
  string _UseVectorSumOfMuon2Tau1ProductsAndMetMassReco;
  string _UseCollinerApproxMuon2Tau1MassReco;
  string _Muon2Tau1DiscrByOSLSType;

  float _Muon2Tau2DeltaRCut;
  float _Muon2Tau2CosDphiMinCut;
  float _Muon2Tau2CosDphiMaxCut;
  float _Muon2Tau2MassMinCut;
  float _Muon2Tau2MassMaxCut;
  float _Muon2Tau2PZetaCutCoefficient;
  float _Muon2Tau2PZetaVisCutCoefficient;
  float _Muon2Tau2CDFzeta2DMinCutValue;
  float _Muon2Tau2CDFzeta2DMaxCutValue;
  float _Muon2Tau2DeltaPtDivSumPtMinCutValue;
  float _Muon2Tau2DeltaPtDivSumPtMaxCutValue;
  float _Muon2Tau2DeltaPtMinCutValue;
  float _Muon2Tau2DeltaPtMaxCutValue;
  string _DoMuon2Tau2DiscrByDeltaR;
  string _DoMuon2Tau2DiscrByCosDphi;
  string _DoDiscrByMuon2Tau2MassReco;
  string _DoMuon2Tau2DiscrByCDFzeta2D;
  string _DoMuon2Tau2DiscrByDeltaPtDivSumPt;
  string _DoMuon2Tau2DiscrByDeltaPt;
  string _UseVectorSumOfMuon2Tau2ProductsAndMetMassReco;
  string _UseCollinerApproxMuon2Tau2MassReco;
  string _Muon2Tau2DiscrByOSLSType;

  float _Electron1Tau1DeltaRCut;
  float _Electron1Tau1CosDphiMinCut;
  float _Electron1Tau1CosDphiMaxCut;
  float _Electron1Tau1MassMinCut;
  float _Electron1Tau1MassMaxCut;
  float _Electron1Tau1PZetaCutCoefficient;
  float _Electron1Tau1PZetaVisCutCoefficient;
  float _Electron1Tau1CDFzeta2DMinCutValue;
  float _Electron1Tau1CDFzeta2DMaxCutValue;
  float _Electron1Tau1DeltaPtDivSumPtMinCutValue;
  float _Electron1Tau1DeltaPtDivSumPtMaxCutValue;
  float _Electron1Tau1DeltaPtMinCutValue;
  float _Electron1Tau1DeltaPtMaxCutValue;
  string _DoElectron1Tau1DiscrByDeltaR;
  string _DoElectron1Tau1DiscrByCosDphi;
  string _DoDiscrByElectron1Tau1MassReco;
  string _DoElectron1Tau1DiscrByCDFzeta2D;
  string _DoElectron1Tau1DiscrByDeltaPtDivSumPt;
  string _DoElectron1Tau1DiscrByDeltaPt;
  string _UseVectorSumOfElectron1Tau1ProductsAndMetMassReco;
  string _UseCollinerApproxElectron1Tau1MassReco;
  string _Electron1Tau1DiscrByOSLSType;

  float _Electron1Tau2DeltaRCut;
  float _Electron1Tau2CosDphiMinCut;
  float _Electron1Tau2CosDphiMaxCut;
  float _Electron1Tau2MassMinCut;
  float _Electron1Tau2MassMaxCut;
  float _Electron1Tau2PZetaCutCoefficient;
  float _Electron1Tau2PZetaVisCutCoefficient;
  float _Electron1Tau2CDFzeta2DMinCutValue;
  float _Electron1Tau2CDFzeta2DMaxCutValue;
  float _Electron1Tau2DeltaPtDivSumPtMinCutValue;
  float _Electron1Tau2DeltaPtDivSumPtMaxCutValue;
  float _Electron1Tau2DeltaPtMinCutValue;
  float _Electron1Tau2DeltaPtMaxCutValue;
  string _DoElectron1Tau2DiscrByDeltaR;
  string _DoElectron1Tau2DiscrByCosDphi;
  string _DoDiscrByElectron1Tau2MassReco;
  string _DoElectron1Tau2DiscrByCDFzeta2D;
  string _DoElectron1Tau2DiscrByDeltaPtDivSumPt;
  string _DoElectron1Tau2DiscrByDeltaPt;
  string _UseVectorSumOfElectron1Tau2ProductsAndMetMassReco;
  string _UseCollinerApproxElectron1Tau2MassReco;
  string _Electron1Tau2DiscrByOSLSType;

  float _Electron2Tau1DeltaRCut;
  float _Electron2Tau1CosDphiMinCut;
  float _Electron2Tau1CosDphiMaxCut;
  float _Electron2Tau1MassMinCut;
  float _Electron2Tau1MassMaxCut;
  float _Electron2Tau1PZetaCutCoefficient;
  float _Electron2Tau1PZetaVisCutCoefficient;
  float _Electron2Tau1CDFzeta2DMinCutValue;
  float _Electron2Tau1CDFzeta2DMaxCutValue;
  float _Electron2Tau1DeltaPtDivSumPtMinCutValue;
  float _Electron2Tau1DeltaPtDivSumPtMaxCutValue;
  float _Electron2Tau1DeltaPtMinCutValue;
  float _Electron2Tau1DeltaPtMaxCutValue;
  string _DoElectron2Tau1DiscrByDeltaR;
  string _DoElectron2Tau1DiscrByCosDphi;
  string _DoDiscrByElectron2Tau1MassReco;
  string _DoElectron2Tau1DiscrByCDFzeta2D;
  string _DoElectron2Tau1DiscrByDeltaPtDivSumPt;
  string _DoElectron2Tau1DiscrByDeltaPt;
  string _UseVectorSumOfElectron2Tau1ProductsAndMetMassReco;
  string _UseCollinerApproxElectron2Tau1MassReco;
  string _Electron2Tau1DiscrByOSLSType;

  float _Electron2Tau2DeltaRCut;
  float _Electron2Tau2CosDphiMinCut;
  float _Electron2Tau2CosDphiMaxCut;
  float _Electron2Tau2MassMinCut;
  float _Electron2Tau2MassMaxCut;
  float _Electron2Tau2PZetaCutCoefficient;
  float _Electron2Tau2PZetaVisCutCoefficient;
  float _Electron2Tau2CDFzeta2DMinCutValue;
  float _Electron2Tau2CDFzeta2DMaxCutValue;
  float _Electron2Tau2DeltaPtDivSumPtMinCutValue;
  float _Electron2Tau2DeltaPtDivSumPtMaxCutValue;
  float _Electron2Tau2DeltaPtMinCutValue;
  float _Electron2Tau2DeltaPtMaxCutValue;
  string _DoElectron2Tau2DiscrByDeltaR;
  string _DoElectron2Tau2DiscrByCosDphi;
  string _DoDiscrByElectron2Tau2MassReco;
  string _DoElectron2Tau2DiscrByCDFzeta2D;
  string _DoElectron2Tau2DiscrByDeltaPtDivSumPt;
  string _DoElectron2Tau2DiscrByDeltaPt;
  string _UseVectorSumOfElectron2Tau2ProductsAndMetMassReco;
  string _UseCollinerApproxElectron2Tau2MassReco;
  string _Electron2Tau2DiscrByOSLSType;

  string _Trigger1FirstRequirement;
  string _Trigger1SecondRequirement;
  string _Trigger2FirstRequirement;
  string _Trigger2SecondRequirement;

  //-----For PDF weights
  vector<double> pdfWeightVector;

  // Define Branches
  void setBranchAddress(TTree* BOOM);
  vector<string>  *Trigger_names;
  vector<int>     *Trigger_decision;
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
  vector<double>  *Muon_charge;
  vector<double>  *patElectron_pt;
  vector<double>  *patElectron_eta;
  vector<double>  *patElectron_phi;
  vector<double>  *patElectron_energy;
  vector<int>     *patElectron_isPassVeto;
  vector<int>     *patElectron_isPassLoose;
  vector<int>     *patElectron_isPassMedium;
  vector<int>     *patElectron_isPassTight;
  vector<int>     *patElectron_isPassHEEPId;
  vector<double>  *patElectron_isoChargedHadrons;
  vector<double>  *patElectron_isoNeutralHadrons;
  vector<double>  *patElectron_isoPhotons;
  vector<double>  *patElectron_isoPU;
  vector<double>  *patElectron_charge;
  vector<double>  *Tau_eta;
  vector<double>  *Tau_phi;
  vector<double>  *Tau_pt;
  vector<double>  *Tau_energy;
  vector<double>  *Tau_charge;
  vector<int>     *Tau_decayModeFinding;
  vector<int>     *Tau_decayModeFindingNewDMs;
  vector<double>  *Tau_chargedIsoPtSum;
  vector<double>  *Tau_neutralIsoPtSum;
  vector<int>     *Tau_againstMuonTight3;
  vector<int>     *Tau_againstElectronMVATightMVA5;
  vector<double>  *Tau_nProngs;
  vector<double>  *Tau_puCorrPtSum;
  vector<int>     *Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits;
  vector<int>     *Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits;
  vector<int>     *Tau_byTightCombinedIsolationDeltaBetaCorr3Hits;
  vector<int>     *Tau_byLooseIsolationMVA3newDMwLT;
  //vector<int>     *Tau_byLooseIsolationMVA3newDMwoLT;
  vector<int>     *Tau_byLooseIsolationMva3oldDMwLT;
  //vector<int>     *Tau_byLooseIsolationMVA3oldDMwoLT;
  vector<int>     *Tau_byMediumIsolationMVA3newDMwLT;
  //vector<int>     *Tau_byMediumIsolationMVA3newDMwoLT;
  vector<int>     *Tau_byMediumIsolationMva3oldDMwLT;
  //vector<int>     *Tau_byMediumIsolationMVA3oldDMwoLT;
  vector<int>     *Tau_byTightIsolationMVA3newDMwLT;
  //vector<int>     *Tau_byTightIsolationMVA3newDMwoLT;
  vector<int>     *Tau_byTightIsolationMva3oldDMwLT;
  //vector<int>     *Tau_byTightIsolationMVA3oldDMwoLT;
  //vector<int>     *Tau_againstMuonLoose2;
  vector<int>     *Tau_againstMuonLoose3;
  //vector<int>     *Tau_againstMuonTight2;
  vector<int>     *Tau_againstElectronMVALooseMVA5;
  vector<int>     *Tau_againstElectronMVAMediumMVA5;
  vector<double>  *Tau_leadChargedCandPt;
  vector<double>  *Tau_leadChargedCandCharge;
  vector<double>  *Tau_leadChargedCandEta;
  vector<double>  *Tau_leadChargedCandPhi;
  vector<double>  *Jet_pt;
  vector<double>  *Jet_eta;
  vector<double>  *Jet_phi;
  vector<double>  *Jet_energy;
  vector<double>  *Jet_neutralHadEnergyFraction;
  vector<double>  *Jet_neutralEmEmEnergyFraction;
  vector<double>  *Jet_muonEnergyFraction;
  vector<double>  *Jet_chargedHadronEnergyFraction;
  vector<double>  *Jet_chargedEmEnergyFraction;
  vector<double>  *Jet_bDiscriminator;
  vector<int>     *Jet_numberOfConstituents;
  vector<int>     *Jet_chargedMultiplicity;
  vector<int>     *Jet_partonFlavour;
  vector<double>  *Gen_pt;
  vector<double>  *Gen_eta;
  vector<double>  *Gen_phi;
  vector<double>  *Gen_energy;
  vector<double>  *Gen_pdg_id;
  vector<double>  *Gen_motherpdg_id;
  vector<double>  *Gen_status;
  vector<int>  *Gen_BmotherIndex;
  Float_t         nTruePUInteractions;
  Int_t           bestVertices;
  Double_t        Met_type1PF_px;
  Double_t        Met_type1PF_py;
  Double_t        Met_type1PF_pz;

  // List of branches
  TBranch        *b_Trigger_decision;   //!
  TBranch        *b_Trigger_names;   //!
  TBranch        *b_Gen_pt;   //!
  TBranch        *b_Gen_eta;   //!
  TBranch        *b_Gen_phi;   //!
  TBranch        *b_Gen_energy;   //!
  TBranch        *b_Gen_pdg_id;   //!
  TBranch        *b_Gen_motherpdg_id;   //!
  TBranch        *b_Gen_status;   //!
  TBranch        *b_Gen_BmotherIndex;   //!
  TBranch        *b_nTruePUInteractions;   //!
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
  TBranch        *b_Muon_charge;   //!
  TBranch        *b_patElectron_pt;   //!
  TBranch        *b_patElectron_eta;   //!
  TBranch        *b_patElectron_phi;   //!
  TBranch        *b_patElectron_energy;   //!
  TBranch        *b_patElectron_isPassVeto;   //!
  TBranch        *b_patElectron_isPassLoose;   //!
  TBranch        *b_patElectron_isPassMedium;   //!
  TBranch        *b_patElectron_isPassTight;   //!
  TBranch        *b_patElectron_isPassHEEPId;   //!
  TBranch        *b_patElectron_isoChargedHadrons;   //!
  TBranch        *b_patElectron_isoNeutralHadrons;   //!
  TBranch        *b_patElectron_isoPhotons;   //!
  TBranch        *b_patElectron_isoPU;   //!
  TBranch        *b_patElectron_charge;   //!
  TBranch        *b_Tau_eta;   //!
  TBranch        *b_Tau_phi;   //!
  TBranch        *b_Tau_pt;   //!
  TBranch        *b_Tau_energy;   //!
  TBranch        *b_Tau_charge;   //!
  TBranch        *b_Tau_decayModeFinding;   //!
  TBranch        *b_Tau_decayModeFindingNewDMs;   //!
  TBranch        *b_Tau_chargedIsoPtSum;   //!
  TBranch        *b_Tau_neutralIsoPtSum;   //!
  TBranch        *b_Tau_againstMuonTight3;   //!
  TBranch        *b_Tau_againstElectronMVATightMVA5;   //!
  TBranch        *b_Tau_nProngs;   //!
  TBranch        *b_Tau_puCorrPtSum;   //!
  TBranch        *b_Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits;   //!
  TBranch        *b_Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits;   //!
  TBranch        *b_Tau_byTightCombinedIsolationDeltaBetaCorr3Hits;   //!
  TBranch        *b_Tau_byLooseIsolationMVA3newDMwLT;   //!
  //TBranch        *b_Tau_byLooseIsolationMVA3newDMwoLT;   //!
  TBranch        *b_Tau_byLooseIsolationMva3oldDMwLT;   //!
  //TBranch        *b_Tau_byLooseIsolationMVA3oldDMwoLT;   //!
  TBranch        *b_Tau_byMediumIsolationMVA3newDMwLT;   //!
  //TBranch        *b_Tau_byMediumIsolationMVA3newDMwoLT;   //!
  TBranch        *b_Tau_byMediumIsolationMva3oldDMwLT;   //!
  //TBranch        *b_Tau_byMediumIsolationMVA3oldDMwoLT;   //!
  TBranch        *b_Tau_byTightIsolationMVA3newDMwLT;   //!
  //TBranch        *b_Tau_byTightIsolationMVA3newDMwoLT;   //!
  TBranch        *b_Tau_byTightIsolationMva3oldDMwLT;   //!
  //TBranch        *b_Tau_byTightIsolationMVA3oldDMwoLT;   //!
  //TBranch        *b_Tau_againstMuonLoose2;   //!
  TBranch        *b_Tau_againstMuonLoose3;   //!
  //TBranch        *b_Tau_againstMuonTight2;   //!
  TBranch        *b_Tau_againstElectronMVALooseMVA5;   //!
  TBranch        *b_Tau_againstElectronMVAMediumMVA5;   //!
  TBranch        *b_Tau_leadChargedCandPt;   //!
  TBranch        *b_Tau_leadChargedCandCharge;   //!
  TBranch        *b_Tau_leadChargedCandEta;   //!
  TBranch        *b_Tau_leadChargedCandPhi;   //!
  TBranch        *b_Jet_pt;   //!
  TBranch        *b_Jet_eta;   //!
  TBranch        *b_Jet_phi;   //!
  TBranch        *b_Jet_energy;   //!
  TBranch        *b_Jet_neutralHadEnergyFraction;   //!
  TBranch        *b_Jet_neutralEmEmEnergyFraction;   //!
  TBranch        *b_Jet_numberOfConstituents;   //!
  TBranch        *b_Jet_muonEnergyFraction;   //!
  TBranch        *b_Jet_chargedHadronEnergyFraction;   //!
  TBranch        *b_Jet_chargedMultiplicity;   //!
  TBranch        *b_Jet_chargedEmEnergyFraction;   //!
  TBranch        *b_Jet_partonFlavour;   //!
  TBranch        *b_Jet_bDiscriminator;   //!
  TBranch        *b_Met_type1PF_px;   //!
  TBranch        *b_Met_type1PF_py;   //!
  TBranch        *b_Met_type1PF_pz;   //!

};
#endif
