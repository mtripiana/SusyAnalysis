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
#include "EventLoop/TorqueDriver.h"

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
  cout << bold("run_chorizo [options] <Sample>") << endl;
  cout << endl;
  cout << " <Sample> : The sample name to run over. " << endl;
  cout << "            To list the implemented samples do: 'run samples'" << endl;
  cout << endl;
  cout << "" << endl;
  cout << " [options] : supported option flags" << endl;
  cout << "       -j=<jOption>  : choose which analysis you want to run over. ( = 'METbb'(default), 'Stop', 'Monojet')	" << endl;		    
  cout << "       -l            : run locally (default)   " << endl;
  cout << "       -b            : run in batch            " << endl;
  cout << "       -p            : run on the grid (prun)        " << endl;
  cout << "       -g            : run on the grid (ganga)        " << endl;
  cout << "       -x            : switch to 'at3_xxl' queue (when running in batch mode) (default='at3')  " << endl;
  cout << "       -t            : to run just a test over 50 events " <<endl;
  cout << "       -v=<V>            : output version. To tag output files. (adds a '_vV' suffix)" <<endl;
  cout << "       -n=<N>        : to run over N  events " <<endl;
  cout << "       -s=<SystList> : systematics to be considered (comma-separated list) [optional]. " << endl;
  cout << "                       Just nominal is run by default (Nom)." << endl;       
  cout << "                       To list the available (recommended) systematic variations do: 'run slist'" << endl;
  cout << "                       or well run './SusyAnalysis/scripts/list_systematics.sh'" << endl; 
  cout << "       -o=<outDir>   : where all the output is saved (if left empty is reads the path from the xml jobOption (FinalPath))." << endl;
  cout << endl;
}

float getLumiWeight(Sample* sample){

  if( sample->getMetaDouble( MetaFields::isData ) ) //don't weight data!
    return 1.;

  return sample->getMetaDouble( MetaFields::crossSection ) * sample->getMetaDouble( MetaFields::kfactor ) * sample->getMetaDouble( MetaFields::filterEfficiency ) / sample->getMetaDouble( MetaFields::numEvents ); 
  
}


