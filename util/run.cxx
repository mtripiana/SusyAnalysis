//---------------------------------------------------------------
//  Master wrapper to run over multiple samples and systematics. 
//  EventLoop has (seems to have) some problems running over a 
//  multiple-sample SampleHandler, so here we are...
//
//  author: Martin <tripiana@cern.ch>
//  24/08/14
//---------------------------------------------------------------

#include "xAODRootAccess/Init.h"
#include "SampleHandler/Sample.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "SampleHandler/ToolsMeta.h"
#include "SampleHandler/DiskListLocal.h"
#include "SampleHandler/DiskListEOS.h"
#include "SampleHandler/DiskListXRD.h"
#include "SampleHandler/MetaFields.h"
#include "SampleHandler/fetch.h"
#include "SampleHandler/TagList.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/ProofDriver.h"
#include "EventLoop/TorqueDriver.h"
#include "EventLoopGrid/PrunDriver.h"
#include "EventLoopGrid/GridDriver.h"
#include "EventLoop/OutputStream.h"
#include "EventLoopAlgs/NTupleSvc.h"

#include "SusyAnalysis/chorizo.h"
#include "SusyAnalysis/RunsMap.h"
#include "SusyAnalysis/tadd.h"
#include "SusyAnalysis/utility.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include "TEnv.h"

// Systematics includes
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"
#include "boost/unordered_map.hpp"

using namespace SH;

#define LumFactor 1000. //in nb-1


void usage(){

  cout << endl;
  cout << bold("run [options] <Sample>") << endl;
  cout << endl;
  cout << " <Sample> : The sample name to run over. " << endl;
  cout << "            To list the implemented samples do: 'run samples'" << endl;
  cout << "            You can also specify the input directory directly here (in combination with -u option. see below)" << endl;
  cout << endl;
  cout << "" << endl;
  cout << " [options] : supported option flags" << endl;
  cout << "       -j=<jOption>  : choose which analysis you want to run over. ( = 'METbb'(default), 'Stop', 'Monojet')	" << endl;		    
  cout << "       -l            : run locally (default)   " << endl;
  cout << "       -b            : run in batch            " << endl;
  cout << "       -p            : run on the grid (prun)        " << endl;
  cout << "       -g            : run on the grid (ganga)        " << endl;
  cout << "       -y            : run using proof-lite " << endl;
  cout << "       -x            : switch to 'at3_xxl' queue (when running in batch mode) (default='at3')  " << endl;
  cout << "       -t            : to run just a test over 50 events " <<endl;
  cout << "       -v=<V>        : output version. To tag output files. (adds a '_vV' suffix)" <<endl;
  cout << "       -i=<ID>       : specify a specific dataset ID inside the container" << endl;
  cout << "       -n=<N>        : to run over N  events " <<endl;
  cout << "       -s=<SystList> : systematics to be considered (comma-separated list) [optional]. " << endl;
  cout << "                       Just nominal is run by default (Nom)." << endl;       
  cout << "                       To list the available (recommended) systematic variations do: 'run slist'" << endl;
  cout << "                       or well run './SusyAnalysis/scripts/list_systematics.sh'" << endl; 
  cout << "       -o=<outDir>   : where all the output is saved (if left empty is reads the path from the xml jobOption (FinalPath))." << endl;
  cout << "       -e=<eventList> : provide list of run & event numbers you want to keep." << endl;  
  cout << "       -c             : run over specific directory, over-passing RunsMap (mainly for tests)" << endl;
  cout << "       -d             : debug mode " << endl;
  cout << endl;
}

float getLumiWeight(Sample* sample){

  if( sample->getMetaString( MetaFields::isData )=="Y" ) //don't weight data!
    return 1.;

  return sample->getMetaDouble( MetaFields::crossSection ) * sample->getMetaDouble( MetaFields::kfactor ) * sample->getMetaDouble( MetaFields::filterEfficiency ) / sample->getMetaDouble( MetaFields::numEvents ); 
  
}


