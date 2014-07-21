#ifndef chorizo_H
#define chorizo_H

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
//#include <EventLoopAlgs/AlgSelect.h>


// Root includes
#include <TH2.h>
#include <TH1F.h>
#include <TTree.h>
#include <TSystem.h>
#include <TAxis.h>
#include <TString.h>
#include <TLorentzVector.h>
#include <TError.h>
#include <TRandom1.h>
#include <TRandom.h>

// code includes
#include "SusyAnalysis/XMLReader.h"
#include "SusyAnalysis/SmartVeto.h"
#include "SusyAnalysis/PDFTool.h"
#include "SusyAnalysis/utility.h"
#include "SusyAnalysis/particles.h"

// std includes
#include <iostream>
#include <stdio.h>
#include <string>

// Tools includes
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/DataPeriod.h"
#include "SUSYTools/ScaleVariatioReweighter.hpp"
#include "SUSYTools/SUSYCrossSection.h"

#include "JVFUncertaintyTool/JVFUncertaintyTool.h"
//#include "MissingETUtility/METUtility.h"
#include "PileupReweighting/TPileupReweighting.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#include "GoodRunsLists/TGRLCollection.h"
#include "SUSYTools/BTagCalib.h"

#include "fastjet/ClusterSequence.hh"

/* #include "METSmearing/MissingETSmearing.h" */
/* #include "TruthToRecoFunctions/ElectronParam.h" */
/* #include "TruthToRecoFunctions/JetParam.h" */
/* #include "TruthToRecoFunctions/TauParam.h" */
/* #include "TruthToRecoFunctions/METParam.h" */
/* #include <TruthToRecoFunctions/MCP_resol.h> */
/* #include "TruthToRecoFunctions/btag_performance.h" */


// Fwd declares
/* class xAODJet; */
/* class xAODElectron; */
/* class xAODMuon; */
/* class xAODTruth; */

/* #include "xAODMuon/MuonContainer.h" */
/* #include "xAODJet/JetContainer.h" */
/* #include "xAODEgamma/ElectronContainer.h" */
/* #include "xAODEgamma/PhotonContainer.h" */
/* #include "xAODTau/TauJetContainer.h" */
//#include "xAODCaloEvent/CaloCluster.h"
/* #include "xAODTruth/TruthParticleContainer.h" */
/* #include "xAODTruth/TruthEventContainer.h" */
/* #include "xAODTruth/TruthEvent.h" */
/* #include "xAODTruth/TruthParticle.h" */
/* #include "xAODMissingET/MissingET.h" */
/* #include "xAODMissingET/MissingETContainer.h" */

/* namespace xAOD{ */
/*   /\* class JetContainer; *\/ */
/*   /\* class ElectronContainer; *\/ */
/*   /\* class MuonContainer; *\/ */
/*   class TruthEventContainer; */
/*   class TruthParticleContainer; */
/*   class TruthParticle; */
/* } */


/* namespace METSmear{ */
/*   class MissingETSmearing; */
/* } */

namespace ST{
  class SUSYObjDef_xAOD;
}

using namespace Particles;
using namespace xAOD;
using namespace std;
using namespace fastjet;

namespace pileupErr
{
  typedef enum  {
    NONE,
    PileupHigh,
    PileupLow
  } pileupSyste;
}

namespace JvfUncErr
{
  typedef enum  {
    NONE,
    JvfUncHigh,
    JvfUncLow
  } JvfSyste;
}

namespace BCHCorrMediumErr
{
  typedef enum  {
    NONE,
    BCHCorrHigh,
    BCHCorrLow
  } BCHSyste;
}

typedef std::pair<std::vector<float>, std::vector<float> > VFloatPair;     
typedef std::vector<std::vector<float> > VVFloat;                          


class JetCleaningTool;


class chorizo : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  EL::NTupleSvc *output; //!

  //algo options
  std::string outputName;
  std::string xmlPath;
  std::string Region;
  std::string defaultRegion;

  bool isQCD;
  bool isSignal;
  bool isTop;
  bool isTruth;
  bool isAtlfast;
  bool isNCBG;
  TString leptonType;

  bool doAnaTree;
  bool doPUTree; 
  bool doFlowTree; 
  bool genPUfile;

  SystErr::Syste syst_ST;
  ScaleVariatioReweighter::variation syst_Scale;
  pileupErr::pileupSyste syst_PU;
  JvfUncErr::JvfSyste syst_JVF;
  BCHCorrMediumErr::BCHSyste syst_BCH;

  bool printMet;
  bool printJet;
  bool printElectron;
  bool printMuon;

  // this is a standard constructor
  chorizo ();

  // Own methods
  virtual void SetTruthSmearing(bool doit=false){ doTTR=doit; }; //to overwrite xml option.

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(chorizo, 1);

