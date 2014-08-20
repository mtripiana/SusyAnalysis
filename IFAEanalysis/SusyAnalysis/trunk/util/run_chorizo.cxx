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
  cout << "run_chorizo <Sample> [Syst]" << endl;
  cout << endl;
  cout << " <Sample> : The sample name to run over. " << endl;
  cout << "            To list the implemented samples do: 'run_chorizo samples'" << endl;
  cout << endl;
  cout << " [Syst]   : systematics to be considered (comma-separated list) [optional]. " << endl;
  cout << "            Just nominal is run by default (Nom)." << endl;       
  cout << "            To list the available (recommended) systematic variations do: 'run_chorizo slist'" << endl;
  cout << "            or well run './SusyAnalysis/scripts/list_systematics.sh'" << endl; 
  cout << endl;
}

std::string getCmdOutput(const std::string& mStr)
{
  std::string result, file;
  FILE* pipe{popen((mStr+" 2>/dev/null").c_str(), "r")};
  if (!pipe) return " ERROR something went wrong with command : %s \n"+mStr;
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
  cout << "---- Sample metadata ----------------" << endl;
  cout << " Name            = " << sample->getMetaString( MetaFields::sampleName ) << endl;
  cout << " isData          = " << (sample->getMetaDouble( MetaFields::isData )==0 ? "No" : "Yes") << endl;
  cout << " Xsection        = " << sample->getMetaDouble( MetaFields::crossSection ) << endl;
  cout << " XsectionRelUnc  = " << sample->getMetaDouble( MetaFields::crossSectionRelUncertainty ) << endl;
  cout << " kfactor         = " << sample->getMetaDouble( MetaFields::kfactor ) << endl;
  cout << " filterEff       = " << sample->getMetaDouble( MetaFields::filterEfficiency ) << endl;
  cout << " N               = " << sample->getMetaDouble( MetaFields::numEvents ) << endl;
  cout << " Lumi            = " << sample->getMetaDouble( MetaFields::lumi ) << endl;
  cout << endl;  
}

void printSamplesList(){
  RunsMap rmap;
  std::vector<TString> samples = rmap.getKeys();

  std::cout << "\n List of samples" << std::endl; 
  std::cout << "--------------------------------------------------" << std::endl;
  for(unsigned int i=0; i<samples.size(); i++)
    cout << samples[i] << endl;

}

void printSystList(){
  Systematics Smap;
  Smap.LoadList();
  Smap.printSystList();
}

//void printSystList(){
//   //FIX THIS! It has to happen *after* all tools' initialization!

//   const CP::SystematicRegistry& registry = CP::SystematicRegistry::getInstance();
//   const CP::SystematicSet& recommendedSystematics = registry.recommendedSystematics();
//   std::vector<CP::SystematicSet> sysList;

//   std::cout << "\n List of recommended systematics" << std::endl; 
//   std::cout << "--------------------------------------------------" << std::endl;

//   // this is the nominal set
//   sysList.push_back(CP::SystematicSet());
//   for(CP::SystematicSet::const_iterator sysItr = recommendedSystematics.begin(); sysItr != recommendedSystematics.end(); ++sysItr){
//     std::cout << sysItr->basename();
//     if (*sysItr == CP::SystematicVariation (sysItr->basename(), CP::SystematicVariation::CONTINUOUS)){
//       std::cout << "\t +-" ;
//     }
//     std::cout << std::endl;
//   }
// }

// void SystTranslate(   TString syste,
// 		      CP::SystematicSet& syst_CP,
// 		      SystErr::Syste &syst_ST,
// 		      ScaleVariatioReweighter::variation &syst_Scale,
// 		      pileupErr::pileupSyste &syst_PU,
// 		      JvfUncErr::JvfSyste &syst_JVF,
// 		      BCHCorrMediumErr::BCHSyste &syst_BCH );



