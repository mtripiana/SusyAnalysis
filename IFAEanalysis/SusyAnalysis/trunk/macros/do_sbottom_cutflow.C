#include <TChain.h>
#include <TH1F.h>
#include <utility>      // std::pair
#include <iostream>     // std::cout
#include <iomanip>     // std::cout

std::pair<int, float> get(TChain* c, TString var, TString cut);
void print(TChain* ch, TString sel, TString var, TString cut);

void do_sbottom_cutflow(TString sample=""){
  
  if(sample=="")
    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_test_sbottom_28.root";
  //    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_test_sbottom.root";

  TChain *ch = new TChain("AnalysisTree");
  ch->Add(sample);

  TString myvar   = "isGRL";
  //TString weights = "(20100*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString weights = "(10000*MC_w*w)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString base    = weights+"*( 1 ";

  
  cout << "SBOTTOM 0L CUTFLOW " << endl;
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
  print(ch, "MET (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100.)");  
  print(ch, "1 < Njets < 5", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5)");  
  print(ch, "jpt1,jpt2>50, |eta|<2.8", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8)");  
  print(ch, "jet veto (jpt4<50, |eta|<2.8)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)>2.8))");  
  print(ch, "2 leading bjets (70%)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)>2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892)");    

  print(ch, "min dPhi(jets,MET)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4)");    

  print(ch, "MET/meff", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25)");    
  print(ch, "mct (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150.)");    
  print(ch, "lepton veto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0)");    
  print(ch, "MET", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250.)");    
  print(ch, "Leading jet pt", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && pt1>130.)");    
  print(ch, "mbb", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && pt1>130. && DiBJet_Mass>200.)");    
  print(ch, "mct", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && n_jets>1 && n_jets<5 && pt2>50. && fabs(eta1)<2.8 && fabs(eta2)<2.8 && (pt4<50. || fabs(eta4)<2.8) && tag_MV1_1>0.7892 && tag_MV1_2>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && pt1>130. && DiBJet_Mass>200. && mct>400.)");    

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
