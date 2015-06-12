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
  cout << chainJoin->GetEntries() << endl;
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

void MergeFiles(std::vector<TString> files, TString outfile){
  
  TFileMerger m;
  for(auto& s : files)
    m.AddFile(s);

  m.AddObjectNames("AnalysisTree");
  m.AddObjectNames("Triggers");
  m.AddObjectNames("jOption");
  m.AddObjectNames("METmap");

  m.OutputFile(outfile);
  m.Merge();
}


void MergeSplittedFiles(TString fileName){

  TFileMerger m;
  
  TString preffix = fileName(0,fileName.Sizeof()-6);

  //--- Check how many splitted files are there                                                                                      
  int fileCounter=0;
  bool doesItNotExist=false;
  while (doesItNotExist==false){
    FileStat_t buf;
    fileCounter += 1;
    doesItNotExist = gSystem->GetPathInfo(Form("%s_%d.root", preffix.Data(), fileCounter), buf);
  }
  
  //--- Add splitted files into a file with the preffix joined.                                                                                                                                                                                                                
  std::vector<TString> files; files.clear();
  cout<<Form("Adding file %s", fileName.Data())<<endl;
  files.push_back(fileName);
  for(int count=1; count<fileCounter; count++){
    cout<<Form("Adding file %s_%d.root", preffix.Data(), count)<<endl;    
    files.push_back(Form("%s_%d.root", preffix.Data(), count));
  }		  
  
  MergeFiles(files, Form("%s_joined.root", preffix.Data()) );
  
  cout<<"\nAdded all splitted files into " << Form("%s_joined.root", preffix.Data())<<endl;
  
  
  //--- Delete the files that have been already merged to the joined root file                                                                                                                                                                                                
  gROOT->ProcessLine(Form(".!rm %s", Form("%s", fileName.Data())));
  for(int count=1; count<fileCounter; count++){
    gROOT->ProcessLine(Form(".!rm %s", Form("%s_%d.root", preffix.Data(), count)));
  }
  
  //--- Change the name of the joined file to the final name.                                                                                                                                                                                                                 
  gROOT->ProcessLine(Form(".!mv %s %s", Form("%s_joined.root", preffix.Data()), Form("%s.root", preffix.Data())));
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

  TFile *f3 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f3->Get("AnalysisTree");

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
  w = fabs(average_w) > 0.0 ? 1./average_w : 1.;

  for (Int_t i = 0; i < nentries; i++){
    b_w->Fill();
  }  
  
  t3->Write("",TObject::kOverwrite);
  f3->Close();
}

//---for grid jobs: FileWeight has to be computed
void ComputeNewBranch(TString fileName, float nsim){

  TFile *f4= new TFile(fileName.Data(),"update");
  cout << fileName.Data() << endl;
  TTree *t3 = (TTree*)f4->Get("AnalysisTree");
  

  TBranch *b_xsec;
  TBranch *b_feff;
  TBranch *b_kfactor;
  //TBranch *b_nsim;

  float xsec = 0.0;
  float feff = 0.0;
  float kfactor = 1.0;
  //float nsim = 1.0;

  
  t3->SetBranchAddress("xsec",&xsec,&b_xsec);
  t3->SetBranchAddress("feff",&feff,&b_feff);
  t3->SetBranchAddress("kfactor",&kfactor,&b_kfactor);
  //t3->SetBranchAddress("nsim",&nsim,&b_nsim);


  
  float FileWeight = 1.;

  TBranch *b_FileWeight = t3-> Branch("FileWeight",&FileWeight,"FileWeight/F");
  
  b_xsec->GetEntry(0);
  b_feff->GetEntry(0);
  b_kfactor->GetEntry(0);
  //b_nsim->GetEntry(0);

  FileWeight = xsec*feff*kfactor/nsim;
    
  for (int i = 0; i< t3->GetEntries();i++){   
    b_FileWeight->Fill();
  }
  t3->Write("",TObject::kOverwrite);
  f4->Close();
}


