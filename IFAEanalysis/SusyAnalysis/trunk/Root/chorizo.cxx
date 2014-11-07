#define  chorizo_cxx
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "SampleHandler/MetaFields.h"
#include <SusyAnalysis/chorizo.h>
#include <SusyAnalysis/chorizoEDM.h>
#include <SusyAnalysis/MCUtils.h>

#include <EventLoop/StatusCode.h>

//ST includes (inside chorizoEDM.h now)
//#include "SUSYTools/SUSYObjDef_xAOD.h"

//Jet Cleaning
#include "JetSelectorTools/JetCleaningTool.h"

//GRL
#include "GoodRunsLists/GoodRunsListSelectionTool.h"

//PDF Reweighting
#include "LHAPDF/LHAPDF.h"

//Pileup Reweighting
#include "PileupReweighting/PileupReweightingTool.h"

//TeV unit (w.r.t MeV)
#ifndef TEV
#define TEV 1000000.
#endif

//Dummy values for variables initialization
#define DUMMYUP  99999
#define DUMMYDN -99999

// this is needed to distribute the algorithm to the workers
ClassImp(chorizo)

//for errorcheck
#define APP_NAME "chorizo"

chorizo :: chorizo ()
{
  outputName="";
  Region="";
  defaultRegion="SR";

  xmlReader = 0;
  jOption = "";

  isMC=false;
  isSignal=false;
  isTop=false;
  isTruth=false;
  leptonType="";

  isAtlfast=false;
  isQCD=false;
  isNCBG=false;  doAnaTree=false;
  doPUTree=false;
  doFlowTree=false;

  printMet=false; 
  printJet=false;
  printElectron=false;
  printMuon=false;

  genPUfile = false;

  syst_CP  = CP::SystematicSet();
  syst_CPstr = "";
  syst_ST  = SystErr::NONE;
  syst_PU  = pileupErr::NONE; 
  syst_JVF = JvfUncErr::NONE;
  syst_BCH = BCHCorrMediumErr::NONE;

  systListOnly = false;
  errIgnoreLevel = kInfo;
}



EL::StatusCode chorizo :: setupJob (EL::Job& job)
{
  job.useXAOD ();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init( "chorizo" ).ignore(); // call before opening first file

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
}

void chorizo :: bookTree(){

  

  //add the branches to be saved
  if (output){
    output->tree()->Branch ("RunNumber", &RunNumber, "RunNumber/I");
    output->tree()->Branch ("EventNumber", &EventNumber, "EventNumber/I");
    //    output->tree()->Branch ("mc_channel_number", &mc_channel_number, "EventNumber/I");
    output->tree()->Branch ("averageIntPerXing", &averageIntPerXing, "averageIntPerXing/F");                        
    //common vars
    output->tree()->Branch ("pileup_w", &pileup_w, "pileup_w/F");                  
    output->tree()->Branch ("MC_w", &MC_w, "MC_w/F");                       

    //presel
    output->tree()->Branch ("passPreselectionCuts",&passPreselectionCuts,"passPreselectionCuts/O", 10000);
    
    //FlowTree
    if (doFlowTree){
      output->tree()->Branch("isGRL",&isGRL,"isGRL/O", 10000);
      output->tree()->Branch("isTrigger",&isTrigger,"isTrigger/O", 10000);
      output->tree()->Branch("isVertexOk",&isVertexOk,"isVertexOk/O", 10000);
      output->tree()->Branch("isLarGood",&isLarGood,"isLarGood/O", 10000);
      output->tree()->Branch("isTileGood",&isTileGood,"isTileGood/O", 10000);
      output->tree()->Branch("isTileTrip",&isTileTrip,"isTileTrip/O", 10000);
      output->tree()->Branch("isCoreFlag",&isCoreFlag,"isCoreFlag/O", 10000);
      output->tree()->Branch("isFakeMet",&isFakeMet,"isFakeMet/O", 10000);
      output->tree()->Branch("isMetCleaned",&isMetCleaned,"isMetCleaned/O", 10000);
      output->tree()->Branch("isBadID",&isBadID,"isBadID/O", 10000);
    }

    
    //metadata
    output->tree()->Branch ("xsec", &meta_xsec, "xsec/F");
    output->tree()->Branch ("xsec_relunc", &meta_xsec_relunc, "xsec_relunc/F");
    output->tree()->Branch ("kfactor", &meta_kfactor, "kfactor/F");
    output->tree()->Branch ("feff", &meta_feff, "feff/F");
    output->tree()->Branch ("nsim", &meta_nsim, "nsim/F");
    output->tree()->Branch ("lumi", &meta_lumi, "lumi/F");


    //bch
    output->tree()->Branch("JetsFailBCHTight",&JetsFailBCHTight,"JetsFailBCHTight/O", 10000);
    output->tree()->Branch("JetsFailBCHMedium",&JetsFailBCHMedium,"JetsFailBCHMedium/O", 10000);
    output->tree()->Branch("Jet1FailBCHTight",&Jet1FailBCHTight,"Jet1FailBCHTight/O", 10000);
    output->tree()->Branch("Jet1FailBCHMedium",&Jet1FailBCHMedium,"Jet1FailBCHMedium/O", 10000);
    output->tree()->Branch("Jet2FailBCHTight",&Jet2FailBCHTight,"Jet2FailBCHTight/O", 10000);
    output->tree()->Branch("Jet2FailBCHMedium",&Jet2FailBCHMedium,"Jet2FailBCHMedium/O", 10000);
    output->tree()->Branch("Jet3FailBCHTight",&Jet3FailBCHTight,"Jet3FailBCHTight/O", 10000);
    output->tree()->Branch("Jet3FailBCHMedium",&Jet3FailBCHMedium,"Jet3FailBCHMedium/O", 10000);
    output->tree()->Branch("Jet1_BCH_CORR_CELL",&Jet1_BCH_CORR_CELL,"Jet1_BCH_CORR_CELL/F", 10000);
    output->tree()->Branch("Jet2_BCH_CORR_CELL",&Jet2_BCH_CORR_CELL,"Jet2_BCH_CORR_CELL/F", 10000);
    output->tree()->Branch("Jet3_BCH_CORR_CELL",&Jet3_BCH_CORR_CELL,"Jet3_BCH_CORR_CELL/F", 10000);
 
    if(doAnaTree){ //AnalysisTree
      //weights
      output->tree()->Branch ("w", &w, "w/F");             
      output->tree()->Branch ("w_average", &w_average, "w_average/F");             
      output->tree()->Branch ("PDF_w", &PDF_w, "PDF_w/F");                      
      output->tree()->Branch ("bosonVect_w", &bosonVect_w, "bosonVect_w/F");                   output->tree()->Branch ("Trigger_w", &Trigger_w, "Trigger_w/F");                  
      output->tree()->Branch ("Trigger_w_avg", &Trigger_w_avg, "Trigger_w_avg/F");
      output->tree()->Branch ("e_SF", &e_SF, "e_SF/F");                       
      output->tree()->Branch ("m_SF", &m_SF, "m_SF/F");                       

      //boson 
      output->tree()->Branch ("bos_pt", &bos_pt, "bos_pt/F");                       

      output->tree()->Branch ("bosonVec_reco_pt", &bosonVec_reco_pt, "bosonVec_reco_pt/F");             
      output->tree()->Branch ("bosonVec_reco_eta", &bosonVec_reco_eta, "bosonVec_reco_eta/F");             
      output->tree()->Branch ("bosonVec_reco_phi", &bosonVec_reco_phi, "bosonVec_reco_phi/F");             
      
      output->tree()->Branch ("bosonVec_truth_pt", &bosonVec_truth_pt, "bosonVec_truth_pt/F");             
      output->tree()->Branch ("bosonVec_truth_eta", &bosonVec_truth_eta, "bosonVec_truth_eta/F");             
      output->tree()->Branch ("bosonVec_truth_phi", &bosonVec_truth_phi, "bosonVec_truth_phi/F");             

      //truth
      output->tree()->Branch("truth_M",&truth_M,"truth_M/F", 10000);
      output->tree()->Branch("truth_MT",&truth_MT,"truth_MT/F", 10000);


      output->tree()->Branch("truth_n_HFjets",&truth_n_HFjets,"truth_n_HFjets/I", 10000);

      output->tree()->Branch("truth_n_leptons",&truth_n_leptons,"truth_n_leptons/I", 10000); 
      output->tree()->Branch("truth_Lep1_pt",&truth_Lep1_pt,"truth_Lep1_pt/F", 10000);
      output->tree()->Branch("truth_Lep2_pt",&truth_Lep2_pt,"truth_Lep2_pt/F", 10000);
      output->tree()->Branch("truth_m_fiducial",&truth_m_fiducial,"truth_m_fiducial/B", 10000);
      output->tree()->Branch("truth_shat",&truth_shat,"truth_shat/F", 10000);
      output->tree()->Branch("truth_met",&truth_met,"truth_met/F", 10000);
      output->tree()->Branch("truth_met_noEle",&truth_met_noEle,"truth_met_noEle/F", 10000);

      //top
      output->tree()->Branch("Top_truth_pt",&Top_truth_pt,"Top_truth_pt/F", 10000);
      output->tree()->Branch("Topbar_truth_pt",&Topbar_truth_pt,"Topbar_truth_pt/F", 10000);
      output->tree()->Branch("avTop_truth_pt",&avTop_truth_pt,"avTop_truth_pt/F", 10000);
      output->tree()->Branch("ttbar_weight",&ttbar_weight,"ttbar_weight/F", 10000);

      //photons
      output->tree()->Branch("ph_N",&ph_N,"ph_N/I", 10000);
      output->tree()->Branch("ph_pt",&ph_pt,"ph_pt/F", 10000);
      output->tree()->Branch("ph_eta",&ph_eta,"ph_eta/F", 10000);
      output->tree()->Branch("ph_phi",&ph_phi,"ph_phi/F", 10000);
      output->tree()->Branch("ph_etiso30",&ph_etiso30,"ph_etiso30/F", 10000);
      output->tree()->Branch("ph_ptiso30",&ph_ptiso30,"ph_ptiso30/F", 10000);
      output->tree()->Branch("ph_tight",&ph_tight,"ph_tight/O", 10000);
      output->tree()->Branch("ph_SF",&photonSF,"ph_SF/F", 10000);
      output->tree()->Branch("ph_SFu",&photonSF,"ph_SFu/F", 10000);
      output->tree()->Branch("ph_SFd",&photonSF,"ph_SFd/F", 10000);

      //electrons
      output->tree()->Branch("e_truth_pt",&e_truth_pt,"e_truth_pt/F", 10000);
      output->tree()->Branch("e_truth_eta",&e_truth_eta,"e_truth_eta/F", 10000);
      output->tree()->Branch("e_truth_phi",&e_truth_phi,"e_truth_phi/F", 10000);

      output->tree()->Branch("e_N",&e_N,"e_N/I", 10000);
      output->tree()->Branch("e_pt",&e_pt,"e_pt/F", 10000);
      output->tree()->Branch("e_eta",&e_eta,"e_eta/F", 10000);
      output->tree()->Branch("e_phi",&e_phi,"e_phi/F", 10000);
      output->tree()->Branch("e_etiso30",&e_etiso30,"e_etiso30/F", 10000);
      output->tree()->Branch("e_ptiso30",&e_ptiso30,"e_ptiso30/F", 10000);
      output->tree()->Branch("e_tight",&e_tight,"e_tight/O", 10000);
      output->tree()->Branch("e_MT",&e_MT,"e_MT/F", 10000);
      output->tree()->Branch("e_M",&e_M,"e_M/F", 10000);
      output->tree()->Branch("e_Zpt",&e_Zpt,"e_Zpt/F", 10000);
      
      output->tree()->Branch("e_SF",&electronSF,"e_SF/F", 10000);
      output->tree()->Branch("e_SFu",&electronSF,"e_SFu/F", 10000);
      output->tree()->Branch("e_SFd",&electronSF,"e_SFd/F", 10000);

      //muons
      output->tree()->Branch("m_N",&m_N,"m_N/I", 10000);                            
      output->tree()->Branch("m_pt",&m_pt,"m_pt/F", 10000);                         
      output->tree()->Branch("m_eta",&m_eta,"m_eta/F", 10000);                      
      output->tree()->Branch("m_phi",&m_phi,"m_phi/F", 10000);                      
      output->tree()->Branch("m2_pt",&m2_pt,"m2_pt/F", 10000);                      
      output->tree()->Branch("m2_eta",&m2_eta,"m2_eta/F", 10000);                   
      output->tree()->Branch("m2_phi",&m2_phi,"m2_phi/F", 10000);                   
      output->tree()->Branch("m_iso",&m_iso,"m_iso/F", 10000);                      
      output->tree()->Branch("m_etiso20",&m_etiso20,"m_etiso20/F", 10000);          
      output->tree()->Branch("m_ptiso20",&m_ptiso20,"m_ptiso20/F", 10000);          
      output->tree()->Branch("m_etiso30",&m_etiso30,"m_etiso30/F", 10000);          
      output->tree()->Branch("m_ptiso30",&m_ptiso30,"m_ptiso30/F", 10000);          
      output->tree()->Branch("m2_iso",&m2_iso,"m2_iso/F", 10000);                      
      output->tree()->Branch("m2_etiso30",&m2_etiso30,"m2_etiso30/F", 10000);       
      output->tree()->Branch("m2_ptiso30",&m2_ptiso30,"m2_ptiso30/F", 10000);       
      output->tree()->Branch("m_M",&m_M,"m_M/F", 10000);                            
      output->tree()->Branch("m_MT",&m_MT,"m_MT/F", 10000);                         
      output->tree()->Branch("m_Zpt",&m_Zpt,"m_Zpt/F", 10000);                      
      output->tree()->Branch("m_EM",&m_EM,"m_EM/F", 10000);                         

      //jets
      output->tree()->Branch("JVF_min",&JVF_min,"JVF_min/O", 10000);                           
      output->tree()->Branch("n_jets",&n_jets,"n_jets/I", 10000);
      output->tree()->Branch("n_jets40",&n_jets40,"n_jets40/I", 10000);
      output->tree()->Branch("n_jets50",&n_jets50,"n_jets50/I", 10000);
      output->tree()->Branch("n_jets60",&n_jets60,"n_jets60/I", 10000);
      output->tree()->Branch("n_jets80",&n_jets80,"n_jets80/I", 10000);
      output->tree()->Branch("n_taujets",&n_taujets,"n_taujets/I", 10000);
      output->tree()->Branch("pt1",&pt1,"pt1/F", 10000);
      output->tree()->Branch("pt2",&pt2,"pt2/F", 10000);
      output->tree()->Branch("pt3",&pt3,"pt3/F", 10000);
      output->tree()->Branch("pt4",&pt4,"pt4/F", 10000);
      output->tree()->Branch("pt5",&pt5,"pt5/F", 10000);
      output->tree()->Branch("pt6",&pt6,"pt6/F", 10000);
      output->tree()->Branch("eta1",&eta1,"eta1/F", 10000);
      output->tree()->Branch("eta2",&eta2,"eta2/F", 10000);
      output->tree()->Branch("eta3",&eta3,"eta3/F", 10000);
      output->tree()->Branch("eta4",&eta4,"eta4/F", 10000);
      output->tree()->Branch("eta5",&eta5,"eta5/F", 10000);
      output->tree()->Branch("eta6",&eta6,"eta6/F", 10000);
      output->tree()->Branch("phi1",&phi1,"phi1/F", 10000);
      output->tree()->Branch("phi2",&phi2,"phi2/F", 10000);
      output->tree()->Branch("phi3",&phi3,"phi3/F", 10000);
      output->tree()->Branch("phi4",&phi4,"phi4/F", 10000);
      output->tree()->Branch("phi5",&phi5,"phi5/F", 10000);
      output->tree()->Branch("phi6",&phi6,"phi6/F", 10000);
      output->tree()->Branch("j1_chf",&j1_chf,"j1_chf/F", 10000);
      output->tree()->Branch("j2_chf",&j2_chf,"j2_chf/F", 10000);
      output->tree()->Branch("j3_chf",&j3_chf,"j3_chf/F", 10000);
      output->tree()->Branch("j4_chf",&j4_chf,"j4_chf/F", 10000);
      output->tree()->Branch("j1_emf",&j1_emf,"j1_emf/F", 10000);
      output->tree()->Branch("j2_emf",&j2_emf,"j2_emf/F", 10000);
      output->tree()->Branch("j3_emf",&j3_emf,"j3_emf/F", 10000);
      output->tree()->Branch("j4_emf",&j4_emf,"j4_emf/F", 10000);
      output->tree()->Branch("j1_fsm",&j1_fsm,"j1_fsm/F", 10000);
      output->tree()->Branch("j2_fsm",&j2_fsm,"j2_fsm/F", 10000);
      output->tree()->Branch("j3_fsm",&j3_fsm,"j3_fsm/F", 10000);
      output->tree()->Branch("j4_fsm",&j4_fsm,"j4_fsm/F", 10000);
      output->tree()->Branch("j1_time",&j1_time,"j1_time/F", 10000);
      output->tree()->Branch("j2_time",&j2_time,"j2_time/F", 10000);
      output->tree()->Branch("j3_time",&j3_time,"j3_time/F", 10000);
      output->tree()->Branch("j4_time",&j4_time,"j4_time/F", 10000);
      output->tree()->Branch("j1_mT",&j1_mT,"j1_mT/F", 10000);
      output->tree()->Branch("j2_mT",&j2_mT,"j2_mT/F", 10000);
      output->tree()->Branch("j3_mT",&j3_mT,"j3_mT/F", 10000);
      output->tree()->Branch("j4_mT",&j4_mT,"j4_mT/F", 10000);
      output->tree()->Branch("j1_nTrk",&j1_nTrk,"j1_nTrk/F", 10000);
      output->tree()->Branch("j2_nTrk",&j2_nTrk,"j2_nTrk/F", 10000);
      output->tree()->Branch("j3_nTrk",&j3_nTrk,"j3_nTrk/F", 10000);
      output->tree()->Branch("j4_nTrk",&j4_nTrk,"j4_nTrk/F", 10000);
      output->tree()->Branch("j1_sumPtTrk",&j1_sumPtTrk,"j1_sumPtTrk/F", 10000);
      output->tree()->Branch("j2_sumPtTrk",&j2_sumPtTrk,"j2_sumPtTrk/F", 10000);
      output->tree()->Branch("j3_sumPtTrk",&j3_sumPtTrk,"j3_sumPtTrk/F", 10000);
      output->tree()->Branch("j4_sumPtTrk",&j4_sumPtTrk,"j4_sumPtTrk/F", 10000);
      output->tree()->Branch("j1_jvtxf",&j1_jvtxf,"j1_jvtxf/F", 10000);
      output->tree()->Branch("j2_jvtxf",&j2_jvtxf,"j2_jvtxf/F", 10000);
      output->tree()->Branch("j3_jvtxf",&j3_jvtxf,"j3_jvtxf/F", 10000);
      output->tree()->Branch("j4_jvtxf",&j4_jvtxf,"j4_jvtxf/F", 10000);
      output->tree()->Branch("j1_tflavor",&j1_tflavor,"j1_tflavor/F", 10000);
      output->tree()->Branch("j2_tflavor",&j2_tflavor,"j2_tflavor/F", 10000);
      output->tree()->Branch("j3_tflavor",&j3_tflavor,"j3_tflavor/F", 10000);
      output->tree()->Branch("j4_tflavor",&j4_tflavor,"j4_tflavor/F", 10000);


      //btagging
      output->tree()->Branch("n_bjets",&n_bjets,"n_bjets/I", 10000);
      output->tree()->Branch("n_bjets_80eff",&n_bjets_80eff,"n_bjets_80eff/I", 10000); 
      // output->tree()->Branch("btag_weight1",&btag_weight1,"btag_weight1/F", 10000);
      // output->tree()->Branch("btag_weight2",&btag_weight2,"btag_weight2/F", 10000);
      // output->tree()->Branch("btag_weight3",&btag_weight3,"btag_weight3/F", 10000);
      // output->tree()->Branch("btag_weight4",&btag_weight4,"btag_weight4/F", 10000);
      // output->tree()->Branch("btag_weight_80eff1",&btag_weight_80eff1,"btag_weight_80eff1/F", 10000);
      // output->tree()->Branch("btag_weight_80eff2",&btag_weight_80eff2,"btag_weight_80eff2/F", 10000);
      // output->tree()->Branch("btag_weight_80eff3",&btag_weight_80eff3,"btag_weight_80eff3/F", 10000);
      // output->tree()->Branch("btag_weight_80eff4",&btag_weight_80eff4,"btag_weight_80eff4/F", 10000);
      output->tree()->Branch("btag_weight_total",&btag_weight_total,"btag_weight_total/F", 10000);
      output->tree()->Branch("btag_weight_total_80eff",&btag_weight_total_80eff,"btag_weight_total_80eff/F", 10000);

      output->tree()->Branch("tag_MV1_1",&tag_MV1_1,"tag_MV1_1/F", 10000);
      output->tree()->Branch("tag_MV1_2",&tag_MV1_2,"tag_MV1_2/F", 10000);
      output->tree()->Branch("tag_MV1_3",&tag_MV1_3,"tag_MV1_3/F", 10000);
      output->tree()->Branch("tag_MV1_4",&tag_MV1_4,"tag_MV1_4/F", 10000);
      output->tree()->Branch("tag_SV1_1",&tag_SV1_1,"tag_SV1_1/F", 10000);
      output->tree()->Branch("tag_SV1_2",&tag_SV1_2,"tag_SV1_2/F", 10000);
      output->tree()->Branch("tag_SV1_3",&tag_SV1_3,"tag_SV1_3/F", 10000);
      output->tree()->Branch("tag_SV1_4",&tag_SV1_4,"tag_SV1_4/F", 10000);
      output->tree()->Branch("tag_JetFitterCu_1",&tag_JetFitterCu_1,"tag_JetFitterCu_1/F", 10000);
      output->tree()->Branch("tag_JetFitterCu_2",&tag_JetFitterCu_2,"tag_JetFitterCu_2/F", 10000);
      output->tree()->Branch("tag_JetFitterCu_3",&tag_JetFitterCu_3,"tag_JetFitterCu_3/F", 10000);
      output->tree()->Branch("tag_JetFitterCu_4",&tag_JetFitterCu_4,"tag_JetFitterCu_4/F", 10000);
      output->tree()->Branch("tag_JetFitterCb_1",&tag_JetFitterCb_1,"tag_JetFitterCb_1/F", 10000);
      output->tree()->Branch("tag_JetFitterCb_2",&tag_JetFitterCb_2,"tag_JetFitterCb_2/F", 10000);
      output->tree()->Branch("tag_JetFitterCb_3",&tag_JetFitterCb_3,"tag_JetFitterCb_3/F", 10000);
      output->tree()->Branch("tag_JetFitterCb_4",&tag_JetFitterCb_4,"tag_JetFitterCb_4/F", 10000);
      
      //met 
      output->tree()->Branch("met",&met,"met/F", 10000);
      output->tree()->Branch("met_phi",&met_phi,"met_phi/F", 10000);
      output->tree()->Branch("met_trk", &met_trk, "met_trk/F", 10000);
      output->tree()->Branch("mettrk_phi",&mettrk_phi,"mettrk_phi/F", 10000);    
      output->tree()->Branch("met_mu", &met_mu, "met_mu/F", 10000);
      output->tree()->Branch("met_ecorr", &met_ecorr, "met_ecorr/F", 10000);
      output->tree()->Branch("met_lochadtopo", &met_lochadtopo, "met_lochadtopo/F", 10000);
      output->tree()->Branch("met_reffinalNoMu", &met_reffinalNoMu, "met_reffinalNoMu/F", 10000);

      //event variables
      //dphi
      output->tree()->Branch("dPhi_met_j1",&dPhi_met_j1,"dPhi_met_j1/f", 10000);
      output->tree()->Branch("dPhi_met_j2",&dPhi_met_j2,"dPhi_met_j2/f", 10000);
      output->tree()->Branch("dPhi_met_j3",&dPhi_met_j3,"dPhi_met_j3/f", 10000);
      output->tree()->Branch("dPhi_met_j4",&dPhi_met_j4,"dPhi_met_j4/f", 10000);
      output->tree()->Branch("dPhi_mettrk_j1",&dPhi_mettrk_j1,"dPhi_mettrk_j1/f", 10000);
      output->tree()->Branch("dPhi_mettrk_j2",&dPhi_mettrk_j2,"dPhi_mettrk_j2/f", 10000);
      output->tree()->Branch("dPhi_mettrk_j3",&dPhi_mettrk_j3,"dPhi_mettrk_j3/f", 10000);
      output->tree()->Branch("dPhi_mettrk_j4",&dPhi_mettrk_j4,"dPhi_mettrk_j4/f", 10000);
      output->tree()->Branch("dPhi_met_mettrk",&dPhi_met_mettrk,"dPhi_met_mettrk/f", 10000);
      output->tree()->Branch("dPhi_min",&dPhi_min,"dPhi_min/f", 10000);
      output->tree()->Branch("dPhi_min_alljets",&dPhi_min_alljets,"dPhi_min_alljets/f", 10000);

      output->tree()->Branch("dPhi_j1_j2",&dPhi_j1_j2,"dPhi_j1_j2/f", 10000);
      output->tree()->Branch("dPhi_j1_j3",&dPhi_j1_j3,"dPhi_j1_j3/f", 10000);
      output->tree()->Branch("dPhi_j2_j3",&dPhi_j2_j3,"dPhi_j2_j3/f", 10000);
      output->tree()->Branch("dPhi_b1_b2",&dPhi_b1_b2,"dPhi_b1_b2/f", 10000);

      //dR
      output->tree()->Branch("dR_j1_j2",&dR_j1_j2,"dR_j1_j2/f", 10000);
      output->tree()->Branch("dR_j1_j3",&dR_j1_j3,"dR_j1_j3/f", 10000);
      output->tree()->Branch("dR_j2_j3",&dR_j2_j3,"dR_j2_j3/f", 10000);

      output->tree()->Branch("dR_j1_m1",&dR_j1_m1,"dR_j1_m1/f", 10000);
      output->tree()->Branch("dR_j1_m2",&dR_j1_m2,"dR_j1_m2/f", 10000);
      output->tree()->Branch("dR_j2_m1",&dR_j2_m1,"dR_j2_m1/f", 10000);
      output->tree()->Branch("dR_j2_m2",&dR_j2_m2,"dR_j2_m2/f", 10000);
      output->tree()->Branch("dR_j3_m1",&dR_j3_m1,"dR_j3_m1/f", 10000);
      output->tree()->Branch("dR_j3_m2",&dR_j3_m2,"dR_j3_m2/f", 10000);

      //dEta
      output->tree()->Branch("dEta_j1_j2",&dEta_j1_j2,"dEta_j1_j2/f", 10000);

      //MTs
      output->tree()->Branch("MT_min_jet_met",&MT_min_jet_met,"MT_min_jet_met/F", 10000);      output->tree()->Branch("MT_bcl_met",&MT_bcl_met,"MT_bcl_met/F", 10000);        
      output->tree()->Branch("MT_bfar_met",&MT_bfar_met,"MT_bfar_met/F", 10000);        
      output->tree()->Branch("MT_lcl_met",&MT_lcl_met,"MT_lcl_met/F", 10000);        
      output->tree()->Branch("MT_jsoft_met",&MT_jsoft_met,"MT_jsoft_met/F", 10000);   

      //Misc
      output->tree()->Branch("DiJet_Mass",&DiJet_Mass,"DiJet_Mass/F", 10000);  
      output->tree()->Branch("DiBJet_Mass",&DiBJet_Mass,"DiBJet_Mass/F", 10000);         
      
      output->tree()->Branch("mct",&mct,"mct/F", 10000);    
      output->tree()->Branch("meff",&meff,"meff/F", 10000);    
      output->tree()->Branch("HT",&HT,"HT/F", 10000);    
      output->tree()->Branch("AlphaT",&AlphaT,"AlphaT/F", 10000);
      
      //Razor
      output->tree()->Branch("MR",&MR,"MR/F", 10000);
      output->tree()->Branch("MTR",&MTR,"MTR/F", 10000);
      output->tree()->Branch("R",&R,"R/F", 10000); 
            
      output->tree()->Branch("shatR",&shatR,"shatR/F", 10000);
      output->tree()->Branch("gaminvR",&gaminvR,"gaminvR/F", 10000);
      output->tree()->Branch("mdeltaR",&mdeltaR,"mdeltaR/F", 10000);
      output->tree()->Branch("cosptR",&cosptR,"cosptR/F", 10000);
      
      //top reconstruction
      output->tree()->Branch("MtTop",&MtTop,"MtTop/F", 10000);     
      output->tree()->Branch("m_top_had1",&m_top_had1,"m_top_had1/F", 10000);     
      output->tree()->Branch("m_top_had2",&m_top_had2,"m_top_had2/F", 10000);      
      //fat jets 
      output->tree()->Branch("m0_antikt12",&m0_antikt12,"m0_antikt12/F", 10000);  
      output->tree()->Branch("m1_antikt12",&m1_antikt12,"m1_antikt12/F", 10000);  
      output->tree()->Branch("m0_antikt08",&m0_antikt08,"m0_antikt08/F", 10000); 
      output->tree()->Branch("m1_antikt08",&m1_antikt08,"m1_antikt08/F", 10000);  
      output->tree()->Branch("pt0_antikt12",&pt0_antikt12,"pt0_antikt12/F", 10000);
      output->tree()->Branch("pt1_antikt12",&pt1_antikt12,"pt1_antikt12/F", 10000);
      output->tree()->Branch("pt0_antikt08",&pt0_antikt08,"pt0_antikt08/F", 10000);
      output->tree()->Branch("pt1_antikt08",&pt1_antikt08,"pt1_antikt08/F", 10000);
      output->tree()->Branch("mtasym12",&mtasym12,"mtasym12/F", 10000);        
      output->tree()->Branch("mtasym08",&mtasym08,"mtasym08/F", 10000);        
      

    }
    else if(doPUTree){
    
    }
  }

}

