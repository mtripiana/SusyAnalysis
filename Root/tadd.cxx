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
 
  TFileMerger m(false);
  //  m.OutputFile(outfile);

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

void AddNewBranch(TString fileName, Float_t FileWeight, float nsimAOD, float sumwAOD){
  TFile *f = new TFile(fileName.Data(),"update");

  if(!f->GetListOfKeys()->Contains("AnalysisTree")){
    cout << "WARNING no AnalysisTree found !! Please check" << endl;
    f->Close();
  }

  TTree *t3 = (TTree*)f->Get("AnalysisTree");
  Int_t nentries = (Int_t)t3->GetEntries();

  if(nentries){
    //TBranch *newBranch = t3-> Branch("FileWeight",&FileWeight,"FileWeight/F");
    TBranch *b_nsimAOD = t3-> Branch("nsimAOD",&nsimAOD,"nsimAOD/F"); 
    TBranch *b_sumwAOD = t3-> Branch("sumwAOD",&sumwAOD,"sumwAOD/F");        
    //read the number of entries in the t3
    Int_t nentries = (Int_t)t3->GetEntries();
    for (Int_t i = 0; i < nentries; i++){
      //newBranch->Fill();
      b_nsimAOD->Fill();       
      b_sumwAOD->Fill();     
    }
    t3->Write("",TObject::kOverwrite);     // save only the new version of the tree
  }

  f->Close();
}

void findDuplicates(TString fileName, float nsim, float sumw, bool isData){
  TFile *f3 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f3->Get("AnalysisTree");
 
  Int_t nentries = (Int_t)t3->GetEntries();
 
  //set to store the list of duplicate entries
  set<unsigned long long> dups;
  set<unsigned long long> type4;  
  //pair containing the eventnumber, truth variable 
  set<std::pair<unsigned long long, float> > eventIds;
  set<std::pair<unsigned long long, float> > eventIds_reco;  
  
  unsigned long long event;
  float truth_pt;
  float MCT;
  unsigned long long ndup=0;
  float wdup=0;  
  double MC_w;
  
  TBranch *b_event;
  TBranch *b_truth_pt;
  TBranch *b_MCT;  
  TBranch *b_MCw;  
  
  t3->SetBranchAddress("EventNumber",&event, &b_event);
  t3->SetBranchAddress("j_truth_pt1",&truth_pt, &b_truth_pt); 
  t3->SetBranchAddress("MC_w",&MC_w, &b_MCw); 
  t3->SetBranchAddress("mct",&MCT, &b_MCT);   
  
  bool isDuplicated = false; 
  bool isDup =false;
  bool isType4=false;
  float nsim_dupcorr;
  float sumw_dupcorr;
  
  if (isData) {
    nsim_dupcorr=1.;
    sumw_dupcorr=1.;       
  }
  else {
    nsim_dupcorr=nsim;
    sumw_dupcorr=sumw;       
  }  
  
    
  std::pair<unsigned long long,float> my_pair;
  std::pair<unsigned long long,float> my_pair_reco;  
   
  
  TBranch *b_dup = t3-> Branch("isDuplicated",&isDuplicated,"isDuplicated/O");
  TBranch *b_dup_type4 = t3-> Branch("isDuplicated_type4",&isType4,"isType4/O");
 
  TBranch *b_nsim_dupcorr = t3-> Branch("nsimAOD_dupcorr",&nsim_dupcorr,"nsimAOD_dupcorr/F");  
  TBranch *b_sumw_dupcorr = t3-> Branch("sumwAOD_dupcorr",&sumw_dupcorr,"sumwAOD_dupcorr/F");  
     
   for(unsigned long long j=0;j<nentries;j++)
     {
       b_event->GetEntry(j);
       b_truth_pt->GetEntry(j);
       b_MCw->GetEntry(j);     
       b_MCT->GetEntry(j);         
   
       
       //if(int(10*j/nentries) > flast)
       //  {
       //    cout << j << " / " << nentries <<  " ( " << 100*j/nentries << " % ) checked for duplicates " <<   endl;
       //    flast=int(10*j/nentries);
       //  }
       if (!isData) {
         my_pair = std::make_pair(event,truth_pt);
         my_pair_reco = std::make_pair(event,MCT);	 
       }
       else my_pair = std::make_pair(event,MCT);
       // check to see if an eventnumber with a met value has already been seen
       if(eventIds.count(my_pair) ==0)
         {
	   // record the eventnumber and met of that event if not seen before
           eventIds.insert(my_pair);
         }
       else
         {
           // if already seen, then duplicate event!!!!
           //cout << event << " " << truth_pt << endl;	   
           dups.insert(j);
	   isDup=true;
           std::cout << "duplicate at entry.." << j << " " << event << " " << truth_pt << std::endl;

         }
        
	if (!isData){
	   
	   if(eventIds_reco.count(my_pair_reco) ==0){	
              
	      eventIds_reco.insert(my_pair_reco);
	      if (isDup){  
	        
	        type4.insert(j);
	        if (!isData){
	          ndup++;
	          wdup+=MC_w;
	   
	       } 

	      }

	    }
	 }  
      }  

   for(unsigned long long j=0;j<nentries;j++)
     {
         isDuplicated = dups.count(j);
	 isType4 = type4.count(j);	
	 
	 if(isDuplicated && isType4) {
	    
          nsim_dupcorr=nsim - ndup;
          sumw_dupcorr=sumw -wdup; 	    
	   
	 }
	  
	 b_dup->Fill();
	 b_dup_type4->Fill();	 
	 b_nsim_dupcorr->Fill();
	 b_sumw_dupcorr->Fill();
	 	 
     }
      

  t3->Write("",TObject::kOverwrite);
  f3->Close();     
     
}

