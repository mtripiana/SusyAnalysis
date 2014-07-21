#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/OutputStream.h"
#include "EventLoopAlgs/NTupleSvc.h"

#include "SusyAnalysis/chorizo.h"

void SystTranslate(   TString syste,
		      SystErr::Syste &syst_ST,
		      ScaleVariatioReweighter::variation &syst_Scale,
		      pileupErr::pileupSyste &syst_PU,
		      JvfUncErr::JvfSyste &syst_JVF,
		      BCHCorrMediumErr::BCHSyste &syst_BCH0 );

int main( int argc, char* argv[] ) {

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
  if( argc > 2 ) FinalPath = argv[ 2 ];    // Take the submit directory from the input if provided:

  TString CollateralPath = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/PartialRootFilesFolder").c_str());

  //--- path to the file containing the list of dataset                                                                                                                             
  std::string listDirectory = xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/ListFolderPath");


  // Construct the samples to run on:
  SH::SampleHandler sh;
  //SH::scanDir( sh, "/afs/cern.ch/atlas/project/PAT/xAODs/r5534/" );
  SH::scanDir( sh, "/nfs/at3/scratch/tripiana/xAOD/"); //mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.recon.AOD.e1727_s1933_s1911_r5591/");

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  //Add NtupleSvc
  EL::OutputStream output  ("output");
  job.outputAdd (output);
  EL::NTupleSvc *ntuple = new EL::NTupleSvc ("output");
  ntuple->treeName("AnalysisTree");
  job.algsAdd (ntuple);
  
  //Systematics
  SystErr::Syste syst_ST = SystErr::NONE;
  ScaleVariatioReweighter::variation syst_Scale = ScaleVariatioReweighter::None;
  pileupErr::pileupSyste syst_PU = pileupErr::NONE;
  JvfUncErr::JvfSyste syst_JVF = JvfUncErr::NONE;
  BCHCorrMediumErr::BCHSyste syst_BCH = BCHCorrMediumErr::NONE;
  
  TString systematic="Nom";
    if(argc > 1) 
      systematic = argv[1];
  
  SystTranslate(systematic, syst_ST, syst_Scale, syst_PU, syst_JVF, syst_BCH);
  
  
  chorizo *alg = new chorizo();
  
  //Alg config options here
  alg->outputName = "output";
  alg->Region = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Mode/name/setDefinitionRegion"));

  alg->defaultRegion = "SR"; //get it from the XML!!
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

  alg->syst_ST    = syst_ST;      // Systematics
  alg->syst_Scale = syst_Scale;
  alg->syst_PU    = syst_PU;
  alg->syst_JVF   = syst_JVF;
  alg->syst_BCH   = syst_BCH;


  alg->printMet      = false;     //debug printing
  alg->printJet      = false;
  alg->printElectron = false;
  alg->printMuon     = false;

  //Load alg to job
  job.algsAdd( alg );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, TString(CollateralPath+"/test/").Data() );

  return 0;
}