float getECM(Sample* sample){ //in TeV
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();
  std::string s_ecm = getCmdOutput( "ami dataset info "+newName+" | grep ECMEnergy | awk '{print $2}'");
  if(s_ecm.empty()){
    //    cout << "no ECMEnergy field. Try new convention next... " << endl;
    s_ecm = getCmdOutput( "ami dataset info "+newName+" | grep beam_energy | awk '{print $2}'");
    s_ecm.erase(std::remove(s_ecm.begin(), s_ecm.end(), ']'), s_ecm.end());
    s_ecm.erase(std::remove(s_ecm.begin(), s_ecm.end(), '['), s_ecm.end());
    return atof(s_ecm.c_str())*2./1000000.;
  }
  return atof(s_ecm.c_str())/1000.;
}

float getEBeam(Sample* sample){ //in TeV
  return getECM(sample)/2.;
}

bool type_consistent(SampleHandler sh){
  
  bool isData=false;
  bool isMC=false;
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){             
    isMC     |= ((*iter)->getMetaString( MetaFields::isData)=="N");
    isData   |= ((*iter)->getMetaString( MetaFields::isData)=="Y");

    if(isData && isMC)
      return false;
  } 
  return true;
}

bool is_data(Sample* sample){ 
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();

  return !getCmdOutput( "ami dataset info "+newName+" | grep runNumber | awk '{print $2}'").empty();

  // std::string itis = getCmdOutput( "ami dataset info "+newName+" | grep beamType | awk '{print $2}'");
  // std::size_t found = itis.find("collisions");
  // if (found!=std::string::npos) return true;

  //add extra check for physics containers
  //  if(TString(itis).Contains("period")) return true;

  // return false;
}

double getNPrimary(Sample* sample){ 
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();
  TString index="0";
  //check if derivation
  std::string gname = sample->getMetaString( MetaFields::gridName );
  if(gname.find("DAOD")!=std::string::npos)
    index="-1";
  std::string provname = getCmdOutput("ami dataset prov "+newName+" | grep -A 1 \"= "+string(index.Data())+"\" | tail -1");
  std::string nev="0";
  if(provname != "")
    nev = getCmdOutput( "ami dataset info "+provname+" | grep totalEvents | awk '{print $2}'");

  return stod(nev);
}

bool is_AtlFast(Sample* sample){
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  return stripName(sampleName).Contains("_a");
}

void HandleMetadata(SampleHandler& sh){

      //*** Handle Meta-Data
      sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

      if( !sh.size() ) return;

      bool amiFound=true;
      TString s_ecm = "13"; 

      //override name with 'provenance' name (weal attempt to allow for user-skimmed and partially-transfered samples)
      //save original name in a new meta field
      for (auto &sample : sh){

	sample->setMetaString( "inputName",  sample->getMetaString( MetaFields::sampleName) ); //I think no longer needed
	sample->setMetaString( MetaFields::gridName,  stripName( TString( sample->getMetaString( MetaFields::sampleName))).Data() );

	//get the number of events of the primary AOD (relevant for running on derivations)
	sample->setMetaDouble( MetaFields::numEvents, getNPrimary(sample) );

	//set Data flag correctly
	sample->setMetaString( MetaFields::isData, (is_data(sample) ? "Y" : "N") );
	if( sample->getMetaString( MetaFields::isData )=="Y" ) sample->addTag("data");
	else sample->addTag("mc");

	//set EBeam field
	sample->setMetaDouble ("ebeam", (double)getEBeam(sample));
	s_ecm = Form("%.0f", sample->getMetaDouble ("ebeam")*2);
	if(s_ecm=="0"){ //if sample not found (e.g. user-made) set to default  //FIX_ME do something about this?
	  s_ecm="13";
	  amiFound=false;
	}

	
	if( s_ecm == "13" ) sample->addTag("13TeV"); 
	if( s_ecm == "8" )  sample->addTag("8TeV"); 
      }

      //  fetch meta-data from AMI
      if(amiFound && 0){
	fetchMetaData (sh, false); 
	for (auto &sample : sh){ //convert to SUSYTools metadata convention (pb)
	  float newxs = sample->getMetaDouble( MetaFields::crossSection )*1000.;
	  sample->setMetaDouble (MetaFields::crossSection, newxs);                
	}                                                          
      }


      //  then override some meta-data from SUSYTools
      TagList tags_mc13("13TeV");
      TagList tags_mc8("8TeV");

      // sh.find(tags_mc8).print();
      // sh.find(tags_mc13).print();

      readSusyMetaDir(sh.find(tags_mc8), "$ROOTCOREBIN/data/SUSYTools/mc12_8TeV/");
      readSusyMetaDir(sh.find(tags_mc13), "$ROOTCOREBIN/data/SUSYTools/mc15_13TeV/");

}


