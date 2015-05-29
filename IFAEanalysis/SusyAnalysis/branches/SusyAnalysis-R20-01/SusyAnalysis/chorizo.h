#ifndef chorizo_H
#define chorizo_H

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <EventLoop/StatusCode.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
//#include <EventLoopAlgs/AlgSelect.h>
#include "SampleHandler/MetaFields.h"
#include "ElectronIsolationSelection/IsolationSelectionTool.h"

// Root includes
#include <TH1.h>
#include <TH2.h>
#include <TNamed.h>
#include <TTree.h>
#include <TSystem.h>
#include <TAxis.h>
#include <TString.h>
#include <TLorentzVector.h>
#include <TError.h>
#include <TRandom1.h>
#include <TRandom.h>
#include <TTreeFormula.h>
#include "TStopwatch.h"

// code includes
#include "SusyAnalysis/XMLReader.h"
#include "SusyAnalysis/utility.h"
#include "SusyAnalysis/particles.h"
#include "SusyAnalysis/Systematics.h"
//#include "SusyAnalysis/SUSYSignal.h"

// std includes
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

// Tools includes
#include "SusyAnalysis/ScaleVariatioReweighter.hpp"
#include "SUSYTools/SUSYCrossSection.h"

#include "SusyAnalysis/TMctLib.h"
#include "SusyAnalysis/mctlib.h"

#include "JVFUncertaintyTool/JVFUncertaintyTool.h"
#include "fastjet/ClusterSequence.hh"
#include "TileTripReader/TTileTripReader.h"


//For trigger test
#define TILETEST
#ifdef TILETEST
#include "JetTileCorrection/JetTileCorrectionTool.h"
#endif 

// Systematics includes
//#include "PATInterfaces/SystematicList.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

#include "EventPrimitives/EventPrimitivesHelpers.h"

#ifndef __MAKECINT__
#include "AssociationUtils/OverlapRemovalTool.h"

#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#endif // not __MAKECINT__

//TeV unit (w.r.t MeV)
#ifndef TEV
#define TEV 1000000.
#endif

//GeV unit (w.r.t MeV)
#ifndef GEV
#define GEV 1000.
#endif 

#define m_warnLimit 5

//fwd declares
namespace LHAPDF{
  class PDF;
}

namespace TrigConf{
  class xAODConfigTool;
}

namespace Trig{
  class TrigDecisionTool;
  class TrigEgammaMatchingTool;
  class TrigMuonMatching;
}

namespace Analysis{
  class JetQuarkLabel;
}

class GoodRunsListSelectionTool;

class BTaggingEfficiencyTool;

namespace CP{
  class PileupReweightingTool;
}

namespace ST{
  struct IsSignalElectronExpCutArgs;
  struct IsSignalMuonExpCutArgs;
  class SUSYObjDef_xAOD;
}

using namespace Particles;
using namespace xAOD;
using namespace std;
//using namespace fastjet;  //conflicts with TError function
using fastjet::PseudoJet;
using fastjet::ClusterSequence;
using fastjet::JetDefinition;
using fastjet::antikt_algorithm;

struct CutflowInfo{
  uint64_t nEvents;
  double   weightSum;
  double   weight2Sum;
};

typedef std::vector<int> VInt;                          
typedef std::vector<float> VFloat;                          
typedef std::pair<VFloat, VFloat > VFloatPair;     
typedef std::vector<VFloat > VVFloat;                          

typedef pair<UInt_t, UInt_t> Pint;
typedef set<Pint> EvtList;

class JetCleaningTool;

namespace SUSY{
  class JetMCSmearingTool;
}

enum ZDecayMode{
  None = -1,
  Unknown,
  ee,
  mumu,
  hadronic,
  invisible
};

