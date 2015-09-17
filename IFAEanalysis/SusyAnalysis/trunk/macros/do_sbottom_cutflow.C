#include <TChain.h>
#include <TH1F.h>
#include <utility>      // std::pair
#include <iostream>     // std::cout
#include <iomanip>     // std::cout



//********* NOTES ****************************************************************************
//
//     no Cosmic veto at the moment. Only remove muons suspected to be so.
//
//
//
//
//********* NOTES ****************************************************************************


std::pair<int, float> get(TChain* c, TString var, TString cut);
void print(TChain* ch, TString sel, TString var, TString cut);

float N_in;
float N_last;

int NCUT=0;


std::vector<TString> getTokens(TString line, TString delim){
  std::vector<TString> vtokens;
  TObjArray* tokens = line.Tokenize(delim); //delimiters                                                                                                                             
  if(tokens->GetEntriesFast()) {
    TIter iString(tokens);
    TObjString* os=0;
    while ((os=(TObjString*)iString())) {
      vtokens.push_back( os->GetString() );
    }
  }
  delete tokens;

  return vtokens;
}


void do_sbottom_cutflow(TString sample="", bool isTruth=false){
  
  N_in=-1;
  N_last=-1;

  if(sample=="")
    sample = "/nfs/at3/scratch/sfracchia/SUSY/METbb_histos/Nom_ttbar_cutflow.root";

  //--- Config
  TString myvar   = "mct";
  //TString weights = "(20100*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  //TString weights = "(10000*MC_w*w)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString weights = "(10000)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  
  TString base    = weights+"*( 1 ";

  //--- Trigger
  std::vector<TString> trigItemsSR;  // make OR of all these items!
  trigItemsSR.push_back("HLT_xe80");

  std::vector<TString> trigItemsCR;  // make OR of all these items!


  //load trigger map
  std::vector<TString> trigchains;                                                                                                                                                  
  TString trigCutSR = "";
  TString trigCutCR = "";

  if(!isTruth){
    TString strig = (TString) TFile::Open(sample)->Get("Triggers")->GetTitle();
    trigchains = getTokens(strig, ",");
    
    std::map<int,TString> trigmap{};
    auto ic=0;
    std::vector<int> trig_idxSR; trig_idxSR.clear();
    std::vector<int> trig_idxCR; trig_idxCR.clear();
    for(const auto& s : trigchains){
      
      for(auto t : trigItemsSR)
	if(t == s) trig_idxSR.push_back(ic);
      
      for(auto t : trigItemsCR)
	if(t == s) trig_idxCR.push_back(ic);
      
      trigmap[ic] = s;
      ic++;
    }
    
    //create trigger cut  
    if(trigItemsSR.size() && trig_idxSR.size() == trigItemsSR.size()){
      trigCutSR = Form(" && ( isTrigger[%d] ", trig_idxSR[0]);
      for(unsigned int ic=1; ic < trig_idxSR.size(); ic++)
	trigCutSR += Form(" || isTrigger[%d] ", trig_idxSR[ic]);
      trigCutSR += ") ";
    }
    else cout << "** WARNING **   Trigger(s) not found! Trigger won't be apply in SR for now... " << endl;

    if(trigItemsCR.size() && trig_idxCR.size() == trigItemsCR.size()){
      trigCutCR = Form(" && ( isTrigger[%d] ", trig_idxCR[0]);
      for(unsigned int ic=1; ic < trig_idxCR.size(); ic++)
	trigCutCR += Form(" || isTrigger[%d] ", trig_idxCR[ic]);
      trigCutCR += ") ";
    }
    else cout << "** WARNING **   Trigger(s) not found! Trigger won't be apply in CR for now... " << endl;
  }

  //SILVIA ?
  // trigCut = " && (isTrigger[61] || isTrigger[62]) ";

  if(isTruth){
    trigCutSR="";
    trigCutCR="";
  }


  //--- MET
  TString metFlv = "11";   //VisMuTST
  if(isTruth)
    metFlv = "0";   //NonInt

  TString nbjvar = "bj_N_77fc";
  TString btag1  = "j_tag_MV2c20[0]>-0.4434";
  TString btag2  = "j_tag_MV2c20[1]>-0.4434";

  if(isTruth){
    
    btag1 = "j_tflavor[0]==5";
    btag2 = "j_tflavor[1]==5";
  }

  TString PVcut="isVertexOk";
  if(isTruth)
    PVcut="1";


  cout << "BEFORE TCHAIN" << endl;

  TChain *ch = new TChain("AnalysisTree");
  ch->Add(sample);
   
  cout << "SBOTTOM 0L CUTFLOW " << endl;
  cout << "-------------------------------------------------------------------------------------------------------------------------------------" << endl;
  std::cout  << std::setw(45)
             << std::left
             << "      Selection"
             << std::setiosflags(std::ios::fixed)
             << std::setprecision(3)
             << std::setw(18)
             << std::left
             << "        Nraw"
             << "\t\t"
             << "   Rel.Eff."
             << "\t"
             << "   Abs.Eff"
             << "\t"
             << "   Nweighted"
             << std::endl;
  cout << "-------presel------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "No sel", myvar, base+" )");
  print(ch, "GRL", myvar, base+" && isGRL )");
  print(ch, "LarGood", myvar, base+" && isGRL && isLarGood)");  
  print(ch, "TileGood", myvar, base+" && isGRL && isLarGood && isTileGood)");  
  print(ch, "TileTrip", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip)");  
  print(ch, "CoreFlag", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag)");  
  print(ch, "Trigger", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+")");
    
  print(ch, "Vertex", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+")"); 
  print(ch, "JetCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID)"); 
  print(ch, "CosmicVeto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2)"); 
  print(ch, "MuonCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon)"); 
  
  cout << "-------SR----------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "MET (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100.)");  
  print(ch, "1 < Njets < 5", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5)");  
  
  print(ch, "jpt1>50, abs(eta)<2.8", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50.)");  
  print(ch, "jet veto (jpt4<50, abs(eta)<2.8)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50.)");  
  print(ch, "2 bjets (MV2c20@77%)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2)");    
  
  print(ch, "leading bjet (MV2c20@77%)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && fabs(j_eta[0])<2.5)");    

  print(ch, "2nd leading bjet (MV2c20@77%)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5)");    

  print(ch, "min dPhi(jets,MET)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4)");    

  print(ch, "MET/meff", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25)");    
  print(ch, "mct (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150.)");    
  print(ch, "lepton veto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0)");    
  print(ch, "MET", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250.)");    
  print(ch, "Leading jet pt", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250. && j_pt[0]>130.)");    
  print(ch, "mbb", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250. && j_pt[0]>130. && mjj>200.)");    
  print(ch, "mct (>400)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250. && j_pt[0]>130. && mjj>200. && mct>400.)");    

  print(ch, "mct (>450)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250. && j_pt[0]>130. && mjj>200. && mct>450.)");    
  print(ch, "mct (>500)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutSR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && met["+metFlv+"]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && fabs(j_eta[0])<2.5 && fabs(j_eta[1])<2.5 && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && mct>150. && (eb_N+mb_N)==0 && met["+metFlv+"]>250. && j_pt[0]>130. && mjj>200. && mct>500.)");    


  cout << "-------CR----------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "1 lepton (e or mu)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0) || (e_N==0 && m_N==1)))"); 

  //  ch->Scan("RunNumber:EventNumber", base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0) || (e_N==0 && m_N==1)))");

  print(ch, "pT lepton", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35.) || (e_N==0 && m_N==1 && m_pt[0]>26.)))"); 
  print(ch, "MET 80", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35.) || (e_N==0 && m_N==1 && m_pt[0]>26.)) && met["+metFlv+"]>80.)"); 
  print(ch, "MT", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80.)"); 
  print(ch, "2,3,4 jets", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5)"); 
  print(ch, "pt2", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50.)"); 
  print(ch, "pt4", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50.)"); 
  print(ch, "2 b-jets", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2)"); 
  print(ch, "leading b-jets", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+")"); 
  print(ch, "min dPhi", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && dPhi_min_4jets["+metFlv+"]>0.4)"); 
  print(ch, "met/meff", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25)"); 
  print(ch, "MET120", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && met["+metFlv+"]>120.)"); 
  print(ch, "mbb", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && met["+metFlv+"]>120. && mbb>200.)"); 
  print(ch, "mct", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCutCR+" && "+PVcut+" && !isBadID && isCosmic<2 && !isBadMuon && ((e_N==1 && m_N==0 && e_pt[0]>35. && e_MT_tst_vmu>30. && e_MT_tst_vmu<100.) || (e_N==0 && m_N==1 && m_pt[0]>26. && m_MT_tst_vmu>30. && m_MT_tst_vmu<100.)) && met["+metFlv+"]>80. && j_N>1 && j_N<5 && j_pt[1]>50. && j_pt[3]<50. && "+nbjvar+"==2 && "+btag1+" && "+btag2+" && dPhi_min_4jets["+metFlv+"]>0.4 && met["+metFlv+"]/(met["+metFlv+"]+j_pt[0]+j_pt[1])>0.25 && met["+metFlv+"]>120. && mbb>200. && mct>120.)"); 


}


std::pair<int, float> get(TChain* ch, TString var, TString cut){

  TH1F* hist = new TH1F("hist","",2,0,2);
  ch->Draw(var+">>hist",cut,"goff"); 
  std::pair<int,float> flow = make_pair( (int)hist->GetEntries(), (float)hist->Integral() );
  delete hist;
  return flow;
}

void print(TChain* ch, TString sel, TString var, TString cut){ 
  std::pair<int, float> flow = get(ch, var, cut); 

  if(N_in<0) N_in = flow.first;
  if(N_last<0) N_last = N_in;

  float releff = (N_last!=0 ? flow.first/(float)N_last : 0.);
  float abseff = (N_in!=0 ? flow.first/(float)N_in : 0.);

  std::cout  << std::setw(4)
	     << Form("C%d",NCUT)
	     << "| "
	     << std::setw(45)
             << std::left
             << sel
	     << "| "
             << std::setiosflags(std::ios::fixed)
             << std::setprecision(3)
             << std::setw(18)
             << std::left
             << flow.first
             << "\t"
	     << " | "
             << releff 
             << "\t"
	     << " | "
             << abseff
             << "\t"
	     << " | "
             << flow.second
             << std::endl;

  N_last = flow.first;

  NCUT++;
}