void printSampleMeta(Sample* sample){
  cout << endl;
  cout << bold("---- Sample metadata -----------------------------------------------------------------------------------------") << endl;
  cout << " Name            = " << sample->getMetaString( MetaFields::sampleName ) << endl;
  cout << " GridName        = " << sample->getMetaString( MetaFields::gridName ) << endl;
  cout << " isData          = " << (is_data(sample) ? "Y" : "N") << endl;
  cout << " ECM (TeV)       = " << sample->getMetaDouble( "ebeam" )*2 << endl;
  cout << " Xsection        = " << sample->getMetaDouble( MetaFields::crossSection ) << endl;
  cout << " XsectionRelUnc  = " << sample->getMetaDouble( MetaFields::crossSectionRelUncertainty ) << endl;
  cout << " kfactor         = " << sample->getMetaDouble( MetaFields::kfactor ) << endl;
  cout << " filterEff       = " << sample->getMetaDouble( MetaFields::filterEfficiency ) << endl;
  cout << " N               = " << sample->getMetaDouble( MetaFields::numEvents ) << endl;
  cout << " Lumi            = " << sample->getMetaDouble( MetaFields::lumi ) << endl;
  cout << bold("--------------------------------------------------------------------------------------------------------------") << endl;
  cout << endl;  
}

void printSamplesList(){
  RunsMap rmap;
  std::vector<TString> samples = rmap.getKeys();
  std::vector<int> ids;
  std::cout << bold("\n List of samples") << std::endl; 
  std::cout << bold("------------------------------------------------------------------------------------------------------------------------") << std::endl;
  for(unsigned int i=0; i<samples.size(); i++){
    cout << setw(70) << left << samples[i];
    cout << "[" ;
    ids = rmap.getIDs(samples[i]);
    for(unsigned int j=0; j<ids.size(); j++){
      cout << ids[j] << ",";
    }
    cout << "]" << endl;
  }
}

int getSystID(TString syst){

  Systematics Sobj;
  Sobj.LoadList();
  return Sobj.getSystID(syst);
  
}

TString getSystShort(TString syst){

  if(syst=="Nom" || syst=="") return ".NOM"; //no tag for nominal run

  //Get systematics ID otherwise
  TString stag = Form(".SYS%d", getSystID(syst));

  //dummy for now
  return stag;
}

void printSystList(){
  Systematics Smap;
  Smap.LoadList();
  Smap.printSystList();
}