float getECM(Sample* sample){ //in TeV
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();
  std::string s_ecm = getCmdOutput( "ami dataset info "+newName+" | grep ECMEnergy | awk '{print $2}'");
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

bool is_data(Sample* sample){ 
  TString sampleName(sample->getMetaString( MetaFields::sampleName ));
  std::string newName = stripName(sampleName).Data();
  std::string itis = getCmdOutput( "ami dataset info "+newName+" | grep beamType | awk '{print $2}'");
  return (itis=="collisions");
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

void printSystList(){
  Systematics Smap;
  Smap.LoadList();
  Smap.printSystList();
}


int main( int argc, char* argv[] ) {

  std::vector<TString> args,opts;
  bool runLocal = true;
  bool runBatch = false;
  bool runPrun  = false;
  bool runGrid  = false;

  std::string jOption = "METbb";
  TString queue = "at3";
  bool quick_test = false;
  TString version="";
  bool genPU=false;
  int single_id = -1;


  //parse input arguments
  for (int i=1 ; i < argc ; i++) {
    if( std::string(argv[i]).find("-") != std::string::npos )// is option
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
    TString torun = Form("run_chorizo slist");
    system(torun.Data());
    return 0;
  }

  //config options
  int nMax=-1;
  TString syst_str="";
  TString outDir="";
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
    else if (opts[iop] == "t"){ //limit run to 50 events
      quick_test = true;
    }
    else if (opts[iop] == "u" ){ //generate pileup file (overrides jOption config)
      genPU = true;
    }
    else if (opts[iop].BeginsWith("s") ){
      syst_str = opts[iop].Copy().ReplaceAll("s=","");
    }
    else if (opts[iop].BeginsWith("o") ){
      outDir = opts[iop].Copy().ReplaceAll("o=","");
    }
    else if (opts[iop].BeginsWith("i") ){
      single_id = opts[iop].ReplaceAll("i=","").Atoi();
    }
    else if (opts[iop].BeginsWith("j") ){
      jOption = opts[iop].Copy().ReplaceAll("j=","");
    }
    else if (opts[iop].BeginsWith("n") ){ //limit run to n events
      nMax = opts[iop].Copy().ReplaceAll("n=","").Atoi();
    }
    // else if (opts[iop].BeginsWith("v") ){
    //   version = opts[iop].ReplaceAll("v=","");
    // }
  }


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
      gSystem->Exec("source $ANALYSISCODE/SusyAnalysis/scripts/submit_batch.sh "+queue+" "+syst_str+" "+jOption+" "+samples[isp]);
    }
    return 0;
  }
  //***

  TString allopts=""; //join all options in one string
  for (unsigned int iopt=0; iopt < opts.size(); iopt++)
    allopts += " -"+opts[iopt]+" ";


  //check for needed setup 
  std::string ami_check = gSystem->GetFromPipe("which ami 2>/dev/null").Data(); 
  if (ami_check.empty()){
    cout << bold(red("\n Ups! "));
    cout << "You need to setup a few things first!" << endl;
    cout << " Please do: " << endl;
    cout << "\n   source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh   \n" << endl;
    cout << "\n ...but ok! this time I'll try to do it for you...  :) \n" << endl;
    gSystem->Exec("source $ANALYSISCODE/SusyAnalysis/scripts/grid_up_pwin.sh");
  }


  //*** Read some input options
  //  std::string DirectoryPath=gSystem->Getenv("ANALYSISCODE");
  std::string maindir = getenv("ROOTCOREBIN");

  std::string xmlPath=maindir+"/data/SusyAnalysis/"+jOption+"_JobOption.xml";

  XMLReader *xmlJobOption = new XMLReader();
  xmlJobOption->readXML(xmlPath);

  bool doAnaTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/doTree");
  bool doFlowTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoCutFlow");
  bool doPUTree = false; //No option in the xml yet!!
  bool generatePUfile = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/GeneratePileupFiles");

  TString FinalPath      = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/RootFilesFolder").c_str());
  if( args.size() > 2 ) FinalPath = args[2];    // Take the submit directory from the input if provided:

  TString CollateralPath = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/PartialRootFilesFolder").c_str());

  //override jOptions if required
  if(genPU){
    doAnaTree=false;
    doFlowTree=false;
    doPUTree=false;
    generatePUfile=true;
    cout << "INFO :: config has been forced to PURW-file writing mode! (by user request)" << endl;
  }


  //Do not run systematics if generating pile-up files!
  if(generatePUfile){
    systematics.clear();
    systematics.push_back("Nom");
  }


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
      
      if(single_id>=0 && run_ids[i_id] != single_id) continue; //pick only chosen id (if given)

      //** Run on local samples
      if(runLocal){
	if( run_patterns[i_id].Contains("/afs/") || run_patterns[i_id].Contains("/nfs/") || run_patterns[i_id].Contains("/tmp/")){//local samples
	  scanDir( sh, run_patterns[i_id].Data() );
	  //.Find( run_patterns[i_id].Data() );
	}
	else{//PIC samples
	  scanDQ2 (sh, run_patterns[i_id].Data() );
	  mgd=true;
	}
      }
      else if(runBatch){
	//** Run in batch 
	scanDQ2 (sh, run_patterns[i_id].Data() );
	mgd=true;
      }    
      else if(runGrid || runPrun){
	//** Run on the grid
	scanDQ2 (sh, run_patterns[i_id].Data() );
      }    
      
      if(mgd)
	makeGridDirect (sh, "IFAE_LOCALGROUPDISK", "srm://srmifae.pic.es", "dcap://dcap.pic.es", false);
      
      
      sh.print();
      

      //*** Handle Meta-Data
      sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

      //override name with 'provenance' name (weal attempt to allow for user-skimmed and partially-transfered samples)
      //save original name in a new meta field
      for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){             
	(*iter)->setMetaString( "inputName",  (*iter)->getMetaString( MetaFields::sampleName) ); //I think no longer needed
	(*iter)->setMetaString( MetaFields::gridName,  stripName( TString( (*iter)->getMetaString( MetaFields::sampleName))).Data() );

	//set Data flag correctly
	(*iter)->setMetaString( MetaFields::isData, (is_data(*iter) ? "Y" : "N") );
      }

      //set EBeam field
      TString s_ecm  = "8"; //default is 8TeV 
      sh.at(0)->setMetaDouble ("ebeam", (double)getEBeam(sh.at(0)));
      s_ecm = Form("%.0f", sh.at(0)->getMetaDouble ("ebeam")*2); 

      bool amiFound=true;
      if(s_ecm=="0"){ //if sample not found (e.g. user-made) set to default  //FIX_ME do something about this?
	s_ecm="8";
	amiFound=false;
      }

      //set ID
      sh.at(0)->setMetaDouble( "DSID", (double)run_ids[i_id] );

      //  fetch meta-data from AMI
      if(amiFound)
	fetchMetaData (sh, false); 

      //  then override some meta-data from SUSYTools
      readSusyMeta(sh,Form("$ROOTCOREBIN/data/SUSYTools/susy_crosssections_%sTeV.txt", s_ecm.Data()));

      isData = (sh.at(0)->getMetaString( MetaFields::isData )=="Y"); //global flag. It means we can't run MC and data at the same time. Probably ok?
      
      if(!isData)
	weights.push_back( getLumiWeight(sh.at(0)) );
      else
	weights.push_back( 1. );

      TString targetName = Form("SYST_%s_%d.root", samples[i_sample].Data(), run_ids[i_id]);
      mergeList.push_back(TString(CollateralPath)+"/"+targetName);

      for(unsigned int i_syst=0; i_syst < systematics.size(); i_syst++){ //systs loop
	TString torun = Form("run_chorizo %s -i=%d %s %s", allopts.Data(), run_ids[i_id], args[i_sample].Data(), systematics[i_syst].Data());
	system(torun.Data());
      }//end of systematics loop
      
      //remove sample from SH
      sh.remove(sh.at(0));

    }//end of id loop


    //Done merging step only if running locally
    if(!runLocal) return 0;


    //Don't merge if generating pile=up files
    if(generatePUfile) return 0;

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