//MET flavours
const string sMetDef[] = {"InvMu", "VisMu", "InvMuECorr", "VisMuECorr", "VisMuMuCorr", "InvMuPhCorr", "VisMuPhCorr", "Track", "InvMuRef", "VisMuRef", "InvMuTST", "VisMuTST", "InvMuTSTECorr", "VisMuTSTECorr", "VisMuTSTMuCorr", "InvMuTruth", "VisMuTruth", "locHadTopo"};
enum class MetDef {InvMu, VisMu, InvMuECorr, VisMuECorr, VisMuMuCorr, InvMuPhCorr, VisMuPhCorr, Track, InvMuRef, VisMuRef, InvMuTST, VisMuTST, InvMuTSTECorr, VisMuTSTECorr, VisMuTSTMuCorr, InvMuTruth, VisMuTruth, locHadTopo, N};

class chorizo : public EL::Algorithm
{

public:
  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  //Ntuple
  EL::NTupleSvc *output; //!

  //SUSYmap
  //  SUSYmap* m_susymap; //!

  //algo options
  std::string outputName;
  std::string jOption;
  std::string Region;
  std::string defaultRegion;

  bool isQCD;
  bool isSignal;
  bool isTop;
  bool isAtlfast;
  bool isNCBG;
  bool is8TeV;
  TString leptonType;

  bool debug;
  TString eventsFile;

  bool isTruth;
  bool dressLeptons;

  bool doAnaTree;
  bool doPUTree; 
  bool doFlowTree; 
  bool genPUfile;
  bool doTrigExt;

  CP::SystematicSet syst_CP; //!
  TString syst_CPstr;
  SystErr::Syste syst_ST;    
  ScaleVariatioReweighter::variation syst_Scale;
  pileupErr::pileupSyste syst_PU;
  JvfUncErr::JvfSyste syst_JVF;
  int  syst_JESNPset;

  bool printMet;
  bool printJet;
  bool printElectron;
  bool printMuon;

  bool systListOnly;
  int  errIgnoreLevel;

  // this is a standard constructor
  chorizo ();

  // Own methods
  virtual void SetTruthSmearing(bool doit=false){ doTTR=doit; }; //to overwrite xml option.
  virtual EL::StatusCode doTrackVeto(std::vector<Particle> electronCandidates, std::vector<Particle> muonCandidates);

  virtual void printSystList(); //systematics list

  virtual void loadMetaData(); 

  virtual EL::StatusCode nextEvent();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode loop ();
  virtual EL::StatusCode loop_truth ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(chorizo, 1);

private:
  xAOD::TEvent *m_event;  //!

  TString m_cfilename; //! 
  TTree*  m_MetaData; //!

  EvtList m_eventList; //!

  //Histograms
  TDirectory* out_TDir; //! 
  //raw 
  TH1F* h_presel_flow; //! 
  //weighted
  TH1F* h_presel_wflow; //!
  
  TH1F* h_cut_var; //!

  //--- Tools
  XMLReader*     xmlReader; //!
#ifndef __CINT__
  ST::SUSYObjDef_xAOD* tool_st; //!

  //MET map
  std::map<MetDef, TVector2> metmap; //!
#endif // not __CINT__
  std::string smetmap="";

  Analysis::JetQuarkLabel* tool_jetlabel; //!

  //  DataPeriod     tool_DPeriod; //!
  JVFUncertaintyTool* tool_jvf; //!
  JetCleaningTool* tool_jClean; //!  
  Root::TTileTripReader* tool_tileTrip; //!

  Trig::TrigDecisionTool* tool_trigdec; //! 
  TrigConf::xAODConfigTool* tool_trigconfig; //!

  Trig::TrigEgammaMatchingTool* tool_trig_match_el; //!
  Trig::TrigMuonMatching* tool_trig_match_mu; //!
   
#ifndef __CINT__  
  OverlapRemovalTool* tool_or; //!

  CP::PileupReweightingTool *tool_purw; //! 

  GoodRunsListSelectionTool *tool_grl; //!

  LHAPDF::PDF* m_PDF; //!

  BTaggingEfficiencyTool* tool_btag;  //! //70%op
  BTaggingEfficiencyTool* tool_btag2; //! //80%op

  CP::IsolationSelectionTool *iso_2; //!
  CP::IsolationSelectionTool *iso_1; //! 
  CP::IsolationSelectionTool *iso_3; //!
#endif // not __CINT__