int main( int argc, char* argv[] ) {

  std::vector<TString> args,opts;
  bool runLocal = true;
  bool runProof = false;
  bool runBatch = false;
  bool runPrun  = false;
  bool runGrid  = false;

  //  bool noBuild = false;

  std::string jOption    = "METbb";
  TString     queue      = "at3";
  int         single_id  = -1;
  bool        isTruth    = false;
  bool        isAFII     = false;
  TString     version    = "";
  bool        userDir    = false;
  TString     eventsFile = "";

  string wildcard   = "*";
  bool systListOnly = false;

  //parse input arguments
  for (int i=1 ; i < argc ; i++) {
    if( std::string(argv[i])[0] == '-' ) //is option
      opts.push_back( TString( strtok(argv[i],"-") ));
    else //is argument
      args.push_back( argv[i] );
  }

  //check if enough arguments
  if( args.size() < 1 ){
    usage();
    return 0;
  }
  else if ( args[0] == "samples" ){ //just print samples list?
    printSamplesList();
    return 0;
  }
  else if ( args[0] == "slist" ){ //just print systematics list?
    systListOnly=true;
    gErrorIgnoreLevel = kFatal;
    args[0] = "singletop_cutflow"; //"test_ttbar_martin"; //"mc15_ttbar"; //TestDF"; //rename to test sample to get systematics list
  }

  //config options
  int nMax=-1;
  TString syst_str="";
  TString outDir="";
  bool debugMode=false;
  for( unsigned int iop=0; iop < opts.size(); iop++){
    if (opts[iop] == "l"){ //run locally
      runLocal = true;
    }
    if (opts[iop] == "y"){ //run using proof-lite
      runProof = true;
    }
    else if (opts[iop] == "b"){ //run in batch mode
      runBatch = true;
      runLocal = false;
    } 
    else if (opts[iop] == "p"){ //run on the grid (prun)
      runPrun = true;
      runLocal = false;
    }
    else if (opts[iop] == "g"){ //run on the grid (ganga)
      runGrid = true;
      runLocal = false;
    }
    else if (opts[iop] == "x"){ //switch to 'at3_xxl' batch queue (at3 by default)
      queue = "at3_xxl";
    }
    else if (opts[iop] == "t"){ //run on truth xAOD
      isTruth = true;
    }
    if (opts[iop] == "a"){ //is AFII reco                                                                                                                                        
      isAFII = true;
    }
    // if (opts[iop] == "r"){ //avoid rebuilding libDS 
    //   noBuild = true;
    // }
    else if (opts[iop] == "c" ){ //user-defined input dir
      userDir = true;
    }
    else if (opts[iop].BeginsWith("s") ){
      syst_str = opts[iop].Copy().ReplaceAll("s=","");
    }
    else if (opts[iop].BeginsWith("o") ){
      outDir = opts[iop].Copy().ReplaceAll("o=","");
    }
    else if (opts[iop].BeginsWith("i") ){
      single_id = opts[iop].Copy().ReplaceAll("i=","").Atoi();
    }
    else if (opts[iop].BeginsWith("j") ){
      jOption = opts[iop].Copy().ReplaceAll("j=","");
    }
    else if (opts[iop].BeginsWith("n") ){ //limit run to n events
      nMax = opts[iop].Copy().ReplaceAll("n=","").Atoi();
    }
    else if (opts[iop].BeginsWith("v") ){
      version = opts[iop].Copy().ReplaceAll("v=","");
    }
    else if (opts[iop].BeginsWith("e") ){
      eventsFile = opts[iop].Copy().ReplaceAll("e=","");
    }
    else if (opts[iop] == "d" ){ //debug mode
      debugMode = true;
    }

  }

  TString vTag="";
  if(version!="")
    vTag = "_v"+version;


  //samples
  std::vector<TString> samples;
  samples = getTokens(args[0], ","); //I already checked there is a 'sample' input
  
  //Systematics
  std::vector<TString> systematics; 
  if(syst_str.Length()){  
    systematics = getTokens(syst_str, ",");
  }
  else{
    syst_str="Nom";
    systematics.push_back("Nom");
  }

  

  //*** BATCH WRAPPER ***
  //(temporary?) Wrapper to run on the batch .   Until we get the TorqueDriver working...
  if(runBatch){
    for(unsigned int isp=0; isp < samples.size(); isp++){
      gSystem->Exec("source $ROOTCOREBIN/../SusyAnalysis/scripts/submit_batch.sh "+queue+" "+syst_str+" "+jOption+" "+samples[isp]);
    }
    return 0;
  }
  //***


  //check for needed setup 
  std::string ami_check = gSystem->GetFromPipe("which ami 2>/dev/null").Data(); 
  if (ami_check.empty()){
    cout << bold(red("\n Ups! "));
    cout << "You need to setup a few things first!" << endl;
    cout << " Please do: " << endl;
    cout << "\n   source $ROOTCOREBIN/../SusyAnalysis/scripts/grid_up.sh   \n" << endl;
    cout << "\n ...but ok! this time I'll try to do it for you...  :) \n" << endl;
    gSystem->Exec("source $ROOTCOREBIN/../SusyAnalysis/scripts/grid_up_pwin.sh");
  }

  // Read some config options
  std::string maindir = getenv("ROOTCOREBIN");
  
  TEnv rEnv;
  int success = -1;
  success = rEnv.ReadFile((maindir+"/data/SusyAnalysis/"+jOption+".conf").c_str(), kEnvAll);
  if(success != 0){
    cout << "   CONFIG FILE NOT FOUND!  Please try again...\n" << endl;
    return -1;
  }
  
  std::string username = getenv("USER");
  TString FinalPath      = TString(rEnv.GetValue(string("Global."+username+".OutFolder").c_str(),"./"));
  if( args.size() > 2 ) FinalPath = args[2];    // Take the submit directory from the input if provided:
  
  TString TmpPath = TString(rEnv.GetValue(string("Global."+username+".TmpFolder").c_str(),"./"));
    
  std::vector<TString> mergeList; 
  std::vector<double> weights;
  bool isData;

  //*** Systematics
  CP::SystematicSet syst_CP = CP::SystematicSet();
  
  Systematics Sobj;
  Sobj.LoadList();

  //***Get map of runs  
  RunsMap mapOfRuns;
  RMap    mymap = mapOfRuns.getMap();

  //*** Run chorizo for every sample-id-systematic combination
  //*** Merge ids for same sample-systematic pair
  SampleHandler sh_all;
  sh_all.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

  //  HandleMetadata(sh_all);

  //  bool first = true;

  // SampleHandler
  SampleHandler sh;
  sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

  std::vector<TString> run_patterns;
  std::vector<int>     run_ids;

  // Run drivers
  EL::DirectDriver Ddriver;
  EL::ProofDriver  ProofDriver;
  EL::TorqueDriver Tdriver;
  EL::PrunDriver   Pdriver;
  EL::GridDriver   Gdriver;

  //** Loop over samples  
  unsigned int isp=0; //sample counter
  for(auto const& sample : samples){

    //check if sample is mapped
    if(!userDir){
      RMap::iterator it = mymap.find( sample );
      if(it == mymap.end()){
	cout << bold(red("\n Ups! "));    
	cout << " Sample '" << sample << "' not found in current map. Please pick another one." << endl;
	cout << "            To list the implemented samples do: 'run_chorizo samples'\n" << endl;
	cout << "            To run over a user-directory directly use -c.'\n" << endl;
	return 0;
      }
    }

    //Print meta-data and save weights+names for later use
    mergeList.clear(); 
    weights.clear();
    isData=false;

    // Get patterns/paths to load for this sample
    if(!userDir){
      run_patterns  = mapOfRuns.getPatterns( sample );
      run_ids       = mapOfRuns.getIDs( sample );
    }
    else{
      run_patterns.push_back( sample );
      run_ids.push_back( 0 ); 
    }

    bool mgd=false;  //make grid direct (for direct access to PIC disks)

    //ID loop
    unsigned int nID=0;
    for(auto dsid : run_ids){ 
      
      if(!userDir && single_id>=0 && dsid != single_id) continue; //pick only chosen DSID (if given)
      
      TString cPattern = (userDir ? TString(getCmdOutput("readlink -f "+std::string(run_patterns[nID].Data()))) : run_patterns[nID]);
      
      //** Run on local samples
      if(runLocal){ // || userDir){
	if( run_patterns[nID].BeginsWith("/eos/") ){
	  //	  SH::DiskListEOS list ("eosatlas.cern.ch", run_patterns[i_id].Data());
	  SH::DiskListXRD list ("eosatlas.cern.ch", gSystem->DirName( cPattern ), true);
	  TString bname_regexp = Form("%s*", gSystem->BaseName( cPattern ));
	  SH::scanDir (sh, list, "*", bname_regexp.Data());
	}
	else if(userDir || cPattern.Contains("/afs/") || cPattern.Contains("/nfs/") || cPattern.Contains("/tmp/")){//local samples
	  scanDir( sh, cPattern.Data() );//, wildcard );
	  //.Find( run_patterns[i_id].Data() );
	}
	else{//PIC samples
	  scanDQ2 (sh, cPattern.Data() );
	  mgd=true;
	}
      }
      else if(runBatch){
	//** Run in batch 
	scanDQ2 (sh, cPattern.Data() );
	mgd=true;
      }    
      else if(runGrid || runPrun){
       
	//** Run on the grid
	// When running in grid mode we add all samples in one single SH, so we speed up the submission time after the first (i.e. no rebuilding)
	for(auto dspn : run_patterns){
	  cPattern = (userDir ? TString(getCmdOutput("readlink -f "+std::string(dspn.Data()))) : dspn);
	  scanDQ2 (sh, cPattern.Data() );
	  nID++;
	}
	if(isp < samples.size()-1){ //if not the last sample, break and keep acumulating into same SH
	  isp++;
	  break;
	}
      }    
      
      if(mgd)
	makeGridDirect (sh, "IFAE_LOCALGROUPDISK", "srm://srmifae.pic.es", "dcap://dcap.pic.es", false);

      
      //MetaData      
      HandleMetadata(sh);
      
      sh.print();
      
      for(auto &se : sh){
	
	if(se->getMetaString( MetaFields::isData )!="Y")
	  weights.push_back( getLumiWeight(se) ); //mc
	else
	  weights.push_back( 1. ); //data
	
	std:: string str_sample(sample.Data());
	se->addTag(str_sample);
	se->addTag(std::to_string(dsid));
      }


      //check if it is AFII simulation (if not explicitly said by command line) 
      if(!isAFII)
	isAFII |= is_AtlFast(sh.at(0));



      //Systematics loop
      unsigned int nsyst=0;
      for( const auto& syst : systematics){

	std::cout << "RUNNING SYST = " << syst <<  std::endl;
	
	  //fill systematics
	Sobj.SystTranslate( syst, syst_CP );
	
	//Create an EventLoop job:
	EL::Job job;
	job.useXAOD();
	job.sampleHandler( sh );
	
	//Add NtupleSvc
	std::string osname="output";
	EL::OutputStream output  (osname);
	job.outputAdd (output);
	
	chorizo *alg = new chorizo();
	
	//Alg config options here
	alg->outputName = osname;
	alg->jOption = jOption;
	
	alg->isSignal   = (bool) rEnv.GetValue("Global.isSignal",0);
	alg->is25ns     = (bool) rEnv.GetValue("Global.is25ns",1);	
	alg->isMC15b     = (bool) rEnv.GetValue("Global.isMC15b",0);
	alg->isQCD      = (bool) rEnv.GetValue("Tools.JetSmear.enable",0);
	alg->isAtlfast  = isAFII; 
	alg->leptonType = "";      //get it from D3PDReader-like code (add metadata to SH)
	alg->isNCBG     = false;   //get it from the XML!!
	
	alg->isTruth    = isTruth;   //to run over truth xAOD (e.g. TRUTH1 derivation)
	
	alg->doAnaTree  = (bool) rEnv.GetValue("Global.doTree",1);     // Output trees
	alg->doTrigExt  = (bool) rEnv.GetValue("Trigger.saveExt",0);
	alg->dumpTile   = (bool) rEnv.GetValue("Booking.dumpTile",0);

	alg->eventsFile = eventsFile;

	alg->syst_CP    = syst_CP;      // Systematics
	alg->syst_CPstr = syst_CP.name();
	alg->syst_JESNPset =  rEnv.GetValue("Syst.JESNPset",0);
	
	
	//debug printing
	alg->debug         = debugMode;
	alg->printMet      = false;     
	alg->printJet      = false;
	alg->printElectron = false;
	alg->printMuon     = false;
	alg->errIgnoreLevel = (systListOnly ? kFatal : kInfo);
	
	alg->systListOnly  = systListOnly;
	
	//Set Max number of events (for testing)
	if(nMax > 0) job.options()->setDouble (EL::Job::optMaxEvents, nMax);
	
	if(systListOnly)
	  job.options()->setDouble (EL::Job::optMaxEvents, 1);
	
	//TTreeCache use
	job.options()->setDouble (EL::Job::optCacheSize, 10*1024*1024);
	job.options()->setString (EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch);

	//Load alg to job
	job.algsAdd( alg );
	
	//create tmp output dir
	string tmpdir = tmpdirname();
	if(debugMode)   std::cout << "OUTPUT TMPDIR = " << tmpdir << std::endl;
	
	
	//submit the job
	if(runLocal){ //local mode 
	  if(runProof){
	    //ProofDriver.numWorkers = 4;
	    ProofDriver.submit( job, tmpdir );
	  }
	  else{
	    Ddriver.submit( job, tmpdir );
	  }
	}
	else if(runBatch){ // batch mode
	  //     const std::string HOME = getenv ("HOME");
	  Tdriver.shellInit = "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase; source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh; source $ROOTCOREBIN/../rcSetup.sh Base,2.3.16 || exit $?; source $ROOTCOREBIN/../SusyAnalysis/scripts/grid_up_pwin.sh || exit $?;";
	  
	  Tdriver.submit( job, tmpdir );
	}
	else if(runPrun){ //Prun mode
	  
	  //** prun
	  std::string outName = std::string(TString("user.%nickname%.IFAE")+getSystShort(syst).Data()+TString(".%in:name[2]%.%in:name[3]%")+vTag.Data()); 
	  Pdriver.options()->setString("nc_outputSampleName", outName);
	  Pdriver.options()->setDouble("nc_disableAutoRetry", 0);
	  sh.setMetaString ("nc_grid_filter", "*.root*");
	  // if(!first){ //noBuild
	  //   cout << "LAST !!" << endl;
	  //   Pdriver.options()->setString (EL::Job::optSubmitFlags, "--libDS LAST --noSubmit ");
	  // }
	  // else
	  //	  Pdriver.options()->setString (EL::Job::optSubmitFlags, "--noSubmit ");

	  Pdriver.submitOnly( job, tmpdir );
	  
	  std::cout << "\n" << bold("Submitted!") << std::endl;
	  for (unsigned int i = 0; i < sh.size(); ++i) {
	    std::cout << "  Check it in " << link(Form("http://bigpanda.cern.ch/task/%d", (int)sh[i]->getMetaDouble("nc_jediTaskID"))) << " \n " << std::endl; //sh[i]->name()
	  }
	  
	  //	  first=false;
	}
	else if(runGrid){ //grid mode
	  
	  //** ganga
	  sh.setMetaString ("nc_grid_filter", "*.root*");
	  Gdriver.outputSampleName = std::string(TString("user.%nickname%.IFAE.%in:name[2]%in:name[3]%")+vTag.Data());
	  Gdriver.nFiles = 1;
	  Gdriver.mergeOutput = 1; 
	  
	  Gdriver.submit( job, tmpdir );
	}
	

	if(!runLocal) continue;
 
	//DEBUGGING	
	std::string debout = getCmdOutput( "ls -1 "+tmpdir+"/data-output/" );
	cout << "LS : \n " << debout << endl;

	
	if(systListOnly) return 0; //that's enough if running systematics list. Leave tmp dir as such.

	//move output to collateral files' path
	TString sampleName, targetName;
	for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){
	  
	  sampleName = Form("%s.root",(*iter)->getMetaString( MetaFields::sampleName ).c_str());
	  targetName = Form("%s_%s%s_%d.root", syst.Data(), gSystem->BaseName(sample), vTag.Data(), dsid);
	  
	  cout << "SAMPLE NAME : " << sampleName << endl;
	  cout << "TARGET NAME : " << targetName << endl;

	  addMetaData(tmpdir+"/data-"+osname+"/"+sampleName.Data(),tmpdir+"/hist-"+sampleName.Data()); //write meta data from hist* to data* rootfile
	  
	  system("mv "+tmpdir+"/data-"+osname+"/"+sampleName.Data()+" "+TmpPath+"/"+targetName.Data());
	  
	  if(nsyst==0){ //only for first systematic in loop
	    TString mergeInName  = Form("SYST_%s%s_%d.root", gSystem->BaseName(sample), vTag.Data(), dsid);
	    mergeList.push_back(TString(TmpPath)+"/"+mergeInName);
	  }
	  
	} //SH loop
	
	cout << "TMPDIR = " << tmpdir << endl;
	system(("rm -rf "+tmpdir).c_str());
	
	nsyst++;
      }//end systematics loop
      
      //If running with PrunDriver we did all in one shot already, so just leave!      
      if(runPrun)
	break;
      
      //remove sample from SH
      for(auto ss : sh)
	sh.remove(ss);
      
      nID++;
      
    }//end of IDs loop
    
    
    //Done merging step only if running locally
    if(!runLocal) continue;
    
    
    //MERGING STEP!
    //** after-burner to merge samples, add weights and anti-SF
    cout << bold("\n\n*** AFTER-BURNER *** ") << endl;
    
    std::vector<TString> tmp_mergeList;
    for(const auto& syst : systematics){  //systs loop    

      tmp_mergeList.clear();

      cout << "\n >> merging "<< mergeList.size() << " sample(s) ...\n" << endl;
      for (unsigned int i=0; i < mergeList.size(); i++){
	tmp_mergeList.push_back( mergeList[i].Copy().ReplaceAll("SYST", syst) );
	cout<<"\t\t" << i << ": " << tmp_mergeList[i] << endl;
      }      
      cout << endl;
      
      TString mergedName = Form("%s_%s%s.root",syst.Data(), gSystem->BaseName(sample), vTag.Data());
      
      tadd(tmp_mergeList, weights, FinalPath+"/"+mergedName, isData);
      
    }//end of syst merging loop
    
  }//end of samples loop
  
  return 0;
}


