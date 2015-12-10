/*
 *  DrawUtils.h
 *  
 *
 *  Created by Martin Tripiana on 8/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <TStyle.h>
#include <TLatex.h>
#include <TColor.h>

void SetMyPalette(){
	const Int_t NRGBs = 5;
	const Int_t NCont = 255;
	Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
	//	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.65 };
	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
	Double_t blue[NRGBs]  = { 0.8, 1.00, 0.12, 0.00, 0.00 };
	//	Double_t blue[NRGBs]  = { 0.81, 1.00, 0.12, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
}

void myText(Double_t x,Double_t y,const char *text, Double_t tsize,Color_t color, float angle=0) {
	TLatex l; 
	l.SetTextSize(tsize);
	l.SetNDC();
	l.SetTextColor(color);
	l.SetTextAngle(angle);
	l.DrawLatex(x,y,text);
}


void ATLASLabel(float x, float y, TString text, float luminosity, int color=1, float tsize=0.04, bool drawLumi=true){
  TLatex* l = new TLatex();
  l->SetNDC();
  l->SetTextFont(72);
  l->SetTextColor(color);
  l->SetTextSize(tsize);
  l->DrawLatex(x,y,"ATLAS");
  
  if(drawLumi){
    TLatex* lumi = new TLatex();
    lumi->SetNDC();
    lumi->SetTextSize(tsize*0.8);
    TString lstr = Form("#int L dt = %.1f fb^{-1}, #sqrt{s} = 13 TeV",luminosity);
    lumi->DrawLatex(x, y-0.1, lstr);
  }

  float delx = 0.115*696*gPad->GetWh()/(472*gPad->GetWw());
  if (text){
    TLatex* p = new TLatex();
    p->SetNDC();
    p->SetTextFont(42);
    p->SetTextColor(color);
    p->SetTextSize(tsize);
    p->DrawLatex(x+delx+0.0,y,text);
  }
}

void SetStyle()
{
		//  gStyle->SetPalette(1);                                                                                                         
	SetMyPalette();
	gStyle->SetFrameFillColor(0);
	gStyle->SetFrameBorderSize(0);
	gStyle->SetFrameBorderMode(0);
	//  gStyle->SetFillColor(0);                                                                                                       
	gStyle->SetCanvasColor(0);
	gStyle->SetOptStat(0);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleFillColor(0);
	gStyle->SetTextFont(42);
	gStyle->SetLabelFont(42,"XY");
	gStyle->SetTitleFont(42,"XY");
	gStyle->SetEndErrorSize(0);
}

void SetLegendStyle(TLegend* leg, int fcolor=0, float bsize=0., int fstyle=1001){
	leg->SetFillColor(fcolor);
	leg->SetFillStyle(fstyle);
	leg->SetBorderSize(bsize);
};

template <class T>
void SetHistoStyle(T histo, int mstyle=20, int mcolor=1, float msize=0.9, int fcolor=0, int lstyle=1, int lcolor=1, float lwidth=1){
	
	histo->SetMarkerStyle(mstyle);
	histo->SetMarkerColor(mcolor);
	histo->SetMarkerSize(msize);
	histo->SetFillColor(fcolor);
	histo->SetLineStyle(lstyle);
	histo->SetLineColor(lcolor);
	histo->SetLineWidth(lwidth);
	
};

template <class T>
void SetUpHisto(T histo){
  histo->GetXaxis()->SetTitle("");
  histo->GetXaxis()->SetLabelSize(0);
  
};

template <class T>
void SetDownHisto(T histo){
  histo->GetYaxis()->SetLabelSize(0.025);
  histo->GetYaxis()->SetTitleSize(0.03);
  histo->GetYaxis()->SetTitleOffset(1.2);
  histo->GetYaxis()->SetNdivisions(5);
  histo->GetYaxis()->CenterTitle(true);

  histo->GetYaxis()->SetRangeUser(0,2);
};



void SetPadStyle(TPad* pad, float lmargin, float rmargin, float tmargin, float bmargin, bool logy=false){
  pad->SetTicks();
  if(logy) pad->SetLogy();
  pad->SetLeftMargin(lmargin);
  pad->SetRightMargin(rmargin);
  pad->SetTopMargin(tmargin);
  pad->SetBottomMargin(bmargin);
  pad->SetFillStyle(0);
  pad->SetFillColor(0);
}

void SetAsTopPad(TPad* pad, bool logy=false){
  SetPadStyle(pad,0.15,0.05,0.1,0.30,logy);
}
void SetAsBottomPad(TPad* pad, bool logy=false){
  SetPadStyle(pad,0.15,0.05,0.7,0.1,logy);
}

void NormHisto(TH1F* histo){
  float integral=histo->Integral();
  if(integral>0){
    histo->Scale(1/integral);
  }
};

/* void makeMCtypeHist(TH2F* hist){ */