  TMctLib* tool_mct; //!

  SUSY::JetMCSmearingTool* tool_jsmear; //!

  //Testing new TileCal correction tool!
#ifdef TILETEST
  CP::JetTileCorrectionTool* tool_jettile; //!
#endif 


  //Member Functions
  virtual void InitVars();
  virtual void ReadXML();

  virtual CutflowInfo getNinfo();
  virtual bool  isDerived();
  virtual void  loadEventList();
  virtual bool  inEventList(UInt_t run, UInt_t event); 

  virtual void bookTree();

  virtual void dumpLeptons();
  virtual void dumpPhotons();
  virtual void dumpJets();
  virtual void fillRazor();
  virtual void fillRazor(TVector3 mymet);

  virtual void Zll_candidate();
  virtual void Zll_extra(TVector2 met);
  
  virtual bool vetoMCevent();
  virtual bool passMCor();

  virtual float GetTruthEtmiss(bool noEleTau=false);
  virtual float GetTruthEtmiss_noEleTau(){ return GetTruthEtmiss(true); };
  virtual VFloat GetTruthBosonPt(float &_M, float &_MT, bool &_Muon, float &_pt1, float &_pt2);
  virtual float GetTTbarReweight(float &Top_truth_pt, float &Topbar_truth_pt, float &avTop_truth_pt);
  virtual int GetNTruthB();
  virtual int GetNTruthHF();
  virtual int GetNTruthLeptons();

  virtual void GetTruthShat(long int sigSamPdgId);

  std::vector<int> GetTruthZDecay(TLorentzVector &ztlv);

  virtual double GetGeneratorUncertaintiesSherpa();

  UInt_t  puRunNumber( UInt_t rn); //obsolete! (new PURW tool now)
  virtual float GetAverageWeight();

  //pdf reweighting
  double  getPdfRW( LHAPDF::PDF* pdfFrom, LHAPDF::PDF* pdfTo, double rwScale=1., double pdf_scale2=0., double pdf_x1=0., double pdf_x2=0., int pdf_id1=0, int pdf_id2=0 );
  double  getPdfRW( LHAPDF::PDF* pdfTo, double rwScale=1., double pdf_scale2=0., double pdf_x1=0., double pdf_x2=0., int pdf_id1=0, int pdf_id2=0 );
  double  getPdfRW( double rwScale=1., double pdf_scale2=0., double pdf_x1=0., double pdf_x2=0., int pdf_id1=0, int pdf_id2=0 );

  //trigger matching
  virtual bool hasTrigMatch(const xAOD::Electron& el, std::string item, double dR=0.07); 
  virtual bool hasTrigMatch(const xAOD::Muon& mu, std::string item, double dR=0.07); 

  //Calculation functions
  virtual float Calc_MT(Particle p, TVector2 met);
  virtual float Calc_mct();
  virtual float Calc_mct(Particle p1, Particle p2);
  virtual float Calc_mct_corr(Particle p1, Particle p2, TVector2 met);  
  virtual float Calc_Mjj();
  virtual float Calc_Mjj(TLorentzVector ja, TLorentzVector jb);
  virtual std::vector<TLorentzVector> CombineJets();
  virtual std::vector<TLorentzVector> CombineJets(std::vector<TLorentzVector>);
  virtual float Calc_MR(TLorentzVector ja, TLorentzVector jb);
  virtual float Calc_MTR(TLorentzVector ja, TLorentzVector jb, TVector3 met);
  virtual float Calc_AlphaT(TLorentzVector ja, TLorentzVector jb);
  virtual void  Calc_SuperRazor(TLorentzVector ja, TLorentzVector jb, TVector3 met, float& shatR ,float& gaminvR, float& mdeltaR, float& cosptR);

  virtual float Calc_Sphericity(std::vector<TLorentzVector> pvectors,
				bool IsTransverseSphericity=false);
  
