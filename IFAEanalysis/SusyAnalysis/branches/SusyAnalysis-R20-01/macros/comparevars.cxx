#include "drawutils.h"

//Dummy values for variables initialization                                                                                             
#define DUMMYUP  99999
#define DUMMYDN -99999

void SetBinning(TString var, int &n, float &min, float &max);
//void draw_var(TString vname, TString file1="", TString file2="", TString label1="", TString label2="",  TString outdir="./", bool loadlib=true);
void draw_var(TString vname, TChain* ch1, TChain* ch2, TString label1="", TString label2="",  TString outdir="./", bool loadlib=true);

int comparevars(TString file1="", TString file2="", TString label1="xAOD", TString label2="NTUP_SUSY", TString outdir="./", TString onlyThis=""){

  cout << "File1  = " << file1 << endl;
  cout << "Label1 = " << label1 << endl;
  cout << "File2  = " << file2 << endl;
  cout << "Label2 = " << label2 << endl;

  if(file1=="") return 0;

  TChain* ch1 = new TChain("AnalysisTree");
  TChain* ch2 = new TChain("AnalysisTree");

  ch1->Add(file1);
  if(file2!="")
    ch2->Add(file2);

  TString vonly = (onlyThis.Index("[")>=0 ? TString(onlyThis(0,onlyThis.Index("["))) : onlyThis);

  TObjArray *leaves  = (TObjArray*) ch1->GetListOfLeaves();
  TObjArray *leaves2;
  TLeaf *leaf;
  TLeaf *leaf2;
  bool loadlib=true;
  for(unsigned int il=0; il < leaves->GetEntriesFast(); il++){ 
    leaf = (TLeaf*)leaves->UncheckedAt(il); 
    
    TString vname = leaf->GetName();

    if(vonly!="" && vonly!=vname) continue;

    bool inboth=false;
    //check if in the other file too
    if(file2!=""){
      leaves2  = (TObjArray*) ch2->GetListOfLeaves();
      for(unsigned int il2=0; il2 < leaves2->GetEntriesFast(); il2++){ 
	leaf2 = (TLeaf*)leaves2->UncheckedAt(il2); 
	if( leaf2->GetName() == vname ){
	  inboth=true;
	  break;
	}
      }
    }
    if(inboth){
      if(vonly!="") vname = onlyThis;
      draw_var(vname, ch1, ch2, label1, label2, outdir, loadlib);
      //draw_var(leaf->GetName(), file1, file2, label1, label2, outdir, loadlib);
      loadlib=false;
    }
    else{
      cout << "VAR '" << vname << "' is not in the second file sorry!" << endl;
      //      draw_var(leaf->GetName(), ch1, ch2, outdir, loadlib);
    }
  }

  // delete leaves;
  // delete leaves2;
  // delete leaf;
  // delete leaf2;

  return 0;
}

void SetBinning(TString var, int &n, float &min, float &max){

  if(var.Contains("_N")){
    n = 15;
    min = 0.;
    max = 15.;
  }

  if(var.Contains("_pt") || var.BeginsWith("met")){
    n = 100;
    min = 0.;
    max = 1500.;
  }

  if(var.Contains("_eta")){
    n = 50;
    min = -5;
    max = 5;
  }

  if(var.Contains("_phi")){
    n = 50;
    min = -3.5;
    max = 3.5;
  }

};