int main( int argc, char* argv[] ) {

  //check if enough arguments
  bool systListOnly=false;
  if( argc < 2 ){
    usage();
    return 0;
  }
  else if (strcmp(argv[1], "samples") == 0){ //just print samples list?
    printSamplesList();
    return 0;
  }
  else if (strcmp(argv[1], "slist") == 0){ //just print systematics list?
    systListOnly=true;
    gErrorIgnoreLevel = kFatal;
    argv[1] = (char*)"TestMClocal"; //rename to test sample to get systematics list
    // printSystList();
    // return 0;
  }

  //check for needed setup 
  std::string ami_check = getCmdOutput(R"( which ami )");
  if (ami_check.empty()){
    cout << "\n Ups! You need to setup a few things first!" << endl;
    cout << " Please do: " << endl;
    cout << "\n   source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh" << endl;
    cout << "\n and get back! :)" << endl;
    return 0;
  }


  //*** obsolete
  //--- path to the file containing the list of dataset                                 
  //  std::string listDirectory = xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/ListFolderPath");

  //***Get map of runs  
  RunsMap mapOfRuns;
  //check if sample is mapped
  RMap mymap = mapOfRuns.getMap();
  RMap::iterator it = mymap.find( argv[1] );
  if(it == mymap.end()){
    cout << "\n Sample '" << argv[1] << "' not found in current map. Please pick another one." << endl;
    cout << "            To list the implemented samples do: 'run_chorizo samples'" << endl;
    return 0;
  }


  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Read some config options
  std::string DirectoryPath=gSystem->Getenv("ANALYSISCODE");
  std::string xmlPath=DirectoryPath+"/SusyAnalysis/util/AnalysisJobOptions/METbb_JobOption.xml";

  XMLReader *xmlJobOption = new XMLReader();
  xmlJobOption->readXML(xmlPath);

  bool doAnaTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/doTree");
  bool doFlowTree = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/DoCutFlow");
  bool doPUTree = false; //No option in the xml yet!!
  bool GeneratePUfiles = xmlJobOption->retrieveBool("AnalysisOptions$GeneralSettings$Mode/name/GeneratePileupFiles");

  TString FinalPath      = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/RootFilesFolder").c_str());
  if( argc > 3 ) FinalPath = argv[ 3 ];    // Take the submit directory from the input if provided:

  TString CollateralPath = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/PartialRootFilesFolder").c_str());


  // Get patterns/paths to load for this sample
  std::vector<TString> run_pattern = mapOfRuns.getPatterns( argv[1] );
  std::vector<int>     run_ids     = mapOfRuns.getIDs( argv[1] );

  // Construct the samples to run on:
  SampleHandler sh;
  bool mgd=false;
  for(unsigned int p=0; p < run_pattern.size(); p++){
    
    bool runLocal = (run_pattern[p].Contains("/afs/") || run_pattern[p].Contains("/nfs/") || systListOnly); //Can we refine this?
    
    //** Run on local samples
    //   e.g. scanDir( sh, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/" );
    if(runLocal || systListOnly){
      scanDir( sh, run_pattern[p].Data() );
    }
    else{
      //** Run on PIC samples
      //   e.g. scanDQ2 (sh, "user.eifert.mc12_13TeV.110090*");
      scanDQ2 (sh, run_pattern[p].Data() );
      mgd=true;
    }    
  }
  if(mgd)
    makeGridDirect (sh, "IFAE_LOCALGROUPDISK", "srm://srmifae.pic.es", "dcap://dcap.pic.es", false);

  sh.print(); //print what we found

  //Handle Meta-Data
  sh.setMetaString( "nc_tree", "CollectionTree" ); //it's always the case for xAOD files

  //  First try reading meta-data from SUSYTools
  //   readSusyMetaDir(sh,"$ROOTCOREBIN/data/SUSYTools");
  //   readSusyMeta(sh,"$ROOTCOREBIN/data/SUSYTools/susy_crosssections_13TeV.txt");
  readSusyMeta(sh,"$ROOTCOREBIN/data/SUSYTools/susy_crosssections_14TeV.txt");

  //  then fetch missing meta-data from AMI
  fetchMetaData (sh, false); //do not override as default. Trust xsection in SUSYTools for the moment.

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
    cout << "\nERROR :: You are mixing MC and data samples!! The current implementation does not support this! Sorry...\n" << endl;
    return 0;
  }

  //Systematics
  CP::SystematicSet syst_CP = CP::SystematicSet();
  SystErr::Syste syst_ST = SystErr::NONE;
  ScaleVariatioReweighter::variation syst_Scale = ScaleVariatioReweighter::None;
  pileupErr::pileupSyste syst_PU = pileupErr::NONE;
  JvfUncErr::JvfSyste syst_JVF = JvfUncErr::NONE;
  BCHCorrMediumErr::BCHSyste syst_BCH = BCHCorrMediumErr::NONE;
  
  TString systematic="Nom";
  if(argc > 2) 
    systematic = argv[2];
  
  Systematics Sobj;
  Sobj.LoadList();
  Sobj.SystTranslate(systematic, syst_CP, syst_ST, syst_Scale, syst_PU, syst_JVF, syst_BCH);

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
  alg->genPUfile  = GeneratePUfiles;
    
  alg->syst_CP    = syst_CP;      // Systematics
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
  job.options()->setDouble (EL::Job::optMaxEvents, 10);
  if(systListOnly)
    job.options()->setDouble (EL::Job::optMaxEvents, 1);

  //TTreeCache use
  job.options()->setDouble (EL::Job::optCacheSize, 10*1024*1024);


  //create tmp output dir
  string tmpdir = tmpdirname();
    
  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
    
  //submit the job
  driver.submit( job, tmpdir );

  if(systListOnly) return 0; //that's enough if running systematics list. Leave tmp dir as such.

  //move output to collateral files' path
  TString sampleName,targetName;
  int isample=0;
  for (SampleHandler::iterator iter = sh.begin(); iter != sh.end(); ++ iter){

    sampleName = Form("%s.root",(*iter)->getMetaString( MetaFields::sampleName ).c_str());
    targetName = Form("%s_%s_%d.root", systematic.Data(), argv[1], run_ids[isample]);
    system("cp "+tmpdir+"/data-output/"+sampleName.Data()+" "+CollateralPath+"/"+targetName.Data());
    
    mergeList.push_back(TString(CollateralPath)+"/"+targetName);
    isample++;
  }


  //** after-burner to merge samples, add weights and anti-SF
  cout << "\n\n*** AFTER-BURNER *** " << endl;
  cout << "\n >> merging "<< mergeList.size() << " samples ..." << endl;
  for (unsigned int i=0; i < mergeList.size(); i++){
    cout<<"\t\t" << i << ": " << mergeList[i] << endl;
  }      
  
  TString mergedName = Form("%s_%s.root",systematic.Data(), argv[1]);
  
  if (!GeneratePUfiles){
    if (!doAnaTree) {
      //--- Case where we run on 1 file
      //... 
    } 
    else {
      tadd(mergeList, weights, FinalPath+"/"+mergedName, isData);
    }
  }

  return 0;
}