  virtual double epsilon(double x);
  virtual double thrustService(TVector2 &n, std::vector<TVector2> &obj);
  virtual double Calc_Thrust(std::vector<TLorentzVector> pvectors);

  virtual float TopTransvMass();
  virtual void  RecoHadTops(int ibtop1, int ibtop2);
  virtual std::vector<TLorentzVector> getFatJets(double R, double fcut=-1);

  virtual void  findBparton(); 

  //for JetSmearing
  virtual bool  isQCDSeedEvent(float, float, float);

#ifndef __MAKECINT__
  TVector2 getMET( const xAOD::MissingETContainer* METcon, TString name );

  float GetBtagSF(xAOD::JetContainer* goodJets, BTaggingEfficiencyTool* btagTool);
#endif // not __MAKECINT__


  template <class xAODPart>
  void  fillTLV( TLorentzVector &v, xAODPart* p, bool inGeV=false ); 
  
  template <class xAODPart>
  TLorentzVector getTLV( xAODPart* p, bool inGeV=false ); 

  virtual TString WccType();

  //--- Variable definition                                                             
  bool isMC; //!  

  bool m_isderived; //!
  int  m_eventCounter; //!
  int  m_metwarnCounter; //!
  int  m_pdfwarnCounter; //!

  bool isGRL; //! //event cleaning
  bool isFakeMet; //!
  bool isBadID; //!
  bool isMetCleaned; //!
  std::vector<int> isTrigger; //!
  bool isVertexOk; //!
  bool isLarGood; //!
  bool isTileGood; //!
  bool isTileTrip; //!
  bool isCoreFlag; //!
  bool isCosmic; //!
  bool isBadMuon; //!
  
  bool passPreselectionCuts; //!

  TNamed *meta_jOption; //!
  TNamed *meta_triggers; //!
  TNamed *meta_el_triggers; //!
  TNamed *meta_mu_triggers; //!
  TNamed *meta_ph_triggers; //!
  TNamed *meta_metmap; //!

  //----- Jet smearing config (QCD)
  TString QCD_btagFileMap; //!
  TString QCD_bvetoFileMap; //!
  TString QCD_btagMap; //!
  TString QCD_bvetoMap; //!
  float   QCD_JetsPtPreselection; //!
  float   QCD_JetsEtaPreselection; //!
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
  unsigned int QCD_SmearedEvents; //!

  //----- PDF Reweighting
  //  LHAPDF::PDF*     m_PDF; //!
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

  bool doCutFlow; //! 
  bool isStopTL; //!  
  bool m_skim; //! 
  
  TString GRLxmlFile; //!
  bool    applyPURW;
  TString PURW_Folder; //!
  TString PURW_IlumicalcFile; //!
  bool    leptonEfficiencyUnitarity; //!

  std::vector<std::string> TriggerNames; //!
  std::vector<std::string> ElTriggers; //!
  std::vector<std::string> MuTriggers; //!
  std::vector<std::string> PhTriggers; //!
  // for QCD jet smearing 
  std::vector<std::string> JS_triggers; //!

  //OverlapRemoval
  bool  doOR; //! 
  bool  m_or_useSigLep; //!
  bool  m_or_useIsoLep; //!
  bool  m_or_bjetOR; //!
  bool  doORphotons; //!

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
  float El_PreselPtCut; //!
  float El_PreselEtaCut; //!
  float El_RecoPtCut; //!
  float El_RecoEtaCut; //!
  string El_baseID; //!
  string El_ID; //!
  string El_isoWP; //!
  bool El_recoSF; //!
  bool El_idSF; //!
  bool El_triggerSF; //!  

  //muons
  float Mu_PreselPtCut; //!
  float Mu_PreselEtaCut; //!
  float Mu_RecoPtCut; //!
  float Mu_RecoEtaCut; //!
  string Mu_ID; //!
  string Mu_isoWP; //!

  //photons
  float Ph_PreselPtCut; //!
  float Ph_PreselEtaCut; //!
  float Ph_RecoPtCut; //!
  float Ph_RecoEtaCut; //!
  string Ph_ID; //!
  string Ph_isoWP; //!
  bool Ph_recoSF; //!
  bool Ph_idSF; //!
  bool Ph_triggerSF; //!  