void addAverageWeight(TString fileName){

  TFile *f3 = new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f3->Get("AnalysisTree");

  float average_w=1.;
  float w=1.;
  
  //float average_w_noPU=1.;
  //float w_noPU=1.;  

  TBranch *b_w = t3-> Branch("w",&w,"w/F");
  //TBranch *b_w_noPU = t3-> Branch("w_noPU",&w_noPU,"w_noPU/F");  

  //TBranch *b_MC_w;
  TBranch *b_pileup_w;

  //float MC_w_loc=1.;
  float pileup_w_loc=1.;

  //t3->SetBranchAddress("MC_w", &MC_w_loc, &b_MC_w);
  t3->SetBranchAddress("pileup_w", &pileup_w_loc, &b_pileup_w);

  Float_t TotalEvents = 0.;  //Float_t TotalEvents_noPU = 0.;
  Int_t nentries = (Int_t)t3->GetEntries();

  for (Int_t i = 0; i < nentries; i++){
    //b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    //TotalEvents += MC_w_loc*pileup_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global.
    TotalEvents += pileup_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global.   
    //TotalEvents_noPU += MC_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global.

  }

  cout<<"Total events: "<<TotalEvents<<endl;
  //cout<<"Total events_noPU: "<<TotalEvents_noPU<<endl; 
  
  average_w = TotalEvents/nentries;
  w = fabs(average_w) > 0.0 ? 1./average_w : 1.;
  
  //average_w_noPU = TotalEvents_noPU/nentries;
  //w_noPU = fabs(average_w_noPU) > 0.0 ? 1./average_w_noPU : 1.;  

  for (Int_t i = 0; i < nentries; i++){
    b_w->Fill();
    //b_w_noPU->Fill();    
  }  
 
  t3->Write("",TObject::kOverwrite);
  f3->Close();
}


