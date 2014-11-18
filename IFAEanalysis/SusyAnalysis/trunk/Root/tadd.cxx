#include "SusyAnalysis/tadd.h"
#include "SusyAnalysis/utility.h"

void JoinSplittedFiles(TString fileName){
  TString preffix = fileName(0,fileName.Sizeof()-6);
  TChain *chainJoin = new TChain("AnalysisTree");
  chainJoin->SetMaxTreeSize(15000000000LL);

  //--- Check how many splitted files are there
  int fileCounter=0;
  bool doesItNotExist=false;
  while (doesItNotExist==false){
    FileStat_t buf;
    fileCounter += 1;
    doesItNotExist = gSystem->GetPathInfo(Form("%s_%d.root", preffix.Data(), fileCounter), buf);
  }

  //--- Add splitted files into a file with the preffix joined.
  cout<<Form("Adding file %s", fileName.Data())<<endl;
  chainJoin->Add(fileName);
  for(int count=1; count<fileCounter; count++){
    cout<<Form("Adding file %s_%d.root", preffix.Data(), count)<<endl;
    chainJoin->Add(Form("%s_%d.root", preffix.Data(), count));
  }
  if (chainJoin->GetEntries()>0){
    cout<<"\nAdded all splitted files into "<<Form("%s_joined.root", preffix.Data())<<endl;
    chainJoin->Merge(Form("%s_joined.root", preffix.Data()));

    //--- Delete the files that have been already merged to the joined root file
    gROOT->ProcessLine(Form(".!rm %s", Form("%s", fileName.Data())));
    for(int count=1; count<fileCounter; count++){
      gROOT->ProcessLine(Form(".!rm %s", Form("%s_%d.root", preffix.Data(), count)));
    }

    //--- Change the name of the joined file to the final name.
    gROOT->ProcessLine(Form(".!mv %s %s", Form("%s_joined.root", preffix.Data()), Form("%s.root", preffix.Data())));
  }
  chainJoin->Delete();
}

//--- Example from http://root.cern.ch/download/doc/ROOTUsersGuideHTML/ch12s16.html

void AddNewBranch(TString fileName, Float_t FileWeight){
  TFile *f = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f->Get("AnalysisTree");
  TBranch *newBranch = t3-> Branch("FileWeight",&FileWeight,"FileWeight/F");
  //read the number of entries in the t3
  Int_t nentries = (Int_t)t3->GetEntries();
  for (Int_t i = 0; i < nentries; i++){
    newBranch->Fill();
  }
  t3->Write("",TObject::kOverwrite);     // save only the new version of the tree
  f->Close();
}

void addAverageWeight(TString fileName){

//  Info("run_chorizo::addAntiWeightToTree", Form("Adding anti weights for the lepton efficiency into the AnalysisTree."));

  TFile *f2 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f2->Get("AnalysisTree");

  float average_w=1.;
  float w=1.;

  TBranch *b_w = t3-> Branch("w",&w,"w/F");

  TBranch *b_MC_w;
  TBranch *b_pileup_w;

  float MC_w_loc=1.;
  float pileup_w_loc=1.;

  t3->SetBranchAddress("MC_w", &MC_w_loc, &b_MC_w);
  t3->SetBranchAddress("pileup_w", &pileup_w_loc, &b_pileup_w);

  Float_t TotalEvents = 0.;
  Int_t nentries = (Int_t)t3->GetEntries();

  for (Int_t i = 0; i < nentries; i++){
    b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    TotalEvents += MC_w_loc*pileup_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global. 

  }

  cout<<"Total events: "<<TotalEvents<<endl;
  
  average_w = TotalEvents/nentries;
  w = 1./average_w;

  for (Int_t i = 0; i < nentries; i++){
    b_w->Fill();
  }  
  

  t3->Write("",TObject::kOverwrite);
  f2->Close();
}