void draw_var(TString vname, TChain* ch1, TChain* ch2, TString label1, TString label2,  TString outdir, bool loadlib){

  bool ok2 = (ch2->GetEntries()>0);

  SetStyle();

  int n;
  float min, max;

  SetBinning(vname, n, min, max);

  TH1F* h1 = new TH1F("h1","",n,min,max);
  TH1F* h2 = new TH1F("h2","",n,min,max);
  h1->SetName("h1");
  h2->SetName("h2");

  TCanvas* c  = new TCanvas("c","",800,600); 
  c->cd();

  TPad* pad1 = new TPad("top","",0,0,1,1,-1,0,0);
  TPad* pad2 = new TPad("bottom","",0,0,1,1,-1,0,0);

  bool dology=false;

  dology=true;

  if(vname=="mjj")
    dology = true;

  if(vname.Contains("ptiso"))
    dology = true;

  if(vname.Contains("_etiso"))
    dology = true;

  if(vname.Contains("m_top"))
    dology = true;

  if(vname.BeginsWith("met") || vname.BeginsWith("HT") || vname.BeginsWith("meff") || vname.BeginsWith("j1_") || vname.BeginsWith("j_pt"))
    dology = true;

  //vectorize!!
  //vname+="[0]";

  SetAsTopPad(pad1, dology);
  SetAsBottomPad(pad2, false);
  pad1->Draw();
  pad2->Draw();

  pad1->cd(); //go to top pad

  TString sel = "("+vname+"!="+Form("%d",DUMMYUP)+" && "+vname+"!="+Form("%d",DUMMYDN)+")";


  //njets requirement!
  //  sel += " && n_jets==4";

  //avoid zero peaks!
  if(vname.BeginsWith("e_") && vname!="e_N")
    sel += " && e_N>0";

  if(vname.BeginsWith("m_") && vname!="m_N")
    sel += " && m_N>0";


  if(vname.BeginsWith("MT_") )
    sel += " && j_N>1 && bj_N>0";

  if(vname.BeginsWith("m0_") || vname.BeginsWith("m1_") || vname.BeginsWith("m_top_"))
    sel += " && j_N>5 && bj_N>1";

  if(vname.BeginsWith("mtasym"))
    sel += " && j_N>3 && bj_N>1";

  if(vname.BeginsWith("met_trk"))
    sel += " && met_trk<500"; 

  // sel += " && e_ptiso30<2500"; //remove some outliers
  // sel += " && m_ptiso30<1000"; //remove some outliers

  //just for now!
  // sel += " && met>120. ";
  //  sel += " && met_lochadtopo>100.";

  //JVF
  if(vname.Contains("j1_jvtxf"))
    sel += " && j1_jvtxf>0";

  //Apply weights  
  //  TString sel2 = "( pileup_w * MC_w) * ("+sel+" && Z_decay==4)";

  // TString sel2 = "( pileup_w * MC_w) * ("+sel+")";
  // sel = "( pileup_w * MC_w) * ("+sel+")";

  TString sel2 = sel;

  cout << "Sel = " << sel << endl;

  TH1F* htmp2;
  ch1->Draw(vname+">>h1",sel,"");
  if(ok2){
    ch2->Draw(vname+">>h2",sel2,"same");
    // ch2.Draw(vname+">>htmp2",sel2,"same");
    // h2 = htmp2;
  } 
  else
    h2 = new TH1F();

  //normalize
  float area1 = h1->Integral();
  if(area1>0) 
    h1->Scale(1./area1);

  TH1F* hratio = (TH1F*)h1->Clone("hratio"); //for the ratio later
  hratio->GetYaxis()->SetRangeUser(-2,4);
  hratio->GetYaxis()->SetTitle("ratio");
  hratio->GetXaxis()->SetTitle(vname);
  hratio->SetLineColor(1);
  hratio->SetTitle("");

  h1->SetLineColor(1);
  h1->SetTitle("");
  h1->GetXaxis()->SetTitle("");
  h1->GetXaxis()->SetLabelSize(0);
  h1->GetYaxis()->SetTitle("arb. units");
  h1->GetYaxis()->SetTitleOffset(1.2);

  h2->SetTitle("");
  h2->GetXaxis()->SetTitle("");
  h2->GetXaxis()->SetLabelSize(0);
 
  h2->GetYaxis()->SetTitle("arb. units");

  if(ok2){
    float area2 = h2->Integral();
    if(area2>0) 
      h2->Scale(1./area2);

    h2->SetLineColor(2);
  
    int bmax1 = h1->GetMaximumBin();
    float max1 = h1->GetBinContent(bmax1) + h1->GetBinError(bmax1);
    int bmax2 = h2->GetMaximumBin();
    float max2 = h2->GetBinContent(bmax2) + h2->GetBinError(bmax2);
    if(max1>max2){
      h1->Draw("hist");
      h2->Draw("histsame");
    }  
    else{
      h2->Draw("hist");
      h1->Draw("histsame");
    }
  }
  else{
    h1->Draw("hist");
  }


  TLegend* leg = new TLegend(0.65,0.7,0.85,0.85);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h1,label1,"L");
  if(ok2) leg->AddEntry(h2,label2,"L");
  leg->Draw();


  pad2->cd(); //go to bottom pad

  //make ratios
  hratio->Divide(h2);

  hratio->Draw();

  c->SaveAs(outdir+"/"+vname+".png");
  c->SaveAs(outdir+"/"+vname+".eps");
  c->SaveAs(outdir+"/"+vname+".C");

  delete leg;
  delete c;
  delete h1;
  if(ok2)  delete h2;
}
