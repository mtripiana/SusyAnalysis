//---------------------------------------------------------------
//  Production of mini ntuples for SUSY analyses @IFAE
//
//  - migrated to xAOD format from RunI StopAnalysis package
//
//  author: Martin <tripiana@cern.ch>
//  August, 2014
//---------------------------------------------------------------

#include "xAODRootAccess/Init.h"
#include "SampleHandler/Sample.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "SampleHandler/ToolsMeta.h"
#include "SampleHandler/DiskListLocal.h"
#include "SampleHandler/MetaFields.h"
#include "SampleHandler/fetch.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/TorqueDriver.h"
#include "EventLoopGrid/PrunDriver.h"
#include "EventLoopGrid/GridDriver.h"
#include "EventLoop/OutputStream.h"
#include "EventLoopAlgs/NTupleSvc.h"

#include "SusyAnalysis/chorizo.h"
#include "SusyAnalysis/RunsMap.h"
//#include "SusyAnalysis/Systematics.h"
#include "SusyAnalysis/tadd.h"
#include "SusyAnalysis/utility.h"

#include <iostream>
#include <stdio.h>
#include <string>

// Systematics includes
#include "PATInterfaces/SystematicList.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"
#include "boost/unordered_map.hpp"

using namespace SH;

#define LumFactor 1000. //in nb-1

#define TEST 0  //to quickly restrict the run to 50 events

void usage(){

  cout << endl;
  cout << bold("run_chorizo [options] <Sample> [Syst]") << endl;
  cout << endl;
  cout << " <Sample> : The sample name to run over. " << endl;
  cout << "            To list the implemented samples do: 'run_chorizo samples'" << endl;
  cout << endl;
  cout << " [Syst]   : systematics to be considered (comma-separated list) [optional]. " << endl;
  cout << "            Just nominal is run by default (Nom)." << endl;       
  cout << "            To list the available (recommended) systematic variations do: 'run_chorizo slist'" << endl;
  cout << "            or well run './SusyAnalysis/scripts/list_systematics.sh'" << endl; 
  cout << "" << endl;
  cout << " [options] : supported option flags" << endl;
  cout << "       -l  : run locally (default)   " << endl;
  cout << "       -b  : run in batch            " << endl;
  cout << "       -p  : run on the grid (prun)        " << endl;
  cout << "       -g  : run on the grid (ganga)        " << endl;
  cout << "       -x  : switch to 'at3_xxl' queue (when running in batch mode) (default='at3')  " << endl;
  cout << "       -t  : to run just a test over 50 events " <<endl;
  cout << endl;
}

std::string getCmdOutput(const std::string& mStr)
{
  std::string result, file;
  FILE* pipe{popen((mStr+" 2>/dev/null").c_str(), "r")};
  if (!pipe) return std::string(red("ERROR").Data())+" something went wrong with command : %s \n"+mStr;
  char buffer[256];

  while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
      file = buffer;
      result += file.substr(0, file.size() - 1);
    }
  pclose(pipe);
  return result;
}

std::string tmpdirname(){
  std::string ret = getCmdOutput("mktemp -d");
  system(("rmdir "+ret).c_str());
  return ret;
}

float getLumiWeight(Sample* sample){

  if( sample->getMetaDouble( MetaFields::isData ) ) //don't weight data!
    return 1.;

  return sample->getMetaDouble( MetaFields::crossSection ) * sample->getMetaDouble( MetaFields::kfactor ) * sample->getMetaDouble( MetaFields::filterEfficiency ) / sample->getMetaDouble( MetaFields::numEvents ); 
  
}


float getECM(Sample* sample){ //in TeV
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();
  std::string s_ecm = getCmdOutput( "ami dataset info "+newName+" | grep ECM | awk '{print $2}'");
  return atof(s_ecm.c_str())/1000.;
}

float getEBeam(Sample* sample){ //in TeV
  return getECM(sample)/2.;
}

bool type_consistent(SampleHandler sh){
  
  bool isData=false;
  bool isMC=false;
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){             
    isMC     |= ((*iter)->getMetaDouble( MetaFields::isData)==0);
    isData   |= ((*iter)->getMetaDouble( MetaFields::isData)==1);

    if(isData && isMC)
      return false;
  } 
  return true;
}