void addAntiWeightToTree(TString fileName, bool isData){

//  Info("run_chorizo::addAntiWeightToTree", Form("Adding anti weights for the lepton efficiency into the AnalysisTree."));

  TFile *f2 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f2->Get("AnalysisTree");

  float e_antiSF=1.;
  float m_antiSF=1.;

  TBranch *b_e_antiSF = t3-> Branch("e_antiSF",&e_antiSF,"e_antiSF/F");
  TBranch *b_m_antiSF = t3-> Branch("m_antiSF",&m_antiSF,"m_antiSF/F");

  TBranch *b_e_SF;
  TBranch *b_m_SF;
  TBranch *b_e_N;
  TBranch *b_m_N;
  TBranch *b_w;
  TBranch *b_MC_w;
  TBranch *b_pileup_w;

  float e_SF_loc=1.;
  float m_SF_loc=1.; 
  int   e_N_loc=0;
  int   m_N_loc=0;
  float w_loc=1.;
  float MC_w_loc=1.;
  float pileup_w_loc=1.;

  t3->SetBranchAddress("e_SF", &e_SF_loc, &b_e_SF);
  t3->SetBranchAddress("m_SF", &m_SF_loc, &b_m_SF);
  t3->SetBranchAddress("e_N", &e_N_loc, &b_e_N);
  t3->SetBranchAddress("m_N", &m_N_loc, &b_m_N);
  t3->SetBranchAddress("w", &w_loc, &b_w);
  t3->SetBranchAddress("MC_w", &MC_w_loc, &b_MC_w);
  t3->SetBranchAddress("pileup_w", &pileup_w_loc, &b_pileup_w);

  Float_t EventsWithElec = 0.;
  Float_t EventsWithMuon = 0.;
  Float_t TotalEvents = 0.;
  Int_t nentries = (Int_t)t3->GetEntries();

  for (Int_t i = 0; i < nentries; i++){
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    b_w->GetEntry(i);
    b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    TotalEvents += w_loc*MC_w_loc*pileup_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global. 
    if(e_N_loc>0){
      EventsWithElec += w_loc*MC_w_loc*pileup_w_loc;
    }
    if(m_N_loc>0){
      EventsWithMuon += w_loc*MC_w_loc*pileup_w_loc;
    }
  }

  cout<<"Total events with electrons: "<<EventsWithElec<<endl;
  cout<<"Total events with Muons: "<<EventsWithMuon<<endl;
  cout<<"Total events: "<<TotalEvents<<endl;

  Float_t e_SF_mean=0.;
  Float_t m_SF_mean=0.;
  TH1F *h_e_SF_loc = new TH1F("h_e_SF_loc", "h_e_SF_loc", 400,0.0,2.0);
  TH1F *h_m_SF_loc = new TH1F("h_m_SF_loc", "h_m_SF_loc", 400,0.0,2.0);

  for (Int_t i = 0; i < nentries; i++){
    b_e_SF->GetEntry(i);
    b_m_SF->GetEntry(i);
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    b_w->GetEntry(i);
    b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    if(e_N_loc>0) {h_e_SF_loc->Fill(e_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    if(m_N_loc>0) {h_m_SF_loc->Fill(m_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
  }
  e_SF_mean = h_e_SF_loc->GetMean(); //--- Both have w, so it cancels.
  m_SF_mean = h_m_SF_loc->GetMean();

  h_e_SF_loc->Delete();
  h_m_SF_loc->Delete();

  if(e_SF_mean==0.) { e_SF_mean=1.;} //--- Just in case
  if(m_SF_mean==0.) { m_SF_mean=1.;}

  cout<<"Mean electron SF: "<<e_SF_mean<<endl;
  cout<<"Mean muon SF: "<<m_SF_mean<<endl;

  if (!isData){
    e_antiSF = 1 + (1-e_SF_mean)*((Float_t)EventsWithElec/((Float_t)TotalEvents-(Float_t)EventsWithElec));
    m_antiSF = 1 + (1-m_SF_mean)*((Float_t)EventsWithMuon/((Float_t)TotalEvents-(Float_t)EventsWithMuon));

    if((e_antiSF < -1000 || e_antiSF > 1000) || (TotalEvents-EventsWithElec==0)) {
      e_antiSF=1; //--- To avoid nan or inf
      cout<<"nan or inf found. I'll correct it."<<endl;
    }
    if((m_antiSF < -1000 || m_antiSF > 1000) || (TotalEvents-EventsWithMuon==0)) {
      m_antiSF=1;
      cout<<"nan or inf found. I'll correct it."<<endl;
    }
  }
  else{
    e_antiSF = 1;
    m_antiSF = 1;
  }

  cout<<"Electron anti-SF: "<<e_antiSF<<endl;
  cout<<"Muon anti-SF: "<<m_antiSF<<endl;

  for (Int_t i = 0; i < nentries; i++){
    b_e_antiSF->Fill();
    b_m_antiSF->Fill();
  }

  t3->Write("",TObject::kOverwrite);
//  f2->Close;
}

void tadd(std::vector< TString> filelist, vector< Double_t> weights, TString outfile, bool isData ){

  //--- Join all splitted files
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Joining splitted files..."<<endl;
    JoinSplittedFiles(filelist.at(i));
  }

  //--- Add some new branches
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding new branches..."<<endl;
    AddNewBranch(filelist.at(i), weights.at(i));
    addAverageWeight(filelist.at(i));
  }

  
  //--- Join the "joined" files in a single root file. Add also FileWeight branch
  TChain *chain = new TChain("AnalysisTree");
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding file: "<<filelist.at(i)<<" with weight : "<<weights.at(i)<<endl;
    chain->Add(filelist.at(i));
  }

  chain->SetMaxTreeSize(15000000000LL);
  chain->Merge(outfile.Data());

  //--- Remove file in the Collateral folder
  for(unsigned int i=0; i<filelist.size(); i++){
    gROOT->ProcessLine(Form(".! rm %s", filelist.at(i).Data()));
  }

  cout<<"\nAdding anti_e_SF and anti_m_SF"<<endl;
  addAntiWeightToTree(outfile.Data(), isData);  

  cout << endl;
  cout << bold("Target file : ") << outfile  << endl;
  cout << endl;
}