/*   //as in https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/MCTruthClassifier/trunk/MCTruthClassifier/MCTruthClassifierDefs.h */

/*   const unsigned int ntype = 39; */
/*   char *types[ntype] = {'Unknown', */
/* 			'UnknownElectron', */
/* 			'IsoElectron', */
/* 			'NonIsoElectron', */
/* 			'BkgElectron', */
/* 			'UnknownMuon', */
/* 			'IsoMuon', */
/* 			'NonIsoMuon', */
/* 			'BkgMuon', */
/* 			'UnknownTau', */
/* 			'IsoTau', */
/* 			'NonIsoTau', */
/* 			'BkgTau', */
/* 			'UnknownPhoton',  */
/* 			'IsoPhoton', */
/* 			'NonIsoPhoton', */
/* 			'BkgPhoton', */
/* 			'Hadron', */
/* 			'Neutrino', */
/* 			'NuclFrag', */
/* 			'NonPrimary', */
/* 			'GenParticle', */
/* 			'SUSYParticle', */
/* 			'BBbarMesonPart',    */
/* 			'BottomMesonPart', */
/* 			'CCbarMesonPart', */
/* 			'CharmedMesonPart', */
/* 			'BottomBaryonPart', */
/* 			'CharmedBaryonPart', */
/* 			'StrangeBaryonPart', */
/* 			'LightBaryonPart', */
/* 			'StrangeMesonPart', */
/* 			'LightMesonPart', */
/* 			'BJet', */
/* 			'CJet', */
/* 			'LJet', */
/* 			'GJet', */
/* 			'TauJet', */
/* 			'UnknownJet'}; */
  
  
/*   const unsigned int  norigin=46; */
/*   char *origins[norigin] = {'NonDefined',   */
/* 			    'SingleElec',   */
/* 			    'SingleMuon',   */
/* 			    'SinglePhot',   */
/* 			    'SingleTau',    */
/* 			    'PhotonConv',   */
/* 			    'DalitzDec',    */
/* 			    'ElMagProc',    */
/* 			    'Mu',           */
/* 			    'TauLep', */
/* 			    'top', */
/* 			    'QuarkWeakDec', */
/* 			    'WBoson', */
/* 			    'ZBoson', */
/* 			    'Higgs', */
/* 			    'HiggsMSSM', */
/* 			    'HeavyBoson', */
/* 			    'WBosonLRSM', */
/* 			    'NuREle', */
/* 			    'NuRMu', */
/* 			    'NuRTau', */
/* 			    'LQ', */
/* 			    'SUSY', */
/* 			    'LightMeson', */
/* 			    'StrangeMeson', */
/* 			    'CharmedMeson', */
/* 			    'BottomMeson', */
/* 			    'CCbarMeson', */
/* 			    'JPsi', */
/* 			    'BBbarMeson', */
/* 			    'LightBaryon', */
/* 			    'StrangeBaryon', */
/* 			    'CharmedBaryon', */
/* 			    'BottomBaryonn', */
/* 			    'PionDecay', */
/* 			    'KaonDecay', */
/* 			    'BremPhot', */
/* 			    'PromptPhot', */
/* 			    'UndrPhot', */
/* 			    'ISRPhot', */
/* 			    'FSRPhot', */
/* 			    'NucReact', */
/* 			    'PiZero', */
/* 			    'DiBoson', */
/* 			    'ZorHeavyBoson', */
/* 			    'QCD'};//      = 45 */


/*   //particle type is assumed to be in the Y axis, origin in the X axis */
/*   for (unsigned int i=1; i<=hist->GetNbinsX() && i<norigin; i++) hist->GetXaxis()->SetBinLabel(i,origins[i-1]); */
/*   for (unsigned int i=1; i<=hist->GetNbinsY() && i<ntype; i++) hist->GetYaxis()->SetBinLabel(i,types[i-1]); */
  
/* }    */

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