// void SystTranslate( TString syste,
// 		    CP::SystematicSet &syst_CP,
// 		    SystErr::Syste &syst_ST,
// 		    ScaleVariatioReweighter::variation &syst_Scale,
// 		    pileupErr::pileupSyste &syst_PU,
// 		    JvfUncErr::JvfSyste &syst_JVF,
// 		    BCHCorrMediumErr::BCHSyste &syst_BCH )
// {

  


//   if(syste=="Nom")                                                    return;
//   else if(syste=="MUONS_ID__1up")                                     syst_CP = CP::SystematicSet(syste.Data()); //new scheme test!
//   else if(syste=="JESHigh")                                           syst_ST = SystErr::JESUP;
//   else if(syste=="JESLow")                                            syst_ST = SystErr::JESDOWN;

//   else if(syste=="EffectiveNP_1Low")                                  syst_ST = SystErr::EffectiveNP_1_Down;
//   else if(syste=="EffectiveNP_1High")                                 syst_ST = SystErr::EffectiveNP_1_Up;
//   else if(syste=="EffectiveNP_2Low")                                  syst_ST = SystErr::EffectiveNP_2_Down;
//   else if(syste=="EffectiveNP_2High")                                 syst_ST = SystErr::EffectiveNP_2_Up;
//   else if(syste=="EffectiveNP_3Low")                                  syst_ST = SystErr::EffectiveNP_3_Down;
//   else if(syste=="EffectiveNP_3High")                                 syst_ST = SystErr::EffectiveNP_3_Up;
//   else if(syste=="EffectiveNP_4Low")                                  syst_ST = SystErr::EffectiveNP_4_Down;
//   else if(syste=="EffectiveNP_4High")                                 syst_ST = SystErr::EffectiveNP_4_Up;
//   else if(syste=="EffectiveNP_5Low")                                  syst_ST = SystErr::EffectiveNP_5_Down;
//   else if(syste=="EffectiveNP_5High")                                 syst_ST = SystErr::EffectiveNP_5_Up;
//   else if(syste=="EffectiveNP_6Low")                                  syst_ST = SystErr::EffectiveNP_6_Down;
//   else if(syste=="EffectiveNP_6High")                                 syst_ST = SystErr::EffectiveNP_6_Up;
//   else if(syste=="EtaIntercalibration_ModellingLow")                  syst_ST = SystErr::EtaIntercalibration_Modelling_Down;
//   else if(syste=="EtaIntercalibration_ModellingHigh")                 syst_ST = SystErr::EtaIntercalibration_Modelling_Up;
//   else if(syste=="EtaIntercalibration_StatAndMethodLow")              syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Down;
//   else if(syste=="EtaIntercalibration_StatAndMethodHigh")             syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Up;
//   else if(syste=="SingleParticle_HighPtLow")                          syst_ST = SystErr::SingleParticle_HighPt_Down;
//   else if(syste=="SingleParticle_HighPtHigh")                         syst_ST = SystErr::SingleParticle_HighPt_Up;
//   else if(syste=="RelativeNonClosure_Pythia8Low")                     syst_ST = SystErr::RelativeNonClosure_Pythia8_Down;
//   else if(syste=="RelativeNonClosure_Pythia8High")                    syst_ST = SystErr::RelativeNonClosure_Pythia8_Up;
//   else if(syste=="PileupOffsetTermMuLow")                             syst_ST = SystErr::PileupOffsetTermMuDown;
//   else if(syste=="PileupOffsetTermMuHigh")                            syst_ST = SystErr::PileupOffsetTermMuUp;
//   else if(syste=="PileupOffsetTermNPVLow")                            syst_ST = SystErr::PileupOffsetTermNPVDown;
//   else if(syste=="PileupOffsetTermNPVHigh")                           syst_ST = SystErr::PileupOffsetTermNPVUp;
//   else if(syste=="PileupPtTermLow")                                   syst_ST = SystErr::PileupPtTermDown;
//   else if(syste=="PileupPtTermHigh")                                  syst_ST = SystErr::PileupPtTermUp;
//   else if(syste=="PileupRhoTopologyLow")                              syst_ST = SystErr::PileupRhoTopologyDown;
//   else if(syste=="PileupRhoTopologyHigh")                             syst_ST = SystErr::PileupRhoTopologyUp;
//   else if(syste=="CloseByLow")                                        syst_ST = SystErr::CloseByDown;
//   else if(syste=="CloseByHigh")                                       syst_ST = SystErr::CloseByUp;
//   else if(syste=="FlavorCompUncertLow")                               syst_ST = SystErr::FlavorCompUncertDown;
//   else if(syste=="FlavorCompUncertHigh")                              syst_ST = SystErr::FlavorCompUncertUp;
//   else if(syste=="FlavorResponseUncertLow")                           syst_ST = SystErr::FlavorResponseUncertDown;
//   else if(syste=="FlavorResponseUncertHigh")                          syst_ST = SystErr::FlavorResponseUncertUp;
//   else if(syste=="BJesLow")                                           syst_ST = SystErr::BJesDown;
//   else if(syste=="BJesHigh")                                          syst_ST = SystErr::BJesUp; //Break down of the JES uncertainties                                           