EL::StatusCode chorizo :: histInitialize ()
{
  gErrorIgnoreLevel = this->errIgnoreLevel;

  if (!outputName.empty()){
    output = EL::getNTupleSvc (wk(), outputName);
  } 
  else{
    output = 0;
  }

  //Book the output Tree
  bookTree();

  //JopOption
  meta_jOption= new TNamed("jOption", jOption.c_str());
  //meta_jOption= new TNamed("jOption", "METbb");
  wk()->addOutput(meta_jOption);     

  //Histos
  //---  right errors in histos with weights
  TH1F::SetDefaultSumw2();
  TH2F::SetDefaultSumw2();
  TProfile::SetDefaultSumw2();

  //need template to do TH1D as well...
  h_average = new TH1D("beginning","beggining",1,0,1);
  wk()->addOutput(h_average);     

  //histograms
  InitGHist(h_cut_var, "h_cut_var", 2000, 0, 1000., "Cut Var (GeV)", "");  
  //  InitGHist(h_presel_flow, "h_presel_flow", 10, 0., 10., "", "");
  //  InitGHist(h_presel_wflow, "h_presel_wflow", 10, 0., 10., "", "");

  // const char *cutNames[] = {"GRL","Trigger","PVertex","LarGood","TileGood","CoreFlag","BadJet","FakeMET","MET cleaning","TileTrip"};

  // for (int i=1; i<h_presel_flow->GetNbinsX(); ++i) {
  //   h_presel_flow->GetXaxis()->SetBinLabel(i,cutNames[i-1]);
  //   h_presel_wflow->GetXaxis()->SetBinLabel(i,cutNames[i-1]);
  // }

  //Systematics (override CP set if string is given) [tmp hack]
  if(!syst_CPstr.IsNull()){
    syst_CP  = CP::SystematicSet(); //needed?
    if(syst_CPstr.Contains("continuous")){
      if(syst_CPstr.Contains("1up"))
	syst_CP.insert( CP::SystematicVariation(std::string(syst_CPstr.Copy().ReplaceAll("__continuous__1up","").Data()), 1));
      else if(syst_CPstr.Contains("1down"))
	syst_CP.insert( CP::SystematicVariation(std::string(syst_CPstr.Copy().ReplaceAll("__continuous__1down","").Data()), -1));
      else{
	Error("histInitialize()", "Cannot configure a continuous systematics like this : %s . Please attach e.g. __1up to it so I know what to do! ", syst_CPstr.Data());
	return EL::StatusCode::FAILURE;
      }
    }
  }

  return EL::StatusCode::SUCCESS;
}