  //Booking options
  int BookElBase;
  int BookElSignal;
  int BookMuBase;
  int BookMuSignal;
  int BookPhSignal;
  int BookJetSignal;

#ifndef __CINT__
  ST::IsSignalElectronExpCutArgs* elIsoArgs; //!
  ST::IsSignalMuonExpCutArgs* muIsoArgs; //!
#endif // not __CINT__

  //jets
  TString JetCollection; //!
  TString Jet_BtagEnv; //! 
  TString Jet_BtagCalib; //!
  float Jet_PreselPtCut; //!
  float Jet_PreselEtaCut; //!
  float Jet_RecoPtCut; //!
  float Jet_RecoEtaCut; //!
  /* float Jet_ORElPt; //! */
  /* float Jet_ORMuPt; //! */
  bool Jet_DoOR;
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
  bool Met_doMuons; //!
  bool Met_doSoftTerms; //!


  //Particle collections
  std::vector<Particle> electronCandidates; //!                                                                                                                                                                                                                              
  std::vector<Particle> muonCandidates; //!                                                                                                                                                                                                                             

  std::vector<Particle> recoElectrons; //!
  std::vector<Particle> recoPhotons; //!
  std::vector<Particle> recoMuons; //!
  std::vector<Particles::Jet> recoJets; //!

  std::vector<Particle> truthElectrons; //!

  std::vector<Particles::Jet> seedJets; //!

  Particles::MET met_obj; //!

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
  
  VFloat isolatedTrack_Pt; //!
  VFloat isolatedTrack_Eta; //!
  VFloat isolatedTrack_trkIso; //!
  vector<int>   isolatedTrack_MatchedLepton; //!
  vector<int>   isolatedTrack_MatchedJet; //!

  vector<int>   trackUnmatched_truthObject; //!
  VFloat trackUnmatched_truthObjectPt; //!
  VFloat trackUnmatched_truthObjectEta; //!


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

  
  // histogram helper functions
  TH1F* InitHist(std::string name, int nbin, float binlow, float binhigh, std::string xaxis="", std::string yaxis="");
  void InitGHist(TH1F* h, std::string name, int nbin, float binlow, float binhigh, std::string xaxis="", std::string yaxis="");

  void Fill(TH1 *h, float value, float weight=1.);

  TStopwatch watch; 

  //MetaData
  float meta_xsec;
  float meta_xsec_relunc;
  float meta_kfactor;
  float meta_feff;
  float meta_nsim;
  float meta_nwsim;
  float meta_lumi;

  //Ntuple branches
  //--- Declaration of leaf types                                                    
  //- Event Info
  UInt_t  RunNumber;        
  UInt_t  EventNumber;
  UInt_t  lb;
  UInt_t  bcid;
  UInt_t  procID;
  UInt_t  mc_channel_number;//!
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
  float   ph_SF;
  float   e_SFu;
  float   m_SFu;
  float   ph_SFu;
  float   e_SFd;
  float   m_SFd;
  float   ph_SFd;

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
  float   bosonVec_reco_pt_vmu;
  float   bosonVec_reco_phi_vmu;  
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
  float truth_shat_pt;

  //- Event selection
  int nVertex;

  //- Photon Info
  int   ph_N;
  VFloat ph_pt;
  VFloat ph_eta;
  VFloat ph_phi;
  VFloat ph_ptiso30;
  VFloat ph_etiso30;
  VInt   ph_tight; 
  VInt   ph_type; 
  VInt   ph_origin; 

  //- Electron Info
  int    e_N;
  VFloat e_pt;
  VFloat e_eta;
  VFloat e_phi;
  VFloat e_type;    
  VFloat e_origin;
  VFloat e_ptiso30;
  VFloat e_etiso30;
  VFloat e_ptiso20;
  VFloat e_etiso20;
  VFloat e_isoTight;
  VFloat e_isoGradient;
  VFloat e_isoLoose;
  VInt   e_id; 
  VFloat e_d0_sig; 
  VFloat e_z0;    

