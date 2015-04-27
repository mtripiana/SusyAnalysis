#include <TChain.h>
#include <utility>      // std::pair
#include <iostream>     // std::cout


std::pair<int, float> get(TChain* c, TString var, TString cut);
void print(TChain* ch, TString sel, TString var, TString cut);

void do_zll_cutflow(TString sample=""){

  //Zmumu=0.02174858634
  //Zee=0.02192982456

  if (sample=="")
    sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14/Nom_TestDataB.root";
  
  //  TString sample = "/nfs/at3/scratch/sfracchia/SUSY/METbb_histos/Nom_Herwigpp_UEEE3_CTEQ6L1_Tt_T600_L1_tree.root";
  //  TString sample = "/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/triggerTest/Nom_Herwigpp_UEEE3_CTEQ6L1_Tt_T600_L1_tree.root";

  TChain *ch = new TChain("AnalysisTree");
  ch->Add(sample);


  cout << ch->GetEntries() << endl;

  //** MC
  //  TString baseline = "(((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0))))*(passPreselectionCuts";

  //** Data
  TString base = "(20300*w*FileWeight*MC_w*pileup_w*ttbar_weight*((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))*((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))) * ( 1 ";

  std::pair<int,float> flow;
  cout << "---presel--------------------------------------------------" << endl;
  flow = print(ch, "No sel", "met", base+" )");
  flow = print(ch, "GRL", "met", base+" && isGRL )");
  flow = print(ch, "Trigger", "met", base+" && isGRL && isTrigger)");
  flow = print(ch, "Vertex", "met", base+" && isGRL && isTrigger && isVertexOk)");  
  flow = print(ch, "LarGood", "met", base+" && isGRL && isTrigger && isVertexOk && isLarGood)");  
  flow = print(ch, "TileGood", "met", base+" && isGRL && isTrigger && isVertexOk && isLarGood && isTileGood)");  
  flow = print(ch, "CoreFlag", "met", base+" && isGRL && isTrigger && isVertexOk && isLarGood && isTileGood && isCoreFlag)");  
  flow = print(ch, "BadJetID", "met", base+" && isGRL && isTrigger && isVertexOk && isLarGood && isTileGood && isCoreFlag && !isBadID)");  
  flow = print(ch, "TileTrip", "met", base+" && isGRL && isTrigger && isVertexOk && isLarGood && isTileGood && isCoreFlag && !isBadID && !isTileTrip)");  

  cout << "---electron selection -------------------------------------" << endl;
  flow = print(ch, "Baseline", "met", base+" && passPreselectionCuts  && !isBadID && !isTileTrip)");
  flow = print(ch, "Electrons", "met", base+" && passPreselectionCuts  && !isBadID && !isTileTrip && e_N==2)");
  flow = print(ch, "Muon Veto", "met", base+" && passPreselectionCuts  && !isBadID && !isTileTrip && e_N==2 && m_N==0)");
  
  cout << "---muon selection -------------------------------------" << endl;
  flow = print(ch, "Baseline", "met", base+" && passPreselectionCuts)  && !isBadID && !isTileTrip");
  flow = print(ch, "Muons", "met", base+" && passPreselectionCuts && !isBadID && !isTileTrip && m_N==2)");
  flow = print(ch, "Electron veto", "met", base+" && passPreselectionCuts && !isBadID && !isTileTrip && m_N==2 && e_N==0)");

}

std::pair<int, float> get(TChain* ch, TString var, TString cut){

  ch->Draw(var+">>hist",cut,"goff"); 
  std::pair<int,float> flow = make_pair( (int)hist->GetEntries(), (float)hist->Integral() );

  return flow;
}

void print(TChain* ch, TString sel, TString var, TString cut){ 
  std::pair<int, float> flow = get(ch, var, cut); 
  cout << sel << "     = " << flow.first << "\t\t" << flow.second << endl;
}