void chorizo :: InitVars()
{
  //Initialize ntuple variables

  //- MetaData
  meta_xsec = 1.;
  meta_xsec_relunc = 0.;
  meta_kfactor = 1.;
  meta_feff = 1.;
  meta_nsim = 0.; 
  meta_lumi = 1.;
  
  //- Event info
  RunNumber = 0;
  EventNumber = 0;
  averageIntPerXing = 0;

  //- Data Quality (event is good by default! (i.e. MC))
  isGRL = true; 
  isFakeMet = false;                 
  isBadID = false;                   
  isMetCleaned = true; //CHECK_ME
  isTrigger = true;                 
  isVertexOk = true;                
  isLarGood = true;                 
  isTileGood = true;                
  isTileTrip = false;                
  isCoreFlag = true;          
      
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
  bosonVec_truth_pt = DUMMYDN;           
  bosonVec_truth_eta = DUMMYDN;          
  bosonVec_truth_phi = DUMMYDN;          
                                       
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

  //- Event selection
  nVertex = 0;

  //- Electron Info
  e_N = 0; //DUMMYDN;               
  e_pt = 0; //DUMMYDN;              
  e_truth_pt = DUMMYDN;        
  e_truth_eta = DUMMYDN;       
  e_truth_phi = DUMMYDN;       
  e_eta = 0.; //DUMMYDN;             
  e_phi = 0.; //DUMMYDN;             
  e_ptiso30 = 0.; //DUMMYDN;         
  e_etiso30 = 0.; //DUMMYDN;         
  e_tight = false;           
  e_M = DUMMYDN;               
  e_Zpt = DUMMYDN;             
  electronSF = DUMMYDN;        
  electronSFu = 1.;
  electronSFd = 1.;

  //- Photon Info
  ph_N = 0; //DUMMYDN;               
  ph_pt = 0; //DUMMYDN;              
  ph_eta = 0.; //DUMMYDN;             
  ph_phi = 0.; //DUMMYDN;             
  ph_ptiso30 = 0.; //DUMMYDN;         
  ph_etiso30 = 0.; //DUMMYDN;         
  ph_tight = false;           
  photonSF = DUMMYDN;        
  photonSFu = 1.;
  photonSFd = 1.;

  //- Muon info
  m_N = 0.; //DUMMYDN;                  
  m_pt = 0.; //DUMMYDN;               
  m_eta = 0.; //DUMMYDN;              
  m_phi = 0.; //DUMMYDN;              
  m2_pt = 0.; //DUMMYDN;              
  m2_eta = 0.; //DUMMYDN;             
  m2_phi = 0.; //DUMMYDN;             
  m_iso = DUMMYDN;              
  m_ptiso30 = 0.; //DUMMYDN;          
  m_etiso30 = 0.; //DUMMYDN;          
  m_ptiso20 = 0.; //DUMMYDN;          
  m_etiso20 = 0.; //DUMMYDN;          
  m2_iso = DUMMYDN;             
  m2_ptiso30 = 0.; //DUMMYDN;         
  m2_etiso30 = 0.; //DUMMYDN;         
  muonSF = DUMMYDN;             
  muonSFu = 1.;
  muonSFd = 1.;
  m_M = DUMMYDN;                
  m_MT = DUMMYDN;               
  m_Zpt = DUMMYDN;              
  m_EM = DUMMYDN;               

  //muons (before overlap removal)      
  muon_N = DUMMYDN;                 
  muon_pt.clear();                
  muon_eta.clear();               
  muon_phi.clear();               
  muon_iso.clear();               
  muon_etiso30.clear();           
  muon_ptiso30.clear();           
  muon_truth.clear();             
  muon_jet_dR.clear();            
  muon_jet_dPhi.clear();          
  muon_jet_pt.clear();            
  muon_jet_eta.clear();           
  muon_jet_phi.clear();           
  muon_jet_nTrk.clear();          
  muon_jet_sumPtTrk.clear();      
  muon_jet_chf.clear();           
  muon_jet_emf.clear();           
  muon_jet_mv1.clear();           
  muon_jet_vtxf.clear();        

  //- Jet Info
  JVF_min=false;
  n_jets=0;
  n_jets40=0;
  n_jets50=0;
  n_jets60=0;
  n_jets80=0;
  n_taujets=0;
  pt1 = DUMMYDN;              
  pt2 = DUMMYDN;              
  pt3 = DUMMYDN;              
  pt4 = DUMMYDN;              
  pt5 = DUMMYDN;              
  pt6 = DUMMYDN;              
  eta1 = DUMMYDN;             
  eta2 = DUMMYDN;             
  eta3 = DUMMYDN;             
  eta4 = DUMMYDN;             
  eta5 = DUMMYDN;             
  eta6 = DUMMYDN;             
  phi1 = DUMMYDN;             
  phi2 = DUMMYDN;             
  phi3 = DUMMYDN;             
  phi4 = DUMMYDN;             
  phi5 = DUMMYDN;             
  phi6 = DUMMYDN;             
  j1_chf = DUMMYDN;           
  j2_chf = DUMMYDN;           
  j3_chf = DUMMYDN;           
  j4_chf = DUMMYDN;           
  j1_emf = DUMMYDN;           
  j2_emf = DUMMYDN;           
  j3_emf = DUMMYDN;           
  j4_emf = DUMMYDN;           
  j1_fsm = DUMMYDN;           
  j2_fsm = DUMMYDN;           
  j3_fsm = DUMMYDN;           
  j4_fsm = DUMMYDN;           
  j1_time = DUMMYDN;          
  j2_time = DUMMYDN;          
  j3_time = DUMMYDN;          
  j4_time = DUMMYDN;          
  j1_mT = DUMMYDN;            
  j2_mT = DUMMYDN;            
  j3_mT = DUMMYDN;            
  j4_mT = DUMMYDN;            
  j1_nTrk = DUMMYDN;                               
  j2_nTrk = DUMMYDN;                               
  j3_nTrk = DUMMYDN;                               
  j4_nTrk = DUMMYDN;                               
  j1_sumPtTrk = DUMMYDN;                           
  j2_sumPtTrk = DUMMYDN;                           
  j3_sumPtTrk = DUMMYDN;                           
  j4_sumPtTrk = DUMMYDN;                           
  j1_jvtxf = DUMMYDN;                              
  j2_jvtxf = DUMMYDN;                              
  j3_jvtxf = DUMMYDN;                              
  j4_jvtxf = DUMMYDN;                              
  j1_tflavor = DUMMYDN;                              
  j2_tflavor = DUMMYDN;                              
  j3_tflavor = DUMMYDN;                              
  j4_tflavor = DUMMYDN;                              
                                               
  //- Btagging                                 
  n_bjets = 0;                               
  n_bjets_80eff = DUMMYDN;                     
  btag_weight_total = 1.;
  btag_weight_total_80eff = 1.;
  // btag_weight1 = DUMMYDN;                          
  // btag_weight2 = DUMMYDN;                          
  // btag_weight3 = DUMMYDN;                          
  // btag_weight4 = DUMMYDN;                          
  // btag_weight_80eff1 = DUMMYDN;                    
  // btag_weight_80eff2 = DUMMYDN;                    
  // btag_weight_80eff3 = DUMMYDN;                    
  // btag_weight_80eff4 = DUMMYDN;                    

  // btag_weight_first.clear();
  // btag_weight_first_80eff.clear();
  // btag_weight.clear();
  // btag_weight_80eff.clear();
  
  // btag_weight_B_down.clear();
  // btag_weight_B_down_80eff.clear();
  // btag_weight_B_up.clear();
  // btag_weight_B_up_80eff.clear();
  // btag_weight_C_down.clear();
  // btag_weight_C_down_80eff.clear();
  // btag_weight_C_up.clear();
  // btag_weight_C_up_80eff.clear();
  // btag_weight_L_down.clear();
  // btag_weight_L_down_80eff.clear();
  // btag_weight_L_up.clear();
  // btag_weight_L_up_80eff.clear();

  tag_MV1_1 = DUMMYDN;
  tag_MV1_2 = DUMMYDN;
  tag_MV1_3 = DUMMYDN;
  tag_MV1_4 = DUMMYDN;
  tag_SV1_1 = DUMMYDN;
  tag_SV1_2 = DUMMYDN;
  tag_SV1_3 = DUMMYDN;
  tag_SV1_4 = DUMMYDN;
  tag_JetFitterCu_1 = DUMMYDN;
  tag_JetFitterCu_2 = DUMMYDN;
  tag_JetFitterCu_3 = DUMMYDN;
  tag_JetFitterCu_4 = DUMMYDN;
  tag_JetFitterCb_1 = DUMMYDN;
  tag_JetFitterCb_2 = DUMMYDN;
  tag_JetFitterCb_3 = DUMMYDN;
  tag_JetFitterCb_4 = DUMMYDN;
  
  //BCH cleaning
  JetsFailBCHTight = false;        
  JetsFailBCHMedium = false;       
  Jet1FailBCHTight = false;        
  Jet1FailBCHMedium = false;       
  Jet2FailBCHTight = false;        
  Jet2FailBCHMedium = false;       
  Jet3FailBCHTight = false;        
  Jet3FailBCHMedium = false;       
  Jet1_BCH_CORR_CELL = DUMMYDN;     
  Jet2_BCH_CORR_CELL = DUMMYDN;     
  Jet3_BCH_CORR_CELL = DUMMYDN;     

  //met 
  met = DUMMYDN;
  met_phi = DUMMYDN;
  met_trk = DUMMYDN;
  mettrk_phi = DUMMYDN;
  met_mu = DUMMYDN;
  met_ecorr = DUMMYDN;
  met_lochadtopo = DUMMYDN;
  met_reffinalNoMu = DUMMYDN;

  //event variables 
  dPhi_met_j1 = DUMMYDN;                         
  dPhi_met_j2 = DUMMYDN;                         
  dPhi_met_j3 = DUMMYDN;                         
  dPhi_met_j4 = DUMMYDN;                         
  dPhi_mettrk_j1 = DUMMYDN;                      
  dPhi_mettrk_j2 = DUMMYDN;                      
  dPhi_mettrk_j3 = DUMMYDN;                      
  dPhi_mettrk_j4 = DUMMYDN;                      
  dPhi_met_mettrk = DUMMYDN;                     
  dPhi_j1_j2 = DUMMYDN;                          
  dPhi_j1_j3 = DUMMYDN;                          
  dPhi_j2_j3 = DUMMYDN;                          
  dPhi_b1_b2 = DUMMYDN;                          
  dPhi_min = DUMMYDN;                            
  dPhi_min_alljets = DUMMYDN;                    
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

  MT_min_jet_met = DUMMYDN;  
  MT_bcl_met = DUMMYDN;  
  MT_bfar_met = DUMMYDN;  
  MT_lcl_met = DUMMYDN;  
  MT_jsoft_met = DUMMYDN;  

  DiJet_Mass=0;
  DiBJet_Mass=0;

  mct=0.;
  meff=0.;
  HT=0.;

  AlphaT=DUMMYDN;

  //Razor
  MR=DUMMYDN;
  MTR=DUMMYDN;
  R=DUMMYDN;
  shatR=DUMMYDN;
  gaminvR=DUMMYDN;
  mdeltaR=DUMMYDN;
  cosptR=DUMMYDN;
    
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
  recoElectrons.clear();
  truthElectrons.clear();
  recoMuons.clear();
  recoJets.clear();
  seedJets.clear();

}


EL::StatusCode chorizo :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode chorizo :: changeInput (bool firstFile)
{
  return EL::StatusCode::SUCCESS;
}