void printSampleMeta(Sample* sample){
  cout << endl;
  cout << bold("---- Sample metadata -----------------------------------------------------------------------------------------") << endl;
  cout << " Name            = " << sample->getMetaString( MetaFields::sampleName ) << endl;
  cout << " GridName        = " << sample->getMetaString( MetaFields::gridName ) << endl;
  cout << " isData          = " << (sample->getMetaDouble( MetaFields::isData )==0 ? "No" : "Yes") << endl;
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


void printSystList(){
  Systematics Smap;
  Smap.LoadList();
  Smap.printSystList();
}


int main( int argc, char* argv[] ) {

  std::vector<TString> args,opts;
  bool runLocal = true;
  bool runBatch = false;
  bool runGrid  = false;
  bool runPrun  = false;
  TString queue = "at3";
  bool quick_test = false;

  //parse input arguments
  for (int i=1 ; i < argc ; i++) {
    if( std::string(argv[i]).find("-") != std::string::npos )// is option
      opts.push_back( TString( strtok(argv[i],"-") ));
    else //is argument
      args.push_back( argv[i] );
  }

  //check if enough arguments
  bool systListOnly=false;
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
    args[0] = "TestMClocal"; //rename to test sample to get systematics list
    // printSystList();
    // return 0;
  }

  int single_id=-1;
  //config options
  for( unsigned int iop=0; iop < opts.size(); iop++){
    if (opts[iop] == "l"){ //run locally
      runLocal = true;
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
    else if (opts[iop] == "t"){ //limit run to n events
      quick_test = true;
    }
    else if (opts[iop].BeginsWith("i") ){
      single_id = opts[iop].ReplaceAll("i=","").Atoi();
    }
  }

  //always run locally for systematics printing!
  if(systListOnly && !runLocal){
    runGrid=false;
    runBatch=false;
    runLocal=true;
    cout << bold(red("\n Ups! "));
    cout << "Running mode forced to 'local' when printing systematics list.\n" << endl;
  }

  //check for needed setup 
  std::string ami_check = gSystem->GetFromPipe("which ami ").Data();
  if (ami_check.empty()){
    cout << bold(red("\n Ups! "));
    cout << "You need to setup a few things first!" << endl;
    cout << " Please do: " << endl;
    cout << "\n   source $ROOTCOREBIN/user_scripts//SusyAnalysis/grid_up_pwin.sh \n" << endl;
    cout << "\n ...but ok! this time I'll try to do it for you...  :) \n" << endl;
    gSystem->Exec("source $ROOTCOREBIN/user_scripts//SusyAnalysis/grid_up_pwin.sh");
    //return 0; //FOR TESTING
  }


  //***Get map of runs  
  RunsMap mapOfRuns;
  //check if sample is mapped
  RMap mymap = mapOfRuns.getMap();
  RMap::iterator it = mymap.find( args[0] );
  if(it == mymap.end()){
    cout << bold(red("\n Ups! "));    
    cout << " Sample '" << args[0] << "' not found in current map. Please pick another one." << endl;
    cout << "            To list the implemented samples do: 'run_chorizo samples'\n" << endl;
    return 0;
  }


  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Read some config options
  std::string maindir = getenv("ROOTCOREBIN");

  std::string xmlPath=maindir+"/data/SusyAnalysis/METbb_JobOption.xml";


  XMLReader *xmlJobOption = new XMLReader();
  xmlJobOption->readXML(xmlPath);

  bool doAnaTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/doTree");
  bool doFlowTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoCutFlow");
  bool doPUTree = false; //No option in the xml yet!!
  bool generatePUfile = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/GeneratePileupFiles");

  TString FinalPath      = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/RootFilesFolder").c_str());
  if( args.size() > 2 ) FinalPath = args[2];    // Take the submit directory from the input if provided:

  TString CollateralPath = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/PartialRootFilesFolder").c_str());


  // Get patterns/paths to load for this sample
  std::vector<TString> run_pattern = mapOfRuns.getPatterns( args[0] );
  std::vector<int>     run_ids     = mapOfRuns.getIDs( args[0] );

  // Construct the samples to run on:
  SampleHandler sh;
  bool mgd=false; //make grid direct (for direct access to PIC disks)
  for(unsigned int p=0; p < run_pattern.size(); p++){
        
    if(single_id>=0 && ((int)p != single_id)) continue; //pick only chosen id (if given)

    //** Run on local samples
    //   e.g. scanDir( sh, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/" );
    if(runLocal){
      if( run_pattern[p].Contains("/afs/") || run_pattern[p].Contains("/nfs/") || run_pattern[p].Contains("/tmp/") ){//local samples
	scanDir( sh, run_pattern[p].Data() );
      }else{//PIC samples
	scanDQ2 (sh, run_pattern[p].Data() );
	mgd=true;
      }
    }
    else{ 
      //** Run on the grid or in batch mode
      //   e.g. scanDQ2 (sh, "user.tripiana.mc14_8TeV.167752.*");
      scanDQ2 (sh, run_pattern[p].Data() );
      if(runBatch)
	mgd=true; //** PIC samples
    }    

    //set ID (do it global since there will be only one sample now)
    sh.setMetaDouble( "DSID", (double)run_ids[p] );
  }
  if(mgd)
    makeGridDirect (sh, "IFAE_LOCALGROUPDISK", "srm://srmifae.pic.es", "dcap://dcap.pic.es", false);

  sh.print(); //print what we found

  //Handle Meta-Data
  sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

  //override name with 'provenance' name (weal attempt to allow for user-skimmed and partially-transfered samples)
  //save original name in a new meta field
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){             
    (*iter)->setMetaString( "inputName", (*iter)->getMetaString( MetaFields::sampleName) ); //no longer needed?
    (*iter)->setMetaString( MetaFields::gridName, (stripName( TString( (*iter)->getMetaString( MetaFields::sampleName)))+"/").Data() );
  }

  //set EBeam field
  TString s_ecm  = "8"; //default is 8TeV 
  TString s_ecm_tmp = "";
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){
    (*iter)->setMetaDouble ("ebeam", (double)getEBeam(*iter));
    
    s_ecm = Form("%.0f", sh.at(0)->getMetaDouble ("ebeam")*2); 
    if(s_ecm_tmp.IsNull()){ 
      s_ecm_tmp = s_ecm;
    }
    else if( ! s_ecm_tmp.EqualTo(s_ecm) ){ // All ECM set by first sample for now. WARNING: we can't mix MC at diff ecm for now!!
      cout << bold(red("\n Ups! "));
      cout << "You are not supposed to merge diff ecm samples !! (for the moment anyways)" << endl;
      cout << "Check the implementation for '" << args[0] << "' in RunsMap class and get back...\n" << endl;
      return 0;
    }
  }
  
  bool amiFound=true;
  if(s_ecm=="0"){ //if sample not found (e.g. user-made) set to default  //FIX_ME do something about this?
    s_ecm="8";
    amiFound=false;
  }

    
  //  fetch meta-data from AMI
  if(amiFound)
    fetchMetaData (sh, false); 
  
  //  then override some meta-data from SUSYTools
  readSusyMeta(sh,Form("$ROOTCOREBIN/data/SUSYTools/susy_crosssections_%sTeV.txt", s_ecm.Data()));
  
  //Print meta-data and save weights+names for later use
  std::vector<TString> mergeList; 
  std::vector<double> weights;
  bool isData = (sh.size() ? (sh.at(0)->getMetaDouble( MetaFields::isData ))==1 : false); //global flag. It means we can't run MC and data at the same time. Probably ok?
  
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){
      printSampleMeta( *iter );
      weights.push_back( getLumiWeight(*iter) );
  }

  //Check data type consistency (e.g. don't mix MC with data!)
  if( ! type_consistent(sh) ){
    cout << "\n" << bold(red("ERROR :: ")) << "You are mixing MC and data samples!! The current implementation does not support this! Sorry...\n" << endl;
    return 0;
  }

  //Systematics
  CP::SystematicSet syst_CP = CP::SystematicSet();
  SystErr::Syste syst_ST = SystErr::NONE;
  ScaleVariatioReweighter::variation syst_Scale = ScaleVariatioReweighter::None;
  pileupErr::pileupSyste syst_PU = pileupErr::NONE;
  JvfUncErr::JvfSyste syst_JVF = JvfUncErr::NONE;
  BCHCorrMediumErr::BCHSyste syst_BCH = BCHCorrMediumErr::NONE;
  
  std::vector<TString> systematic; 
  if(args.size() > 1){ 
    systematic = getTokens(args[1], ",");
  }
  else
    systematic.push_back("Nom");

  Systematics Sobj;
  Sobj.LoadList();

  //Loop over systematics  (NOTE: without effect for now!! WE run a wrapper on top for this!)
  for (unsigned int isys=0; isys  < systematic.size(); isys++){

    //fill systematics
    Sobj.SystTranslate(systematic[isys], syst_CP, syst_ST, syst_Scale, syst_PU, syst_JVF, syst_BCH);
    
    //Create an EventLoop job:
    EL::Job job;
    job.useXAOD();
    job.sampleHandler( sh );
    
    //Add NtupleSvc
    EL::OutputStream output  ("output");
    job.outputAdd (output);
    EL::NTupleSvc *ntuple = new EL::NTupleSvc ("output");
    ntuple->treeName("AnalysisTree");
    job.algsAdd (ntuple);
    
    chorizo *alg = new chorizo();
    
    //Alg config options here
    alg->outputName = "output";
    alg->Region = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Mode/name/setDefinitionRegion"));
    
    alg->defaultRegion = "SR"; //from XML?
    alg->xmlPath = xmlPath;
    
    alg->isSignal   = false;   //get it from D3PDReader-like code (add metadata to SH)
    alg->isTop      = true;    //get it from D3PDReader-like code (add metadata to SH)
    alg->isQCD      = false;   //get it from D3PDReader-like code (add metadata to SH)
    alg->isAtlfast  = false;   //get it from D3PDReader-like code (add metadata to SH)
    alg->leptonType = "";      //get it from D3PDReader-like code (add metadata to SH)
    alg->isNCBG     = false;   //get it from the XML!!
    
    alg->doAnaTree  = doAnaTree;     // Output trees
    alg->doFlowTree = doFlowTree;
    alg->doPUTree   = false;         //get it from the XML!!
    alg->genPUfile  = generatePUfile;
    
    alg->syst_CP    = syst_CP;      // Systematics
    alg->syst_CPstr = syst_CP.name();
    alg->syst_ST    = syst_ST;      
    alg->syst_Scale = syst_Scale;
    alg->syst_PU    = syst_PU;
    alg->syst_JVF   = syst_JVF;
    alg->syst_BCH   = syst_BCH;
    
    
    alg->printMet      = false;     //debug printing
    alg->printJet      = false;
    alg->printElectron = false;
    alg->printMuon     = false;
    alg->errIgnoreLevel = (systListOnly ? kFatal : kInfo);
    
    alg->systListOnly  = systListOnly;
    
    
    //Load alg to job
    job.algsAdd( alg );
    
    //Set Max number of events (for testing)
    if(TEST || quick_test) job.options()->setDouble (EL::Job::optMaxEvents, 50);
    if(systListOnly)
      job.options()->setDouble (EL::Job::optMaxEvents, 1);
    
    //TTreeCache use
    job.options()->setDouble (EL::Job::optCacheSize, 10*1024*1024);
    
    
    //create tmp output dir
    string tmpdir = tmpdirname();
    
    // Run the job using the appropiate driver:
    EL::DirectDriver Ddriver;
    EL::TorqueDriver Tdriver;
    EL::PrunDriver   Pdriver;
    EL::GridDriver   Gdriver;
        
    //submit the job
    if(runLocal){ //local mode 
      Ddriver.submit( job, tmpdir );
    }
    else if(runBatch){ // batch mode
      //     const std::string HOME = getenv ("HOME");
      Tdriver.shellInit = "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase; source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh; source $ANALYSISCODE/rcSetup.sh Base,2.0.6 || exit $?; source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh || exit $?;";

      Tdriver.submit( job, tmpdir );
    }
    else if(runPrun){ //Prun mode

      //** prun
      //      Pdriver.options()->setString("nc_outputSampleName", "user.%nickname%.SAtest.%in:name[2]%.v0");
      Pdriver.options()->setString("nc_outputSampleName", "user.tripiana.SASimpleTest.v0");
      Pdriver.options()->setDouble("nc_disableAutoRetry", 1);
      //      Pdriver.options()->setString("nc_nFilesPerJob", "5"); //By default, split in as few jobs as possible
      //      Pdriver.options()->setDouble("nc_nFiles", 1);
      //      Pdriver.options()->setDouble("nc_mergeOutput", 1); //run merging jobs for all samples before downloading (recommended) 
      sh.setMetaString ("nc_grid_filter", "*.root*");

      Pdriver.submitOnly( job, tmpdir );
      break;
    }
    else if(runGrid){ //grid mode

      //** ganga
      sh.setMetaString ("nc_grid_filter", "*.root*");
      Gdriver.outputSampleName = "user.%nickname%.SAtest.%in:name[2]%.v0";
      Gdriver.nFiles = 1;
      Gdriver.mergeOutput = 1; 

      Gdriver.submit( job, tmpdir );
    }


    if(systListOnly) return 0; //that's enough if running systematics list. Leave tmp dir as such.
    
    //move output to collateral files' path
    TString sampleName,targetName;
    for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){
      
      if(!generatePUfile){
	sampleName = Form("%s.root",(*iter)->getMetaString( MetaFields::sampleName ).c_str());
	targetName = Form("%s_%s_%d.root", systematic[isys].Data(), args[0].Data(), run_ids[single_id]);
	
	system("cp "+tmpdir+"/data-output/"+sampleName.Data()+" "+CollateralPath+"/"+targetName.Data());
	
	mergeList.push_back(TString(CollateralPath)+"/"+targetName);
      }
    }
  
    if(single_id<0){ //NOTE: moved to the run wrapper for now!!

      //** after-burner to merge samples, add weights and anti-SF
      cout << bold("\n\n*** AFTER-BURNER *** ") << endl;
      cout << "\n >> merging "<< mergeList.size() << " sample(s) ...\n" << endl;
      for (unsigned int i=0; i < mergeList.size(); i++){
	cout<<"\t\t" << i << ": " << mergeList[i] << endl;
      }      
      
      TString mergedName = Form("%s_%s.root",systematic[isys].Data(), args[0].Data());
      
      if (!generatePUfile){
	if (!doAnaTree) {
	  //--- Case where we run on 1 file
	  //... 
	} 
	else {
	  tadd(mergeList, weights, FinalPath+"/"+mergedName, isData);
	}
      }
    }

  }//end systematics list

  return 0;
}