//---for grid jobs: FileWeight has to be computed
void ComputeNewBranch(TString fileName, float nsimAOD, float sumwAOD){

  TFile *f4= new TFile(fileName.Data(),"update");
  TTree *t3 = (TTree*)f4->Get("AnalysisTree");

  TBranch *b_xsec;
  TBranch *b_feff;
  TBranch *b_kfactor;

  float xsec = 0.0;
  float feff = 0.0;
  float kfactor = 1.0;
 
  t3->SetBranchAddress("xsec",&xsec,&b_xsec);
  t3->SetBranchAddress("feff",&feff,&b_feff);
  t3->SetBranchAddress("kfactor",&kfactor,&b_kfactor);
 
  //float FileWeight = 1.;

  //TBranch *b_FileWeight = t3-> Branch("FileWeight",&FileWeight,"FileWeight/F");
  TBranch *b_nsimAOD = t3-> Branch("nsimAOD",&nsimAOD,"nsimAOD/F");  
  TBranch *b_sumwAOD = t3-> Branch("sumwAOD",&sumwAOD,"sumwAOD/F"); 

  b_xsec->GetEntry(0);
  b_feff->GetEntry(0);
  b_kfactor->GetEntry(0);

  //FileWeight = xsec*feff*kfactor/nsimAOD;
   
  for (int i = 0; i< t3->GetEntries();i++){  
    //b_FileWeight->Fill();
    b_sumwAOD->Fill();    
    b_nsimAOD->Fill();    
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
  //float ph_antiSF=1.;
  
  TBranch *b_e_antiSF = t3-> Branch("e_antiSF",&e_antiSF,"e_antiSF/F");
  TBranch *b_m_antiSF = t3-> Branch("m_antiSF",&m_antiSF,"m_antiSF/F");
  //TBranch *b_ph_antiSF = t3-> Branch("ph_antiSF",&ph_antiSF,"ph_antiSF/F");
  
  TBranch *b_e_SF;
  TBranch *b_m_SF;
  //TBranch *b_ph_SF;
  TBranch *b_e_N;
  TBranch *b_m_N;
  //TBranch *b_ph_N;
  TBranch *b_w;
  //TBranch *b_MC_w;
  TBranch *b_pileup_w;

  float e_SF_loc=1.;
  float m_SF_loc=1.;
  //float ph_SF_loc=1.;
  int   e_N_loc=0;
  int   m_N_loc=0;
  //int   ph_N_loc=0;
  float w_loc=1.;
  //float MC_w_loc=1.;
  float pileup_w_loc=1.;

  t3->SetBranchAddress("e_SF", &e_SF_loc, &b_e_SF);
  t3->SetBranchAddress("m_SF", &m_SF_loc, &b_m_SF);
  //t3->SetBranchAddress("ph_SF", &ph_SF_loc, &b_ph_SF);
  t3->SetBranchAddress("e_N", &e_N_loc, &b_e_N);
  t3->SetBranchAddress("m_N", &m_N_loc, &b_m_N);
  //t3->SetBranchAddress("ph_N", &ph_N_loc, &b_ph_N);
  t3->SetBranchAddress("w", &w_loc, &b_w);
  //t3->SetBranchAddress("MC_w", &MC_w_loc, &b_MC_w);
  t3->SetBranchAddress("pileup_w", &pileup_w_loc, &b_pileup_w);

  Float_t EventsWithElec = 0.;
  Float_t EventsWithMuon = 0.;
  //Float_t EventsWithPhoton = 0.;
  Float_t TotalEvents = 0.;
  Int_t nentries = (Int_t)t3->GetEntries();

  for (Int_t i = 0; i < nentries; i++){
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    //b_ph_N->GetEntry(i);
    b_w->GetEntry(i);
    //b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    TotalEvents += w_loc*pileup_w_loc; //--- It's done independently for each NPx (so same FileWeight), and the luminosity is global.                                      
    if(e_N_loc>0){
      //EventsWithElec += w_loc*MC_w_loc*pileup_w_loc;
      EventsWithElec += w_loc*pileup_w_loc;      
    }
    if(m_N_loc>0){
      //EventsWithMuon += w_loc*MC_w_loc*pileup_w_loc;
      EventsWithMuon += w_loc*pileup_w_loc;      
    }
    /*
    if(ph_N_loc>0){
      //EventsWithPhoton += w_loc*MC_w_loc*pileup_w_loc;
      EventsWithPhoton += w_loc*pileup_w_loc;      
      
    }*/

  }

  std::cout<<"Total events with electrons : "<<EventsWithElec<<std::endl;
  std::cout<<"Total events with muons     : "<<EventsWithMuon<<std::endl;
  //std::cout<<"Total events with photons   : "<<EventsWithPhoton<<std::endl;
  std::cout<<"Total events: "<<TotalEvents<<std::endl;

  Float_t e_SF_mean=0.;
  Float_t m_SF_mean=0.;
  //Float_t ph_SF_mean=0.;
  TH1F *h_e_SF_loc = new TH1F("h_e_SF_loc", "h_e_SF_loc", 400,0.0,2.0);
  TH1F *h_m_SF_loc = new TH1F("h_m_SF_loc", "h_m_SF_loc", 400,0.0,2.0);
  //TH1F *h_ph_SF_loc = new TH1F("h_ph_SF_loc", "h_ph_SF_loc", 400,0.0,2.0);

  for (Int_t i = 0; i < nentries; i++){
    b_e_SF->GetEntry(i);
    b_m_SF->GetEntry(i);
    //b_ph_SF->GetEntry(i);
    b_e_N->GetEntry(i);
    b_m_N->GetEntry(i);
    //b_ph_N->GetEntry(i);
    b_w->GetEntry(i);
    //b_MC_w->GetEntry(i);
    b_pileup_w->GetEntry(i);

    //if(e_N_loc>0) {h_e_SF_loc->Fill(e_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    //if(m_N_loc>0) {h_m_SF_loc->Fill(m_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    //if(ph_N_loc>0) {h_ph_SF_loc->Fill(ph_SF_loc, w_loc*MC_w_loc*pileup_w_loc);}
    if(e_N_loc>0) {h_e_SF_loc->Fill(e_SF_loc, w_loc*pileup_w_loc);}
    if(m_N_loc>0) {h_m_SF_loc->Fill(m_SF_loc, w_loc*pileup_w_loc);}
    //if(ph_N_loc>0) {h_ph_SF_loc->Fill(ph_SF_loc, w_loc*pileup_w_loc);}	 
  }
  e_SF_mean  = h_e_SF_loc->GetMean(); //--- Both have w, so it cancels.                                                                                                          
  m_SF_mean  = h_m_SF_loc->GetMean();
  //ph_SF_mean = h_ph_SF_loc->GetMean();

  h_e_SF_loc->Delete();
  h_m_SF_loc->Delete();
  //h_ph_SF_loc->Delete();

  if(e_SF_mean==0.) { e_SF_mean=1.;} //--- Just in case                                                                                                                          
  if(m_SF_mean==0.) { m_SF_mean=1.;}
  //if(ph_SF_mean==0.) { ph_SF_mean=1.;}

  std::cout<<"Mean electron SF : "<<e_SF_mean<<std::endl;
  std::cout<<"Mean muon SF     : "<<m_SF_mean<<std::endl;
  //std::cout<<"Mean photon SF   : "<<ph_SF_mean<<std::endl;

  std::cout<<"Mean electron SF : "<<e_SF_mean<<std::endl;
  std::cout<<"Mean muon SF     : "<<m_SF_mean<<std::endl;
  //std::cout<<"Mean photon SF   : "<<ph_SF_mean<<std::endl;

  if (!isData){
    e_antiSF = 1 + (1-e_SF_mean)*((Float_t)EventsWithElec/((Float_t)TotalEvents-(Float_t)EventsWithElec));
    m_antiSF = 1 + (1-m_SF_mean)*((Float_t)EventsWithMuon/((Float_t)TotalEvents-(Float_t)EventsWithMuon));
    //ph_antiSF = 1 + (1-ph_SF_mean)*((Float_t)EventsWithPhoton/((Float_t)TotalEvents-(Float_t)EventsWithPhoton));

    if((e_antiSF < -1000 || e_antiSF > 1000) || (TotalEvents-EventsWithElec==0)) {
      e_antiSF=1; //--- To avoid nan or inf                                                                                                                                          
      std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    }
    if((m_antiSF < -1000 || m_antiSF > 1000) || (TotalEvents-EventsWithMuon==0)) {
      m_antiSF=1;
      std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    }
    //if((ph_antiSF < -1000 || ph_antiSF > 1000) || (TotalEvents-EventsWithPhoton==0)) {
    //  ph_antiSF=1;
    //  std::cout<<"nan or inf found. I'll correct it."<<std::endl;
    //}
  }
  else{
    e_antiSF = 1;
    m_antiSF = 1;
    //ph_antiSF = 1;
  }

  std::cout<<"Electron anti-SF : "<<e_antiSF<<std::endl;
  std::cout<<"Muon anti-SF     : "<<m_antiSF<<std::endl;
  //std::cout<<"Photon anti-SF   : "<<ph_antiSF<<std::endl;

  for (Int_t i = 0; i < nentries; i++){
    b_e_antiSF->Fill();
    b_m_antiSF->Fill();
    //b_ph_antiSF->Fill();
  }

  t3->Write("",TObject::kOverwrite);
  f5->Close();
}


void hadd(std::vector< TString> filelist, TString outfile){

  TString torun = "hadd -f "+outfile+" ";

  for(auto file : filelist){
    torun += file;
    torun += "  ";
  }
  cout << "HADD command : " << torun.Data() << endl; 
  system(torun.Data());

}


void tadd(std::vector< TString> filelist, std::vector< Double_t> weights, TString outfile, bool isData ){

  //--- Join all splitted files
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Joining splitted files..."<<endl;
    //JoinSplittedFiles(filelist.at(i));
    MergeSplittedFiles(filelist.at(i));
  }

  float nsim_total = 0;  
  float nweight_total =0;
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding file: "<<filelist.at(i)<<endl;
    TFile f1(filelist.at(i));
    TH1F *histo_nsim = (TH1F*)f1.Get("h_presel_flow");
    TH1F *histo_nweight = (TH1F*)f1.Get("h_presel_wflow");    
    nsim_total+=histo_nsim->GetBinContent(1);
    nweight_total+=histo_nweight->GetBinContent(1);
    
    delete histo_nsim;
    delete histo_nweight;    
  }
  cout << "Total events for this sample: " << nsim_total << endl;


  //--- Add some new branches
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding new branches..."<<endl;
    AddNewBranch(filelist.at(i), weights.at(i),nsim_total, nweight_total);
    addAverageWeight(filelist.at(i));
  }

  //--- Join the "joined" files in a single root file. Add also FileWeight branch
  MergeFiles(filelist, outfile.Data());

  //--- Remove file in the Collateral folder
  for(unsigned int i=0; i<filelist.size(); i++){
    gROOT->ProcessLine(Form(".! rm %s", filelist.at(i).Data()));
  }

  cout<<"\nAdding anti_e_SF and anti_m_SF..."<<endl;
  addAntiWeightToTree(outfile.Data(), isData);  
  findDuplicates(outfile.Data(),nsim_total, nweight_total, isData); 

  cout << endl;
  cout << bold("Target file : \n                    ") << outfile  << endl;
  cout << endl;
}