void SystTranslate( TString syste,
		    SystErr::Syste &syst_ST,
		    ScaleVariatioReweighter::variation &syst_Scale,
		    pileupErr::pileupSyste &syst_PU,
		    JvfUncErr::JvfSyste &syst_JVF,
		    BCHCorrMediumErr::BCHSyste &syst_BCH )
{

  if(syste=="Nom")                                                    syst_ST = SystErr::NONE;
  else if(syste=="JESHigh")                                           syst_ST = SystErr::JESUP;
  else if(syste=="JESLow")                                            syst_ST = SystErr::JESDOWN;

  else if(syste=="EffectiveNP_1Low")                                  syst_ST = SystErr::EffectiveNP_1_Down;
  else if(syste=="EffectiveNP_1High")                                 syst_ST = SystErr::EffectiveNP_1_Up;
  else if(syste=="EffectiveNP_2Low")                                  syst_ST = SystErr::EffectiveNP_2_Down;
  else if(syste=="EffectiveNP_2High")                                 syst_ST = SystErr::EffectiveNP_2_Up;
  else if(syste=="EffectiveNP_3Low")                                  syst_ST = SystErr::EffectiveNP_3_Down;
  else if(syste=="EffectiveNP_3High")                                 syst_ST = SystErr::EffectiveNP_3_Up;
  else if(syste=="EffectiveNP_4Low")                                  syst_ST = SystErr::EffectiveNP_4_Down;
  else if(syste=="EffectiveNP_4High")                                 syst_ST = SystErr::EffectiveNP_4_Up;
  else if(syste=="EffectiveNP_5Low")                                  syst_ST = SystErr::EffectiveNP_5_Down;
  else if(syste=="EffectiveNP_5High")                                 syst_ST = SystErr::EffectiveNP_5_Up;
  else if(syste=="EffectiveNP_6Low")                                  syst_ST = SystErr::EffectiveNP_6_Down;
  else if(syste=="EffectiveNP_6High")                                 syst_ST = SystErr::EffectiveNP_6_Up;
  else if(syste=="EtaIntercalibration_ModellingLow")                  syst_ST = SystErr::EtaIntercalibration_Modelling_Down;
  else if(syste=="EtaIntercalibration_ModellingHigh")                 syst_ST = SystErr::EtaIntercalibration_Modelling_Up;
  else if(syste=="EtaIntercalibration_StatAndMethodLow")              syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Down;
  else if(syste=="EtaIntercalibration_StatAndMethodHigh")             syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Up;
  else if(syste=="SingleParticle_HighPtLow")                          syst_ST = SystErr::SingleParticle_HighPt_Down;
  else if(syste=="SingleParticle_HighPtHigh")                         syst_ST = SystErr::SingleParticle_HighPt_Up;
  else if(syste=="RelativeNonClosure_Pythia8Low")                     syst_ST = SystErr::RelativeNonClosure_Pythia8_Down;
  else if(syste=="RelativeNonClosure_Pythia8High")                    syst_ST = SystErr::RelativeNonClosure_Pythia8_Up;
  else if(syste=="PileupOffsetTermMuLow")                             syst_ST = SystErr::PileupOffsetTermMuDown;
  else if(syste=="PileupOffsetTermMuHigh")                            syst_ST = SystErr::PileupOffsetTermMuUp;
  else if(syste=="PileupOffsetTermNPVLow")                            syst_ST = SystErr::PileupOffsetTermNPVDown;
  else if(syste=="PileupOffsetTermNPVHigh")                           syst_ST = SystErr::PileupOffsetTermNPVUp;
  else if(syste=="PileupPtTermLow")                                   syst_ST = SystErr::PileupPtTermDown;
  else if(syste=="PileupPtTermHigh")                                  syst_ST = SystErr::PileupPtTermUp;
  else if(syste=="PileupRhoTopologyLow")                              syst_ST = SystErr::PileupRhoTopologyDown;
  else if(syste=="PileupRhoTopologyHigh")                             syst_ST = SystErr::PileupRhoTopologyUp;
  else if(syste=="CloseByLow")                                        syst_ST = SystErr::CloseByDown;
  else if(syste=="CloseByHigh")                                       syst_ST = SystErr::CloseByUp;
  else if(syste=="FlavorCompUncertLow")                               syst_ST = SystErr::FlavorCompUncertDown;
  else if(syste=="FlavorCompUncertHigh")                              syst_ST = SystErr::FlavorCompUncertUp;
  else if(syste=="FlavorResponseUncertLow")                           syst_ST = SystErr::FlavorResponseUncertDown;
  else if(syste=="FlavorResponseUncertHigh")                          syst_ST = SystErr::FlavorResponseUncertUp;
  else if(syste=="BJesLow")                                           syst_ST = SystErr::BJesDown;
  else if(syste=="BJesHigh")                                          syst_ST = SystErr::BJesUp; //Break down of the JES uncertainties                                           

  else if(syste=="JER")                                               syst_ST = SystErr::JER;
  else if(syste=="EGZEEHigh")                                         syst_ST = SystErr::EGZEEUP;
  else if(syste=="EGZEELow")                                          syst_ST = SystErr::EGZEEDOWN;
  else if(syste=="EGMATHigh")                                         syst_ST = SystErr::EGMATUP;
  else if(syste=="EGMATLow")                                          syst_ST = SystErr::EGMATDOWN;
  else if(syste=="EGPSHigh")                                          syst_ST = SystErr::EGPSUP;
  else if(syste=="EGPSLow")                                           syst_ST = SystErr::EGPSDOWN;
  else if(syste=="EGLOWHigh")                                         syst_ST = SystErr::EGLOWUP;
  else if(syste=="EGLOWLow")                                          syst_ST = SystErr::EGLOWDOWN;
  else if(syste=="EGRESHigh")                                         syst_ST = SystErr::EGRESUP;
  else if(syste=="EGRESLow")                                          syst_ST = SystErr::EGRESDOWN;
  else if(syste=="EEFFHigh")                                          syst_ST = SystErr::EEFFUP;
  else if(syste=="EEFFLow")                                           syst_ST = SystErr::EEFFDOWN;
  else if(syste=="MSCALEHigh")                                        syst_ST = SystErr::MSCALEUP;
  else if(syste=="MSCALELow")                                         syst_ST = SystErr::MSCALELOW;
  else if(syste=="MMSHigh")                                           syst_ST = SystErr::MMSUP;
  else if(syste=="MMSLow")                                            syst_ST = SystErr::MMSLOW;
  else if(syste=="MIDHigh")                                           syst_ST = SystErr::MIDUP;
  else if(syste=="MIDLow")                                            syst_ST = SystErr::MIDLOW;
  else if(syste=="MEFFHigh")                                          syst_ST = SystErr::MEFFUP;
  else if(syste=="MEFFLow")                                           syst_ST = SystErr::MEFFDOWN;
  else if(syste=="SCALESTHigh")                                       syst_ST = SystErr::SCALESTUP;
  else if(syste=="SCALESTLow")                                        syst_ST = SystErr::SCALESTDOWN;
  else if(syste=="BJETHigh")                                          syst_ST = SystErr::BJETUP;
  else if(syste=="BJETLow")                                           syst_ST = SystErr::BJETDOWN;
  else if(syste=="CJETHigh")                                          syst_ST = SystErr::CJETUP;
  else if(syste=="CJETLow")                                           syst_ST = SystErr::CJETDOWN;
  else if(syste=="BMISTAGHigh")                                       syst_ST = SystErr::BMISTAGUP;
  else if(syste=="BMISTAGLow")                                        syst_ST = SystErr::BMISTAGDOWN;
  else if(syste=="RESOST")                                            syst_ST = SystErr::RESOST;
  else if(syste=="PileupHigh") {                                      syst_ST = SystErr::NONE; syst_PU = pileupErr::PileupHigh;}
  else if(syste=="PileupLow")  {                                      syst_ST = SystErr::NONE; syst_PU = pileupErr::PileupLow;}
  else if(syste=="JvfUncHigh") {                                      syst_ST = SystErr::NONE; syst_JVF = JvfUncErr::JvfUncHigh;}
  else if(syste=="JvfUncLow")  {                                      syst_ST = SystErr::NONE; syst_JVF = JvfUncErr::JvfUncLow;}
  //--- for Z/W+jet Sherpa                                                                                                                                                          
  else if(syste=="ktfacHigh")  {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::ktfacUP;}
  else if(syste=="ktfacLow")   {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::ktfacDOWN;}
  else if(syste=="qfacHigh")   {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::qfacUP;}
  else if(syste=="qfacLow")    {                                      syst_ST = SystErr::NONE; syst_Scale = ScaleVariatioReweighter::qfacDOWN;}
  //--- For BCH medium correction                                                                                                                                                   
  else if(syste=="BCHCorrHigh") {                                     syst_ST = SystErr::NONE; syst_BCH = BCHCorrMediumErr::BCHCorrHigh; }
  else if(syste=="BCHCorrLow") {                                      syst_ST = SystErr::NONE; syst_BCH = BCHCorrMediumErr::BCHCorrLow; }

  else Fatal("run_chorizo", Form("No systematic named like this: %s",syste.Data()));

}