//   else if(syste=="JER")                                               syst_ST = SystErr::JER;
//   else if(syste=="EGZEEHigh")                                         syst_ST = SystErr::EGZEEUP;
//   else if(syste=="EGZEELow")                                          syst_ST = SystErr::EGZEEDOWN;
//   else if(syste=="EGMATHigh")                                         syst_ST = SystErr::EGMATUP;
//   else if(syste=="EGMATLow")                                          syst_ST = SystErr::EGMATDOWN;
//   else if(syste=="EGPSHigh")                                          syst_ST = SystErr::EGPSUP;
//   else if(syste=="EGPSLow")                                           syst_ST = SystErr::EGPSDOWN;
//   else if(syste=="EGLOWHigh")                                         syst_ST = SystErr::EGLOWUP;
//   else if(syste=="EGLOWLow")                                          syst_ST = SystErr::EGLOWDOWN;
//   else if(syste=="EGRESHigh")                                         syst_ST = SystErr::EGRESUP;
//   else if(syste=="EGRESLow")                                          syst_ST = SystErr::EGRESDOWN;
//   else if(syste=="EEFFHigh")                                          syst_ST = SystErr::EEFFUP;
//   else if(syste=="EEFFLow")                                           syst_ST = SystErr::EEFFDOWN;
//   else if(syste=="MSCALEHigh")                                        syst_ST = SystErr::MSCALEUP;
//   else if(syste=="MSCALELow")                                         syst_ST = SystErr::MSCALELOW;
//   else if(syste=="MMSHigh")                                           syst_ST = SystErr::MMSUP;
//   else if(syste=="MMSLow")                                            syst_ST = SystErr::MMSLOW;
//   else if(syste=="MIDHigh")                                           syst_ST = SystErr::MIDUP;
//   else if(syste=="MIDLow")                                            syst_ST = SystErr::MIDLOW;
//   else if(syste=="MEFFHigh")                                          syst_ST = SystErr::MEFFUP;
//   else if(syste=="MEFFLow")                                           syst_ST = SystErr::MEFFDOWN;
//   else if(syste=="SCALESTHigh")                                       syst_ST = SystErr::SCALESTUP;
//   else if(syste=="SCALESTLow")                                        syst_ST = SystErr::SCALESTDOWN;
//   else if(syste=="BJETHigh")                                          syst_ST = SystErr::BJETUP;
//   else if(syste=="BJETLow")                                           syst_ST = SystErr::BJETDOWN;
//   else if(syste=="CJETHigh")                                          syst_ST = SystErr::CJETUP;
//   else if(syste=="CJETLow")                                           syst_ST = SystErr::CJETDOWN;
//   else if(syste=="BMISTAGHigh")                                       syst_ST = SystErr::BMISTAGUP;
//   else if(syste=="BMISTAGLow")                                        syst_ST = SystErr::BMISTAGDOWN;
//   else if(syste=="RESOST")                                            syst_ST = SystErr::RESOST;
//   else if(syste=="PileupHigh") {                                      syst_ST = SystErr::NONE; syst_PU = pileupErr::PileupHigh;}
//   else if(syste=="PileupLow")  {                                      syst_ST = SystErr::NONE; syst_PU = pileupErr::PileupLow;}
//   else if(syste=="JvfUncHigh") {                                      syst_ST = SystErr::NONE; syst_JVF = JvfUncErr::JvfUncHigh;}
//   else if(syste=="JvfUncLow")  {                                      syst_ST = SystErr::NONE; syst_JVF = JvfUncErr::JvfUncLow;}
//   //--- for Z/W+jet Sherpa                                                                                                                                                          
//   else if(syste=="ktfacHigh")  {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::ktfacUP;}
//   else if(syste=="ktfacLow")   {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::ktfacDOWN;}
//   else if(syste=="qfacHigh")   {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::qfacUP;}
//   else if(syste=="qfacLow")    {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::qfacDOWN;}
//   //--- For BCH medium correction                                                                                                                                                   
//   else if(syste=="BCHCorrHigh") {                                     syst_ST = SystErr::NONE; syst_BCH = BCHCorrMediumErr::BCHCorrHigh; }
//   else if(syste=="BCHCorrLow") {                                      syst_ST = SystErr::NONE; syst_BCH = BCHCorrMediumErr::BCHCorrLow; }

//   else Fatal("run_chorizo", Form("No systematic named like this: %s",syste.Data()));

// }
