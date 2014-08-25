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
#include "SampleHandler/MetaFields.h"
#include "SampleHandler/fetch.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
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
  cout << "       -g  : run on the grid         " << endl;
  cout << "       -x  : switch to 'at3_xxl' queue (when running in batch mode) (default='at3')  " << endl;
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
  std::string s_ecm = getCmdOutput( "ami dataset info "+sample->getMetaString( MetaFields::sampleName )+" | grep ECM | awk '{print $2}'");
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
  std::cout << bold("------------------------------------------------------------") << std::endl;
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
  TString queue = "at3";

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

  //config options
  for( unsigned int iop=0; iop < opts.size(); iop++){
    if (opts[iop] == "l"){ //run locally
      runLocal = true;
    }
    else if (opts[iop] == "b"){ //run in batch mode
      runBatch = true;
      runLocal = false;
    } 
    else if (opts[iop] == "g"){ //run on the grid
      runGrid = true;
      runLocal = false;
    }
    else if (opts[iop] == "x"){ //switch to 'at3_xxl' batch queue (at3 by default)
      queue = "at3_xxl";
    }
  }
  TString allopts=""; //join all options in one string
  for (unsigned int iopt=0; iopt < opts.size(); iopt++)
    allopts += " -"+opts[iopt]+" ";

  //always run locally for systematics printing!
  if(systListOnly && !runLocal){
    runGrid=false;
    runBatch=false;
    runLocal=true;
    cout << bold(red("\n Ups! "));
    cout << "Running mode forced to 'local' when printing systematics list.\n" << endl;
  }

  //check for needed setup 
  std::string ami_check = getCmdOutput(R"( which ami )");
  if (ami_check.empty()){
    cout << bold(red("\n Ups! "));
    cout << "You need to setup a few things first!" << endl;
    cout << " Please do: " << endl;
    cout << "\n   source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh" << endl;
    cout << "\n and get back! :) \n" << endl;
    return 0;
  }


  //samples
  std::vector<TString> samples;
  samples = getTokens(args[0], ","); //I already checked there is a 'sample' input
  
  //Systematics
  std::vector<TString> systematics; 
  if(args.size() > 1){ 
    systematics = getTokens(args[1], ",");
  }
  else
    systematics.push_back("Nom");

  //*** Read some input options
  std::string DirectoryPath=gSystem->Getenv("ANALYSISCODE");
  std::string xmlPath=DirectoryPath+"/SusyAnalysis/util/AnalysisJobOptions/METbb_JobOption.xml";

  XMLReader *xmlJobOption = new XMLReader();
  xmlJobOption->readXML(xmlPath);

  bool doAnaTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/doTree");
  bool doFlowTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoCutFlow");
  bool doPUTree = false; //No option in the xml yet!!
  bool generatePUfile = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/GeneratePileupFiles");

  TString FinalPath      = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/RootFilesFolder").c_str());
  if( args.size() > 2 ) FinalPath = args[2];    // Take the submit directory from the input if provided:

  TString CollateralPath = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/PartialRootFilesFolder").c_str());

  //*** Call run_chorizo for every sample-id-systematic combination
  //*** Merge ids for same sample-systematic pair
  SampleHandler sh;
  sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

  std::vector<TString> mergeList; 
  std::vector<double> weights;
  bool isData;
  
  for(unsigned int i_sample=0; i_sample < samples.size(); i_sample++){ //samples loop

    //***Get map of runs  
    RunsMap mapOfRuns;
    //check if sample is mapped
    RMap mymap = mapOfRuns.getMap();
    RMap::iterator it = mymap.find( samples[i_sample] );
    if(it == mymap.end()){
      cout << bold(red("\n Ups! "));    
      cout << " Sample '" << args[0] << "' not found in current map. Please pick another one." << endl;
      cout << "            To list the implemented samples do: 'run_chorizo samples'\n" << endl;
      return 0;
    }

    //Print meta-data and save weights+names for later use
    mergeList.clear(); 
    weights.clear();
    isData=false;

    // Get patterns/paths to load for this sample
    std::vector<TString> run_patterns  = mapOfRuns.getPatterns( args[i_sample] );
    std::vector<int>     run_ids       = mapOfRuns.getIDs( args[i_sample] );
    bool mgd=false;  //make grid direct (for direct access to PIC disks)
    for(unsigned int i_id = 0; i_id < run_ids.size(); i_id++){ //id loop
      
      //** Run on local samples
      if(runLocal){
	scanDir( sh, run_patterns[i_id].Data() );
      }
      else if(runBatch){
	//** Run on PIC samples
	scanDQ2 (sh, run_patterns[i_id].Data() );
	mgd=true;
      }    
      else if(runGrid){
	//** Run on the grid
	scanDQ2 (sh, run_patterns[i_id].Data() );
      }    
      
      if(mgd)
	makeGridDirect (sh, "IFAE_LOCALGROUPDISK", "srm://srmifae.pic.es", "dcap://dcap.pic.es", false);
      
      
      //Handle Meta-Data
      //set EBeam field
      TString s_ecm  = "8"; //default is 8TeV 
      sh.at(0)->setMetaDouble ("ebeam", (double)getEBeam(sh.at(0)));
      s_ecm = Form("%.0f", sh.at(0)->getMetaDouble ("ebeam")*2); 

      //  First try reading meta-data from SUSYTools
      readSusyMeta(sh,Form("$ROOTCOREBIN/data/SUSYTools/susy_crosssections_%sTeV.txt", s_ecm.Data()));

      //  then fetch missing meta-data from AMI
      fetchMetaData (sh, false); //do not override as default. Trust xsection in SUSYTools for the moment.
      
      isData = (sh.at(0)->getMetaDouble( MetaFields::isData )==1);
      
      weights.push_back( getLumiWeight(sh.at(0)) );

      TString targetName = Form("SYST_%s_%d.root", samples[i_sample].Data(), run_ids[i_id]);
      mergeList.push_back(TString(CollateralPath)+"/"+targetName);

      for(unsigned int i_syst=0; i_syst < systematics.size(); i_syst++){ //systs loop
	TString torun = Form("run_chorizo %s -i=%d %s %s", allopts.Data(), i_id, args[i_sample].Data(), systematics[i_syst].Data());
	cout << "RUNNING :  " << torun << endl;
	system(torun.Data());
      }//end of systematics loop
      
      //remove sample from SH
      sh.remove(sh.at(0));

    }//end of id loop


    //MERGING STEP!
    //** after-burner to merge samples, add weights and anti-SF
    cout << bold("\n\n*** AFTER-BURNER *** ") << endl;

    std::vector<TString> tmp_mergeList;
    for(unsigned int i_syst=0; i_syst < systematics.size(); i_syst++){ //systs loop    

      tmp_mergeList.clear();

      cout << "\n >> merging "<< mergeList.size() << " sample(s) ...\n" << endl;
      for (unsigned int i=0; i < mergeList.size(); i++){
	tmp_mergeList.push_back( mergeList[i].Copy().ReplaceAll("SYST", systematics.at(i_syst)) );
	cout<<"\t\t" << i << ": " << tmp_mergeList[i] << endl;
      }      
      cout << endl;

      TString mergedName = Form("%s_%s.root",systematics[i_syst].Data(), samples[i_sample].Data());
    
      if (!generatePUfile){
	if (!doAnaTree) {
	  //--- Case where we run on 1 file
	  //... 
	} 
	else {
	  tadd(tmp_mergeList, weights, FinalPath+"/"+mergedName, isData);
	}
      }
    }//end of syst loop
  }//end of samples loop

  return 0;
}