void addAntiWeightToTree(TString fileName, bool isData){

  //  Info("run_chorizo::addAntiWeightToTree", Form("Adding anti weights for the lepton efficiency into the AnalysisTree."));                                                          

  TFile *f5 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f5->Get("AnalysisTree");

  float e_antiSF=1.;
  float m_antiSF=1.;
  float ph_antiSF=1.;

  TBranch *b_e_antiSF = t3-> Branch("e_antiSF",&e_antiSF,"e_antiSF/F");
  TBranch *b_m_antiSF = t3-> Branch("m_antiSF",&m_antiSF,"m_antiSF/F");
  TBranch *b_ph_antiSF = t3-> Branch("ph_antiSF",&ph_antiSF,"ph_antiSF/F");

  TBranch *b_e_SF;
  TBranch *b_m_SF;
  TBranch *b_ph_SF;
  TBranch *b_e_N;
  TBranch *b_m_N;
  TBranch *b_ph_N;
  TBranch *b_w;
  TBranch *b_MC_w;
  TBranch *b_pileup_w;

  float e_SF_loc=1.;
  float m_SF_loc=1.;
  float ph_SF_loc=1.;
  int   e_N_loc=0;
  int   m_N_loc=0;
  int   ph_N_loc=0;
  float w_loc=1.;
  float MC_w_loc=1.;
  float pileup_w_loc=1.;

  t3->SetBranchAddress("e_SF", &e_SF_loc, &b_e_SF);
  t3->SetBranchAddress("m_SF", &m_SF_loc, &b_m_SF);
  t3->SetBranchAddress("ph_SF", &ph_SF_loc, &b_ph_SF);
  t3->SetBranchAddress("e_N", &e_N_loc, &b_e_N);
  t3->SetBranchAddress("m_N", &m_N_loc, &b_m_N);
  t3->SetBranchAddress("ph_N", &ph_N_loc, &b_ph_N);
  t3->SetBranchAddress("w", &w_loc, &b_w);
  t3->SetBranchAddress("MC_w", &MC_w_loc, &b_MC_w);
  t3->SetBranchAddress("pileup_w", &pileup_w_loc, &b_pileup_w);

  Float_t EventsWithElec = 0.;
  Float_t EventsWithMuon = 0.;
  Float_t EventsWithPhoton = 0.;
  Float_t TotalEvents = 0.;
  Int_t nentries = (Int_t)t3->GetEntries();

  for (Int_t i = 0; i < nentries; i++){
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    b_ph_N->GetEntry(i);
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
    if(ph_N_loc>0){
      EventsWithPhoton += w_loc*MC_w_loc*pileup_w_loc;
    }

  }

  std::cout<<"Total events with electrons : "<<EventsWithElec<<std::endl;
  std::cout<<"Total events with muons     : "<<EventsWithMuon<<std::endl;
  std::cout<<"Total events with photons   : "<<EventsWithPhoton<<std::endl;
  std::cout<<"Total events: "<<TotalEvents<<std::endl;

  Float_t e_SF_mean=0.;
  Float_t m_SF_mean=0.;
  Float_t ph_SF_mean=0.;
  TH1F *h_e_SF_loc = new TH1F("h_e_SF_loc", "h_e_SF_loc", 400,0.0,2.0);
  TH1F *h_m_SF_loc = new TH1F("h_m_SF_loc", "h_m_SF_loc", 400,0.0,2.0);
  TH1F *h_ph_SF_loc = new TH1F("h_ph_SF_loc", "h_ph_SF_loc", 400,0.0,2.0);

  for (Int_t i = 0; i < nentries; i++){
    b_e_SF->GetEntry(i);
    b_m_SF->GetEntry(i);
    b_ph_SF->GetEntry(i);
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    b_ph_N->GetEntry(i);
    b_w->GetEntry(i);
    b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    if(e_N_loc>0) {h_e_SF_loc->Fill(e_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    if(m_N_loc>0) {h_m_SF_loc->Fill(m_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    if(ph_N_loc>0) {h_ph_SF_loc->Fill(ph_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
  }
  e_SF_mean = h_e_SF_loc->GetMean(); //--- Both have w, so it cancels.                                                                                                               
  m_SF_mean = h_m_SF_loc->GetMean();
  ph_SF_mean = h_ph_SF_loc->GetMean();

  h_e_SF_loc->Delete();
  h_m_SF_loc->Delete();
  h_ph_SF_loc->Delete();

  if(e_SF_mean==0.) { e_SF_mean=1.;} //--- Just in case                                                                                                                              
  if(m_SF_mean==0.) { m_SF_mean=1.;}
  if(ph_SF_mean==0.) { ph_SF_mean=1.;}

  std::cout<<"Mean electron SF : "<<e_SF_mean<<std::endl;
  std::cout<<"Mean muon SF     : "<<m_SF_mean<<std::endl;
  std::cout<<"Mean photon SF   : "<<ph_SF_mean<<std::endl;

  std::cout<<"Mean electron SF : "<<e_SF_mean<<std::endl;
  std::cout<<"Mean muon SF     : "<<m_SF_mean<<std::endl;
  std::cout<<"Mean photon SF   : "<<ph_SF_mean<<std::endl;

  if (!isData){
    e_antiSF = 1 + (1-e_SF_mean)*((Float_t)EventsWithElec/((Float_t)TotalEvents-(Float_t)EventsWithElec));
    m_antiSF = 1 + (1-m_SF_mean)*((Float_t)EventsWithMuon/((Float_t)TotalEvents-(Float_t)EventsWithMuon));
    ph_antiSF = 1 + (1-ph_SF_mean)*((Float_t)EventsWithPhoton/((Float_t)TotalEvents-(Float_t)EventsWithPhoton));

    if((e_antiSF < -1000 || e_antiSF > 1000) || (TotalEvents-EventsWithElec==0)) {
      e_antiSF=1; //--- To avoid nan or inf                                                                                                                                          
      std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    }
    if((m_antiSF < -1000 || m_antiSF > 1000) || (TotalEvents-EventsWithMuon==0)) {
      m_antiSF=1;
      std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    }
    if((ph_antiSF < -1000 || ph_antiSF > 1000) || (TotalEvents-EventsWithPhoton==0)) {
      ph_antiSF=1;
      std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    }
  }
  else{
    e_antiSF = 1;
    m_antiSF = 1;
    ph_antiSF = 1;
  }

  std::cout<<"Electron anti-SF : "<<e_antiSF<<std::endl;
  std::cout<<"Muon anti-SF     : "<<m_antiSF<<std::endl;
  std::cout<<"Photon anti-SF   : "<<ph_antiSF<<std::endl;

  for (Int_t i = 0; i < nentries; i++){
    b_e_antiSF->Fill();
    b_m_antiSF->Fill();
    b_ph_antiSF->Fill();
  }

  t3->Write("",TObject::kOverwrite);
  f5->Close();
}



void tadd(std::vector< TString> filelist, std::vector< Double_t> weights, TString outfile, bool isData ){

  //--- Join all splitted files
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Joining splitted files..."<<endl;
    //JoinSplittedFiles(filelist.at(i));
    MergeSplittedFiles(filelist.at(i));
  }

  //--- Add some new branches
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding new branches..."<<endl;
    AddNewBranch(filelist.at(i), weights.at(i));
    addAverageWeight(filelist.at(i));
  }

  
  //--- Join the "joined" files in a single root file. Add also FileWeight branch
  MergeFiles(filelist, outfile.Data());

  //--- Remove file in the Collateral folder
  for(unsigned int i=0; i<filelist.size(); i++){
    gROOT->ProcessLine(Form(".! rm %s", filelist.at(i).Data()));
  }

  cout<<"\nAdding anti_e_SF and anti_m_SF"<<endl;
  addAntiWeightToTree(outfile.Data(), isData);  

  cout << endl;
  cout << bold("Target file : \n                    ") << outfile  << endl;
  cout << endl;
}

void tadd_grid(std::vector< TString> filelist, TString outfile, bool isData ){
  
  //--- Join the "joined" files in a single root file. Add also FileWeight branch
  TChain *chain = new TChain("AnalysisTree");
    
  float nsim_total = 0;
  
  for(unsigned int i=0; i<filelist.size(); i++){
  
    TFile f1(filelist.at(i));
    TH1F *histo_nsim = (TH1F*)f1.Get("h_presel_flow");
    nsim_total+=histo_nsim->GetBinContent(1);
    delete histo_nsim;
    
    cout<<"Adding file: "<<filelist.at(i)<<endl;
    chain->Add(filelist.at(i));
  }
    cout<<"Total events for this sample: "<<nsim_total<<endl;


  chain->SetMaxTreeSize(15000000000LL);
  chain->Merge(outfile.Data());

  //--- Remove file in the Collateral folder
  //for(unsigned int i=0; i<filelist.size(); i++){
  //  gROOT->ProcessLine(Form(".! rm %s", filelist.at(i).Data()));
  //}

  cout<<"\nAdding FileWeight"<<endl;  
  cout<<"\nAdding average weight w"<<endl;  
  cout<<"\nAdding anti_e_SF and anti_m_SF"<<endl;
  ComputeNewBranch(outfile.Data(),nsim_total);
  addAverageWeight(outfile.Data());
  addAntiWeightToTree(outfile.Data(), isData);  

  cout << endl;
  cout << bold("Target file : ") << outfile  << endl;
  cout << endl;
}


void addMetaData(TString rootName, TString histName, TString outName){

  //MetaData (aka all TNamed objects)
  TNamed* Triggers = (TNamed*) TFile::Open(histName)->Get("Triggers")->Clone("Triggers");
  TNamed* jOption = (TNamed*) TFile::Open(histName)->Get("jOption")->Clone("jOption");
  TNamed* METmap = (TNamed*) TFile::Open(histName)->Get("METmap")->Clone("METmap");

  //analysis tree  
  TFile* newfile = new TFile(rootName, "update");

  Triggers->Write();
  jOption->Write();
  METmap->Write();
  
  newfile->Close();
}
