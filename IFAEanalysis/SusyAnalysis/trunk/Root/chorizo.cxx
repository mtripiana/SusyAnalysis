#define  chorizo_cxx
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"
#include "SampleHandler/MetaFields.h"
#include <SusyAnalysis/chorizo.h>
#include <SusyAnalysis/chorizoEDM.h>
#include <SusyAnalysis/MCUtils.h>
#include <iostream>
#include <fstream>
#include <EventLoop/StatusCode.h>

#include "NewMT2/MT2.h"

//Jet Cleaning
#include "JetSelectorTools/JetCleaningTool.h"

//Jet Smearing
#include "JetSmearing/JetMCSmearingTool.h"

//GRL
#include "GoodRunsLists/GoodRunsListSelectionTool.h"

//PDF Reweighting
#include "LHAPDF/LHAPDF.h"

//Pileup Reweighting
#include "PileupReweighting/PileupReweightingTool.h"

//CutBookkeeping
#include "xAODCutFlow/CutBookkeeper.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"

//BTagging SF
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

//TeV unit (w.r.t MeV)
#ifndef TEV
#define TEV 1000000.
#endif

//Dummy values for variables initialization
#define DUMMYUP  99999
#define DUMMYDN -99999

//For code profiling tests
//#define PROFCODE
#ifdef PROFCODE
#include <valgrind/callgrind.h>
#endif


//For branch-use stats
//#define CODE_STATS
#ifdef CODE_STATS
#include "xAODCore/tools/IOStats.h"
#include "xAODCore/tools/ReadStats.h"
#endif

//For trigger test
#define TRIGGERTEST
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigEgammaMatchingTool/TrigEgammaMatchingTool.h"
#include "TrigMuonMatching/TrigMuonMatching.h"

// this is needed to distribute the algorithm to the workers
ClassImp(chorizo)

//for errorcheck
#define APP_NAME "chorizo"


//decorators and accessors
static SG::AuxElement::Decorator<char> dec_baseline("baseline");
static SG::AuxElement::Decorator<char> dec_signal("signal");
static SG::AuxElement::Decorator<char> dec_isol("isol");
static SG::AuxElement::Decorator<char> dec_passOR("passOR");
static SG::AuxElement::Decorator<char> dec_failOR("overlaps");
static SG::AuxElement::Decorator<char> dec_badjet("bad");
static SG::AuxElement::Decorator<char> dec_bjet("bjet");
static SG::AuxElement::Decorator<char> dec_cosmic("cosmic");
static SG::AuxElement::Decorator<char> dec_final("final");

static SG::AuxElement::Accessor<float>  acc_jvt("Jvt");
static SG::AuxElement::ConstAccessor<float> cacc_jvt("Jvt");

static SG::AuxElement::Accessor<float> acc_ptcone20("ptcone20");
static SG::AuxElement::Accessor<float> acc_ptcone30("ptcone30");
static SG::AuxElement::Accessor<float> acc_etcone20("topoetcone20");
static SG::AuxElement::Accessor<float> acc_etcone30("topoetcone30");

static SG::AuxElement::Accessor<float> acc_truth_etcone20("EtCone20");
static SG::AuxElement::Accessor<float> acc_truth_ptcone30("PtCone30");
static SG::AuxElement::Accessor<float> acc_pt_dressed("pt_dressed");
static SG::AuxElement::Accessor<float> acc_eta_dressed("eta_dressed");
static SG::AuxElement::Accessor<float> acc_phi_dressed("phi_dressed");
static SG::AuxElement::Accessor<float> acc_e_dressed("e_dressed");

static SG::AuxElement::Accessor<unsigned char> acc_nPixHits("numberOfPixelHits");
static SG::AuxElement::Accessor<unsigned char> acc_nSCTHits("numberOfSCTHits");

static SG::AuxElement::Accessor<double> acc_PUweight("PileupWeight");

chorizo :: chorizo ()
  : m_MetaData(0), 
    debug(false),
    tool_st(0),
    tool_trigdec(0), 
    tool_trigconfig(0), 
    tool_trig_match_el(0),
    tool_trig_match_mu(0),
    tool_trig_match_ph(0),    
    tool_jClean(0), 
    tool_tileTrip(0), 
    tool_or(0), 
    tool_purw(0), 
    tool_grl(0),
    iso_1(0),
    iso_2(0),
    iso_3(0),
    iso_4(0),
    iso_5(0),
    tool_bsel70(0), 
    tool_bsel77(0), 
    tool_bsel85(0), 
    tool_btag70(0),    
    tool_btag77(0), 
    tool_btag85(0),
    tool_btag_truth1(0),
    tool_btag_truth2(0),
    tool_btag_truth3(0),
    tool_mcc(0),
    tool_jsmear(0),
#ifdef TILETEST
    tool_jettile(0),
#endif
    tool_mct(0)
{
  if(debug)  Info("chorizo()", "HERE");

  outputName="";

  xmlReader = 0;
  jOption = "";

  isMC=false;
  isSignal=false;
  isTop=false;
  isTruth=false;
  leptonType="";

  isAtlfast=false;
  isQCD=false;
  isNCBG=false; 
  doAnaTree=false;
  doPUTree=false;
  doFlowTree=false;
  doTrigExt=false;

  debug=false;
  eventsFile="";
  printMet=false; 
  printJet=false;
  printElectron=false;
  printMuon=false;

  genPUfile = false;
  isPUfile = false;

  syst_CP  = CP::SystematicSet();
  syst_CPstr = "";
  syst_ST  = SystErr::NONE;
  syst_PU  = pileupErr::NONE; 
  syst_JVF = JvfUncErr::NONE;
  syst_JESNPset = 1;
  
  systListOnly = false;
  errIgnoreLevel = kInfo;
}



EL::StatusCode chorizo :: setupJob (EL::Job& job)
{
  if(debug)   Info("setupJob()", "HERE");

  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init( "chorizo" ).ignore(); // call before opening first file
  
  xAOD::TReturnCode::enableFailure();
  StatusCode::enableFailure();
  CP::SystematicCode::enableFailure();

  return EL::StatusCode::SUCCESS;
}

TH1F* chorizo :: InitHist(std::string name, int nbin, float binlow, float binhigh, std::string xaxis, std::string yaxis) {
  TH1F *h = new TH1F(name.c_str(), name.c_str(), nbin, binlow, binhigh);
  h->GetXaxis()->SetTitle(xaxis.c_str());
  h->GetYaxis()->SetTitle(yaxis.c_str());
  h->Sumw2();
  wk()->addOutput(h);
  return h;
}

void chorizo :: InitGHist(TH1F* h, std::string name, int nbin, float binlow, float binhigh, std::string xaxis, std::string yaxis) {
  h = new TH1F(name.c_str(), name.c_str(), nbin, binlow, binhigh);
  h->GetXaxis()->SetTitle(xaxis.c_str());
  h->GetYaxis()->SetTitle(yaxis.c_str());
  h->Sumw2();
  wk()->addOutput(h);
  h->SetDirectory(out_TDir);
}

void chorizo :: bookTree(){
  if(debug)  Info("bookTree()", "HERE");  

  //add the branches to be saved
  if (output){
    m_atree->Branch ("RunNumber", &RunNumber, "RunNumber/I");
    m_atree->Branch ("EventNumber", &EventNumber, "EventNumber/I");
    m_atree->Branch ("lb", &lb, "lb/I");
    m_atree->Branch ("bcid", &bcid, "bcid/I");
    m_atree->Branch ("procID", &procID, "procID/I");
    //    m_atree->Branch ("mc_channel_number", &mc_channel_number, "EventNumber/I");
    m_atree->Branch ("averageIntPerXing", &averageIntPerXing, "averageIntPerXing/F");
    m_atree->Branch ("nVertex", &nVertex, "nVertex/I");                        
    //common vars
    m_atree->Branch ("pileup_w", &pileup_w, "pileup_w/F");                  
    m_atree->Branch ("MC_w", &MC_w, "MC_w/D");                       

    //presel
    m_atree->Branch ("passPreselectionCuts",&passPreselectionCuts,"passPreselectionCuts/O", 10000);
    m_atree->Branch("isTrigger",&isTrigger);
    
    //FlowTree
    if (doFlowTree){
      m_atree->Branch("isGRL",&isGRL,"isGRL/O", 10000);
      m_atree->Branch("isVertexOk",&isVertexOk,"isVertexOk/O", 10000);
      m_atree->Branch("isLarGood",&isLarGood,"isLarGood/O", 10000);
      m_atree->Branch("isTileGood",&isTileGood,"isTileGood/O", 10000);
      m_atree->Branch("isTileTrip",&isTileTrip,"isTileTrip/O", 10000);
      m_atree->Branch("isCoreFlag",&isCoreFlag,"isCoreFlag/O", 10000);
      m_atree->Branch("isFakeMet",&isFakeMet,"isFakeMet/O", 10000);
      m_atree->Branch("isMetCleaned",&isMetCleaned,"isMetCleaned/O", 10000);
      m_atree->Branch("isBadID",&isBadID,"isBadID/O", 10000);
      m_atree->Branch("isCosmic",&isCosmic,"isCosmic/O", 10000);
      m_atree->Branch("isBadMuon",&isBadMuon,"isBadMuon/O", 10000); 
    }

    
    //metadata
    m_atree->Branch ("xsec", &meta_xsec, "xsec/F");
    m_atree->Branch ("xsec_relunc", &meta_xsec_relunc, "xsec_relunc/F");
    m_atree->Branch ("kfactor", &meta_kfactor, "kfactor/F");
    m_atree->Branch ("feff", &meta_feff, "feff/F");
    m_atree->Branch ("nsim", &meta_nsim, "nsim/F");
    m_atree->Branch ("nwsim", &meta_nwsim, "nwsim/F");
    m_atree->Branch ("lumi", &meta_lumi, "lumi/F");


    if(doAnaTree){ //AnalysisTree
      //weights
      //m_atree->Branch ("w", &w, "w/F");  // CHECK_ME For the moment we do it offline (in the merging)           
      m_atree->Branch ("w_average", &w_average, "w_average/F");             
      m_atree->Branch ("PDF_w", &PDF_w, "PDF_w/F");                      
      m_atree->Branch ("bosonVect_w", &bosonVect_w, "bosonVect_w/F");                 
      m_atree->Branch ("Trigger_w", &Trigger_w, "Trigger_w/F");                  
      m_atree->Branch ("Trigger_w_avg", &Trigger_w_avg, "Trigger_w_avg/F");
      m_atree->Branch ("e_SF", &e_SF,"e_SF/F");
      m_atree->Branch ("m_SF", &m_SF,"m_SF/F");
      m_atree->Branch ("ph_SF", &ph_SF,"ph_SF/F");
      m_atree->Branch ("eb_SF", &eb_SF,"eb_SF/F");
      m_atree->Branch ("mb_SF", &mb_SF,"mb_SF/F");
      m_atree->Branch ("phb_SF", &phb_SF,"phb_SF/F");      
      m_atree->Branch ("e_SFu", &e_SFu,"e_SFu/F");
      m_atree->Branch ("m_SFu", &m_SFu,"m_SFu/F");
      m_atree->Branch ("ph_SFu", &ph_SFu,"ph_SFu/F");
      m_atree->Branch ("e_SFd", &e_SFd,"e_SFd/F");
      m_atree->Branch ("m_SFd", &m_SFd,"m_SFd/F");
      m_atree->Branch ("ph_SFd", &ph_SFd,"ph_SFd/F");
      m_atree->Branch ("eb_SFu", &eb_SFu,"eb_SFu/F");
      m_atree->Branch ("mb_SFu", &mb_SFu,"mb_SFu/F");
      m_atree->Branch ("phb_SFu", &phb_SFu,"phb_SFu/F");
      m_atree->Branch ("eb_SFd", &eb_SFd,"eb_SFd/F");
      m_atree->Branch ("mb_SFd", &mb_SFd,"mb_SFd/F");
      m_atree->Branch ("phb_SFd", &phb_SFd,"phb_SFd/F");      
      
      

      //boson 
      m_atree->Branch ("bos_pt", &bos_pt, "bos_pt/F");                       

      m_atree->Branch ("bosonVec_reco_pt", &bosonVec_reco_pt, "bosonVec_reco_pt/F");             
      m_atree->Branch ("bosonVec_reco_eta", &bosonVec_reco_eta, "bosonVec_reco_eta/F");             
      m_atree->Branch ("bosonVec_reco_phi", &bosonVec_reco_phi, "bosonVec_reco_phi/F");      
      
      m_atree->Branch ("bosonVec_reco_pt_vmu", &bosonVec_reco_pt_vmu, "bosonVec_reco_pt_vmu/F");             
      //m_atree->Branch ("bosonVec_reco_eta", &bosonVec_reco_eta, "bosonVec_reco_eta/F");             
      m_atree->Branch ("bosonVec_reco_phi_vmu", &bosonVec_reco_phi_vmu, "bosonVec_reco_phi_vmu/F");                
      
      m_atree->Branch ("bosonVec_truth_pt", &bosonVec_truth_pt, "bosonVec_truth_pt/F");             
      m_atree->Branch ("bosonVec_truth_eta", &bosonVec_truth_eta, "bosonVec_truth_eta/F");             
      m_atree->Branch ("bosonVec_truth_phi", &bosonVec_truth_phi, "bosonVec_truth_phi/F");             

      m_atree->Branch ("Z_decay", &Z_decay, "Z_decay/I");             
      m_atree->Branch ("Z_pt", &Z_pt, "Z_pt/F");             
      m_atree->Branch ("Z_eta", &Z_eta, "Z_eta/F");             
      m_atree->Branch ("Z_phi", &Z_phi, "Z_phi/F");             

      //truth
      m_atree->Branch("truth_M",&truth_M,"truth_M/F", 10000);
      m_atree->Branch("truth_MT",&truth_MT,"truth_MT/F", 10000);


      m_atree->Branch("truth_n_HFjets",&truth_n_HFjets,"truth_n_HFjets/I", 10000);

      m_atree->Branch("truth_n_leptons",&truth_n_leptons,"truth_n_leptons/I", 10000); 
      m_atree->Branch("truth_Lep1_pt",&truth_Lep1_pt,"truth_Lep1_pt/F", 10000);
      m_atree->Branch("truth_Lep2_pt",&truth_Lep2_pt,"truth_Lep2_pt/F", 10000);
      m_atree->Branch("truth_m_fiducial",&truth_m_fiducial,"truth_m_fiducial/B", 10000);
      m_atree->Branch("truth_shat",&truth_shat,"truth_shat/F", 10000);
      m_atree->Branch("truth_shat_pt",&truth_shat_pt,"truth_shat_pt/F", 10000);
      m_atree->Branch("truth_met",&truth_met,"truth_met/F", 10000);
      m_atree->Branch("truth_met_noEle",&truth_met_noEle,"truth_met_noEle/F", 10000);

      //top
      m_atree->Branch("Top_truth_pt",&Top_truth_pt,"Top_truth_pt/F", 10000);
      m_atree->Branch("Topbar_truth_pt",&Topbar_truth_pt,"Topbar_truth_pt/F", 10000);
      m_atree->Branch("avTop_truth_pt",&avTop_truth_pt,"avTop_truth_pt/F", 10000);
      m_atree->Branch("ttbar_weight",&ttbar_weight,"ttbar_weight/F", 10000);

      //photons
      m_atree->Branch("ph_N",&ph_N,"ph_N/I", 10000);
      m_atree->Branch("ph_pt",&ph_pt);
      m_atree->Branch("ph_eta",&ph_eta);
      m_atree->Branch("ph_phi",&ph_phi);
      //m_atree->Branch("ph_etiso30",&ph_etiso30);
      //m_atree->Branch("ph_ptiso30",&ph_ptiso30);
      m_atree->Branch("ph_tight",&ph_tight);
      m_atree->Branch("ph_type",&ph_type);
      m_atree->Branch("ph_origin",&ph_origin);
      m_atree->Branch("ph_Cone20",&ph_Cone20);      
      m_atree->Branch("ph_Cone40CaloOnly",&ph_Cone40CaloOnly);      
      m_atree->Branch("ph_Cone40",&ph_Cone40);     
      

      m_atree->Branch("ph_SF",&ph_SF,"ph_SF/F");
      m_atree->Branch("ph_SFu",&ph_SFu,"ph_SF/F");
      m_atree->Branch("ph_SFd",&ph_SFd,"ph_SF/F");
      
      m_atree->Branch("ph_trigger",&ph_trigger);

      //electrons
      m_atree->Branch("e_truth_pt",&e_truth_pt,"e_truth_pt/F", 10000);
      m_atree->Branch("e_truth_eta",&e_truth_eta,"e_truth_eta/F", 10000);
      m_atree->Branch("e_truth_phi",&e_truth_phi,"e_truth_phi/F", 10000);

      m_atree->Branch("e_N",&e_N,"e_N/I", 10000);
      m_atree->Branch("e_pt",&e_pt);      
      m_atree->Branch("e_eta",&e_eta);
      m_atree->Branch("e_phi",&e_phi);
      m_atree->Branch("e_type",&e_type);
      m_atree->Branch("e_origin",&e_origin);            
      //m_atree->Branch("e_etiso30",&e_etiso30);
      //m_atree->Branch("e_ptiso30",&e_ptiso30);
      //m_atree->Branch("e_etiso20",&e_etiso20);
      //m_atree->Branch("e_ptiso20",&e_ptiso20);
      m_atree->Branch("e_isoT",&e_isoTight);
      m_atree->Branch("e_isoL",&e_isoLoose);
      m_atree->Branch("e_isoLTO",&e_isoLooseTrackOnly);
      m_atree->Branch("e_isoG",&e_isoGradient);
      m_atree->Branch("e_isoGLoose",&e_isoGradientLoose);
      m_atree->Branch("e_id",&e_id);
      //m_atree->Branch("e_d0_sig",&e_d0_sig);  
      //m_atree->Branch("e_z0",&e_z0);      

      m_atree->Branch("eb_N",&eb_N,"eb_N/I", 10000);
      m_atree->Branch("eb_pt",&eb_pt);
      m_atree->Branch("eb_eta",&eb_eta);
      m_atree->Branch("eb_phi",&eb_phi);

      m_atree->Branch("e_trigger",&e_trigger);

      m_atree->Branch("e_MT",&e_MT,"e_MT/F", 10000);
      m_atree->Branch("e_MT_vmu",&e_MT_vmu,"e_MT_vmu/F", 10000);      
      m_atree->Branch("e_MT_tst",&e_MT_tst,"e_MT_tst/F", 10000);
      m_atree->Branch("e_MT_tst_vmu",&e_MT_tst_vmu,"e_MT_tst_vmu/F", 10000);      
      m_atree->Branch("e_M",&e_M,"e_M/F", 10000);
      m_atree->Branch("e_Zpt",&e_Zpt,"e_Zpt/F", 10000);

      
      //muons
      m_atree->Branch("m_N",&m_N,"m_N/I",10000);
      m_atree->Branch("m_pt",&m_pt);
      m_atree->Branch("m_eta",&m_eta);
      m_atree->Branch("m_phi",&m_phi);      
      m_atree->Branch("m_type",&m_type);
      m_atree->Branch("m_origin",&m_origin);
      //m_atree->Branch("m_etiso20",&m_etiso20);          
      //m_atree->Branch("m_ptiso20",&m_ptiso20);          
      //m_atree->Branch("m_etiso30",&m_etiso30);          
      //m_atree->Branch("m_ptiso30",&m_ptiso30);          
      m_atree->Branch("m_isoTight",&m_isoTight);
      m_atree->Branch("m_isoLoose",&m_isoLoose);
      m_atree->Branch("m_isoLooseTrackOnly",&m_isoLooseTrackOnly);
      m_atree->Branch("m_isoGradient",&m_isoGradient);
      m_atree->Branch("m_isoGradientLoose",&m_isoGradientLoose);

      m_atree->Branch("mb_N",&mb_N,"mb_N/I",10000);
      m_atree->Branch("mb_pt",&mb_pt);
      m_atree->Branch("mb_eta",&mb_eta);
      m_atree->Branch("mb_phi",&mb_phi);

      m_atree->Branch("m_trigger",&m_trigger);

      m_atree->Branch("m_M",&m_M,"m_M/F", 10000);                            
      m_atree->Branch("m_MT",&m_MT,"m_MT/F", 10000);    
      m_atree->Branch("m_MT_vmu",&m_MT_vmu,"m_MT_vmu/F", 10000);                           
      m_atree->Branch("m_MT_tst",&m_MT_tst,"m_MT_tst/F", 10000);    
      m_atree->Branch("m_MT_tst_vmu",&m_MT_tst_vmu,"m_MT_tst_vmu/F", 10000);                            
      m_atree->Branch("m_Zpt",&m_Zpt,"m_Zpt/F", 10000);                      
      m_atree->Branch("m_EM",&m_EM,"m_EM/F", 10000);                         


      //jets
      m_atree->Branch("j_N"   ,&j_N   ,"j_N/I"   ,10000);
      m_atree->Branch("j_N30" ,&j_N30 ,"j_N30/I" ,10000);
      m_atree->Branch("j_N40" ,&j_N40 ,"j_N40/I" ,10000);
      m_atree->Branch("j_N50" ,&j_N50 ,"j_N50/I" ,10000);
      m_atree->Branch("j_N60" ,&j_N60 ,"j_N60/I" ,10000);
      m_atree->Branch("j_N80" ,&j_N80 ,"j_N80/I" ,10000);

      m_atree->Branch("j_pt", &j_pt);
      m_atree->Branch("j_eta",&j_eta);
      m_atree->Branch("j_phi",&j_phi);
      m_atree->Branch("j_m",  &j_m);

      m_atree->Branch("j_chf", &j_chf);
      m_atree->Branch("j_emf", &j_emf);
      m_atree->Branch("j_fsm", &j_fsm);
      m_atree->Branch("j_time",&j_time);
      m_atree->Branch("j_nTrk",&j_nTrk);
      m_atree->Branch("j_sumPtTrk",&j_sumPtTrk);
      m_atree->Branch("j_jvf",&j_jvf);
      m_atree->Branch("j_jvt",&j_jvt);
      m_atree->Branch("j_tflavor",&j_tflavor);

      m_atree->Branch("j1_mT" ,&j1_mT);
      m_atree->Branch("j2_mT" ,&j2_mT);
      m_atree->Branch("j3_mT" ,&j3_mT);
      m_atree->Branch("j4_mT" ,&j4_mT);

      m_atree->Branch("j_truth_N",&j_truth_N,"j_truth_N/I", 10000);
      m_atree->Branch("j_tau_N",&j_tau_N);

      m_atree->Branch("j_truth_pt1",&j_truth_pt1,"j_truth_pt1/F", 10000);
      m_atree->Branch("j_truth_eta1",&j_truth_eta1,"j_truth_eta1/F", 10000);

      //tile output
      if(dumpTile){
	m_atree->Branch("j_const_pt",&j_const_pt);
	m_atree->Branch("j_const_eta",&j_const_eta);
	m_atree->Branch("j_const_phi",&j_const_phi);
	m_atree->Branch("j_const_m",&j_const_m);

	m_atree->Branch("j1_cl_N",&j1_cl_N,"j1_cl_N/I",10000);
	m_atree->Branch("j1_cl_pt",&j1_cl_pt);
	m_atree->Branch("j1_cl_eta",&j1_cl_eta);
	m_atree->Branch("j1_cl_phi",&j1_cl_phi);
	m_atree->Branch("j1_cl_emf",&j1_cl_emf);

	m_atree->Branch("j2_cl_N",&j2_cl_N,"j2_cl_N/I",10000);
	m_atree->Branch("j2_cl_pt",&j2_cl_pt);
	m_atree->Branch("j2_cl_eta",&j2_cl_eta);
	m_atree->Branch("j2_cl_phi",&j2_cl_phi);
	m_atree->Branch("j2_cl_emf",&j2_cl_emf);
      }

      //btagging
      m_atree->Branch("bj_N_70fc",&bj_N_70fc,"bj_N_70fc/I", 10000);
      m_atree->Branch("bj_N_77fc",&bj_N_77fc,"bj_N_77fc/I", 10000);
      m_atree->Branch("bj_N_85fc",&bj_N_85fc,"bj_N_85fc/I", 10000);
     
      m_atree->Branch("btag_weight_total_70fc",&btag_weight_total_70fc,"btag_weight_total_70fc/F", 10000);
      m_atree->Branch("btag_weight_total_77fc",&btag_weight_total_77fc,"btag_weight_total_77fc/F", 10000);
      m_atree->Branch("btag_weight_total_85fc",&btag_weight_total_85fc,"btag_weight_total_85fc/F", 10000);


      //m_atree->Branch("j_tag_MV1",&j_tag_MV1);
      m_atree->Branch("j_tag_MV2c20",&j_tag_MV2c20);

      m_atree->Branch("bj_Nt70",&bj_Nt70,"bj_Nt70/I", 10000);
      m_atree->Branch("bj_Nt77",&bj_Nt77,"bj_Nt77/I", 10000);
      m_atree->Branch("bj_Nt80",&bj_Nt80,"bj_Nt80/I", 10000);

      m_atree->Branch("j_btruth_70",&j_btruth_70);
      m_atree->Branch("j_btruth_77",&j_btruth_77);
      m_atree->Branch("j_btruth_85",&j_btruth_85);
      
      m_atree->Branch("bj_Nf70",&bj_Nf70,"bj_Nf70/I", 10000);
      m_atree->Branch("bj_N",&bj_N,"bj_N/I", 10000);
      m_atree->Branch("bj_Nf85",&bj_Nf85,"bj_Nf85/I", 10000);
      
      m_atree->Branch("j_bflat_70",&j_bflat_70);
      m_atree->Branch("j_bflat_77",&j_bflat_77);
      m_atree->Branch("j_bflat_85",&j_bflat_85);
      
      //met 
      m_atree->Branch("met",&met);
      m_atree->Branch("met_phi",&met_phi);

      m_atree->Branch("met_cst",&met_cst,"met_cst/f", 10000);
      m_atree->Branch("met_tst",&met_tst,"met_tst/f", 10000);
      
      //m_atree->Branch("met_lochadtopo", &met_lochadtopo, "met_lochadtopo/F", 10000);
      m_atree->Branch("met_top", &met_top, "met_top/F", 10000);

      //met recoil system
      //m_atree->Branch("rmet_par", &rmet_par);
      //m_atree->Branch("rmet_norm", &rmet_norm);
      //m_atree->Branch("rmet_par_mod", &rmet_par_mod);
      //m_atree->Branch("rmet_norm_mod", &rmet_norm_mod);
      //m_atree->Branch("rmet_dPhi_met_jetsys", &rmet_dPhi_met_jetsys);

      //(transv. thrust and sphericity
      //m_atree->Branch("tr_spher",&tr_spher); 
      //m_atree->Branch("tr_thrust",&tr_thrust); 

      m_atree->Branch("amt2_0",&amt2_0);
      m_atree->Branch("amt2_1",&amt2_1);
      m_atree->Branch("amt2_2",&amt2_2);      
      m_atree->Branch("amt2_3",&amt2_3);      

      //event variables
      //dphi
      m_atree->Branch("dPhi_met_j1",&dPhi_met_j1);
      m_atree->Branch("dPhi_met_j2",&dPhi_met_j2);
      //m_atree->Branch("dPhi_met_j3",&dPhi_met_j3);
      //m_atree->Branch("dPhi_met_j4",&dPhi_met_j4);
      
      // Max bb-system
      m_atree->Branch("index_min_dR_bb",&index_min_dR_bb); 
      m_atree->Branch("index_min_dR_pt_bb",&index_min_dR_pt_bb);      
      m_atree->Branch("min_dR_bb",&min_dR_bb); 
      m_atree->Branch("min_dR_pt_bb",&min_dR_pt_bb);      
      
      m_atree->Branch("dPhi_met_mettrk",&dPhi_met_mettrk); //recompute from mini-ntuples? //CHECK_ME
      m_atree->Branch("dPhi_min",&dPhi_min);
      m_atree->Branch("dPhi_min_alljets",&dPhi_min_alljets);
      m_atree->Branch("dPhi_min_4jets",&dPhi_min_4jets);
  
      m_atree->Branch("dPhi_j1_j2",&dPhi_j1_j2,"dPhi_j1_j2/f", 10000);
      m_atree->Branch("dPhi_j1_j3",&dPhi_j1_j3,"dPhi_j1_j3/f", 10000);
      m_atree->Branch("dPhi_j2_j3",&dPhi_j2_j3,"dPhi_j2_j3/f", 10000);
      //m_atree->Branch("dPhi_b1_b2",&dPhi_b1_b2,"dPhi_b1_b2/f", 10000);

      //dR
      m_atree->Branch("dR_j1_j2",&dR_j1_j2,"dR_j1_j2/f", 10000);
      m_atree->Branch("dR_j1_j3",&dR_j1_j3,"dR_j1_j3/f", 10000);
      m_atree->Branch("dR_j2_j3",&dR_j2_j3,"dR_j2_j3/f", 10000);

//       m_atree->Branch("dR_j1_m1",&dR_j1_m1,"dR_j1_m1/f", 10000);
//       m_atree->Branch("dR_j1_m2",&dR_j1_m2,"dR_j1_m2/f", 10000);
//       m_atree->Branch("dR_j2_m1",&dR_j2_m1,"dR_j2_m1/f", 10000);
//       m_atree->Branch("dR_j2_m2",&dR_j2_m2,"dR_j2_m2/f", 10000);
//       m_atree->Branch("dR_j3_m1",&dR_j3_m1,"dR_j3_m1/f", 10000);
//       m_atree->Branch("dR_j3_m2",&dR_j3_m2,"dR_j3_m2/f", 10000);

      //dEta
      m_atree->Branch("dEta_j1_j2",&dEta_j1_j2,"dEta_j1_j2/f", 10000);

      //Mbl_min  (for single top CR)
      m_atree->Branch("Melb_min",&Melb_min,"Melb_min/f", 10000);   
      m_atree->Branch("Mmub_min",&Mmub_min,"Mmub_min/f", 10000);         
      
      m_atree->Branch("sumET_cst",&sumET_cst,"sumET_cst/f", 10000);
      m_atree->Branch("sumET_cst_vmu",&sumET_cst_vmu,"sumET_cst_vmu/f", 10000);
      m_atree->Branch("sumET_tst",&sumET_tst,"sumET_tst/f", 10000);
      m_atree->Branch("sumET_tst_vmu",&sumET_tst_vmu,"sumET_tst_vmu/f", 10000);
      m_atree->Branch("sumET_truth",&sumET_truth,"sumET_truth/f", 10000);
      m_atree->Branch("sumET_truth_vmu",&sumET_truth_vmu,"sumET_truth_vmu/f", 10000);


      //MTs
      m_atree->Branch("MT_min_jet_met",&MT_min_jet_met);
      m_atree->Branch("MT_bcl_met",&MT_bcl_met);
      m_atree->Branch("MT_bfar_met",&MT_bfar_met);
      m_atree->Branch("MT_lcl_met",&MT_lcl_met);
      m_atree->Branch("MT_jsoft_met",&MT_jsoft_met);

      //Misc
      m_atree->Branch("mjj",&mjj,"mjj/F", 10000);  
      m_atree->Branch("mbb",&mbb,"mbb/F", 10000);         
      
      m_atree->Branch("mct",&mct,"mct/F", 10000);    
      m_atree->Branch("mct_corr",&mct_corr);    
      m_atree->Branch("meff",&meff);
      m_atree->Branch("HT",&HT,"HT/F", 10000);    
//       m_atree->Branch("AlphaT",&AlphaT,"AlphaT/F", 10000);
//       
//       //Razor
      m_atree->Branch("MR",&MR,"MR/F", 10000);
      m_atree->Branch("MTR",&MTR);
      m_atree->Branch("R",&R);
             
      m_atree->Branch("shatR",&shatR);
      m_atree->Branch("gaminvR",&gaminvR);
      m_atree->Branch("mdeltaR",&mdeltaR);
      m_atree->Branch("cosptR",&cosptR);
      
      //Z candidate 
      m_atree->Branch("Z_flav",&Z_flav,"Z_flav/I", 10000);
      m_atree->Branch("Z_lep1",&Z_lep1,"Z_lep1/I", 10000);
      m_atree->Branch("Z_lep2",&Z_lep2,"Z_lep2/I", 10000);
      m_atree->Branch("Z_m",&Z_m,"Z_m/F", 10000);
      m_atree->Branch("lep3_MT",&lep3_MT);
      m_atree->Branch("lep_mct",&lep_mct,"lep_mct/F", 10000);


      //top reconstruction
      m_atree->Branch("MtTop",&MtTop,"MtTop/F", 10000);     
      m_atree->Branch("m_top_had1",&m_top_had1,"m_top_had1/F", 10000);     
      m_atree->Branch("m_top_had2",&m_top_had2,"m_top_had2/F", 10000);      
      //fat jets 
      m_atree->Branch("m0_antikt12",&m0_antikt12,"m0_antikt12/F", 10000);  
      m_atree->Branch("m1_antikt12",&m1_antikt12,"m1_antikt12/F", 10000);  
      m_atree->Branch("m0_antikt08",&m0_antikt08,"m0_antikt08/F", 10000); 
      m_atree->Branch("m1_antikt08",&m1_antikt08,"m1_antikt08/F", 10000);  
      m_atree->Branch("pt0_antikt12",&pt0_antikt12,"pt0_antikt12/F", 10000);
      m_atree->Branch("pt1_antikt12",&pt1_antikt12,"pt1_antikt12/F", 10000);
      m_atree->Branch("pt0_antikt08",&pt0_antikt08,"pt0_antikt08/F", 10000);
      m_atree->Branch("pt1_antikt08",&pt1_antikt08,"pt1_antikt08/F", 10000);
      m_atree->Branch("mtasym12",&mtasym12,"mtasym12/F", 10000);        
      m_atree->Branch("mtasym08",&mtasym08,"mtasym08/F", 10000);        
      
      //Extended trigger info
      if(doTrigExt){

	m_atree->Branch("trig_l1_ex",               &trig_l1_ex 		              ,"trig_l1_ex/F"); 		            
	m_atree->Branch("trig_l1_ey", 		   &trig_l1_ey 		    	      ,"trig_l1_ey/F"); 		    
	m_atree->Branch("trig_l1_et", 		   &trig_l1_et 		    	      ,"trig_l1_et/F"); 		    
	m_atree->Branch("trig_l1_sumet", 	   &trig_l1_sumet 	   	      ,"trig_l1_sumet/F"); 		    
	m_atree->Branch("trig_l1_phi",		   &trig_l1_phi		    	      ,"trig_l1_phi/F");		    

	m_atree->Branch("trig_hlt_EFJetEtSum_ex",                   &trig_hlt_EFJetEtSum_ex 		              ,"trig_hlt_EFJetEtSum_ex/F"); 		            
	m_atree->Branch("trig_hlt_EFJetEtSum_ey", 		   &trig_hlt_EFJetEtSum_ey 		    	      ,"trig_hlt_EFJetEtSum_ey/F"); 		    
	m_atree->Branch("trig_hlt_EFJetEtSum_et", 		   &trig_hlt_EFJetEtSum_et 		    	      ,"trig_hlt_EFJetEtSum_et/F"); 		    
	m_atree->Branch("trig_hlt_EFJetEtSum_sumet", 		   &trig_hlt_EFJetEtSum_sumet 		    	      ,"trig_hlt_EFJetEtSum_sumet/F"); 		    
	m_atree->Branch("trig_hlt_EFJetEtSum_phi",		   &trig_hlt_EFJetEtSum_phi		    	      ,"trig_hlt_EFJetEtSum_phi/F");		    
									   					    	      					    
	m_atree->Branch("trig_hlt_T2MissingET_ex", 		   &trig_hlt_T2MissingET_ex 		    	      ,"trig_hlt_T2MissingET_ex/F"); 		    
	m_atree->Branch("trig_hlt_T2MissingET_ey", 		   &trig_hlt_T2MissingET_ey 		    	      ,"trig_hlt_T2MissingET_ey/F"); 		    
	m_atree->Branch("trig_hlt_T2MissingET_et", 		   &trig_hlt_T2MissingET_et 		    	      ,"trig_hlt_T2MissingET_et/F"); 		    
	m_atree->Branch("trig_hlt_T2MissingET_sumet", 		   &trig_hlt_T2MissingET_sumet 		    	      ,"trig_hlt_T2MissingET_sumet/F"); 		    
	m_atree->Branch("trig_hlt_T2MissingET_phi",		   &trig_hlt_T2MissingET_phi		      ,"trig_hlt_T2MissingET_phi/F");		    

	m_atree->Branch("trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex", 	&trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex 	,"trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex/F"); 		    
	m_atree->Branch("trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey", 	&trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey 	,"trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey/F"); 		    
	m_atree->Branch("trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et", 	&trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et 	,"trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et/F"); 		    
	m_atree->Branch("trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet", 	&trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet 	,"trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet/F"); 		    
	m_atree->Branch("trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi",	&trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi	,"trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi/F");		    
									   					    	      					    
	m_atree->Branch("trig_hlt_TrigL2MissingET_FEB_ex", 	   &trig_hlt_TrigL2MissingET_FEB_ex 		      ,"trig_hlt_TrigL2MissingET_FEB_ex/F"); 		    
	m_atree->Branch("trig_hlt_TrigL2MissingET_FEB_ey", 	   &trig_hlt_TrigL2MissingET_FEB_ey 		      ,"trig_hlt_TrigL2MissingET_FEB_ey/F"); 		    
	m_atree->Branch("trig_hlt_TrigL2MissingET_FEB_et", 	   &trig_hlt_TrigL2MissingET_FEB_et 		      ,"trig_hlt_TrigL2MissingET_FEB_et/F"); 		    
	m_atree->Branch("trig_hlt_TrigL2MissingET_FEB_sumet", 	   &trig_hlt_TrigL2MissingET_FEB_sumet 	    	      ,"trig_hlt_TrigL2MissingET_FEB_sumet/F"); 	    
	m_atree->Branch("trig_hlt_TrigL2MissingET_FEB_phi",	   &trig_hlt_TrigL2MissingET_FEB_phi	    	      ,"trig_hlt_TrigL2MissingET_FEB_phi/F");	    
									   					    	      					    
	m_atree->Branch("trig_hlt_TrigEFMissingET_ex", 		   &trig_hlt_TrigEFMissingET_ex 		      ,"trig_hlt_TrigEFMissingET_ex/F"); 		    
	m_atree->Branch("trig_hlt_TrigEFMissingET_ey", 		   &trig_hlt_TrigEFMissingET_ey 		      ,"trig_hlt_TrigEFMissingET_ey/F"); 		    
	m_atree->Branch("trig_hlt_TrigEFMissingET_et", 		   &trig_hlt_TrigEFMissingET_et 		      ,"trig_hlt_TrigEFMissingET_et/F"); 		    
	m_atree->Branch("trig_hlt_TrigEFMissingET_sumet", 	   &trig_hlt_TrigEFMissingET_sumet 	    	      ,"trig_hlt_TrigEFMissingET_sumet/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_phi",   	   &trig_hlt_TrigEFMissingET_phi	    	      ,"trig_hlt_TrigEFMissingET_phi/F");	    
									   					    	      					    
	m_atree->Branch("trig_hlt_TrigEFMissingET_FEB_ex", 	   &trig_hlt_TrigEFMissingET_FEB_ex 	    	      ,"trig_hlt_TrigEFMissingET_FEB_ex/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_FEB_ey", 	   &trig_hlt_TrigEFMissingET_FEB_ey 	    	      ,"trig_hlt_TrigEFMissingET_FEB_ey/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_FEB_et", 	   &trig_hlt_TrigEFMissingET_FEB_et 	    	      ,"trig_hlt_TrigEFMissingET_FEB_et/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_FEB_sumet", 	   &trig_hlt_TrigEFMissingET_FEB_sumet 	    	      ,"trig_hlt_TrigEFMissingET_FEB_sumet/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_FEB_phi",	   &trig_hlt_TrigEFMissingET_FEB_phi	              ,"trig_hlt_TrigEFMissingET_FEB_phi/F");	    

	m_atree->Branch("trig_hlt_TrigEFMissingET_mht_ex", 	   &trig_hlt_TrigEFMissingET_mht_ex 	    	      ,"trig_hlt_TrigEFMissingET_mht_ex/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_mht_ey", 	   &trig_hlt_TrigEFMissingET_mht_ey 	    	      ,"trig_hlt_TrigEFMissingET_mht_ey/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_mht_et", 	   &trig_hlt_TrigEFMissingET_mht_et 	    	      ,"trig_hlt_TrigEFMissingET_mht_et/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_mht_sumet", 	   &trig_hlt_TrigEFMissingET_mht_sumet 	    	      ,"trig_hlt_TrigEFMissingET_mht_sumet/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_mht_phi",	   &trig_hlt_TrigEFMissingET_mht_phi	              ,"trig_hlt_TrigEFMissingET_mht_phi/F");	    
									   					    	      					    
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_ex", 	   &trig_hlt_TrigEFMissingET_topocl_ex 	    	      ,"trig_hlt_TrigEFMissingET_topocl_ex/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_ey", 	   &trig_hlt_TrigEFMissingET_topocl_ey 	    	      ,"trig_hlt_TrigEFMissingET_topocl_ey/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_et", 	   &trig_hlt_TrigEFMissingET_topocl_et 	    	      ,"trig_hlt_TrigEFMissingET_topocl_et/F"); 	    
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_sumet",    &trig_hlt_TrigEFMissingET_topocl_sumet     	      ,"trig_hlt_TrigEFMissingET_topocl_sumet/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_phi",      &trig_hlt_TrigEFMissingET_topocl_phi   	      ,"trig_hlt_TrigEFMissingET_topocl_phi/F");   
									   					    	      					    
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PS_ex",    &trig_hlt_TrigEFMissingET_topocl_PS_ex     	      ,"trig_hlt_TrigEFMissingET_topocl_PS_ex/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PS_ey",	   &trig_hlt_TrigEFMissingET_topocl_PS_ey     	      ,"trig_hlt_TrigEFMissingET_topocl_PS_ey/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PS_et",    &trig_hlt_TrigEFMissingET_topocl_PS_et     	      ,"trig_hlt_TrigEFMissingET_topocl_PS_et/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PS_sumet", &trig_hlt_TrigEFMissingET_topocl_PS_sumet  	      ,"trig_hlt_TrigEFMissingET_topocl_PS_sumet/F");  
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PS_phi",   &trig_hlt_TrigEFMissingET_topocl_PS_phi	      ,"trig_hlt_TrigEFMissingET_topocl_PS_phi/F");

	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PUC_ex" ,    &trig_hlt_TrigEFMissingET_topocl_PUC_ex          ,"trig_hlt_TrigEFMissingET_topocl_PUC_ex/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PUC_ey",     &trig_hlt_TrigEFMissingET_topocl_PUC_ey          ,"trig_hlt_TrigEFMissingET_topocl_PUC_ey/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PUC_et",     &trig_hlt_TrigEFMissingET_topocl_PUC_et          ,"trig_hlt_TrigEFMissingET_topocl_PUC_et/F");     
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PUC_sumet",  &trig_hlt_TrigEFMissingET_topocl_PUC_sumet       ,"trig_hlt_TrigEFMissingET_topocl_PUC_sumet/F");  
	m_atree->Branch("trig_hlt_TrigEFMissingET_topocl_PUC_phi",    &trig_hlt_TrigEFMissingET_topocl_PUC_phi	      ,"trig_hlt_TrigEFMissingET_topocl_PUC_phi/F");
      }
    }
    else if(doPUTree){
      
    }
  }
  
}


EL::StatusCode chorizo :: histInitialize ()
{
  if(debug)  Info("histInitialize()", "HERE");

  gErrorIgnoreLevel = this->errIgnoreLevel;

  // if (!outputName.empty()){
  //   output = EL::getNTupleSvc (wk(), outputName);
  // } 
  // else{
  //   output = 0;
  // }

  out_TFile = (TFile*) wk()->getOutputFile ("output");
  m_atree = new TTree ("AnalysisTree", "analysis output tree");
  m_atree->SetDirectory (out_TFile);

  out_TDir  = (TDirectory*) wk()->getOutputFile ("output");

  //Book the output Tree
  bookTree();

  //Load event list (if provided)
  loadEventList();

  //Init MetaData
  meta_xsec = 1.;
  meta_xsec_relunc = 0.;
  meta_kfactor = 1.;
  meta_feff = 1.;
  meta_lumi = 1.;
  
  meta_nsim=0.; 
  meta_nwsim=0.;


  //JopOption
  meta_jOption= new TNamed("jOption", jOption.c_str());
  wk()->addOutput(meta_jOption);     
  
  //Trigger meta
  meta_triggers = new TNamed("Triggers", " "); //to be filled later
  wk()->addOutput(meta_triggers);     

  //METMAP
  meta_metmap = new TNamed("METmap", " ");
  wk()->addOutput(meta_metmap);     

  //Histos
  //---  right errors in histos with weights
  TH1F::SetDefaultSumw2();
  TH2F::SetDefaultSumw2();
  TProfile::SetDefaultSumw2();


  // ** NOT working ... not sure why!
  // InitGHist(h_cut_var, "h_cut_var", 2000, 0, 1000., "Cut Var (GeV)", "");  
  // InitGHist(h_presel_flow, "h_presel_flow", 10, 0., 10., "", "");
  // InitGHist(h_presel_wflow, "h_presel_wflow", 10, 0., 10., "", "");

  //need template to do TH1D as well...
  h_average = new TH1D("beginning","beggining",1,0,1);
  wk()->addOutput(h_average);     
  h_average->SetDirectory(out_TDir);

  //histograms
  h_presel_flow = new TH1F("h_presel_flow","",10,0,10);
  h_presel_flow->Sumw2();
  wk()->addOutput(h_presel_flow);     
  h_presel_flow->SetDirectory(out_TDir);

  h_presel_wflow = new TH1F("h_presel_wflow","",10,0,10);
  h_presel_wflow->Sumw2();
  wk()->addOutput(h_presel_wflow);     
  h_presel_wflow->SetDirectory(out_TDir);

  
  TString cutNames[] = {"None","GRL","Trigger","PVertex","LarGood","TileGood","TileTrip","CoreFlag","CleanJet","CleanMuon"};  
  for (int i=1; i<h_presel_flow->GetNbinsX()+1; ++i) {
    h_presel_flow->GetXaxis()->SetBinLabel(i,cutNames[i-1].Data());
    h_presel_wflow->GetXaxis()->SetBinLabel(i,cutNames[i-1].Data());
  }

  //Systematics (override CP set if string is given) [tmp hack]
  if(!syst_CPstr.IsNull()){
    syst_CP  = CP::SystematicSet(); //needed?

    TString ctag="";
    if(syst_CPstr.Contains("continuous")){
      ctag="__continuous";
    
      if(syst_CPstr.Contains("1up"))
	syst_CP.insert( CP::SystematicVariation(std::string(syst_CPstr.Copy().ReplaceAll(ctag+"__1up","").Data()), 1));
      else if(syst_CPstr.Contains("1down"))
	syst_CP.insert( CP::SystematicVariation(std::string(syst_CPstr.Copy().ReplaceAll(ctag+"__1down","").Data()), -1));
      else{
	Error("histInitialize()", "Cannot configure a continuous systematic like this : %s . Please attach e.g. __1up to it so I know what to do! ", syst_CPstr.Data());
	return EL::StatusCode::FAILURE;
      }
    }
    else{
      syst_CP.insert( CP::SystematicVariation(std::string(syst_CPstr)));
    }
  }

  return EL::StatusCode::SUCCESS;
}


CutflowInfo chorizo :: getNinfo(){

  if(debug)  Info("getNinfo()", "HERE");

  CutflowInfo sinfo = {};
  
  m_event = wk()->xaodEvent();   

  //  if(m_isderived || 1){
  if(!this->isTruth){

    //    m_event = wk()->xaodEvent();   

    const xAOD::CutBookkeeperContainer* incompleteCBC = nullptr;
    if(!m_event->retrieveMetaInput(incompleteCBC, "IncompleteCutBookkeepers").isSuccess()){
      Error("initializeEvent()","Failed to retrieve IncompleteCutBookkeepers from MetaData! Exiting.");
      return sinfo;
    }
    if ( incompleteCBC->size() != 0 ) {
      Error("initializeEvent()","Found incomplete Bookkeepers! Check file for corruption.");
      return sinfo;
    }

    //Read the CutBookkeeper container
    const xAOD::CutBookkeeperContainer* completeCBC = 0;
    if (!m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess()) {
      Error( APP_NAME, "Failed to retrieve CutBookkeepers from MetaData! Exiting.");
      return sinfo;
    }

    //for DEBUGGING
    cout << " CBK CICLES ::   AMI\t" << wk()->metaData()->getDouble( SH::MetaFields::numEvents ) << endl; 

    // First, let's find the smallest cycle number,
    // i.e., the original first processing step/cycle
    int minCycle = 10000;
    for ( auto cbk : *completeCBC ) {
      if ( ! cbk->name().empty()  && minCycle > cbk->cycle() ){ minCycle = cbk->cycle(); }

      //for DEBUGGING
      cout << " CBK CICLES ::   " << cbk->cycle() << "\t" <<  cbk->name() << "\t" << cbk->nAcceptedEvents() << "\t" << cbk->sumOfEventWeights() << "\t" << cbk->sumOfEventWeightsSquared() << endl;
    }

    // Now, let's actually find the right one that contains all the needed info...
    const xAOD::CutBookkeeper* allEventsCBK=0;
    const xAOD::CutBookkeeper* DxAODEventsCBK=0;
    std::string derivationName = "SUSY1Kernel"; //need to replace by appropriate name
    for ( auto cbk :  *completeCBC ) {
      if ( minCycle == cbk->cycle() && cbk->name() == "AllExecutedEvents" ) {
	allEventsCBK = cbk;
	//	break;
      }
      if ( cbk->name() == derivationName){
	DxAODEventsCBK = cbk;
      }
    }

    if(allEventsCBK){    
      sinfo.nEvents    = allEventsCBK->nAcceptedEvents();
      sinfo.weightSum  = allEventsCBK->sumOfEventWeights();
      sinfo.weight2Sum = allEventsCBK->sumOfEventWeightsSquared();
    }
    if(DxAODEventsCBK){
      sinfo.nEventsDx    = DxAODEventsCBK->nAcceptedEvents();
      sinfo.weightSumDx  = DxAODEventsCBK->sumOfEventWeights();
      sinfo.weight2SumDx = DxAODEventsCBK->sumOfEventWeightsSquared();
    }
  }
  else{
    try{
      m_ctree = dynamic_cast<TTree*> (wk()->inputFile()->Get("CollectionTree"));
      sinfo.nEvents      = m_ctree->GetEntries();
      sinfo.weightSum    = sinfo.nEvents;
      sinfo.weight2Sum   = sinfo.nEvents*sinfo.nEvents;
      sinfo.nEventsDx    = sinfo.nEvents;
      sinfo.weightSumDx  = sinfo.nEvents;
      sinfo.weight2SumDx = sinfo.nEvents*sinfo.nEvents;
    }
    catch(...){
      // pass
    }
  }
  return sinfo;
  
}

bool chorizo :: isDerived(){

  //Try to get the original number of events (relevant in case of derivations)
  if (!m_MetaData) 
    m_MetaData = dynamic_cast<TTree*> (wk()->inputFile()->Get("MetaData"));

  if (!m_MetaData) {
    Error("isDerived()", "\nDid not manage to get MetaData tree. Leaving...");
    return EL::StatusCode::FAILURE;
  }
  
  
  //check if file is from a DxAOD
  return !m_MetaData->GetBranch("StreamAOD");

}

void chorizo :: loadEventList(){
  //load event list 
  m_eventList={};
  if(eventsFile=="") return;

  std::ifstream infile(eventsFile);
  
  UInt_t a, b;
  cout << "EVENTS LIST " << endl;
  cout << "-----------------------------------------" << endl;
  while (infile >> a >> b){
    cout << "ADDING (RUN, EVENT) :  " << a << "   " << b << endl;
    m_eventList.insert( std::make_pair(a, b) );
  }
}

bool chorizo :: inEventList(UInt_t run, UInt_t event){
  Pint p  = std::make_pair(run,event);
  if( m_eventList.find(p) != m_eventList.end() ) return true;
  return false;
}

void chorizo :: InitVars()
{
  if(debug)  Info("InitVars()", "HERE");
 
  //Initialize ntuple variables

  //- Event info
  RunNumber = 0;
  EventNumber = 0;
  lb = 0;
  bcid = 0;
  procID = 0;
  averageIntPerXing = 0;
 
  //- Data Quality (event is good by default! (i.e. MC))
  isGRL = true; 
  isFakeMet = false;                 
  isBadID = false;                   
  isMetCleaned = true; //CHECK_ME
  isTrigger.clear();
  isVertexOk = false;                
  isLarGood = true;                 
  isTileGood = true;                
  isTileTrip = false;                
  isCoreFlag = false;          
  isCosmic = false;          
  isBadMuon = false;          
      
  passPreselectionCuts = false;

  //- weights
  w = 1.;            
  w_average = 1.;            
  MC_w = 1.;         
  PDF_w = 1.;        
  pileup_w = 1.;        
  bosonVect_w = 1.;  
  Trigger_w = 1.;    
  Trigger_w_avg = 1.;
  e_SF = 1.;
  m_SF = 1.;
  ph_SF = 1.;
  eb_SF = 1.;
  mb_SF = 1.;
  phb_SF = 1.;  
  e_SFu = 1.;
  m_SFu = 1.;
  ph_SFu = 1.;
  e_SFd = 1.;
  m_SFd = 1.;
  ph_SFd = 1.;
  eb_SFu = 1.;
  mb_SFu = 1.;
  phb_SFu = 1.;
  eb_SFd = 1.;
  mb_SFd = 1.;
  phb_SFd = 1.;

  //- Top    
  ttbar_weight = 1.;                  
  Top_truth_pt = DUMMYDN;                  
  Topbar_truth_pt = DUMMYDN;                  
  avTop_truth_pt = DUMMYDN;                  
                                       
  //- Boson info                       
  bos_pt = DUMMYDN;                      
  bos_m = DUMMYDN;                       
  bos_pdgId = DUMMYDN;                   
  bosonVec_reco_pt = DUMMYDN;            
  bosonVec_reco_eta = DUMMYDN;           
  bosonVec_reco_phi = DUMMYDN;   
  bosonVec_reco_pt_vmu = DUMMYDN;          
  bosonVec_reco_phi_vmu = DUMMYDN;             
  bosonVec_truth_pt = DUMMYDN;           
  bosonVec_truth_eta = DUMMYDN;          
  bosonVec_truth_phi = DUMMYDN;          
  Z_decay = DUMMYDN;
  Z_pt = DUMMYDN;
  Z_eta = DUMMYDN;
  Z_phi = DUMMYDN;
                       
  //- truth kin info                   
  truth_MT = DUMMYDN;                      
  truth_M = DUMMYDN;                       
  truth_Lep1_pt = DUMMYDN;                 
  truth_Lep2_pt = DUMMYDN;                 
  truth_m_fiducial = false;              
  truth_met = DUMMYDN;                     
  truth_met_noEle = DUMMYDN;               
                            
  truth_n_bjets = DUMMYDN;        
  truth_n_HFjets = DUMMYDN;       
  truth_n_leptons = DUMMYDN;      
  
  //- Production kin        
  //only for signal samples 
  sigSam_pt1 = DUMMYDN;         
  sigSam_eta1 = DUMMYDN;        
  sigSam_phi1 = DUMMYDN;        
  sigSam_E1 = DUMMYDN;          
  sigSam_pt2 = DUMMYDN;         
  sigSam_eta2 = DUMMYDN;        
  sigSam_phi2 = DUMMYDN;        
  sigSam_E2 = DUMMYDN;          
  truth_shat = DUMMYDN;         
  truth_shat_pt = DUMMYDN;        

  //- Event selection
  nVertex = 0;

  //- Electron Info
  e_N = 0;                
  e_pt.clear();
  e_eta.clear();
  e_phi.clear();
  e_type.clear();
  e_origin.clear();  
  e_ptiso30.clear();
  e_etiso30.clear();
  e_ptiso20.clear();
  e_etiso20.clear();
  e_isoTight.clear();
  e_isoLoose.clear();
  e_isoLooseTrackOnly.clear();
  e_isoGradient.clear();
  e_isoGradientLoose.clear();
  e_id.clear();
  e_d0_sig.clear();
  e_z0.clear();

  eb_N = 0;  
  eb_pt.clear();
  eb_eta.clear();
  eb_phi.clear();

  e_truth_pt = DUMMYDN;        
  e_truth_eta = DUMMYDN;       
  e_truth_phi = DUMMYDN;       

  e_trigger.clear();
  
  e_M = DUMMYDN;      
  e_MT = DUMMYDN;   
  e_MT_vmu = DUMMYDN;     
  e_MT_tst = DUMMYDN;   
  e_MT_tst_vmu = DUMMYDN;     
  e_Zpt = DUMMYDN;             

  //- Photon Info
  ph_N = 0; 
  ph_pt.clear();
  ph_eta.clear();
  ph_phi.clear();
  ph_ptiso30.clear();
  ph_etiso30.clear();
  ph_tight.clear();           
  ph_type.clear();
  ph_origin.clear();
  ph_Cone20.clear();
  ph_Cone40CaloOnly.clear();  
  ph_Cone40.clear();  
  
  ph_trigger.clear();
  
  //- Muon info
  m_N = 0;
  m_pt.clear(); 
  m_eta.clear();               
  m_phi.clear();     
  m_type.clear();
  m_origin.clear();            
  m_ptiso30.clear();          
  m_etiso30.clear();          
  m_ptiso20.clear();          
  m_etiso20.clear(); 
  m_isoTight.clear();
  m_isoLoose.clear();
  m_isoLooseTrackOnly.clear();
  m_isoGradient.clear();
  m_isoGradientLoose.clear();

  mb_N = 0;
  mb_pt.clear(); 
  mb_eta.clear();
  mb_phi.clear();

  m_trigger.clear();

  m_M = DUMMYDN;                
  m_MT = DUMMYDN;  
  m_MT_vmu = DUMMYDN;                
  m_MT_tst = DUMMYDN;  
  m_MT_tst_vmu = DUMMYDN;                
  m_Zpt = DUMMYDN;              
  m_EM = DUMMYDN;               


  //- Jet Info
  j_N=0;
  j_N30=0;
  j_N40=0;
  j_N50=0;
  j_N60=0;
  j_N80=0;

  jb_truth_N = 0;
  j_truth_N  = 0;
  j_truth_pt1=DUMMYDN;
  j_truth_eta1=DUMMYDN;
  
  j1_mT.clear(); 
  j2_mT.clear(); 
  j3_mT.clear(); 
  j4_mT.clear(); 

  j_tau_N.clear();

  j_pt.clear();                
  j_eta.clear();               
  j_phi.clear();               
  j_m.clear();               
  j_chf.clear();
  j_emf.clear();
  j_fsm.clear();
  j_time.clear();
  j_nTrk.clear();
  j_sumPtTrk.clear();
  j_jvf.clear();
  j_jvt.clear();
  j_tflavor.clear();
  j_tag_MV1.clear();
  j_tag_MV2c20.clear();

  if(dumpTile){
    j_const_pt.clear();
    j_const_eta.clear();
    j_const_phi.clear();
    j_const_m.clear();

    j1_cl_N = 0;
    j1_cl_pt.clear();
    j1_cl_eta.clear();
    j1_cl_phi.clear();
    j1_cl_emf.clear();

    j2_cl_N = 0;
    j2_cl_pt.clear();
    j2_cl_eta.clear();
    j2_cl_phi.clear();
    j2_cl_emf.clear();
  };
                                               
  //- Btagging                                 
  bj_N_70fc = 0;                               
  bj_N_77fc = 0; 
  bj_N_85fc = 0; 
                                  
  btag_weight_total_70fc = 1.;
  btag_weight_total_77fc = 1.;
  btag_weight_total_85fc = 1.;

  bj_Nt70 = 0;                               
  bj_Nt77 = 0;                               
  bj_Nt80 = 0;                              
   
  j_btruth_70.clear();
  j_btruth_77.clear();
  j_btruth_85.clear();
  
  bj_Nf70 = 0;                               
  bj_N = 0;                               
  bj_Nf85 = 0;   

  j_bflat_70.clear();
  j_bflat_77.clear();
  j_bflat_85.clear();

  //met 
  met.clear();
  met_phi.clear();

  met_cst = DUMMYDN;
  met_tst = DUMMYDN;

  met_lochadtopo = DUMMYDN;
  met_top = DUMMYDN;

  index_min_dR_bb.clear();
  index_min_dR_pt_bb.clear();  
  min_dR_bb.clear();
  min_dR_pt_bb.clear(); 

  //recoiling system
  rmet_par.clear();
  rmet_norm.clear();
  rmet_par_mod.clear(); 
  rmet_norm_mod.clear();
  rmet_dPhi_met_jetsys.clear();

  //(transverse) sphericity
  tr_spher.clear();
  tr_thrust.clear(); 
  
  amt2_0.clear();
  amt2_1.clear();
  amt2_2.clear();
  amt2_3.clear();

  //event variables 
  dPhi_met_j1.clear();
  dPhi_met_j2.clear(); 
  dPhi_met_j3.clear(); 
  dPhi_met_j4.clear(); 
  dPhi_met_mettrk.clear();                     
  dPhi_j1_j2 = DUMMYDN;                          
  dPhi_j1_j3 = DUMMYDN;                          
  dPhi_j2_j3 = DUMMYDN;                          
  dPhi_b1_b2 = DUMMYDN;                          
  dPhi_min.clear();
  dPhi_min_alljets.clear();
  dPhi_min_4jets.clear();  
  dR_j1_j2 = DUMMYDN;                            
  dR_j1_j3 = DUMMYDN;                            
  dR_j2_j3 = DUMMYDN;                            
  dR_j1_m1 = DUMMYDN;                            
  dR_j1_m2 = DUMMYDN;                            
  dR_j2_m1 = DUMMYDN;                            
  dR_j2_m2 = DUMMYDN;                            
  dR_j3_m1 = DUMMYDN;                            
  dR_j3_m2 = DUMMYDN;                            
  dEta_j1_j2 = DUMMYDN;                          

  M12 = DUMMYDN;
  
  Melb_min = DUMMYDN; 
  Mmub_min = DUMMYDN;   
  

  sumET_cst = DUMMYDN;
  sumET_cst_vmu = DUMMYDN;
  sumET_tst = DUMMYDN;
  sumET_tst_vmu = DUMMYDN;
  sumET_truth = DUMMYDN;
  sumET_truth_vmu = DUMMYDN;


  MT_min_jet_met.clear();  
  MT_bcl_met.clear();  
  MT_bfar_met.clear();  
  MT_lcl_met.clear();  
  MT_jsoft_met.clear();  

  mjj=0.;
  mbb=0.;

  mct=0.;
  mct_corr.clear();
  meff.clear();
  HT=0.;

  AlphaT=DUMMYDN;

  //Razor
  MR=DUMMYDN;
  MTR.clear();
  R.clear();
  shatR.clear();
  gaminvR.clear();
  mdeltaR.clear();
  cosptR.clear();

  //Z reco
  Z_flav = -99;
  Z_lep1 = -99;
  Z_lep2 = -99;
  Z_m = DUMMYDN;
  lep3_MT.clear();
  lep_mct = DUMMYDN;
    
  //top reconstruction
  MtTop = DUMMYDN;
  m_top_had1 = DUMMYDN;
  m_top_had2 = DUMMYDN;

  //fat jets
  m0_antikt12 = DUMMYDN;
  m1_antikt12 = DUMMYDN;
  m0_antikt08 = DUMMYDN;
  m1_antikt08 = DUMMYDN;
  pt0_antikt12 = DUMMYDN;
  pt1_antikt12 = DUMMYDN;
  pt0_antikt08 = DUMMYDN;
  pt1_antikt08 = DUMMYDN;
  mtasym12 = DUMMYDN;
  mtasym08 = DUMMYDN;

  warningPileup = true; //to do some warnings prints first time only! (GetAverageWeight())

  //Clear particle collections
  electronCandidates.clear();
  muonCandidates.clear();
  recoElectrons.clear();
  recoPhotons.clear();
  recoMuons.clear();
  recoJets.clear();
  truthElectrons.clear();
  seedJets.clear();

  //Extended trigger
  if(doTrigExt){

    trig_l1_ex=0.; 			   
    trig_l1_ey=0.; 			   
    trig_l1_et=0.; 			   
    trig_l1_sumet=0.; 			   
    trig_l1_phi=0.;			   

    trig_hlt_EFJetEtSum_ex=0.; 			   
    trig_hlt_EFJetEtSum_ey=0.; 			   
    trig_hlt_EFJetEtSum_et=0.; 			   
    trig_hlt_EFJetEtSum_sumet=0.; 			   
    trig_hlt_EFJetEtSum_phi=0.;			   
                                                   
    trig_hlt_T2MissingET_ex=0.; 			   
    trig_hlt_T2MissingET_ey=0.; 			   
    trig_hlt_T2MissingET_et=0.; 			   
    trig_hlt_T2MissingET_sumet=0.; 			   
    trig_hlt_T2MissingET_phi=0.;			   
                                                   
    trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex=0.;   
    trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey=0.;   
    trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et=0.;   
    trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet=0.;
    trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi=0.;  
                                                   
                                                   
    trig_hlt_TrigL2MissingET_FEB_ex=0.; 		   
    trig_hlt_TrigL2MissingET_FEB_ey=0.; 		   
    trig_hlt_TrigL2MissingET_FEB_et=0.; 		   
    trig_hlt_TrigL2MissingET_FEB_sumet=0.; 		   
    trig_hlt_TrigL2MissingET_FEB_phi=0.;		   
                                                   
    trig_hlt_TrigEFMissingET_FEB_ex=0.; 		   
    trig_hlt_TrigEFMissingET_FEB_ey=0.; 		   
    trig_hlt_TrigEFMissingET_FEB_et=0.; 		   
    trig_hlt_TrigEFMissingET_FEB_sumet=0.; 		   
    trig_hlt_TrigEFMissingET_FEB_phi=0.;		   
                                                   
    trig_hlt_TrigEFMissingET_ex=0.; 			   
    trig_hlt_TrigEFMissingET_ey=0.; 			   
    trig_hlt_TrigEFMissingET_et=0.; 			   
    trig_hlt_TrigEFMissingET_sumet=0.; 		   
    trig_hlt_TrigEFMissingET_phi=0.;			   
                                                   
    trig_hlt_TrigEFMissingET_mht_ex=0.; 		   
    trig_hlt_TrigEFMissingET_mht_ey=0.; 		   
    trig_hlt_TrigEFMissingET_mht_et=0.; 		   
    trig_hlt_TrigEFMissingET_mht_sumet=0.; 		   
    trig_hlt_TrigEFMissingET_mht_phi=0.;		   
                                                   
    trig_hlt_TrigEFMissingET_topocl_ex=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_ey=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_et=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_sumet=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_phi=0.;		   
                                                   
    trig_hlt_TrigEFMissingET_topocl_PS_ex=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_PS_ey=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_PS_et=0.; 		   
    trig_hlt_TrigEFMissingET_topocl_PS_sumet=0.; 	   
    trig_hlt_TrigEFMissingET_topocl_PS_phi=0.;		   
                                                   
    trig_hlt_TrigEFMissingET_topocl_PUC_ex=0.; 	   
    trig_hlt_TrigEFMissingET_topocl_PUC_ey=0.; 	   
    trig_hlt_TrigEFMissingET_topocl_PUC_et=0.; 	   
    trig_hlt_TrigEFMissingET_topocl_PUC_sumet=0.; 	   
    trig_hlt_TrigEFMissingET_topocl_PUC_phi=0.;           
  }

}


EL::StatusCode chorizo :: fileExecute ()
{
  if(debug)  Info("fileExecute()", "HERE");

  //--- Get sum of weights and initial numbers of events
  // get the MetaData tree once a new file is opened, with
  //  if (!m_MetaData) 
  m_MetaData = dynamic_cast<TTree*> (wk()->inputFile()->Get("MetaData"));

  if (!m_MetaData) {
    Error("fileExecute()", "MetaData not found! Exiting.");
    return EL::StatusCode::FAILURE;
  }

  try{
    m_MetaData->LoadTree(0);
  }
  catch(...){
    Info("fileExecute()", "Could not LoadTree()");
  }

  //check if file is from a DxAOD
  try{
    m_isderived = ! m_MetaData->GetBranch("StreamAOD");
  }
  catch(...){
    Info("fileExecute()", "Could not determine derivation state (no StreamAOD accesible)");
  }


  CutflowInfo sinfo = getNinfo(); 
  meta_nsim  += sinfo.nEvents; //load number of events
  meta_nwsim += sinfo.weightSum; //load weighted number of events

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode chorizo :: changeInput (bool firstFile)
{
  if(debug)  Info("changeInput()", "HERE");

  m_event = wk()->xaodEvent();   

  // get event info needed for tool's config
  const xAOD::EventInfo* eventInfo = 0;
  CHECK( m_event->retrieve( eventInfo, "EventInfo") );
  
  isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  
  //--- Load xs_section, kfactors, etc...
  loadMetaData();

  return EL::StatusCode::SUCCESS;
}

void chorizo :: ReadXML(){
  if(debug)  Info("ReadXML()", "HERE");

  const char* whereAmI = "chorizo::ReadXML()";
  Info(whereAmI, "--- Read XML options ---");
  //read XML options
  std::string maindir = getenv("ROOTCOREBIN");

  xmlReader = new XMLReader();
  xmlReader->readXML(maindir+"/data/SusyAnalysis/"+jOption);

  //------ Define and read global variables from the XML
  Info(whereAmI, Form(" - Cut Flow") );
  doCutFlow = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoCutFlow");  
  
  isStopTL    = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/StopTL");
  m_skim_btag = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoSkimming_BTAG");  
  m_skim_met  = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoSkimming_MET");  
  dumpTile    = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/TileDump");  

  Info(whereAmI, Form(" - PDF reweighting") );
  doPDFrw = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$PdfRw$Enable");
  beamE_to = xmlReader->retrieveFloat("AnalysisOptions$GeneralSettings$PdfRw$ToBeamE");

  Info(whereAmI, Form(" - GRL") );
  GRLxmlFile = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/GRLxmlFile").c_str());

  Info(whereAmI, Form(" - Pileup") );
  applyPURW = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Pileup$Mode/name/ApplyPileupReweighting");
  PURW_Folder = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Pileup$Path/name/PileupReweightingConfigFolder").c_str());
  PURW_IlumicalcFile = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Pileup$Path/name/PileupReweightingIlumicalcFile").c_str());

  Info(whereAmI, Form(" - LeptonEff Unitarity") );
  leptonEfficiencyUnitarity = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$LeptonEfficiency$Unitarity");

#ifdef TRIGGERTEST
  Info(whereAmI, Form(" - Trigger") );
  std::string triggerNameStr="";
  triggerNameStr = xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Trigger$chains");

  std::istringstream triggerNameIStr(triggerNameStr);
  std::string s;
  while (std::getline(triggerNameIStr, s, ',')) {
    Info(whereAmI, Form("adding trigger chain = %s",s.c_str()));
    TriggerNames.push_back(s);
  }

  //electron triggers for matching
  triggerNameStr = xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Electron$triggers");
  std::istringstream EltriggerNameIStr(triggerNameStr);
  while (std::getline(EltriggerNameIStr, s, ',')) {
    Info(whereAmI, Form("adding electron trigger chain = %s",s.c_str()));
    ElTriggers.push_back(s);
  }

  //muon triggers for matching
  triggerNameStr = xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Muon$triggers");
  std::istringstream MutriggerNameIStr(triggerNameStr);
  while (std::getline(MutriggerNameIStr, s, ',')) {
    Info(whereAmI, Form("adding muon trigger chain = %s",s.c_str()));
    MuTriggers.push_back(s);
  }

  //photon triggers for matching
  triggerNameStr = xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Photon$triggers");
  std::istringstream PhtriggerNameIStr(triggerNameStr);
  while (std::getline(PhtriggerNameIStr, s, ',')) {
    Info(whereAmI, Form("adding photon trigger chain = %s",s.c_str()));
    PhTriggers.push_back(s);
  }

#endif

  Info(whereAmI, Form(" - Truth") );
  dressLeptons = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Truth$dressLeptons");
  simBtagging  = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Truth$simBtagging");

  Info(whereAmI, Form(" - Overlap Removal") );
  doOR = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$Enable");
  m_or_useSigLep = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$useSignalLeptons");
  m_or_useIsoLep = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$useIsoLeptons");
  m_or_bjetOR = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$doBjetOR");
  doORphotons = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$doPhotons");
  
  Info(whereAmI, Form(" - TrackVeto" ));
  tVeto_Enable = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$TrackVeto$Enable");
  tVeto_Pt               = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$Pt");
  tVeto_Eta              = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$Eta");
  tVeto_d0               = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$d0");
  tVeto_z0               = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$z0");
  tVeto_ndof             = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$ndof");
  tVeto_chi2OverNdof_min = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$chi2OverNdof_min");
  tVeto_chi2OverNdof_max = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$chi2OverNdof_max");
  PixHitsAndSCTHits      = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$PixHitsAndSCTHits");
  tVeto_TrackIso         = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$TrackVeto$TrackIso");

  Info(whereAmI, Form(" - Vertex") );
  nTracks = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Vertex$nTracks");

  Info(whereAmI, Form(" - Electrons") );
  El_PreselPtCut   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$PreselPtCut");
  El_PreselEtaCut  = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$PreselEtaCut");
  El_RecoPtCut     = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$RecoPtCut");
  El_RecoEtaCut    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$RecoEtaCut");

  El_baseID        = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Electron$baseID")).Data());
  El_ID            = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Electron$ID")).Data());
  El_isoWP         = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Electron$isoWP")).Data());
  El_d0sigcut      = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$d0sigcut"); 
  El_z0cut         = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Electron$z0cut"); 
  
  El_recoSF        = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Electron$recoSF");
  El_idSF          = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Electron$idSF");
  El_triggerSF     = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Electron$triggerSF");

  Info(whereAmI, Form(" - Muons") );
  Mu_PreselPtCut   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$PreselPtCut");
  Mu_PreselEtaCut  = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$PreselEtaCut");
  Mu_RecoPtCut     = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$RecoPtCut");
  Mu_RecoEtaCut    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$RecoEtaCut");

  Mu_ID            = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Muon$ID")).Data());
  Mu_isoWP         = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Muon$isoWP")).Data());    
  Mu_d0sigcut      = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$d0sigcut"); 
  Mu_z0cut         = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Muon$z0cut"); 

  Info(whereAmI, Form(" - Photons") );
  Ph_PreselPtCut   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Photon$PreselPtCut");
  Ph_PreselEtaCut  = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Photon$PreselEtaCut");
  Ph_RecoPtCut     = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Photon$RecoPtCut");
  Ph_RecoEtaCut    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Photon$RecoEtaCut");

  Ph_ID            = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Photon$ID")).Data());
  Ph_isoWP         = std::string(TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Photon$isoWP")).Data());    
  
  Ph_recoSF        = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Photon$recoSF");
  Ph_idSF          = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Photon$idSF");
  Ph_triggerSF     = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Photon$triggerSF");
  
  Info(whereAmI, Form(" - Jets") );
  JetCollection     = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Jet$Collection").c_str());
  Jet_Tagger        = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Jet$Tagger").c_str());
  Jet_Btag_WP       = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Jet$BtagWP").c_str());
  Jet_Btag_WP_OR    = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Jet$BtagWP_OR").c_str());

  Jet_PreselPtCut  = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$PreselPtCut");
  Jet_PreselEtaCut = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$PreselEtaCut");
  Jet_RecoPtCut    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$RecoPtCut");
  Jet_RecoEtaCut   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$RecoEtaCut");
  Jet_RecoJVTCut   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$RecoJVTCut");
  
  Info(whereAmI, Form(" - Etmiss") );
  METCollection        = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Etmiss$Collection").c_str());
  Met_FakeMetEstimator = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$Etmiss$Path/name/FakeMetEstimator").c_str());
  Met_doFakeEtmiss     = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$doFakeEtmiss");
  Met_doMetCleaning    = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$doMetCleaning");
  Met_doRefEle         = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doRefEle");
  Met_doRefGamma       = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doRefGamma");
  Met_doRefTau         = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doRefTau");
  Met_doRefJet         = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doRefJet");
  Met_doMuons          = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doMuons");
  Met_doSoftTerms      = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Etmiss$Term/name/doSoftTerms");

  Info(whereAmI, Form("- QCD") );
  std::string QCD_triggerNameStr="";
  QCD_triggerNameStr = xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$TrigChains");
  QCD_JetsPtPreselection = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$QCD$JetsPtPreselection");
  QCD_JetsPtSelection    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$RecoPtCut");
  QCD_JetsEtaSelection   = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$Jet$RecoEtaCut");
  QCD_METSig             = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$QCD$METSigCut");
  QCD_LeadJetPreSel      = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$LeadJetPreSel").c_str());
  QCD_RandomSeedOffset   = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$QCD$RandomSeedOffset");
  QCD_SmearType          = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$SmearType").c_str());
  QCD_SmearUseBweight    = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$QCD$SmearUseBweight");
  QCD_SmearBtagWeight    = xmlReader->retrieveFloat("AnalysisOptions$ObjectDefinition$QCD$SmearBtagWeight");
  QCD_SmearMeanShift     = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$SmearMeanShift").c_str());
  QCD_SmearExtraSmr      = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$QCD$SmearExtraSmr");
  QCD_DoPhiSmearing      = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$QCD$DoPhiSmearing");
  QCD_SmearedEvents      = (unsigned int)xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$QCD$SmearedEvents");

  QCD_btagFileMap        = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$BtagFileMap").c_str());
  QCD_btagMap            = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$BtagMap").c_str());
  QCD_bvetoFileMap       = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$BvetoFileMap").c_str());
  QCD_bvetoMap           = TString(xmlReader->retrieveChar("AnalysisOptions$ObjectDefinition$QCD$BvetoMap").c_str());

  std::istringstream QCD_triggerNameIStr(QCD_triggerNameStr);
  std::string stqcd;
  while (std::getline(QCD_triggerNameIStr, stqcd, ',')) {
    JS_triggers.push_back(stqcd);
  }

  //Booking options
  Info(whereAmI, Form(" - Booking" ));
  BookElBase   = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$ElBase");
  BookElSignal = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$ElSignal");
  BookMuBase   = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$MuBase");
  BookMuSignal = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$MuSignal");
  BookPhSignal = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$PhSignal");
  BookJetSignal = xmlReader->retrieveInt("AnalysisOptions$ObjectDefinition$Booking$JetSignal");
  
  //Object options
  Info(whereAmI, Form(" - Objects" ));
  usePhotons   = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Objects$usePhotons");
  useTrueJets  = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Objects$useTrueJets");
  isoSignalLep = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$Objects$isoSignalLep");
 
  //--- To know if we gonna smear a truth sample                                         
  doTTR = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$SmearTruth$Enable");
  ttr_mu = xmlReader->retrieveInt("AnalysisOptions$GeneralSettings$SmearTruth$Mu");
  ttr_eleWP = (short) xmlReader->retrieveInt("AnalysisOptions$GeneralSettings$SmearTruth$ElectronWP");
  ttr_recjetflav = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$SmearTruth$RecJetFlav");

  TString st_ttr_metWP = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$SmearTruth$MetWP"));

  //only these four configured at the moment!                                           
  // if( st_ttr_metWP=="mu60" )       ttr_metWP =  METSmear::MissingETSmearing::mu60;
  // else if( st_ttr_metWP=="mu80" )  ttr_metWP =  METSmear::MissingETSmearing::mu80;
  // else if( st_ttr_metWP=="mu140" ) ttr_metWP =  METSmear::MissingETSmearing::mu140;
  // else                             ttr_metWP =  METSmear::MissingETSmearing::mu200;


  delete xmlReader;

  //--- Systematics
  //...
  
  Info(whereAmI, Form("--------------------------------------------\n\n") );
}

EL::StatusCode chorizo :: initialize ()
{
  if(debug)  Info("initialize()", "HERE");

  //Start Clock
  watch.Start();

  m_event = wk()->xaodEvent();
  //  TEvent::kBranchAccess

  m_store = new xAOD::TStore(); 

  //Read XML options
  ReadXML();

  //Initialize event counter
  m_eventCounter=0;
  m_metwarnCounter=0;
  m_pdfwarnCounter=0;


  //--- Create the output tree
  // out_TFile = (TFile*) wk()->getOutputFile ("output");
  // m_atree = new TTree ("AnalysisTree", "analysis output tree");
  // m_atree->SetDirectory (out_TFile);

  //Book the output Tree
  //bookTree();

  //Initialize variables
  InitVars();

#ifdef TRIGGERTEST
  if(!this->isTruth){
    //--- Trigger Decision
    // The configuration tool.
    if(!tool_trigconfig)
      tool_trigconfig = new TrigConf::xAODConfigTool ("MYxAODConfigTool");
    CHECK( tool_trigconfig->initialize() ); 
    
    ToolHandle<TrigConf::ITrigConfigTool> configHandle(tool_trigconfig);
    
    // The decision tool  
    if(!tool_trigdec)
      tool_trigdec = new Trig::TrigDecisionTool("MYTrigDecTool");
    CHECK( tool_trigdec->setProperty("ConfigTool",configHandle) );
    CHECK( tool_trigdec->setProperty("TrigDecisionKey","xTrigDecision") );
    //   tool_trigdec->setProperty("OutputLevel", MSG::VERBOSE);
    CHECK( tool_trigdec->initialize() );
    
    if(!tool_trig_match_el)    
      tool_trig_match_el = new Trig::TrigEgammaMatchingTool("TrigEgammaMatchingTool");
    CHECK( tool_trig_match_el->setProperty( "TriggerTool", ToolHandle< Trig::TrigDecisionTool >( tool_trigdec ) ));
    CHECK( tool_trig_match_el->initialize());
    
    if(!tool_trig_match_mu)    
      tool_trig_match_mu = new Trig::TrigMuonMatching("TrigMuonMatchingTool");
    CHECK( tool_trig_match_mu->setProperty( "TriggerTool", ToolHandle< Trig::TrigDecisionTool >( tool_trigdec ) ));
    CHECK( tool_trig_match_mu->initialize());
   
    if(!tool_trig_match_ph)    
      tool_trig_match_ph = new Trig::TrigEgammaMatchingTool("TrigEgammaMatchingTool");
    CHECK( tool_trig_match_ph->setProperty( "TriggerTool", ToolHandle< Trig::TrigDecisionTool >( tool_trigdec ) ));
    CHECK( tool_trig_match_ph->initialize());    
  }
#endif
  
  
  // get event info needed for tool's config
  const xAOD::EventInfo* eventInfo = 0;
  CHECK( m_event->retrieve( eventInfo, "EventInfo") );
  
  isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  
  //--- Corrected mct calculator
  tool_mct = new TMctLib();

  //*** Initialize all CP tools
  std::string maindir = getenv("ROOTCOREBIN");
  maindir = maindir + "/data/";

  ST::SettingDataSource datasource = !this->isMC ? ST::Data : (this->isAtlfast ? ST::AtlfastII : ST::FullSim);
  //  m_isderived = isDerived(); //should be filled in already!

  //--- SUSYTools
  if(!this->isTruth){
    
    tool_st = new ST::SUSYObjDef_xAOD( "SUSYObjDef_xAOD" );
    CHECK(tool_st->setProperty("DataSource", datasource) );
    //  CHECK(tool_st->setProperty("IsDerived", m_isderived) );
    //  CHECK(tool_st->setProperty("Is8TeV", this->is8TeV) );
    CHECK(tool_st->setProperty("Is25ns", false));

    if(JetCollection.Contains("LCTopo"))
      CHECK( tool_st->setProperty( "JetInputType",   xAOD::JetInput::LCTopo ));
    else if(JetCollection.Contains("EMTopo"))
      CHECK( tool_st->setProperty( "JetInputType",   xAOD::JetInput::EMTopo ));
    
    //redefine isolation WPs (if needed)
    if(El_isoWP=="None"){      
      El_isoWP = "LooseTrackOnly";
      noElIso = true;
    }
    if(Mu_isoWP=="None"){      
      Mu_isoWP = "LooseTrackOnly";
      noMuIso = true;
    }
    if(Ph_isoWP=="None"){      
      Ph_isoWP = "LooseTrackOnly";
      noPhIso = true;
    }
    
    CHECK( tool_st->setProperty("EleIdBaseline",El_baseID) ); 
    CHECK( tool_st->setProperty("EleId",El_ID) ); 
    CHECK( tool_st->setProperty("EleIsoWP",El_isoWP) ); 

    if(Mu_ID=="Loose")       CHECK( tool_st->setProperty("MuId", xAOD::Muon::Loose));
    else if(Mu_ID=="Medium") CHECK( tool_st->setProperty("MuId", xAOD::Muon::Medium));
    else if(Mu_ID=="Tight")  CHECK( tool_st->setProperty("MuId", xAOD::Muon::Tight));
    else                     CHECK( tool_st->setProperty("MuId", xAOD::Muon::VeryLoose));
    CHECK( tool_st->setProperty("MuIsoWP",Mu_isoWP) ); 

    if (usePhotons){
      CHECK( tool_st->setProperty("PhotonId",Ph_ID) );
      CHECK( tool_st->setProperty("PhotonIsoWP",Ph_isoWP) );
    }

    CHECK( tool_st->setProperty("RequireIsoSignal", isoSignalLep));
    
    // Set Btagging WPs
    CHECK( tool_st->setProperty("BtagWP", Jet_Btag_WP.Data() ) );
    CHECK( tool_st->setProperty("BtagWP_OR", Jet_Btag_WP_OR.Data() ) );
    
    // Set to true for DxAOD, false for primary xAOD
    CHECK(tool_st->setProperty("DoJetAreaCalib",m_isderived) );
    // Set to false if not doing JetAreaCalib
    CHECK(tool_st->setProperty("DoJetGSCCalib",m_isderived) );
    // Set 0 for 14NP, 1,2,3,4 for 3NP sets                                                                                   
    CHECK(tool_st->setProperty("JESNuisanceParameterSet",syst_JESNPset) ); 
    
    // if(!Met_doMuons)      CHECK(tool_st->setProperty("METMuonTerm", "")); //No MuonTerm default
    // if(!Met_doRefGamma)   CHECK(tool_st->setProperty("METGammaTerm","")); //No GammaTerm default
    // if(!Met_doRefTau)     CHECK(tool_st->setProperty("METTauTerm", "")); //No TauTerm default
    
    CHECK( tool_st->SUSYToolsInit() );
    CHECK( tool_st->initialize() );
    tool_st->msg().setLevel( MSG::ERROR ); //set message level 

    if(syst_CP.size()){
      // Tell the SUSYObjDef_xAOD which variation to apply
      if( tool_st->applySystematicVariation( syst_CP ) != CP::SystematicCode::Ok){
	Error("initialize()", "Cannot configure SUSYTools for systematic var. %s", (syst_CP.name()).c_str() );
      }else{
	if(debug) Info("initialize()", "Variation \"%s\" configured...", (syst_CP.name()).c_str() );
      }
    }
  }


  //--- Overlap Removal
  tool_or = new OverlapRemovalTool("OverlapRemovalTool");
  // Turn on the object links for debugging
  CHECK( tool_or->setProperty("InputLabel", "baseline") );
  CHECK( tool_or->setProperty("OverlapLabel", "overlaps") );
  CHECK( tool_or->setProperty("LinkOverlapObjects", false) );
  tool_or->msg().setLevel(MSG::INFO);
  CHECK( tool_or->initialize() );
  
  
  //--- B-tagging
  TString JetTagCollection = JetCollection+"Jets";
  string FlvTagCutFN = "xAODBTaggingEfficiency/13TeV/2015-PreRecomm-13TeV-MC12-CDI_August3-v1.root";
  tool_bsel70 = new BTaggingSelectionTool("BTagSelEMTopoJets70");
  CHECK( tool_bsel70->setProperty("MaxEta",2.5) );
  CHECK( tool_bsel70->setProperty("MinPt",20000.) );
  CHECK( tool_bsel70->setProperty("FlvTagCutDefinitionsFileName", FlvTagCutFN) );
  CHECK( tool_bsel70->setProperty("TaggerName","MV2c20") );
  CHECK( tool_bsel70->setProperty("OperatingPoint","FlatBEff_70") );
  CHECK( tool_bsel70->setProperty("JetAuthor",JetTagCollection.Data()) );
  CHECK( tool_bsel70->initialize() );

  tool_bsel77 = new BTaggingSelectionTool("BTagSelEMTopoJets77");
  CHECK( tool_bsel77->setProperty("MaxEta",2.5) );
  CHECK( tool_bsel77->setProperty("MinPt",20000.) );
  CHECK( tool_bsel77->setProperty("FlvTagCutDefinitionsFileName", FlvTagCutFN) );
  CHECK( tool_bsel77->setProperty("TaggerName","MV2c20") );
  CHECK( tool_bsel77->setProperty("OperatingPoint","FlatBEff_77") );
  CHECK( tool_bsel77->setProperty("JetAuthor",JetTagCollection.Data()) );
  CHECK( tool_bsel77->initialize() );

  tool_bsel85 = new BTaggingSelectionTool("BTagSelEMTopoJets85");
  CHECK( tool_bsel85->setProperty("MaxEta",2.5) );
  CHECK( tool_bsel85->setProperty("MinPt",20000.) );
  CHECK( tool_bsel85->setProperty("FlvTagCutDefinitionsFileName", FlvTagCutFN) );
  CHECK( tool_bsel85->setProperty("TaggerName","MV2c20") );
  CHECK( tool_bsel85->setProperty("OperatingPoint","FlatBEff_85") );
  CHECK( tool_bsel85->setProperty("JetAuthor",JetTagCollection.Data()) );
  CHECK( tool_bsel85->initialize() );

  tool_btag70 = new BTaggingEfficiencyTool("BTagSF70_EMTopoJets");
  CHECK( tool_btag70->setProperty("TaggerName",          Jet_Tagger.Data()) );
  CHECK( tool_btag70->setProperty("OperatingPoint",      "-0_0436") ); //"FixedCutBEff_70") );
  CHECK( tool_btag70->setProperty("JetAuthor",           JetTagCollection.Data()) );
  CHECK( tool_btag70->setProperty("ScaleFactorFileName", FlvTagCutFN) );
  //CHECK( tool_btag70->setProperty("SystematicsStrategy","Envelope") );
  CHECK( tool_btag70->initialize() ); 
  tool_btag70->msg().setLevel( MSG::FATAL );  
  
  tool_btag77 = new BTaggingEfficiencyTool("BTagSF77_EMTopoJets");
  CHECK( tool_btag77->setProperty("TaggerName",          Jet_Tagger.Data()) );
  CHECK( tool_btag77->setProperty("OperatingPoint",      "-0_4434") ); //"FixedCutBEff_77") );
  CHECK( tool_btag77->setProperty("JetAuthor",           JetTagCollection.Data() ) );
  CHECK( tool_btag77->setProperty("ScaleFactorFileName", FlvTagCutFN) );
  //CHECK( tool_btag77->setProperty("SystematicsStrategy","Envelope") );  
  CHECK( tool_btag77->initialize() );
  tool_btag77->msg().setLevel( MSG::FATAL );  
  
  tool_btag85 = new BTaggingEfficiencyTool("BTagSF85_EMTopoJets");
  CHECK( tool_btag85->setProperty("TaggerName",          Jet_Tagger.Data()) );
  CHECK( tool_btag85->setProperty("OperatingPoint",      "-0_7887") ); //FixedCutBEff_85") );
  CHECK( tool_btag85->setProperty("JetAuthor",           JetTagCollection.Data() ) ); 
  CHECK( tool_btag85->setProperty("ScaleFactorFileName", FlvTagCutFN) );
  //  CHECK( tool_btag85->setProperty("SystematicsStrategy","Envelope") );  
  CHECK( tool_btag85->initialize() );
  tool_btag85->msg().setLevel( MSG::FATAL );

   
  tool_btag_truth1 = new BTagEfficiencyReader();
  tool_btag_truth2 = new BTagEfficiencyReader();
  tool_btag_truth3 = new BTagEfficiencyReader();
  tool_btag_truth1->Initialize("FlatBEff", "1D", "MV2c20", "70");
  tool_btag_truth2->Initialize("FlatBEff", "1D", "MV2c20", "77");
  tool_btag_truth3->Initialize("FlatBEff", "1D", "MV2c20", "80");  //85 is not supported yet!

  //--- MCTruthClassifier
  if (isMC){    
    tool_mcc = new MCTruthClassifier("MCTruthClassifier");
    CHECK( tool_mcc->initialize() );
  }

  //--- truth jet labeling
  // tool_jetlabel = new Analysis::JetQuarkLabel("JetLabelTool");
  // CHECK( tool_jetlabel->setProperty("McEventCollection","TruthEvents") );
  // CHECK( tool_jetlabel->initialize() );
  
  //--- Jet smearing tool
  SUSY::SmearingType gsmtype = SUSY::SmearingType::optimal;
  if(QCD_SmearType=="high") gsmtype = SUSY::SmearingType::high;
  else if(QCD_SmearType=="low") gsmtype = SUSY::SmearingType::low;
  else if(QCD_SmearType=="none" || QCD_SmearType=="") gsmtype = SUSY::SmearingType::none;

  SUSY::SmearingType mstype = SUSY::SmearingType::optimal;
  if(QCD_SmearMeanShift=="high") mstype = SUSY::SmearingType::high;
  else if(QCD_SmearMeanShift=="low") mstype = SUSY::SmearingType::low;
  else if(QCD_SmearMeanShift=="none" || QCD_SmearMeanShift=="") mstype = SUSY::SmearingType::none;

  tool_jsmear = new SUSY::JetMCSmearingTool("JStool");
  CHECK( tool_jsmear->setProperty("NumberOfSmearedEvents",(unsigned int)QCD_SmearedEvents) );
  CHECK( tool_jsmear->setProperty("DoBJetSmearing",QCD_SmearUseBweight) );
  CHECK( tool_jsmear->setProperty("BtagWeight",QCD_SmearBtagWeight) );
  CHECK( tool_jsmear->setProperty("UseTailWeights",true) );
  CHECK( tool_jsmear->setProperty("DoGaussianCoreSmearing",QCD_SmearExtraSmr) );
  CHECK( tool_jsmear->setProperty("GaussianCoreSmearingType", gsmtype) );
  CHECK( tool_jsmear->setProperty("ShiftMeanType",mstype) );
  CHECK( tool_jsmear->setProperty("DoPhiSmearing",QCD_DoPhiSmearing) );
  CHECK( tool_jsmear->initialize() );

  TFile* bVetoJetFile = new TFile(maindir+"/JetSmearing/"+QCD_bvetoFileMap);
  TH2F* bVetoJetResponse = (TH2F*)bVetoJetFile->Get(QCD_bvetoMap);
  TFile* bTagJetFile = new TFile(maindir+"/JetSmearing/"+QCD_btagFileMap);
  TH2F* bTagJetResponse = (TH2F*)bTagJetFile->Get(QCD_btagMap);
  tool_jsmear->SetResponseMaps(bVetoJetResponse,bTagJetResponse);

  //--- Isolation tools
  // Setup standard working point for photons, electrons and muons
  iso_1 = new CP::IsolationSelectionTool("iso_1");
  CHECK( iso_1->setProperty("ElectronWP","GradientLoose") );
  CHECK( iso_1->setProperty("MuonWP","GradientLoose") );
  if (usePhotons) CHECK( iso_1->setProperty("PhotonWP","Cone20") );
  CHECK( iso_1->initialize() );

  iso_2 = new CP::IsolationSelectionTool("iso_2");
  CHECK( iso_2->setProperty("ElectronWP","Loose") );
  CHECK( iso_2->setProperty("MuonWP","Loose") );
  if (usePhotons) CHECK( iso_2->setProperty("PhotonWP","Cone40CaloOnly") );
  CHECK( iso_2->initialize() );

  iso_3 = new CP::IsolationSelectionTool("iso_3");
  CHECK( iso_3->setProperty("ElectronWP","LooseTrackOnly") );
  CHECK( iso_3->setProperty("MuonWP","LooseTrackOnly") );
  if (usePhotons) CHECK( iso_3->setProperty("PhotonWP","Cone40") );
  CHECK( iso_3->initialize() );

  iso_4 = new CP::IsolationSelectionTool("iso_4");
  CHECK( iso_4->setProperty("ElectronWP","Gradient") );
  CHECK( iso_4->setProperty("MuonWP","Gradient") );
  //if (usePhotons) CHECK( iso_4->setProperty("PhotonWP","Cone20") );
  CHECK( iso_4->initialize() );

  iso_5 = new CP::IsolationSelectionTool("iso_5");
  CHECK( iso_5->setProperty("ElectronWP","Tight") );
  CHECK( iso_5->setProperty("MuonWP","Tight") );
  //if (usePhotons) CHECK( iso_5->setProperty("PhotonWP","Cone20") );
  CHECK( iso_5->initialize() );





  //--- Pileup Reweighting
  // trick: path in the tool name so it gets saved to the desired place
  //  TString purw_name = Form("myPURWtool.%s/%d", PURW_Folder.Data(), (int)wk()->metaData()->getDouble( "DSID" ));
  TString purw_name = Form("myPURWtool.%s/%d",   TString(maindir + "/SusyAnalysis/PURW/").Data(), (int)wk()->metaData()->getDouble( "DSID" )); //readmode
  //if(PURW_Folder.IsWhitespace())
  PURW_Folder = maindir + "/SusyAnalysis/PURW/";

  if(this->isMC && genPUfile){ 
    purw_name = Form("myPURWtool.%s/%d",   PURW_Folder.Data(), (int)wk()->metaData()->getDouble( "DSID" )); //write mode
  }
  tool_purw = new CP::PileupReweightingTool(purw_name.Data());
  if (this->isMC){
    if(genPUfile && !doPUTree){ //--- Generate the pileup root files
      CHECK( tool_purw->initialize() );
    }
    else if(applyPURW || doPUTree){ //--- Apply the weights found after generating the pileup root files
      std::vector<std::string> prwFiles;	 
      std::vector<std::string> lumiFiles;	 
      
      //--- read the dataset number from the TTree
      Info("initialize()", Form("Reading PileupReweighting file : %i.prw.root",  eventInfo->mcChannelNumber()) );

      TString prwfile=PURW_Folder+Form("%i",  eventInfo->mcChannelNumber())+".prw.root";
      std::ifstream test_prwfile(prwfile);
      
      if (test_prwfile.good()) {
	isPUfile=true;
	prwFiles.push_back(prwfile.Data());
	CHECK( tool_purw->setProperty("ConfigFiles",prwFiles) );
	lumiFiles.push_back((PURW_Folder+PURW_IlumicalcFile).Data());      
	CHECK( tool_purw->setProperty("LumiCalcFiles", lumiFiles) );
	CHECK( tool_purw->setProperty("UnrepresentedDataAction",2) );

	CHECK( tool_purw->setProperty("DataScaleFactor", 1./1.16) );
	if (this->syst_PU == pileupErr::PileupLow)        
	  CHECK( tool_purw->setProperty("DataScaleFactor", 1./1.23) );
	else if(this->syst_PU == pileupErr::PileupHigh) 
	  CHECK( tool_purw->setProperty("DataScaleFactor", 1.) );
	

	CHECK( tool_purw->initialize() );
      } 
      
      else {
	isPUfile=false;
	prwfile=PURW_Folder+"410000.prw.root";
	prwFiles.push_back(prwfile.Data());
	CHECK( tool_purw->setProperty("ConfigFiles",prwFiles) );
	CHECK( tool_purw->setProperty("DefaultChannel",410000) );      
	lumiFiles.push_back((PURW_Folder+PURW_IlumicalcFile).Data());      
	CHECK( tool_purw->setProperty("LumiCalcFiles", lumiFiles) );
	CHECK( tool_purw->setProperty("UnrepresentedDataAction",2) );
	
	CHECK( tool_purw->setProperty("DataScaleFactor", 1./1.16) );
	if (this->syst_PU == pileupErr::PileupLow)        
	  CHECK( tool_purw->setProperty("DataScaleFactor", 1./1.23) );
	else if(this->syst_PU == pileupErr::PileupHigh) 
	  CHECK( tool_purw->setProperty("DataScaleFactor", 1.) );
	
	CHECK( tool_purw->initialize() );
      }      
    }
  }

  
  //--- GRL
  tool_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> grlist;

  std::istringstream GRLIStr(GRLxmlFile.Data());
  std::string g;
  while (std::getline(GRLIStr, g, ',')) {
    grlist.push_back(maindir+"/../../SusyAnalysis/share/GRL/"+g);
  }

  CHECK( tool_grl->setProperty( "GoodRunsListVec", grlist) );
  CHECK( tool_grl->setProperty("PassThrough", false) );
  CHECK( tool_grl->initialize() );
  
 
  //--- TileTripReader
  tool_tileTrip = new Root::TTileTripReader("TileTripReader");
  std::string TileTripReader_file = "CompleteTripList_2011-2012.root";
  CHECK( tool_tileTrip->setTripFile((maindir + "TileTripReader/" + TileTripReader_file).data()) );
      
  //--- JetTileCorrectionTool
#ifdef TILETEST
  tool_jettile = new CP::JetTileCorrectionTool("JetTileCorrectionTool");
  std::string parfile="param_test.root";
  // @param part tile partition: LBA=0, LBC=1, EBA=2, EBC=3
  std::vector<std::string> dead_modules = {"0 1","0 10"};
  
  CHECK( tool_jettile->setProperty("ParFile",maindir+"JetTileCorrection/"+parfile) );
  CHECK( tool_jettile->setProperty("DeadModules",dead_modules) );
  CHECK( tool_jettile->initializeTool(tool_tileTrip) );
#endif 

  
  //--- PDF reweighting
  // LHAPDF::setPDFPath( gSystem->Getenv("LHAPATH") );
  // Info("initialize()", Form(" -- LHAPATH = %s", gSystem->Getenv("LHAPATH")) );
  LHAPDF::setPaths( gSystem->Getenv("LHAPDF_DATA_PATH") );
  Info("initialize()", Form(" -- LHAPATH = %s", gSystem->Getenv("LHAPDF_DATA_PATH")) );
  
  TString input_pdfset    = "CT10"; // 10800 //CHECK_ME this was not CT10as but CT10 !!
  int input_pdfset_member = 0;     
  if (this->isSignal || this->isTop) {
    input_pdfset = "cteq6l1"; //10042; // cteq6L1.LHgrid 
    input_pdfset_member = 0;  
  }

  // scale beam energy from 8 TeV to 13 TeV
  if(doPDFrw && this->isMC){
    beamE_from = (float) wk()->metaData()->getDouble("ebeam", 4.); //default is 8TeV!
    m_PDF = LHAPDF::mkPDF(input_pdfset.Data(), input_pdfset_member); //LHAPDF6
  }

  // initialize and configure the jet cleaning tool
  tool_jClean = new JetCleaningTool("JetCleaning");
  //  tool_jClean->msg().setLevel( MSG::DEBUG ); 
  CHECK( tool_jClean->setProperty( "CutLevel", "LooseBad") );
  CHECK( tool_jClean->initialize() );
  

  //--- number of events
  Info("initialize()", Form("Total number of events = %lli", m_event->getEntries() ));
  watch.Stop();
  Info("initialize()", Form("Init time = %f", watch.CpuTime()));
  watch.Start();
  
  return EL::StatusCode::SUCCESS;
  

}

void chorizo :: printSystList(){
  //Warning: It has to happen *after* all tools' initialization!

  cout << bold("\n\n\n List of recommended/implemented systematics") << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  
  vector<ST::SystInfo> systInfoList = tool_st->getSystInfoList();
  for(const auto& sysInfo : systInfoList){
    const CP::SystematicSet& sys = sysInfo.systset;
    //    Info("", (sys.name()).c_str());
    
    string affectedType;
    switch(sysInfo.affectsType) {
    case ST::SystObjType::Unknown  : affectedType = "UNKNOWN";  break;
    case ST::SystObjType::Jet      : affectedType = "JET";      break;
    case ST::SystObjType::Egamma   : affectedType = "EGAMMA";   break;
    case ST::SystObjType::Electron : affectedType = "ELECTRON"; break;
    case ST::SystObjType::Photon   : affectedType = "PHOTON";   break;
    case ST::SystObjType::Muon     : affectedType = "MUON";     break;
    case ST::SystObjType::Tau      : affectedType = "TAU";      break;
    case ST::SystObjType::BTag     : affectedType = "BTAG";     break;
    case ST::SystObjType::MET_TST      : affectedType = "MET_TST";      break;
    }
    
    cout <<  sys.name() << "    : affects " << ( sysInfo.affectsWeights ? "weights " : "" ) << ( sysInfo.affectsKinematics ? "kinematics " : "" ) << "  for " << affectedType << endl;
  }
  
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;

}

void chorizo :: loadMetaData(){

  if(debug)  Info("loadMetaData()", "HERE");

  if(!isMC) return;

  meta_xsec        = wk()->metaData()->getDouble( SH::MetaFields::crossSection );
  meta_xsec_relunc = wk()->metaData()->getDouble( SH::MetaFields::crossSectionRelUncertainty );
  meta_kfactor     = wk()->metaData()->getDouble( SH::MetaFields::kfactor );
  meta_feff        = wk()->metaData()->getDouble( SH::MetaFields::filterEfficiency );
  //  meta_nsim        = wk()->metaData()->getDouble( SH::MetaFields::numEvents );
  meta_lumi        = wk()->metaData()->getDouble( SH::MetaFields::lumi );
  //  meta_id = (int)wk()->metaData()->getDouble( "DSID" ));

  //protect some samples for kfactor=0
  if(meta_kfactor==0) meta_kfactor=1.;

}

EL::StatusCode chorizo :: nextEvent(){
  
  if(debug)  Info("nextEvent()", "HERE");

  //*** clean event (store, pointers, etc...) before leaving successfully... ***
  
  // Clear View container
  if(m_goodJets)
    m_goodJets->clear();

  if(m_smdJets)
    m_smdJets->clear();

  if(m_inv_el)
    m_inv_el->clear();

  if(m_inv_mu)
    m_inv_mu->clear();

  if(m_inv_ph)
    m_inv_ph->clear();

  // Clear transient store
  m_store->clear();

  return EL::StatusCode::SUCCESS;   
}


EL::StatusCode chorizo :: execute ()
{
  if(debug)  Info("execute()", "HERE");

  if(this->isTruth) //truth derivations
    return loop_truth();
  
  return loop();
}


EL::StatusCode chorizo :: loop ()
{
  if(debug)  Info("loop()", "HERE");
  
#ifdef PROFCODE
  if(m_eventCounter!=0)
    CALLGRIND_START_INSTRUMENTATION;
#endif  

  ofstream myfile; //produces a text file with event information
  string ocfile; 
  if (doCutFlow){
    ocfile = string(gSystem->Getenv("ROOTCOREBIN"))+"/cutflow.txt";
    myfile.open (ocfile.c_str(), ios::app);
  }
  if(systListOnly){  //just print systematics list and leave!
    this->printSystList();
    wk()->skipEvent();
  }

  InitVars();

  if ( m_eventCounter%1000==0 ){ 
    printf("\r %6d/%lld\t[%3.0f]",m_eventCounter,m_event->getEntries(),100*m_eventCounter/float(m_event->getEntries()));
    fflush(stdout);
  }

  m_eventCounter++;

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  CHECK( m_event->retrieve( eventInfo, "EventInfo") );

  //  loadMetaData();

  RunNumber = eventInfo->runNumber();
  mc_channel_number = (isMC ? eventInfo->mcChannelNumber() : 0); 
  EventNumber = eventInfo->eventNumber();

  //EventList
  if(m_eventList.size()){
    if(isMC && !inEventList(mc_channel_number, EventNumber) ){
      return nextEvent();
    } 
    if(!isMC && !inEventList(RunNumber, EventNumber) ){
      return nextEvent();
    } 
  }
   

  //--- Analysis Code 
  //--- 
  if (doCutFlow) myfile << "EventNumber: " << EventNumber  << " \n";
  
  lb = eventInfo->lumiBlock();
  bcid = eventInfo->bcid();
  averageIntPerXing = ( isMC ? eventInfo->averageInteractionsPerCrossing() : tool_purw->getLumiBlockMu( *eventInfo ) );

  
  //PURW
  // if(isMC && applyPURW)
  //   CHECK( tool_purw->apply(*eventInfo) );  //it does already the filling in 'ConfigMode'
  //--- Generate Pileup file??
  if (genPUfile && isMC){
    if (RunNumber==0){
      Info("loop()", Form("Skipping event %d because RunNumber=0 !!", EventNumber));
      return nextEvent();
    }
    
    if (!doPUTree)         
      return nextEvent();
    //if (isMC && applyPURW) pileup_w = tool_purw->GetCombinedWeight(222500, 410000, averageIntPerXing);    
    if (isMC && applyPURW) pileup_w = tool_purw->GetCombinedWeight(RunNumber, mc_channel_number, averageIntPerXing);      
    return nextEvent();
  }

  //  -- Retrieve Truth containers
  m_truthE= 0;
  m_truthP= 0;
  m_truth_jets= 0;
  if (isMC){

    CHECK( m_event->retrieve( m_truthE,     "TruthEvents" ) );
    CHECK( m_event->retrieve( m_truthP,     "TruthParticles" ) );

    //--- Weights for MC
    
    //--- Weigths for Sherpa, MC@NLO, Acer, ...
    MC_w = eventInfo->mcEventWeight();
    
    //--- PDF reweighting
    xAOD::TruthEventContainer::const_iterator truthE_itr = m_truthE->begin();
    int id1=0; int id2=0;
    int pdfId1, pdfId2, pdf1, pdf2;
    float x1, x2, scalePDF;

    //pdf reweighting
    if(doPDFrw){
      
      //** For newer tags when they come...
      ( *truthE_itr )->pdfInfoParameter(id1, xAOD::TruthEvent::PDGID1);
      ( *truthE_itr )->pdfInfoParameter(id2, xAOD::TruthEvent::PDGID2);
      ( *truthE_itr )->pdfInfoParameter(x1, xAOD::TruthEvent::X1);
      ( *truthE_itr )->pdfInfoParameter(x2, xAOD::TruthEvent::X2);
      ( *truthE_itr )->pdfInfoParameter(pdfId1, xAOD::TruthEvent::PDFID1);
      ( *truthE_itr )->pdfInfoParameter(pdfId2, xAOD::TruthEvent::PDFID2);
      ( *truthE_itr )->pdfInfoParameter(pdf1, xAOD::TruthEvent::PDF1);
      ( *truthE_itr )->pdfInfoParameter(pdf2, xAOD::TruthEvent::PDF2);
      ( *truthE_itr )->pdfInfoParameter(scalePDF, xAOD::TruthEvent::SCALE);
      // ( *truthE_itr )->pdfInfoParameter(pdf_Q, xAOD::TruthEvent::Q);
      // ( *truthE_itr )->pdfInfoParameter(pdf_xf1, xAOD::TruthEvent::XF1);
      // ( *truthE_itr )->pdfInfoParameter(pdf_xf2, xAOD::TruthEvent::XF2);
            
      PDF_w *= getPdfRW((double)beamE_to/beamE_from, (double)(scalePDF*scalePDF), (double)x1, (double)x2, id1, id2);     
      
      if(debug){
	cout << "-------------------------------------" << endl;
	cout << "DEBUG :: rwScale = " << beamE_to/beamE_from << endl; 
	cout << "DEBUG :: scalePDF = " << scalePDF << endl; 
	cout << "DEBUG :: x1 = " << x1 << endl; 
	cout << "DEBUG :: x2 = " << x2 << endl; 
	cout << "DEBUG :: id1 = " << id1 << endl; 
	cout << "DEBUG :: id2 = " << id2 << endl; 
	cout << "DEBUG :: pdfId1 = " << pdfId1 << endl; 
	cout << "DEBUG :: pdfId2 = " << pdfId2 << endl; 
	cout << "DEBUG :: PDF_w = " << PDF_w << endl; 
	cout << "-------------------------------------" << endl;
      }
    }

    //Find Hard Process particles
    int pdgid1 = 0;
    int pdgid2 = 0;
    CHECK(tool_st->FindSusyHP(m_truthP, pdgid1, pdgid2) );
    
    //procID
    if(!m_isderived && pdgid1!=0 && pdgid2!=0) //(just to avoid warnings)
      procID = SUSY::finalState(id1, id2); // get prospino proc ID

    //--- Get sum of the weigths from the slimmed samples //FIX_ME
    this->w_average = this->GetAverageWeight();
    this->w = 1/w_average;

    //---pileup weight 
    if(applyPURW)
       if (isMC) pileup_w = tool_purw->GetCombinedWeight(RunNumber, mc_channel_number, averageIntPerXing);
       //if (isMC) pileup_w = tool_purw->GetCombinedWeight(222510.5, 410000, averageIntPerXing);
    //--- For histograms : combine all the weights in a single variable //CHECK_ME
    if (!doAnaTree) 
      w *= (MC_w*pileup_w);     

  }
  //--- Generator level uncertainites for Znunu+jet Sherpa 
  //    --> scaleVariationWeight
  this->w *= GetGeneratorUncertaintiesSherpa();

  //--- MC vetoes
  if(vetoMCevent() ){
    //vetoed MC event! Just leave...
    return nextEvent();
  }

  if(! passMCor() ){ //remove overlap among filtered samples
    //vetoed MC event! Just leave...
    return nextEvent();
  }  
    
  //------------------------ ttbar reweighting ---------------------------
  if (isMC) {
    if (mc_channel_number==117050 || mc_channel_number==110401) {
      ttbar_weight = this->GetTTbarReweight(Top_truth_pt, Topbar_truth_pt, avTop_truth_pt);
    }     
    truth_n_leptons = this->GetNTruthLeptons();
    
    truth_met_noEle = this->GetTruthEtmiss_noEleTau();	
    truth_n_bjets   = this->GetNTruthB();	
    truth_n_HFjets  = this->GetNTruthHF();	
    
    long int sigSamPdgId=0;
    if( isSignal_WIMP(mc_channel_number) ) sigSamPdgId=1000022; //--- WIMPs
    else if( isSignal_ADD(mc_channel_number) ) sigSamPdgId=5000039; //--- ADD
    
    this->GetTruthShat(sigSamPdgId);
  }

  //----------------------------------------------------------
  //--- ANALYSIS CRITERIA ------------------------------------
  //----------------------------------------------------------

  //--- Vertex selection   (Note: It might not be needed at all in Run2!)
  const xAOD::VertexContainer* vertices(0);
  CHECK( m_event->retrieve( vertices, "PrimaryVertices") );

  for( const auto& vx : *vertices ) {
    if(vx->vertexType() == xAOD::VxType::PriVtx){
      this->isVertexOk = true;
    }
    nVertex++;
  }
  //  this->isVertexOk = (nVertex > 0);


  //--- trigger debugging (check all MET triggers in menu)
#ifdef TRIGGERTEST
  if(m_eventCounter<2){
    //save Trigger metadata
    std::string trigchains="";
    std::vector<string> trigtmp; trigtmp.clear();
    for(const auto& s : TriggerNames){
      auto chainGroup = tool_trigdec->getChainGroup(s);
      for(auto &trig : chainGroup->getListOfTriggers()) {
	trigtmp.push_back(trig);
	trigchains += (trig+",");
      }
    }
    TriggerNames = trigtmp; //overwrite it with fully expanded list
    meta_triggers->SetTitle( trigchains.c_str() );
    
    if(debug){
      Info("loop()", "  ");
      //      Info("loop()", "  MET TRIGGERS IN MENU ");
      Info("loop()", "  TRIGGER MENU ");
      Info("loop()", "--------------------------------");
      Info("loop()", "  L1 ");      
      Info("loop()", "--------------------------------");
      //      auto chainGroup = tool_trigdec->getChainGroup("HLT_xe.*");
      auto chainGroupL1 = tool_trigdec->getChainGroup("L1_.*");
      for(auto &trig : chainGroupL1->getListOfTriggers()) {
	Info("loop()", trig.c_str()); 
      }
      Info("loop()", "--------------------------------");
      Info("loop()", "  HLT ");      
      Info("loop()", "--------------------------------");
      auto chainGroupHLT = tool_trigdec->getChainGroup("HLT_.*");
      for(auto &trig : chainGroupHLT->getListOfTriggers()) {
	Info("loop()", trig.c_str()); 
      }
      Info("loop()", "--------------------------------");
    }
  }

  //--- Trigger 
  if(!this->isQCD){
    for(const auto& chain : TriggerNames)
      this->isTrigger.push_back( (int)tool_trigdec->isPassed(chain) );
  }
  else{ //-- preselection for QCD jet smearing (trigger)
    bool oktrig=false;
    for(const auto& chain : JS_triggers){
      this->isTrigger.push_back( (int)tool_trigdec->isPassed(chain) );
      oktrig |= this->isTrigger.back();
    }
    if(!oktrig)
      return nextEvent();  //skip event
  }
#endif

  //--- GRL
  if( !this->isMC )
    this->isGRL = tool_grl->passRunLB(RunNumber, lb);

  //---   preselection1 for QCD jet smearing data (GRL on data) [time saver]
  if( this->isQCD && !this->isGRL){
    //    output->setFilterPassed(false);
    return nextEvent(); //skip event
  }

  //--- Quality cuts applied only on data
  if (!this->isMC) {
    this->isLarGood = (eventInfo->errorState(xAOD::EventInfo::LAr) != xAOD::EventInfo::Error);
    this->isTileGood = (eventInfo->errorState(xAOD::EventInfo::Tile) != xAOD::EventInfo::Error);
    this->isTileTrip = !tool_tileTrip->checkEvent(RunNumber,  lb,  EventNumber); //--- Does not depend on the definition of the objects.
    this->isCoreFlag = !(eventInfo->eventFlags(xAOD::EventInfo::Core) & 0x40000);
  }

  //--- Preselection flag
  this->passPreselectionCuts = this->isGRL && this->isVertexOk && this->isLarGood && this->isTileGood && this->isCoreFlag && this->isMetCleaned && !this->isTileTrip;
 

  //---   preselection2 for QCD jet smearing data (GRL on data) [time saver]
  if ( this->isQCD  && (!this->passPreselectionCuts) ){ 
    //    output->setFilterPassed(false);
    return nextEvent();
  }

  //--- add Trigger requirement to preselection (if not pseudo-data)
  // if(this->isTrigger.size())
  //   this->passPreselectionCuts &= this->isTrigger[0];

  //--- Non-collision background selection
  if ( this->isNCBG ){
    if ( ! isBeamHalo(RunNumber,EventNumber) ){
      return nextEvent(); // select only events spotted by the beam halo tool
    }
    if(debug) Info("loop()", Form("NCBG run %d\t%d", RunNumber, EventNumber));
  }


  //-- Retrieve objects Containers
  const xAOD::MissingETContainer* cmet_truth;    
  const xAOD::MissingET* mtruth_inv;
  const xAOD::MissingET* mtruth_vis;
  if(this->isMC){
        
    if(useTrueJets)    CHECK( m_event->retrieve( m_truth_jets, "AntiKt4TruthJets" ) );
    CHECK( m_event->retrieve( cmet_truth,   "MET_Truth") );//PROBLEM!!!!!!

    mtruth_inv = (*cmet_truth)["Int"];
    mtruth_vis = (*cmet_truth)["NonInt"];

    sumET_truth     = (*cmet_truth)["Int"]->sumet()*0.001;
    sumET_truth_vmu = (*cmet_truth)["NonInt"]->sumet()*0.001;
  }
  

  // View container provides access to selected jets   (for MET recalculation and JetSmearing)
  m_goodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
  m_smdJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);

  m_inv_el = new xAOD::IParticleContainer(SG::VIEW_ELEMENTS);
  m_inv_mu = new xAOD::IParticleContainer(SG::VIEW_ELEMENTS);
  m_inv_ph = new xAOD::IParticleContainer(SG::VIEW_ELEMENTS);

  // MET container (general)
  xAOD::MissingETContainer* metRFC       = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* metRFCAux = new xAOD::MissingETAuxContainer;
  metRFC->setStore(metRFCAux);

  //--- Get Electrons
  xAOD::ElectronContainer* electrons_sc(0);
  xAOD::ShallowAuxContainer* electrons_scaux(0);
  CHECK( tool_st->GetElectrons(electrons_sc, electrons_scaux, false, El_PreselPtCut, El_PreselEtaCut ) ); //'baseline' decoration

  for(const auto& el_itr : *electrons_sc){

    //redefine iso decoration if needed
    if(noElIso) dec_isol(*el_itr) = true;

    //decorate electron with baseline pt requirements ('signal')
    tool_st->IsSignalElectron( (*el_itr), El_PreselPtCut, El_d0sigcut, El_z0cut);
    //tool_st->IsSignalElectron( (*el_itr));    
    
    //decorate electron with final pt requirements ('final')
    if( (*el_itr).p4().Perp2() > El_RecoPtCut * El_RecoPtCut )
      dec_final(*el_itr) = dec_signal(*el_itr);
    
  }

  //--- Get Muons
  xAOD::MuonContainer* muons_sc(0);
  xAOD::ShallowAuxContainer* muons_scaux(0);
  CHECK( tool_st->GetMuons(muons_sc, muons_scaux, false, Mu_PreselPtCut, Mu_PreselEtaCut ) ); //'baseline' decoration
  for(const auto& mu_itr : *muons_sc){
        
    //redefine iso decoration if needed
    if(noMuIso) dec_isol(*mu_itr) = true;

    //decorate muon with final pt requirements ('final')
    tool_st->IsSignalMuon( *mu_itr, Mu_PreselPtCut, Mu_d0sigcut, Mu_z0cut);  //'signal' decoration.
    //tool_st->IsSignalMuon( *mu_itr);  //'signal' decoration.

    //decorate muon with final pt requirements ('final')
    if( (*mu_itr).p4().Perp2() > Mu_RecoPtCut * Mu_RecoPtCut )
      dec_final(*mu_itr) = dec_signal(*mu_itr); 

  }

  //--- Get Photons
  xAOD::PhotonContainer* photons_sc(0);
  xAOD::ShallowAuxContainer* photons_scaux(0);
  if (usePhotons){
    CHECK( tool_st->GetPhotons(photons_sc, photons_scaux, false, Ph_PreselPtCut, Ph_PreselEtaCut ) ); //'baseline' decoration
    
    for(const auto& ph_itr : *photons_sc){ 
      
      //redefine iso decoration if needed
      if(noPhIso) dec_isol(*ph_itr) = true;
      
      //decorate photon with baseline pt requirements ('signal')
      tool_st->IsSignalPhoton( (*ph_itr), Ph_PreselPtCut);
      //tool_st->IsSignalPhoton( (*ph_itr));    
      
      //decorate photon with final pt requirements ('final')
      if( (*ph_itr).pt() > Ph_RecoPtCut )
	dec_final(*ph_itr) = dec_signal(*ph_itr);
      
    }
  }//usephotons

  
  //--- Get Jets
  std::vector<Particles::Jet> jetCandidates; //intermediate selection jets

  xAOD::JetContainer* jets_sc(0);
  xAOD::ShallowAuxContainer* jets_scaux(0);
  CHECK( tool_st->GetJets(jets_sc, jets_scaux) ); //'baseline' and 'bad' decoration
  
  xAOD::Jet jet;
  for( const auto& jet_itr : *jets_sc ){
    
//#ifdef TILETEST
//    if ( tool_jettile->applyCorrection(*jet_itr) != CP::CorrectionCode::Ok )
//      Error("loop()", "Failed to apply JetTileCorrection!");
//#endif

    //book jets for smearing method
    if(dec_baseline(*jet_itr))
      m_smdJets->push_back(jet_itr);
  }

  //--- Do overlap removal   
  if(doOR){
    if(doORphotons && usePhotons)
      CHECK( tool_st->OverlapRemoval(electrons_sc, muons_sc, jets_sc, photons_sc, m_or_useSigLep, m_or_useIsoLep, m_or_bjetOR) );
    else
      CHECK( tool_st->OverlapRemoval(electrons_sc, muons_sc, jets_sc, m_or_useSigLep, m_or_useIsoLep, m_or_bjetOR) );
  }

  // Apply the overlap removal to all objects (dumb example)
  if (usePhotons) CHECK( tool_or->removeOverlaps(electrons_sc, muons_sc, jets_sc, 0, photons_sc) );
  else CHECK( tool_or->removeOverlaps(electrons_sc, muons_sc, jets_sc, 0, 0) );

  
  //-- Pre-book baseline electrons (after OR)
  bool IsElectron = false; // any good not-overlapping electron in the event?
  int iEl = -1;
  for(const auto& el_itr : *electrons_sc ){

    iEl++;
    if(! dec_baseline(*el_itr)) continue;
    
    //define preselected electron                
    Particle recoElectron;
    recoElectron.SetVector( getTLV( &(*el_itr) ));

/*    
    if (doCutFlow){
      myfile << "baseline electron before OR: \n";      
      myfile << "pt: " << recoElectron.Pt() << " \n";    
      myfile << "eta: " << recoElectron.Eta() << " \n";          
      myfile << "phi: " << recoElectron.Phi() << " \n"; 
    }    
    
    */
    //TEST NEW OR tool
    if(dec_passOR(*el_itr) && dec_failOR(*el_itr)) 
      if(debug) Info("loop()"," Electron passed STor but not ORtool");
    //book not-overlapping electrons
    if (! ((!doOR) || dec_passOR(*el_itr) )) continue;

    eb_N++;  //baseline electrons
    

    recoElectron.index = iEl;
    recoElectron.ptcone20 = acc_ptcone20(*el_itr) * 0.001;
    recoElectron.etcone20 = acc_etcone20(*el_itr) * 0.001;
    recoElectron.ptcone30 = acc_ptcone30(*el_itr) * 0.001;
    recoElectron.etcone30 = acc_etcone30(*el_itr) * 0.001;
    
    if(iso_1->accept(*el_itr)) recoElectron.isoGradientLoose = 1.0;
    if(iso_2->accept(*el_itr)) recoElectron.isoLoose = 1.0;
    if(iso_3->accept(*el_itr)) recoElectron.isoLooseTrackOnly = 1.0;
    if(iso_4->accept(*el_itr)) recoElectron.isoGradient= 1.0;
    if(iso_5->accept(*el_itr)) recoElectron.isoTight = 1.0;

    (*el_itr).passSelection(recoElectron.isTight, "Tight");
    
    const xAOD::TrackParticle* track =  (*el_itr).trackParticle();
    if (track){
      const xAOD::Vertex* pv = tool_st->GetPrimVtx();
      double primvertex_z = pv ? pv->z() : 0;
      recoElectron.d0_sig = fabs(track->d0()) / Amg::error(track->definingParametersCovMatrix(),0);
      recoElectron.z0 = track->z0() + track->vz() - primvertex_z;
    }

    recoElectron.type   = xAOD::TruthHelpers::getParticleTruthType( *el_itr );
    recoElectron.origin = xAOD::TruthHelpers::getParticleTruthOrigin( *el_itr );
    //trigger matching
    std::vector<bool> el_trig_pass;
    for(const auto& t : ElTriggers){
      if(t.substr(0, 4) == "HLT_"){ 
	std::string tsub = t.substr(4);
	el_trig_pass.push_back( tool_trig_match_el->matchHLT( el_itr, tsub ));
      }
      else
	el_trig_pass.push_back( false ); //only check for HLT items at the moment
     
      recoElectron.isTrigMatch |= el_trig_pass.back();
    }
    
    //get electron scale factors
    if(this->isMC){
      //nominal
      recoElectron.SF = tool_st->GetSignalElecSF( *el_itr, El_recoSF, El_idSF, El_triggerSF );
      
      //+1 sys up
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
      if (tool_st->applySystematicVariation( CP::SystematicSet("ELECSFSYS__1up")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	Error("loop()", "Cannot configure SUSYTools for systematic var. ELECSFSYS__1up");
      }
      recoElectron.SFu = tool_st->GetSignalElecSF( *el_itr, El_recoSF, El_idSF, El_triggerSF ); 
      
      //+1 sys down
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){ //reset back to requested systematic!
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
      if (tool_st->applySystematicVariation( CP::SystematicSet("ELECSFSYS__1down")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	Error("loop()", "Cannot configure SUSYTools for systematic var. ELECSFSYS__1down");
      }
      recoElectron.SFd = tool_st->GetSignalElecSF( *el_itr, El_recoSF, El_idSF, El_triggerSF ); 
      
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
      
      eb_SF *= recoElectron.SF;
      eb_SFu *= recoElectron.SFu;
      eb_SFd *= recoElectron.SFd;
     
    }
    
    //save signal electrons
    if( dec_signal(*el_itr) ){
      recoElectrons.push_back(recoElectron);
      e_N++;
      IsElectron=true;
      if(this->isMC){
	e_SF *= recoElectron.SF;
	e_SFu *= recoElectron.SFu;
	e_SFd *= recoElectron.SFd;
      } 
    }
    else{
      electronCandidates.push_back(recoElectron);
    }

  }//electron loop

  //sort the electrons in Pt
  if (electronCandidates.size()>0) std::sort(electronCandidates.begin(), electronCandidates.end()); //non-signal electrons
  if (recoElectrons.size()>0) std::sort(recoElectrons.begin(), recoElectrons.end()); //signal electrons


  //-- Pre-book baseline muons (after OR)
  bool IsMuon = false; // any good not-overlapping muon in the event?
  int iMu=-1;
  auto nCosmicMuons = 0;
  auto nBadMuons = 0;  
  for(const auto& mu_itr : *muons_sc){

    iMu++;  //to keep in sync even if we continue below

    if(isStopTL){
      dec_baseline(*mu_itr) &= ((*mu_itr).muonType() == xAOD::Muon::Combined || (*mu_itr).muonType() == xAOD::Muon::SegmentTagged); //for Sbottom as well?
      dec_signal(*mu_itr)   &= dec_baseline(*mu_itr); //update signal decoration too!
      dec_final(*mu_itr )   &= dec_baseline(*mu_itr); //update final decoration too!
    }

    if(! dec_baseline(*mu_itr) ) continue; //keep baseline objects only

    tool_st->IsCosmicMuon( *mu_itr );  //'cosmic'   decoration  //now after overlap removal!

    this->isCosmic |= dec_cosmic(*mu_itr); //check if at least one cosmic in the event
    
    bool muonok=true;
    if(tool_st->IsBadMuon( *mu_itr )){ //any bad muon before OR? 
      nBadMuons+=1;     
      muonok=false;
    }

    Particle recoMuon;
    recoMuon.SetVector( getTLV( &(*mu_itr) ));	
/*    
    if (doCutFlow){
      myfile << "baseline muon before OR: \n pt: " << recoMuon.Pt() << " \n eta: " << recoMuon.Eta() << 
      myfile << "passOR     : " << (((!doOR) || dec_passOR(*mu_itr)) ? 1 : 0 ) << "\n";
      myfile << "passSignal : " << (dec_signal(*mu_itr) ? 1 : 0 ) << "\n"; //save signal muons (after OR and no-cosmic already)      
    }
*/
    if(doOR && !dec_passOR(*mu_itr)) continue; //pass OR    

    if(tool_st->IsCosmicMuon( *mu_itr )){  //'cosmic' decoration
      nCosmicMuons+=1;  
      muonok=false;
    }
    
    if(!muonok) continue;

    mb_N++;
   

    recoMuon.index = iMu;
    recoMuon.ptcone20 = acc_ptcone20(*mu_itr) * 0.001;
    recoMuon.etcone20 = acc_etcone20(*mu_itr) * 0.001;
    recoMuon.ptcone30 = acc_ptcone30(*mu_itr) * 0.001;
    recoMuon.etcone30 = acc_etcone30(*mu_itr) * 0.001;
    
    const xAOD::TrackParticle* track =  (*mu_itr).primaryTrackParticle();
    if (track){
      const xAOD::Vertex* pv = tool_st->GetPrimVtx();
      double primvertex_z = pv ? pv->z() : 0;
      recoMuon.d0_sig = fabs(track->d0()) /  Amg::error(track->definingParametersCovMatrix(),0);
      recoMuon.z0 = track->z0() + track->vz() - primvertex_z;
    }
    recoMuon.charge   = (float) (*mu_itr).charge();
    //(float)input.primaryTrackParticle()->charge()  in SUSYTools.  //same thing!

    if(iso_1->accept(*mu_itr)) recoMuon.isoGradientLoose = 1.0;
    if(iso_2->accept(*mu_itr)) recoMuon.isoLoose = 1.0;
    if(iso_3->accept(*mu_itr)) recoMuon.isoLooseTrackOnly = 1.0;
    if(iso_4->accept(*mu_itr)) recoMuon.isoGradient= 1.0;
    if(iso_5->accept(*mu_itr)) recoMuon.isoTight = 1.0;

    recoMuon.type   = xAOD::TruthHelpers::getParticleTruthType( *mu_itr );
    recoMuon.origin = xAOD::TruthHelpers::getParticleTruthOrigin( *mu_itr );
    
    //trigger matching
    std::vector<bool> mu_trig_pass;
    for(const auto& t : MuTriggers){
      if(t.substr(0, 4) == "HLT_"){ 
	std::string tsub = t;
	mu_trig_pass.push_back( tool_trig_match_mu->match( mu_itr, tsub ));
      }
      else
	mu_trig_pass.push_back( false ); //only check for HLT items at the moment
     
      recoMuon.isTrigMatch |= mu_trig_pass.back();
    }

    //get muon scale factors
    if(this->isMC){
      //nominal 
      recoMuon.SF = tool_st->GetSignalMuonSF(*mu_itr);
      
      //+1 sys up
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
      if (tool_st->applySystematicVariation( CP::SystematicSet("MUONSFSYS__1up")) != CP::SystematicCode::Ok){
	Error("loop()", "Cannot configure SUSYTools for systematic var. MUONSFSYS__1up");
      }
      recoMuon.SFu = tool_st->GetSignalMuonSF(*mu_itr);
      
      //+1 sys down
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
      if (tool_st->applySystematicVariation( CP::SystematicSet("MUONSFSYS__1down")) != CP::SystematicCode::Ok){
	Error("loop()", "Cannot configure SUSYTools for systematic var. MUONSFSYS__1down");
      }
      recoMuon.SFd = tool_st->GetSignalMuonSF(*mu_itr);
      
      if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	Error("loop()", "Cannot configure SUSYTools for default systematics");
      }
   
      mb_SF *= recoMuon.SF;
      mb_SFu *= recoMuon.SFu;
      mb_SFd *= recoMuon.SFd;
   
    }
    
    //save signal muons
    if( dec_signal(*mu_itr) ){ 
      recoMuons.push_back(recoMuon);
      m_N++;
      IsMuon = true;
      if(this->isMC){
	m_SF *= recoMuon.SF;
	m_SFu *= recoMuon.SFu;
	m_SFd *= recoMuon.SFd;
      }
    }
    else{
      muonCandidates.push_back(recoMuon);
    }

  }//muon loop
  
  //sort the muon candidates in Pt
  if (muonCandidates.size()>0) std::sort(muonCandidates.begin(), muonCandidates.end());
  if (recoMuons.size()>0) std::sort(recoMuons.begin(), recoMuons.end());

  this->isCosmic  = (nCosmicMuons>0);
  this->isBadMuon = (nBadMuons>0);


  //-- Pre-book baseline photons (after OR)
  std::vector<Particle> photonCandidates; //intermediate selection photons
  int iPh = 0;
  ph_N=0; //signal photons
  if (usePhotons){
    for(const auto& ph_itr : *photons_sc ){
      if( dec_baseline(*ph_itr) &&
	  ((!doOR) || (!doORphotons) || dec_passOR(*ph_itr))){
	
	//define preselected photon                
	Particle recoPhoton;
	recoPhoton.SetVector( getTLV( &(*ph_itr) ));
	if (recoPhoton.Pt() < Ph_PreselPtCut/1000.)   continue;
	if (fabs(recoPhoton.Eta()) > Ph_PreselEtaCut) continue;
	
	recoPhoton.id = iPh;
	recoPhoton.ptcone20 = acc_ptcone20(*ph_itr) * 0.001;
	recoPhoton.etcone20 = acc_etcone20(*ph_itr) * 0.001;
	recoPhoton.ptcone30 = acc_ptcone30(*ph_itr) * 0.001;
	recoPhoton.etcone30 = acc_etcone30(*ph_itr) * 0.001;
	(*ph_itr).passSelection(recoPhoton.isTight, "Tight");
	
	if(iso_1->accept(*ph_itr)) recoPhoton.isoCone20 = 1.0;
	if(iso_2->accept(*ph_itr)) recoPhoton.isoCone40CaloOnly = 1.0;
	if(iso_3->accept(*ph_itr)) recoPhoton.isoCone40= 1.0;
	
	
	recoPhoton.type   = xAOD::TruthHelpers::getParticleTruthType( *ph_itr );
	recoPhoton.origin = xAOD::TruthHelpers::getParticleTruthOrigin( *ph_itr );
	
	//trigger matching
	std::vector<bool> ph_trig_pass;
	for(const auto& t : PhTriggers){
	  if(t.substr(0, 4) == "HLT_"){ 
	    std::string tsub = t.substr(4);
	    ph_trig_pass.push_back( tool_trig_match_ph->matchHLT( ph_itr, tsub ));
	  }
	  else
	    ph_trig_pass.push_back( false ); //only check for HLT items at the moment
	  
	  recoPhoton.isTrigMatch |= ph_trig_pass.back();
	}
	//get photon scale factors
	if(this->isMC){
	  //nominal
	  recoPhoton.SF = tool_st->GetSignalPhotonSF( *ph_itr ); //, Ph_recoSF, Ph_idSF, Ph_triggerSF );
	  
	  //+1 sys up
	  if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	    Error("loop()", "Cannot configure SUSYTools for default systematics");
	  }
	  if (tool_st->applySystematicVariation( CP::SystematicSet("PHSFSYS__1up")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	    Error("loop()", "Cannot configure SUSYTools for systematic var. PHSFSYS__1up");
	  }
	  recoPhoton.SFu = tool_st->GetSignalPhotonSF( *ph_itr ); //, Ph_recoSF, Ph_idSF, Ph_triggerSF ); 
	  
	  //+1 sys down
	  if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	    Error("loop()", "Cannot configure SUSYTools for default systematics");
	  }
	  if (tool_st->applySystematicVariation( CP::SystematicSet("PHSFSYS__1down")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	    Error("loop()", "Cannot configure SUSYTools for systematic var. PHSFSYS__1down");
	  }
	  recoPhoton.SFd = tool_st->GetSignalPhotonSF( *ph_itr ); //, Ph_recoSF, Ph_idSF, Ph_triggerSF ); 
	  
	  if( tool_st->applySystematicVariation(this->syst_CP) != CP::SystematicCode::Ok){  //reset back to requested systematic!
	    Error("loop()", "Cannot configure SUSYTools for default systematics");
	  }
	}
	
	photonCandidates.push_back(recoPhoton);
	
	//save signal electrons
	if( dec_signal(*ph_itr) ){
	  recoPhotons.push_back(recoPhoton);
	  ph_N++;
	  if(this->isMC){
	    ph_SF *= recoPhoton.SF;
	    ph_SFu *= recoPhoton.SFu;
	    ph_SFd *= recoPhoton.SFd;
	  }
	}
	
	iPh++;
      }//if baseline 
    }//photon loop
    
    //sort the photons in Pt
    if (photonCandidates.size()>0) std::sort(photonCandidates.begin(), photonCandidates.end());
    if (recoPhotons.size()>0) std::sort(recoPhotons.begin(), recoPhotons.end());
  }//usephotons

  //-- pre-book good jets now (after OR)
  auto jet_itr = jets_sc->begin();
  auto jet_end = jets_sc->end();
  Particles::Jet recoJet;
  int iJet = 0;
  for( ; jet_itr != jet_end; ++jet_itr ){
    
    if( doOR && !dec_passOR(**jet_itr) ) continue;

    bool isbadjet = tool_st->IsBadJet( **jet_itr, Jet_RecoJVTCut); // Change preselEta to recoEta    
    bool isgoodjet = tool_st->IsSignalJet( **jet_itr, Jet_RecoPtCut, Jet_RecoEtaCut, Jet_RecoJVTCut); // Change preselEta to recoEta

    //** Bjet decoration  
    tool_st->IsBJet( **jet_itr );  //rel20 0.77 eff value from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_antikt4topoem_jets

    //flag event if bad jet is found
    this->isBadID |= dec_badjet(**jet_itr);

    if(isStopTL) {
      if( fabs((*jet_itr)->eta()) > Jet_RecoEtaCut ) continue; 
    }

    if( !isgoodjet ) continue; //just book good jets!

    m_goodJets->push_back (*jet_itr);

    recoJet.SetVector( getTLV( &(**jet_itr) ) );
    recoJet.id = iJet;

    TLorentzVector p4c;
    p4c.SetPtEtaPhiE( (*jet_itr)->jetP4(xAOD::JetConstitScaleMomentum).pt()*0.001,
		      (*jet_itr)->jetP4(xAOD::JetConstitScaleMomentum).eta(),
		      (*jet_itr)->jetP4(xAOD::JetConstitScaleMomentum).phi(),
		      (*jet_itr)->jetP4(xAOD::JetConstitScaleMomentum).e()*0.001 );
    recoJet.p4const = p4c;

    //--- Flavor-tagging    
    //from SUSYTools 
    recoJet.isbjet = dec_bjet(**jet_itr);

    if(isMC){
      tool_btag_truth1->setRandomSeed(int( 1e5 + 5000 * fabs((*jet_itr)->eta()))); //set a unique seed for each jet                                                             
      tool_btag_truth2->setRandomSeed(int( 1e5 + 5000 * fabs((*jet_itr)->eta())));
      tool_btag_truth3->setRandomSeed(int( 1e5 + 5000 * fabs((*jet_itr)->eta())));

      recoJet.isbjet_t70 = tool_btag_truth1->performTruthTagging(*jet_itr);
      recoJet.isbjet_t77 = tool_btag_truth2->performTruthTagging(*jet_itr);
      recoJet.isbjet_t80 = tool_btag_truth3->performTruthTagging(*jet_itr);
    }

    //new flat b-tagging eff
    recoJet.isbjet_fb70 = tool_bsel70->accept(*jet_itr);
    recoJet.isbjet_fb77 = tool_bsel77->accept(*jet_itr);
    recoJet.isbjet_fb85 = tool_bsel85->accept(*jet_itr);

    int local_truth_flavor=0;         //for bjets ID
    if ( this->isMC ){
      try{
	local_truth_flavor = (*jet_itr)->getAttribute<int>("PartonTruthLabelID"); 
      }
      catch(...){
	try{
	  local_truth_flavor = (*jet_itr)->getAttribute<int>("TruthLabelID"); 
	}
	catch(...){
	  // try{
	  //   local_truth_flavor = xAOD::jetFlavourLabel(*jet_itr);
	  // }
	  // catch(...){ //else 
	    Warning("loop()","Impossible to get truth label ID. (Set to 0)");
	    local_truth_flavor = 0;
	    //	  }
	}
      }
    }
      
    //local_truth_flavor = (*jet_itr)->getAttribute<int>("PartonTruthLabelID");
    //      local_truth_flavor = (*jet_itr)->getAttribute(xAOD::JetAttribute::JetLabel, local_truth_flavor); //CHECK_ME //zero always . To be fixed in the future?
    //   local_truth_flavor = xAOD::jetFlavourLabel(*jet_itr);
    // }    
  
    const xAOD::BTagging* btag =(*jet_itr)->btagging();
    recoJet.MV1 = btag->MV1_discriminant(); 
    double wmv2=-1;
    if ( !btag->MVx_discriminant("MV2c20", wmv2)) {
      Error(APP_NAME, "Failed to retrieve MV2c20 weight!");
    }
    else{
      recoJet.MV2c20 = wmv2;
    }
    
    recoJet.SV1plusIP3D = btag->SV1plusIP3D_discriminant();
    
    recoJet.IP3D_pb = btag->IP3D_pb(); 
    // recoJet.IP3D_pc = btag->IP3D_pc();    //IP3D_pc not in p187*
    // recoJet.IP3D_pu = btag->IP3D_pu(); 

    recoJet.SV1_pb = btag->SV1_pb(); 
    // recoJet.SV1_pc = btag->SV1_pc();      //SV1_pc not in p187*
    // recoJet.SV1_pu = btag->SV1_pu(); 

    recoJet.FlavorTruth = local_truth_flavor;
    recoJet.bTagOPw = Jet_TaggerOp77.Atof();

    

    //--- Get some other jet attributes
    std::vector<float> sumpttrk_vec;                                                   
    (*jet_itr)->getAttribute(xAOD::JetAttribute::SumPtTrkPt500, sumpttrk_vec); //SumPtTrkPt1000? //CHECK_ME                      

    recoJet.sumPtTrk = ( sumpttrk_vec.size() ? sumpttrk_vec[0]*0.001 : 0.); // assume the 0th vertex is the primary one (in GeV)
    recoJet.chf = recoJet.sumPtTrk/recoJet.Pt(); 
    
    std::vector<int> ntrk_vec;
    (*jet_itr)->getAttribute(xAOD::JetAttribute::NumTrkPt500, ntrk_vec); //NumTrkPt1000? CHECK_ME
    recoJet.nTrk = (ntrk_vec.size() ? ntrk_vec[0] : 0); // assume the 0th vertex is the primary one (in GeV)
    
    //JVT
    recoJet.jvt = acc_jvt(**jet_itr);

    (*jet_itr)->getAttribute(xAOD::JetAttribute::EMFrac, recoJet.emf);
    (*jet_itr)->getAttribute(xAOD::JetAttribute::Timing, recoJet.time);
    (*jet_itr)->getAttribute(xAOD::JetAttribute::FracSamplingMax, recoJet.fsm);
    (*jet_itr)->getAttribute(xAOD::JetAttribute::Width, recoJet.width); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::N90Constituents, recoJet.n90); //CHECK_ME
    (*jet_itr)->getAttribute(xAOD::JetAttribute::HECFrac, recoJet.hecf); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::HECQuality, recoJet.HECQuality); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::LArQuality, recoJet.LArQuality); 

    recoJet.Pt_up = recoJet.Pt();
    recoJet.Pt_down = recoJet.Pt();
    
    if(dumpTile){

      //TopoClusters constituents 
      xAOD::JetConstituentVector vec = (*jet_itr)->getConstituents();                                                                                                                                                                                                   
      xAOD::JetConstituentVector::iterator it = vec.begin();                                                                                                                                                                                                           
      xAOD::JetConstituentVector::iterator itE = vec.end();                                                                                                                                                                                                        
      for( ; it != itE; it++){                                               
	Cluster cl;
	cl.SetVector( getTLV( &(**it) ) );
                                                                                                                                                                                           
	double eEM0 = static_cast<const xAOD::CaloCluster*>((*it)->rawConstituent())->energyBE(0);                                                                                                                                                      
	double eEM1 = static_cast<const xAOD::CaloCluster*>((*it)->rawConstituent())->energyBE(1);                                                                                                                                                           
	double eEM2 = static_cast<const xAOD::CaloCluster*>((*it)->rawConstituent())->energyBE(2);                                                                                                                                                           
	double eEM3 = static_cast<const xAOD::CaloCluster*>((*it)->rawConstituent())->energyBE(3);                                                                                                                                                           

	cl.emf = ((*it)->pt()>0. ? (eEM0+eEM1+eEM2+eEM3)/(*it)->e() : 0.);
	recoJet.clusters.push_back(cl);
      }
    }

    //** ANDREA
    //    recoJet.ptraw = dec_ptraw(**jet_itr);
    //
    // TLorentzVector Truth;
    // Truth.SetPtEtaPhiE(dec_truthpt(**jet_itr),dec_trutheta(**jet_itr),dec_truthphi(**jet_itr),dec_truthe(**jet_itr));
    // recoJet.TruthJet = Truth;

    jetCandidates.push_back(recoJet);
/*    
    if (doCutFlow){
      myfile << "baseline jet after OR: \n";      
      myfile << "pt: " <<  recoJet.Pt() << " \n"; 
      myfile << "eta: " << recoJet.Eta() << " \n";          
      myfile << "phi: " << recoJet.Phi() << " \n"; 
    }    
*/
    if(this->printJet){
      std::cout << "Jet " << iJet << ":" << endl;
      recoJet.PrintInfo();
    }

    iJet++;

  }  //jet loop
 // if (doCutFlow) myfile << "n of baseline jets after OR: " << jetCandidates.size() << " \n"; 
    
  //sort the jet candidates in Pt
  if (jetCandidates.size() > 0) std::sort(jetCandidates.begin(), jetCandidates.end());

  //--- Jet cleaning
  //  Reject events with at least one looser bad jet.
  //  *after lepton overlap removal*
  //from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr178TeV
  this->passPreselectionCuts &= (!this->isBadID);
  

  //--- Get truth electrons 
  if ( this->isMC && !this->isSignal ){

    //truth particles loop
    for(const xAOD::TruthParticle* tp : *m_truthP) {
      
      if( !isStable(tp) || !isElectron(tp) ) continue;  //->isElectron() ? 
      
      Particle electron;
      electron.SetVector( getTLV(tp) ); 
      truthElectrons.push_back(electron);

    }
    //sort truth electrons in pt 
    if (truthElectrons.size()>0) std::sort(truthElectrons.begin(), truthElectrons.end());
  } 
  //--- Get Tracks
  if(tVeto_Enable)
    doTrackVeto(electronCandidates, muonCandidates);


  //--- Print some info if requested!   
  if ( this->printElectron ){
    for (unsigned int iEle=0; iEle<recoElectrons.size(); iEle++){
      recoElectrons[iEle].PrintInfo();
    }
  }
  if ( this->printMuon ){
    for (unsigned int iMu=0; iMu<recoMuons.size(); iMu++){
      recoMuons[iMu].PrintInfo();
    }
  }

  //Get truth jets vectors
  if(isMC && useTrueJets){

    //loop over truth jets & save 'high-pt' jets for later use 
    jb_truth_N=0;
    j_truth_pt1=0.;
    for(const xAOD::Jet* tj : *m_truth_jets) {    

      if ( tj->pt()/1000.0 > j_truth_pt1 ){
	j_truth_pt1 = tj->pt()/1000.0;
	j_truth_eta1 = tj->eta();
      }
      if ( tj->pt() < 10000. ) continue;
     
      if ( tj->pt() > 20000. )
	jb_truth_N++;
      if ( tj->pt() > 50000. && fabs(tj->eta()) < Jet_RecoEtaCut ) j_truth_N++;

    }
  }
  
  //--- Define signal jets
  std::vector<float> calibJets_pt;
  std::vector<float> calibJets_eta;
  std::vector<float> calibJets_MV1;
  std::vector<int> calibJets_flavor;

  int bjet_counter_70fc=0;
  int bjet_counter_77fc=0;
  int bjet_counter_85fc=0;
  
  int bjet_counter=0;

  for (unsigned int iJet=0; iJet < jetCandidates.size(); ++iJet){

    //if ( jetCandidates.at(iJet).Pt() < (Jet_RecoPtCut/1000.) ) continue; //comment
    //if ( fabs(jetCandidates.at(iJet).Eta()) > Jet_RecoEtaCut ) continue; // comment
    
    //if ( jetCandidates.at(iJet).isBTagged_70eff(Jet_Tagger.Data()) && fabs(jetCandidates.at(iJet).Eta())<2.5 )
    if ( jetCandidates.at(iJet).isBTagged_70fc(Jet_Tagger.Data()) && fabs(jetCandidates.at(iJet).Eta())<2.5 )   
      bjet_counter_70fc++;        
    if ( jetCandidates.at(iJet).isBTagged_77fc(Jet_Tagger.Data()) && fabs(jetCandidates.at(iJet).Eta())<2.5 ) 
      bjet_counter_77fc++;
    if ( jetCandidates.at(iJet).isBTagged_85fc(Jet_Tagger.Data()) && fabs(jetCandidates.at(iJet).Eta())<2.5 ) 
      bjet_counter_85fc++;	
    
    if( jetCandidates.at(iJet).isbjet_t70  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nt70++;
    if( jetCandidates.at(iJet).isbjet_t77  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nt77++;
    if( jetCandidates.at(iJet).isbjet_t80  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nt80++;
    
    if( jetCandidates.at(iJet).isbjet_fb70  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nf70++;
    //if( jetCandidates.at(iJet).isbjet_fb77  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nf77++;
    if( jetCandidates.at(iJet).isbjet_fb85  && fabs(jetCandidates.at(iJet).Eta())<2.5) bj_Nf85++;    
    
    if( jetCandidates.at(iJet).isbjet && fabs(jetCandidates.at(iJet).Eta())<2.5) 
      bjet_counter++;
    
    //recoJets.push_back( jetCandidates.at(iJet) ); //Save Signal Jets
    
    //count high pt jet multiplicity
    if(jetCandidates.at(iJet).Pt()>30.){
      j_N30++;
      if(jetCandidates.at(iJet).Pt()>40.){
	j_N40++;
	if(jetCandidates.at(iJet).Pt()>50.){
	  j_N50++; 
	  if(jetCandidates.at(iJet).Pt()>60.){
	    j_N60++;
	    if(jetCandidates.at(iJet).Pt()>80.){
	      j_N80++;
	    }
	  }
	}
      }
    }
    
    if (isStopTL){
      if ( jetCandidates.at(iJet).Pt() < (Jet_RecoPtCut/1000.) ) continue; //comment
    }
    recoJets.push_back( jetCandidates.at(iJet) ); //Save Signal Jets
/*    
    if (doCutFlow){
      myfile << "signal jet: \n";      
      myfile << "pt: "  << jetCandidates.at(iJet).Pt() << " \n"; 
      myfile << "eta: " << jetCandidates.at(iJet).Eta() << " \n";          
      myfile << "phi: " << jetCandidates.at(iJet).Phi() << " \n"; 
    } 

*/
    calibJets_pt.push_back(jetCandidates.at(iJet).Pt()*1000.);
    calibJets_eta.push_back( BtagEta( jetCandidates.at(iJet).Eta() ) ); //eta to be defined in [-2.5,2.5]
    calibJets_MV1.push_back(jetCandidates.at(iJet).MV1);
    calibJets_flavor.push_back(jetCandidates.at(iJet).FlavorTruth);

  }//end of jets loop

  j_N  = recoJets.size();
  bj_N = bjet_counter;
  bj_N_77fc = bjet_counter_77fc; 
  bj_N_85fc = bjet_counter_85fc; 
  bj_N_70fc = bjet_counter_70fc;   
  
  //if (doCutFlow) myfile << "n of signal jets: " << j_N << " \n"; 
  //if (doCutFlow) myfile << "n of signal b-jets: " << bj_N << " \n"; 

  //** btagging weights
  if(isMC){
    btag_weight_total_70fc       = GetBtagSF(m_goodJets, tool_btag70, -0.0436);   
    btag_weight_total_77fc       = GetBtagSF(m_goodJets, tool_btag77, -0.4434);
    btag_weight_total_85fc       = GetBtagSF(m_goodJets, tool_btag85, -0.7887); //CHECK! not to trust for now
  }
  
  //the list of jets to smear for qcd are not the jet-candidates!
  for (unsigned int iJet=0; iJet < jetCandidates.size(); ++iJet){
    if ( jetCandidates.at(iJet).Pt() < QCD_JetsPtPreselection/1000.0/*20.*/ ) continue;
    if ( fabs(jetCandidates.at(iJet).Eta()) > QCD_JetsEtaSelection/*2.8*/ ) continue;
    if ( jetCandidates.at(iJet).Pt() < 50. && fabs(jetCandidates.at(iJet).Eta())<2.4 && jetCandidates.at(iJet).jvf < 0.5) continue;
    seedJets.push_back( jetCandidates.at(iJet) );
  }

  //--- Get MET
  // For the moment only the official flavors are available.
  // Re-computing MET will be possible in the future though, once the METUtility interface is updated!!

  const xAOD::MissingETContainer* cmet_reffinal;
  const xAOD::MissingETContainer* cmet_lhtopo;
  const xAOD::MissingETContainer* cmet_track;

  CHECK( m_event->retrieve( cmet_reffinal, "MET_Reference_AntiKt4EMTopo") ); 
  //  CHECK( m_event->retrieve( cmet_reffinal, "MET_Reference_AntiKt4LCTopo") ); //??
  const xAOD::MissingET* mrf    = (*cmet_reffinal)["FinalClus"];

  const xAOD::MissingET* mtopo(0);
  const xAOD::MissingET* mtrack(0);

  if(!m_isderived){
    CHECK( m_event->retrieve( cmet_lhtopo, "MET_LocHadTopo") ); //not in MC15??
    mtopo  = (*cmet_lhtopo)["LocHadTopo"];
                  

    CHECK( m_event->retrieve( cmet_track, "MET_Track") );
    mtrack = (*cmet_track)["PVTrack_vx0"];
    // mtrack = (*cmet_track)["Track"];
  }                 
         
  //** Met components
  //** see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/Run2xAODMissingET 

  //- Recomputed MET via SUSYTools  (Cluster Soft Term)
  //- usually muons are treated as invisible pwarticles here! (i.e. Met_doRefMuon=Met_doMuonTotal=false, set via jOpt)

  // MET (cluster soft term) -- visible muons
  metRFC->clear(); 
  CHECK( tool_st->GetMET(*metRFC,
			 jets_sc,
			 electrons_sc,
			 muons_sc,
			 0,
			 0,
			 false,  //calo ST
			 true)); //JVT
  TVector2 v_met_ST_vmu = getMET( metRFC, "Final");
  met_obj.SetVector(v_met_ST_vmu,"met_vmu");  //- Copy met vector to the met data member
  sumET_cst_vmu = (*metRFC)["Final"]->sumet()*0.001;

  //Calo soft terms
  met_cst = (*metRFC)["SoftClus"]->met()*0.001;

  //(OLD WAY)
  //  TVector2 v_met_ST_vmu_MU = getMET( metRFC, "Muons"); //Muon visible Term
  //  float sumEt_cst_muons    = (*metRFC)["Muons"]->sumet()*0.001; //Muon visible sumEt 
  //
  //  TVector2 v_met_ST = v_met_ST_vmu;
  //  v_met_ST -= v_met_ST_vmu_MU; //subtract muon term
  //  sumET_cst = sumET_cst_vmu - sumEt_cst_muons;    
  
  // if(muons_sc->size()){
  //   cout << "-----------------------------------" << endl;
  //   cout << "N muons = " << muons_sc->size() << endl;
  //   cout << "OLD MET  = " << v_met_ST.Mod() << endl;
  //   cout << "NEW MET  = " << v_met_ST_vmu_NEW.Mod() << endl;
  // }
    

  // MET (cluster soft term) -- invisible muons
  metRFC->clear(); 
  CHECK( tool_st->GetMET(*metRFC,
  			 jets_sc,
  			 electrons_sc,
  			 muons_sc,
  			 0,
  			 0,
  			 false,      //calo ST
			 true,       //JVT
			 muons_sc)); //invisible particles
  TVector2 v_met_ST = getMET( metRFC, "Final"); //Muon visible Term 

  met_obj.SetVector(v_met_ST,"met_imu");  //- Copy met vector to the met data member
  sumET_cst = (*metRFC)["Final"]->sumet()*0.001;



  //- Recomputed MET via SUSYTools (Track Soft Term (TST))
  if(this->isVertexOk){   //protect against crash in Data from METRebuilder  ///FIX_ME

    // MET (track soft term) -- visible muons
    metRFC->clear();
    CHECK( tool_st->GetMET(*metRFC,
			   jets_sc,
			   electrons_sc,
			   muons_sc,
			   0,
			   0,
			   true,    //track ST
			   true));  //JVT
    TVector2 v_met_ST_vmu_tst = getMET( metRFC, "Final"); 
    met_obj.SetVector( v_met_ST_vmu_tst, "met_tst_vmu");  //- Copy met vector to the met data member
    sumET_tst_vmu = (*metRFC)["Final"]->sumet()*0.001;

    // track soft term
    met_tst = (*metRFC)["PVSoftTrk"]->met()*0.001;

    //(OLD WAY)
    // v_met_ST_vmu_MU       = getMET( metRFC, "Muons");          //Muon visible Term
    // float sumEt_tst_muons = (*metRFC)["Muons"]->sumet()*0.001; //Muon visible sumEt
    
    // // MET (track soft term) -- invisible muons
    // TVector2 v_met_ST_tst = v_met_ST_vmu_tst;
    // v_met_ST_tst -= v_met_ST_vmu_MU; //subtract muon term
    // met_obj.SetVector(v_met_ST_tst,"met_tst_imu");  //- Copy met vector to the met data member
    // sumET_tst = sumET_tst_vmu - sumEt_tst_muons;

    // MET (track soft term) -- invisible muons
    metRFC->clear(); 
    CHECK( tool_st->GetMET(*metRFC,
			   jets_sc,
			   electrons_sc,
			   muons_sc,
			   0,
			   0,
			   true,      //calo ST
			   true,       //JVT
			   muons_sc)); //invisible particles
    TVector2 v_met_ST_tst = getMET( metRFC, "Final"); //Muon visible Term 
    
    met_obj.SetVector(v_met_ST_tst,"met_tst_imu");  //- Copy met vector to the met data member
    sumET_tst = (*metRFC)["Final"]->sumet()*0.001;
    
  }
  else{
    met_obj.SetVector( met_obj.GetVector("met_imu"), "met_tst_imu", true );  //- Copy met vector to the met data member
    met_obj.SetVector( met_obj.GetVector("met_vmu"), "met_tst_vmu", true );  //- Copy met vector to the met data member
    Warning("loop()", "MET_CST assigned to MET_TST to avoid METRebuilder crash.");
  }
  
  //- Track met
  if(mtrack){
    //cout << "checking MET track" << endl;
    TVector2 v_met_trk( mtrack->mpx(), mtrack->mpy() ); 
    met_obj.SetVector(v_met_trk, "met_trk");
  }

  //--- Met LocHadTopo (from the branches)
  if(mtopo){
     TVector2 v_met_lochadtopo( mtopo->mpx(), mtopo->mpy() ); 
     met_obj.SetVector(v_met_lochadtopo, "met_locHadTopo");
  }

  //--- Met RefFinal, visible muons (from the branches) [retrieved above]
  TVector2 v_met_rfinal_mu(mrf->mpx(), mrf->mpy());
  met_obj.SetVector(v_met_rfinal_mu, "met_refFinal_vmu");

  //--- Met RefFinal, invisible muons (from the branches)
  TVector2 v_met_rfinal_muinv = met_obj.GetVector("met_refFinal_vmu"); //in GeV
  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
    TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
    v_met_rfinal_muinv += muon_vector2;
  }
  met_obj.SetVector(v_met_rfinal_muinv, "met_refFinal_imu",true); //already in GeV
  
  //--- Met with electrons as invisible (signal electrons)
  TVector2 v_met_elinv_ST = met_obj.GetVector("met_imu"); //== met_ST (GeV) 
  TVector2 v_met_elinv_ST_vmu = met_obj.GetVector("met_vmu"); //== met_ST (GeV)   
  TVector2 v_met_elinv_TST = met_obj.GetVector("met_tst_imu"); //== met_ST (GeV)                                                      
  TVector2 v_met_elinv_TST_vmu = met_obj.GetVector("met_tst_vmu"); //== met_ST (GeV) 

  TLorentzVector vels(0.,0.,0.,0.);
  for (unsigned int iEl=0; iEl < recoElectrons.size(); iEl++){ 
    vels += recoElectrons.at(iEl).GetVector();
  }
  v_met_elinv_ST.Set( v_met_elinv_ST.Px() + vels.Px(), v_met_elinv_ST.Py() + vels.Py() );
  v_met_elinv_ST_vmu.Set( v_met_elinv_ST_vmu.Px() + vels.Px(), v_met_elinv_ST_vmu.Py() + vels.Py() );

  v_met_elinv_TST.Set( v_met_elinv_TST.Px() + vels.Px(), v_met_elinv_TST.Py() + vels.Py() );
  v_met_elinv_TST_vmu.Set( v_met_elinv_TST_vmu.Px() + vels.Px(), v_met_elinv_TST_vmu.Py() + vels.Py() );

  met_obj.SetVector(v_met_elinv_ST, "met_imu_ecorr", true); //already in GeV    
  met_obj.SetVector(v_met_elinv_ST_vmu, "met_vmu_ecorr", true); //already in GeV     
  
  met_obj.SetVector(v_met_elinv_TST, "met_tst_imu_ecorr", true); //already in GeV
  met_obj.SetVector(v_met_elinv_TST_vmu, "met_tst_vmu_ecorr", true); //already in GeV 

  //--- Met (with muon term) with muons as invisible (signal muons)
  TVector2 v_met_muinv_ST_vmu = met_obj.GetVector("met_vmu"); //== met_ST (GeV)  
  TVector2 v_met_muinv_TST_vmu = met_obj.GetVector("met_tst_vmu"); //== met_ST (GeV)
 
  TLorentzVector vmus(0.,0.,0.,0.);
  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){ 
    vmus += recoMuons.at(iMu).GetVector();
  }
  v_met_muinv_ST_vmu.Set( v_met_muinv_ST_vmu.Px() + vmus.Px(), v_met_muinv_ST_vmu.Py() + vmus.Py() );
  v_met_muinv_TST_vmu.Set( v_met_muinv_TST_vmu.Px() + vmus.Px(), v_met_muinv_TST_vmu.Py() + vmus.Py() );
 
  met_obj.SetVector(v_met_muinv_ST_vmu, "met_vmu_mucorr", true); //already in GeV       
  met_obj.SetVector(v_met_muinv_TST_vmu, "met_tst_vmu_mucorr", true); //already in GeV
  
  //--- Met with photons as invisible 
  TVector2 v_met_phinv_ST = met_obj.GetVector("met_tst_imu"); //== met_ST (GeV) 
  TVector2 v_met_phinv_ST_vmu = met_obj.GetVector("met_tst_vmu"); //== met_ST (GeV)   
 
  TLorentzVector vphs(0.,0.,0.,0.);
  for (unsigned int iPh=0; iPh < recoPhotons.size(); iPh++){ 
    vphs += recoPhotons.at(iPh).GetVector();
  }
  v_met_phinv_ST.Set( v_met_phinv_ST.Px() + vphs.Px(), v_met_phinv_ST.Py() + vphs.Py() ); //met TST photon corrected
  v_met_phinv_ST_vmu.Set( v_met_phinv_ST_vmu.Px() + vphs.Px(), v_met_phinv_ST_vmu.Py() + vphs.Py() );  //met TST photon corrected

  met_obj.SetVector(v_met_phinv_ST, "met_imu_phcorr", true); //already in GeV    
  met_obj.SetVector(v_met_phinv_ST_vmu, "met_vmu_phcorr", true); //already in GeV  
  
  
  // truth met
  if(this->isMC){
    TVector2 v_met_truth_imu(mtruth_inv->mpx(), mtruth_inv->mpy()); 
    TVector2 v_met_truth_vmu(mtruth_vis->mpx(), mtruth_vis->mpy());   
    
    met_obj.SetVector(v_met_truth_imu, "met_truth_imu");  //- Copy met vector to the met data member
    met_obj.SetVector(v_met_truth_vmu, "met_truth_vmu");  //- Copy met vector to the met data member 
  }

  //*** CONFIG
  // book MET flavours
  metmap={};
  metmap[::MetDef::InvMu] = met_obj.GetVector("met_imu");
  metmap[::MetDef::VisMu] = met_obj.GetVector("met_vmu");  
  metmap[::MetDef::InvMuECorr] = met_obj.GetVector("met_imu_ecorr");
  metmap[::MetDef::VisMuECorr] = met_obj.GetVector("met_vmu_ecorr");  
  metmap[::MetDef::VisMuMuCorr] = met_obj.GetVector("met_vmu_mucorr");  
  metmap[::MetDef::InvMuPhCorr] = met_obj.GetVector("met_imu_phcorr");
  metmap[::MetDef::VisMuPhCorr] = met_obj.GetVector("met_vmu_phcorr");
  if(mtrack)
    metmap[::MetDef::Track] = met_obj.GetVector("met_trk");
  else 
    metmap[::MetDef::Track] = TVector2((float)DUMMYDN, (float)DUMMYDN);
  metmap[::MetDef::InvMuRef] = met_obj.GetVector("met_refFinal_imu");
  metmap[::MetDef::VisMuRef] = met_obj.GetVector("met_refFinal_vmu");  
  metmap[::MetDef::InvMuTST] = met_obj.GetVector("met_tst_imu");
  metmap[::MetDef::VisMuTST] = met_obj.GetVector("met_tst_vmu");
  
  metmap[::MetDef::InvMuTSTECorr] = met_obj.GetVector("met_tst_imu_ecorr");
  metmap[::MetDef::VisMuTSTECorr] = met_obj.GetVector("met_tst_vmu_ecorr");
  metmap[::MetDef::VisMuTSTMuCorr] = met_obj.GetVector("met_tst_vmu_mucorr");
  
  metmap[::MetDef::InvMuTruth] = met_obj.GetVector("met_truth_imu");
  metmap[::MetDef::VisMuTruth] = met_obj.GetVector("met_truth_vmu");  
  
  if(mtopo)
    metmap[::MetDef::locHadTopo] = met_obj.GetVector("met_locHadTopo");  
  else  
    metmap[::MetDef::locHadTopo] = TVector2((float)DUMMYDN, (float)DUMMYDN);
  
  
  //meta data (fill it once)
  if(smetmap==""){ 
    for (auto& mk : metmap){ 
      smetmap += sMetDef[(unsigned int)mk.first];
      smetmap += ",";
    }
    meta_metmap->SetTitle( smetmap.c_str() );
  }

  //***


  if (printMet){
    cout<<"Info for MET "<<endl;
    met_obj.PrintInfo();
    cout<<"met_ST.Mod() = " << v_met_ST.Mod()*0.001 << endl;
    cout<<"MET from D3PD = " << v_met_rfinal_mu.Mod()*0.001 << endl;
    cout<<endl;
  }


  //*** Event Skimming (if requested)
  if(m_skim_met || m_skim_btag){
    
    bool SK_passBtagging = (bj_N>0);
    
    bool SK_passMETdef = (metmap[::MetDef::InvMu].Mod() > 100. 
		       || metmap[::MetDef::VisMu].Mod() > 100. );
    
    bool SK_passSR = ( ((eb_N+mb_N)==0) &&  (metmap[::MetDef::InvMu].Mod() > 250. 
					  || metmap[::MetDef::VisMu].Mod() > 250. 
					  || metmap[::MetDef::InvMuTST].Mod() > 250. 
					  || metmap[::MetDef::VisMuTST].Mod() > 250. ) );
    
    bool SK_passCRe = ( (e_N>0) &&  (SK_passMETdef
				     || metmap[::MetDef::InvMuECorr].Mod() > 100. 
				     || metmap[::MetDef::VisMuECorr].Mod() > 100. 
				     || metmap[::MetDef::InvMuTSTECorr].Mod() > 100. 
				     || metmap[::MetDef::VisMuTSTECorr].Mod() > 100.)); 
    
    bool SK_passCRmu = ( (m_N>0) &&  ( SK_passMETdef
				       || metmap[::MetDef::VisMuMuCorr].Mod() > 100.
				       || metmap[::MetDef::VisMuTSTMuCorr].Mod() > 100.));
    
    bool SK_passCRph = ( (ph_N>0) &&  ( SK_passMETdef
				        || metmap[::MetDef::VisMuPhCorr].Mod() > 100.));


    bool SK_passAnyMet = (metmap[::MetDef::InvMu].Mod() > 100.
                       || metmap[::MetDef::VisMu].Mod() > 100. 
                       || metmap[::MetDef::InvMuTST].Mod() > 100. 
                       || metmap[::MetDef::VisMuTST].Mod() > 100. 
                       || metmap[::MetDef::InvMuECorr].Mod() > 100. 
                       || metmap[::MetDef::VisMuECorr].Mod() > 100. 
                       || metmap[::MetDef::InvMuTSTECorr].Mod() > 100. 
                       || metmap[::MetDef::VisMuTSTECorr].Mod() > 100. 
                       || metmap[::MetDef::VisMuMuCorr].Mod() > 100.
                       || metmap[::MetDef::VisMuTSTMuCorr].Mod() > 100.);
    
    bool keep_event = ( (m_skim_btag && SK_passBtagging && (SK_passSR || SK_passCRe || SK_passCRmu || SK_passCRph)) || (m_skim_met && SK_passAnyMet) );

    if(!keep_event){
      delete jets_sc;              delete jets_scaux;
      delete muons_sc;             delete muons_scaux;
      delete electrons_sc;         delete electrons_scaux;
      delete photons_sc;           delete photons_scaux;
      delete metRFC;               delete metRFCAux;

      if(myfile.is_open())
	myfile.close();
      return nextEvent();
    }
  }


  //- Met Cleaning
  if (Met_doMetCleaning){
    const xAOD::MissingET* mrf_refmuon = (*cmet_reffinal)["Muons"];
    TVector2 myMetMuon( mrf_refmuon->mpx(), mrf_refmuon->mpy() ); //MET_MuonBoy in run1
    isMetCleaned = ( ( (myMetMuon.Mod() / v_met_rfinal_mu.Mod()) * cos(v_met_rfinal_mu.Phi() - myMetMuon.Phi()) ) < 0.8);
  }

  jet_itr = jets_sc->begin();
  jet_end = jets_sc->end();
  int n_fakemet_jets=0;
  for( ; jet_itr != jet_end; ++jet_itr ){   
    //look for fake-met jets    
    if (Met_doFakeEtmiss){
      float bchcorrjet;
      (*jet_itr)->getAttribute(xAOD::JetAttribute::BchCorrJet, bchcorrjet);
      if((*jet_itr)->pt() > 40000. && 
	 bchcorrjet > 0.05 && 
	 deltaPhi(met_obj.GetVector("met_vmu").Phi(), (*jet_itr)->phi()) < 0.3){
	n_fakemet_jets++;
      }
    }
  }    


  if (printMet){
    cout<<"Info for MET "<<endl;
    met_obj.PrintInfo();
    cout<<"met_ST.Mod() = " << v_met_ST.Mod()*0.001 << endl;
    cout<<"MET from D3PD = " << v_met_rfinal_mu.Mod()*0.001 << endl;
    cout<<endl;
  }

  //--- Additional jet cleaning 
  //  To remove events with fake missing Et due to non operational cells in the Tile and the HEC.
  //  *before lepton overlap removal*
  //  from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr178TeV
  this->isFakeMet = (n_fakemet_jets>0);
  this->passPreselectionCuts &= (!this->isFakeMet);

  
  //Recoiling system against MET (prebooking)
  TLorentzVector sjet(0.,0.,0.,0.);
  bool LeadOnly=true; //use only the leading jet 
  std::vector<TLorentzVector> thrust_jets;
  if(LeadOnly && recoJets.size()) sjet = recoJets.at(0).GetVector();   
  for(const auto& jet : recoJets){
    if(!LeadOnly) sjet += jet.GetVector();
    HT   += jet.Pt();
    thrust_jets.push_back( jet.GetVector());
  }

  TVector2 sjet2(sjet.Px(), sjet.Py());


  //Z(ll) candidate
  Zll_candidate();

  //--- Fill MET related variables  -  vectorized now!
  for (auto& mk : metmap) {

    met.push_back(mk.second.Mod());
    met_phi.push_back( TVector2::Phi_mpi_pi( mk.second.Phi() ) ); //--- Phi defined between -pi and pi
    
    //Recoiling system against MET (prebooking)
    rmet_par.push_back( sjet2.Mod() * TMath::Cos(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) ); //leading jet only
    rmet_norm.push_back( sjet2.Mod() * TMath::Sin(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_par_mod.push_back( TMath::Cos(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_norm_mod.push_back( TMath::Sin(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_dPhi_met_jetsys.push_back( deltaPhi(sjet2.Phi(), mk.second.Phi()) ); 


    //sphericity and thrust
    thrust_jets.push_back( makeTLV(mk.second) ); //add met to jet vectors

    tr_spher.push_back( Calc_Sphericity(thrust_jets, true) );
    tr_thrust.push_back( Calc_Thrust(thrust_jets) );

    thrust_jets.pop_back(); //remove met from jet vectors
    
    dPhi_met_mettrk.push_back( deltaPhi( metmap[MetDef::Track].Phi(), mk.second.Phi()) );	

    if (recoJets.size()>=2 && (recoElectrons.size()>=1 || recoMuons.size()>=1)){
        
      TLorentzVector lead_lep(0.,0.,0.,0.); 
      if (recoMuons.size()==0) lead_lep = recoElectrons.at(0).GetVector(); 
      else if (recoElectrons.size()==0) lead_lep = recoMuons.at(0).GetVector();
      else {
        if (recoMuons.at(0).Pt()>recoElectrons.at(0).Pt()) lead_lep = recoMuons.at(0).GetVector();
        if (recoMuons.at(0).Pt()<recoElectrons.at(0).Pt()) lead_lep = recoElectrons.at(0).GetVector();	
      }
      
      
      amt2_0.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 160));
      amt2_1.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 170));
      amt2_2.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 180));
      amt2_3.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 200));
    }
    
    //some extra variables
    auto ntaujs =0;
    auto dphi_min_allj_tmp=-1.;
    auto dphi_min_4j_tmp=-1.;
    auto dphi_min_tmp=-1.;
    int ibcl=-1;  //closest bjet to met
    int ibfar=-1; //most faraway bjet from met
    int ilcl=-1;  //closest light jet to met
    float min_dphi_bm=999.;
    float max_dphi_bm=0.;
    float min_dphi_lm=999.;

    auto min_mt_jm=99999.;
    auto ijet=0;
    for(auto& jet : recoJets ){  //jet loop

      //count 'tau' jets candidates
      if ( jet.isTauJet( mk.second.Phi(), Jet_Tagger.Data()) ) 
	ntaujs += 1;


      //look for min dphi(jet,met)   
      float dphi_jm = deltaPhi( jet.Phi(), mk.second.Phi() );
      
      if(dphi_min_allj_tmp < 0 || dphi_min_allj_tmp > deltaPhi(jet.Phi(), mk.second.Phi()) ) //closest jet to met (all)
	dphi_min_allj_tmp = dphi_jm; //deltaPhi(recoJets.at(ijet).Phi(), met_phi[0]);
      
      if( ijet < 3 && (dphi_min_tmp < 0 || dphi_min_tmp > dphi_jm) ) //closest jet to met (3-leading only)
	dphi_min_tmp = dphi_jm;

      if( ijet < 4 && (dphi_min_4j_tmp < 0 || dphi_min_4j_tmp > dphi_jm) ) //closest jet to met (4-leading only)
	dphi_min_4j_tmp = dphi_jm;

      //--- look for min MT(jet,MET)
      float mt_jm = Calc_MT( jet, mk.second );
      if(mt_jm < min_mt_jm){
	min_mt_jm = mt_jm;
      }
            

 
      //--- look for closest/faraway bjet and closer light jet to MET
        if( jet.isbjet && fabs(jet.Eta())<2.5){
	
	  if( dphi_jm < min_dphi_bm){ //closest bjet
	    min_dphi_bm = dphi_jm;
	    ibcl = ijet;
	  }
	
	  if( dphi_jm > max_dphi_bm){ //most faraway bjet
	    max_dphi_bm = dphi_jm;
	    ibfar = ijet;
	  }
	
        }
        else{
	  if( dphi_jm < min_dphi_lm){ //closest light-jet
	    min_dphi_lm = dphi_jm;
	    ilcl = ijet;
	  }
        }
      
   
      
      
      
      
      ijet++;
    }
    dPhi_min_alljets.push_back(dphi_min_allj_tmp);
    dPhi_min_4jets.push_back(dphi_min_4j_tmp);    
    dPhi_min.push_back(dphi_min_tmp);

    if( min_mt_jm==999999. )
      min_mt_jm=0.; //it shouldn't matter, but let's force zero-jets events to fail this!

    //fill MT variables from found values above
    MT_min_jet_met.push_back( min_mt_jm );
    MT_bcl_met.push_back( (ibcl >= 0 ? Calc_MT( recoJets.at(ibcl), mk.second ): 0.) );
    MT_bfar_met.push_back( (ibfar >= 0 ? Calc_MT( recoJets.at(ibfar), mk.second ) : 0.) );
    MT_lcl_met.push_back( (ilcl >= 0 ? Calc_MT( recoJets.at(ilcl), mk.second ) : 0.) );
    MT_jsoft_met.push_back( (recoJets.size() > 0 ? Calc_MT( recoJets.back(), mk.second ) : 0.) ); //it assumes jets are pt ordered!

    //dphi(jet_i, met+j) ,  MT(j_i, met_j)
    if (j_N>0){
      dPhi_met_j1.push_back( deltaPhi( mk.second.Phi(), recoJets.at(0).Phi()) );	
      j1_mT.push_back( Calc_MT( recoJets.at(0), mk.second ));
      if (j_N>1){
	dPhi_met_j2.push_back( deltaPhi( mk.second.Phi(), recoJets.at(1).Phi()) );	
	j2_mT.push_back( Calc_MT( recoJets.at(1), mk.second ));
	if (j_N>2){
	  dPhi_met_j3.push_back( deltaPhi( mk.second.Phi(), recoJets.at(2).Phi()) );	
	  j3_mT.push_back( Calc_MT( recoJets.at(2), mk.second ));
	  if (j_N>3){
	    dPhi_met_j4.push_back( deltaPhi( mk.second.Phi(), recoJets.at(3).Phi()) );	
	    j4_mT.push_back( Calc_MT( recoJets.at(3), mk.second ));
	  }
	  else{
	    dPhi_met_j4.push_back( DUMMYUP );
	    j4_mT.push_back( DUMMYUP );
	  }
	}
	else{
	  dPhi_met_j3.push_back( DUMMYUP );
	  j3_mT.push_back( DUMMYUP );
	}
      }
      else{
	dPhi_met_j2.push_back( DUMMYUP );
	j2_mT.push_back( DUMMYUP );
      }
    }
    else{
      dPhi_met_j1.push_back( DUMMYUP );
      j1_mT.push_back( DUMMYUP );
    }


    //meff = HT + met
    meff.push_back( HT + mk.second.Mod() );
    
    //tau jet candidates
    j_tau_N.push_back( ntaujs );

    //==== Razor ====
    fillRazor( makeV3( mk.second ) );

    //==== Corrected mct ====
    if (j_N>1) 
      mct_corr.push_back(Calc_mct_corr(recoJets.at(0), recoJets.at(1), mk.second));
    else 
      mct_corr.push_back(DUMMYUP);
    
    //==== Z-related vars
    Zll_extra(mk.second); //be sure to call Zll_candidate() before!

  }//end of met flavor loop

  //MET LocHadTopo
  if(mtopo)
    met_lochadtopo = met_obj.GetVector("met_locHadTopo").Mod();    

  //Truth Filter MET (all neutrinos from top)

  UInt_t MET_ids[] = {410000, 410013, 410014}; //, 407012, 407019, 407021}; //nominal   //filtered : 407012, 407019, 407021;
  if ( this->isMC && count(MET_ids, MET_ids+84, this->mc_channel_number) ){
    met_top = (float) Calc_TruthNuMET()*0.001;  // this cast is actually needed to reproduce what the filter does

  }
  

  //--- Track Veto
  //init vars
  std::vector<float> dR_jet_track_min;                    
  float              dR_jet_track_min_event;              
  std::vector<int>   dR_jet_track_min_flavor;            
   
  //--- Tracks
  for(unsigned int iTrack = 0; iTrack < RecoUnmatchedTracksElMu.size(); ++iTrack){
    float dR_jet_track_min_float = -1;
    int dR_jet_track_min_flavor_float = -1;
    for (unsigned int iJet=0; iJet < recoJets.size(); ++iJet){
      
      if (dR_jet_track_min_float < 0 || RecoUnmatchedTracksElMu.at(iTrack).DeltaR(recoJets[iJet]) < dR_jet_track_min_float) {
	dR_jet_track_min_float = RecoUnmatchedTracksElMu.at(iTrack).DeltaR(recoJets[iJet]);
	dR_jet_track_min_flavor_float = recoJets[iJet].FlavorTruth;
      }
    }
    
    dR_jet_track_min.push_back(dR_jet_track_min_float);
    dR_jet_track_min_flavor.push_back(dR_jet_track_min_flavor_float);
    
    if (dR_jet_track_min_float < 0.4) {
      isolatedTrack_MatchedJet.at(RecoUnmatchedTracksIdx.at(iTrack)) = 1;
    }
    
  } // end of unmatched tracks
  //--- If size of dR_jet_track_min==0, means all tracks are matched to leptons. I fill them with dummy info, to plot well.
  if (dR_jet_track_min.size()==0) {
    dR_jet_track_min.push_back(10);
    dR_jet_track_min_flavor.push_back(10);
  }
  
  dR_jet_track_min_event = -1;
  for (unsigned int it=0; it < dR_jet_track_min.size(); ++it){
    if (dR_jet_track_min.at(it) > 9.9) continue; //this never happens no? O.o
    if (dR_jet_track_min_event < 0 || dR_jet_track_min.at(it) < dR_jet_track_min_event)
      dR_jet_track_min_event = dR_jet_track_min.at(it);
    if (dR_jet_track_min.at(it) > 0.4){
      m_isolatedUnmatchedTracksPlusJets += 1;
    }
  } 
  //--- Some other event variables
  //fill info for truth b-sb decay
  findBparton();  //FIX_ME  //needed?
  //*** otherwise t_b_*** variables are not filled !
  
  //--- Invariant mass of the 2 first jets (M12)
  if(j_N>1){
    TLorentzVector jet_M = (recoJets[0].GetVector() + recoJets[1].GetVector());
    M12 = jet_M.M();
  }
  
  //- dPhi & dR
  //dPhi_met_mettrk = deltaPhi(metmap[MetDef::Track].Phi(), metmap[MetDef::InvMu].Phi());
  
  if (j_N>0){

    dPhi_j1_bp1 = deltaPhi(recoJets.at(0).Phi(), t_b_phi1);
    dPhi_j1_bp2 = deltaPhi(recoJets.at(0).Phi(), t_b_phi2);
    dR_j1_bp1   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi1, t_b_eta1);
    dR_j1_bp2   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi2, t_b_eta2);
    
    if (recoMuons.size()>0){
      dR_j1_m1 = recoJets.at(0).DeltaR(recoMuons.at(0));
      if (recoMuons.size()>1) 
	dR_j1_m2 = recoJets.at(0).DeltaR(recoMuons.at(1));
    }
    
    if (j_N>1){
      
      dPhi_j1_j2  = deltaPhi(recoJets.at(0).Phi(), recoJets.at(1).Phi());
      dPhi_j2_bp1 = deltaPhi(recoJets.at(1).Phi(), t_b_phi1);
      dPhi_j2_bp2 = deltaPhi(recoJets.at(1).Phi(), t_b_phi2);
      dEta_j1_j2  = fabs(recoJets.at(0).Eta()-recoJets.at(1).Eta());
      
      dR_j1_j2    = recoJets.at(0).DeltaR(recoJets.at(1));
      dR_j2_bp1   = deltaR(recoJets.at(1).Phi(),recoJets.at(1).Eta(),t_b_phi1,t_b_eta1);
      dR_j2_bp2   = deltaR(recoJets.at(1).Phi(),recoJets.at(1).Eta(),t_b_phi2,t_b_eta2);

      if (recoMuons.size()>0){
	dR_j2_m1 = recoJets.at(1).DeltaR(recoMuons.at(0));
	
	TLorentzVector j1 = recoJets.at(0).GetVector();
	TLorentzVector j2 = recoJets.at(1).GetVector();	
	TLorentzVector mu = recoMuons.at(0).GetVector();	
	
	Mmub_min = min((j1+mu).M(), (j2+mu).M());
	
	
	if (recoMuons.size()>1) 
	  dR_j2_m2=recoJets.at(1).DeltaR(recoMuons.at(1));
      }
      
      if (recoElectrons.size()>0){      
      
	TLorentzVector j1 = recoJets.at(0).GetVector();
	TLorentzVector j2 = recoJets.at(1).GetVector();	
	TLorentzVector el = recoElectrons.at(0).GetVector();	
	
	Melb_min = min((j1+el).M(), (j2+el).M());      
      
      }
      
      
      if (j_N>2){
	dPhi_j1_j3  = deltaPhi(recoJets.at(0).Phi(), recoJets.at(2).Phi());
	dPhi_j2_j3  = deltaPhi(recoJets.at(1).Phi(), recoJets.at(2).Phi());
	dPhi_j3_bp1 = deltaPhi(recoJets.at(2).Phi(), t_b_phi1);
	dPhi_j3_bp2 = deltaPhi(recoJets.at(2).Phi(), t_b_phi2);

	dR_j1_j3    = recoJets.at(0).DeltaR(recoJets.at(2));
	dR_j2_j3    = recoJets.at(1).DeltaR(recoJets.at(2));
	dR_j3_bp1   = deltaR(recoJets.at(2).Phi(),recoJets.at(2).Eta(),t_b_phi1,t_b_eta1);
	dR_j3_bp2   = deltaR(recoJets.at(2).Phi(),recoJets.at(2).Eta(),t_b_phi2,t_b_eta2);

	if(recoMuons.size()>0){
	  dR_j3_m1  = recoJets.at(2).DeltaR(recoMuons.at(0));
	  if(recoMuons.size()>1) 
	    dR_j3_m2 = recoJets.at(2).DeltaR(recoMuons.at(1));
	}
      }
    }
  }    
  //Do we need this?? //CHECK_ME
  float dPhi_bp1_bp2     = deltaPhi(t_b_phi1, t_b_phi2);
  float dPhi_bp1_met     = deltaPhi(t_b_phi1, met_obj.Phi("met"));
  float dPhi_bp2_met     = deltaPhi(t_b_phi2, met_obj.Phi("met"));
  float dPhi_sbp1_sbp2   = deltaPhi(t_sb_phi1,t_sb_phi2);
  float dPhi_neutr1_sbp1 = deltaPhi(t_neutr_phi1,t_sb_phi1);
  float dPhi_neutr2_sbp2 = deltaPhi(t_neutr_phi2,t_sb_phi2);
  float dPhi_neutr1_bp1  = deltaPhi(t_neutr_phi1,t_b_phi1);
  float dPhi_neutr2_bp2  = deltaPhi(t_neutr_phi2,t_b_phi2);
  float dR_bp1_bp2       = deltaR(t_b_phi1,t_b_eta1,t_b_phi2,t_b_eta2);
  float dR_sbp1_sbp2     = deltaR(t_sb_phi1,t_sb_eta1,t_sb_phi2,t_sb_eta2);
  
    
  
  //Do we need this??
  //- (truth) Tau veto for 3rd- and 4th leading jet
  float dR_truthTau_j3=999.;
  float dR_truthTau_j4=999.;
  
  if (this->isMC){
    if (j_N>2){
      
      TLorentzVector TruthTau(0.,0.,0.,0.);
      
      if(recoJets.at(2).nTrk>0 && recoJets.at(2).nTrk<5 && dPhi_met_j3[0] < TMath::Pi()/5. && j3_mT[0]<100.){
	
	for(const xAOD::TruthParticle* tp : *m_truthP) {
	  
	  if ( isHard(tp) && isTau(tp) ){
	    fillTLV(TruthTau, tp);
	    
 	    float tmpDR = TruthTau.DeltaR(recoJets.at(2));
	    if(dR_truthTau_j3 > tmpDR)
	      dR_truthTau_j3 = tmpDR;
	    
	  }

	  if (j_N>3){
	    if(recoJets.at(3).nTrk>0 && recoJets.at(3).nTrk < 5 && dPhi_met_j4[0] < TMath::Pi()/5. && j4_mT[0]<100.){

	      float tmpDR = TruthTau.DeltaR(recoJets.at(3));
	      if(dR_truthTau_j4 > tmpDR)
		dR_truthTau_j4 = tmpDR;
	      
	    }
	  }
	}
      } 
      
    }
  }
   
  //Dijet Mass
  if (j_N>1){  
    mjj = Calc_Mjj();

      if( recoJets.at(0).isbjet && recoJets.at(1).isbjet && fabs(recoJets.at(0).Eta())<2.5 && fabs(recoJets.at(1).Eta())<2.5)
        mbb = mjj;
    
	
  }

  //Mct
  mct = Calc_mct();
  
  //==== Hadronic top reconstruction ====
  int ibtop1=-1; //book two highest btag-weight jets for later use
  int ibtop2=-1;
  int iblead1=-1;
  int iblead2=-1;
  float maxbw1=-99;
  float maxbw2=-99;
  auto ijet=0;
  for( auto& jet : recoJets ){  //jet loop
    
    if( jet.isbjet && fabs(jet.Eta())<2.5){
	
	if(iblead1<0)//leadings
	  iblead1=ijet;
	else if(iblead2<0)
	  iblead2=ijet;

	float locbw = recoJets.at(ijet).getBweight(Jet_Tagger); //book high bweight jets
	if(locbw > maxbw1){
	  if(maxbw1 > -99){ //if already filled, the old max1 becomes the new max2
	    maxbw2 = maxbw1;
	    ibtop2 = ibtop1;
	  }
	  ibtop1 = ijet;
	  maxbw1 = locbw;
	}
	else if(locbw > maxbw2){
	  ibtop2 = ijet;
	  maxbw2 = locbw;
	}
      }


      ijet++;
 
  }

  
  RecoHadTops(ibtop1, ibtop2);

  // ------ Max bb-system variables  
  if (ibtop1!=-1 && ibtop2!=-1)
  {
    std::vector<unsigned int> leadB_index;
    leadB_index.push_back(ibtop1);
    leadB_index.push_back(ibtop2);  

    std::vector<pair<unsigned int, double> > dR_Max;
    std::vector<pair<unsigned int, double> > dR_pt_Max;
    TLorentzVector refVector;  
  
    refVector = recoJets.at(ibtop1).GetVector() + recoJets.at(ibtop2).GetVector();           
    dR_Max = GetOrderedJetIndexdR(refVector, leadB_index);
    dR_pt_Max = GetOrderedJetIndexAntiKtd(refVector, leadB_index);      

    for (unsigned int i=0; i<dR_Max.size(); i++)
    {
	  index_min_dR_bb.push_back(dR_Max.at(i).first);
	  min_dR_bb.push_back(dR_Max.at(i).second);
	  
    } 

    for (unsigned int i=0; i<dR_pt_Max.size(); i++)
    {
	  index_min_dR_pt_bb.push_back(dR_pt_Max.at(i).first);
	  min_dR_pt_bb.push_back(dR_pt_Max.at(i).second);
    }   
  }
  // -----------------------------  


  //dPhi_b1_b2  
  dPhi_b1_b2 = (iblead1>=0 && iblead2>=0 ? deltaPhi( recoJets.at(iblead1).Phi(), recoJets.at(iblead2).Phi() ) : 0.);

  //==== Fat Jets building ==============
  //run algo_jet algorithm over RecoJets collection to build new-R jets
  std::vector<TLorentzVector> jets_R_12 = getFatJets(1.2);
  m0_antikt12  = (jets_R_12.size()>0 ? jets_R_12.at(0).M() : 0.);
  m1_antikt12  = (jets_R_12.size()>1 ? jets_R_12.at(1).M() : 0.);
  pt0_antikt12 = (jets_R_12.size()>0 ? jets_R_12.at(0).Pt() : 0.);
  pt1_antikt12 = (jets_R_12.size()>1 ? jets_R_12.at(1).Pt() : 0.);
  mtasym12     = getAsymmetry( m0_antikt12, m1_antikt12 );
  
  std::vector<TLorentzVector> jets_R_08 = getFatJets(0.8);
  m0_antikt08  = (jets_R_08.size()>0 ? jets_R_08.at(0).M() : 0.);
  m1_antikt08  = (jets_R_08.size()>1 ? jets_R_08.at(1).M() : 0.);
  pt0_antikt08 = (jets_R_08.size()>0 ? jets_R_08.at(0).Pt() : 0.);
  pt1_antikt08 = (jets_R_08.size()>1 ? jets_R_08.at(1).Pt() : 0.);
  mtasym08     = getAsymmetry( m0_antikt08, m1_antikt08 );
  
  
  //--- Inv masses involving leptons
  //- Define the invariant mass mu-mu (m_M) and the Pt of the mu-mu system (m_Zpt)
  if (recoMuons.size()==2){
    TLorentzVector m12 = (recoMuons.at(0).GetVector() + recoMuons.at(1).GetVector());
    m_M = m12.M();
    m_Zpt = m12.Pt();
  }
  
  //- Define the invariant mass e-e (m_E) and the Pt of the e-e system (e_Zpt)
  if (recoElectrons.size()==2){
    TLorentzVector e12 = (recoElectrons.at(0).GetVector() + recoElectrons.at(1).GetVector());
    e_M = e12.M();
    e_Zpt = e12.Pt();
  }
  
  //- Define the invariant mass between the electron and the muon
  if (recoMuons.size()==1 && recoElectrons.size()==1){
    TLorentzVector inv_EM = (recoElectrons.at(0).GetVector() + recoMuons.at(0).GetVector());
    m_EM = inv_EM.M();
  }
  
  //--- Define the transverse mass e_MT - we recompute the MET with the electron in order to have the pt of the nu
  if(recoElectrons.size()){
    TVector2 v_e1(recoElectrons.at(0).Px(), recoElectrons.at(0).Py());
    //TVector2 met_electron = met_obj.GetVector("met") - v_e1; //CHECK_ME  - o + ?
    TVector2 met_electron = met_obj.GetVector("met_imu"); 
    TVector2 met_electron_vmu = met_obj.GetVector("met_vmu");
    TVector2 met_tst_electron = met_obj.GetVector("met_tst_imu"); 
    TVector2 met_tst_electron_vmu = met_obj.GetVector("met_tst_vmu");
    e_MT = Calc_MT( recoElectrons.at(0), met_electron);
    e_MT_vmu = Calc_MT( recoElectrons.at(0), met_electron_vmu);    
    e_MT_tst = Calc_MT( recoElectrons.at(0), met_tst_electron);
    e_MT_tst_vmu = Calc_MT( recoElectrons.at(0), met_tst_electron_vmu);    
  }
  
  //--- Define m_MT - we recompute the MET with the muon in order to have the pt of the nu
  if(recoMuons.size()>0){ //--- Careful: Optimized for Etmiss without muons!
    TVector2 v_m1(recoMuons.at(0).Px(), recoMuons.at(0).Py());
    //TVector2 met_muon = met_obj.GetVector("met") - v_m1; //CHECK_ME  - o + ? met has inv muons already o.O
    TVector2 met_muon = met_obj.GetVector("met_imu") - v_m1; 
    TVector2 met_muon_vmu = met_obj.GetVector("met_vmu"); 
    TVector2 met_tst_muon = met_obj.GetVector("met_tst_imu") - v_m1;
    TVector2 met_tst_muon_vmu = met_obj.GetVector("met_tst_vmu");
    m_MT = Calc_MT( recoMuons.at(0), met_muon);
    m_MT_vmu = Calc_MT( recoMuons.at(0), met_muon_vmu);  
    m_MT_tst = Calc_MT( recoMuons.at(0), met_tst_muon);
    m_MT_tst_vmu = Calc_MT( recoMuons.at(0), met_tst_muon_vmu);  
  
  }
 

  
  //--- Boson Pt   <---  V(lv).Pt()
  TVector2 V_lnu = met_obj.GetVector("met_imu");
  TVector2 V_lnu_vmu = met_obj.GetVector("met_vmu");  
  for (unsigned int iEl=0; iEl < recoElectrons.size(); iEl++){
    TVector2 electron_vector2(recoElectrons.at(iEl).GetVector().Px(), recoElectrons.at(iEl).GetVector().Py());
    V_lnu += electron_vector2;
    V_lnu_vmu += electron_vector2;    
  }
  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
      TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
      V_lnu_vmu += muon_vector2;
    }
  
    
  bosonVec_reco_pt = V_lnu.Mod();
  bosonVec_reco_pt_vmu = V_lnu_vmu.Mod();  
  //bosonVec_reco_eta = V_lnu.Eta();  //not accesible!
  bosonVec_reco_phi = TVector2::Phi_mpi_pi( V_lnu.Phi() );  
  bosonVec_reco_phi_vmu = TVector2::Phi_mpi_pi( V_lnu_vmu.Phi() );       
  
  MtTop = TopTransvMass();
    
  //Z decay mode 
  TLorentzVector Ztlv(0.,0.,0.,0.);
  if( this->isMC ){
    std::vector<int> zdecays =  this->GetTruthZDecay(Ztlv);
    if(zdecays.size()){
      Z_decay  = zdecays.at(0);
      Z_pt     = Ztlv.Pt();
      Z_eta    = Ztlv.Eta();
      Z_phi    = Ztlv.Phi();
      //    Z_m      = Ztlv.M(); //not needed! Everyone knows the Z mass :)
    }
  }
  
  //Extended trigger info (if requested)
  if(doTrigExt){
    const xAOD::EnergySumRoI*           cmet_l1_roi = 0;
    const xAOD::TrigMissingETContainer* cmet_hlt_EFJetEtSum = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_T2MissingET = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigL2MissingET_FEB = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET_mht = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET_FEB = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET_topocl = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET_topocl_PS = 0;    
    const xAOD::TrigMissingETContainer* cmet_hlt_TrigEFMissingET_topocl_PUC = 0;    

    if(!m_isderived){
      
      CHECK( m_event->retrieve( cmet_l1_roi, "LVL1EnergySumRoI"));
      if(cmet_l1_roi){
	trig_l1_sumet    =  (cmet_l1_roi->energyT())*0.001;
	trig_l1_ex    =  - (cmet_l1_roi->energyX())*0.001;
	trig_l1_ey    =  - (cmet_l1_roi->energyY())*0.001;
	trig_l1_et =	 sqrt(trig_l1_ex*trig_l1_ex + trig_l1_ey*trig_l1_ey);
	trig_l1_phi   =	 atan2(trig_l1_ey, trig_l1_ex);
      }
      
      CHECK( m_event->retrieve( cmet_hlt_EFJetEtSum, "HLT_xAOD__TrigMissingETContainer_EFJetEtSum") );
      if(cmet_hlt_EFJetEtSum->size()){
	trig_hlt_EFJetEtSum_ex = cmet_hlt_EFJetEtSum->at(0)->ex()*0.001;
	trig_hlt_EFJetEtSum_ey = cmet_hlt_EFJetEtSum->at(0)->ey()*0.001;
	trig_hlt_EFJetEtSum_et = TMath::Hypot(cmet_hlt_EFJetEtSum->at(0)->ex(), cmet_hlt_EFJetEtSum->at(0)->ey())*0.001;
	trig_hlt_EFJetEtSum_sumet = cmet_hlt_EFJetEtSum->at(0)->sumEt()*0.001;
	trig_hlt_EFJetEtSum_phi = atan2(cmet_hlt_EFJetEtSum->at(0)->ey(), cmet_hlt_EFJetEtSum->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_T2MissingET, "HLT_xAOD__TrigMissingETContainer_T2MissingET") );
      if(cmet_hlt_T2MissingET->size()){
	trig_hlt_T2MissingET_ex = cmet_hlt_T2MissingET->at(0)->ex()*0.001;
	trig_hlt_T2MissingET_ey = cmet_hlt_T2MissingET->at(0)->ey()*0.001;
	trig_hlt_T2MissingET_et = TMath::Hypot(cmet_hlt_T2MissingET->at(0)->ex(), cmet_hlt_T2MissingET->at(0)->ey())*0.001;
	trig_hlt_T2MissingET_sumet = cmet_hlt_T2MissingET->at(0)->sumEt()*0.001;
	trig_hlt_T2MissingET_phi = atan2(cmet_hlt_T2MissingET->at(0)->ey(), cmet_hlt_T2MissingET->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC, "HLT_xAOD__TrigMissingETContainer_EFMissingET_Fex_2sidednoiseSupp_PUC") );
      if(cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->size()){
	trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ex = cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ex()*0.001;
	trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_ey = cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ey()*0.001;
	trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_et = TMath::Hypot(cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ex(), cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ey())*0.001;
	trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_sumet = cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->sumEt()*0.001;
	trig_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC_phi = atan2(cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ey(), cmet_hlt_EFMissingET_Fex_2sidednoiseSupp_PUC->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigL2MissingET_FEB, "HLT_xAOD__TrigMissingETContainer_TrigL2MissingET_FEB") );
      if(cmet_hlt_TrigL2MissingET_FEB->size()){
        trig_hlt_TrigL2MissingET_FEB_ex = cmet_hlt_TrigL2MissingET_FEB->at(0)->ex()*0.001;
        trig_hlt_TrigL2MissingET_FEB_ey = cmet_hlt_TrigL2MissingET_FEB->at(0)->ey()*0.001;
        trig_hlt_TrigL2MissingET_FEB_et = TMath::Hypot(cmet_hlt_TrigL2MissingET_FEB->at(0)->ex(), cmet_hlt_TrigL2MissingET_FEB->at(0)->ey())*0.001;
        trig_hlt_TrigL2MissingET_FEB_sumet = cmet_hlt_TrigL2MissingET_FEB->at(0)->sumEt()*0.001;
        trig_hlt_TrigL2MissingET_FEB_phi = atan2(cmet_hlt_TrigL2MissingET_FEB->at(0)->ey(), cmet_hlt_TrigL2MissingET_FEB->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET_FEB, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_FEB") );
      if(cmet_hlt_TrigEFMissingET_FEB->size()){
	trig_hlt_TrigEFMissingET_FEB_ex = cmet_hlt_TrigEFMissingET_FEB->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_FEB_ey = cmet_hlt_TrigEFMissingET_FEB->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_FEB_et = TMath::Hypot(cmet_hlt_TrigEFMissingET_FEB->at(0)->ex(), cmet_hlt_TrigEFMissingET_FEB->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_FEB_sumet = cmet_hlt_TrigEFMissingET_FEB->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_FEB_phi = atan2(cmet_hlt_TrigEFMissingET_FEB->at(0)->ey(), cmet_hlt_TrigEFMissingET_FEB->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET") );
      if(cmet_hlt_TrigEFMissingET->size()){
	trig_hlt_TrigEFMissingET_ex = cmet_hlt_TrigEFMissingET->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_ey = cmet_hlt_TrigEFMissingET->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_et = TMath::Hypot(cmet_hlt_TrigEFMissingET->at(0)->ex(), cmet_hlt_TrigEFMissingET->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_sumet = cmet_hlt_TrigEFMissingET->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_phi = atan2(cmet_hlt_TrigEFMissingET->at(0)->ey(), cmet_hlt_TrigEFMissingET->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET_mht, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_mht") );
      if(cmet_hlt_TrigEFMissingET_mht->size()){
	trig_hlt_TrigEFMissingET_mht_ex = cmet_hlt_TrigEFMissingET_mht->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_mht_ey = cmet_hlt_TrigEFMissingET_mht->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_mht_et = TMath::Hypot(cmet_hlt_TrigEFMissingET_mht->at(0)->ex(), cmet_hlt_TrigEFMissingET_mht->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_mht_sumet = cmet_hlt_TrigEFMissingET_mht->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_mht_phi = atan2(cmet_hlt_TrigEFMissingET_mht->at(0)->ey(), cmet_hlt_TrigEFMissingET_mht->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET_topocl, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_topocl") );
      if(cmet_hlt_TrigEFMissingET_topocl->size()){
	trig_hlt_TrigEFMissingET_topocl_ex = cmet_hlt_TrigEFMissingET_topocl->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_topocl_ey = cmet_hlt_TrigEFMissingET_topocl->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_topocl_et = TMath::Hypot(cmet_hlt_TrigEFMissingET_topocl->at(0)->ex(), cmet_hlt_TrigEFMissingET_topocl->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_topocl_sumet = cmet_hlt_TrigEFMissingET_topocl->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_topocl_phi = atan2(cmet_hlt_TrigEFMissingET_topocl->at(0)->ey(), cmet_hlt_TrigEFMissingET_topocl->at(0)->ex());
	
	
	//DEBUGGING
	// cout << "- DEBUG -------------------------------------------" << endl;
	// for(auto ii=0; ii < cmet_hlt_TrigEFMissingET_topocl->at(0)->getNumberOfComponents(); ii++){
	// 	cout << "MET component (" << ii << "/" << cmet_hlt_TrigEFMissingET_topocl->at(0)->getNumberOfComponents() << ") = " << cmet_hlt_TrigEFMissingET_topocl->at(0)->nameOfComponent(ii) <<  endl;
	// 	cout << "-------------------------------------------------" << endl;
	// 	cout << " MEtx  = " << cmet_hlt_TrigEFMissingET_topocl->at(0)->exComponent(ii)*0.001 << endl;
	// 	cout << " MEty  = " << cmet_hlt_TrigEFMissingET_topocl->at(0)->eyComponent(ii)*0.001 << endl;
	// 	cout << " MEt   = " << TMath::Hypot(cmet_hlt_TrigEFMissingET_topocl->at(0)->exComponent(ii), cmet_hlt_TrigEFMissingET_topocl->at(0)->eyComponent(ii))*0.001 << endl;
	// 	cout << " SumEt = " << cmet_hlt_TrigEFMissingET_topocl->at(0)->sumEtComponent(ii)*0.001 << endl;
	// 	cout << " phi   = " << atan2(cmet_hlt_TrigEFMissingET_topocl->at(0)->eyComponent(ii), cmet_hlt_TrigEFMissingET_topocl->at(0)->exComponent(ii)) << endl;
	// }
	
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET_topocl_PS, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_topocl_PS") );
      if(cmet_hlt_TrigEFMissingET_topocl_PS->size()){
	trig_hlt_TrigEFMissingET_topocl_PS_ex = cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PS_ey = cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PS_et = TMath::Hypot(cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ex(), cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_topocl_PS_sumet = cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PS_phi = atan2(cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ey(), cmet_hlt_TrigEFMissingET_topocl_PS->at(0)->ex());
      }
      
      CHECK( m_event->retrieve( cmet_hlt_TrigEFMissingET_topocl_PUC, "HLT_xAOD__TrigMissingETContainer_TrigEFMissingET_topocl_PUC") );
      if(cmet_hlt_TrigEFMissingET_topocl_PUC->size()){
	trig_hlt_TrigEFMissingET_topocl_PUC_ex = cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ex()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PUC_ey = cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ey()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PUC_et = TMath::Hypot(cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ex(), cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ey())*0.001;
	trig_hlt_TrigEFMissingET_topocl_PUC_sumet = cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->sumEt()*0.001;
	trig_hlt_TrigEFMissingET_topocl_PUC_phi = atan2(cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ey(), cmet_hlt_TrigEFMissingET_topocl_PUC->at(0)->ex());
      }
    }
  }

  //QCD Trigger stuff...  //FIX_ME    once we have access to trigger decision!
  if ( this->isQCD  && ! isQCDSeedEvent(0., 0., QCD_METSig) ){ //FIX !! ( met, sumet, QCD_METSig) ){
    //    output->setFilterPassed(false);
    return nextEvent();
  }


  //---Fill missing data members 
  this->dumpLeptons();
  this->dumpPhotons();
  this->dumpJets();
  
  //Redefine run number for MC, to reflect the channel_number instead //CHECK_ME
  if(isMC) RunNumber = mc_channel_number;
  
  // The containers created by the shallow copy are owned by you. Remember to delete them
  // The containers created by the shallow copy are owned by you. Remember to delete them
  delete jets_sc;              delete jets_scaux;
  delete muons_sc;             delete muons_scaux;
  delete electrons_sc;         delete electrons_scaux;
  delete photons_sc;           delete photons_scaux;
  delete metRFC;               delete metRFCAux; 

  if(myfile.is_open())
    myfile.close();

  //output->setFilterPassed (true);
  m_atree->Fill();
  return nextEvent(); //SUCCESS + cleaning
}




//****************************//
//*** TRUTH LEVEL ANALYSIS ***//
//****************************//
EL::StatusCode chorizo :: loop_truth()
{
  //  Info("loop_truth()", "Inside loop_truth");

  InitVars();

  if ( m_eventCounter%1000==0 ){ 
    printf("\r %6d/%lld\t[%3.0f]",m_eventCounter,m_event->getEntries(),100*m_eventCounter/float(m_event->getEntries()));
    fflush(stdout);
  }

  m_eventCounter++;
  
  //save dummy meta_data
  if(m_eventCounter<2){
    //save Trigger metadata
    std::string trigchains="";
    for(const auto& s : TriggerNames){
      trigchains += (s+",");
    }
    meta_triggers = new TNamed("Triggers", trigchains.c_str());
    wk()->addOutput(meta_triggers);     
  }


  //----------------------------
  // Event information
  //---------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  CHECK( m_event->retrieve( eventInfo, "EventInfo") );

  RunNumber = eventInfo->runNumber();
  mc_channel_number = (isMC ? eventInfo->mcChannelNumber() : 0); 
  EventNumber = eventInfo->eventNumber();

  //  loadMetaData();

  //--- Weigths for Sherpa, MC@NLO, Acer, ...
  MC_w = eventInfo->mcEventWeight();

  //-- Retrieve objects Containers
  m_truthE= 0;
  m_truthP= 0;
  m_truth_jets= 0;

  //  -- Event presel  
  isVertexOk = true; //dummy 

 
  //  -- Truth Particles
  //  CHECK( m_event->retrieve( m_truthE, "TruthEvents" ) );
  //  CHECK( m_event->retrieve( m_truthP, "TruthParticles" ) );

  // -- Find Signal Subprocess

  //find Hard Process particles
  int id1=0; int id2=0;
  //  bool spOK = ST::SUSYObjDef_xAOD::FindSusyHardProc(m_truthP, id1, id2);  //CHECK :   what we can do with this without TruthParticles (TRUTH3)??
  bool spOK = false;
  
  //get procID
  if(spOK && id1!=0 && id2!=0) //(just to avoid warnings)
    procID = SUSY::finalState(id1, id2); // get prospino proc ID
  

  //Truth Electrons 
  CHECK( m_event->retrieve( m_truthEl, "TruthElectrons" ) );

  std::vector<Particle> electronCandidates; //intermediate selection electrons

  for(const xAOD::TruthParticle* tEl : *m_truthEl) {

      // FS selection
      if( !isStable( tEl ) ) continue; 
      
      //define preselected electron                
      Particle recoElectron;

      TLorentzVector tlv_d;
      if( dressLeptons ){
	tlv_d.SetPtEtaPhiE( tEl->auxdata< float >( "pt_dressed" )*0.001,
			    tEl->auxdata< float >( "eta_dressed" ),
			    tEl->auxdata< float >( "phi_dressed" ),
			    tEl->auxdata< float >( "e_dressed" )*0.001);
      }
      else{
	tlv_d.SetPtEtaPhiE( tEl->pt()*0.001,
			    tEl->eta(),
			    tEl->phi(),
			    tEl->e()*0.001);
      }

      recoElectron.SetVector( tlv_d, true );

      if (recoElectron.Pt() < El_PreselPtCut/1000.)   continue;
      if (fabs(recoElectron.Eta()) > El_PreselEtaCut) continue;
         

      recoElectron.etcone20 = (tEl->isAvailable<float>("etcone20") ? tEl->auxdata< float >( "etcone20" )*0.001 : -999.);
      recoElectron.ptcone30 = (tEl->isAvailable<float>("ptcone30") ? tEl->auxdata< float >( "ptcone30" )*0.001 : -999.);
      
      recoElectron.type   = (tEl->isAvailable<unsigned int>("particleType")   ? tEl->auxdata<unsigned int>("particleType")   : tEl->auxdata<unsigned int>("classifierParticleType"));
      recoElectron.origin = (tEl->isAvailable<unsigned int>("particleOrigin") ? tEl->auxdata<unsigned int>("particleOrigin") : tEl->auxdata<unsigned int>("classifierParticleOrigin"));


    //get only real electrons ( ! (bkg || noniso) )
    if( recoElectron.type != MCTruthPartClassifier::BkgElectron && recoElectron.type != MCTruthPartClassifier::NonIsoElectron ){
            
      electronCandidates.push_back(recoElectron);
    }

  }//electron loop

  //sort the electrons in Pt
  if (electronCandidates.size()>0) std::sort(electronCandidates.begin(), electronCandidates.end());


  //Truth Muons 
  CHECK( m_event->retrieve( m_truthMu, "TruthMuons" ) );

  std::vector<Particle> muonCandidates; //intermediate selection muons
  for(const xAOD::TruthParticle* tMu : *m_truthMu) {

    // FS selection
    if( !isStable( tMu ) ) continue; 

    //define preselected muon                
    Particle recoMuon;
    
    TLorentzVector tlv_d;
    if( dressLeptons ){
      tlv_d.SetPtEtaPhiE( tMu->auxdata< float >( "pt_dressed" )*0.001,
			  tMu->auxdata< float >( "eta_dressed" ),
			  tMu->auxdata< float >( "phi_dressed" ),
			  tMu->auxdata< float >( "e_dressed" )*0.001);
    }
    else{
      tlv_d.SetPtEtaPhiE( tMu->pt()*0.001,
			  tMu->eta(),
			  tMu->phi(),
			  tMu->e()*0.001);
    }
    
    recoMuon.SetVector( tlv_d, true );
    if (recoMuon.Pt() < Mu_PreselPtCut/1000.)   continue;
    if (fabs(recoMuon.Eta()) > Mu_PreselEtaCut) continue;
    
    recoMuon.etcone20 = (tMu->isAvailable<float>("etcone20") ? tMu->auxdata< float >( "etcone20" )*0.001 : -999.);
    recoMuon.ptcone30 = (tMu->isAvailable<float>("ptcone30") ? tMu->auxdata< float >( "ptcone30" )*0.001 : -999.);

    recoMuon.type   = (tMu->isAvailable<unsigned int>("particleType")   ? tMu->auxdata< unsigned int >("particleType")   : tMu->auxdata<unsigned int>("classifierParticleType"));
    recoMuon.origin = (tMu->isAvailable<unsigned int>("particleOrigin") ? tMu->auxdata< unsigned int >("particleOrigin") : tMu->auxdata<unsigned int>("classifierParticleOrigin"));


    //get only real muons ( ! (bkg || noniso) )
    if( recoMuon.type != MCTruthPartClassifier::BkgMuon && recoMuon.type != MCTruthPartClassifier::NonIsoMuon ){
      
      muonCandidates.push_back(recoMuon);
    }
        
  }//muon loop
  
  //sort the muon candidates in Pt
  if (muonCandidates.size()>0) std::sort(muonCandidates.begin(), muonCandidates.end());


  //Truth Photons 
  if (usePhotons){
    if( !m_event->retrieve( m_truthPh, "TruthPhotons").isSuccess() ){
      if( !m_event->retrieve( m_truthPh, "Truth3Photons").isSuccess() ){
	Error("loop_truth()", "Failed to retrieve Truth(3)Photon container. Exiting." );
	return EL::StatusCode::FAILURE;
      }
    }
    //CHECK( m_event->retrieve( m_truthPh, "TruthPhotons" ) );
    //CHECK( m_event->retrieve( m_truthPh, "Truth3Photons" ) );
  }//usephotons
  
  std::vector<Particle> photonCandidates; //intermediate selection photons
  if (usePhotons){
    for(const xAOD::TruthParticle* tPh : *m_truthPh) {
      
      // FS selection
      if( !isStable( tPh ) ) continue; 
      
      //define preselected photon                
      Particle recoPhoton;
      recoPhoton.SetVector( getTLV( tPh ) );
      if (recoPhoton.Pt() < El_PreselPtCut/1000.)   continue;
      if (fabs(recoPhoton.Eta()) > El_PreselEtaCut) continue;
      
      
      recoPhoton.etcone20 = (tPh->isAvailable<float>("etcone20") ? tPh->auxdata< float >( "etcone20" )*0.001 : -999.);
      recoPhoton.ptcone30 = (tPh->isAvailable<float>("ptcone30") ? tPh->auxdata< float >( "ptcone30" )*0.001 : -999.);

      recoPhoton.type   = (tPh->isAvailable<unsigned int>("particleType")   ? tPh->auxdata<unsigned int>("particleType")   : tPh->auxdata<unsigned int>("classifierParticleType"));
      recoPhoton.origin = (tPh->isAvailable<unsigned int>("particleOrigin") ? tPh->auxdata<unsigned int>("particleOrigin") : tPh->auxdata<unsigned int>("classifierParticleOrigin"));

      //get only real photons (prompt+brem+ISR/FSR)
      if(recoPhoton.type == MCTruthPartClassifier::BremPhot   ||
	 recoPhoton.type == MCTruthPartClassifier::PromptPhot ||
	 recoPhoton.type == MCTruthPartClassifier::UndrPhot   ||
	 recoPhoton.type == MCTruthPartClassifier::ISRPhot    ||
	 recoPhoton.type == MCTruthPartClassifier::FSRPhot){
      
	photonCandidates.push_back(recoPhoton);

      }
      
    }//photon loop
    
    //sort the photons in Pt
    if (photonCandidates.size()>0) std::sort(photonCandidates.begin(), photonCandidates.end());
  }//usephotons
  
  
  //Truth Jets
  CHECK( m_event->retrieve( m_truth_jets, "AntiKt4TruthJets" ) );
  
  std::vector<Particles::Jet> jetCandidates; //intermediate selection photons
  for(const xAOD::Jet* tJet : *m_truth_jets) {
    
    Particles::Jet recoJet;
    recoJet.SetVector( getTLV( tJet ));
    if (recoJet.Pt() < Jet_PreselPtCut/1000.)   continue;
    if (fabs(recoJet.Eta()) > Jet_PreselEtaCut) continue;
    
    try{
      recoJet.FlavorTruth = tJet->getAttribute<int>("HadronConeExclTruthLabelID"); 
    }
    catch(...){
      try{
	recoJet.FlavorTruth = tJet->getAttribute<int>("PartonTruthLabelID"); 
      }
      catch(...){
	recoJet.FlavorTruth = 0;
      }
    }
    
    //hack for cutflow afterwards
    if(recoJet.FlavorTruth==5){
      recoJet.MV1 = 99.;
      recoJet.MV2c20 = 99.;
    }
    else{
      recoJet.MV1 = -99.;
      recoJet.MV2c20 = -99.;
    }
    
    // tool_btag_truth1->setRandomSeed(int( 1e5 + 5000 * fabs(tJet->eta()))); //set a unique seed for each jet                                                             
    // tool_btag_truth2->setRandomSeed(int( 1e5 + 5000 * fabs(tJet->eta())));
    // tool_btag_truth3->setRandomSeed(int( 1e5 + 5000 * fabs(tJet->eta())));
    
    // recoJet.isbjet_t70 = tool_btag_truth1->performTruthTagging(tJet);
    // recoJet.isbjet_t77 = tool_btag_truth2->performTruthTagging(tJet);
    // recoJet.isbjet_t80 = tool_btag_truth3->performTruthTagging(tJet);
    
    //emulate btagging efficiency! (77% here)
    if( simBtagging ){
      float bprob = gRandom->Rndm(1);
      if(recoJet.FlavorTruth==5){
	if(bprob<0.77){
	  recoJet.FlavorTruth = 5;
	  recoJet.MV1 = 99.;
	  recoJet.MV2c20 = -0.1;
	}
	else{
	  recoJet.FlavorTruth = 0;
	  recoJet.MV1 = -99.;
	  recoJet.MV2c20 = -99.;
	}
      }
      else if (recoJet.FlavorTruth==4){
	if(bprob<0.22){
	  recoJet.FlavorTruth = 5;
	  recoJet.MV1 = 99.;
	  recoJet.MV2c20 = -0.1;
	}
	else{
	  recoJet.FlavorTruth = 4;
	  recoJet.MV1 = -99.;
	  recoJet.MV2c20 = -99.;
	}
      }
      else if (recoJet.FlavorTruth==15){
	if(bprob<0.1){
	  recoJet.FlavorTruth = 5;
	  recoJet.MV1 = 99.;
	  recoJet.MV2c20 = -0.1;
	}
	else{
	  recoJet.FlavorTruth = 15;
	  recoJet.MV1 = -99.;
	  recoJet.MV2c20 = -99.;
	}
      }
      else{
	if(bprob<0.007){
	  recoJet.FlavorTruth = 5;
	  recoJet.MV1 = 99.;
	  recoJet.MV2c20 = -0.1;
	}
	else{
	  recoJet.FlavorTruth = 0;
	  recoJet.MV1 = -99.;
	  recoJet.MV2c20 = -99.;
	}
      }
    }
    
    recoJet.isbjet = recoJet.isBTagged_77fc("Truth");
    
    jetCandidates.push_back(recoJet);
    
    //apply signal cuts...
    if ( recoJet.Pt() < (Jet_RecoPtCut/1000.) ) continue;
    if ( fabs(recoJet.Eta()) > Jet_RecoEtaCut ) continue;
    
    if ( recoJet.isbjet ){
      bj_N_77fc++;
      bj_N_85fc++;
      bj_N_70fc++;      
    }
    
    //check for higher pt jet multiplicity
    if(recoJet.Pt() > 30.){
      j_N30++;
      if(recoJet.Pt() > 40.){
	j_N40++;
	if(recoJet.Pt() > 50.){
	  j_N50++;
	  if(recoJet.Pt() > 60.){
	    j_N60++;
	    if(recoJet.Pt() > 80.){
	      j_N80++;
	    }
	  }
	}
      }
    }

    //check for overlapping jets
    if(doOR){
      bool overlap=false;
      for(const auto& c_el : electronCandidates){
	if(recoJet.GetVector().DeltaR(c_el) < 0.2){
	  overlap=true;
	  break;
	}
      }
      if(overlap) continue;
      
      if (usePhotons && doORphotons){
	for(const auto& c_ph : photonCandidates){
	  if(recoJet.GetVector().DeltaR(c_ph) < 0.4){
	    overlap=true;
	    break;
	  }
	}
      }//usephoton

      if(overlap) continue;
    }
    
    recoJets.push_back(recoJet);
    j_N++;
  }

  //sort the jet candidates in Pt
  if (jetCandidates.size() > 0) std::sort(jetCandidates.begin(), jetCandidates.end());


  //Remove leptons/photons overlapping with jets now
  for(const auto& c_el : electronCandidates){
    
    if(doOR){
      bool overlap=false;
      for(const auto& c_jet : recoJets){
	
	if( c_el.GetVector().DeltaR(c_jet) < 0.4 ){
	  overlap=true;
	  break;
	}
      }
      if(overlap) continue;
    }

    //apply signal cuts...
    //if(...)

    recoElectrons.push_back(c_el);
    e_N++;
  }  
  //sort the electrons in Pt (they should be already, but anyways)
  if (recoElectrons.size()>0) std::sort(recoElectrons.begin(), recoElectrons.end());


  if (usePhotons && doORphotons){
    for(const auto& c_ph : photonCandidates){
      
      if(doOR){    
	bool overlap=false;
	for(const auto& c_jet : recoJets){
	  if( c_ph.GetVector().DeltaR(c_jet) < 0.4 ){
	    overlap=true;
	    break;
	  }
	}
	if(overlap) continue;
      }
      
      //apply signal cuts...
      //if(...)
      
      recoPhotons.push_back(c_ph);
      ph_N++;
    }  
    //sort the photons in Pt (they should be already, but anyways)
    if (recoPhotons.size()>0) std::sort(recoPhotons.begin(), recoPhotons.end());
  }//usephoton
  
  //just to TEST directHG thing
  // if(recoPhotons.size()>1){
  //   double mgg = (recoPhotons.at(0).GetVector()+recoPhotons.at(1).GetVector()).M();
  //   //cout << "INV MGG = " << mgg << endl;
  //   pileup_w = mgg;   //TAKE THIS OUT!!
  // }

  for(const auto& c_mu : muonCandidates){

    if(doOR){    
      bool overlap=false;
      for(const auto& c_jet : recoJets){
	if( c_mu.GetVector().DeltaR(c_jet) < 0.4 ){
	  overlap=true;
	  break;
	}
      }
      if(overlap) continue;
    }

    //apply signal cuts...
    //if(...)
    
    recoMuons.push_back(c_mu);
    m_N++;
  }  
  //sort the muons in Pt (they should be already, but anyways)
  if (recoMuons.size()>0) std::sort(recoMuons.begin(), recoMuons.end());


  //MET
  // NOTE:: Here MET could be recomputed with dressed leptons, as in SUSYTools, but not yet...
  const xAOD::MissingETContainer* cmet_truth;
  CHECK( m_event->retrieve( cmet_truth, "MET_Truth") );

  const xAOD::MissingET* mtruth_inv = (*cmet_truth)["Int"];
  const xAOD::MissingET* mtruth_vis = (*cmet_truth)["NonInt"];

  met_obj.SetVector(TVector2(mtruth_inv->mpx(), mtruth_inv->mpy()), "met_truth_imu");  //- Copy met vector to the met data member
  
  met_obj.SetVector(TVector2(mtruth_vis->mpx(), mtruth_vis->mpy()), "met_truth_vmu"); //to be redone with dressed leptons?


  // book MET flavours (to global map)
  metmap={};
  metmap[::MetDef::InvMuTruth] = met_obj.GetVector("met_truth_imu");
  metmap[::MetDef::VisMuTruth] = met_obj.GetVector("met_truth_vmu");

  //meta data (fill it once)
  if(smetmap==""){ 
    for (auto& mk : metmap){ 
      smetmap += sMetDef[(unsigned int)mk.first];
      smetmap += ",";
    }
    meta_metmap = new TNamed("METmap", smetmap.c_str());
    wk()->addOutput(meta_metmap);     
  }

  //Recoiling system against MET (prebooking)
  TLorentzVector sjet(0.,0.,0.,0.);
  bool LeadOnly=true; //use only the leading jet 
  std::vector<TLorentzVector> thrust_jets;
  if(LeadOnly && recoJets.size()) sjet = recoJets.at(0).GetVector();   
  for(const auto& jet : recoJets){
    if(!LeadOnly) sjet += jet.GetVector();
    HT   += jet.Pt();
    thrust_jets.push_back( jet.GetVector());
  }

  TVector2 sjet2(sjet.Px(), sjet.Py());

  //--- Fill MET related variables  -  vectorized now!
  for (const auto& mk : metmap) {

    met.push_back(mk.second.Mod());
    met_phi.push_back( TVector2::Phi_mpi_pi( mk.second.Phi() ) ); //--- Phi defined between -pi and pi
    
    dPhi_met_mettrk.push_back(deltaPhi(metmap[MetDef::Track].Phi(), mk.second.Phi())); 
    
    //Recoiling system against MET (prebooking)
    rmet_par.push_back( sjet2.Mod() * TMath::Cos(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) ); //leading jet only
    rmet_norm.push_back( sjet2.Mod() * TMath::Sin(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_par_mod.push_back( TMath::Cos(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_norm_mod.push_back( TMath::Sin(deltaPhi(sjet2.Phi(), mk.second.Rotate(TMath::Pi()).Phi())) );
    rmet_dPhi_met_jetsys.push_back( deltaPhi(sjet2.Phi(), mk.second.Phi()) ); 


    //sphericity and thrust
    thrust_jets.push_back( makeTLV(mk.second) ); //add met to jet vectors

    tr_spher.push_back( Calc_Sphericity(thrust_jets, true) );
    tr_thrust.push_back( Calc_Thrust(thrust_jets) );

    thrust_jets.pop_back(); //remove met from jet vectors

    if (recoJets.size()>=2 && (recoElectrons.size()>=1 || recoMuons.size()>=1)){
        
      TLorentzVector lead_lep(0.,0.,0.,0.); 
      if (recoMuons.size()==0) lead_lep = recoElectrons.at(0).GetVector(); 
      else if (recoElectrons.size()==0) lead_lep = recoMuons.at(0).GetVector();
      else {
        if (recoMuons.at(0).Pt()>recoElectrons.at(0).Pt()) lead_lep = recoMuons.at(0).GetVector();
        if (recoMuons.at(0).Pt()<recoElectrons.at(0).Pt()) lead_lep = recoElectrons.at(0).GetVector();	
      }
      
      
      amt2_0.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 160));
      amt2_1.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 170));
      amt2_2.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 180));
      amt2_3.push_back(amt2_calc(recoJets.at(0).GetVector(), recoJets.at(1).GetVector(), lead_lep,mk.second, 200));
    }

    auto ntaujs =0;
    auto dphi_min_allj_tmp=-1.;
    auto dphi_min_4j_tmp=-1.;    
    auto dphi_min_tmp=-1.;
    int ibcl=-1;  //closest bjet to met
    int ibfar=-1; //most faraway bjet from met
    int ilcl=-1;  //closest light jet to met
    float min_dphi_bm=999.;
    float max_dphi_bm=0.;
    float min_dphi_lm=999.;

    auto min_mt_jm=99999.;
    auto ijet=0;
    for( auto jet : recoJets ){  //jet loop

      //count 'tau' jets candidates
      if ( jet.isTauJet( mk.second.Phi(), Jet_Tagger.Data()) ) 
	ntaujs += 1;

      //look for min dphi(jet,met)   
      float dphi_jm = deltaPhi( jet.Phi(), mk.second.Phi() );
      
      if(dphi_min_allj_tmp < 0 || dphi_min_allj_tmp > deltaPhi(jet.Phi(), mk.second.Phi()) ) //closest jet to met (all)
	dphi_min_allj_tmp =  dphi_jm;   //deltaPhi(recoJets.at(ijet).Phi(), met_phi[0]);
      
      if( ijet < 3 && (dphi_min_tmp < 0 || dphi_min_tmp > dphi_jm) ) //closest jet to met (3-leading only)
	dphi_min_tmp = dphi_jm;

      if( ijet < 4 && (dphi_min_4j_tmp < 0 || dphi_min_4j_tmp > dphi_jm) ) //closest jet to met (3-leading only)
	dphi_min_4j_tmp = dphi_jm;

      //--- look for min MT(jet,MET)
      float mt_jm = Calc_MT( jet, mk.second );
      if(mt_jm < min_mt_jm){
	min_mt_jm = mt_jm;
      }
      
      
      //--- look for closest/faraway bjet and closer light jet to MET
      if( jet.isbjet && fabs(jet.Eta())<2.5){
	
	if( dphi_jm < min_dphi_bm){ //closest bjet
	  min_dphi_bm = dphi_jm;
	  ibcl = ijet;
	}
	
	if( dphi_jm > max_dphi_bm){ //most faraway bjet
	  max_dphi_bm = dphi_jm;
	  ibfar = ijet;
	}
	
      }
      else{
	if( dphi_jm < min_dphi_lm){ //closest light-jet
	  min_dphi_lm = dphi_jm;
	  ilcl = ijet;
	}
      }
      
      ijet++;
    }
    dPhi_min_alljets.push_back(dphi_min_allj_tmp);
    dPhi_min_4jets.push_back(dphi_min_4j_tmp);    
    dPhi_min.push_back(dphi_min_tmp);

    if( min_mt_jm==999999. )
      min_mt_jm=0.; //it shouldn't matter, but let's force zero-jets events to fail this!

    //fill MT variables from found values above
    MT_min_jet_met.push_back( min_mt_jm );
    MT_bcl_met.push_back( (ibcl >= 0 ? Calc_MT( recoJets.at(ibcl), mk.second ): 0.) );
    MT_bfar_met.push_back( (ibfar >= 0 ? Calc_MT( recoJets.at(ibfar), mk.second ) : 0.) );
    MT_lcl_met.push_back( (ilcl >= 0 ? Calc_MT( recoJets.at(ilcl), mk.second ) : 0.) );
    MT_jsoft_met.push_back( (recoJets.size() > 0 ? Calc_MT( recoJets.back(), mk.second ) : 0.) ); //it assumes jets are pt ordered!


    //dphi(jet_i, met+j) ,  MT(j_i, met_j)
    if (j_N>0){
      dPhi_met_j1.push_back( deltaPhi( mk.second.Phi(), recoJets.at(0).Phi()) );	
      j1_mT.push_back( Calc_MT( recoJets.at(0), mk.second ));
      if (j_N>1){
	dPhi_met_j2.push_back( deltaPhi( mk.second.Phi(), recoJets.at(1).Phi()) );	
	j2_mT.push_back( Calc_MT( recoJets.at(1), mk.second ));
	if (j_N>2){
	  dPhi_met_j3.push_back( deltaPhi( mk.second.Phi(), recoJets.at(2).Phi()) );	
	  j3_mT.push_back( Calc_MT( recoJets.at(2), mk.second ));
	  if (j_N>3){
	    dPhi_met_j4.push_back( deltaPhi( mk.second.Phi(), recoJets.at(3).Phi()) );	
	    j4_mT.push_back( Calc_MT( recoJets.at(3), mk.second ));
	  }
	}
      }
    }

    //meff = HT + met
    meff.push_back( HT + mk.second.Mod() );
    
    //tau jet candidates
    j_tau_N.push_back( ntaujs );

    //==== Razor ====
    fillRazor( makeV3( mk.second ) );

  }//end of met flavor loop

  //  met_lochadtopo = met_obj.GetVector("met_locHadTopo").Mod();


  //--- Some other event variables
  
  //fill info for truth b-sb decay
  findBparton();  //FIX_ME  //needed?
  //*** otherwise t_b_*** variables are not filled otherwise!
  
  //--- Invariant mass of the 2 first jets (M12)
  if(j_N>1){
    TLorentzVector jet_M = (recoJets[0].GetVector() + recoJets[1].GetVector());
    M12 = jet_M.M();
  }
  
  //- dPhi & dR
  //dPhi_met_mettrk = deltaPhi(metmap[MetDef::Track].Phi(), metmap[MetDef::InvMu].Phi());
  
  if (j_N>0){

    dPhi_j1_bp1 = deltaPhi(recoJets.at(0).Phi(), t_b_phi1);
    dPhi_j1_bp2 = deltaPhi(recoJets.at(0).Phi(), t_b_phi2);
    dR_j1_bp1   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi1, t_b_eta1);
    dR_j1_bp2   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi2, t_b_eta2);
    
    if (recoMuons.size()>0){
      dR_j1_m1 = recoJets.at(0).DeltaR(recoMuons.at(0));
      if (recoMuons.size()>1) 
	dR_j1_m2 = recoJets.at(0).DeltaR(recoMuons.at(1));
    }
    
    if (j_N>1){
      
      dPhi_j1_j2  = deltaPhi(recoJets.at(0).Phi(), recoJets.at(1).Phi());
      dPhi_j2_bp1 = deltaPhi(recoJets.at(1).Phi(), t_b_phi1);
      dPhi_j2_bp2 = deltaPhi(recoJets.at(1).Phi(), t_b_phi2);
      dEta_j1_j2  = fabs(recoJets.at(0).Eta()-recoJets.at(1).Eta());
      
      dR_j1_j2    = recoJets.at(0).DeltaR(recoJets.at(1));
      dR_j2_bp1   = deltaR(recoJets.at(1).Phi(),recoJets.at(1).Eta(),t_b_phi1,t_b_eta1);
      dR_j2_bp2   = deltaR(recoJets.at(1).Phi(),recoJets.at(1).Eta(),t_b_phi2,t_b_eta2);

      if (recoMuons.size()>0){
	dR_j2_m1 = recoJets.at(1).DeltaR(recoMuons.at(0));
	if (recoMuons.size()>1) 
	  dR_j2_m2=recoJets.at(1).DeltaR(recoMuons.at(1));
      }

      if (j_N>2){
	dPhi_j1_j3  = deltaPhi(recoJets.at(0).Phi(), recoJets.at(2).Phi());
	dPhi_j2_j3  = deltaPhi(recoJets.at(1).Phi(), recoJets.at(2).Phi());
	dPhi_j3_bp1 = deltaPhi(recoJets.at(2).Phi(), t_b_phi1);
	dPhi_j3_bp2 = deltaPhi(recoJets.at(2).Phi(), t_b_phi2);

	dR_j1_j3    = recoJets.at(0).DeltaR(recoJets.at(2));
	dR_j2_j3    = recoJets.at(1).DeltaR(recoJets.at(2));
	dR_j3_bp1   = deltaR(recoJets.at(2).Phi(),recoJets.at(2).Eta(),t_b_phi1,t_b_eta1);
	dR_j3_bp2   = deltaR(recoJets.at(2).Phi(),recoJets.at(2).Eta(),t_b_phi2,t_b_eta2);

	if(recoMuons.size()>0){
	  dR_j3_m1  = recoJets.at(2).DeltaR(recoMuons.at(0));
	  if(recoMuons.size()>1) 
	    dR_j3_m2 = recoJets.at(2).DeltaR(recoMuons.at(1));
	}
      }
    }
  }
  

  
  //Dijet Mass
  if (j_N>1){  
    mjj = Calc_Mjj();
    if( recoJets.at(0).isbjet && recoJets.at(1).isbjet && fabs(recoJets.at(0).Eta())<2.5 && fabs(recoJets.at(1).Eta())<2.5)
      mbb = mjj;
  }

  //Mct
  mct = Calc_mct();


  //==== Hadronic top reconstruction ====
  int ibtop1=-1; //book two highest btag-weight jets for later use
  int ibtop2=-1;
  int iblead1=-1;
  int iblead2=-1;
  float maxbw1=-999;
  float maxbw2=-999;
  auto ijet=0;
  for( auto jet : recoJets ){  //jet loop
        
    if( jet.isbjet && fabs(jet.Eta())<2.5){

	
	if(iblead1<0)//leadings
	  iblead1=ijet;
	else if(iblead2<0)
	  iblead2=ijet;

	float locbw = recoJets.at(ijet).getBweight(Jet_Tagger); //book high bweight jets
	if(locbw > maxbw1){
	  if(maxbw1 > -999){ //if already filled, the old max1 becomes the new max2
	    maxbw2 = maxbw1;
	    ibtop2 = ibtop1;
	  }
	  ibtop1 = ijet;
	  maxbw1 = locbw;
	}
	else if(locbw > maxbw2){
	  ibtop2 = ijet;
	  maxbw2 = locbw;
	}
      }

      ijet++;
  }
  //RecoHadTops(ibtop1, ibtop2);


  //dPhi_b1_b2  
  dPhi_b1_b2 = (iblead1>=0 && iblead2>=0 ? deltaPhi( recoJets.at(iblead1).Phi(), recoJets.at(iblead2).Phi() ) : 0.);

  //==== Fat Jets building ==============
  //run algo_jet algorithm over RecoJets collection to build new-R jets
  std::vector<TLorentzVector> jets_R_12 = getFatJets(1.2);
  m0_antikt12  = (jets_R_12.size()>0 ? jets_R_12.at(0).M() : 0.);
  m1_antikt12  = (jets_R_12.size()>1 ? jets_R_12.at(1).M() : 0.);
  pt0_antikt12 = (jets_R_12.size()>0 ? jets_R_12.at(0).Pt() : 0.);
  pt1_antikt12 = (jets_R_12.size()>1 ? jets_R_12.at(1).Pt() : 0.);
  mtasym12     = getAsymmetry( m0_antikt12, m1_antikt12 );
  
  std::vector<TLorentzVector> jets_R_08 = getFatJets(0.8);
  m0_antikt08  = (jets_R_08.size()>0 ? jets_R_08.at(0).M() : 0.);
  m1_antikt08  = (jets_R_08.size()>1 ? jets_R_08.at(1).M() : 0.);
  pt0_antikt08 = (jets_R_08.size()>0 ? jets_R_08.at(0).Pt() : 0.);
  pt1_antikt08 = (jets_R_08.size()>1 ? jets_R_08.at(1).Pt() : 0.);
  mtasym08     = getAsymmetry( m0_antikt08, m1_antikt08 );
  
  
  //--- Inv masses involving leptons
  //- Define the invariant mass mu-mu (m_M) and the Pt of the mu-mu system (m_Zpt)
  if (recoMuons.size()==2){
    TLorentzVector m12 = (recoMuons.at(0).GetVector() + recoMuons.at(1).GetVector());
    m_M = m12.M();
    m_Zpt = m12.Pt();
  }
  
  //- Define the invariant mass e-e (m_E) and the Pt of the e-e system (e_Zpt)
  if (recoElectrons.size()==2){
    TLorentzVector e12 = (recoElectrons.at(0).GetVector() + recoElectrons.at(1).GetVector());
    e_M = e12.M();
    e_Zpt = e12.Pt();
  }
  
  //- Define the invariant mass between the electron and the muon
  if (recoMuons.size()==1 && recoElectrons.size()==1){
    TLorentzVector inv_EM = (recoElectrons.at(0).GetVector() + recoMuons.at(0).GetVector());
    m_EM = inv_EM.M();
  }
  

  //--- Define the transverse mass e_MT - we recompute the MET with the electron in order to have the pt of the nu
  if(recoElectrons.size()){
    TVector2 v_e1(recoElectrons.at(0).Px(), recoElectrons.at(0).Py());
    //TVector2 met_electron = met_obj.GetVector("met") - v_e1; //CHECK_ME  - o + ?
    TVector2 met_electron = met_obj.GetVector("met_imu"); //arely
    TVector2 met_electron_vmu = met_obj.GetVector("met_vmu"); //arely    
    e_MT = Calc_MT( recoElectrons.at(0), met_electron);
    e_MT_vmu = Calc_MT( recoElectrons.at(0), met_electron_vmu);    
  }
  
  //--- Define m_MT - we recompute the MET with the muon in order to have the pt of the nu
  if(recoMuons.size()>0){ //--- Careful: Optimized for Etmiss without muons!
    TVector2 v_m1(recoMuons.at(0).Px(), recoMuons.at(0).Py());
    //TVector2 met_muon = met_obj.GetVector("met") - v_m1; //CHECK_ME  - o + ? met has inv muons already o.O
    TVector2 met_muon = met_obj.GetVector("met_imu") - v_m1; //arely
    TVector2 met_muon_vmu = met_obj.GetVector("met_vmu"); //arely    
    m_MT = Calc_MT( recoMuons.at(0), met_muon);
    m_MT_vmu = Calc_MT( recoMuons.at(0), met_muon_vmu);    
  }
  
  
  //--- Boson Pt   <---  V(lv).Pt()
  TVector2 V_lnu = met_obj.GetVector("met_imu");
  TVector2 V_lnu_vmu = met_obj.GetVector("met_vmu");  
  
  for (unsigned int iEl=0; iEl < recoElectrons.size(); iEl++){
    TVector2 electron_vector2(recoElectrons.at(iEl).GetVector().Px(), recoElectrons.at(iEl).GetVector().Py());
    V_lnu += electron_vector2;
    V_lnu_vmu += electron_vector2;    
  }

  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
      TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
      V_lnu_vmu += muon_vector2;
    }
 
    
  bosonVec_reco_pt = V_lnu.Mod();
  bosonVec_reco_pt_vmu = V_lnu_vmu.Mod();  
  //bosonVec_reco_eta = V_lnu.Eta();  //not accesible!
  bosonVec_reco_phi = TVector2::Phi_mpi_pi( V_lnu.Phi() );       
  bosonVec_reco_phi_vmu = TVector2::Phi_mpi_pi( V_lnu_vmu.Phi() );    
  
  MtTop = TopTransvMass();
  
  //Z decay mode 
  TLorentzVector Ztlv(0.,0.,0.,0.);
  std::vector<int> zdecays;
  if(m_truthP) zdecays = this->GetTruthZDecay(Ztlv);
  if(zdecays.size()){
    Z_decay  = zdecays.at(0);
    Z_pt     = Ztlv.Pt();
    Z_eta    = Ztlv.Eta();
    Z_phi    = Ztlv.Phi();
    //    Z_m      = Ztlv.M(); //not needed! Everyone knows the Z mass :)
  }
  


  //Redefine run number for MC, to reflect the channel_number instead //CHECK_ME
  RunNumber = mc_channel_number;
  
  //---Fill missing data members 
  this->dumpLeptons();
  this->dumpPhotons();
  this->dumpJets();


  m_atree->Fill();
  //  output->setFilterPassed (true);
  return nextEvent(); //SUCCESS + cleaning
}

//Fill lepton data members
void chorizo :: dumpLeptons(){
    
  bool fill=false;

  //--- Dump Electrons
  //-baseline
  //  for(unsigned int iel = 0; iel < TMath::Min((int)electronCandidates.size(), BookElBase); iel++){
  for(unsigned int iel = 0; iel < BookElBase; iel++){
    
    fill = (iel < electronCandidates.size() );
    eb_pt.push_back( fill ? electronCandidates.at(iel).Pt() : DUMMYDN );
    eb_eta.push_back( fill ? electronCandidates.at(iel).Eta() : DUMMYDN );
    eb_phi.push_back( fill ? electronCandidates.at(iel).Phi() : DUMMYDN );
  }

  //-signal
  //  for(unsigned int iel = 0; iel < TMath::Min((int)recoElectrons.size(), BookElSignal); iel++){
  for(unsigned int iel = 0; iel < BookElSignal; iel++){
    
    fill = (iel < recoElectrons.size() );
    e_pt.push_back( fill ?  recoElectrons.at(iel).Pt()  : DUMMYDN );
    e_eta.push_back( fill ?  recoElectrons.at(iel).Eta()  : DUMMYDN );
    e_phi.push_back( fill ?  recoElectrons.at(iel).Phi()  : DUMMYDN );
    e_type.push_back( fill ?  recoElectrons.at(iel).type  : DUMMYDN );   
    e_origin.push_back( fill ?  recoElectrons.at(iel).origin  : DUMMYDN );        
    e_etiso30.push_back( fill ?  recoElectrons.at(iel).etcone30  : DUMMYDN );
    e_ptiso30.push_back( fill ?  recoElectrons.at(iel).ptcone30  : DUMMYDN );
    e_etiso20.push_back( fill ?  recoElectrons.at(iel).etcone20  : DUMMYDN );
    e_ptiso20.push_back( fill ?  recoElectrons.at(iel).ptcone20  : DUMMYDN );
    e_isoTight.push_back( fill ?  recoElectrons.at(iel).isoTight  : DUMMYDN );
    e_isoLoose.push_back( fill ?  recoElectrons.at(iel).isoLoose  : DUMMYDN );
    e_isoLooseTrackOnly.push_back( fill ?  recoElectrons.at(iel).isoLooseTrackOnly  : DUMMYDN );
    e_isoGradient.push_back( fill ?  recoElectrons.at(iel).isoGradient  : DUMMYDN );
    e_isoGradientLoose.push_back( fill ?  recoElectrons.at(iel).isoGradientLoose  : DUMMYDN );
    e_id.push_back( fill ?   recoElectrons.at(iel).id  : DUMMYDN );
    e_d0_sig.push_back( fill ?  recoElectrons.at(iel).d0_sig  : DUMMYDN );
    e_z0.push_back( fill ?  recoElectrons.at(iel).z0  : DUMMYDN );
    e_trigger.push_back( fill ?  (int)recoElectrons.at(iel).isTrigMatch : DUMMYDN );
  }
  
  e_truth_pt  = (truthElectrons.size()>0) ? truthElectrons.at(0).Pt()  : DUMMYDN;
  e_truth_eta = (truthElectrons.size()>0) ? truthElectrons.at(0).Eta() : DUMMYDN;
  e_truth_phi = (truthElectrons.size()>0) ? truthElectrons.at(0).Phi() : DUMMYDN;
  
  
  //--- Dump Muons 
  //-baseline
  //for(unsigned int imu = 0; imu < TMath::Min((int)muonCandidates.size(), BookMuBase); imu++){
  for(unsigned int imu = 0; imu < BookMuBase; imu++){
    
    fill = (imu < muonCandidates.size());
    mb_pt.push_back(  fill ? muonCandidates.at(imu).Pt()  : DUMMYDN );
    mb_eta.push_back( fill ? muonCandidates.at(imu).Eta() : DUMMYDN );
    mb_phi.push_back( fill ? muonCandidates.at(imu).Phi() : DUMMYDN );
  }
  
  //-signal
  //  for(unsigned int imu = 0; imu < TMath::Min((int)recoMuons.size(), BookMuSignal); imu++){
  for(unsigned int imu = 0; imu < BookMuSignal; imu++){

    fill = (imu < recoMuons.size());    
    m_pt.push_back( fill ?  recoMuons.at(imu).Pt()  : DUMMYDN );
    m_eta.push_back( fill ?  recoMuons.at(imu).Eta()  : DUMMYDN );
    m_phi.push_back( fill ?  recoMuons.at(imu).Phi()  : DUMMYDN );
    m_type.push_back( fill ?  recoMuons.at(imu).type  : DUMMYDN ); 
    m_origin.push_back( fill ?  recoMuons.at(imu).origin  : DUMMYDN );        
    m_etiso20.push_back( fill ?  recoMuons.at(imu).etcone20  : DUMMYDN );
    m_ptiso20.push_back( fill ?  recoMuons.at(imu).ptcone20  : DUMMYDN );
    m_etiso30.push_back( fill ?  recoMuons.at(imu).etcone30  : DUMMYDN );
    m_ptiso30.push_back( fill ?  recoMuons.at(imu).ptcone30  : DUMMYDN );
    m_isoTight.push_back( fill ?  recoMuons.at(imu).isoTight  : DUMMYDN );
    m_isoLoose.push_back( fill ?  recoMuons.at(imu).isoLoose  : DUMMYDN );
    m_isoLooseTrackOnly.push_back( fill ?  recoMuons.at(imu).isoLooseTrackOnly  : DUMMYDN );
    m_isoGradient.push_back( fill ?  recoMuons.at(imu).isoGradient  : DUMMYDN );
    m_isoGradientLoose.push_back( fill ?  recoMuons.at(imu).isoGradientLoose  : DUMMYDN );
    m_trigger.push_back( fill ?  (int)recoMuons.at(imu).isTrigMatch  : DUMMYDN );
  }
  
}


//Fill photon data members
void chorizo :: dumpPhotons(){

  bool fill=false;

  //-signal
  //  for(unsigned int iph = 0; iph < TMath::Min((int)recoPhotons.size(), BookPhSignal); iph++){
  for(unsigned int iph = 0; iph < BookPhSignal; iph++){
    
    fill = (iph < recoPhotons.size());
    ph_pt.push_back( fill ?  recoPhotons.at(iph).Pt()  : DUMMYDN );
    ph_eta.push_back( fill ?  recoPhotons.at(iph).Eta()  : DUMMYDN );
    ph_phi.push_back( fill ?  recoPhotons.at(iph).Phi()  : DUMMYDN );
    ph_etiso30.push_back( fill ?  recoPhotons.at(iph).etcone30  : DUMMYDN );
    ph_ptiso30.push_back( fill ?  recoPhotons.at(iph).ptcone30  : DUMMYDN );
    ph_tight.push_back( fill ?  (int)recoPhotons.at(iph).isTight  : DUMMYDN );
    ph_type.push_back( fill ?  recoPhotons.at(iph).type  : DUMMYDN );
    ph_origin.push_back( fill ?  recoPhotons.at(iph).origin  : DUMMYDN );
    ph_trigger.push_back( fill ?  (int)recoPhotons.at(iph).isTrigMatch  : DUMMYDN );
    
  }
}

//Fill jet data members 
void chorizo :: dumpJets(){

  bool fill=false;

  //--- Dump Jets 
  //-signal
  //  for(unsigned int ijet = 0; ijet < TMath::Min((int)recoJets.size(), BookJetSignal); ijet++){
  for(unsigned int ijet = 0; ijet < BookJetSignal; ijet++){

    fill = (ijet < recoJets.size());

    j_pt.push_back(  fill ?  recoJets.at(ijet).Pt()   : DUMMYDN );
    j_eta.push_back( fill ?  recoJets.at(ijet).Eta() : DUMMYDN );
    j_phi.push_back( fill ?  recoJets.at(ijet).Phi() : DUMMYDN );
    j_m.push_back(   fill ?  recoJets.at(ijet).M()   : DUMMYDN );

    j_tag_MV1.push_back( fill ?  recoJets.at(ijet).MV1  : DUMMYDN );
    j_tag_MV2c20.push_back( fill ?  recoJets.at(ijet).MV2c20  : DUMMYDN );
    
    j_btruth_70.push_back( fill ?  (int) recoJets.at(ijet).isbjet_t70  : DUMMYDN );
    j_btruth_77.push_back( fill ?  (int) recoJets.at(ijet).isbjet_t77  : DUMMYDN );
    j_btruth_85.push_back( fill ?  (int) recoJets.at(ijet).isbjet_t80  : DUMMYDN ); 

    j_bflat_70.push_back( fill ?  (int) recoJets.at(ijet).isbjet_fb70  : DUMMYDN );
    j_bflat_77.push_back( fill ?  (int) recoJets.at(ijet).isbjet_fb77  : DUMMYDN );
    j_bflat_85.push_back( fill ?  (int) recoJets.at(ijet).isbjet_fb85  : DUMMYDN ); 

    j_chf.push_back( fill  ?  recoJets.at(ijet).chf  : DUMMYDN );
    j_emf.push_back( fill  ?  recoJets.at(ijet).emf  : DUMMYDN );
    j_fsm.push_back( fill  ?  recoJets.at(ijet).fsm  : DUMMYDN );
    j_time.push_back( fill ?  recoJets.at(ijet).time : DUMMYDN );
    j_jvf.push_back( fill  ?  recoJets.at(ijet).jvf  : DUMMYDN );
    j_jvt.push_back( fill  ?  recoJets.at(ijet).jvt  : DUMMYDN );
    j_tflavor.push_back( fill ?  recoJets.at(ijet).FlavorTruth  : DUMMYDN );
    j_nTrk.push_back( fill ?  recoJets.at(ijet).nTrk  : DUMMYDN );
    j_sumPtTrk.push_back( fill ?  recoJets.at(ijet).sumPtTrk  : DUMMYDN );

    if(dumpTile){
      j_const_pt.push_back(  fill ?  recoJets.at(ijet).p4const.Pt()  : DUMMYDN );
      j_const_eta.push_back( fill ?  recoJets.at(ijet).p4const.Eta() : DUMMYDN );
      j_const_phi.push_back( fill ?  recoJets.at(ijet).p4const.Phi() : DUMMYDN );
      j_const_m.push_back(   fill ?  recoJets.at(ijet).p4const.M()   : DUMMYDN );
      
      if(ijet==0 ){
	if(fill){
	  j1_cl_N = recoJets.at(ijet).clusters.size();
	  for(unsigned int icl=0; icl < recoJets.at(ijet).clusters.size(); icl++){
	    j1_cl_pt.push_back( recoJets.at(ijet).clusters.at(icl).Pt() );
	    j1_cl_eta.push_back( recoJets.at(ijet).clusters.at(icl).Eta() );
	    j1_cl_phi.push_back( recoJets.at(ijet).clusters.at(icl).Phi() );
	    j1_cl_emf.push_back( recoJets.at(ijet).clusters.at(icl).emf );
	  }
	}
	else{
	  j1_cl_N = DUMMYDN;
	  j1_cl_pt.push_back(  DUMMYDN );
	  j1_cl_eta.push_back( DUMMYDN );
	  j1_cl_phi.push_back( DUMMYDN );
	  j1_cl_emf.push_back( DUMMYDN );
	}
      }
      else if(ijet==1){
	if(fill){
	  j2_cl_N = recoJets.at(ijet).clusters.size();
	  for(unsigned int icl=0; icl < recoJets.at(ijet).clusters.size(); icl++){
	    j2_cl_pt.push_back( recoJets.at(ijet).clusters.at(icl).Pt() );
	    j2_cl_eta.push_back( recoJets.at(ijet).clusters.at(icl).Eta() );
	    j2_cl_phi.push_back( recoJets.at(ijet).clusters.at(icl).Phi() );
	    j2_cl_emf.push_back( recoJets.at(ijet).clusters.at(icl).emf );
	  }
	}
	else{
	  j2_cl_N = DUMMYDN;
	  j2_cl_pt.push_back(  DUMMYDN );
	  j2_cl_eta.push_back( DUMMYDN );
	  j2_cl_phi.push_back( DUMMYDN );
	  j2_cl_emf.push_back( DUMMYDN );
	}
      }
    }
  }
  
} //end filling jet data members



EL::StatusCode chorizo :: postExecute ()
{
  if(debug)  Info("postExecute()","HERE");
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode chorizo :: finalize ()
{
  if(debug)  Info("finalize()","HERE");

  h_presel_flow->Fill(0.5, meta_nsim);
  h_presel_wflow->Fill(0.5, meta_nwsim);

  //VIEW containers
  if(m_goodJets)
    delete m_goodJets;
  if(m_smdJets)
    delete m_smdJets;
  if(m_inv_el)
    delete m_inv_el;
  if(m_inv_mu)
    delete m_inv_mu;
  if(m_inv_ph)
    delete m_inv_ph;

  //OverlapRemoval
  if(tool_or){
    delete tool_or;
    tool_or=0;
  }

  //B-tagging
  if(tool_btag70){
    delete tool_btag70;
    tool_btag70=0;
  }
  if(tool_btag77){
    delete tool_btag77;
    tool_btag77=0;
  }
  if(tool_btag85){
    delete tool_btag85;
    tool_btag85=0;
  }  
  
  
  if(tool_btag_truth1){
    delete tool_btag_truth1;
    tool_btag_truth1=0;
  }
  if(tool_btag_truth2){
    delete tool_btag_truth2;
    tool_btag_truth2=0;
  }
  if(tool_btag_truth3){
    delete tool_btag_truth3;
    tool_btag_truth3=0;
  }

  //isolation tool
  if(iso_1){
    delete iso_1;
    iso_1=0;
  }
  if(iso_2){
    delete iso_2;
    iso_2=0;
  }
  if(iso_3){
    delete iso_3;
    iso_3=0;
  }
  if(iso_4){
    delete iso_4;
    iso_4=0;
  }
  if(iso_5){
    delete iso_5;
    iso_5=0;
  }


  //PURW
  if(tool_purw){
    if(genPUfile)
      tool_purw->finalize();

    delete tool_purw;
    tool_purw=0;
  }

  //SUSYTools
  if( tool_st ){
    delete tool_st;
    tool_st = 0;
  }

  //Trigger
  if(tool_trigdec){
    delete tool_trigdec;
    tool_trigdec=0;
  }

  if(tool_trigconfig){
    delete tool_trigconfig;
    tool_trigconfig=0;
  }

  if(tool_trig_match_el){
    delete tool_trig_match_el;
    tool_trig_match_el=0;
  }

  if(tool_trig_match_mu){
    delete tool_trig_match_mu;
    tool_trig_match_mu=0;
  }
  if(tool_trig_match_ph){
    delete tool_trig_match_ph;
    tool_trig_match_ph=0;
  }

  //GRL
  if( tool_grl ) {
    delete tool_grl;
    tool_grl = 0;
  }

  //TileTripReader
  if( tool_tileTrip ) {
    delete tool_tileTrip;
    tool_tileTrip = 0;
  }

  //jet tile recovery
#ifdef TILETEST
  if( tool_jettile ) {
    delete tool_jettile;
    tool_jettile = 0;
  }
#endif 

  //jet smearing
  if( tool_jsmear ) {
    delete tool_jsmear;
    tool_jsmear = 0;
  }

  //MCTruthClassifier
  if( tool_mcc ) {
    delete tool_mcc;
    tool_mcc = 0;
  }
  
  //mct corrected
  if( tool_mct ) {
    delete tool_mct;
    tool_mct = 0;
  }

  //Stop clock
  watch.Stop();
  //  Info("finalize()", Form("Execution time = %f (%f/ev)", watch.CpuTime(), watch.CpuTime()/(float)m_event->getEntries()));
  Info("finalize()", Form("Execution time = %f", watch.CpuTime()));

#ifdef PROFCODE
  if(m_eventCounter!=0){
    CALLGRIND_STOP_INSTRUMENTATION;
    CALLGRIND_DUMP_STATS;
  }
#endif
  
#ifdef CODE_STATS
  xAOD::IOStats::instance().stats().printSmartSlimmingBranchList();
#endif


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode chorizo :: histFinalize ()
{
  if(debug)  Info("histFinalize()","HERE");
  return EL::StatusCode::SUCCESS;
}


void chorizo :: Fill(TH1 *h, float value, float weight) {
  TAxis *x = h->GetXaxis();
  int nbins = x->GetNbins();
  float max = x->GetBinUpEdge(nbins);
  float min = x->GetBinLowEdge(1);
  float thisvalue = value;
  float width = h->GetBinWidth(nbins);
  if (thisvalue > max) thisvalue = max-width/2;
  if (thisvalue < min) thisvalue = min+width/2;
  h->Fill(thisvalue, weight);
}

bool chorizo :: vetoMCevent(){

  if(!this->isMC) return false; //do not veto data events!

  bool doVeto=false;
  
  //to loop over  
  xAOD::TruthParticleContainer::const_iterator truthP_itr;
  xAOD::TruthParticleContainer::const_iterator truthP_end;

  //1*** Bug fix for the Sherpa di-boson : https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyMCSamples#Sherpa_AN4
  //     Veto events with a status 3 b-quark. Apply the veto only to WW samples

  // Apply the veto only to WW samples
  if(this->mc_channel_number==126892 || (157817<=this->mc_channel_number && this->mc_channel_number<=157819)){
      int count = 0;
      double charge_V1 = 0.;
      double charge_V2 = 0.;

      //truth particles loop
      truthP_itr = m_truthP->begin();
      truthP_end = m_truthP->end();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {
      
	if(( *truthP_itr )->status()!=3) continue;
	count++;

	/// Veto events with status 3 b-quarks
	if( ( *truthP_itr )->absPdgId() == 5) 
	  doVeto = true;

	//cout << "Charge = " << ( *truthP_itr )->charge() << endl;
	// //	Identify WZ events    //FIX_ME
	// if(count==3 || count==4)
	//   charge_V1 += ( *truthP_itr )->charge();
 	// else if(count==5 || count==6)
	//   charge_V2 += ( *truthP_itr )->charge();

      }
  
      /// For lnuqq sample, apply the veto only on WW events
      if(this->mc_channel_number==157817 || this->mc_channel_number==157818 || this->mc_channel_number==157819)
        if(charge_V1==0. || charge_V2==0.)
	  doVeto = false;
  }


  //2***  Separate Wcc and Wc samples
  if ( (mc_channel_number>=117284 && mc_channel_number<=117286) || (mc_channel_number>=117293 && mc_channel_number<=117297) ){
    if (WccType() != this->leptonType)
      doVeto = true;
  }
  
  return doVeto;
}

//Check Wcc type
TString chorizo :: WccType(){

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("WccType()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }
  
  //--- get W                                                                           
  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();  
  
  for( ; truthP_itr != truthP_end; ++truthP_itr ) {

    //*** FIX_ME   truth navigation??

    // if ( (*truthP_itr)->absPdgId()==24 ){
    //   //--- W with two children                                                          
    //   std::vector<int> vect_child_index = mc_child_index->at(ipart);
    //   if (vect_child_index.size()>=2){
    //     for (unsigned int ich=0; ich<vect_child_index.size(); ++ich){
    //       int index = vect_child_index.at(ich);
    //       //--- Get the lepton flavour
    //       if ( fabs(mc_pdgId->at(index))==11 || fabs(mc_pdgId->at(index))==12 ){
    //         return "ele";
    //       }
    //       else if ( fabs(mc_pdgId->at(index))==15 || fabs(mc_pdgId->at(index))==16 ){
    //         return "tau";
    //       }
    //       else{
    //         return "muo";
    //       }
    //     }
    //   }
    // }
  }
  return "muo";
}

//Remove overlap among filtered MC samples
bool chorizo :: passMCor(){

  if (!this->isMC) //it only obviously applies to MC
    return true;

  //--- to take into account new MET filtered samples  (don't apply it by default (yet))
  // UInt_t MET_ids[] = {410000, 410013, 410014};   //filtered : 407012, 407019, 407021;
  // if ( count(MET_ids, MET_ids+84, this->mc_channel_number) ){
    
  //   double nuMET = Calc_TruthNuMET();
  //   if( nuMET.Pt() > 200000. ) //nominal samples
  //     return false;
  //   else
  //     return true;
  // }


  double cutting_var=0.;
  //to loop over  
  if (this->mc_channel_number>=156803 && this->mc_channel_number<=156828){ //Znunu AlpGen

    for(const xAOD::TruthParticle* tp : *m_truthP) {
      if(tp->status()==124 && tp->absPdgId()==23){ //Z boson
	cutting_var = tp->pt();
	this->bos_pt = cutting_var;
      }
    } 
    //   if (!doAnaTree) h_cut_var->Fill(cutting_var,w); //FIX_ME
  }
  
  if ((mc_channel_number>=157537 && mc_channel_number<=157540) ||//Znunu1,2,3,4 Sherpa
      (mc_channel_number>=157534 && mc_channel_number<=157536) ||//Wlnu1 Sherpa
      (mc_channel_number>=147773 && mc_channel_number<=147776) ||//Znunu0 and Wlnu0 Sherpa
      (mc_channel_number>=167740 && mc_channel_number<=167760) || // W, Z --> MassiveCB --> Pt0 --> BFilter, CJetFilterBVeto, CJetVetoBVeto
      (mc_channel_number>=167761 && mc_channel_number<=167844) || // W, Z --> MassiveCB --> PtXPtY --> BFilter, CJetFilterBVeto, CJetVetoBVeto. Just to have the bos_pt distribution.
      (mc_channel_number>=173041 && mc_channel_number<=173046) || // Sherpa Low mass DYee, DYmumu, DYtautau
      (mc_channel_number>=167150 && mc_channel_number<=167155) || // WxnuMassiveBPt0_BFilter, WxnuMassiveBPt0_VetoB
      (mc_channel_number>=167156 && mc_channel_number<=167161) ) // WxnuMassiveBPt140_BFilter, WxnuMassiveBPt140_VetoB
    {
      
      //load mc particles info vectors
      int mc3_n = -1;
      const int mcall_n = static_cast< const int >( m_truthP->size() );
      vector<int> mc_status(mcall_n);
      vector<int> mc_pdgId(mcall_n);
      vector<float> mc_pt(mcall_n);
      vector<float> mc_eta(mcall_n);
      vector<float> mc_phi(mcall_n);
      vector<float> mc_m(mcall_n);
      
      for(const xAOD::TruthParticle* tp : *m_truthP) {
	if( !isHard( tp ) ) continue; //look at ME particles only
	if( !isLepNeut( tp ) ) continue; //look for leptons only
	mc3_n++;
	mc_status[mc3_n] = tp->status();
	mc_pdgId[mc3_n]  = tp->pdgId();
	mc_pt[mc3_n]     = tp->pt();
	mc_eta[mc3_n]    = tp->eta();
	mc_phi[mc3_n]    = tp->phi();
	mc_m[mc3_n]      = tp->m();
      }
      mc3_n++;  // index --> size
      const int mc_n = static_cast< const int >( mc3_n );
      //get boson pt weight
      this->bosonVect_w = boson_sherpa_stop_charm(mc_n, mc_status, mc_pdgId, mc_pt, mc_eta, mc_phi, mc_m);
      
      truth_met          = this->GetTruthEtmiss();
      vector<float> bTruthKin = this->GetTruthBosonPt(truth_M, truth_MT, truth_m_fiducial, truth_Lep1_pt, truth_Lep2_pt);

      bosonVec_truth_pt  = bTruthKin.at(0);
      bosonVec_truth_eta  = bTruthKin.at(1);
      bosonVec_truth_phi  = bTruthKin.at(2);

      
      int lep_count=0;
      TLorentzVector v1(0.,0.,0.,0.);
      TLorentzVector v2(0.,0.,0.,0.);

      for(const xAOD::TruthParticle* tp : *m_truthP) {
      
	if( isLepNeut( tp ) && isStable( tp ) ){
	  bos_pdgId = tp->pdgId(); //CHECK_ME
	  lep_count++;
	  if(lep_count==1){
	    fillTLV(v1, tp);
	  }
	  else if(lep_count==2){
	    fillTLV(v2, tp);
	    break;
	  }
	}//if                                                                                   
      }//mc particles loop

      TLorentzVector v3(0.,0.,0.,0.);
      v3+=v1;
      v3+=v2;
      

      cutting_var=bosonVec_truth_pt;

      bos_pt=cutting_var;
      if( ! DoIHaveToKeepIt(mc_channel_number,cutting_var) )
	return false; 

      if (!doAnaTree) {
	if(applyPURW){
	  w_average = this->GetAverageWeight();//(Int_t)RunNumber, (Int_t)mc_channel_number);
	}
	else{
	  w_average = 1.;
	}
	h_cut_var->Fill(cutting_var,w/w_average);
      }
    }

  UInt_t check_ids[] = {157851 , 176266 , 176267 ,
			157852 , 176268 , 176269 ,
			157853 , 176270 , 176271 ,
			157854 , 176272 , 176273 ,
			157855 , 176274 , 176275 ,
			157856 , 176276 , 176277 ,
			157857 , 176278 , 176279 ,
			157858 , 176280 , 176281 ,
			157859 , 176282 , 176283 ,
			157860 , 176284 , 176285 ,
			157861 , 176286 , 176287 ,
			157862 , 176288 , 176289 ,
			157863 , 176290 , 176291 ,
			157864 , 176292 , 176293 ,
			157865 , 176294 , 176295 ,
			157866 , 176296 , 176297 ,
			157867 , 176298 , 176299 ,
			157868 , 176300 , 176301 ,
			157869 , 176302 , 176303 ,
			157870 , 176304 , 176305 ,
			157871 , 176306 , 176307 ,
			157872 , 176308 , 176309 ,
			157873 , 176310 , 176311 ,
			157874 , 176312 , 176313 ,
			157875 , 176314 , 176315 ,
			157876 , 176316 , 176317 ,
			157877 , 176318 , 176319 ,
			157878 , 176320 , 176321 };
  
  if( count(check_ids, check_ids+84, this->mc_channel_number) ){
    TVector2 met(0.,0.);
    xAOD::MissingETContainer* metRFcutvar = 0; // = new xAOD::MissingETContainer;
    //    CHECK( m_event->retrieve( metRFcutvar, "MET_RefFinal") );
    CHECK( m_event->retrieve( metRFcutvar, "MET_Reference_AntiKt4EMTopo")); //??
    //    CHECK( m_event->retrieve( metRFcutvar, "LocHadTopo") );
    met = getMET( metRFcutvar, "Final"); //METRefFinal for now... //FIX_ME !! Reco MET??

    bos_pt = met.Mod();
    cutting_var = bos_pt/1000.;

    if( ! DoIHaveToKeepIt(mc_channel_number,cutting_var) )
      return false;

  }
  
  return true;
}


float chorizo :: GetTruthEtmiss(bool noEleTau){

  /// Calculate the boson pt
  TLorentzVector V(0.,0.,0.,0.);
  TLorentzVector l1;

  //to loop over  
  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();
  bool found=false;
  for( ; truthP_itr != truthP_end; ++truthP_itr ) {
    if( isStable( (*truthP_itr) ) && isLepNeut( (*truthP_itr) )){
      if( noEleTau && (isElectron((*truthP_itr)) || isTau((*truthP_itr))) ) continue;
      fillTLV(l1, (*truthP_itr) );
      V += l1;
      found=true;
    }
  }

  if(!found) return -999;
  return (V.Pt()*0.001);

}

std::vector<float> chorizo :: GetTruthBosonPt(float &_M, float &_MT, bool &_Muon, float &_pt1, float &_pt2){
  std::vector<float> weight(3,1.);
  
  std::vector<float> vector_boson(3);  
  vector_boson.at(0) = 0.;
  vector_boson.at(1) = 99.;
  vector_boson.at(2) = -99.;
      
  _Muon = false;
  float _Muon_pt = 0.0, _Muon_eta = 0.0;

  /// Calculate the boson pt
  TLorentzVector V(0.,0.,0.,0.);
  TLorentzVector l1(0.,0.,0.,0.);
  TLorentzVector l2(0.,0.,0.,0.);
  bool foundFirst = false;
  bool foundSecond = false;
 
  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();
  
  for( ; truthP_itr != truthP_end; ++truthP_itr ) {

    if( isHard(*truthP_itr) && isLepNeut(*truthP_itr) ){
      if (!foundFirst){     
	fillTLV( l1, (*truthP_itr) );
	foundFirst = true;
        _pt1 = l1.Pt()/1000.;
        _Muon_pt  = l1.Pt()/1000.;
        _Muon_eta = l1.Eta();

      } 
      else if(!foundSecond){
	fillTLV( l2, (*truthP_itr) );
	foundSecond = true;
        _pt2 = l2.Pt()/1000.;

	if ( (*truthP_itr)->absPdgId() == 13 ){
          _Muon_pt  = l2.Pt()/1000.;
          _Muon_eta = l2.Eta();
        }

      } else {
        Warning("GetTruthBosonPt()","Error in boson_sherpa_stop_charm : more than 2 leptons");
        return std::vector<float>(weight);
      }
    }
  }
  _Muon = (_Muon_pt > 10.0) && (fabs(_Muon_eta)<2.4);  

  if (!foundSecond) {
    Warning("GetTruthBosonPt()", "Error in boson_sherpa_stop_charm : unable to find 2 leptons");
    return std::vector<float>(weight);
  }

  V = l1 + l2;

  double epsilon=0.0000000001; //define 'same'
  vector_boson.at(0) = V.Pt()*0.001;
  vector_boson.at(1) = ( fabs(l1.Pt()-l2.Pt())>epsilon ? V.Eta() : 99.); //eta not defined for zero-pt object
  vector_boson.at(2) = V.Phi();           
  _M  = V.M()*0.001;
  //_MT = TMath::Sqrt(2*l1.Pt()*l2.Pt()*(1-TMath::Cos(l1.DeltaPhi(l2)))) * 0.001;
  _MT = V.Mt()*0.001;

  return std::vector<float>(vector_boson);
}

float chorizo :: GetTTbarReweight(float &Top_truth_pt, float &Topbar_truth_pt, float &avTop_truth_pt){
  
  float weight = 1;
  Top_truth_pt = 0;
  Topbar_truth_pt = 0;  
  avTop_truth_pt = 0;
  float ttbar_reweight=1;

  /// Calculate the top and anti-top pt
  TLorentzVector TTbar;  
  
  TLorentzVector Top;
  TLorentzVector Topbar;
  
  bool foundTop = false;
  bool foundTopbar = false;

  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();
  
  for( ; truthP_itr != truthP_end; ++truthP_itr ) {

    if ( (*truthP_itr)->status()==3 && (*truthP_itr)->pdgId()==6 ){
      if (!foundTop){
	fillTLV(Top, (*truthP_itr));
	foundTop = true;
      } 
    }
    else if ( (*truthP_itr)->status()==3 && (*truthP_itr)->pdgId()==-6 ){
      if (!foundTopbar){
	fillTLV(Topbar, (*truthP_itr));
	foundTopbar = true;
      }       
      
    }
  }

  if (!foundTop || !foundTopbar) {
    Warning("GetTTbarReweight()", "Error in ttbar : unable to find top+antitop quark pair");
    return weight;
  }
  else {
    
    TTbar = Top + Topbar;
    
    Top_truth_pt = Top.Pt()*0.001;
    Topbar_truth_pt = Topbar.Pt()*0.001;   
    avTop_truth_pt = TTbar.Pt()*0.001;
    
    if (avTop_truth_pt < 40)       ttbar_reweight = (1/1.01185 + 1/0.994193)/2;
    else if (avTop_truth_pt >= 40 && avTop_truth_pt <170 )    ttbar_reweight = (1/1.09592 + 1/1.03448)/2;
    else if (avTop_truth_pt >= 170 && avTop_truth_pt <340 )   ttbar_reweight = (1/1.40728 + 1/1.31911)/2;
    else               ttbar_reweight = (1/1.79938 + 1/1.71078)/2; 
    
  }
  return ttbar_reweight;

}

int chorizo::GetNTruthB(){
  
  int b_counter = 0;

  float truth_bjet_threshold = 30.;
  float truth_bjet_eta = 2.5;


  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();
  
  for( ; truthP_itr != truthP_end; ++truthP_itr ) {

    if ( (*truthP_itr)->status()==3 && (*truthP_itr)->absPdgId()==5 ){
      if ( (*truthP_itr)->pt() > truth_bjet_threshold && fabs((*truthP_itr)->eta()) < truth_bjet_eta ){
	b_counter+=1;
      }
    }
  }
    
  return b_counter;
}


int chorizo :: GetNTruthHF(){
  
  int hf_counter = 0;

  float truth_HFjet_threshold = 30.;
  float truth_HFjet_eta = 2.5;

  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();  

  for( ; truthP_itr != truthP_end; ++truthP_itr ) {

    if ( isHard( (*truthP_itr) )&& isHF( (*truthP_itr) ) ){
      if ( (*truthP_itr)->pt() > truth_HFjet_threshold && fabs((*truthP_itr)->eta()) < truth_HFjet_eta ){

	hf_counter+=1;
        
      }
    }
  }
    
  return hf_counter;
}


int chorizo :: GetNTruthLeptons(){
  
  int lep_counter = 0;

  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();  

  for( ; truthP_itr != truthP_end; ++truthP_itr ) {
    if ( isHard( (*truthP_itr) ) && isLepton( (*truthP_itr) ) ){
	lep_counter+=1;
    }
  }
    
  return lep_counter;
}

//Find Z-decay 
std::vector<int> chorizo::GetTruthZDecay(TLorentzVector &ztlv){

  //*** NOTE: this is highly generator-dependent. Tuned to get (at least) MadGraph structure right for now.   //CHECK_ME

  std::vector<int> zmodes;
  int Zmode = ZDecayMode::None;

  xAOD::TruthParticleContainer::const_iterator tpItr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();  

  for( ; tpItr != truthP_end; ++tpItr ) {
    
    if( (*tpItr)->isZ() && (*tpItr)->p4().M() > 5000 && isHard(*tpItr) ){

      //book Z TLV (return hardest)
      if((*tpItr)->p4().Pt() > ztlv.Pt())
	ztlv = (*tpItr)->p4();
      
      bool badZ = false;
      int nuZ=0;
      int hadZ=0;
      int eZ=0;
      int muZ=0;
      Zmode = ZDecayMode::Unknown;

      const xAOD::TruthVertex* dvtx = 0;
      if( (*tpItr)->hasDecayVtx() ) 
	dvtx = (*tpItr)->decayVtx();
      
      // Has valid vertex
      if( dvtx ){

	const std::vector< ElementLink<xAOD::TruthParticleContainer> >&
	  outPart = dvtx->outgoingParticleLinks();

	for(unsigned int k=0; k<outPart.size(); ++k){
	  if( ! (outPart[k]).isValid() ) continue;
	  const xAOD::TruthParticle* kid = *(outPart[k]);
	  if( ! kid ) continue;
	  int kidId = kid->pdgId();
	  int kidIda = std::abs(kidId);

	  // Remove self-decays, Z -> Z
	  if( kidId == (*tpItr)->pdgId() ) badZ = true;
	  // PowhegPythia6 has Z's with no real descendants
	  if( kid->status() != 1 && !(kid->decayVtx()) ) badZ = true;
	  
	  // Find Z decay mode
	  nuZ  += (int) is_Neutrino(kidIda);
	  hadZ += (int) is_Parton(kidIda);
	  eZ   += (int) is_Electron(kidIda);
	  muZ  += (int) is_Muon(kidIda);
	}

	if(nuZ+hadZ+eZ+muZ != 2) Zmode = ZDecayMode::Unknown;
	else if(nuZ == 2)        Zmode = ZDecayMode::invisible;
	else if(hadZ == 2)       Zmode = ZDecayMode::hadronic;
	else if(eZ == 2)         Zmode = ZDecayMode::ee;
	else if(muZ == 2)        Zmode = ZDecayMode::mumu;
	else Zmode = ZDecayMode::Unknown;

      }//has dcay vtx

      zmodes.push_back(Zmode);

    }//is Z boson
  }//truth p loop

  return zmodes;
}


void chorizo::GetTruthShat(long int sigSamPdgId){

  sigSam_pt1 = 0.;
  sigSam_eta1 = 0.;
  sigSam_phi1 = 0.;
  sigSam_E1 = 0.;
  sigSam_pt2 = 0.;
  sigSam_eta2 = 0.;
  sigSam_phi2 = 0.;
  sigSam_E2 = 0.;
  truth_shat = 0.;
  truth_shat_pt = 0.;
 
  if (sigSamPdgId==0) return;
 
  TLorentzVector V(0.,0.,0.,0.);
  TLorentzVector l1, l2;
  int i= 0;

  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();
  
  if (sigSamPdgId==1000022){ //--- For WIMPs (produced in pairs)
    for( ; truthP_itr != truthP_end; ++truthP_itr ) {

      if( isStable( (*truthP_itr) ) && (*truthP_itr)->absPdgId() == sigSamPdgId ){
        i++;
	fillTLV(l1, (*truthP_itr), true);
	V += l1;

        if(i==1){
          sigSam_pt1=l1.Pt();
          sigSam_eta1=l1.Eta();
          sigSam_phi1=l1.Phi();
          sigSam_E1=l1.E();
        }
        if(i==2){
          sigSam_pt2=l1.Pt();
          sigSam_eta2=l1.Eta();
          sigSam_phi2=l1.Phi();
          sigSam_E2=l1.E();
        }
        if(i>2) {
          Warning("GetTruthShat()"," More than 2 sig particles...?");
          break;
        }
      }
    }
  }
  else if (sigSamPdgId==5000039){ //--- For ADD gravitons (only one is produced)
    for( ; truthP_itr != truthP_end; ++truthP_itr ) {

      if( (*truthP_itr)->absPdgId() == sigSamPdgId ){ //--- Assuming they are ordered
	fillTLV(l1, (*truthP_itr), true);
	
	V = l1;

        sigSam_pt1=l1.Pt();
        sigSam_eta1=l1.Eta();
        sigSam_phi1=l1.Phi();
        sigSam_E1=l1.E();

	xAOD::TruthParticleContainer::const_iterator truthP_itr_2 = truthP_itr;
	truthP_itr_2++;
	fillTLV(l2, (*truthP_itr_2), true);
	//        fillTV(l2, (*(std::next(truthP_itr,1))), true);
        V += l2;
        break;
      }
    }
  }

  truth_shat = V.M();//magnitude or V.M()
  truth_shat_pt = V.Pt();//some sort of truth met

}


double chorizo :: GetGeneratorUncertaintiesSherpa(){
  //--- Generator level uncertainites for Znunu+jet Sherpa :

  // This procedure consists of applying the Alpgen scale variations to Sherpa using a reweighting based on the number of truth jets with pt > 30 GeV.
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSystematicUncertainties2012#Sherpa_as_baseline_AN3
  // Sherpa runs [167758 to 167844]
  // Alpgen runs : Np0=156803 Np1=156804 Np2=156809 Np3=156814 Np4=156819 Np5=156824
  //
  // double GetWeight(int sampleId, variation var, mcperiod mc = mc12);
  // enum variation{None,ktfacUP,ktfacDOWN,qfacUP,qfacDOWN,ptminUP,ptminDOWN,IqoptUP,IqoptDOWN,ScalesUP,ScalesDOWN,qfacUPktfacDOWN,qfacDOWNktfacUP};

  if ( syst_Scale == ScaleVariatioReweighter::None ) return 1.;

  //-- samples id
  UInt_t wenu_ids[] = {167740, 167741, 167742, 167761, 167762, 167763, 167770, 167771, 167772, 167779, 167780, 167781, 167788, 167789, 167790};

  UInt_t wmunu_ids[] = {167743, 167744, 167745, 167764, 167765, 167766, 167773, 167774, 167775, 167782, 167783, 167784, 167791, 167792, 167793};

  UInt_t wtaunu_ids[] = {167746, 167747, 167748, 167767, 167768, 167769, 167776, 167777, 167778, 167785, 167786, 167787, 167794, 167795, 167796};

  UInt_t zee_ids[] = {167749, 167750, 167751, 167797, 167798, 167799, 167809, 167810, 167811, 167821, 167822, 167823, 167833, 167834, 167835};

  UInt_t zmumu_ids[] = {167752, 167753, 167754, 167800, 167801, 167802, 167812, 167813, 167814, 167824, 167825, 167826, 167836, 167837, 167838};

  UInt_t ztautau_ids[] = {167755, 167756, 167757, 167803, 167804, 167805, 167815, 167816, 167817, 167827, 167828, 167829, 167839, 167840, 167841};

  UInt_t znunu_ids[] = {167758, 167759, 167760, 167806, 167807, 167808, 167818, 167819,	167820, 167830, 167831, 167832, 167842, 167843, 167844};

  //-- check sample
  bool is_wenu = count(wenu_ids, wenu_ids+15, this->mc_channel_number) ;
  bool is_wmunu = count(wmunu_ids, wmunu_ids+15, this->mc_channel_number) ;
  bool is_wtaunu = count(wtaunu_ids, wtaunu_ids+15, this->mc_channel_number) ;

  bool is_zee = count(zee_ids, zee_ids+15, this->mc_channel_number) ;
  bool is_zmumu = count(zmumu_ids, zmumu_ids+15, this->mc_channel_number) ;
  bool is_ztautau = count(ztautau_ids, ztautau_ids+15, this->mc_channel_number) ;
  bool is_znunu = count(znunu_ids, znunu_ids+15, this->mc_channel_number) ;

  bool is_other = !(is_wenu || is_wmunu || is_wtaunu || is_zee || is_zmumu || is_ztautau || is_znunu);


  if ( is_other ) return 1.; //if not W/Z sample just leave

   
  int nTruthJets=0;
  int sampleId=0;

  //loop over truth jets
  TLorentzVector truthJet;
  for(const xAOD::Jet* tjet : *m_truth_jets) {
  
    fillTLV( truthJet, tjet, true);
    
    //look for overlapping electron
    bool isElectronFakingJet = false;
    for(const xAOD::TruthParticle* tp : *m_truthP) {
      
      TLorentzVector truthPart;
      if ( isElectron(tp) && isStable(tp) ){
  	fillTLV( truthPart, tp, true);
  	if (truthPart.DeltaR(truthJet) < 0.1) {
  	  isElectronFakingJet = true;
  	  break;
  	}
      }
    }
    
    if ( truthJet.Pt() > this->Jet_RecoPtCut && !isElectronFakingJet)
      ++nTruthJets;
  }
  
  //saturate n-jets
  if(nTruthJets>5) nTruthJets=5;
  
  if (is_wenu)        sampleId = 107680+nTruthJets;
  else if (is_wmunu)  sampleId = 107690+nTruthJets;
  else if (is_wtaunu) sampleId = 107700+nTruthJets;
  else if (is_zee)    sampleId = 107650+nTruthJets;
  else if (is_zmumu)    sampleId = 107660+nTruthJets;
  else if (is_ztautau)    sampleId = 107670+nTruthJets;
  
  if (is_znunu){
    if (nTruthJets==0) sampleId=156803;
    if (nTruthJets==1) sampleId=156804;
    if (nTruthJets==2) sampleId=156809;
    if (nTruthJets==3) sampleId=156814;
    if (nTruthJets==4) sampleId=156819;
    if (nTruthJets>=5) sampleId=156824;
  }
  
  return ScaleVariatioReweighter::GetWeight(sampleId, syst_Scale );
  
}
 

 UInt_t chorizo :: puRunNumber( UInt_t rn){ //just fixes RunNumber for new xAOD production (otherwise the PURW doesn't know what to do
  if(rn==212272)
    return 195848; //mc12b
  return rn;
}

float chorizo :: GetAverageWeight(){
  
  return 1.; //DUMMY FOR NOW!!! TAKE THIS OUT!!
  
  if ( strcmp(Form("%s", h_average->GetName()), Form("pileup_chan%d_run%d", mc_channel_number, RunNumber))==0 ){ //--- Strings look the same
    return (h_average->Integral() / h_average->GetEntries());
  }
  else{
    h_average->Reset();
    f_average->Close();
    
    if (warningPileup)
      Info("GetAverageWeight()", Form("Changing TFile to %s/pileup_%d.root", PURW_Folder.Data(), mc_channel_number));
    f_average = new TFile(Form("%s/pileup_%d.root", PURW_Folder.Data(), mc_channel_number));
    h_average = (TH1D *)f_average->Get(Form("pileup_chan%d_run%d", mc_channel_number, RunNumber));
    if (h_average==0){
      if (warningPileup){
	Warning("GetAverageWeight()", "Is this MC12b? We hope so... looks like the RunNumber wasn't set correctly in the pileup file (known issue). \n\n I'll read the histograms with other runnumber (the one used in MC12a).\n\n");
	warningPileup = false;
      }
      UInt_t runnumber = 195847;
      h_average = (TH1D *)f_average->Get(Form("pileup_chan%d_run%d", mc_channel_number, runnumber));
    }
    return (h_average->Integral() / h_average->GetEntries());
   }
}


float chorizo::GetBtagSF(xAOD::JetContainer* goodJets, BTaggingEfficiencyTool* btagTool, float btag_op)
{
  float totalSF = 1.;
  for ( const auto& jet : *goodJets ) {

    float sf = 1.;
    
    const xAOD::BTagging* btag =jet->btagging();
    double wmv2=-1;
    btag->MVx_discriminant("MV2c20",wmv2);
    
    if ( fabs(jet->eta()) > 2.5 ) {
      ATH_MSG_VERBOSE( " Trying to retrieve b-tagging SF for jet with |eta|>2.5 (jet eta=" << jet->eta() << "), jet will be skipped");
    } else if ( jet->pt() < 20e3 && jet->pt() < 1e6 ) {
      ATH_MSG_VERBOSE( " Trying to retrieve b-tagging SF for jet with invalid pt (jet pt=" << jet->pt() << "), jet will be skipped");
    } else {

      CP::CorrectionCode result;
      int truthlabel(-1);
      if (!jet->getAttribute("ConeTruthLabelID", truthlabel)) {
        ATH_MSG_ERROR("Failed to get jet truth label!");
      }
      ATH_MSG_VERBOSE("This jet is " << (dec_bjet(*jet) ? "" : "not ") << "b-tagged.");
      ATH_MSG_VERBOSE("This jet's truth label is " << truthlabel);

      if ( wmv2>btag_op ) {  //FIX_ME !!

        result = btagTool->getScaleFactor(*jet, sf);

        switch (result) {
        case CP::CorrectionCode::Error:
          ATH_MSG_ERROR( " Failed to retrieve SF for jet in SUSYTools_xAOD::BtagSF" );
          break;
        case CP::CorrectionCode::OutOfValidityRange:
          ATH_MSG_VERBOSE( " No valid SF for jet in SUSYTools_xAOD::BtagSF" );
          break;
        default:
          ATH_MSG_VERBOSE( " Retrieve SF for b-tagged jet in SUSYTools_xAOD::BtagSF with value " << sf );
        }
      } else {

        result = btagTool->getInefficiencyScaleFactor(*jet, sf);

        switch (result) {
        case CP::CorrectionCode::Error:
          ATH_MSG_ERROR( " Failed to retrieve SF for non-b-tagged jet in SUSYTools_xAOD::BtagSF" );
          break;
        case CP::CorrectionCode::OutOfValidityRange:
          ATH_MSG_VERBOSE( " No valid inefficiency SF for non-b-tagged jet in SUSYTools_xAOD::BtagSF" );
          break;
        default:
          ATH_MSG_VERBOSE( " Retrieve SF for non-b-tagged jet in SUSYTools_xAOD::BtagSF with value " << sf );
        }
      }
    }

    //dec_effscalefact(*jet) = sf;

    totalSF *= sf;

  }

  return totalSF;
  
}

// VVFloat chorizo :: GetBTagCalibSyst(BTagCalib* calibTool){

//   std::vector<float> jetWeight_B_down(recoJets.size());
//   std::vector<float> jetWeight_B_up(recoJets.size());
//   std::vector<float> jetWeight_C_down(recoJets.size());
//   std::vector<float> jetWeight_C_up(recoJets.size());
//   std::vector<float> jetWeight_L_down(recoJets.size());
//   std::vector<float> jetWeight_L_up(recoJets.size());

//   std::vector<float> wtmp;
//   std::vector<float> v1(1,0.);
//   std::vector<float> v2(1,0.);
//   std::vector<float> v3(1,0.);
//   std::vector<int>   v4(1,0.);

//   for (unsigned int ij=0; ij < recoJets.size(); ij++){
//     v1[0] = recoJets.at(ij).Pt() * 1000.;
//     v2[0] = BtagEta( recoJets.at(ij).Eta() );
//     v3[0] = recoJets.at(ij).MV1;
//     v4[0] = recoJets.at(ij).FlavorTruth;

//     wtmp = (calibTool->BTagCalibrationFunction(v1, v2 ,v3 ,v4, false)).first;

//     jetWeight_B_down[ij] = wtmp.at(1);
//     jetWeight_C_down[ij] = wtmp.at(2);
//     jetWeight_L_down[ij] = wtmp.at(3);
//     jetWeight_B_up[ij] = wtmp.at(4);
//     jetWeight_C_up[ij] = wtmp.at(5);
//     jetWeight_L_up[ij] = wtmp.at(6);

//   }
//   std::vector< std::vector<float> > jetWeightVectorSys;

//   jetWeightVectorSys.push_back(jetWeight_B_down);
//   jetWeightVectorSys.push_back(jetWeight_B_up);
//   jetWeightVectorSys.push_back(jetWeight_C_down);
//   jetWeightVectorSys.push_back(jetWeight_C_up);
//   jetWeightVectorSys.push_back(jetWeight_L_down);
//   jetWeightVectorSys.push_back(jetWeight_L_up);

//   return VVFloat(jetWeightVectorSys);

// };


//Trigger matching
bool chorizo :: hasTrigMatch(const xAOD::Electron& el, std::string item, double dR){

  //check if at least the event passed this trigger
  if(!tool_trigdec->isPassed(item))  return false;

  TLorentzVector eloff;
  eloff.SetPtEtaPhiE(el.pt(), el.trackParticle()->eta(), el.trackParticle()->phi(), el.e());

  // Get the container of online electrons associated to passed item

  // auto fc = tool_trigdec->features(item, TrigDefs::alsoDeactivateTEs);
  // auto eleFeatureContainers = fc.containerFeature<TrigElectronContainer>(); 
  // bool matched=false;
  // for(auto elfeat : eleFeatureContainers){
  //   cout << "taking new online electron..." << endl;
  //   //const xAOD::ElectronContainer *elCont = elfeat.cptr();
  //   for(const auto& eg : *elfeat.cptr()){
  //     cout << "trying to match..." << endl;
  //     TLorentzVector elLV;
  //     elLV.SetPtEtaPhiE(eg->pt(), eg->trackParticle()->eta(), eg->trackParticle()->phi(), eg->e());
  //     cout << "online " << elLV.Pt() << elLV.Eta() << elLV.Phi() << elLV.E() << endl;
  //     cout << "deltaR = " << elLV.DeltaR(eloff) << endl;
  //     if(elLV.DeltaR(eloff) < dR) 
  // 	return true;
  //   }
  // }

  // auto cg = tool_trigdec->getChainGroup(item);
  // auto fc = cg->features();
  // auto elFeatureContainers = fc.containerFeature<ElectronContainer>();
  // for(auto elcont : elFeatureContainers) {
  //   TLorentzVector elLV;
  //   for (auto el : *elcont.cptr()) {
  //     elLV.SetPtEtaPhiE(el->pt(), el->trackParticle()->eta(), el->trackParticle()->phi(), el->e());
  //     if(elLV.DeltaR(muoff) < dR) 
  // 	return true;
  //   }
  // }


  //ElectronContainer
  //TrigElectronContainer
  //TrigEMCluster

  // auto fc = tool_trigdec->features(item, TrigDefs::alsoDeactivateTEs);
  // auto vec_el = fc.get<xAOD::ElectronContainer>("",TrigDefs::alsoDeactivateTEs);
  // bool matched=false;
  // //cout << "Electron containers : " << vec_el.size() << endl;
  // for(auto elfeat : vec_el){
  //   //    cout << "taking new online electron..." << endl;
  //   for(const auto& eg : *elfeat.cptr()){
  //     cout << "trying to match..." << endl;
  //     TLorentzVector elLV;
  //     elLV.SetPtEtaPhiE(eg->pt(), eg->trackParticle()->eta(), eg->trackParticle()->phi(), eg->e());
  //     //elLV.SetPtEtaPhiE(eg->pt(), eg->eta(), eg->phi(), eg->e());
  //     // cout << "online " << elLV.Pt() << elLV.Eta() << elLV.Phi() << elLV.E() << endl;
  //     // cout << "deltaR = " << elLV.DeltaR(eloff) << endl;
  //     if(elLV.DeltaR(eloff) < dR) 
  // 	return true;
  //   }
  // }


  Trig::FeatureContainer fc = tool_trigdec->features(item);
  const std::vector< Trig::Feature<xAOD::ElectronContainer> > vec_el = fc.get<xAOD::ElectronContainer>();
  for(auto elfeat : vec_el){
    const xAOD::ElectronContainer *elCont = elfeat.cptr();
    TLorentzVector elLV;
    for(const auto& eg : *elCont){
      elLV.SetPtEtaPhiE(eg->pt(), eg->trackParticle()->eta(), eg->trackParticle()->phi(), eg->e());                                                                                                                                                                          
      // cout << "online " << elLV.Pt() << elLV.Eta() << elLV.Phi() << elLV.E() << endl;
      // cout << "deltaR = " << elLV.DeltaR(eloff) << endl;
      if(elLV.DeltaR(eloff) < dR) 
  	return true;
    }
  }

  return false;
}


bool chorizo :: hasTrigMatch(const xAOD::Muon& mu, std::string item, double dR){

  //check if at least the event passed this trigger
  if(!tool_trigdec->isPassed(item))   return false;
  
  TLorentzVector muoff;
  muoff.SetPtEtaPhiE(mu.pt(), mu.eta(), mu.phi(), mu.e());

  auto cg = tool_trigdec->getChainGroup(item);
  auto fc = cg->features();
  auto muFeatureContainers = fc.containerFeature<MuonContainer>();
  for(auto mucont : muFeatureContainers){
    TLorentzVector muLV;
    for (auto mu : *mucont.cptr()){
      muLV.SetPtEtaPhiE(mu->pt(), mu->eta(), mu->phi(), mu->e());
      if(muLV.DeltaR(muoff) < dR) 
	return true;
    }
  }

  return false;
}



//Calculation of extra variables
float chorizo :: Calc_MT(Particles::Particle p, TVector2 met){

  return sqrt(2 * p.Pt() * met.Mod() * (1-cos(deltaPhi(TVector2::Phi_mpi_pi( met.Phi() ), p.Phi()))));

}

float chorizo :: Calc_mct(){

  if(j_N<2) return -1;

  return Calc_mct(recoJets.at(0), recoJets.at(1));
};

float chorizo :: Calc_mct(Particle p1, Particle p2){

  float mct =  (p1.Mt() + p2.Mt())*(p1.Mt() + p2.Mt()) - (p1-p2).Perp2();
  mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);   

  return mct;
};


float chorizo :: Calc_mct_corr(Particle p1, Particle p2, TVector2 met){

  TLorentzVector vds(0.,0.,0.,0.);
  float mct_corr = tool_mct->mctcorr(p1,p2,vds,met,13000.0);; 

  return mct_corr;
};


double chorizo :: Calc_TruthNuMET(){

  TLorentzVector nuMET;
  for(const xAOD::TruthParticle* tp1 : *m_truthP) {
    if (!tp1->isNeutrino()) continue;

    std::pair<MCTruthPartClassifier::ParticleType,MCTruthPartClassifier::ParticleOrigin> res;
    res = tool_mcc->particleTruthClassifier(tp1);
    MCTruthPartClassifier::ParticleOrigin iPartOrig = res.second;

    switch (iPartOrig) {
    case MCTruthPartClassifier::PhotonConv:
    case MCTruthPartClassifier::DalitzDec:
    case MCTruthPartClassifier::ElMagProc:
    case MCTruthPartClassifier::Mu:
    case MCTruthPartClassifier::TauLep:
    case MCTruthPartClassifier::LightMeson:
    case MCTruthPartClassifier::StrangeMeson:
    case MCTruthPartClassifier::CharmedMeson:
    case MCTruthPartClassifier::BottomMeson:
    case MCTruthPartClassifier::CCbarMeson:
    case MCTruthPartClassifier::JPsi:
    case MCTruthPartClassifier::BBbarMeson:
    case MCTruthPartClassifier::LightBaryon:
    case MCTruthPartClassifier::StrangeBaryon:
    case MCTruthPartClassifier::CharmedBaryon:
    case MCTruthPartClassifier::BottomBaryon:
    case MCTruthPartClassifier::PionDecay:
    case MCTruthPartClassifier::KaonDecay:

    case MCTruthPartClassifier::NonDefined:
      continue;
    default:
      break;
    }
    nuMET += tp1->p4();

  }
  return nuMET.Pt();

}


float chorizo :: TopTransvMass(){

  if (j_N < 3) return -10.;
  float m12 = (recoJets.at(0).GetVector() + recoJets.at(1).GetVector()).M();
  float m23 = (recoJets.at(1).GetVector() + recoJets.at(2).GetVector()).M();
  float m13 = (recoJets.at(0).GetVector() + recoJets.at(2).GetVector()).M();

  int bjet = -1;
  if (min(min(m12, m23), m13) == m12){
    bjet = 2;
  }
  if (min(min(m12, m23), m13) == m23){
    bjet = 0;
  }
  if (min(min(m12, m23), m13) == m13){
    bjet = 1;
  }

  return sqrt( pow(recoJets.at(bjet).Et() + sqrt(pow(met_obj.GetVector("met").Mod(), 2)+pow(80.403, 2)), 2) - pow(recoJets.at(bjet).Px()+met_obj.GetVector("met").Px(), 2) - pow(recoJets.at(bjet).Py()+met_obj.GetVector("met").Py(), 2));

}

void chorizo :: RecoHadTops(int ibtop1, int ibtop2){     

  //==== Hadronic top reconstruction ====
  // DRmin implemented as in https://cds.cern.ch/record/1570992/files/ATL-COM-PHYS-2013-1092.pdf
  // 1- select two highest btag-weights bjets
  // 2- two closest light jets --> W1_had --> add closest bjet from before --> t1_had
  // 3- next two closest light jets --> W2_had --> add remaining bjet from before --> t2_had
  //
  // Update: by default the leading good electron and/or muon are added 'as' jets. Although no effect is obviously expected 
  //         in the SRs (since we apply lepton veto), it is needed for the 1-lepton CR afterwards.
  //         This way we avoid doubling the number of variables.
  //
  
  int Wjet1, Wjet2;
  int Wjet3, Wjet4;
  int bjet1 = -1;
  int bjet2 = -1;
  
  //add leading leptons to jet collection
  Particles::Jet ElJet, MuJet;
  bool eladded=false;
  bool muadded=false;
  if(recoElectrons.size()>0){
    ElJet.SetVector( recoElectrons.at(0).GetVector());
    recoJets.push_back(ElJet);
    eladded=true;
  }
  if(recoMuons.size()>0){
    MuJet.SetVector( recoMuons.at(0).GetVector());
    recoJets.push_back(MuJet);
    muadded=true;
  }

  if (bj_N>=1 && recoJets.size()>=3)
    {

      Wjet1=Wjet2=-1;
       
      double mindr = 1000.;
       
      for (unsigned int i = 0 ; i < recoJets.size() ; i++){
	for (unsigned int j = i+1 ; j < recoJets.size() ; j++)
	  {
	    
	   if(recoJets.size()==3){
	    if(i!=ibtop1 && j!=ibtop1){
	      double curr_dr = (recoJets.at(i).GetVector()).DeltaR(recoJets.at(j).GetVector());

	      if (curr_dr<mindr)
		{
		  Wjet1 = i;
		  Wjet2 = j;
		  mindr = curr_dr;
		}
	    }
	   } 
	   else{
	    if(i!=ibtop1 && j!=ibtop1 && i!=ibtop2 && j!=ibtop2){
	      double curr_dr = (recoJets.at(i).GetVector()).DeltaR(recoJets.at(j).GetVector());

	      if (curr_dr<mindr)
		{
		  Wjet1 = i;
		  Wjet2 = j;
		  mindr = curr_dr;
		}
	    }
	   } 	    
	    
	  }
      }
      mindr = 1000.;

      //book first W
      TLorentzVector W1candidate = recoJets.at(Wjet1).GetVector()+recoJets.at(Wjet2).GetVector();

      for (unsigned int i = 0 ; i < recoJets.size() ; i++)
	{
	  if(i==ibtop1 || i==ibtop2 ){	      
	    double curr_dr = (recoJets.at(i).GetVector()).DeltaR(W1candidate);
    	     
	    if (curr_dr<mindr)
	      {
		bjet1 = i;
		mindr = curr_dr;
	      }
	  }
	}

      //book first top
      TLorentzVector top1candidate = recoJets.at(bjet1).GetVector()+W1candidate;
      
      if (bj_N>=2 && recoJets.size()>=6)
      {     
        Wjet3=Wjet4=-1;
        mindr = 1000.;
        for (unsigned int i = 0 ; i < recoJets.size() ; i++){
	  for (unsigned int j = i+1 ; j < recoJets.size() ; j++){
	    
	    if(i!=ibtop1 && i!=ibtop2 && j!=ibtop1 && j!=ibtop2){	    
	      if ((int)i!=Wjet1 && (int)i!=Wjet2 && (int)j!=Wjet1 && (int)j!=Wjet2)
	        {
		  double curr_dr = (recoJets.at(i).GetVector()).DeltaR(recoJets.at(j).GetVector());

		  if (curr_dr<mindr)
		    {
		      Wjet3 = i;
		      Wjet4 = j;
		      mindr = curr_dr;
		    }
	      }
	  }
		
	}	
      }	
      mindr = 1000.;

      //book second W
      TLorentzVector W2candidate = recoJets.at(Wjet3).GetVector()+recoJets.at(Wjet4).GetVector();

      for (unsigned int i = 0 ; i < recoJets.size() ; i++){
	if ((int)i!=bjet1 && ((int)i==ibtop1 || (int)i==ibtop2))
	  {
	    double curr_dr = (recoJets.at(i).GetVector()).DeltaR(W2candidate);
	    	    
	    if (curr_dr<mindr)
	      {
		bjet2 = i;
		mindr = curr_dr;
	      }
	  }
      }

      //book second top
      TLorentzVector top2candidate = recoJets.at(bjet2).GetVector()+W2candidate;
            
      //m_top_had1 = top1candidate.M();
      m_top_had2 = top2candidate.M();
   
    }
      m_top_had1 = top1candidate.M();    
    }
  //recover original jet vector
  if(eladded)
    recoJets.pop_back();
  if(muadded)
    recoJets.pop_back();
 
}


std::vector<TLorentzVector> chorizo :: getFatJets(double R, double fcut) {

  //==== Jet Reclustering ====
  // Recluster good antikt04 jets into new R-jets.
  // It uses fastjet magic with the same antikt algorithm. (at least for now)
  // 
  // Update: by default the leading good electron and/or muon are added 'as' jets. Although no effect is obviously expected 
  //         in the SRs (since we apply lepton veto), it is needed for the 1-lepton CR afterwards.
  //         This way we avoid doubling the number of variables.
  //
  
  //add leading leptons to jet collection
  Particles::Jet ElJet, MuJet;
  bool eladded=false;
  bool muadded=false;
  if(recoElectrons.size()>0){
    ElJet.SetVector( recoElectrons.at(0).GetVector());
    recoJets.push_back(ElJet);
    eladded=true;
  }
  if(recoMuons.size()>0){
    MuJet.SetVector( recoMuons.at(0).GetVector());
    recoJets.push_back(MuJet);
    muadded=true;
  }
  
  vector<PseudoJet> particles;
  vector<TLorentzVector> new_jets_tlv;

  for(unsigned int ijet=0; ijet < recoJets.size(); ijet++){
    //fill      px    py  pz      E
    particles.push_back( PseudoJet( recoJets.at(ijet).Px(),
				    recoJets.at(ijet).Py(),
				    recoJets.at(ijet).Pz(),
				    recoJets.at(ijet).E() ) ); 
  }

  // choose a jet definition
  JetDefinition jet_def(antikt_algorithm, R); //anti-kt as default for now

  // run the clustering, extract the jets
  ClusterSequence cs(particles, jet_def);
  vector<PseudoJet> rc_jets = sorted_by_pt(cs.inclusive_jets());

  //trim & fill new fat jets
  for(const auto& rcjet : rc_jets){
    TLorentzVector trimjet = TLorentzVector();
    vector<PseudoJet> constituents = rcjet.constituents();
    
    for(const auto& jconst : constituents){
      if(fcut>0 && jconst.pt()<fcut*rcjet.pt()) continue; //skip soft subjets
      TLorentzVector subjet = TLorentzVector();
      subjet.SetPxPyPzE(jconst.px(),
			jconst.py(),
			jconst.pz(),
			jconst.E());
      trimjet += subjet;
    }

    new_jets_tlv.push_back( trimjet );
  }

  //recover original jet vector
  if(eladded)
    recoJets.pop_back();
  if(muadded)
    recoJets.pop_back();

  return new_jets_tlv;
} 

float chorizo::Calc_Mjj(TLorentzVector ja,TLorentzVector jb){
  return (ja + jb).M();
}

float chorizo :: Calc_Mjj(){
  if(j_N < 2) return -1.;
  return Calc_Mjj(recoJets.at(0).GetVector(), recoJets.at(1).GetVector());
}


float chorizo :: Calc_Sphericity(std::vector<TLorentzVector> pvectors,
				 bool IsTransverseSphericity)
{
  //!matrix elements are set to 0
  // Defines the Sphericity Matrix
  TMatrixD sphericityMatrix(3, 3);
  for (int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      sphericityMatrix[i][j] = 0.;
  Double_t d_sph_p = 0.;

  //////////////////////////
  //! Calculates Sphericity Matrix S_{ij} = \sum_k p_k^j p_k^j  / \sum_k |p_k|^2
  Double_t d_sph_Pxyz[3];
  for(int i = 0; i < 3; i++) d_sph_Pxyz[i] = 0.;
  for(unsigned int indexI = 0;indexI < pvectors.size();indexI++) {
    for(int i = 0; i < 3; i++) d_sph_Pxyz[i] = 0.;
    d_sph_Pxyz[0] = pvectors[indexI].Px();
    d_sph_Pxyz[1] = pvectors[indexI].Py();

    //! only if the 3D Sph is calculated, one needs pz != 0
    if (!IsTransverseSphericity) d_sph_Pxyz[2] = pvectors[indexI].Pz();

    //! The Sphericity Matrix is calculated. If pz = 0 it
    //! is the needed 2x2 Matrix
    for(int iMatrixI = 0; iMatrixI < 3; iMatrixI++)
      for(int iMatrixJ = 0; iMatrixJ < 3; iMatrixJ++)
        sphericityMatrix[iMatrixI][iMatrixJ] += d_sph_Pxyz[iMatrixI]*d_sph_Pxyz[iMatrixJ];

    //! calculates \sum_k |p_k|^2
    d_sph_p += d_sph_Pxyz[0]*d_sph_Pxyz[0] +
               d_sph_Pxyz[1]*d_sph_Pxyz[1] +
      d_sph_Pxyz[2]*d_sph_Pxyz[2];
  }

  //!  Normalizes S_{ij}
  if(d_sph_p != 0.) {
    for(int iMatrixI = 0; iMatrixI < 3; iMatrixI++) {
      for(int iMatrixJ = 0; iMatrixJ < 3; iMatrixJ++) {
        sphericityMatrix[iMatrixI][iMatrixJ]=
          sphericityMatrix[iMatrixI][iMatrixJ]/d_sph_p;
      }
    }
    //! if there are no values available, it crashes.
  } else {
    // Cleaning
    return -99.;
  }

  //! Calculate the EigenValues
  TVectorD eigenValues;
  const TMatrixD eigenVectoren = sphericityMatrix.EigenVectors(eigenValues);

  //! The EigenValues have to be sorted: Lambda1 > Lambda2 > Lambda3
  Int_t eigenLambda1 = 0, eigenLambda2 = 1, eigenLambda3 = 2;
  // from the babar sphericity code...
  double Emax = eigenValues[0];
  double Emin = eigenValues[0];
  for ( int i = 0; i <= 2 ; ++i ) {
    if ( eigenValues[i] > Emax ) {
      Emax = eigenValues[i];
      eigenLambda1 = i;
    }
    if ( eigenValues[i] < Emin ) {
      Emin = eigenValues[i];
      eigenLambda3 = i;
    }
  }
  eigenLambda2 = 3 - eigenLambda3 - eigenLambda1;

  //! Calculates the Sphericity with
  //! S_T = 2 \lambda_2/(\lambda_1 + \lambda_2)
  //! TDR Vol II, May 1999, Page 820
  double sphericity = -99.;
  if (IsTransverseSphericity)
    sphericity = 2.*eigenValues[eigenLambda2]/
      (eigenValues[eigenLambda1] + eigenValues[eigenLambda2]);
  else
    sphericity = (3./2.)*
      (eigenValues[eigenLambda2] + eigenValues[eigenLambda3]);

  return sphericity;
}

//----------- THRUST
//A modified step function used to compute the thrust axis
double chorizo::epsilon(double x){
  if(x>=0) return 1;
  if(x<0) return -1;
  return -99;
}

//This function return the transverse thrust value for a given thrust axis
double chorizo::thrustService(TVector2 &n, std::vector<TVector2> &obj){
  double num=0;
  double den=0;

  for(unsigned int i=0; i<obj.size(); i++){
    num+=TMath::Abs(n*obj[i]);
    den+=obj[i].Mod();
  }

  if(den==0) return -99;
  return num/den;
}

//Main function where the thrust axis which maximize the transverse thrust value is computed
double chorizo::Calc_Thrust(std::vector<TLorentzVector> pvectors) {
  
  if(pvectors.size()==1) return 0;

  //transform to TVector2
  std::vector<TVector2> obj;
  for(const auto& tlv : pvectors)
    obj.push_back( tlv.Vect().XYvector() );

  if(obj.size()==0) return -99; //?

  TVector2 n;
  TVector2 np1(obj[0]);
  np1=np1.Unit();
  bool loop_controller=true;

  int iloop=0;
  int iloop2=0;
  double thrust_old=0;
  double thrust_new=-1;
  double thrust_larger=0;
  double T=-99;
  int n_lar_T_with_same_value=0;

  //Initializing Thrust axis. By default it use the leading jet direction to initialize the thrust axis
  for(unsigned int i=1; i<obj.size();i++) np1+=obj[i];
  if(np1.X()==0 && np1.Y()==0) np1.Set(obj[0]);
  np1=np1.Unit();
 

  //Computing transverse Thrust value. To ensure the convergence the algorithm requires to obtain 3 times the same maximum transverse Thrust value using 3 different initializations for the thrust axis
  do{
    loop_controller=true;

    //changing thrust axis initialization after the first loop
    if(iloop2>0){
      np1.Set(1.,0.);
      np1=np1.Rotate((iloop2-1));
      np1=np1.Unit();
    }
    iloop=0;

    //loop to find the axis which maximize the transverse Thrust value
    do{
      n=np1;
      iloop++;

      if(iloop==1000){
        loop_controller=false;
        thrust_new=-1;
        break;
      }

      thrust_old=thrustService(np1,obj);

      //the axis which maximize the thrust value is the result of the convergence of this series
      for(unsigned int i=0; i<obj.size(); i++){
        np1+=epsilon(n*obj[i])*obj[i];
      }

      np1=np1.Unit();
      thrust_new=thrustService(np1,obj);

    }while(fabs(thrust_new-thrust_old)>10e-9);


    if(thrust_new>thrust_larger && loop_controller ){
      thrust_larger=thrust_new;
      n_lar_T_with_same_value=0;
    }

   
    if(fabs((thrust_new-thrust_larger)<10e-9) && loop_controller ) n_lar_T_with_same_value++;

    iloop2++;

    loop_controller=true;

    if(iloop2==1000){
      loop_controller=false;
      if(errIgnoreLevel==kPrint){ //Debugging
	Warning("Calc_Thrust()", Form("Thrust Computation: convergence 2 failed after: %d loop", iloop2));
	std::cout<<"n_lar_T_with_same_value: "<<n_lar_T_with_same_value<<std::endl;
	std::cout<<"(1-thrust_larger)/(1-2/TMath::Pi()): "<<(1-thrust_larger)/(1-2/TMath::Pi())<<std::endl;
	std::cout<<"pvectors.size(): "<<pvectors.size()<<std::endl;
	std::cout<<"i     pt      px      py      pxobj   pyobj"<<std::endl;
	for(unsigned int i=0; i<pvectors.size();i++){
	  std::cout<<i<<"  "<<pvectors[i].Pt()<<"    "<<pvectors[i].Px()<<"    "<<pvectors[i].Py()<<"    "<<obj[i].X()<<"        "<<obj[i].Y()<<std::endl;
	}
      }
      return -99;
    }
    //to accept the thrust value we require:
    //1- it has to be into [2/pi,1] which is the range for the transverse thrust
    //2- The maximum thrust value has to be obtain 3 times using 3 different initializations for the trust axis

  }while( ( thrust_larger<2/TMath::Pi() || n_lar_T_with_same_value<3 ) && loop_controller==true );

  T=(1-thrust_larger)/(1-2/TMath::Pi());

  if(T<0-10e-9 || T>1){
    if(pvectors.size()>1){
      Warning("Calc_Thrust()", Form("Thrust Computation: ERROR, thrust value out of range: %.2f", T));
      std::cout<<"i       pt      px      py      pxobj   pyobj"<<std::endl;
      for(unsigned int i=0; i<pvectors.size();i++){
	std::cout<<i<<"  "<<pvectors[i].Pt()<<"    "<<pvectors[i].Px()<<"    "<<pvectors[i].Py()<<"    "<<obj[i].X()<<"        "<<obj[i].Y()<<std::endl;
      }
    }
    return -99;
  }

  return T;
 
}




//----------- RAZOR

//fill Razor variables
void chorizo :: fillRazor(){
  fillRazor(met_obj.GetMET_Razor());
}

void chorizo :: fillRazor(TVector3 mymet){

  std::vector<TLorentzVector> megajets = CombineJets();
  
  MR = Calc_MR(megajets.at(0), megajets.at(1));
  AlphaT = Calc_AlphaT(megajets.at(0), megajets.at(1));

  float tmp_MTR = Calc_MTR(megajets.at(0), megajets.at(1), mymet);   
  MTR.push_back( tmp_MTR );

  if(MR > 0.) 
    R.push_back( tmp_MTR / MR );
  
  //new super-razor variables
  float sr1=0.;
  float sr2=0.;
  float sr3=0.;
  float sr4=0.;
  Calc_SuperRazor(megajets.at(0), megajets.at(1), mymet, sr1, sr2, sr3, sr4);

  shatR.push_back(sr1);  
  gaminvR.push_back(sr2);
  mdeltaR.push_back(sr3);
  cosptR.push_back(sr4);

}

//Build megajets
vector<TLorentzVector> chorizo :: CombineJets(){ //from recoJets collection

  vector<TLorentzVector> myjets;
  for (unsigned int ij=0; ij < recoJets.size(); ij++)
    myjets.push_back( recoJets.at(ij).GetVector() );

  return CombineJets(myjets);
}

vector<TLorentzVector> chorizo :: CombineJets(vector<TLorentzVector> myjets){

  vector<TLorentzVector> mynewjets;
  TLorentzVector j1, j2;
  int N_comb = 1;
  unsigned int size =0;

  //at most loop over 5 jets (otherwise problem for data events)
  if (myjets.size() < 5) size = myjets.size();
  else size = 5;
 
  for(unsigned int i = 0; i < size; i++){
    N_comb *= 2;
  }

  double M_min = 9999999999.0;
  int j_count;
  for(unsigned int i = 1; i < size; i++){// instead of N_comb-1
    TLorentzVector j_temp1, j_temp2;
    int itemp = i;
    j_count = N_comb/2;
    int count = 0;
    while(j_count > 0){
      if(itemp/j_count == 1){
        j_temp1 += myjets[count];
      } else {
        j_temp2 += myjets[count];
      }
      itemp -= j_count*(itemp/j_count);
      j_count /= 2;
      count++;
    }
    double M_temp = j_temp1.M2()+j_temp2.M2();
    // smallest mass
    if(M_temp < M_min){
      M_min = M_temp;
      j1 = j_temp1;
      j2 = j_temp2;
    }
  }  
  if(j2.Pt() > j1.Pt()){
    TLorentzVector temp = j1;
    j1 = j2;
    j2 = temp;
  }
  mynewjets.push_back(j1);
  mynewjets.push_back(j2);

  return mynewjets;
}

float chorizo :: Calc_MR(TLorentzVector ja, TLorentzVector jb){
  float A = ja.P();
  float B = jb.P();
  float az = ja.Pz();
  float bz = jb.Pz();
  TVector3 jaT, jbT;
  jaT.SetXYZ(ja.Px(),ja.Py(),0.0);
  jbT.SetXYZ(jb.Px(),jb.Py(),0.0);
  float ATBT = (jaT+jbT).Mag2();
  float temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-(jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());
  float mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
  float mygamma = 1./sqrt(1.-mybeta*mybeta);
  //gamma times MRstar
  temp *= mygamma;
  return temp;
}

float chorizo :: Calc_MTR(TLorentzVector ja, TLorentzVector jb, TVector3 met){
  float temp = met.Mag()*(ja.Pt()+jb.Pt()) - met.Dot(ja.Vect()+jb.Vect());
  temp /= 2.;
  temp = sqrt(temp);
  return temp;
 
}

float chorizo :: Calc_AlphaT(TLorentzVector ja, TLorentzVector jb){

  float form = (ja.Et()+jb.Et())*(ja.Et()+jb.Et()) - (ja.Px()+jb.Px())*(ja.Px()+jb.Px()) - (ja.Py()+jb.Py())*(ja.Py()+jb.Py());
  float mT = ( form >=0 ? sqrt(form) : 0.);
  
  float AlphaT = (mT > 0 ? jb.Et()/mT : -999);
  return AlphaT;
}


void chorizo :: Calc_SuperRazor(TLorentzVector J1, TLorentzVector J2, TVector3 met, float& shatR ,float& gaminvR, float& mdeltaR, float& cosptR){

  ////////////////////////////////
  // SUPER-RAZOR VARIABLES CALCULATION 
  //
  // Code written by Christopher Rogan <crogan@cern.ch>, 27-04-14
  // Adapted from paper PRD 89, 055020 (http://arxiv.org/abs/1310.4827) written by
  // Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
  ////////////////////////////////

  /////////////
  // LAB frame
  /////////////
  //Reconstructed mega-jets and missing transverse energy (from somewhere else...)

  J1.SetVectM(J1.Vect(),0.0);
  J2.SetVectM(J2.Vect(),0.0);

  TVector3 vBETA_z = (1./(J1.E()+J2.E()))*(J1+J2).Vect();
  vBETA_z.SetX(0.0);
  vBETA_z.SetY(0.0);

  //transformation from lab frame to approximate rest frame along beam-axis
  J1.Boost(-vBETA_z);
  J2.Boost(-vBETA_z);

  TVector3 pT_CM = (J1+J2).Vect() + met;
  pT_CM.SetZ(0.0); //should be redundant...

  float Minv2 = (J1+J2).M2();
  float Einv = sqrt(met.Mag2()+Minv2);

  //////////////////////
  // definition of shatR
  //////////////////////
  shatR = sqrt( ((J1+J2).E()+Einv)*((J1+J2).E()+Einv) - pT_CM.Mag2() );

  TVector3 vBETA_R = (1./sqrt(pT_CM.Mag2() + shatR*shatR))*pT_CM;

  //transformation from lab frame to R frame
  J1.Boost(-vBETA_R);
  J2.Boost(-vBETA_R);

  /////////////
  // R-frame
  /////////////
  TVector3 vBETA_Rp1 = (1./(J1.E()+J2.E()))*(J1.Vect() - J2.Vect());

  ////////////////////////
  // definition of gaminvR
  gaminvR = sqrt(1.-vBETA_Rp1.Mag2());

  //transformation from R frame to R+1 frames
  J1.Boost(-vBETA_Rp1);
  J2.Boost(vBETA_Rp1);

  //////////////
  // R+1-frames
  //////////////
  ///////////////////////
  // definition of mdeltaR
  mdeltaR = J1.E()+J2.E();

  ///////////////////////
  // definition of cosptR
  cosptR = pT_CM.Mag()/sqrt(pT_CM.Mag2()+mdeltaR*mdeltaR);

}

void chorizo :: findBparton(){

  TLorentzVector B;
  TLorentzVector SB;
  TLorentzVector N1;

  std::vector<TLorentzVector> v_B;
  std::vector<TLorentzVector> v_SB;
  std::vector<TLorentzVector> v_N1;

  
  // //--- get stop/sbottom
  // for (unsigned int ipart=0; ipart<(unsigned int)mc_n; ++ipart){
  //   if ( fabs(mc_pdgId->at(ipart))==1000005 || fabs(mc_pdgId->at(ipart))==1000006 ){

  //     SB.SetPtEtaPhi( (*truthP_itr->pt()*0.001, (*truthP_itr)->eta(), (*truthP_itr)->phi(),(*truthP_itr)->e()*0.001) );
  //     v_SB.push_back( SB );

  //     //--- sparticle with 2 childrens
  //     std::vector<int> vect_child_index = mc_child_index->at(ipart);
  //     if (vect_child_index.size()>=2){
  //       for (unsigned int ich=0; ich<vect_child_index.size(); ++ich){
  //         int index = vect_child_index.at(ich);
 
  //        //--- Get the Neutralino
  //         if ( fabs(mc_pdgId->at(index))==1000022 ){
    
  // 	    N1.SetPtEtaPhi( (*truthP2_itr->pt()*0.001, (*truthP2_itr)->eta(), (*truthP2_itr)->phi(),(*truthP2_itr)->e()*0.001) );
  // 	    v_N1s.push_back( N1 );

  //         }

  //         //--- Get the b/c parton
  //         if ( fabs(mc_pdgId->at(index))==5 || fabs(mc_pdgId->at(index))==4 ){

  // 	    B.SetPtEtaPhi( (*truthP2_itr->pt()*0.001, (*truthP2_itr)->eta(), (*truthP2_itr)->phi(),(*truthP2_itr)->e()*0.001) );
  // 	    v_B.push_back( B );

  //         }
  //       }
  //     }
  //   }
  // }

  //--- For bottom partons and Neutralinos (numeration in accordance to the bottom pt)
  if (v_B.size()==2){ 
    int ih = (v_B.at(0).Pt() > v_B.at(1).Pt() ? 0 : 1); //leading
    int is = (ih==0 ? 1 : 0);                           //sub-leading

    t_sb_pt1    = v_SB.at(ih).Pt();    t_sb_eta1    = v_SB.at(ih).Eta();    t_sb_phi1    = v_SB.at(ih).Phi();
    t_b_pt1     = v_B.at(ih).Pt();     t_b_eta1     = v_B.at(ih).Eta();     t_b_phi1     = v_B.at(ih).Phi();
    t_neutr_pt1 = v_N1.at(ih).Pt();     t_neutr_eta1 = v_N1.at(ih).Eta();     t_neutr_phi1 = v_N1.at(ih).Phi();

    t_sb_pt2    = v_SB.at(ih).Pt();    t_sb_eta2    = v_SB.at(ih).Eta();    t_sb_phi2    = v_SB.at(ih).Phi();
    t_b_pt2     = v_B.at(ih).Pt();     t_b_eta2     = v_B.at(ih).Eta();     t_b_phi2     = v_B.at(ih).Phi();
    t_neutr_pt2 = v_N1.at(ih).Pt();     t_neutr_eta2 = v_N1.at(ih).Eta();     t_neutr_phi2 = v_N1.at(ih).Phi();
  }
  else { //--- If I don't pick two sbottoms, I'm not using the information
    t_sb_pt1 = 0; t_sb_eta1 = 0; t_sb_phi1 = 0;
    t_sb_pt2 = 0; t_sb_eta2 = 0; t_sb_phi2 = 0;
    t_b_pt1 = 0; t_b_eta1 = 0; t_b_phi1 = 0;
    t_b_pt2 = 0; t_b_eta2 = 0; t_b_phi2 = 0;
    t_neutr_pt1 = 0; t_neutr_eta1 = 0; t_neutr_phi1 = 0;
    t_neutr_pt2 = 0; t_neutr_eta2 = 0; t_neutr_phi2 = 0;
  }
  /*
    else if (vectorOfB.size()==1) {
    t_sb_pt1    = v_SB.at(ih).Pt();    t_sb_eta1    = v_SB.at(ih).Eta();    t_sb_phi1    = v_SB.at(ih).Phi();
    t_b_pt1     = v_B.at(ih).Pt();     t_b_eta1     = v_B.at(ih).Eta();     t_b_phi1     = v_B.at(ih).Phi();
    t_neutr_pt1 = v_N1.at(ih).Pt();     t_neutr_eta1 = v_N1.at(ih).Eta();     t_neutr_phi1 = v_N1.at(ih).Phi();
    t_sb_pt2 = 0; t_sb_eta2 = 0; t_sb_phi2 = 0;
    t_b_pt2 = 0; t_b_eta2 = 0; t_b_phi2 = 0;
    t_neutr_pt2 = 0; t_neutr_eta2 = 0; t_neutr_phi2 = 0;
    }
  */
}

bool chorizo :: isQCDSeedEvent(float MissingET, float sumET, float etMissSigCut){
    
  const double etMissSig = MissingET / sqrt(sumET);
  if (etMissSig < etMissSigCut) return true;
  
  return false;
}

void chorizo :: Zll_candidate(){
  
  float m_ll_tmp = 0.;  
  float ZMASS=91.1876;

   if (recoElectrons.size()>1){
     for (unsigned int iEl=0; iEl<recoElectrons.size(); iEl++){
       for (unsigned int jEl=iEl+1; jEl<recoElectrons.size(); jEl++){   

	 if ((recoElectrons.at(iEl).charge * recoElectrons.at(jEl).charge) < 0){
         
	   TLorentzVector ee = recoElectrons.at(iEl).GetVector() + recoElectrons.at(jEl).GetVector();	 
	   m_ll_tmp = ee.M();	 

	   if (fabs(m_ll_tmp - ZMASS) < fabs(Z_m -  ZMASS)){
	     Z_m = m_ll_tmp;
	     Z_lep1 = iEl;
	     Z_lep2 = jEl;	   
	     Z_flav = 0;
	   }
	 }
       }
     }
   } 
   
   if (recoMuons.size()>1){
     for (unsigned int iMu=0; iMu<recoMuons.size(); iMu++){
       for (unsigned int jMu=iMu+1; jMu<recoMuons.size(); jMu++){   
	 
	 if ((recoMuons.at(iMu).charge * recoMuons.at(jMu).charge) < 0){     
           TLorentzVector mumu = recoMuons.at(iMu).GetVector() + recoMuons.at(jMu).GetVector();	 
	   m_ll_tmp = mumu.M();	 

	   if (fabs(m_ll_tmp - ZMASS) < fabs(Z_m - ZMASS)){
	     Z_m = m_ll_tmp;
	     Z_lep1 = iMu;
	     Z_lep2 = jMu;
	     Z_flav = 1;		
	   }
	 }
       }  
     }
   }

   if (recoElectrons.size()+recoMuons.size()==4){

     std::vector<Particle> extra_lep;  
     
     for (unsigned int iEl=0; iEl<recoElectrons.size(); iEl++){
       
       if(Z_flav==0 && ((int)Z_lep1==iEl || (int)Z_lep2==iEl)) continue; 
       extra_lep.push_back(recoElectrons.at(iEl));
       
     } 	
     
     
     for (unsigned int iMu=0; iMu<recoMuons.size(); iMu++){
       
       if(Z_flav==1 && ((int)Z_lep1==iMu || (int)Z_lep2==iMu)) continue;	      
       extra_lep.push_back(recoMuons.at(iMu));
       
     }  
     
     if(extra_lep.size()>1)  //redundant I think!
       lep_mct = Calc_mct(extra_lep.at(0), extra_lep.at(1));
     
   }  
   
}

void chorizo :: Zll_extra(TVector2 met){

  
  if (recoElectrons.size()+recoMuons.size()==3){
    
    for (unsigned int iEl=0; iEl<recoElectrons.size(); iEl++){
      
      if(Z_flav==0 && ((int)Z_lep1==iEl || (int)Z_lep2==iEl)) continue; 
      lep3_MT.push_back( Calc_MT(recoElectrons.at(iEl), met) );
      
    } 	
    
    for (unsigned int iMu=0; iMu<recoMuons.size(); iMu++){
      
      if(Z_flav==1 && ((int)Z_lep1==iMu || (int)Z_lep2==iMu)) continue;	      
      lep3_MT.push_back( Calc_MT(recoMuons.at(iMu), met) );
      
    }  
  }
} 


float chorizo :: amt2_calc(TLorentzVector b1v, TLorentzVector b2v, TLorentzVector lepton,TVector2 EtMissVec, double cut) {

  ComputeMT2* mycalc;
   
  TLorentzVector lepb1 = b1v + lepton;
  TLorentzVector lepb2 = b2v + lepton;
  float amt2=0;

  if((lepb1.M()>cut)&&(lepb2.M()>cut))  //set overflow bin to 0 if no combinations are a 'top'
    {
      amt2=0;
    }
  else if ((lepb1.M()<cut)&&(lepb2.M()>cut))  // if lepb1 is the 'top'
    {
      mycalc = new ComputeMT2(lepb1,b2v,EtMissVec,0,0);
      amt2 = mycalc->Compute();
      delete mycalc;
    }
  else if ((lepb1.M()>cut)&&(lepb2.M()<cut)) // if lepb2 is the 'top'
    {
      mycalc = new ComputeMT2(lepb2,b1v,EtMissVec,0,0);
      amt2 = mycalc->Compute();
      delete mycalc;
    }
  else  // if both lepb1 and lepb2 are the 'top' calculate minimum of mt2
    {
      mycalc = new ComputeMT2(lepb1,b2v,EtMissVec,0,0);
      double tmp1 = mycalc->Compute();
      mycalc = new ComputeMT2(lepb2,b1v,EtMissVec,0,0);
      double tmp2 = mycalc->Compute();
      amt2 = min(tmp1,tmp2);
      delete mycalc;
    }
  return amt2;
 
}


TVector2 chorizo :: getMET( const xAOD::MissingETContainer* METcon, TString name ) {
  
  TVector2 myMet(0.,0.);
  xAOD::MissingETContainer::const_iterator met_it = METcon->find( name.Data() );
  if (met_it == METcon->end())
    Warning("getMET()", Form("No '%s' inside MET container. Setting MET=(0,0) !", name.Data()));
  else
    myMet.Set((*met_it)->mpx(), (*met_it)->mpy());

  return myMet;
}

// TVector2 chorizo :: getMET( const xAOD::MissingETContainer METcon, TString name ) {
  
//   TVector2 myMet(0.,0.);
//   xAOD::MissingETContainer::const_iterator met_it = METcon.find( name.Data() );
//   if (met_it == METcon.end())
//     Warning("getMET()", Form("No '%s' inside MET container. Setting MET=(0,0) !", name.Data()));
//   else
//     myMet.Set((*met_it)->mpx(), (*met_it)->mpy());

//   return myMet;
// }


//Returns factor for PDF reweighting
double chorizo :: getPdfRW(   double rwScale,
			      double pdf_scale2,
			      double pdf_x1, 
			      double pdf_x2,
			      int pdg_id1,
			      int pdg_id2
			      ){
  return this->getPdfRW(m_PDF, rwScale, pdf_scale2, pdf_x1, pdf_x2, pdg_id1, pdg_id2);
}

double chorizo :: getPdfRW(   LHAPDF::PDF* pdf,
			      double rwScale,
			      double pdf_scale2,
			      double pdf_x1, 
			      double pdf_x2,
			      int pdg_id1,
			      int pdg_id2
			      ){
  return this->getPdfRW(pdf, pdf, rwScale, pdf_scale2, pdf_x1, pdf_x2, pdg_id1, pdg_id2);
}

double chorizo :: getPdfRW(   LHAPDF::PDF* pdfFrom,
			      LHAPDF::PDF* pdfTo,
			      double rwScale,
			      double pdf_scale2,
			      double pdf_x1, 
			      double pdf_x2,
			      int pdg_id1,
			      int pdg_id2
			      ){

  //if same to-from pdfs and no scaling required just leave
  if( rwScale==1 && 
      pdfFrom->lhapdfID()==pdfTo->lhapdfID() && 
      pdfFrom->memberID()==pdfTo->memberID() )
    return 1.;
  

  if (pdf_x1==0 || pdf_x2==0 || pdg_id1==0 || pdg_id2==0 || pdf_scale2==0) return 0.; //it doesn't know what to do then

  // original pdf of particle1
  double pdf1org = pdfFrom->xfxQ2( pdg_id1, pdf_x1, pdf_scale2) / pdf_x1;

  // original pdf of particle2
  double pdf2org = pdfFrom->xfxQ2( pdg_id2, pdf_x2, pdf_scale2) / pdf_x2;

  // new pdf of particle1
  double pdf1new = pdfTo->xfxQ2( pdg_id1, pdf_x1/rwScale, pdf_scale2) / (pdf_x1/rwScale);

  // new pdf of particle2
  double pdf2new = pdfTo->xfxQ2( pdg_id2, pdf_x2/rwScale, pdf_scale2) / (pdf_x2/rwScale);

  // factor for reweighting
  double weight = (pdf1new * pdf2new) / (pdf1org * pdf2org) / pow(rwScale,2);

  return weight;
};


template <class xAODPart>
void chorizo :: fillTLV( TLorentzVector &v, xAODPart* p, bool inGeV ){
  // Fills xAOD particle 4-mom to TLV.
  // Note: in MEV, unless you set inGeV=true ! 

  if( inGeV ){
    v.SetPtEtaPhiM( p->pt()*0.001, p->eta(), p->phi(), p->m()*0.001 );
  }
  else{
    //    v = p->p4();
    v.SetPtEtaPhiM( p->pt(), p->eta(), p->phi(), p->m() );
  }
} 

template <class xAODPart>
TLorentzVector chorizo :: getTLV( xAODPart* p, bool inGeV ){
  // Returns TLV from xAOD particle 4-mom.
  // NOTe: in MEV, unless you set inGeV=true ! 

  TLorentzVector v;
  fillTLV(v, p, inGeV);

  return v;
}


template<class T, class V> bool SortBySecond(std::pair<T, V> pair1, std::pair<T, V> pair2)
{
	return pair1.second < pair2.second;
}


/// <summary>
/// Gets the jet incidices ordered by delta R to the refVector.
/// </summary>
/// <param name="lcJetReader">The lcJetReader.</param>
/// <param name="refVector">The reference vector.</param>
/// <param name="skipIndices">The indices to skip.</param>
/// <returns>vector of pairs (JetIndex, dR) to the refVector</returns>
std::vector<pair<unsigned int, double> > chorizo :: GetOrderedJetIndexdR(TLorentzVector refVector, std::vector<unsigned int> skipIndices)
{
	std::vector<pair<unsigned int, double> > jetIndexdRPairVector;
        for (unsigned int jetIndex = 0 ; jetIndex < recoJets.size() ; jetIndex++)
	//for (size_t jetIndex = 0; jetIndex < lcJetReader.NJets(); jetIndex++)
	{
		if (find(skipIndices.begin(), skipIndices.end(), jetIndex) != skipIndices.end())
		{
			//Found the index in skip list, skipping.
			continue;
		}
		
		TLorentzVector jet;
		jet.SetPtEtaPhiM(
			recoJets.at(jetIndex).Pt(),
			recoJets.at(jetIndex).Eta(),
			recoJets.at(jetIndex).Phi(),
			recoJets.at(jetIndex).M()
			);

		jetIndexdRPairVector.push_back(make_pair(jetIndex, refVector.DeltaR(jet)));		
	}

	sort(jetIndexdRPairVector.begin(), jetIndexdRPairVector.end(), SortBySecond<unsigned int, double>);

	return jetIndexdRPairVector;
}

/// <summary>
/// Gets the jet incidices ordered by delta R/pT to the refVector.
/// </summary>
/// <param name="lcJetReader">The lcJetReader.</param>
/// <param name="refVector">The reference vector.</param>
/// <param name="skipIndices">The indices to skip.</param>
/// <returns>vector of pairs (JetIndex, dR) to the refVector</returns>
std::vector<pair<unsigned int, double> > chorizo :: GetOrderedJetIndexAntiKtd(TLorentzVector refVector, std::vector<unsigned int> skipIndices)
{

     enum dAlgorithm
     { 
	JETPT,
	REFPT,
	MAXJETBBPT,
     };
        
	dAlgorithm d_algorithm;
        	
	std::vector<pair<unsigned int, double> > jetIndexdRPairVector;
        for (unsigned int jetIndex = 0 ; jetIndex < recoJets.size() ; jetIndex++)
	//for (size_t jetIndex = 0; jetIndex < lcJetReader.NJets(); jetIndex++)
	{
		if (find(skipIndices.begin(), skipIndices.end(), jetIndex) != skipIndices.end())
		{
			//Found the index in skip list, skipping.
			continue;
		}
		TLorentzVector jet;
		jet.SetPtEtaPhiM(
			recoJets.at(jetIndex).Pt(),
			recoJets.at(jetIndex).Eta(),
			recoJets.at(jetIndex).Phi(),
			recoJets.at(jetIndex).M()
			);

		double d;
		switch (d_algorithm)
		{
			case JETPT:
				d = refVector.DeltaR(jet)/recoJets.at(jetIndex).Pt();
				break;
			case REFPT:
				d = refVector.DeltaR(jet)/refVector.Pt();
				break;
			case MAXJETBBPT:
			default:
				d = refVector.DeltaR(jet)/max(recoJets.at(jetIndex).Pt(), refVector.Pt());
				break;
		}
		jetIndexdRPairVector.push_back(make_pair(jetIndex, d));
	}

	sort(jetIndexdRPairVector.begin(), jetIndexdRPairVector.end(), SortBySecond<unsigned int, double>);

	return jetIndexdRPairVector;
}



EL::StatusCode chorizo :: doTrackVeto(std::vector<Particle> electronCandidates, std::vector<Particle> muonCandidates){
  
  //clear track vectors
  trackUnmatched_truthObject.clear();
  trackUnmatched_truthObjectPt.clear();
  trackUnmatched_truthObjectEta.clear();
  
  isolatedTrack_Pt.clear();
  isolatedTrack_Eta.clear();
  isolatedTrack_trkIso.clear();
  isolatedTrack_MatchedLepton.clear();
  isolatedTrack_MatchedJet.clear();
  
  RecoUnmatchedTracksElMu.clear();
  RecoUnmatchedTracksIdx.clear();
  
  //retrieve track container
  const xAOD::TrackParticleContainer* tracks;
  CHECK( m_event->retrieve( tracks, "GSFTrackParticles" ) );

  //track loop
  xAOD::TrackParticleContainer::const_iterator trk_itr = tracks->begin();
  xAOD::TrackParticleContainer::const_iterator trk_end = tracks->end();
  for( ; trk_itr != trk_end; ++trk_itr ) {
    
    TLorentzVector RecoTrack;
    RecoTrack.SetPtEtaPhiE( (*trk_itr)->pt(), (*trk_itr)->eta(), (*trk_itr)->phi(), (*trk_itr)->pt()*cosh((*trk_itr)->eta()) );//CHECK_ME //why not ->e()?
    
    // track selection
    if( (*trk_itr)->pt()  < tVeto_Pt ) continue;
    if( fabs((*trk_itr)->eta()) < tVeto_Eta ) continue;
    if( fabs((*trk_itr)->d0()) < tVeto_d0 ) continue; //d0_wrtPV // FIX_ME
    if( fabs((*trk_itr)->z0()) < tVeto_z0 ) continue; //z0_wrtPV // FIX_ME
    if( (*trk_itr)->numberDoF() == tVeto_ndof ) continue;
    double trch = (*trk_itr)->chiSquared() / (*trk_itr)->numberDoF();
    if( (trch < tVeto_chi2OverNdof_min) || (trch > tVeto_chi2OverNdof_max) ) continue;
    int trk_PixHits = acc_nPixHits(**trk_itr);
    int trk_SCTHits = acc_nSCTHits(**trk_itr);
    if( trk_PixHits+trk_SCTHits < PixHitsAndSCTHits ) continue;
    
    // require track isolation
    //      if((*trk_cone40_ptmin3gev_hitschi_nTrackIso)[it] > tVeto_TrackIso[iter->first]) continue; //FIX_ME
    
    isolatedTrack_Pt.push_back( RecoTrack.Pt() );
    isolatedTrack_Eta.push_back( RecoTrack.Eta() );
    isolatedTrack_trkIso.push_back( 0.);//(*trk_cone40_ptmin3gev_hitschi_nTrackIso)[it]); //FIX_ME
    
    // Fill RecoTrack
    bool isMatchedLepton = false;
    for (unsigned int iEl=0; iEl < electronCandidates.size(); ++iEl){
      if (RecoTrack.DeltaR(electronCandidates[iEl])<=0.2){ //--- Unmatched track-electron
	isMatchedLepton = true;
	isolatedTrack_MatchedLepton.push_back(11);
	break;
      } 
    }
    
    for (unsigned int iMu=0; iMu < muonCandidates.size(); ++iMu){
      if (RecoTrack.DeltaR(muonCandidates[iMu])<=0.2){ //--- Unmatched track-muon
	isMatchedLepton = true;
	isolatedTrack_MatchedLepton.push_back(13);
	break;
	}
    }
    // If not matched to electron or muon, fill vector with track info
    if (!isMatchedLepton) {
      RecoUnmatchedTracksElMu.push_back(RecoTrack);
      isolatedTrack_MatchedLepton.push_back(0);
      RecoUnmatchedTracksIdx.push_back(isolatedTrack_MatchedJet.size());
      isolatedTrack_MatchedJet.push_back(0);
    } 
    else 
      isolatedTrack_MatchedJet.push_back(-1);
  }
  
  m_isolatedUnmatchedTracks = RecoUnmatchedTracksElMu.size();
  
  //--- Match to truth particles (for processes with taus).
  if(isMC){
    xAOD::TruthParticleContainer::const_iterator truthP_itr;
    xAOD::TruthParticleContainer::const_iterator truthP_end;
    xAOD::TruthParticleContainer::const_iterator truthP2_itr;
    xAOD::TruthParticleContainer::const_iterator truthP2_end;
    TLorentzVector v1;
    
    for (unsigned int itrk=0; itrk < RecoUnmatchedTracksElMu.size(); ++itrk){
      bool hasElectron=false;
      bool hasNuEl=false;
      bool hasMuon=false;
      bool hasNuMu=false;
      bool hasTau=false;
      bool hasNuTau=false;
      bool hasBeenMatchedToElMu = false;
      bool hasBeenMatchedToTau = false;
      
      //truth particles loop
      truthP_itr = m_truthP->begin();
      truthP_end = m_truthP->end();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	if ( (*truthP_itr)->absPdgId()==11 ) hasElectron=true;
	if ( (*truthP_itr)->absPdgId()==12 ) hasNuEl=true;
	if ( (*truthP_itr)->absPdgId()==13 ) hasMuon=true;
	if ( (*truthP_itr)->absPdgId()==14 ) hasNuMu=true;
	if ( (*truthP_itr)->absPdgId()==15 ) hasTau=true;
	if ( (*truthP_itr)->absPdgId()==16 ) hasNuTau=true;
      }
      
      truthP_itr = m_truthP->begin();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	
	fillTLV( v1, (*truthP_itr));
	
	if( isStable( (*truthP_itr) ) && RecoUnmatchedTracksElMu.at(itrk).DeltaR(v1) < 0.4){
	  
	  if ( (*truthP_itr)->absPdgId()==11 && hasElectron && !hasMuon ) { //--- Forbid muon decaying to electron. Allow tau->el and other electrons
	    trackUnmatched_truthObject.push_back(11);
	    trackUnmatched_truthObjectPt.push_back( (*truthP_itr)->pt() );
	    trackUnmatched_truthObjectEta.push_back( (*truthP_itr)->eta() );
	    hasBeenMatchedToElMu = true;
	    break;
	  }
	  else if ( (*truthP_itr)->absPdgId()==13 && hasMuon ) { //--- Require matching muon. Allow tau->mu and other muons
	    trackUnmatched_truthObject.push_back(13);
	    trackUnmatched_truthObjectPt.push_back( (*truthP_itr)->pt() );
	    trackUnmatched_truthObjectEta.push_back( (*truthP_itr)->eta() );
	    hasBeenMatchedToElMu = true;
	  }
	}
      }
      
      if (!hasBeenMatchedToElMu){
	
	truthP_itr = m_truthP->begin();
	TLorentzVector v1(0, 0, 0, 0);
	for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	  
	  fillTLV(v1, (*truthP_itr));

	  if( (*truthP_itr)->absPdgId()==15 && RecoUnmatchedTracksElMu.at(itrk).DeltaR(v1) < 0.4){

	    truthP2_itr = truthP_itr;
	    truthP2_end = m_truthP->end();
	    for( ; truthP2_itr != truthP2_end; ++truthP2_itr ) {
	      
	      // if mc_parents->at(index).size()>0 && !hasBeenMatchedToTau){ //FIX_ME
	      // for (unsigned int in=0; in<mc_pt->size(); ++in){
	      // 	  if (mc_parents->at(index).size()>0 && mc_parents->at(index).at(0)==mc_barcode->at(in)){
	      // 	    if (abs(mc_pdgId->at(in))==24){
	      // 	      trackUnmatched_truthObject[iter->first].push_back(15);
	      // 	      trackUnmatched_truthObjectPt[iter->first].push_back(mc_pt->at(p));
	      // 	      trackUnmatched_truthObjectEta[iter->first].push_back(mc_eta->at(p));
	      // 	      hasBeenMatchedToTau = true;
	      // 	      break;
	      // 	    }
	      // 	    index = in;
	      // 	  }
	      //     // 	} 
	      //   }
	      //   else break;
	    }
	  }
	}
	if (!hasBeenMatchedToTau && !hasBeenMatchedToElMu) {
	  trackUnmatched_truthObject.push_back(211); //--- The rest. Allow jets not coming from taus.
	  trackUnmatched_truthObjectPt.push_back(-100.0);
	  trackUnmatched_truthObjectEta.push_back(-100.0);
	}
      	
	//--- dR and dPhi with truth boson
	float dR_min_track_cjet_float = -10;
	float dPhi_min_track_cjet_float = -10;
	
	truthP_itr = m_truthP->begin();
	for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	  
	  TLorentzVector v1(0, 0, 0, 0);
	  v1.SetPtEtaPhiM( (*truthP_itr)->pt(), (*truthP_itr)->eta(), (*truthP_itr)->phi(), (*truthP_itr)->m() );
	  
	  if ( (*truthP_itr)->absPdgId()==23 || (*truthP_itr)->absPdgId()==24) { //--- Z or W boson
	    
	    dR_min_track_truthBoson = RecoUnmatchedTracksElMu.at(itrk).DeltaR(v1);
	    dPhi_min_track_truthBoson = deltaPhi(RecoUnmatchedTracksElMu.at(itrk).Phi(), v1.Phi());
	    
	  }
	  else if ( (*truthP_itr)->absPdgId() == 4){ //--- charm quark
	    if (dR_min_track_cjet_float==-10 || RecoUnmatchedTracksElMu.at(itrk).DeltaR(v1) < dR_min_track_cjet_float){
	      dR_min_track_cjet_float = RecoUnmatchedTracksElMu.at(itrk).DeltaR(v1);
	    }
	    if (dPhi_min_track_cjet_float==-10 || deltaPhi(RecoUnmatchedTracksElMu.at(itrk).Phi(), v1.Phi()) < dPhi_min_track_cjet_float){
	      dPhi_min_track_cjet_float = deltaPhi(RecoUnmatchedTracksElMu.at(itrk).Phi(), v1.Phi());
	    }
	  }
	}
	dR_min_track_truthCjet = dR_min_track_cjet_float;
	dPhi_min_track_truthCjet = dPhi_min_track_cjet_float;
      }
      
    }//end truth loop
  }//if(MC)

  return EL::StatusCode::SUCCESS; 
}