void chorizo :: ReadXML(){

  const char* whereAmI = "chorizo::ReadXML()";
  Info(whereAmI, "--- Read XML options ---");
  //read XML options
  //  DirectoryPath = gSystem->Getenv("ANALYSISCODE");
  std::string maindir = getenv("ROOTCOREBIN");
  const char* cRegion = Region.c_str();
  const char* defRegion = defaultRegion.c_str();

  xmlReader = new XMLReader();
  xmlReader->readXML(maindir+"/data/SusyAnalysis/"+jOption);

  //------ Define and read global variables from the XML
  Info(whereAmI, Form(" - PDF reweighting") );
  doPDFrw = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$PdfRw$Enable");
  beamE_to = xmlReader->retrieveFloat("AnalysisOptions$GeneralSettings$PdfRw$ToBeamE");

  Info(whereAmI, Form(" - GRL") );
  GRLxmlFile = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/GRLxmlFile").c_str());

  Info(whereAmI, Form(" - Pileup") );
  applyPURW = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$Pileup$Mode/name/ApplyPileupReweighting");
  PURW_Folder = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Pileup$Path/name/PileupReweightingConfigFolder").c_str());
  PURW_IlumicalcFile = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Pileup$Path/name/PileupReweightingIlumicalcFile").c_str());
  PeriodConfig = TString(xmlReader->retrieveChar("AnalysisOptions$GeneralSettings$Pileup$PeriodConfig").c_str());

  Info(whereAmI, Form(" - LeptonEff Unitarity") );
  leptonEfficiencyUnitarity = xmlReader->retrieveBool("AnalysisOptions$GeneralSettings$LeptonEfficiency$Unitarity");

  Info(whereAmI, Form(" - Trigger") );
  std::string triggerNameStr="";
  try{
    triggerNameStr = (xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Trigger$region/name/%s", cRegion))).c_str();
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    triggerNameStr = (xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Trigger$region/name/%s", defRegion))).c_str();
  }
  std::istringstream triggerNameIStr(triggerNameStr);
  std::string s;
  while (std::getline(triggerNameIStr, s, ',')) {
    TriggerNames.push_back(s);
  }

  Info(whereAmI, Form(" - Overlap Removal") );
  doOR = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$OverlapRemoval$Enable");
  
  Info(whereAmI, Form(" - TrackVeto" ));
  tVeto_Enable = xmlReader->retrieveBool("AnalysisOptions$ObjectDefinition$TrackVeto$Enable");
  try{
    tVeto_Pt               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$Pt", cRegion));
    tVeto_Eta              = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$Eta", cRegion));
    tVeto_d0               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$d0", cRegion));
    tVeto_z0               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$z0", cRegion));
    tVeto_ndof             = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$ndof", cRegion));
    tVeto_chi2OverNdof_min = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$chi2OverNdof_min", cRegion));
    tVeto_chi2OverNdof_max = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$chi2OverNdof_max", cRegion));
    PixHitsAndSCTHits      = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$PixHitsAndSCTHits", cRegion));
    tVeto_TrackIso         = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$TrackIso", cRegion));
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    tVeto_Pt               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$Pt", defRegion));
    tVeto_Eta              = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$Eta", defRegion));
    tVeto_d0               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$d0", defRegion));
    tVeto_z0               = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$z0", defRegion));
    tVeto_ndof             = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$ndof", defRegion));
    tVeto_chi2OverNdof_min = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$chi2OverNdof_min", defRegion));
    tVeto_chi2OverNdof_max = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$chi2OverNdof_max", defRegion));
    PixHitsAndSCTHits      = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$PixHitsAndSCTHits", defRegion));
    tVeto_TrackIso         = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$TrackVeto$region/name/%s$TrackIso", defRegion));
  }


  Info(whereAmI, Form(" - Vertex") );
  try{
    nTracks = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Vertex$region/name/%s", cRegion));
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    nTracks = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Vertex$region/name/%s", defRegion));
  }

  Info(whereAmI, Form(" - Electrons") );
  try{
    El_PreselPtCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$PreselPtCut", cRegion));
    El_PreselEtaCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$PreselEtaCut", cRegion));
    El_RecoPtCut     = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$RecoPtCut", cRegion));
    El_RecoEtaCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$RecoEtaCut", cRegion));
    
    El_recoSF         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$recoSF", cRegion));
    El_idSF           = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$idSF", cRegion));
    El_triggerSF      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$triggerSF", cRegion));
    El_isoType = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$isoType", cRegion)).c_str());
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    El_PreselPtCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$PreselPtCut", defRegion));
    El_PreselEtaCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$PreselEtaCut", defRegion));
    El_RecoPtCut     = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$RecoPtCut", defRegion));
    El_RecoEtaCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$RecoEtaCut", defRegion));
    
    El_recoSF         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$recoSF", defRegion));
    El_idSF           = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$idSF", defRegion));
    El_triggerSF      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$triggerSF", defRegion));
    El_isoType = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Electron$region/name/%s$isoType", defRegion)).c_str());
  }

  Info(whereAmI, Form(" - Muons") );
  try{
    Mu_PreselPtCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$PreselPtCut", cRegion));
    Mu_PreselEtaCut = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$PreselEtaCut", cRegion));
    Mu_RecoPtCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$RecoPtCut", cRegion));
    Mu_RecoEtaCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$RecoEtaCut", cRegion));
    Mu_isoType = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$isoType", cRegion)).c_str());    
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    Mu_PreselPtCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$PreselPtCut", defRegion));
    Mu_PreselEtaCut = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$PreselEtaCut", defRegion));
    Mu_RecoPtCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$RecoPtCut", defRegion));
    Mu_RecoEtaCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$RecoEtaCut", defRegion));
    Mu_isoType = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Muon$region/name/%s$isoType", defRegion)).c_str());        
  }
  
  Info(whereAmI, Form(" - Jets") );
  try{
    JetCollection=TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Collection", cRegion)).c_str());
    Jet_BtagEnv    = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Path/name/BtagEnv", cRegion)).c_str());
    Jet_BtagCalib  = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Path/name/BtagCalib", cRegion)).c_str());
    Jet_Tagger     = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Tagger", cRegion)).c_str());
    Jet_TaggerOp   = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerOpPoint", cRegion)).c_str());
    Jet_TaggerOp2   = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerOpPoint2", cRegion)).c_str());      
    Jet_Tagger_Collection = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerCollection", cRegion)).c_str());
    
    Jet_PreselPtCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$PreselPtCut", cRegion));
    Jet_PreselEtaCut = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$PreselEtaCut", cRegion));
    Jet_RecoPtCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoPtCut", cRegion));
    Jet_RecoEtaCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoEtaCut", cRegion));
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    JetCollection=TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Collection", defRegion)).c_str());
    Jet_BtagEnv    = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Path/name/BtagEnv", defRegion)).c_str());
    Jet_BtagCalib  = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Path/name/BtagCalib", defRegion)).c_str());
    Jet_Tagger     = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$Tagger", defRegion)).c_str());
    Jet_TaggerOp   = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerOpPoint", defRegion)).c_str());
    Jet_TaggerOp2   = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerOpPoint2", defRegion)).c_str());      
    Jet_Tagger_Collection   = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$TaggerCollection", defRegion)).c_str());
    Jet_PreselPtCut  = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$PreselPtCut", defRegion));
    Jet_PreselEtaCut = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$PreselEtaCut", defRegion));
    Jet_RecoPtCut    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoPtCut", defRegion));
    Jet_RecoEtaCut   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoEtaCut", defRegion));
  }
  
  Info(whereAmI, Form(" - Etmiss") );
  try{
    METCollection        = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Collection",cRegion)).c_str());
    Met_FakeMetEstimator = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Path/name/FakeMetEstimator", cRegion)).c_str());
    Met_doFakeEtmiss     = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$doFakeEtmiss", cRegion));
    Met_doMetCleaning    = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$doMetCleaning", cRegion));
    Met_doRefEle         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefEle", cRegion));
    Met_doRefGamma       = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefGamma", cRegion));
    Met_doRefTau         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefTau", cRegion));
    Met_doRefJet         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefJet", cRegion));
    Met_doMuons          = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doMuons", cRegion));
    Met_doSoftTerms      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doSoftTerms", cRegion));
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion));
    METCollection        = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Collection",defRegion)).c_str());
    Met_FakeMetEstimator = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Path/name/FakeMetEstimator", defRegion)).c_str());
    Met_doFakeEtmiss     = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$doFakeEtmiss", defRegion));
    Met_doMetCleaning    = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$doMetCleaning", defRegion));
    Met_doRefEle         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefEle", defRegion));
    Met_doRefGamma       = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefGamma", defRegion));
    Met_doRefTau         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefTau", defRegion));
    Met_doRefJet         = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doRefJet", defRegion));
    Met_doMuons          = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doMuons", defRegion));
    Met_doSoftTerms      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$Etmiss$region/name/%s$Term/name/doSoftTerms", defRegion));
  }

  Info(whereAmI, Form("- QCD") );
  try{
    QCD_JetsPtPreselection = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$JetsPtPreselection", cRegion));
    QCD_JetsPtSelection    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoPtCut", cRegion));
    QCD_JetsEtaSelection   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoEtaCut", cRegion));
    QCD_METSig             = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$METSigCut", cRegion));
    QCD_LeadJetPreSel      = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$LeadJetPreSel", cRegion)).c_str());
    QCD_RandomSeedOffset   = xmlReader->retrieveInt(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$RandomSeedOffset", cRegion));
    QCD_SmearType          = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearType", cRegion)).c_str());
    QCD_SmearUseBweight    = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearUseBweight", cRegion));
    QCD_SmearBtagWeight    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearBtagWeight", cRegion));
    QCD_SmearMeanShift     = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearMeanShift", cRegion)).c_str());
    QCD_SmearExtraSmr      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearExtraSmr", cRegion));
    QCD_DoPhiSmearing      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$DoPhiSmearing", cRegion));
    QCD_SmearedEvents      = xmlReader->retrieveInt(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearedEvents", cRegion));
  }
  catch(...){
    Warning(whereAmI, Form("%s region not found. Getting the default region %s.", cRegion, defRegion)); 
    QCD_JetsPtPreselection = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$JetsPtPreselection", defRegion));
    QCD_JetsPtSelection    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoPtCut", defRegion));
    QCD_JetsEtaSelection   = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$Jet$region/name/%s$RecoEtaCut", defRegion));
    QCD_METSig             = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$METSigCut", defRegion));
    QCD_LeadJetPreSel      = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$LeadJetPreSel", defRegion)).c_str());
    QCD_RandomSeedOffset   = xmlReader->retrieveInt(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$RandomSeedOffset", defRegion));
    QCD_SmearType          = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearType", defRegion)).c_str());
    QCD_SmearUseBweight    = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearUseBweight", defRegion));
    QCD_SmearBtagWeight    = xmlReader->retrieveFloat(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearBtagWeight", defRegion));
    QCD_SmearMeanShift     = TString(xmlReader->retrieveChar(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearMeanShift", defRegion)).c_str());
    QCD_SmearExtraSmr      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearExtraSmr", defRegion));
    QCD_DoPhiSmearing      = xmlReader->retrieveBool(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$DoPhiSmearing", defRegion));
    QCD_SmearedEvents      = xmlReader->retrieveInt(Form("AnalysisOptions$ObjectDefinition$QCD$region/name/%s$SmearedEvents", defRegion));
  }
  
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


  //--- Systematics
  
  Info(whereAmI, Form("--------------------------------------------\n\n") );
}

EL::StatusCode chorizo :: initialize ()
{

  //Start Clock
  watch.Start();

  m_event = wk()->xaodEvent();

  // Create a transient object store. Needed for the tools.
  // Like m_event but does not assume new objects are written to file
  m_store = new xAOD::TStore(); 

  //Read XML options
  ReadXML();


  //initialize lepton isolation 
  elIsoArgs = new ST::IsSignalElectronExpCutArgs();
  muIsoArgs = new ST::IsSignalMuonExpCutArgs();

  El_isoType.ToLower();
  if(El_isoType.EqualTo("loose"))       elIsoType = ST::SignalIsoExp::LooseIso;
  else if(El_isoType.EqualTo("medium")) elIsoType = ST::SignalIsoExp::MediumIso;
  else if(El_isoType.EqualTo("tight"))  elIsoType = ST::SignalIsoExp::TightIso;
  else{
    elIsoType = ST::SignalIsoExp::LooseIso;
    elIsoArgs->_id_isocut = -1; //do not apply it 
  }

  Mu_isoType.ToLower();
  if(Mu_isoType.EqualTo("loose"))       muIsoType = ST::SignalIsoExp::LooseIso;
  else if(Mu_isoType.EqualTo("medium")) muIsoType = ST::SignalIsoExp::MediumIso;
  else if(Mu_isoType.EqualTo("tight"))  muIsoType = ST::SignalIsoExp::TightIso;
  else{
    muIsoType = ST::SignalIsoExp::LooseIso;
    muIsoArgs->_id_isocut = -1; //do not apply it 
  }

  //Initialize variables
  InitVars();

  // get event info needed for tool's config
  const xAOD::EventInfo* eventInfo = 0;
  if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    Error("initialize()", "Failed to retrieve event info collection. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );

  //*** Initialize all CP tools
  std::string maindir = getenv("ROOTCOREBIN");
  maindir = maindir + "/data/";

  //--- SUSYTools
  tool_st = new ST::SUSYObjDef_xAOD( "SUSYObjDef_xAOD" );
  tool_st->SUSYToolsInit().ignore();
  tool_st->setProperty("IsData",    (int)!this->isMC);
  tool_st->setProperty("IsAtlfast", (int)this->isAtlfast);   
  if(!Met_doMuons)
    tool_st->setProperty("METMuonTerm", ""); //No MuonTerm default
  tool_st->initialize();
  tool_st->msg().setLevel( MSG::ERROR ); //set message level 

  if(syst_CP.size()){

    // Tell the SUSYObjDef_xAOD which variation to apply
    if (tool_st->applySystematicVariation( syst_CP ) != CP::SystematicCode::Ok){
      Error("initialize()", "Cannot configure SUSYTools for systematic var. %s", (syst_CP.name()).c_str() );
    }else{
      Info("initialize()", "Variation \"%s\" configured...", (syst_CP.name()).c_str() );
    }
  }

  //    --- For the BCH correction
  tool_DPeriod = DataPeriod();
  tool_DPeriod.initialize();

  //--- MET
  //    --- Used in the SmartVeto function.
  tool_fakeMet = new FakeMetEstimator(TString(maindir + Met_FakeMetEstimator).Data());

  //--- B-tagging
  tool_btag  = new BTaggingEfficiencyTool("BTag70");
  tool_btag->setProperty("TaggerName",          Jet_Tagger.Data());
  tool_btag->setProperty("OperatingPoint",      Jet_TaggerOp.Data());
  tool_btag->setProperty("JetAuthor",           Jet_Tagger_Collection.Data()); 
  tool_btag->setProperty("ScaleFactorFileName",maindir+"SUSYTools/2014-Winter-8TeV-MC12-CDI.root");
  tool_btag->initialize();

  tool_btag2  = new BTaggingEfficiencyTool("BTag80");
  tool_btag2->setProperty("TaggerName",          Jet_Tagger.Data());
  tool_btag2->setProperty("OperatingPoint",      Jet_TaggerOp2.Data());
  tool_btag2->setProperty("JetAuthor",           Jet_Tagger_Collection.Data()); 
  tool_btag2->setProperty("ScaleFactorFileName",maindir+"SUSYTools/2014-Winter-8TeV-MC12-CDI.root");
  tool_btag2->initialize();

  //--- Pileup Reweighting
  // trick: path in the tool name so it gets saved to the desired place
  TString purw_name = Form("myPURWtool.%s/%d", PURW_Folder.Data(), (int)wk()->metaData()->getDouble( "DSID" ));
  tool_purw = new CP::PileupReweightingTool(purw_name.Data());
  tool_purw->setProperty("Input","EventInfo");
  if (this->isMC){
    if(genPUfile && !doPUTree){ //--- Generate the pileup root files
      //      tool_purw->setProperty("UsePeriodConfig", PeriodConfig); //allow for auto-detect for now
      tool_purw->initialize();
    }
    else if(applyPURW || doPUTree){ //--- Apply the weights found after generating the pileup root files
      std::vector<std::string> prwFiles;	 
      std::vector<std::string> lumiFiles;	 
      
      //--- read the dataset number from the TTree
      Info("initialize()", Form("Reading PileupReweighting file : %i.prw.root",  eventInfo->mcChannelNumber()) );

      TString prwfile=PURW_Folder+Form("%i",  eventInfo->mcChannelNumber())+".prw.root";
      prwFiles.push_back(prwfile.Data());
      tool_purw->setProperty("ConfigFiles",prwFiles);

      tool_purw->setProperty("DataScaleFactor", 1./1.09);
      if (this->syst_PU == pileupErr::PileupLow)        
	tool_purw->setProperty("DataScaleFactor", 1./1.05);
      else if(this->syst_PU == pileupErr::PileupHigh) 
	tool_purw->setProperty("DataScaleFactor", 1./1.13);
      
      lumiFiles.push_back(PURW_IlumicalcFile.Data());      
      tool_purw->setProperty("LumiCalcFiles", lumiFiles);

      tool_purw->setProperty("UnrepresentedDataAction",2);

      tool_purw->initialize();
    }
  }

  //--- GRL
  tool_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> grlist;
  grlist.push_back((maindir + GRLxmlFile).Data());
  tool_grl->setProperty( "GoodRunsListVec", grlist);
  tool_grl->setProperty("PassThrough", false);
  if (!tool_grl->initialize().isSuccess()) {
    Error("initialize()", "Failed to properly initialize the GRL. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  //--- TileTripReader
  tool_tileTrip = new Root::TTileTripReader("TileTripReader");
  std::string TileTripReader_file = "CompleteTripList_2011-2012.root";
  tool_tileTrip->setTripFile((maindir + "TileTripReader/" + TileTripReader_file).data());

  //--- BCH cleaning
  tool_BCH = new BCHTool::BCHCleaningToolRoot();
  tool_BCH->InitializeTool(!isMC, tool_tileTrip, maindir+"BCHCleaningTool/FractionsRejectedJetsMC.root");

  //--- JVF uncertainty
  tool_jvf = new JVFUncertaintyTool();
  tool_jvf->UseGeV(true);

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
  tool_jClean->setProperty( "CutLevel", "MediumBad");
  tool_jClean->initialize();

  //--- Random number generator for truth-->reco efficiency                  
  //  myRand = new TRandom1(time(0));
                                                                                              
  //--- number of events
  Info("initialize()", Form("Total number of events = %lli", m_event->getEntries() ));
  watch.Stop();
  Info("initialize()", Form("Init time = %f", watch.CpuTime()));
  watch.Start();

  return EL::StatusCode::SUCCESS;
}

void chorizo :: printSystList(){
  //Warning: It has to happen *after* all tools' initialization!

  const CP::SystematicRegistry& registry = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recommendedSystematics = registry.recommendedSystematics();

  std::cout << bold("\n\n\n List of recommended/implemented systematics") << std::endl;
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

  // this is the nominal set
  for(CP::SystematicSet::const_iterator sysItr = recommendedSystematics.begin(); sysItr != recommendedSystematics.end(); ++sysItr){
    //   std::cout << sysItr->basename();
    std::cout << sysItr->name();
    // if (*sysItr == CP::SystematicVariation (sysItr->basename(), CP::SystematicVariation::CONTINUOUS)){
    //   std::cout << "\t +-" ;
    // }
    std::cout << std::endl;
  }
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
}

void chorizo :: loadMetaData(){

  if(!isMC) return;

  meta_xsec        = wk()->metaData()->getDouble( SH::MetaFields::crossSection );
  meta_xsec_relunc = wk()->metaData()->getDouble( SH::MetaFields::crossSectionRelUncertainty );
  meta_kfactor     = wk()->metaData()->getDouble( SH::MetaFields::kfactor );
  meta_feff        = wk()->metaData()->getDouble( SH::MetaFields::filterEfficiency );
  meta_nsim        = wk()->metaData()->getDouble( SH::MetaFields::numEvents );
  meta_lumi        = wk()->metaData()->getDouble( SH::MetaFields::lumi );
  //  meta_id = (int)wk()->metaData()->getDouble( "DSID" ));
}

EL::StatusCode chorizo :: nextEvent(){
  //*** clean event (store, pointers, etc...) before leaving successfully... ***
  
  // Clear View container
  m_goodJets->clear();

  // Clear transient store
  m_store->clear();

  return EL::StatusCode::SUCCESS;   
}


EL::StatusCode chorizo :: execute ()
{
  return loop();
}

EL::StatusCode chorizo :: loop ()
{
  //Info("loop()", "Inside loop");

  if(systListOnly){  //just print systematics list and leave!
    this->printSystList();
    wk()->skipEvent();
  }

  InitVars();

  if ( m_eventCounter%50==0 ){ //put back to 2000 or so!
    printf("\r %6d/%lld\t[%3.0f]",m_eventCounter,m_event->getEntries(),100*m_eventCounter/float(m_event->getEntries()));
    fflush(stdout);
  }

  m_eventCounter++;

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    Error("loop()", "Failed to retrieve 'EventInfo' collection. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  loadMetaData();
  //  isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ); //already set

  //-- Retrieve objects Containers
  m_truthE= 0;
  m_truthP= 0;
  m_jets= 0;
  m_truth_jets= 0;
  m_electrons= 0;
  m_muons= 0;
  
  //  -- Jets
  CHECK( m_event->retrieve( m_jets, "AntiKt4LCTopoJets" ) );
  // if ( !m_event->retrieve( m_jets, "AntiKt4LCTopoJets" ).isSuccess() ){ 
  //   Error("loop()", "Failed to retrieve 'AntiKt4LCTopoJets' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }
  //  -- Electrons
  if ( !m_event->retrieve( m_electrons, "ElectronCollection" ).isSuccess() ){ 
    Error("loop()", "Failed to retrieve 'ElectronCollection' container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  //  -- Muons
  if ( !m_event->retrieve( m_muons, "Muons" ).isSuccess() ){ 
    Error("loop()", "Failed to retrieve 'Muons' container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  //  -- Truth Particles
  if(isMC){
    if ( !m_event->retrieve( m_truthE, "TruthEvent" ).isSuccess() ){ 
      Error("loop()", "Failed to retrieve 'TruthEvent' container. Exiting." );
      return EL::StatusCode::FAILURE;
    }
    if ( !m_event->retrieve( m_truthP, "TruthParticle" ).isSuccess() ){ 
      Error("loop()", "Failed to retrieve 'TruthParticle' container. Exiting." );
      return EL::StatusCode::FAILURE;
    }
    if( ! m_event->retrieve( m_truth_jets, "AntiKt4TruthJets").isSuccess() ){
      Error("loop()", "Failed to retrieve Truth jets collection : AntiKt4TruthJets. Exiting." );
      return EL::StatusCode::FAILURE;
    }
  }
  xAOD::TruthParticleContainer::const_iterator truthP_itr;
  xAOD::TruthParticleContainer::const_iterator truthP_end;

  // View container provides access to selected jets   (for MET recalculation)
  m_goodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
  CHECK( m_store->record( m_goodJets, "MySelJets" ) );

  xAOD::MissingETContainer* metRFC = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* metRFCAux = new xAOD::MissingETAuxContainer;
  metRFC->setStore(metRFCAux);

  CHECK( m_store->record( metRFC, "MET_MyRefFinal" ) );
  CHECK( m_store->record( metRFCAux, "MET_MyRefFinalAux." ) );


  //--- Analysis Code 
  //--- 
  RunNumber = eventInfo->runNumber();
  mc_channel_number = (isMC ? eventInfo->mcChannelNumber() : 0); 
  EventNumber = eventInfo->eventNumber();

  int lb = eventInfo->lumiBlock();
  averageIntPerXing = eventInfo->averageInteractionsPerCrossing();

  int bch_runN = -999; //for BCH correction
  int bch_lb   = -999; //for BCH correction
  if(isMC){  
    bch_runN = tool_DPeriod.getRandomRunNumber(RunNumber, averageIntPerXing, 0);
    if(bch_runN>0)
      bch_lb   = tool_DPeriod.getRandomLumiBlockNumber(RunNumber);
    // Info("execute()", Form("BCH random Generated run = %d", bch_runN));
    // Info("execute()", Form("BCH random Generated lb  = %d", bch_lb));
  }
  else{
    bch_runN = RunNumber;
    bch_lb   = lb;       
  }


  //PURW
  if(isMC)
    tool_purw->apply(eventInfo);  //it does already the filling in 'ConfigMode'

  //--- Generate Pileup file??
  if (genPUfile && isMC){
    //** xAODs should be unaffected by this bug!! https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
    //    averageIntPerXing = (isMC && lb==1 && int(averageIntPerXing+0.5)==1) ? 0. : averageIntPerXing; //--- lb doesn't exist in slimmed ntuples! //CHECK_ME 

    if (RunNumber==0){
      Info("execute()", Form("Skipping event %d because RunNumber=0 !!", EventNumber));
      return nextEvent();
    }
    
    if (!doPUTree) {        
      //   tool_purw->Fill(RunNumber, mc_channel_number, eventInfo->mcEventWeight(), averageIntPerXing); //the tool is not configured to handle the new MC yet. Use MCb RunNumber instead (should be the same for now at 8TeV).
      return nextEvent();
    }
    else{
      pileup_w = eventInfo->auxdata<double>("PileupWeight");

      output->setFilterPassed ();
      return nextEvent();
    }
  }
  
  //--- Weights for MC
  if (isMC){

    //--- Weigths for Sherpa, MC@NLO, Acer, ...
    MC_w = eventInfo->mcEventWeight();
    
    //--- PDF reweighting
    xAOD::TruthEventContainer::const_iterator truthE_itr = m_truthE->begin();
    int id1, id2, pdfId1, pdfId2; //, pdf1, pdf2;
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
      // ( *truthE_itr )->pdfInfoParameter(pdf1, xAOD::TruthEvent::PDF1);
      // ( *truthE_itr )->pdfInfoParameter(pdf2, xAOD::TruthEvent::PDF2);
      ( *truthE_itr )->pdfInfoParameter(scalePDF, xAOD::TruthEvent::SCALE);
      // ( *truthE_itr )->pdfInfoParameter(pdf_Q, xAOD::TruthEvent::Q);
      // ( *truthE_itr )->pdfInfoParameter(pdf_xf1, xAOD::TruthEvent::XF1);
      // ( *truthE_itr )->pdfInfoParameter(pdf_xf2, xAOD::TruthEvent::XF2);
      
      //** For the old EDM
      // ( *truthE_itr )->pdfInfoParameter(id1, xAOD::TruthEvent::id1);
      // ( *truthE_itr )->pdfInfoParameter(id2, xAOD::TruthEvent::id2);
      // ( *truthE_itr )->pdfInfoParameter(x1, xAOD::TruthEvent::x1);
      // ( *truthE_itr )->pdfInfoParameter(x2, xAOD::TruthEvent::x2);
      // ( *truthE_itr )->pdfInfoParameter(pdfId1, xAOD::TruthEvent::pdfId1);
      // ( *truthE_itr )->pdfInfoParameter(pdfId2, xAOD::TruthEvent::pdfId2);
      // // ( *truthE_itr )->pdfInfoParameter(pdf1, xAOD::TruthEvent::pdf1);
      // // ( *truthE_itr )->pdfInfoParameter(pdf2, xAOD::TruthEvent::pdf2);
      // ( *truthE_itr )->pdfInfoParameter(scalePDF, xAOD::TruthEvent::scalePDF);
      
      PDF_w *= getPdfRW((double)beamE_to/beamE_from, (double)(scalePDF*scalePDF), (double)x1, (double)x2, id1, id2);     
      
      // cout << "-------------------------------------" << endl;
      // cout << "DEBUG :: rwScale = " << beamE_to/beamE_from << endl; 
      // cout << "DEBUG :: scalePDF = " << scalePDF << endl; 
      // cout << "DEBUG :: x1 = " << x1 << endl; 
      // cout << "DEBUG :: x2 = " << x2 << endl; 
      // cout << "DEBUG :: id1 = " << id1 << endl; 
      // cout << "DEBUG :: id2 = " << id2 << endl; 
      // cout << "DEBUG :: pdfId1 = " << pdfId1 << endl; 
      // cout << "DEBUG :: pdfId2 = " << pdfId2 << endl; 
      // cout << "DEBUG :: PDF_w = " << PDF_w << endl; 
      // cout << "-------------------------------------" << endl;
    }
  
    //--- Get sum of the weigths from the slimmed samples //FIX_ME
    this->w_average = this->GetAverageWeight();
    this->w = 1/w_average;

    //---pileup weight 
    if(applyPURW)
      pileup_w = tool_purw->GetCombinedWeight(RunNumber, mc_channel_number, averageIntPerXing);

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
    if (mc_channel_number==117050) {
      ttbar_weight = this->GetTTbarReweight(Top_truth_pt, Topbar_truth_pt, avTop_truth_pt);
      truth_n_leptons = this->GetNTruthLeptons();
    }     
    
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
  const xAOD::VertexContainer* vertices = 0;
  if( ! m_event->retrieve( vertices, "PrimaryVertices").isSuccess() ){
    Error("loop()", "Failed to retrieve PrimaryVertices container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  nVertex = static_cast< int >( vertices->size() );

  // if (!doFlowTree && nVertex==0)
  //   return EL::StatusCode::SUCCESS; //skip event

  this->isVertexOk = (nVertex>0);

  //--- Trigger 
  //
  // ... NOT YET
  this->isTrigger = true;
  //
  //--- Do a pre-selection for QCD
    // if (this->isQCD){
   
    //   // GRL
    //   if (goodrunslist.HasRunLumiBlock(RunNumber, lb)!=1) continue;
	
    //   //Trigger
    //   if (!(EF_j460_a4tchad ||  
    // 	    EF_j360_a4tchad || 
    // 	    EF_j280_a4tchad || 
    // 	    EF_j220_a4tchad || 
    // 	    EF_j180_a4tchad || 
    // 	    EF_j145_a4tchad || 
    // 	    EF_j110_a4tchad || 
    // 	    EF_j80_a4tchad  || 
    // 	    EF_j55_a4tchad  )) continue;
    //   //EF_j55_a4tchad  || 
    //   //EF_j45_a4tchad  || 
    //   //EF_j35_a4tchad  )) continue;
	      
    // }
    

  //--- GRL
  if( !this->isMC )
    this->isGRL = tool_grl->passRunLB(RunNumber, lb);

  //smearing data pre-selection (QCD-mode) to save some time
  if( this->isQCD && !this->isGRL )
    return nextEvent(); //EL::StatusCode::SUCCESS; //skip event

  //--- Quality cuts applied only on data
  if (!this->isMC) {
    this->isLarGood = (eventInfo->eventFlags(xAOD::EventInfo::LAr)!=2);
    this->isTileGood = (eventInfo->eventFlags(xAOD::EventInfo::Tile)!=2);
    this->isTileTrip = !tool_tileTrip->checkEvent(RunNumber,  lb,  EventNumber); //--- Does not depend on the definition of the objects.
    this->isCoreFlag = (eventInfo->eventFlags(xAOD::EventInfo::Core)==0);
  }

  //fill cutflow
  // if(this->isGRL){
  //   h_presel_flow->Fill(0.5);
  //   if(this->isTrigger){
  //     h_presel_flow->Fill(1.5);
  //     if(this->isVertexOk){
  // 	h_presel_flow->Fill(2.5);
  // 	if(this->isLarGood){
  // 	  h_presel_flow->Fill(3.5);
  // 	  if(this->isTileGood){
  // 	    h_presel_flow->Fill(4.5);
  // 	    if(this->isCoreFlag){
  // 	      h_presel_flow->Fill(5.5);
  // 	      if(!this->isBadID){
  // 		h_presel_flow->Fill(6.5);
  // 		if(!this->isFakeMet){
  // 		  h_presel_flow->Fill(7.5);
  // 		  if(this->isMetCleaned){
  // 		    h_presel_flow->Fill(8.5);
  // 		    if(!this->isTileTrip){
  // 		      h_presel_flow->Fill(9.5);
  // 		    }
  // 		  }
  // 		}
  // 	      }
  // 	    }
  // 	  }
  // 	}
  //     }
  //   }
  // }
  
  this->passPreselectionCuts = this->isGRL && this->isTrigger && this->isVertexOk && this->isLarGood && this->isTileGood && this->isCoreFlag && !this->isBadID && !this->isFakeMet && this->isMetCleaned && !this->isTileTrip;
  
  //skip event no-preselected events for smearing                                       
  if ( this->isQCD  && (!this->passPreselectionCuts) ) 
    return nextEvent();
  

  //--- Non-collision background selection
  if ( this->isNCBG ){
    if ( ! isBeamHalo(RunNumber,EventNumber) ){
      return nextEvent(); // select only events spotted by the beam halo tool
    }
    Info("loop()", Form("NCBG run %d\t%d", RunNumber, EventNumber));
  }

  //--- Get Electrons
  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > electrons_sc = xAOD::shallowCopyContainer( *m_electrons );
  bool setLinks = xAOD::setOriginalObjectLink(*m_electrons, *electrons_sc.first);

  xAOD::ElectronContainer::iterator el_itr = (electrons_sc.first)->begin();
  xAOD::ElectronContainer::iterator el_end = (electrons_sc.first)->end();
  for( ; el_itr != el_end; ++el_itr ) {
    
    tool_st->FillElectron( (**el_itr), El_PreselPtCut, El_PreselEtaCut );

    //decorate electron with final pt requirements ('final')
    elIsoArgs->_etcut = El_RecoPtCut;
    tool_st->IsSignalElectronExp( (**el_itr), elIsoType, *elIsoArgs);
    (**el_itr).auxdata< char >("final") = (**el_itr).auxdata< char >("signal");

    //decorate electron with baseline pt requirements ('signal')
    elIsoArgs->_etcut = El_PreselPtCut;
    tool_st->IsSignalElectronExp( (**el_itr), elIsoType, *elIsoArgs);
  
  }

  //--- Get Muons
  std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > muons_sc = xAOD::shallowCopyContainer( *m_muons );
  setLinks = xAOD::setOriginalObjectLink(*m_muons, *muons_sc.first);

  xAOD::MuonContainer::iterator mu_itr = (muons_sc.first)->begin();
  xAOD::MuonContainer::iterator mu_end = (muons_sc.first)->end();
  for( ; mu_itr != mu_end; ++mu_itr ) {
    
    tool_st->FillMuon( **mu_itr, Mu_PreselPtCut, Mu_PreselEtaCut);      //'baseline' decoration

    //decorate muon with final pt requirements ('final')
    muIsoArgs->_ptcut = Mu_RecoPtCut;
    tool_st->IsSignalMuonExp( **mu_itr, muIsoType, *muIsoArgs);  //'signal' decoration.
    (**mu_itr).auxdata< char >("final") = (**mu_itr).auxdata< char >("signal");

    //decorate muon with final pt requirements ('final')
    muIsoArgs->_ptcut = Mu_PreselPtCut;
    tool_st->IsSignalMuonExp( **mu_itr, muIsoType, *muIsoArgs);  //'signal' decoration.

    tool_st->IsCosmicMuon( **mu_itr );  //'cosmic'   decoration
  }

  //--- Get Jets
  std::vector<Particles::Jet> jetCandidates; //intermediate selection jets
  
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > jets_sc = xAOD::shallowCopyContainer( *m_jets );
  setLinks = xAOD::setOriginalObjectLink(*m_jets, *jets_sc.first);

  xAOD::JetContainer::iterator jet_itr = (jets_sc.first)->begin();
  xAOD::JetContainer::iterator jet_end = (jets_sc.first)->end();
  

  //jets for met recalculation in qcd events //CHECK_ME couldn't I just read the 'CalibratedAntiKt4LCTopoJets' now?
  smr_met_jets_pt.clear();
  smr_met_jets_eta.clear();
  smr_met_jets_phi.clear();
  smr_met_jets_E.clear();
  
  xAOD::Jet jet;
  for( ; jet_itr != jet_end; ++jet_itr ) {
    
    tool_st->FillJet( **jet_itr );
    tool_st->IsGoodJet( **jet_itr, Jet_PreselPtCut, Jet_PreselEtaCut );
    tool_st->IsBJet( **jet_itr ); //CHECK ME! replace decoration with our own?

    //book it for smearing (before overlap removal) //CHECK (DOING NOTHING FOR NOW!!
    smr_met_jets_pt.push_back( 0. ); //recoJet.Pt() ); //in GeV!
    smr_met_jets_eta.push_back( 0. ); //recoJet.Eta() );
    smr_met_jets_phi.push_back( 0. ); //recoJet.Phi() );
    smr_met_jets_E.push_back( 0. ); //recoJet.E() );  //in GeV!
  }

  
  //--- Do overlap removal   
  if(doOR)
    CHECK( tool_st->OverlapRemoval(electrons_sc.first, muons_sc.first, jets_sc.first, false) );
    //CHECK( tool_st->OverlapRemoval(electrons_sc.first, muons_sc.first, jets_sc.first) );

  //-- Pre-book baseline electrons (after OR)
  std::vector<Particle> electronCandidates; //intermediate selection electrons
  bool IsElectron = false; // any good not-overlapping electron in the event?
  int iEl = 0;
  e_N=0; //signal electrons
  el_itr = (electrons_sc.first)->begin();
  el_end = (electrons_sc.first)->end();
  for( ; el_itr != el_end; ++el_itr ) {

    if( (**el_itr).auxdata< char >("baseline") &&
	((!doOR) || (**el_itr).auxdata< char >("passOR")) ){ 
      
      //define preselected electron                
      Particle recoElectron;
      recoElectron.SetVector( getTLV( &(**el_itr) ));
      if (recoElectron.Pt() < El_PreselPtCut/1000.)   continue;
      if (fabs(recoElectron.Eta()) > El_PreselEtaCut) continue;

      recoElectron.id = iEl;
      recoElectron.ptcone20 = (**el_itr).auxdata<float>("ptcone20")/1000.;
      recoElectron.etcone20 = (**el_itr).auxdata<float>("etcone20")/1000.;
      recoElectron.ptcone30 = (**el_itr).auxdata<float>("ptcone30")/1000.;
      recoElectron.etcone30 = (**el_itr).auxdata<float>("etcone30")/1000.;
      (**el_itr).passSelection(recoElectron.isTight, "Tight");

      recoElectron.type   = xAOD::EgammaHelpers::getParticleTruthType( *el_itr );
      recoElectron.origin = xAOD::EgammaHelpers::getParticleTruthOrigin( *el_itr );

      //get electron scale factors
      if(this->isMC){
	//nominal
	recoElectron.SF = tool_st->GetSignalElecSF( **el_itr, El_recoSF, El_idSF, El_triggerSF );

	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!
	if (tool_st->applySystematicVariation( CP::SystematicSet("ELECSFSYS__1up")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	  Error("loop()", "Cannot configure SUSYTools for systematic var. ELECSFSYS__1up");
	}
	recoElectron.SFu = tool_st->GetSignalElecSF( **el_itr, El_recoSF, El_idSF, El_triggerSF ); 

	//+1 sys down
	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!
	if (tool_st->applySystematicVariation( CP::SystematicSet("ELECSFSYS__1down")) != CP::SystematicCode::Ok){ //FIX_ME // ok yes, this systematic doesn't exist yet
	  Error("loop()", "Cannot configure SUSYTools for systematic var. ELECSFSYS__1down");
	}
	recoElectron.SFd = tool_st->GetSignalElecSF( **el_itr );

	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!

      }

      electronCandidates.push_back(recoElectron);

      //save signal electrons
      if( (**el_itr).auxdata< char >("final") ){
	recoElectrons.push_back(recoElectron);
	e_N++;
	IsElectron=true;
	if(this->isMC){
	  electronSF  *= recoElectron.SF;
	  electronSFu *= recoElectron.SFu;
	  electronSFd *= recoElectron.SFd;
	}
      }

      iEl++;
    }//if baseline 
  }//electron loop

  //sort the electrons in Pt
  if (electronCandidates.size()>0) std::sort(electronCandidates.begin(), electronCandidates.end());
  if (recoElectrons.size()>0) std::sort(recoElectrons.begin(), recoElectrons.end());


  //-- Pre-book baseline muons (after OR)
  std::vector<Particle> muonCandidates; //intermediate selection muons
  bool IsMuon = false; // any good not-overlapping muon in the event?
  int iMu=0;
  mu_itr = (muons_sc.first)->begin();
  mu_end = (muons_sc.first)->end();  
  for( ; mu_itr != mu_end; ++mu_itr ) {
   
    if( (**mu_itr).auxdata< char >("signal")==1 &&
	((!doOR) || (**mu_itr).auxdata< char >("passOR")) &&
	(**mu_itr).auxdata< char >("cosmic")==0 ){
    
      Particle recoMuon;
      recoMuon.SetVector( getTLV( &(**mu_itr) ));

      recoMuon.id = iMu;
      recoMuon.ptcone20 = (**mu_itr).auxdata<float>("ptcone20")*0.001;
      recoMuon.etcone20 = (**mu_itr).auxdata<float>("etcone20")*0.001;
      recoMuon.ptcone30 = (**mu_itr).auxdata<float>("ptcone30")*0.001;
      recoMuon.etcone30 = (**mu_itr).auxdata<float>("etcone30")*0.001;
      recoMuon.charge   = (float) (**mu_itr).charge();
      //(float)input.primaryTrackParticle()->charge()  in SUSYTools.  //same thing!
      
      recoMuon.type   = xAOD::EgammaHelpers::getParticleTruthType( *mu_itr );
      recoMuon.origin = xAOD::EgammaHelpers::getParticleTruthOrigin( *mu_itr );

      //get muon scale factors
      if(this->isMC){
	//nominal 
	recoMuon.SF = tool_st->GetSignalMuonSF(**mu_itr);

	//+1 sys up
	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!
	if (tool_st->applySystematicVariation( CP::SystematicSet("MUONSFSYS__1up")) != CP::SystematicCode::Ok){
	  Error("loop()", "Cannot configure SUSYTools for systematic var. MUONSFSYS__1up");
	}
	recoMuon.SFu = tool_st->GetSignalMuonSF(**mu_itr);

	//+1 sys down
	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!
	if (tool_st->applySystematicVariation( CP::SystematicSet("MUONSFSYS__1down")) != CP::SystematicCode::Ok){
	  Error("loop()", "Cannot configure SUSYTools for systematic var. MUONSFSYS__1down");
	}
	recoMuon.SFd = tool_st->GetSignalMuonSF(**mu_itr);

	tool_st->applySystematicVariation(this->syst_CP); //reset back to requested systematic!
      }

      muonCandidates.push_back(recoMuon);

      //save signal muons
      if( (**mu_itr).auxdata< char >("final") ){
	recoMuons.push_back(recoMuon);
	m_N++;
	IsMuon = true;
	if(this->isMC){
	  muonSF  *= recoMuon.SF;
	  muonSFu *= recoMuon.SFu;
	  muonSFd *= recoMuon.SFd;
	}
      }
      
    }//if baseline 
    
    iMu++;
  }//muon loop
  
  //sort the muon candidates in Pt
  if (muonCandidates.size()>0) std::sort(muonCandidates.begin(), muonCandidates.end());
  if (recoMuons.size()>0) std::sort(recoMuons.begin(), recoMuons.end());


  //-- pre-book good jets now (after OR)
  jet_itr = (jets_sc.first)->begin();
  jet_end = (jets_sc.first)->end();
  Particles::Jet recoJet;
  int iJet = 0;
  for( ; jet_itr != jet_end; ++jet_itr ){
    
    if( (*jet_itr)->auxdata< char >("baseline")!=1 ) continue;
    if( doOR && (*jet_itr)->auxdata< char >("passOR")!=1 ) continue;

    //flag event if bad jet is found
    this->isBadID |= (*jet_itr)->auxdata< char >("bad");

    if( (*jet_itr)->auxdata< char >("bad")==1 ) continue; //just book good jets!

    m_goodJets->push_back (*jet_itr);

    
    recoJet.SetVector( getTLV( &(**jet_itr) ) );
    recoJet.id = iJet;
    iJet++;
    
    int local_truth_flavor=0;         //for bjets ID
    if ( this->isMC )
      (*jet_itr)->getAttribute(xAOD::JetAttribute::JetLabel, local_truth_flavor); //CHECK_ME
    //      (*jet_itr)->getAttribute(xAOD::JetAttribute::TruthLabelID, local_truth_flavor); 
    
    
    //--- Get some other jet attributes
    std::vector<float> sumpttrk_vec;                                                   
    (*jet_itr)->getAttribute(xAOD::JetAttribute::SumPtTrkPt500, sumpttrk_vec); //SumPtTrkPt1000? //CHECK_ME                                                                     
    if (sumpttrk_vec.size()>0)                                                    
      recoJet.sumPtTrk = sumpttrk_vec[0]*0.001; // assume the 0th vertex is the primary one (in GeV)
    else                                                                          
      recoJet.sumPtTrk = 0;                                                           
    
    recoJet.chf = recoJet.sumPtTrk/recoJet.Pt(); 
    
    std::vector<int> ntrk_vec;
    (*jet_itr)->getAttribute(xAOD::JetAttribute::NumTrkPt500, ntrk_vec); //NumTrkPt1000? CHECK_ME
    if (ntrk_vec.size()>0)                                                    
      recoJet.nTrk = ntrk_vec[0]; // assume the 0th vertex is the primary one (in GeV)
    else                                                                          
      recoJet.nTrk = 0;                                                              
    
    (*jet_itr)->getAttribute(xAOD::JetAttribute::EMFrac, recoJet.emf);
    (*jet_itr)->getAttribute(xAOD::JetAttribute::Timing, recoJet.time);
    (*jet_itr)->getAttribute(xAOD::JetAttribute::FracSamplingMax, recoJet.fsm);
    
    //JVF
    float jvfsum = 0;
    std::vector<float> v_jvf = (*jet_itr)->getAttribute< std::vector<float> >(xAOD::JetAttribute::JVF);
    for(unsigned int ii=0; ii < v_jvf.size()-1; ++ii){
      jvfsum += v_jvf[ii];
    }
    //    recoJet.jvtxf = jvfsum;
    
    recoJet.jvtxf = (v_jvf.size() ? v_jvf[0] : 0.);
    
    //--- BCH correction
    (*jet_itr)->getAttribute(xAOD::JetAttribute::BchCorrCell, recoJet.BCH_CORR_CELL); //CHECK_ME
    (*jet_itr)->getAttribute(xAOD::JetAttribute::BchCorrJet, recoJet.BCH_CORR_JET);
    
    recoJet.failBCHTight = false;
    recoJet.failBCHMedium = false;
    
    if (bch_runN == 0){ //CHECK_ME
      recoJet.failBCHTight = true;
      recoJet.failBCHMedium = true;
    }
    else{ //--> getRandomRunNumber has worked
      if ( tool_BCH->IsBadTightBCH(bch_runN, bch_lb, recoJet.Eta(), recoJet.Phi(), recoJet.BCH_CORR_CELL, recoJet.emf, recoJet.Pt() )) {
	recoJet.failBCHTight = true;
      }
      int BCHUncNumber = 0;
      if (syst_BCH == BCHCorrMediumErr::BCHCorrHigh) BCHUncNumber=1;
      if (syst_BCH == BCHCorrMediumErr::BCHCorrLow)  BCHUncNumber=-1;
      if ( tool_BCH->IsBadMediumBCH(bch_runN, bch_lb, recoJet.Eta(), recoJet.Phi(), recoJet.BCH_CORR_CELL, recoJet.emf, recoJet.Pt(), BCHUncNumber)) {
	recoJet.failBCHMedium = true;
      }
    }
    
    //--- B-tagging
    
    //from SUSYTools (based on SV1plusIP3D (70%) at the moment!)
    recoJet.isbjet = (bool)(*jet_itr)->auxdata< char >("bjet");

    const xAOD::BTagging* btag =(*jet_itr)->btagging();

    recoJet.MV1 = btag->MV1_discriminant();
    recoJet.SV1plusIP3D = btag->SV1plusIP3D_discriminant();

    recoJet.IP3D_pb = btag->IP3D_pb(); 
    recoJet.IP3D_pc = btag->IP3D_pc(); 
    recoJet.IP3D_pu = btag->IP3D_pu(); 

    recoJet.SV1_pb = btag->SV1_pb(); 
    recoJet.SV1_pc = btag->SV1_pc(); 
    recoJet.SV1_pu = btag->SV1_pu(); 

    recoJet.JetFitterCombNN  = 0;// btag->JetFitterCombNN_pb(); //not saved yet! //FIX_ME
    recoJet.JetFitterCombNNc = 9; //btag->JetFitterCombNN_pc(); //CHECK_ME //not saved yet! //FIX_ME
    //log(jet_flavor_component_jfitcomb_pb->at(k)/jet_flavor_component_jfitcomb_pc->at(k));
    recoJet.JetFitterCu = 0.;//log(jet_flavor_component_jfitcomb_pc->at(k)/jet_flavor_component_jfitcomb_pu->at(k)); //FIX_ME
    recoJet.JetFitterCb = 0.;//log(jet_flavor_component_jfitcomb_pc->at(k)/jet_flavor_component_jfitcomb_pu->at(k)); //FIX_ME 

    recoJet.FlavorTruth = local_truth_flavor;
    recoJet.bTagOPw = Jet_TaggerOp;
    (*jet_itr)->getAttribute(xAOD::JetAttribute::Width, recoJet.width); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::N90Constituents, recoJet.n90); //CHECK_ME
    (*jet_itr)->getAttribute(xAOD::JetAttribute::HECFrac, recoJet.hecf); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::HECQuality, recoJet.HECQuality); 
    (*jet_itr)->getAttribute(xAOD::JetAttribute::LArQuality, recoJet.LArQuality); 
    recoJet.Pt_up = recoJet.Pt();
    recoJet.Pt_down = recoJet.Pt();

    jetCandidates.push_back(recoJet);

    if(this->printJet){
      std::cout << "Jet " << iJet << ":" << endl;
      recoJet.PrintInfo();
    }

    iJet++;

  }  //jet loop

  //sort the jet candidates in Pt
  if (jetCandidates.size() > 0) std::sort(jetCandidates.begin(), jetCandidates.end());


  //--- Get (recalculated) MissingEt  
  CHECK( tool_st->GetMET(*metRFC,
			 jets_sc.first,
			 electrons_sc.first,
			 muons_sc.first,
			 0,
			 0) );//CHECK_ME arely: the MuonTerm is set to "" for tool_st-> no muon term here then. 
  
  
  TVector2 metRF = getMET(metRFC, "Final"); 
  
  //--- Remove events with fake Etmiss
  if (Met_doFakeEtmiss && !this->isTruth){
    for(unsigned int iJet=0; iJet < jetCandidates.size(); iJet++){
      if(jetCandidates.at(iJet).Pt() > 40. && 
	 jetCandidates.at(iJet).BCH_CORR_JET > 0.05 && 
	 deltaPhi(metRF.Phi(), jetCandidates.at(iJet).Phi()) < 0.3){
	this->isFakeMet=true;
      }
    }
  }


  //--- Get truth electrons 
  if ( this->isMC && !this->isSignal && !this->isTruth ){

    //truth particles loop
    truthP_itr = m_truthP->begin();
    truthP_end = m_truthP->end();
    for( ; truthP_itr != truthP_end; ++truthP_itr ) {
      
      if( !isStable( *truthP_itr ) || !isElectron(*truthP_itr) ) continue;  //->isElectron() ? 

      Particle electron;
      electron.SetVector( getTLV( *truthP_itr ) ); 
      truthElectrons.push_back(electron);
    }
    //sort truth electrons in pt 
    if (truthElectrons.size()>0) std::sort(truthElectrons.begin(), truthElectrons.end());
  }

  //--- Get Tracks
  if(tVeto_Enable)
    doTrackVeto(electronCandidates, muonCandidates);

  //---non-isolated muons
  float DeltaR=10;
  for(unsigned int iMu=0; iMu < muonCandidates.size(); ++iMu){
    
    // Here we save the info on all the muons (before isolation, and before OR with jets)
    muon_pt.push_back(muonCandidates.at(iMu).Pt());
    muon_eta.push_back(muonCandidates.at(iMu).Eta());
    muon_phi.push_back(muonCandidates.at(iMu).Phi());
    muon_iso.push_back(muonCandidates.at(iMu).ptcone20);
    muon_ptiso30.push_back(muonCandidates.at(iMu).ptcone30);
    muon_etiso30.push_back(muonCandidates.at(iMu).etcone30);
    
    //loop over truth
    bool truthMuon = false;
    if (this->isMC){
      truthMuon = -1.0;

      truthP_itr = m_truthP->begin();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {

	if ( isHard( (*truthP_itr) ) ) continue;
	if ( ! isMuon( (*truthP_itr) ) ) continue;
	TLorentzVector v1(0, 0, 0, 0);
	fillTLV(v1, (*truthP_itr));
	
	if (v1.DeltaR(muonCandidates.at(iMu)) < 0.1){
	  truthMuon = true;
	  break;
	}
      }
    }
    muon_truth.push_back(truthMuon);
        
    // Find closest jet instead of removing the muon
    double closestJetDr = 99999.9;
    int    closestJetId = -1; 
    for(unsigned int iJet=0; iJet < jetCandidates.size(); ++iJet){
      DeltaR = jetCandidates.at(iJet).DeltaR(muonCandidates.at(iMu));
      if (DeltaR < closestJetDr){
	closestJetDr = DeltaR;
	closestJetId = iJet;
      }      
    }
    // I found closest jet...
    if (closestJetId >= 0){
      muon_jet_dR.push_back(closestJetDr);
      muon_jet_dPhi.push_back( jetCandidates.at(closestJetId).DeltaPhi(muonCandidates.at(iMu)) );
      muon_jet_pt.push_back(jetCandidates.at(closestJetId).Pt());
      muon_jet_eta.push_back(jetCandidates.at(closestJetId).Eta());
      muon_jet_phi.push_back(jetCandidates.at(closestJetId).Phi());
      muon_jet_nTrk.push_back(jetCandidates.at(closestJetId).nTrk);
      muon_jet_sumPtTrk.push_back(jetCandidates.at(closestJetId).chf * jetCandidates.at(closestJetId).Pt());
      muon_jet_chf.push_back(jetCandidates.at(closestJetId).chf);
      muon_jet_emf.push_back(jetCandidates.at(closestJetId).emf);
      muon_jet_mv1.push_back(jetCandidates.at(closestJetId).MV1);
      muon_jet_vtxf.push_back(jetCandidates.at(closestJetId).jvtxf);
    }
    else{
      muon_jet_dR.push_back(-100.0);
      muon_jet_dPhi.push_back(-100.0);
      muon_jet_pt.push_back(-100.0);
      muon_jet_eta.push_back(-100.0);
      muon_jet_phi.push_back(-100.0);
      muon_jet_nTrk.push_back(-100.0);
      muon_jet_sumPtTrk.push_back(-100.0);
      muon_jet_chf.push_back(-100.0);
      muon_jet_emf.push_back(-100.0);
      muon_jet_mv1.push_back(-100.0);
      muon_jet_vtxf.push_back(-100.0);
    }
  }
  
  muon_N = muon_pt.size();

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
  std::vector<TLorentzVector> truthJetsJVF;
  if(isMC){
    xAOD::JetContainer::const_iterator tjet_itr = m_truth_jets->begin();
    xAOD::JetContainer::const_iterator tjet_end = m_truth_jets->end();
    
    //loop over truth jets & save 'high-pt' jets for later use 
    for( ; tjet_itr != tjet_end; ++tjet_itr ) {
      
      if ( (*tjet_itr)->pt() < 10000. ) continue;
      
      TLorentzVector truthJet;
      truthJet.SetPtEtaPhiE( ( *tjet_itr )->pt()*0.001, ( *tjet_itr )->eta(), ( *tjet_itr )->phi(), ( *tjet_itr )->e()*0.001 ); 
      
      truthJetsJVF.push_back( truthJet );
    }
  }
  
  //--- Define signal jets
  int bch_cntr = 0;
  std::vector<float> calibJets_pt;
  std::vector<float> calibJets_eta;
  std::vector<float> calibJets_MV1;
  std::vector<int> calibJets_flavor;

  int bjet_counter=0;
  int bjet_counter_80eff=0;

  for (unsigned int iJet=0; iJet < jetCandidates.size(); ++iJet){
    if ( jetCandidates.at(iJet).Pt() < (Jet_RecoPtCut/1000.) ) continue;
    if ( fabs(jetCandidates.at(iJet).Eta()) > Jet_RecoEtaCut ) continue;
    
    //--- jvf cut   //CHECK_ME
    if (!this->isTruth && jetCandidates.at(iJet).Pt() < 50. && fabs(jetCandidates.at(iJet).Eta()) < 2.4) {

      float jvfCut = 0.5;
      
      if(isMC){
	bool isPileUp = tool_jvf->isPileUpJet(jetCandidates.at(iJet).GetVector(), truthJetsJVF);
	//--- Check if the RecoJet is a muon. If so, isPileUp=false !
	if (isPileUp && (syst_JVF==JvfUncErr::JvfUncHigh || syst_JVF==JvfUncErr::JvfUncLow) ){
	  TLorentzVector truthMuon(0, 0, 0, 0);
	  truthP_itr = m_truthP->begin();
	  for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	    
	    if ( ! isHard( (*truthP_itr) ) ) continue; //CHECK_ME
	    if ( ! isMuon( (*truthP_itr) ) ) continue;
	    fillTLV(truthMuon, (*truthP_itr));
	    if ( truthMuon.DeltaR(jetCandidates.at(iJet))<0.2 ) {
	      isPileUp = false;
	      break;
	    }
	  }
	}
	
	if (syst_JVF==JvfUncErr::JvfUncHigh) jvfCut = tool_jvf->getJVFcut(0.5, isPileUp, jetCandidates.at(iJet).Pt(), jetCandidates.at(iJet).Eta(), true );
	else if (syst_JVF==JvfUncErr::JvfUncLow) jvfCut = tool_jvf->getJVFcut(0.5, isPileUp, jetCandidates.at(iJet).Pt(), jetCandidates.at(iJet).Eta(), false );
      }
      
      // if ( jetCandidates.at(iJet).jvtxf < jvfCut) {  //MARTIN PUT THIS BACK!!
      // 	JVF_min = false;
      // 	continue;          
      // }
    }

    if ( jetCandidates.at(iJet).isBTagged(Jet_Tagger.Data()) ) 
      bjet_counter++;
    if ( jetCandidates.at(iJet).isBTagged_80eff(Jet_Tagger.Data()) ) 
      bjet_counter_80eff++;	
    
    recoJets.push_back( jetCandidates.at(iJet) ); //Save Signal Jets

    // //Save jet candidates for razor 
    // VectorJets_Razor.push_back(JetCandidates.at(iJet).GetVector());

    calibJets_pt.push_back(jetCandidates.at(iJet).Pt()*1000.);
    calibJets_eta.push_back( BtagEta( jetCandidates.at(iJet).Eta() ) ); //eta to be defined in [-2.5,2.5]
    calibJets_MV1.push_back(jetCandidates.at(iJet).MV1);
    calibJets_flavor.push_back(jetCandidates.at(iJet).FlavorTruth);


    //--- BCH correction
    if (bch_cntr==0) this->Jet1_BCH_CORR_CELL = jetCandidates.at(iJet).BCH_CORR_CELL;
    if (bch_cntr==1) this->Jet2_BCH_CORR_CELL = jetCandidates.at(iJet).BCH_CORR_CELL;
    if (bch_cntr==2) this->Jet3_BCH_CORR_CELL = jetCandidates.at(iJet).BCH_CORR_CELL;
    if (jetCandidates.at(iJet).failBCHTight==true){
      this->JetsFailBCHTight = true;
      if (bch_cntr==0) this->Jet1FailBCHTight = true;
      if (bch_cntr==1) this->Jet2FailBCHTight = true;
      if (bch_cntr==2) this->Jet3FailBCHTight = true;
    }
    if (jetCandidates.at(iJet).failBCHMedium==true){
      this->JetsFailBCHMedium = true;
      if (bch_cntr==0) this->Jet1FailBCHMedium = true;
      if (bch_cntr==1) this->Jet2FailBCHMedium = true;
      if (bch_cntr==2) this->Jet3FailBCHMedium = true;
    }
    ++bch_cntr;
  }//end of jets loop

  n_jets = recoJets.size();

  //check for higher pt jet multiplicity
  for (unsigned int ij=0; ij<recoJets.size(); ij++){
    
    if(recoJets.at(ij).Pt()>40.){
      n_jets40++;
      if(recoJets.at(ij).Pt()>50.){
        n_jets50++;
        if(recoJets.at(ij).Pt()>60.){
	  n_jets60++;
	  if(recoJets.at(ij).Pt()>80.){
	    n_jets80++;
	  }
        }
      }
    }
  }

  n_bjets = bjet_counter; 
  n_bjets_80eff = bjet_counter_80eff; 


  //** btagging weights
  if(isMC){
    btag_weight_total       = GetBtagSF(m_goodJets, tool_btag);
    btag_weight_total_80eff = GetBtagSF(m_goodJets, tool_btag2);
  }
  
  //the list of jets to smear for qcd are not the jet-candidates!
  for (unsigned int iJet=0; iJet < jetCandidates.size(); ++iJet){
    if ( jetCandidates.at(iJet).Pt() < QCD_JetsPtPreselection/1000.0/*20.*/ ) continue;
    if ( fabs(jetCandidates.at(iJet).Eta()) > QCD_JetsEtaSelection/*2.8*/ ) continue;
    if ( jetCandidates.at(iJet).Pt() < 50. && fabs(jetCandidates.at(iJet).Eta())<2.4 && jetCandidates.at(iJet).jvtxf < 0.5) continue;
    seedJets.push_back( jetCandidates.at(iJet) );
  }

  //--- Get MET
  // For the moment only the official flavors are available.
  // Re-computing MET will be possible in the future though, once the METUtility interface is updated!!

  const xAOD::MissingETContainer* cmet_reffinal;
  const xAOD::MissingETContainer* cmet_lhtopo;
  const xAOD::MissingETContainer* cmet_track;

  if( ! m_event->retrieve( cmet_reffinal, "MET_RefFinal").isSuccess() ){
    Error("loop()", "Failed to retrieve MET_RefFinal container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  if( ! m_event->retrieve( cmet_lhtopo, "MET_LocHadTopo").isSuccess() ){
    Error("loop()", "Failed to retrieve MET_LocHadTopo container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  if( ! m_event->retrieve( cmet_track, "MET_Track").isSuccess() ){
    Error("loop()", "Failed to retrieve MET_Track container. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  const xAOD::MissingET* mrf    = (*cmet_reffinal)["Final"];
  const xAOD::MissingET* mtopo  = (*cmet_lhtopo)["LocHadTopo"];
  //  const xAOD::MissingET* mtrack = (*cmet_track)["PVTrack"];
  const xAOD::MissingET* mtrack = (*cmet_track)["Track"];

  //** Met components
  //** see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/Run2xAODMissingET 
  //  const xAOD::MissingET* mrf_refele = (*cmet_reffinal)["RefEle"];      //standard
  //  const xAOD::MissingET* mrf_refjet = (*cmet_reffinal)["RefJet"];
  //  const xAOD::MissingET* mrf_refgam = (*cmet_reffinal)["RefGamma"];
  //  const xAOD::MissingET* mrf_reftau = (*cmet_reffinal)["RefTau"];
  //  const xAOD::MissingET* mrf_refmuon = (*cmet_reffinal)["Muons"];
  //  const xAOD::MissingET* mrf_refscls = (*cmet_reffinal)["SoftClus"];
  //  const xAOD::MissingET* mrf_refstrk = (*cmet_reffinal)["SoftTrk"]; 
  //  const xAOD::MissingET* mrf_refjetjvf = (*cmet_reffinal)["RefJetJVF"];  //refined
  //  const xAOD::MissingET* mrf_refpvstrk = (*cmet_reffinal)["PVSoftTrk"]; 

  //- Recomputed MET via SUSYTools
  //- usually muons are treated as invisible pwarticles here! (i.e. Met_doRefMuon=Met_doMuonTotal=false, set via jOpt)
  if(Met_doMuons){
    CHECK( tool_st->GetMET(*metRFC,
			   jets_sc.first,
			   electrons_sc.first,
			   muons_sc.first,
			   0,
			   0));
  }
  else{
    CHECK( tool_st->GetMET(*metRFC,
			   jets_sc.first,
			   electrons_sc.first,
			   0,
			   0,
			   0));
  }
  
  TVector2 v_met_ST = getMET( metRFC, "Final");

  met_obj.SetVector(v_met_ST,"");  //- Copy met vector to the met data member
  met_obj.SetHasMuons( Met_doMuons );  //-- Set if muons enter into the MET computation
  
  //- Track met
  TVector2 v_met_trk( mtrack->mpx(), mtrack->mpy() ); 
  met_obj.SetVector(v_met_trk, "met_trk");

  //--- Met definition with the muons summed (i.e. visible)
  TVector2 v_met_muinv_ST = met_obj.GetVector(); //== met_ST (GeV) 
  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
    TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
    v_met_muinv_ST -= muon_vector2;
  }
  met_obj.SetVector(v_met_muinv_ST, "met_mu", true); //it is in GeV already!
  
  //--- Met LocHadTopo (from the branches)
  TVector2 v_met_lochadtopo( mtopo->mpx(), mtopo->mpy() ); 
  met_obj.SetVector(v_met_lochadtopo, "met_locHadTopo");
  
  //--- Met RefFinal, visible muons (from the branches) [retrieved above]
  TVector2 v_met_rfinal_mu(mrf->mpx(), mrf->mpy());
  met_obj.SetVector(v_met_rfinal_mu, "met_refFinal");


  //--- Met RefFinal, invisible muons (from the branches)
  TVector2 v_met_rfinal_muinv = met_obj.GetVector("met_refFinal"); //in GeV
  for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
    TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
    v_met_rfinal_muinv += muon_vector2;
  }
  met_obj.SetVector(v_met_rfinal_muinv, "met_refFinal_mu", true); //already in GeV
  
  //--- Met with electrons as invisible 
  TVector2 v_met_elinv_ST = met_obj.GetVector(); //== met_ST (GeV) 
  if (e_N==1) { 
    float metmod=v_met_elinv_ST.Mod();
    float metphi=TVector2::Phi_mpi_pi(v_met_elinv_ST.Phi());
    float e0pt  = recoElectrons.at(0).Pt();
    float e0phi = recoElectrons.at(0).Phi();
    v_met_elinv_ST.Set( metmod*cos(metphi) + e0pt*cos(e0phi), metmod*sin(metphi)+e0pt*sin(e0phi) );
  }
  met_obj.SetVector(v_met_elinv_ST, "met_ecorr", true); //already in GeV    

  //- Met Cleaning
  if (Met_doMetCleaning){
    const xAOD::MissingET* mrf_refmuon = (*cmet_reffinal)["Muons"];
    TVector2 myMetMuon( mrf_refmuon->mpx(), mrf_refmuon->mpy() ); //MET_MuonBoy in run1
    isMetCleaned = ( ( (myMetMuon.Mod() / v_met_rfinal_mu.Mod()) * cos(v_met_rfinal_mu.Phi() - myMetMuon.Phi()) ) < 0.8);
  }

  if (printMet){
    cout<<"Info for MET "<<endl;
    met_obj.PrintInfo();
    cout<<"met_ST.Mod() = " << v_met_ST.Mod()*0.001 << endl;
    cout<<"MET from D3PD = " << v_met_rfinal_mu.Mod()*0.001 << endl;
    cout<<endl;
  }
  
  //--- Fill metPhi and metMod data members
  met = met_obj.GetVector("").Mod();
  met_trk = met_obj.GetVector("met_trk").Mod();
  met_mu = met_obj.GetVector("met_mu").Mod();
  met_lochadtopo = met_obj.GetVector("met_locHadTopo").Mod();
  met_reffinalNoMu = met_obj.GetVector("met_refFinal_mu").Mod();
  met_ecorr = met_obj.GetVector("met_ecorr").Mod();
  
  met_phi = TVector2::Phi_mpi_pi( met_obj.GetVector("").Phi() ); //--- Phi defined between -pi and pi
  mettrk_phi = TVector2::Phi_mpi_pi( met_obj.GetVector("met_trk").Phi() ); //--- Phi defined between -pi and pi	


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
  //*** otherwise t_b_*** variables are not filled otherwise!

  //--- Invariant mass of the 2 first jets (M12)
  if(n_jets>1){
    TLorentzVector jet_M = (recoJets[0].GetVector() + recoJets[1].GetVector());
    M12 = jet_M.M();
  }

  //- dPhi & dR
  dPhi_met_mettrk = deltaPhi(met_obj.Phi("met_trk"), met_obj.Phi("met"));
  
  if (n_jets>0){
    dPhi_met_j1    = deltaPhi( met_obj.Phi(""), recoJets.at(0).Phi());
    dPhi_mettrk_j1 = deltaPhi( met_obj.Phi("met_trk"), recoJets.at(0).Phi());	
    dPhi_j1_bp1 = deltaPhi(recoJets.at(0).Phi(), t_b_phi1);
    dPhi_j1_bp2 = deltaPhi(recoJets.at(0).Phi(), t_b_phi2);
    dR_j1_bp1   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi1, t_b_eta1);
    dR_j1_bp2   = deltaR(recoJets.at(0).Phi(), recoJets.at(0).Eta(), t_b_phi2, t_b_eta2);

    if (recoMuons.size()>0){
      dR_j1_m1 = recoJets.at(0).DeltaR(recoMuons.at(0));
      if (recoMuons.size()>1) 
	dR_j1_m2 = recoJets.at(0).DeltaR(recoMuons.at(1));
    }

    if (n_jets>1){
      dPhi_met_j2    = deltaPhi( met_obj.Phi(""), recoJets.at(1).Phi());
      dPhi_mettrk_j2 = deltaPhi( met_obj.Phi("met_trk"), recoJets.at(1).Phi());	

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

      if (n_jets>2){
	dPhi_met_j3    = deltaPhi( met_obj.Phi(""), recoJets.at(2).Phi());
	dPhi_mettrk_j3 = deltaPhi( met_obj.Phi("met_trk"), recoJets.at(2).Phi());	
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

	if(n_jets>3){
	  dPhi_met_j4    = deltaPhi( met_obj.Phi(""), recoJets.at(3).Phi());
	  dPhi_mettrk_j4 = deltaPhi( met_obj.Phi("met_trk"), recoJets.at(3).Phi());
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


  //- MT
  if(n_jets>0){
    j1_mT = Calc_MT( recoJets.at(0), met_obj.GetVector("") );
    if(n_jets>1){
      j2_mT = Calc_MT( recoJets.at(1), met_obj.GetVector("") );
      if(n_jets>2){
	j3_mT = Calc_MT( recoJets.at(2), met_obj.GetVector("") );
	if(n_jets>3){
	  j4_mT = Calc_MT( recoJets.at(3), met_obj.GetVector("") );    
	}
      }
    }
  }


  //Do we need this??
  //- (truth) Tau veto for 3rd- and 4th leading jet
  float dR_truthTau_j3, dR_truthTau_j4;

  if (this->isMC){
    if (n_jets>2){
      
      TLorentzVector TruthTau3(0.,0.,0.,0.);
      TLorentzVector TruthTau4(0.,0.,0.,0.);
      
      if(recoJets.at(2).nTrk>0 && recoJets.at(2).nTrk<5 && dPhi_met_j3 < TMath::Pi()/5. && j3_mT<100.){
	
	truthP_itr = m_truthP->begin();
	for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	  
	  if ( isHard((*truthP_itr)) && isTau((*truthP_itr)) ){
	    fillTLV(TruthTau3, (*truthP_itr));

	    dR_truthTau_j3 = TruthTau3.DeltaR(recoJets.at(2));
	  }
	}
      } 
      
      if (n_jets>3){
	if(recoJets.at(3).nTrk>0 && recoJets.at(3).nTrk < 5 && dPhi_met_j4 < TMath::Pi()/5. && j4_mT<100.){

	  truthP_itr = m_truthP->begin();
	  for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	    
	    if ( isHard((*truthP_itr)) && isTau((*truthP_itr)) ){
	      fillTLV(TruthTau4, (*truthP_itr));
	      
	      dR_truthTau_j4 = TruthTau4.DeltaR(recoJets.at(3));
	    }
	  }
	}            
      }
    }
  }


  float min_mt_jm=999999.;
  int ibcl=-1;  //closest bjet to met
  int ibfar=-1; //most faraway bjet from met
  int ilcl=-1;  //closest light jet to met
  float min_dphi_bm=999.;
  float max_dphi_bm=0.;
  float min_dphi_lm=999.;
    
  int ibtop1=-1; //book two highest btag-weight jets for later use
  int ibtop2=-1;
  int iblead1=-1;
  int iblead2=-1;
  float maxbw1=0;
  float maxbw2=0;
  
  std::vector<float> jets_mT;
  std::vector<float> jets_dPhi_met;
  std::vector<float> jets_dPhi_mettrk;

  for(int ijet=0; ijet < n_jets; ijet++){        

    float dphi_jm = deltaPhi( recoJets.at(ijet).Phi(), met_phi );

    jets_dPhi_met.push_back( dphi_jm );
    jets_dPhi_mettrk.push_back( deltaPhi(recoJets.at(ijet).Phi(), mettrk_phi) );

    jets_mT.push_back( Calc_MT( recoJets.at(0), met_obj.GetVector("") ) );
	
    if(dPhi_min_alljets < 0 || dPhi_min_alljets > deltaPhi(recoJets.at(ijet).Phi(), met_phi) )
      dPhi_min_alljets = deltaPhi(recoJets.at(ijet).Phi(), met_phi);

    if(ijet < 3 && (dPhi_min < 0 || dPhi_min > deltaPhi(recoJets.at(ijet).Phi(), met_phi) ))
      dPhi_min = deltaPhi(recoJets.at(ijet).Phi(), met_phi );


    //--- check for 'tau' jets
    if ( recoJets.at(ijet).isTauJet(met_phi, Jet_Tagger.Data()) ) 
      n_taujets += 1;
	
    //--- look for min MT(jet,MET)
    float mt_jm = Calc_MT( recoJets.at(ijet), met_obj.GetVector("") );
    if(mt_jm < min_mt_jm){
      min_mt_jm = mt_jm;
    }

    //--- look for closest/faraway bjet and closer light jet to MET
    if( recoJets.at(ijet).isBTagged(Jet_Tagger) ){

      if(iblead1<0)//leadings
	iblead1=ijet;
      else if(iblead2<0)
	iblead2=ijet;

      if( dphi_jm < min_dphi_bm){ //closest
	min_dphi_bm = dphi_jm;
	ibcl = ijet;
      }

      if( dphi_jm > max_dphi_bm){ //most faraway
	max_dphi_bm = dphi_jm;
	ibfar = ijet;
      }

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
    else{
      if( dphi_jm < min_dphi_lm){
	min_dphi_lm = dphi_jm;
	ilcl = ijet;
      }
    }

    //--- use jet info for other variables
    HT   += recoJets.at(ijet).Pt();
    meff += recoJets.at(ijet).Pt();
    
  }//end n_jets loop

  //meff = HT + met
  meff += met;

  
  //Dijet Mass
  if (n_jets>1){  
    DiJet_Mass = Calc_dijetMass();
    if( recoJets.at(0).isBTagged(Jet_Tagger) && recoJets.at(1).isBTagged(Jet_Tagger) )
      DiBJet_Mass = DiJet_Mass;
  }

  //Mct
  mct = Calc_mct();

  if( min_mt_jm==999999. )
    min_mt_jm=0.; //it shouldn't matter, but let's force zero-jets events to fail this!
  
  MT_min_jet_met = min_mt_jm;
  MT_bcl_met = (ibcl >= 0 ? Calc_MT( recoJets.at(ibcl), met_obj.GetVector("") ) : 0.);
  MT_bfar_met = (ibfar >= 0 ? Calc_MT( recoJets.at(ibfar), met_obj.GetVector("") ) : 0.);
  MT_lcl_met = (ilcl >= 0 ? Calc_MT( recoJets.at(ilcl), met_obj.GetVector("") ) : 0.);
  MT_jsoft_met = (recoJets.size()>0 ? Calc_MT( recoJets.back(), met_obj.GetVector("") ) : 0.); //it assumes jets are pt ordered!
  
  dPhi_b1_b2 = (iblead1>=0 && iblead2>=0 ? deltaPhi( recoJets.at(iblead1).Phi(), recoJets.at(iblead2).Phi() ) : 0.);


  //==== Razor ====
  fillRazor();

  //==== Hadronic top reconstruction ====
  RecoHadTops(ibtop1, ibtop2);

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
    TVector2 met_electron = met_obj.GetVector("met"); //arely
    e_MT = Calc_MT( recoElectrons.at(0), met_electron);
  }
  
  //--- Define m_MT - we recompute the MET with the muon in order to have the pt of the nu
  if(recoMuons.size()>0){ //--- Careful: Optimized for Etmiss without muons!
    TVector2 v_m1(recoMuons.at(0).Px(), recoMuons.at(0).Py());
    //TVector2 met_muon = met_obj.GetVector("met") - v_m1; //CHECK_ME  - o + ? met has inv muons already o.O
    TVector2 met_muon = met_obj.GetVector("met"); //arely
    if (!met_obj.GetHasMuons())//muons are invisible, need to remove them from MET
         met_muon = met_obj.GetVector("met") - v_m1;
    m_MT = Calc_MT( recoMuons.at(0), met_muon);
  }
   

  //--- Boson Pt   <---  V(lv).Pt()
  TVector2 V_lnu = met_obj.GetVector();
  for (unsigned int iEl=0; iEl < recoElectrons.size(); iEl++){
    TVector2 electron_vector2(recoElectrons.at(iEl).GetVector().Px(), recoElectrons.at(iEl).GetVector().Py());
    V_lnu += electron_vector2;
  }
  if (met_obj.GetHasMuons()){
    for (unsigned int iMu=0; iMu < recoMuons.size(); iMu++){
      TVector2 muon_vector2(recoMuons.at(iMu).GetVector().Px(), recoMuons.at(iMu).GetVector().Py());
      V_lnu += muon_vector2;
    }
  }
    
  bosonVec_reco_pt = V_lnu.Mod();
  //bosonVec_reco_eta = V_lnu.Eta();  //not accesible!
  bosonVec_reco_phi = TVector2::Phi_mpi_pi( V_lnu.Phi() );       
  
  if (!isTruth)
    MtTop = TopTransvMass();
  
  
  //QCD Trigger stuff...  //FIX_ME    once we have access to trigger decision!
  //...

  //---Fill missing data members 
  this->dumpLeptons();
  this->dumpJets();
  

  //Redefine run number for MC, to reflect the channel_number instead //CHECK_ME
  if(isMC) RunNumber = mc_channel_number;


  // The containers created by the shallow copy are owned by you. Remember to delete them
  delete jets_sc.first;
  delete jets_sc.second;
  delete muons_sc.first;
  delete muons_sc.second;
  delete electrons_sc.first;
  delete electrons_sc.second;
  // delete photons_sc.first;
  // delete photons_sc.second;
    
  output->setFilterPassed ();

  return nextEvent(); //SUCCESS + cleaning
}

//Fill lepton data members
void chorizo :: dumpLeptons(){

  if(e_N>0){
    e_pt = recoElectrons.at(0).Pt();
    e_eta = recoElectrons.at(0).Eta();
    e_phi = recoElectrons.at(0).Phi();
    e_etiso30 = recoElectrons.at(0).etcone30;
    e_ptiso30 = recoElectrons.at(0).ptcone30;
    e_tight = recoElectrons.at(0).isTight;
  }

  if (truthElectrons.size()>0){
    e_truth_pt = truthElectrons.at(0).Pt();
    e_truth_eta = truthElectrons.at(0).Eta();
    e_truth_phi = truthElectrons.at(0).Phi();
  }  

  if(recoMuons.size()>0){
    m_pt = recoMuons.at(0).Pt();
    m_eta = recoMuons.at(0).Eta();
    m_phi = recoMuons.at(0).Phi();
    m_etiso20 = recoMuons.at(0).etcone20;
    m_ptiso20 = recoMuons.at(0).ptcone20;
    m_etiso30 = recoMuons.at(0).etcone30;
    m_ptiso30 = recoMuons.at(0).ptcone30;
  }

  if(recoMuons.size()>1){
    m2_pt = recoMuons.at(1).Pt();
    m2_eta = recoMuons.at(1).Eta();
    m2_phi = recoMuons.at(1).Phi();
    m2_etiso30 = recoMuons.at(1).etcone30;
    m2_ptiso30 = recoMuons.at(1).ptcone30;
  }

}

//Fill jet data members 
void chorizo :: dumpJets(){

  //jets + btagging  
  if (n_jets>0){
    tag_MV1_1 = recoJets.at(0).MV1;
    tag_SV1_1 = recoJets.at(0).SV1plusIP3D;
    // tag_JetFitterCOMBNN_1 = recoJets.at(0).JetFitterCOMBNN;
    // tag_JetFitterCOMBNNc_1 = recoJets.at(0).JetFitterCOMBNNc;
    tag_JetFitterCu_1 = recoJets.at(0).JetFitterCu;
    tag_JetFitterCb_1 = recoJets.at(0).JetFitterCb;
    pt1 = recoJets.at(0).Pt();
    eta1 = recoJets.at(0).Eta();
    phi1 = recoJets.at(0).Phi();
    j1_chf = recoJets.at(0).chf;
    j1_emf = recoJets.at(0).emf;
    j1_fsm = recoJets.at(0).fsm;
    j1_time = recoJets.at(0).time;
    j1_jvtxf = recoJets.at(0).jvtxf;
    j1_tflavor = recoJets.at(0).FlavorTruth;
    j1_nTrk = recoJets.at(0).nTrk;      
    j1_sumPtTrk = recoJets.at(0).sumPtTrk;
    
    if (n_jets>1){
      //Dijet Mass
      DiJet_Mass = Calc_dijetMass();
      if( recoJets.at(0).isBTagged(Jet_Tagger) && recoJets.at(1).isBTagged(Jet_Tagger) )
	DiBJet_Mass = DiJet_Mass;
      
      //btagging
      tag_MV1_2 = recoJets.at(1).MV1;
      tag_SV1_2 = recoJets.at(1).SV1plusIP3D;
      // tag_JetFitterCOMBNN_2 = recoJets.at(1).JetFitterCOMBNN;
      // tag_JetFitterCOMBNNc_2 = recoJets.at(1).JetFitterCOMBNNc;
      tag_JetFitterCu_2 = recoJets.at(1).JetFitterCu;
      tag_JetFitterCb_2 = recoJets.at(1).JetFitterCb;
      //if (!isTruth) btageffSF2 = recoJets.at(1).TagEffSF;
      //if (!isTruth) btagineffSF2 = recoJets.at(1).TagIneffSF;
      pt2 = recoJets.at(1).Pt();
      eta2 = recoJets.at(1).Eta();
      phi2 = recoJets.at(1).Phi();
      j2_chf = recoJets.at(1).chf;
      j2_emf = recoJets.at(1).emf;
      j2_fsm = recoJets.at(1).fsm;
      j2_time = recoJets.at(1).time;
      j2_jvtxf = recoJets.at(1).jvtxf;
      j2_tflavor = recoJets.at(1).FlavorTruth;
      j2_nTrk = recoJets.at(1).nTrk;      
      j2_sumPtTrk = recoJets.at(1).sumPtTrk;
        
      if (n_jets>2){
	tag_MV1_3 = recoJets.at(2).MV1;
	tag_SV1_3 = recoJets.at(2).SV1plusIP3D;
	// tag_JetFitterCOMBNN_3 = recoJets.at(2).JetFitterCOMBNN;
	// tag_JetFitterCOMBNNc_3 = recoJets.at(2).JetFitterCOMBNNc;
	tag_JetFitterCu_3 = recoJets.at(2).JetFitterCu;
	tag_JetFitterCb_3 = recoJets.at(2).JetFitterCb;
	//if (!isTruth) btageffSF3 = recoJets.at(2).TagEffSF;
	//if (!isTruth) btagineffSF3 = recoJets.at(2).TagIneffSF;
	pt3 = recoJets.at(2).Pt();
	eta3 = recoJets.at(2).Eta();
	phi3 = recoJets.at(2).Phi();
	j3_chf = recoJets.at(2).chf;
	j3_emf = recoJets.at(2).emf;
	j3_fsm = recoJets.at(2).fsm;
	j3_time = recoJets.at(2).time;
	j3_jvtxf = recoJets.at(2).jvtxf;
	j3_tflavor = recoJets.at(2).FlavorTruth;
	j3_nTrk = recoJets.at(2).nTrk;      
	j3_sumPtTrk = recoJets.at(2).sumPtTrk; 
          
	if (n_jets>3){
	  tag_MV1_4 = recoJets.at(3).MV1;
	  tag_SV1_4 = recoJets.at(3).SV1plusIP3D;
	  // tag_JetFitterCOMBNN_4 = recoJets.at(3).JetFitterCOMBNN;
	  // tag_JetFitterCOMBNNc_4 = recoJets.at(3).JetFitterCOMBNNc;
	  tag_JetFitterCu_4 = recoJets.at(3).JetFitterCu;
	  tag_JetFitterCb_4 = recoJets.at(3).JetFitterCb;
	  //if (!isTruth) btageffSF4 = recoJets.at(3).TagEffSF;
	  //if (!isTruth) btagineffSF4 = recoJets.at(3).TagIneffSF;
	  pt4 = recoJets.at(3).Pt();
	  eta4 = recoJets.at(3).Eta();
	  phi4 = recoJets.at(3).Phi();
	  j4_chf = recoJets.at(3).chf;
	  j4_emf = recoJets.at(3).emf;
	  j4_fsm = recoJets.at(3).fsm;
	  j4_time = recoJets.at(3).time;
	  j4_jvtxf = recoJets.at(3).jvtxf;
	  j4_tflavor = recoJets.at(3).FlavorTruth;
	  j4_nTrk = recoJets.at(3).nTrk;      
	  j4_sumPtTrk = recoJets.at(3).sumPtTrk; 	    
	    

	  if (n_jets>4){
	    pt5 = recoJets.at(4).Pt();
	    eta5 = recoJets.at(4).Eta();
	    phi5 = recoJets.at(4).Phi();

	    if (n_jets>5){
	      pt6 = recoJets.at(5).Pt();
	      eta6 = recoJets.at(5).Eta();
	      phi6 = recoJets.at(5).Phi();
	    } // end n_jets>5 
	  } // end n_jets>4
	} // end n_jets>3
      } // end n_jets>2
    } // end n_jets>1
  } // end n_jets>0

} //end filling jet data members



EL::StatusCode chorizo :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode chorizo :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  if(tool_purw){
    if(genPUfile)
      tool_purw->finalize();

    delete tool_purw;
    tool_purw=0;
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
  
  //jet cleaning
  if( tool_jClean ) {
    delete tool_jClean;
    tool_jClean = 0;
  }

  //Stop clock
  watch.Stop();
  //  Info("finalize()", Form("Execution time = %f (%f/ev)", watch.CpuTime(), watch.CpuTime()/(float)m_event->getEntries()));
  Info("finalize()", Form("Execution time = %f", watch.CpuTime()));

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode chorizo :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.  
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

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("vetoMCevent()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }
  
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

  //to loop over  
  xAOD::TruthParticleContainer::const_iterator truthP_itr;
  xAOD::TruthParticleContainer::const_iterator truthP_end;
  
  double cutting_var=0.;
  if (this->mc_channel_number>=156803 && this->mc_channel_number<=156828){ //Znunu AlpGen

    //truth particles loop                                                              
    truthP_itr = m_truthP->begin();
    truthP_end = m_truthP->end();
    for( ; truthP_itr != truthP_end; ++truthP_itr ) {

      if(( *truthP_itr )->status()==124 && ( *truthP_itr )->absPdgId()==23){ //Z boson
	cutting_var = ( *truthP_itr )->pt();
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
      
      truthP_itr = m_truthP->begin();
      truthP_end = m_truthP->end();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	if( !isHard( *truthP_itr ) ) continue; //look at ME particles only
	if( !isLepNeut( *truthP_itr) ) continue; //look for leptons only
	mc3_n++;
	mc_status[mc3_n] = ( *truthP_itr )->status();
	mc_pdgId[mc3_n] = ( *truthP_itr )->pdgId();
	mc_pt[mc3_n] =  ( *truthP_itr )->pt();
	mc_eta[mc3_n] = ( *truthP_itr )->eta();
	mc_phi[mc3_n] = ( *truthP_itr )->phi();
	mc_m[mc3_n] = ( *truthP_itr )->m();
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

      //reinitialize iterators
      truthP_itr = m_truthP->begin();
      for( ; truthP_itr != truthP_end; ++truthP_itr ) {
	
	if( isLepNeut( (*truthP_itr) ) && isStable( (*truthP_itr) ) ){
	  bos_pdgId = (*truthP_itr)->pdgId(); //CHECK_ME
	  lep_count++;
	  if(lep_count==1){
	    fillTLV(v1, (*truthP_itr));
	  }
	  else if(lep_count==2){
	    fillTLV(v2, (*truthP_itr));
	    break;
	  }
	}//if                                                                                   
      }//mc particles loop

      TLorentzVector v3(0.,0.,0.,0.);
      v3+=v1;
      v3+=v2;
      
      cutting_var=v3.Pt()/1000.;
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
    xAOD::MissingETContainer* metRFcutvar; // = new xAOD::MissingETContainer;
    if( ! m_event->retrieve( metRFcutvar, "MET_RefFinal").isSuccess() ){
      Error("loop()", "Failed to retrieve MET_RefFinal container. Exiting." );
      return EL::StatusCode::FAILURE;
    }
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

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetTTbarReweight()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

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

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetNTruthB()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

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

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetNTruthHF()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

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

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetNTruthLeptons()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

  xAOD::TruthParticleContainer::const_iterator truthP_itr = m_truthP->begin();
  xAOD::TruthParticleContainer::const_iterator truthP_end = m_truthP->end();  

  for( ; truthP_itr != truthP_end; ++truthP_itr ) {
    if ( isHard( (*truthP_itr) ) && isLepton( (*truthP_itr) ) ){
	lep_counter+=1;
    }
  }
    
  return lep_counter;
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
 
  if (sigSamPdgId==0) return;
 
  TLorentzVector V(0.,0.,0.,0.);
  TLorentzVector l1, l2;
  int i= 0;

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetTruthShat()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   throw EL::StatusCode::FAILURE;
  //   //return 
  // }

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

  // //--- Count the truth jets above 30GeV in the event  //CHECK_ME
  // const xAOD::JetContainer* truth_jets = 0;
  // if( ! m_event->retrieve( truth_jets, "AntiKt4TruthJets").isSuccess() ){
  //   Error("GetGeneratorUncertaintiesSherpa()", "Failed to retrieve Truth jets collection : AntiKt4TruthJets. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

  xAOD::JetContainer::const_iterator tjet_itr = m_truth_jets->begin();
  xAOD::JetContainer::const_iterator tjet_end = m_truth_jets->end();

  // //retrieve truth particles container 
  // const xAOD::TruthParticleContainer* truthP;    
  // if ( !m_event->retrieve( truthP, "TruthParticle" ).isSuccess() ){ 
  //   Error("GetGeneratorUncertaintiesSherpa()", "Failed to retrieve 'TruthParticle' container. Exiting." );
  //   return EL::StatusCode::FAILURE;
  // }

  xAOD::TruthParticleContainer::const_iterator truthP_itr;
  xAOD::TruthParticleContainer::const_iterator truthP_end;

  //loop over truth jets
  TLorentzVector truthJet;
  for( ; tjet_itr != tjet_end; ++tjet_itr ) {
  
    fillTLV( truthJet, (*tjet_itr), true);
    
    //look for overlapping electron
    bool isElectronFakingJet = false;
    truthP_itr = m_truthP->begin();
    truthP_end = m_truthP->end();
    for( ; truthP_itr != truthP_end; ++truthP_itr ) {
      
      TLorentzVector truthPart;
      if ( isElectron( *truthP_itr ) && isStable( *truthP_itr ) ){
  	fillTLV( truthPart, ( *truthP_itr ), true);
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


float chorizo::GetBtagSF(xAOD::JetContainer* goodJets, BTaggingEfficiencyTool* btagTool)
{
  float totalSF = 1.;
  
  if (this->isTruth || this->isMC) return totalSF;
  
  xAOD::JetContainer::const_iterator jet_itr = goodJets->begin();
  xAOD::JetContainer::const_iterator jet_end = goodJets->end();
  
  for( ; jet_itr != jet_end; ++jet_itr ) {
    
    float sf = 1.;
    
    CP::CorrectionCode result;
    
    if ( (*jet_itr)->auxdata< char >("bjet") )
      {
	result = btagTool->getScaleFactor(**jet_itr, sf);
	
	if (result != CP::CorrectionCode::Ok)
	  Error("BTagSF", " Failed to retrieve eff SF for b-tagged jet!" );
      }
    else
      {
	result = btagTool->getInefficiencyScaleFactor(**jet_itr, sf);
	
	if (result != CP::CorrectionCode::Ok)
	  Error("BTagSF", " Failed to retrieve Ineff SF for b-tagged jet!" );
      }
    
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

//Calculation of extra variables
float chorizo :: Calc_MT(Particles::Jet jet, TVector2 met){

  return sqrt(2 * jet.Pt() * met.Mod() * (1-cos(deltaPhi(TVector2::Phi_mpi_pi( met.Phi() ), jet.Phi()))));

}

float chorizo :: Calc_MT(Particles::Particle p, TVector2 met){

  return sqrt(2 * p.Pt() * met.Mod() * (1-cos(deltaPhi(TVector2::Phi_mpi_pi( met.Phi() ), p.Phi()))));

}

float chorizo :: Calc_mct(){

  if(n_jets<2) return -1;

  float mct =  (recoJets.at(0).Mt() + recoJets.at(1).Mt())*(recoJets.at(0).Mt() + recoJets.at(1).Mt()) - (recoJets.at(0)-recoJets.at(1)).Perp2();
  mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);   

  return mct;
};

float chorizo :: TopTransvMass(){

  if (n_jets < 3) return -10.;
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

  if(ibtop1<0 || ibtop2<0 || (recoJets.size()<4)){ //at least two bjets and 4 jets  (to build at least the first top)
    m_top_had1=-1;
    m_top_had2=-1;
    return;
  }
    
  int W1j1 = -1;
  int W1j2 = -1;
  int W1dr = 999.;
  int W2j1 = -1;
  int W2j2 = -1;
  int W2dr = 999.;
  //       tuple<int, int, float> Whad1(-1, -1, 999.);  //requires C++0x
  //       tuple<int, int, float> Whad2(-1, -1, 999.);

  for(int ij1=0; ij1<n_jets; ij1++){
    if(ij1==ibtop1  || ij1==ibtop2) continue; //skip btags
    for(int ij2=ij1+1; ij2<n_jets; ij2++){
      if(ij2==ibtop1  || ij2==ibtop2) continue; //skip btags
      float locdr = recoJets.at(ij1).DeltaR( recoJets.at(ij2).GetVector() );
      
      if(locdr < W1dr){
	if(W1dr<999){
	  W2j1 = W1j1;
	  W2j2 = W1j2;
	  W2dr = W1dr;
	}
	W1j1 = ij1;
	W1j2 = ij2;
	W1dr = locdr;
      }
      else if(locdr < W2dr &&
	      ij1 != W1j1 &&
	      ij1 != W1j2 &&
	      ij2 != W1j1 &&
	      ij2 != W1j2 ){
	W2j1 = ij1;
	W2j2 = ij2;
	W2dr = locdr;
      }
    }
  }
 
  //top1 mass
  bool b1used=false;
  if(W1j1>=0 && W1j2>=0){
    TLorentzVector vW1 = (recoJets.at(W1j1).GetVector() + recoJets.at(W1j2).GetVector());
    if( vW1.DeltaR( recoJets.at(ibtop1).GetVector()) < vW1.DeltaR(recoJets.at(ibtop2).GetVector() ) ){
      m_top_had1 = ( vW1 + recoJets.at(ibtop1).GetVector() ).M();
      b1used=true;
    }
    else{
      m_top_had1 = ( vW1 + recoJets.at(ibtop2).GetVector() ).M();
    }
  }
  else{
    m_top_had1 = -1;
  }
  
  //top2 mass
  if(W2j1>=0 && W2j2>=0){
    TLorentzVector vW2 = (recoJets.at(W2j1).GetVector() + recoJets.at(W2j2).GetVector());
    if( !b1used ) //I think it is enough!
      m_top_had2 = ( vW2 + recoJets.at(ibtop1).GetVector() ).M();
    else 
      m_top_had2 = ( vW2 + recoJets.at(ibtop2).GetVector() ).M();
  }
  else{
    m_top_had2 = -1;
  }

  //recover original jet vector
  if(eladded)
    recoJets.pop_back();
  if(muadded)
    recoJets.pop_back();
};


std::vector<TLorentzVector> chorizo :: getFatJets(double R) {

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
  vector<PseudoJet> new_jets = sorted_by_pt(cs.inclusive_jets());

  //fill new fat jets
  for(unsigned int ifjet=0; ifjet < new_jets.size(); ifjet++){
    //fill      px    py  pz      E
    TLorentzVector p;
    p.SetPxPyPzE(new_jets[ifjet].px(),
		 new_jets[ifjet].py(),
		 new_jets[ifjet].pz(),
		 new_jets[ifjet].E());
    new_jets_tlv.push_back( p );
  }

  //recover original jet vector
  if(eladded)
    recoJets.pop_back();
  if(muadded)
    recoJets.pop_back();

  return new_jets_tlv;
} 

float chorizo::Calc_dijetMass(TLorentzVector ja,TLorentzVector jb){
  return (ja + jb).M();
}

float chorizo :: Calc_dijetMass(){
  if(n_jets<2) return -1.;
  return Calc_dijetMass(recoJets.at(0).GetVector(), recoJets.at(1).GetVector());
}

//----------- RAZOR

//fill Razor variables
void chorizo :: fillRazor(){

  std::vector<TLorentzVector> megajets = CombineJets();
  
  MR = Calc_MR(megajets.at(0), megajets.at(1));
  
  MTR = Calc_MTR(megajets.at(0), megajets.at(1), met_obj.GetMET_Razor());
  
  if(MR > 0.) R = MTR/MR;

  AlphaT = Calc_AlphaT(megajets.at(0), megajets.at(1));
  
  //new super-razor variables
  Calc_SuperRazor(megajets.at(0), megajets.at(1), met_obj.GetMET_Razor(), shatR, gaminvR, mdeltaR, cosptR);
  
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
    v = p->p4();
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
  if ( !m_event->retrieve( tracks, "GSFTrackParticles" ).isSuccess() ){ 
    Error("doTrackVeto()", "Failed to retrieve 'GSFTrackParticles' container. Exiting." );
    return EL::StatusCode::FAILURE;
  }

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
    int trk_PixHits = (*trk_itr)->auxdata< unsigned char>( "numberOfPixelHits" );
    int trk_SCTHits = (*trk_itr)->auxdata< unsigned char>( "numberOfSCTHits" );
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


