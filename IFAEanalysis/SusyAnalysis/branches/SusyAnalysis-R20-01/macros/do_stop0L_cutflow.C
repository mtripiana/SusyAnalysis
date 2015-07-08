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

void do_stop0L_cutflow(TString sample=""){
  
  if(sample=="")
    sample = "/nfs/at3/scratch/sfracchia/SUSY/METbb_histos/Nom_13TeV_Stop_800_1_cutflow.root";
  //    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_test_sbottom.root";

  //--- Config
  TString myvar   = "j_pt[0]";
  //TString weights = "(20100*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString weights = "(10000*MC_w*w)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString base    = weights+"*( 1 ";

  TString trigItem = "HLT_xe100";

  //load trigger map
  std::vector<TString> trigchains;                                                                                                                                                  
  TString strig = (TString)TFile::Open(sample)->Get("Triggers")->GetTitle();
  trigchains = getTokens(strig, ",");

  std::map<int,TString> trigmap{};
  auto ic=0;
  int trig_idx=-1;
  for(const auto& s : trigchains){
    if(trigItem == s) trig_idx = ic;
    
    trigmap[ic] = s;
    ic++;
  }
  
  TString trigCut = "";
  if(trig_idx>=0) trigCut = Form(" && isTrigger[%d] ", trig_idx);
  else cout << "** WARNING **   Trigger " << trigItem << " not found! Trigger won't be apply for now... " << endl;

  TChain *ch = new TChain("AnalysisTree");
  ch->Add(sample);
   
  cout << "SBOTTOM 0L CUTFLOW " << endl;
  cout << "---------------------------------------------------------------------------------------------------------------------------------" << endl;
  std::cout  << std::setw(45)
             << std::left
             << "Selection"
             << std::setiosflags(std::ios::fixed)
             << std::setprecision(3)
             << std::setw(18)
             << std::left
             << "Nraw"
             << "\t\t"
             << "Rel.Eff."
             << "\t"
             << "Abs.Eff"
             << "\t\t"
             << "Nweighted"
             << std::endl;
  cout << "---presel------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "No sel", myvar, base+" )");
  print(ch, "GRL", myvar, base+" && isGRL )");
  print(ch, "LarGood", myvar, base+" && isGRL && isLarGood)");  
  print(ch, "TileGood", myvar, base+" && isGRL && isLarGood && isTileGood)");  
  print(ch, "TileTrip", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip)");  
  print(ch, "CoreFlag", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag)");  
  print(ch, "Trigger", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+")");  //NO TRIGGER FOR NOW
  print(ch, "Vertex", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk)"); 
  print(ch, "JetCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID)"); 
  print(ch, "CosmicVeto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic)"); 
  print(ch, "MuonCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon)"); 
  cout << "---SRA----------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "Lepton veto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0)"); 
  print(ch, "pt1, pt2>80", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80.)"); 
  print(ch, "MET 250", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250.)"); 
  print(ch, "Jet multiplicity", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5)"); 
  print(ch, "Min dPhi", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5.)"); 
  print(ch, "MET track", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30.)"); 
  
  print(ch, "dPhi MET MET_trk ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3.)"); 
  print(ch, "2 b-tags ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1)"); 
  print(ch, "Tau veto ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0)"); 
  print(ch, "MT_bcl_met ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175.)"); 
  print(ch, "Had top mass 1 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && m_top_had1>50. && m_top_had1<250.)"); 
  print(ch, "Had top mass 2 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400.)"); 
  print(ch, "Min MT ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400. && MT_min_jet_met[11]>50.)"); 
  print(ch, "MET 350 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400. && MT_min_jet_met[11]>50. && met[11]>350.)"); 
  
  cout << "---SRB----------------------------------------------------------------------------------------------------------------------------" << endl; 
  print(ch, "MT_bcl_met ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175.)"); 
  print(ch, "mtasym12 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5)"); 
  print(ch, "m0_antikt12 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5 && m0_antikt12>80.)"); 
  print(ch, "m1_antikt12 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200.)"); 
  print(ch, "m0_antikt08 ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50.)"); 
  print(ch, "MT_lcl_met ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50. && MT_lcl_met[11]>175.)"); 
  print(ch, "HT ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && eb_N==0 && mb_N==0 && j_pt[0]>80. && j_pt[1]>80. && met[11]>250. && j_N>5 && dPhi_min[11]>TMath::Pi()/5. && met[7]>30. && dPhi_met_mettrk[11]<TMath::Pi()/3. && bj_N>1 && j_tau_N[11]==0 && MT_bcl_met[11]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50. && HT>170.)"); 





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

  std::cout  << std::setw(45)
             << std::left
             << sel
             << std::setiosflags(std::ios::fixed)
             << std::setprecision(3)
             << std::setw(18)
             << std::left
             << flow.first
             << "\t\t"
             << releff
             << "\t\t"
             << abseff
             << "\t\t"
             << flow.second
             << std::endl;

  N_last = flow.first;

}