void tadd_grid(std::vector< TString> filelist, TString outfile, bool isData ){
 
  if( !filelist.size() ){
    cout << "tadd_grid()   WARNING ** no input files provided! Just leaving..." << endl;
    return;
  }

  float nsim_total = 0;  
  float nweight_total =0;
  for(unsigned int i=0; i<filelist.size(); i++){
    cout<<"Adding file: "<<filelist.at(i)<<endl;
    TFile f1(filelist.at(i));
    TH1F *histo_nsim = (TH1F*)f1.Get("h_presel_flow");
    TH1F *histo_nweight = (TH1F*)f1.Get("h_presel_wflow");    
    nsim_total+=histo_nsim->GetBinContent(1);
    nweight_total+=histo_nweight->GetBinContent(1);
    
    delete histo_nsim;
    delete histo_nweight;    
  }
  cout << "Total events for this sample: " << nsim_total << endl;

  MergeFiles(filelist, outfile);
 

  cout<<"\nAdding FileWeight ..."<<endl;  
  ComputeNewBranch(outfile.Data(),nsim_total, nweight_total);
  cout<<"\nAdding average weight w ..."<<endl;  
  addAverageWeight(outfile.Data());
  cout<<"\nAdding anti_e_SF and anti_m_SF ..."<<endl;
  addAntiWeightToTree(outfile.Data(), isData);  
  findDuplicates(outfile.Data(),nsim_total, nweight_total, isData); 

  cout << endl;
  cout << bold("Target file : \n                    ") << outfile  << endl;
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