private:
  xAOD::TEvent *m_event;  //!

  //--- Containers  //IT can't be done in ROOT5!
  /* const xAOD::JetContainer* jets; */
  /* const xAOD::ElectronContainer* electrons; */
  /* const xAOD::MuonContainer* muons; */
  /* const xAOD::TruthEventContainer* truthE; */
  /* const xAOD::TruthParticleContainer* truthP; */

  //--- Tools
  XMLReader*     xmlJobOption; //!
#ifndef __CINT__
  ST::SUSYObjDef_xAOD* tool_st; //!
#endif // not __CINT__
  

  DataPeriod     tool_DPeriod; //!
  //METUtility*    tool_metutil; //NOT USED YET
  FakeMetEstimator* tool_fakeMet; //!
  BTagCalib*     tool_calib1; //!
  BTagCalib*     tool_calib2; //!
  Root::TPileupReweighting* tool_purw; //!
  Root::TGoodRunsListReader grl_reader; //!
  Root::TGRLCollection      grlist; //!
  JVFUncertaintyTool* tool_jvf; //!
  PDFTool*       tool_pdf; //!
  JetCleaningTool *tool_jClean; //!  

  //Member Functions
  virtual void InitVars();
  virtual void ReadXML();

  virtual void bookTree();

  virtual void dumpLeptons();
  virtual void dumpJets();
  virtual void fillRazor();

  virtual bool vetoMCevent();
  virtual bool passMCor();

  virtual float GetTruthEtmiss(bool noEleTau=false);
  virtual float GetTruthEtmiss_noEleTau(){ return GetTruthEtmiss(true); };
  virtual std::vector<float> GetTruthBosonPt(float &_M, float &_MT, bool &_Muon, float &_pt1, float &_pt2);
  virtual float GetTTbarReweight(float &Top_truth_pt, float &Topbar_truth_pt, float &avTop_truth_pt);
  virtual int GetNTruthB();
  virtual int GetNTruthHF();
  virtual int GetNTruthLeptons();

  virtual void GetTruthShat(long int sigSamPdgId);

  virtual double GetGeneratorUncertaintiesSherpa();

  virtual float GetAverageWeight();

  VVFloat GetBTagCalibSyst(BTagCalib* calibTool);

  //Calculation functions
  virtual float Calc_MT(Particle p, TVector2 met);
  virtual float Calc_MT(Particles::Jet j, TVector2 met);
  virtual float Calc_mct();
  virtual float Calc_dijetMass();
  virtual float Calc_dijetMass(TLorentzVector ja, TLorentzVector jb);
  virtual std::vector<TLorentzVector> CombineJets();
  virtual std::vector<TLorentzVector> CombineJets(std::vector<TLorentzVector>);
  virtual float Calc_MR(TLorentzVector ja, TLorentzVector jb);
  virtual float Calc_MTR(TLorentzVector ja, TLorentzVector jb, TVector3 met);
  virtual float Calc_AlphaT(TLorentzVector ja, TLorentzVector jb);
  virtual void  Calc_SuperRazor(TLorentzVector ja, TLorentzVector jb, TVector3 met, float& shatR ,float& gaminvR, float& mdeltaR, float& cosptR);
  virtual float TopTransvMass();
  virtual void  RecoHadTops(int ibtop1, int ibtop2);
  virtual std::vector<TLorentzVector> getFatJets(double R);
  virtual void  findBparton();

  //Aux functions
  virtual float getAsymmetry(float a, float b);

  virtual bool  isLepton(int pid){ return (fabs(pid)==11 || fabs(pid)==13 || fabs(pid)==15); };
  virtual bool  isNeutrino(int pid){ return (fabs(pid)==12 || fabs(pid)==14 || fabs(pid)==16); };
  virtual bool  isLepNeut(int pid){ return (isLepton(pid) || isNeutrino(pid)); };
  virtual bool  isHF(int pid){ return (fabs(pid)==4 || fabs(pid)==5); };
  virtual bool  isStableP(int status){ return (status==1); };
  virtual bool  isHardP(int status){ return (status==3); };

  /* virtual bool  isLepton(xAOD::TruthParticle* p); */
  /* virtual bool  isNeutrino(xAOD::TruthParticle* p); */
  /* virtual bool  isLepNeut(xAOD::TruthParticle* p); */
  /* virtual bool  isHF(xAOD::TruthParticle* p); */
  /* virtual bool  isStableP(xAOD::TruthParticle* p); */
  /* virtual bool  isHardP(xAOD::TruthParticle* p); */

  /* virtual void  fillTLV( TLorentzVector &v, xAOD::TruthParticle* p, bool inGeV=false ); */

  virtual bool  isSignal_WIMP(int mc_id); //move to RunsMap.h or so? //CHECK_ME
  virtual bool  isSignal_ADD(int mc_id); //move to RunsMap.h or so? //CHECK_ME

  virtual TString WccType();

  virtual float BtagEta(float eta); //saturates eta to 2.5 (btag calib defined region)

  //--- Variable definition                                                             
  bool isMC; //!  

  int m_eventCounter; //!

  bool isGRL; //! //event cleaning
  bool isFakeMet; //!
  bool isBadID; //!
  bool isMetCleaned; //!
  bool isTrigger; //!
  bool isVertexOk; //!
  bool isLarGood; //!
  bool isTileGood; //!
  bool isTileTrip; //!
  bool isCoreFlag; //!
  
  bool passPreselectionCuts; //!

  //----- Jet smearing config (QCD)
  float   QCD_JetsPtPreselection; //!
  float   QCD_JetsPtSelection; //! //taken from the jets section
  float   QCD_JetsEtaSelection; //!//taken from the jets section
  float   QCD_METSig; //!
  TString QCD_LeadJetPreSel; //!
  int     QCD_RandomSeedOffset; //!
  TString QCD_SmearType; //!
  bool    QCD_SmearUseBweight; //!
  double  QCD_SmearBtagWeight; //!
  TString QCD_SmearMeanShift; //!
  bool    QCD_SmearExtraSmr; //!
  bool    QCD_DoPhiSmearing; //!
  int     QCD_SmearedEvents; //!

  //----- PDF Reweighting
  bool    doPDFrw; //!
  float   beamE_from; //!
  float   beamE_to; //!

  //----- Truth-->Reco smearing
  bool doTTR; //!  // do truth to reco smearing ? 
  int ttr_mu; //!
  short ttr_eleWP; //!
  unsigned int ttr_metWP; //!
  bool ttr_recjetflav; //!

  //--- 
  TFile *f_average; //!
  TH1D  *h_average; //!
  bool  warningPileup; //!

  //------ Define global variables for XML options
  //selection
  std::string DirectoryPath; //! 

  TString GRLxmlFile; //!
  bool    applyPURW;
  TString PURW_Folder; //!
  TString PURW_IlumicalcFile; //!
  TString PeriodConfig; //!
  bool    leptonEfficiencyUnitarity; //!

  std::vector<std::string> TriggerNames; //!

  //track veto
  bool  tVeto_Enable; //! 
  int   nTracks; //! 
  float tVeto_Pt; //!
  float tVeto_Eta; //!
  float tVeto_d0; //!
  float tVeto_z0; //!
  float tVeto_ndof; //!
  float tVeto_chi2OverNdof_min; //!
  float tVeto_chi2OverNdof_max; //!
  float PixHitsAndSCTHits; //!
  float tVeto_TrackIso; //!

  //electrons
  float El_DefinPtCut; //! 
  float El_DefinEtaCut; //!
  float El_PreselPtCut; //!
  float El_PreselEtaCut; //!
  float El_RecoPtCut; //!
  float El_RecoEtaCut; //!
  bool El_recoSF; //!
  bool El_idSF; //!
  bool El_triggerSF; //!
  TString El_isolationVar; //!
  float El_isolationThres; //!
  //muons
  float Mu_DefinPtCut; //! 
  float Mu_DefinEtaCut; //!
  float Mu_PreselPtCut; //!
  float Mu_PreselEtaCut; //!
  float Mu_RecoPtCut; //!
  float Mu_RecoEtaCut; //!
  TString Mu_isolationVar; //!
  float Mu_isolationThres; //!
  //jets
  TString JetCollection; //!
  TString Jet_BtagEnv; //! 
  TString Jet_BtagCalib; //!
  float Jet_DefinPtCut; //!
  float Jet_DefinEtaCut; //!
  float Jet_PreselPtCut; //!
  float Jet_PreselEtaCut; //!
  float Jet_RecoPtCut; //!
  float Jet_RecoEtaCut; //!
  float Jet_ORElPt; //!
  float Jet_ORMuPt; //!
  TString Jet_Tagger;
  TString Jet_TaggerOp;
  TString Jet_TaggerOp2;  
  TString Jet_Tagger_Collection;
  //met
  TString METCollection; //!
  TString Met_FakeMetEstimator; //! 
  bool Met_doFakeEtmiss; //!
  bool Met_doMetCleaning; //!
  //met recalculation
  bool Met_doRefEle; //!
  bool Met_doRefGamma; //!
  bool Met_doRefTau; //!
  bool Met_doRefJet; //!
  bool Met_doRefMuon; //!
  bool Met_doMuonTotal; //!
  bool Met_doSoftTerms; //!

  //btag weights & systematics
  std::vector<float> btag_weight_first;
  std::vector<float> btag_weight_first_80eff;
  std::vector<float> btag_weight;
  std::vector<float> btag_weight_80eff;

  std::vector<float> btag_weight_B_down; //!
  std::vector<float> btag_weight_B_down_80eff; //!
  std::vector<float> btag_weight_B_up; //!
  std::vector<float> btag_weight_B_up_80eff; //!
  std::vector<float> btag_weight_C_down; //!
  std::vector<float> btag_weight_C_down_80eff; //!
  std::vector<float> btag_weight_C_up; //!
  std::vector<float> btag_weight_C_up_80eff; //!
  std::vector<float> btag_weight_L_down; //!
  std::vector<float> btag_weight_L_down_80eff; //!
  std::vector<float> btag_weight_L_up; //!
  std::vector<float> btag_weight_L_up_80eff; //!

  //RecoToTruth smearing                                                           
  bool recalc_jetflav; //!
  //  TRandom1* myRand; //!

  /* ElectronParam m_ElectronParam; */
  /* //PhotonParam m_PhotonParam; */
  /* JetParam m_JetParam; */
  /* TauParam m_TauParam; */
  /* METParam m_METParam; */

  //smeared jets kin (for QCD estimation)
  std::vector<float> smr_met_jets_pt; //!
  std::vector<float> smr_met_jets_eta; //!
  std::vector<float> smr_met_jets_phi; //!
  std::vector<float> smr_met_jets_E; //!

  //muons (before overlap removal)
  float         muon_N;  //!
  vector<float> muon_pt; //!
  vector<float> muon_eta; //!
  vector<float> muon_phi; //!
  vector<float> muon_iso; //!
  vector<float> muon_etiso30; //!
  vector<float> muon_ptiso30; //!
  vector<bool>  muon_truth; //!
  vector<float> muon_jet_dR; //! //matched jet to muon 
  vector<float> muon_jet_dPhi; //!
  vector<float> muon_jet_pt; //!
  vector<float> muon_jet_eta; //!
  vector<float> muon_jet_phi; //!
  vector<float> muon_jet_nTrk; //!
  vector<float> muon_jet_sumPtTrk; //!
  vector<float> muon_jet_chf; //!
  vector<float> muon_jet_emf; //!
  vector<float> muon_jet_mv1; //!
  vector<float> muon_jet_vtxf; //!

  //Particle collections
  std::vector<Particle> recoElectrons; //!
  std::vector<Particle> truthElectrons; //!
  std::vector<Particle> recoMuons; //!
  std::vector<Particles::Jet> recoJets; //!
  std::vector<Particles::Jet> seedJets; //!
  Particles::MET met_obj;
  std::vector<TLorentzVector> RecoUnmatchedTracksElMu; //!
  std::vector<int> RecoUnmatchedTracksIdx; //!

  //Track veto stuff
  int   m_isolatedUnmatchedTracks; //!
  int   m_isolatedUnmatchedTracksPlusJets; //!

  float dR_min_track_truthBoson; //!
  float dPhi_min_track_truthBoson; //!
  float dR_min_track_truthCjet; //!
  float dPhi_min_track_truthCjet; //!
  float dR_jet_track_min_event; //!
  
  std::vector<float> isolatedTrack_Pt; //!
  std::vector<float> isolatedTrack_Eta; //!
  std::vector<float> isolatedTrack_trkIso; //!
  std::vector<int>   isolatedTrack_MatchedLepton; //!
  std::vector<int>   isolatedTrack_MatchedJet; //!

  std::vector<int>   trackUnmatched_truthObject; //!
  std::vector<float> trackUnmatched_truthObjectPt; //!
  std::vector<float> trackUnmatched_truthObjectEta; //!


  //truth sbottom decay info 
  float t_b_pt1, t_b_eta1, t_b_phi1;  //!       //Decide if we want to save all this b-parton info! //CHECK_ME
  float t_b_pt2, t_b_eta2, t_b_phi2;  //!
  float t_sb_pt1, t_sb_eta1, t_sb_phi1;  //!
  float t_sb_pt2, t_sb_eta2, t_sb_phi2;  //!
  float t_neutr_pt1, t_neutr_eta1, t_neutr_phi1;  //!
  float t_neutr_pt2, t_neutr_eta2, t_neutr_phi2;  //!

  float dPhi_j1_bp1, dPhi_j1_bp2;  //!
  float dPhi_j2_bp1, dPhi_j2_bp2;  //!
  float dPhi_j3_bp1, dPhi_j3_bp2;  //!
  float dR_j1_bp1, dR_j1_bp2;  //!
  float dR_j2_bp1, dR_j2_bp2;  //!
  float dR_j3_bp1, dR_j3_bp2;  //!


  //Histograms
  TH1F *h_cut_var; //!

  // histogram helper functions
  TH1F* InitHist(std::string name, int nbin, float binlow, float binhigh, std::string xaxis="", std::string yaxis="");
  void InitGHist(TH1F* h, std::string name, int nbin, float binlow, float binhigh, std::string xaxis="", std::string yaxis="");

  void Fill(TH1 *h, float value, float weight=1.);

  //Ntuple branches
  //--- Declaration of leaf types                                                    
  //- Event Info
  UInt_t  RunNumber;        
  UInt_t  EventNumber;
  UInt_t  mc_channel_number;
  float   averageIntPerXing;

  //- Weights  
  float   w;
  float   w_average;
  float   MC_w;
  float   PDF_w;
  float   pileup_w;
  float   bosonVect_w;
  float   Trigger_w;
  float   Trigger_w_avg;
  float   e_SF;
  float   m_SF;

  //- ttbar reweighting
  float ttbar_weight;
  float Top_truth_pt;   
  float Topbar_truth_pt;   
  float avTop_truth_pt;  

  //- Boson info
  float   bos_pt; 
  float   bos_m;
  int     bos_pdgId;
  float   bosonVec_reco_pt;
  float   bosonVec_reco_eta;
  float   bosonVec_reco_phi;
  float   bosonVec_truth_pt;
  float   bosonVec_truth_eta;
  float   bosonVec_truth_phi;

  //- truth kin info
  float truth_MT;
  float truth_M;
  float truth_Lep1_pt;
  float truth_Lep2_pt;
  bool  truth_m_fiducial;
  float truth_met;
  float truth_met_noEle;
  
  int truth_n_bjets;
  int truth_n_HFjets;
  int truth_n_leptons;  

  //- Production kin
  //only for signal samples
  float sigSam_pt1;
  float sigSam_eta1;
  float sigSam_phi1;
  float sigSam_E1;
  float sigSam_pt2;
  float sigSam_eta2;
  float sigSam_phi2;
  float sigSam_E2;

  float truth_shat;


  //- Event selection
  int nVertex;

  //- Electron Info
  int   e_N;
  float e_pt;
  float e_truth_pt;
  float e_truth_eta;
  float e_truth_phi;
  float e_eta;
  float e_phi;
  float e_ptiso30;
  float e_etiso30;
  bool  e_tight; 
  float electronSF;
  float electronSFu;
  float electronSFd;

  //- Muon Info
  int m_N;
  float m_pt;
  float m_eta;
  float m_phi;
  float m2_pt;
  float m2_eta;
  float m2_phi;
  float m_iso;
  float m_ptiso30;
  float m_etiso30;
  float m2_iso;
  float m2_ptiso30;
  float m2_etiso30;
  float muonSF;
  float muonSFu;
  float muonSFd;
  
  //- 'boson' properties
  float e_M;
  float e_MT;
  float e_Zpt;
  float m_M;
  float m_MT;
  float m_Zpt;
  float m_EM;

  //- Jet info
  bool  JVF_min;
  int   n_jets;
  int   n_taujets;

  float pt1;
  float pt2;
  float pt3;
  float pt4;  
  float pt5;
  float pt6;
  float eta1;
  float eta2;
  float eta3;
  float eta4;  
  float eta5;
  float eta6;
  float phi1;
  float phi2;
  float phi3;
  float phi4;  
  float phi5;
  float phi6;
  float j1_chf;
  float j2_chf;
  float j3_chf;
  float j4_chf;  
  float j1_emf;
  float j2_emf;
  float j3_emf;
  float j4_emf;  
  float j1_fsm;
  float j2_fsm;
  float j3_fsm;
  float j4_fsm;  
  float j1_time;
  float j2_time;
  float j3_time;
  float j4_time;  
  float j1_nTrk;
  float j2_nTrk;
  float j3_nTrk;
  float j4_nTrk;  
  float j1_sumPtTrk;
  float j2_sumPtTrk;
  float j3_sumPtTrk;
  float j4_sumPtTrk;  
  float j1_jvtxf;
  float j2_jvtxf;
  float j3_jvtxf;
  float j4_jvtxf;  
  float j1_tflavor;
  float j2_tflavor;
  float j3_tflavor;
  float j4_tflavor;
 
  //- Btagging
  int   n_bjets;
  int   n_bjets_80eff;
  float btag_weight1; 
  float btag_weight2; 
  float btag_weight3; 
  float btag_weight4; 
  float btag_weight_80eff1; 
  float btag_weight_80eff2; 
  float btag_weight_80eff3; 
  float btag_weight_80eff4; 
  float btag_weight_total;
  float btag_weight_total_80eff;

  float tag_MV1_1, tag_MV1_2, tag_MV1_3, tag_MV1_4;
  float tag_JetFitterCu_1, tag_JetFitterCu_2, tag_JetFitterCu_3, tag_JetFitterCu_4;
  float tag_JetFitterCb_1, tag_JetFitterCb_2, tag_JetFitterCb_3, tag_JetFitterCb_4;

  //- BCH cleaning
  bool JetsFailBCHTight; 
  bool JetsFailBCHMedium; 
  bool Jet1FailBCHTight; 
  bool Jet1FailBCHMedium; 
  bool Jet2FailBCHTight; 
  bool Jet2FailBCHMedium; 
  bool Jet3FailBCHTight; 
  bool Jet3FailBCHMedium; 
  float Jet1_BCH_CORR_CELL; 
  float Jet2_BCH_CORR_CELL; 
  float Jet3_BCH_CORR_CELL; 
  
  //- MET
  float met;
  float met_phi;

  float met_trk;
  float mettrk_phi;

  float met_mu;
  float met_ecorr;

  float met_lochadtopo;
  float met_reffinalNoMu;


  //- Topologic variables
  float dPhi_met_j1;
  float dPhi_met_j2;
  float dPhi_met_j3;
  float dPhi_met_j4;
  float dPhi_mettrk_j1;
  float dPhi_mettrk_j2;
  float dPhi_mettrk_j3;
  float dPhi_mettrk_j4;    
  float dPhi_met_mettrk;
  float dPhi_j1_j2;
  float dPhi_j1_j3;
  float dPhi_j2_j3;
  float dPhi_b1_b2;
  float dPhi_min;
  float dPhi_min_alljets;
  float dR_j1_j2;
  float dR_j1_j3;
  float dR_j2_j3;
  float dR_j1_m1;
  float dR_j1_m2;
  float dR_j2_m1;
  float dR_j2_m2;
  float dR_j3_m1;
  float dR_j3_m2;
  float dEta_j1_j2;

  float j1_mT;
  float j2_mT;
  float j3_mT;
  float j4_mT;  

  float M12;

  float MT_min_jet_met;  
  float MT_bcl_met;  
  float MT_bfar_met;  
  float MT_lcl_met;  
  float MT_jsoft_met;  

  float DiJet_Mass;  
  float DiBJet_Mass;    
  
  float mct;   
  float meff;
  float HT;   

  float AlphaT;

  //Razor
  float MR;  
  float MTR;
  float R;
  
  //new super-razor (Martin)
  float shatR;  
  float gaminvR;
  float mdeltaR;
  float cosptR;

  //top reconstruction
  float MtTop;
  float m_top_had1;
  float m_top_had2;

  //fat jets
  float m0_antikt12;
  float m1_antikt12;
  float m0_antikt08;
  float m1_antikt08;
  float pt0_antikt12;
  float pt1_antikt12;
  float pt0_antikt08;
  float pt1_antikt08;
  float mtasym12; //build these offline ?
  float mtasym08;
};

#endif