  int    eb_N;
  VFloat eb_pt;
  VFloat eb_eta;
  VFloat eb_phi;

  float  e_truth_pt;
  float  e_truth_eta;
  float  e_truth_phi;

  VInt   e_trigger; 

  //- Muon Info
  int    m_N;
  VFloat m_pt;
  VFloat m_eta;
  VFloat m_phi;
  VFloat m_type;
  VFloat m_origin;
  VFloat m_ptiso20;
  VFloat m_etiso20;
  VFloat m_ptiso30;
  VFloat m_etiso30;
  VFloat m_isoTight;
  VFloat m_isoGradient;
  VFloat m_isoLoose;

  int    mb_N;
  VFloat mb_pt;
  VFloat mb_eta;
  VFloat mb_phi;

  VInt   m_trigger; 

  //- 'boson' properties
  float e_M;
  float e_MT;
  float e_MT_vmu;  
  float e_MT_tst;
  float e_MT_tst_vmu;  
  float e_Zpt;

  float m_M;
  float m_MT;
  float m_MT_vmu;  
  float m_MT_tst;
  float m_MT_tst_vmu;  
  float m_Zpt;
  float m_EM;

  //- Z candidate                                          
                                                                                                        
  //truth
  int     Z_decay; //0=unknown, 1=ee, 2=mumu, 3=qq, 4=nunu, -99=no-Z-found
  float   Z_pt; 
  float   Z_eta;
  float   Z_phi;
  //leplep
  int Z_flav;                                                                                         
  int Z_lep1;
  int Z_lep2;
  float Z_m;
  std::vector<float> lep3_MT;
  float lep_mct;

  //- Jet info
  int   j_N;
  int   j_N30;
  int   j_N40;
  int   j_N50;
  int   j_N60;
  int   j_N80;

  std::vector<int> j_tau_N;

  bool  JVF_min;

  int   jb_truth_N; //baseline truth jets
  int   j_truth_N;  //signal truth jets
  float j_truth_pt1;
  float j_truth_eta1;

  VFloat j_pt;
  VFloat j_eta;
  VFloat j_phi;
  VFloat j_m;
  VFloat j_chf;
  VFloat j_emf;
  VFloat j_fsm;
  VFloat j_time;
  VFloat j_nTrk;
  VFloat j_sumPtTrk;
  VFloat j_jvtxf;
  VFloat j_tflavor;
  VFloat j_tag_MV1;
  VFloat j_tag_MV2c20;

  //- Btagging
  int   bj_N;
  int   bj_Ne80;
  float btag_weight_total;
  float btag_weight_total_80eff;

  
  //- MET
  VFloat met; 
  VFloat met_phi; 

  float  met_lochadtopo;

  //recoiling system
  VFloat rmet_par;  //parallel
  VFloat rmet_norm;  //perpendicular
  VFloat rmet_par_mod;  //parallel modided by module
  VFloat rmet_norm_mod;  //perpendicular  modided by module
  VFloat rmet_dPhi_met_jetsys; //dphi(met, jet_system) 

  //(transverse) sphericity
  std::vector<float>  tr_spher;
  std::vector<double> tr_thrust; 

  //- Topologic variables
  VFloat dPhi_met_j1;
  VFloat dPhi_met_j2;
  VFloat dPhi_met_j3;
  VFloat dPhi_met_j4;
  float dPhi_met_mettrk;
  float dPhi_j1_j2;
  float dPhi_j1_j3;
  float dPhi_j2_j3;
  float dPhi_b1_b2;
  VFloat dPhi_min;
  VFloat dPhi_min_4jets;
  VFloat dPhi_min_alljets;
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

  VFloat j1_mT;
  VFloat j2_mT;
  VFloat j3_mT;
  VFloat j4_mT;  

  float M12;
  
  float Melb_min;
  float Mmub_min; 
  
  float j1_E;
  float j2_E;  
  float j3_E;   
  float j4_E;     
  float j5_E;     
  float j6_E;     

