#include <TChain.h>
#include <TH1F.h>
#include <utility>      // std::pair
#include <iostream>     // std::cout
#include <iomanip>     // std::cout

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

void do_sbottom_cutflow(TString sample=""){
  
  if(sample=="")
    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_TestTruthDC14.root";
  //    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_test_sbottom.root";

  //--- Config
  TString myvar   = "isGRL";
  //TString weights = "(20100*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString weights = "(10000*MC_w*w)"; //*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))*btag_weight_total)";
  TString base    = weights+"*( 1 ";

  TString trigItem = "HLT_xe70";

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
  cout << "---SR----------------------------------------------------------------------------------------------------------------------------" << endl;
  print(ch, "MET (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100.)");  
  print(ch, "1 < Njets < 5", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5)");  
  print(ch, "jpt1>50, |eta|<2.8", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[1])<2.8 && fabs(j_eta[1])<2.8)");  
  print(ch, "jet veto (jpt4<50, |eta|<2.8)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<250. || fabs(j_eta[3])>2.8))");  
  print(ch, "2 leading bjets (70%)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])>2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892)");    

  print(ch, "min dPhi(jets,MET)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4)");    

  print(ch, "MET/meff", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25)");    
  print(ch, "mct (loose)", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150.)");    
  print(ch, "lepton veto", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0)");    
  print(ch, "MET", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250.)");    
  print(ch, "Leading jet pt", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && j_pt[0]>130.)");    
  print(ch, "mbb", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && j_pt[0]>130. && mbb>200.)");    
  print(ch, "mct", myvar, base+" && isGRL && isLarGood && isTileGood && !isTileTrip && !isCoreFlag "+trigCut+" && isVertexOk && !isBadID && !isCosmic && !isBadMuon && met[0]>100. && j_N>1 && j_N<5 && j_pt[1]>50. && fabs(j_eta[0])<2.8 && fabs(j_eta[1])<2.8 && (j_pt[3]<50. || fabs(j_eta[3])<2.8) && j_tag_MV1[0]>0.7892 && j_tag_MV1[1]>0.7892 && dPhi_min_alljets[0]>0.4 && met[0]/meff[0]>0.25 && mct>150. && (eb_N+mb_N)==0 && met[0]>250. && j_pt[0]>130. && mbb>200. && mct>400.)");    

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
