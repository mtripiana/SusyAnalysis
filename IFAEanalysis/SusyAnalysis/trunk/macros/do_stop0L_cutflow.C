#include <TChain.h>
#include <TH1F.h>
#include <utility>      // std::pair
#include <iostream>     // std::cout
#include <iomanip>     // std::cout

std::pair<int, float> get(TChain* c, TString var, TString cut);
void print(TChain* ch, TString sel, TString var, TString cut);

void do_stop0L_cutflow(TString sample=""){
  
  if(sample=="")
    sample = "/nfs/at3/scratch/arodriguez/StopAnalysis/Nom_test_stop.root";

  TChain *ch = new TChain("AnalysisTree");
  ch->Add(sample);

  TString myvar   = "isGRL";
  //TString weights = "(20100*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString weights = "(10000*MC_w*w)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString base    = weights+"*( 1 ";

  
  cout << "STOP 0L CUTFLOW " << endl;
  //  cout << "-----------------------------------------------------------------------------------------------------" << endl;
  cout << "---presel--------------------------------------------------------------------------------------------" << endl;
  print(ch, "No sel", myvar, base+" )");
  print(ch, "GRL", myvar, base+" && isGRL )");
  print(ch, "LarGood", myvar, base+" && isGRL && isLarGood)");  
  print(ch, "TileGood", myvar, base+" && isGRL && isLarGood && isTileGood)");  
  print(ch, "TileTrip", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip)");  
  print(ch, "CoreFlag", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag)");  
  print(ch, "Trigger", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag)");  //NO TRIGGER FOR NOW
  print(ch, "Vertex", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk)"); 
  print(ch, "JetCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID)"); 
  print(ch, "CosmicVeto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic)"); 
  print(ch, "MuonCleaning", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon)"); 
  cout << "---SR------------------------------------------------------------------------------------------------" << endl;
  print(ch, "LeptonVeto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0)");   
  print(ch, "TrigMatch", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0)");   
  print(ch, "SignalJets (pt1,2>80, |eta|<2.8)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80.)");   
  print(ch, "MET", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250.)");   
  print(ch, "N jets", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5)");   
  print(ch, "dPhiMetJetMin3", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.))");   
  print(ch, "dPhiMetTrackMet", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80.  && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3.))");   
  print(ch, "2 btags", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3.) && n_bjets>1)");   
  print(ch, "tau-veto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3.) && n_bjets>1 && n_taujets[1]==0)");   
  cout << "---SRA-----------------------------------------------------------------------------------------------" << endl;
  print(ch, "Mt", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175.)");   
  print(ch, "TopDRminB0", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.)) && dPhi_met_mettrk[1]<(TMath::Pi()/3 && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && m_top_had1>50. && m_top_had1<250.)");   
  print(ch, "TopDRminB1", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400.)");   
  print(ch, "minMt", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400. && MT_min_jet_met[1]>50.)");   
  print(ch, "highMET", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && m_top_had1>50. && m_top_had1<250. && m_top_had2>50. && m_top_had2<400. && MT_min_jet_met[1]>50. && met[1]>350.)");   
  cout << "---SRB-----------------------------------------------------------------------------------------------" << endl;
  print(ch, "Mt (as above)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175.)");
  print(ch, "AntiKt12MAsymmetry ", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 )");   
  print(ch, "AntiKt12M0>80", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 && m0_antikt12>80.)");   
  print(ch, "60<AntiKt12M1<200", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200.)");   
  print(ch, "Antikt8M>50", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50.)");
  print(ch, "MtNonBMin>175", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50. && MT_lcl_met[1]>175.)");
  print(ch, "HT", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && (eb_N+mb_N)==0 && pt1>80. && pt2>80. && met[1]>250. && n_jets>5 && dPhi_min[1]>(TMath::Pi()/5.) && dPhi_met_mettrk[1]<(TMath::Pi()/3) && n_bjets>1 && n_taujets[1]==0 && MT_bcl_met[1]>175. && mtasym12<0.5 && m0_antikt12>80. && m1_antikt12>60. && m1_antikt12<200. && m0_antikt08>50. && MT_lcl_met[1]>175. && met[1]/sqrt(HT)>17.)");



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
  std::cout  << std::setw(45)
	     << std::left
	     << sel
	     << std::setiosflags(std::ios::fixed)
	     << std::setprecision(3)
	     << std::setw(18)
	     << std::left
	     << flow.first
	     << "\t\t" 
	     << flow.second 
	     << std::endl;
  //cout << sel << "     = " << flow.first << "\t\t" << flow.second << endl;
}