  VFloat MT_min_jet_met;  
  VFloat MT_bcl_met;  
  VFloat MT_bfar_met;  
  VFloat MT_lcl_met;  
  VFloat MT_jsoft_met;  

  float mjj;  
  float mbb;    
  
  float  mct;   
  VFloat mct_corr;   
  VFloat meff;
  float  HT;   


  //Razor
  float  MR;  
  VFloat MTR;
  VFloat R;

  float  AlphaT;
  
  //new super-razor (Martin)
  VFloat shatR;  
  VFloat gaminvR;
  VFloat mdeltaR;
  VFloat cosptR;

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

  //Extended trigger info
  float trig_l1_ex; 
  float trig_l1_ey; 
  float trig_l1_et; 
  float trig_l1_sumet; 
  float trig_l1_phi;

  float trig_hlt_EFJetEtSum_ex; 
  float trig_hlt_EFJetEtSum_ey; 
  float trig_hlt_EFJetEtSum_et; 
  float trig_hlt_EFJetEtSum_sumet; 
  float trig_hlt_EFJetEtSum_phi;
 
  float trig_hlt_T2MissingET_ex; 
  float trig_hlt_T2MissingET_ey; 
  float trig_hlt_T2MissingET_et; 
  float trig_hlt_T2MissingET_sumet; 
  float trig_hlt_T2MissingET_phi;

  float trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex; 
  float trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey; 
  float trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et; 
  float trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet; 
  float trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi;

 
  float trig_hlt_TrigL2MissingET_FEB_ex; 
  float trig_hlt_TrigL2MissingET_FEB_ey; 
  float trig_hlt_TrigL2MissingET_FEB_et; 
  float trig_hlt_TrigL2MissingET_FEB_sumet; 
  float trig_hlt_TrigL2MissingET_FEB_phi;
 
  float trig_hlt_TrigEFMissingET_FEB_ex; 
  float trig_hlt_TrigEFMissingET_FEB_ey; 
  float trig_hlt_TrigEFMissingET_FEB_et; 
  float trig_hlt_TrigEFMissingET_FEB_sumet; 
  float trig_hlt_TrigEFMissingET_FEB_phi;

  float trig_hlt_TrigEFMissingET_ex; 
  float trig_hlt_TrigEFMissingET_ey; 
  float trig_hlt_TrigEFMissingET_et; 
  float trig_hlt_TrigEFMissingET_sumet; 
  float trig_hlt_TrigEFMissingET_phi;
 
  float trig_hlt_TrigEFMissingET_mht_ex; 
  float trig_hlt_TrigEFMissingET_mht_ey; 
  float trig_hlt_TrigEFMissingET_mht_et; 
  float trig_hlt_TrigEFMissingET_mht_sumet; 
  float trig_hlt_TrigEFMissingET_mht_phi;
 
  float trig_hlt_TrigEFMissingET_topocl_ex; 
  float trig_hlt_TrigEFMissingET_topocl_ey; 
  float trig_hlt_TrigEFMissingET_topocl_et; 
  float trig_hlt_TrigEFMissingET_topocl_sumet; 
  float trig_hlt_TrigEFMissingET_topocl_phi;

  float trig_hlt_TrigEFMissingET_topocl_PS_ex; 
  float trig_hlt_TrigEFMissingET_topocl_PS_ey; 
  float trig_hlt_TrigEFMissingET_topocl_PS_et; 
  float trig_hlt_TrigEFMissingET_topocl_PS_sumet; 
  float trig_hlt_TrigEFMissingET_topocl_PS_phi;

  float trig_hlt_TrigEFMissingET_topocl_PUC_ex; 
  float trig_hlt_TrigEFMissingET_topocl_PUC_ey; 
  float trig_hlt_TrigEFMissingET_topocl_PUC_et; 
  float trig_hlt_TrigEFMissingET_topocl_PUC_sumet; 
  float trig_hlt_TrigEFMissingET_topocl_PUC_phi;           
  
};

#endif
