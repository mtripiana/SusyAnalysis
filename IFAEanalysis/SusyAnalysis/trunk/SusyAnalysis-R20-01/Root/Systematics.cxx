
#include "SusyAnalysis/Systematics.h"

Systematics::Systematics(){
  LoadList();
}

Systematics::~Systematics(){}

std::vector<TString> Systematics::getSystList(){
  std::vector<TString> fList;
  fList.reserve( m_CP_list.size() + m_ST_list.size() + m_user_list.size() ); // preallocate memory
  fList.insert( fList.end(), m_CP_list.begin(), m_CP_list.end() );
  fList.insert( fList.end(), m_ST_list.begin(), m_ST_list.end() );
  fList.insert( fList.end(), m_user_list.begin(), m_user_list.end() );

  return fList;
}

void Systematics::printSystList(){

  std::vector<TString>::const_iterator itr;

  std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  std::cout << "\n List of CP recommended systematics" << std::endl; 
  std::cout << "--------------------------------------------------" << std::endl;
  for(itr = m_CP_list.begin(); itr != m_CP_list.end(); ++itr)
    std::cout << *itr << std::endl;

  std::cout << "\n List of user systematics" << std::endl; 
  std::cout << "--------------------------------------------------" << std::endl;
  for(itr = m_user_list.begin(); itr != m_user_list.end(); ++itr)
    std::cout << *itr << std::endl;

  std::cout << "\n List of SUSYTools systematics (OBSOLETE)" << std::endl; 
  std::cout << "--------------------------------------------------" << std::endl;
  for(itr = m_ST_list.begin(); itr != m_ST_list.end(); ++itr)
    std::cout << *itr << std::endl;

  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n" << std::endl;
}

 
void Systematics::LoadList(){

  //CP registry
  m_CP_list.push_back("EIGEN_B_0__1down");
  m_CP_list.push_back("EIGEN_B_0__1up");
  m_CP_list.push_back("EIGEN_B_1__1down");
  m_CP_list.push_back("EIGEN_B_1__1up");
  m_CP_list.push_back("EIGEN_B_2__1down");
  m_CP_list.push_back("EIGEN_B_2__1up");
  m_CP_list.push_back("EIGEN_B_3__1down");
  m_CP_list.push_back("EIGEN_B_3__1up");
  m_CP_list.push_back("EIGEN_B_4__1down");
  m_CP_list.push_back("EIGEN_B_4__1up");
  m_CP_list.push_back("EIGEN_B_5__1down");
  m_CP_list.push_back("EIGEN_B_5__1up");
  m_CP_list.push_back("EIGEN_B_6__1down");
  m_CP_list.push_back("EIGEN_B_6__1up");
  m_CP_list.push_back("EIGEN_B_7__1down");
  m_CP_list.push_back("EIGEN_B_7__1up");
  m_CP_list.push_back("EIGEN_B_8__1down");
  m_CP_list.push_back("EIGEN_B_8__1up");
  m_CP_list.push_back("EIGEN_B_9__1down");
  m_CP_list.push_back("EIGEN_B_9__1up");
  m_CP_list.push_back("EIGEN_C_0__1down");
  m_CP_list.push_back("EIGEN_C_0__1up");
  m_CP_list.push_back("EIGEN_C_1__1down");
  m_CP_list.push_back("EIGEN_C_1__1up");
  m_CP_list.push_back("EIGEN_C_2__1down");
  m_CP_list.push_back("EIGEN_C_2__1up");
  m_CP_list.push_back("EIGEN_C_3__1down");
  m_CP_list.push_back("EIGEN_C_3__1up");
  m_CP_list.push_back("EIGEN_Light_0__1down");
  m_CP_list.push_back("EIGEN_Light_0__1up");
  m_CP_list.push_back("EIGEN_Light_10__1down");
  m_CP_list.push_back("EIGEN_Light_10__1up");
  m_CP_list.push_back("EIGEN_Light_11__1down");
  m_CP_list.push_back("EIGEN_Light_11__1up");
  m_CP_list.push_back("EIGEN_Light_1__1down");
  m_CP_list.push_back("EIGEN_Light_1__1up");
  m_CP_list.push_back("EIGEN_Light_2__1down");
  m_CP_list.push_back("EIGEN_Light_2__1up");
  m_CP_list.push_back("EIGEN_Light_3__1down");
  m_CP_list.push_back("EIGEN_Light_3__1up");
  m_CP_list.push_back("EIGEN_Light_4__1down");
  m_CP_list.push_back("EIGEN_Light_4__1up");
  m_CP_list.push_back("EIGEN_Light_5__1down");
  m_CP_list.push_back("EIGEN_Light_5__1up");
  m_CP_list.push_back("EIGEN_Light_6__1down");
  m_CP_list.push_back("EIGEN_Light_6__1up");
  m_CP_list.push_back("EIGEN_Light_7__1down");
  m_CP_list.push_back("EIGEN_Light_7__1up");
  m_CP_list.push_back("EIGEN_Light_8__1down");
  m_CP_list.push_back("EIGEN_Light_8__1up");
  m_CP_list.push_back("EIGEN_Light_9__1down");
  m_CP_list.push_back("EIGEN_Light_9__1up");
  m_CP_list.push_back("EIGEN_T_0__1down");
  m_CP_list.push_back("EIGEN_T_0__1up");
  m_CP_list.push_back("EIGEN_T_1__1down");
  m_CP_list.push_back("EIGEN_T_1__1up");
  m_CP_list.push_back("EIGEN_T_2__1down");
  m_CP_list.push_back("EIGEN_T_2__1up");
  m_CP_list.push_back("EIGEN_T_3__1down");
  m_CP_list.push_back("EIGEN_T_3__1up");
  m_CP_list.push_back("MUONSFSTAT__1down");
  m_CP_list.push_back("MUONSFSTAT__1up");
  m_CP_list.push_back("MUONSFSYS__1down");
  m_CP_list.push_back("MUONSFSYS__1up");
  m_CP_list.push_back("MUONS_ID__1down");
  m_CP_list.push_back("MUONS_ID__1up");
  m_CP_list.push_back("MUONS_MS__1down");
  m_CP_list.push_back("MUONS_MS__1up");
  m_CP_list.push_back("MUONS_SCALE__1down");
  m_CP_list.push_back("MUONS_SCALE__1up");
  m_CP_list.push_back("extrapolation__1down");
  m_CP_list.push_back("extrapolation__1up");
  m_CP_list.push_back("EffectiveNP_1__continuous");
  m_CP_list.push_back("EffectiveNP_2__continuous");
  m_CP_list.push_back("EffectiveNP_3__continuous");
  m_CP_list.push_back("EffectiveNP_4__continuous");
  m_CP_list.push_back("EffectiveNP_5__continuous");
  m_CP_list.push_back("EffectiveNP_6restTerm__continuous");
  m_CP_list.push_back("EtaIntercalibration_Modelling__continuous");
  m_CP_list.push_back("EtaIntercalibration_StatAndMethod__continuous");
  m_CP_list.push_back("Flavor_Composition__continuous");
  m_CP_list.push_back("Flavor_Response__continuous");
  m_CP_list.push_back("Pileup_OffsetMu__continuous");
  m_CP_list.push_back("Pileup_OffsetNPV__continuous");
  m_CP_list.push_back("Pileup_PtTerm__continuous");
  m_CP_list.push_back("Pileup_RhoTopology__continuous");
  m_CP_list.push_back("RelativeNonClosure_MC12a__continuous");
  m_CP_list.push_back("SingleParticle_HighPt__continuous");
  

  //Others (pile-up, jvf, scales, BCH, ...)
  m_user_list.push_back("PileupHigh");
  m_user_list.push_back("PileupLow");

  m_user_list.push_back("JvfUncHigh");
  m_user_list.push_back("JvfUncLow");

  m_user_list.push_back("ktfacHigh");
  m_user_list.push_back("ktfacLow");
  m_user_list.push_back("qfacHigh");
  m_user_list.push_back("qfacLow");

  m_user_list.push_back("BCHCorrHigh");
  m_user_list.push_back("BCHCorrLow");
 
  
  //(old)SUSYTools systematics
  m_ST_list.push_back("JESHigh");                                           
  m_ST_list.push_back("JESLow");                                            

  m_ST_list.push_back("EffectiveNP_1Low");                                  
  m_ST_list.push_back("EffectiveNP_1High");                                 
  m_ST_list.push_back("EffectiveNP_2Low");                                  
  m_ST_list.push_back("EffectiveNP_2High");                                 
  m_ST_list.push_back("EffectiveNP_3Low");                                  
  m_ST_list.push_back("EffectiveNP_3High");                                 
  m_ST_list.push_back("EffectiveNP_4Low");                                  
  m_ST_list.push_back("EffectiveNP_4High");                                 
  m_ST_list.push_back("EffectiveNP_5Low");                                  
  m_ST_list.push_back("EffectiveNP_5High");                                 
  m_ST_list.push_back("EffectiveNP_6Low");                                  
  m_ST_list.push_back("EffectiveNP_6High");                                 
  m_ST_list.push_back("EtaIntercalibration_ModellingLow");                  
  m_ST_list.push_back("EtaIntercalibration_ModellingHigh");                 
  m_ST_list.push_back("EtaIntercalibration_StatAndMethodLow");              
  m_ST_list.push_back("EtaIntercalibration_StatAndMethodHigh");             
  m_ST_list.push_back("SingleParticle_HighPtLow");                          
  m_ST_list.push_back("SingleParticle_HighPtHigh");                         
  m_ST_list.push_back("RelativeNonClosure_Pythia8Low");                     
  m_ST_list.push_back("RelativeNonClosure_Pythia8High");                    
  m_ST_list.push_back("PileupOffsetTermMuLow");                             
  m_ST_list.push_back("PileupOffsetTermMuHigh");                            
  m_ST_list.push_back("PileupOffsetTermNPVLow");                            
  m_ST_list.push_back("PileupOffsetTermNPVHigh");                           
  m_ST_list.push_back("PileupPtTermLow");                                   
  m_ST_list.push_back("PileupPtTermHigh");                                  
  m_ST_list.push_back("PileupRhoTopologyLow");                              
  m_ST_list.push_back("PileupRhoTopologyHigh");                             
  m_ST_list.push_back("CloseByLow");                                        
  m_ST_list.push_back("CloseByHigh");                                       
  m_ST_list.push_back("FlavorCompUncertLow");                               
  m_ST_list.push_back("FlavorCompUncertHigh");                              
  m_ST_list.push_back("FlavorResponseUncertLow");                           
  m_ST_list.push_back("FlavorResponseUncertHigh");                          
  m_ST_list.push_back("BJesLow");                                           
  m_ST_list.push_back("BJesHigh");                                                                                 

  m_ST_list.push_back("JER");                                               
  m_ST_list.push_back("EGZEEHigh");                                         
  m_ST_list.push_back("EGZEELow");                                          
  m_ST_list.push_back("EGMATHigh");                                         
  m_ST_list.push_back("EGMATLow");                                          
  m_ST_list.push_back("EGPSHigh");                                          
  m_ST_list.push_back("EGPSLow");                                           
  m_ST_list.push_back("EGLOWHigh");                                         
  m_ST_list.push_back("EGLOWLow");                                          
  m_ST_list.push_back("EGRESHigh");                                         
  m_ST_list.push_back("EGRESLow");                                          
  m_ST_list.push_back("EEFFHigh");                                          
  m_ST_list.push_back("EEFFLow");                                           
  m_ST_list.push_back("MSCALEHigh");                                        
  m_ST_list.push_back("MSCALELow");                                         
  m_ST_list.push_back("MMSHigh");                                           
  m_ST_list.push_back("MMSLow");                                            
  m_ST_list.push_back("MIDHigh");                                           
  m_ST_list.push_back("MIDLow");                                            
  m_ST_list.push_back("MEFFHigh");                                          
  m_ST_list.push_back("MEFFLow");                                           
  m_ST_list.push_back("SCALESTHigh");                                       
  m_ST_list.push_back("SCALESTLow");                                        
  m_ST_list.push_back("BJETHigh");                                          
  m_ST_list.push_back("BJETLow");                                           
  m_ST_list.push_back("CJETHigh");                                          
  m_ST_list.push_back("CJETLow");                                           
  m_ST_list.push_back("BMISTAGHigh");                                       
  m_ST_list.push_back("BMISTAGLow");                                        
  m_ST_list.push_back("RESOST");                                            
 
}

bool Systematics::implemented(const TString &syst, const std::vector<TString> &Slist)
{
  return std::find(Slist.begin(), Slist.end(), syst) != Slist.end();
}

void Systematics::SystTranslate( TString syste,
				 CP::SystematicSet &syst_CP,
				 SystErr::Syste &syst_ST,
				 ScaleVariatioReweighter::variation &syst_Scale,
				 pileupErr::pileupSyste &syst_PU,
				 JvfUncErr::JvfSyste &syst_JVF,
				 BCHCorrMediumErr::BCHSyste &syst_BCH )
{
  //Init to Nominal per default
  syst_CP = CP::SystematicSet();
  syst_ST = SystErr::NONE;
  syst_Scale = ScaleVariatioReweighter::None;
  syst_PU = pileupErr::NONE;
  syst_JVF = JvfUncErr::NONE;
  syst_BCH = BCHCorrMediumErr::NONE;

  //Check implementation
  if(syste=="Nom")                                                    return;
  else if( implemented(syste, this->m_CP_list) )                      syst_CP.insert( CP::SystematicVariation(std::string(syste.Data())));
  else if( implemented(syste.Copy().ReplaceAll("__1up",""), this->m_CP_list) )  syst_CP.insert( CP::SystematicVariation(std::string(syste.Copy().ReplaceAll("__continuous__1up","").Data()), 1));
  else if( implemented(syste.Copy().ReplaceAll("__1down",""), this->m_CP_list) )  syst_CP.insert( CP::SystematicVariation(std::string(syste.Copy().ReplaceAll("__continuous__1down","").Data()), 1));

  //obsolete!!
  // else if(syste=="JESHigh")                                           syst_ST = SystErr::JESUP;
  // else if(syste=="JESLow")                                            syst_ST = SystErr::JESDOWN;
  
  // else if(syste=="EffectiveNP_1Low")                                  syst_ST = SystErr::EffectiveNP_1_Down;
  // else if(syste=="EffectiveNP_1High")                                 syst_ST = SystErr::EffectiveNP_1_Up;
  // else if(syste=="EffectiveNP_2Low")                                  syst_ST = SystErr::EffectiveNP_2_Down;
  // else if(syste=="EffectiveNP_2High")                                 syst_ST = SystErr::EffectiveNP_2_Up;
  // else if(syste=="EffectiveNP_3Low")                                  syst_ST = SystErr::EffectiveNP_3_Down;
  // else if(syste=="EffectiveNP_3High")                                 syst_ST = SystErr::EffectiveNP_3_Up;
  // else if(syste=="EffectiveNP_4Low")                                  syst_ST = SystErr::EffectiveNP_4_Down;
  // else if(syste=="EffectiveNP_4High")                                 syst_ST = SystErr::EffectiveNP_4_Up;
  // else if(syste=="EffectiveNP_5Low")                                  syst_ST = SystErr::EffectiveNP_5_Down;
  // else if(syste=="EffectiveNP_5High")                                 syst_ST = SystErr::EffectiveNP_5_Up;
  // else if(syste=="EffectiveNP_6Low")                                  syst_ST = SystErr::EffectiveNP_6_Down;
  // else if(syste=="EffectiveNP_6High")                                 syst_ST = SystErr::EffectiveNP_6_Up;
  // else if(syste=="EtaIntercalibration_ModellingLow")                  syst_ST = SystErr::EtaIntercalibration_Modelling_Down;
  // else if(syste=="EtaIntercalibration_ModellingHigh")                 syst_ST = SystErr::EtaIntercalibration_Modelling_Up;
  // else if(syste=="EtaIntercalibration_StatAndMethodLow")              syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Down;
  // else if(syste=="EtaIntercalibration_StatAndMethodHigh")             syst_ST = SystErr::EtaIntercalibration_StatAndMethod_Up;
  // else if(syste=="SingleParticle_HighPtLow")                          syst_ST = SystErr::SingleParticle_HighPt_Down;
  // else if(syste=="SingleParticle_HighPtHigh")                         syst_ST = SystErr::SingleParticle_HighPt_Up;
  // else if(syste=="RelativeNonClosure_Pythia8Low")                     syst_ST = SystErr::RelativeNonClosure_Pythia8_Down;
  // else if(syste=="RelativeNonClosure_Pythia8High")                    syst_ST = SystErr::RelativeNonClosure_Pythia8_Up;
  // else if(syste=="PileupOffsetTermMuLow")                             syst_ST = SystErr::PileupOffsetTermMuDown;
  // else if(syste=="PileupOffsetTermMuHigh")                            syst_ST = SystErr::PileupOffsetTermMuUp;
  // else if(syste=="PileupOffsetTermNPVLow")                            syst_ST = SystErr::PileupOffsetTermNPVDown;
  // else if(syste=="PileupOffsetTermNPVHigh")                           syst_ST = SystErr::PileupOffsetTermNPVUp;
  // else if(syste=="PileupPtTermLow")                                   syst_ST = SystErr::PileupPtTermDown;
  // else if(syste=="PileupPtTermHigh")                                  syst_ST = SystErr::PileupPtTermUp;
  // else if(syste=="PileupRhoTopologyLow")                              syst_ST = SystErr::PileupRhoTopologyDown;
  // else if(syste=="PileupRhoTopologyHigh")                             syst_ST = SystErr::PileupRhoTopologyUp;
  // else if(syste=="CloseByLow")                                        syst_ST = SystErr::CloseByDown;
  // else if(syste=="CloseByHigh")                                       syst_ST = SystErr::CloseByUp;
  // else if(syste=="FlavorCompUncertLow")                               syst_ST = SystErr::FlavorCompUncertDown;
  // else if(syste=="FlavorCompUncertHigh")                              syst_ST = SystErr::FlavorCompUncertUp;
  // else if(syste=="FlavorResponseUncertLow")                           syst_ST = SystErr::FlavorResponseUncertDown;
  // else if(syste=="FlavorResponseUncertHigh")                          syst_ST = SystErr::FlavorResponseUncertUp;
  // else if(syste=="BJesLow")                                           syst_ST = SystErr::BJesDown;
  // else if(syste=="BJesHigh")                                          syst_ST = SystErr::BJesUp; //Break down of the JES uncertainties                                           

  // else if(syste=="JER")                                               syst_ST = SystErr::JER;
  // else if(syste=="EGZEEHigh")                                         syst_ST = SystErr::EGZEEUP;
  // else if(syste=="EGZEELow")                                          syst_ST = SystErr::EGZEEDOWN;
  // else if(syste=="EGMATHigh")                                         syst_ST = SystErr::EGMATUP;
  // else if(syste=="EGMATLow")                                          syst_ST = SystErr::EGMATDOWN;
  // else if(syste=="EGPSHigh")                                          syst_ST = SystErr::EGPSUP;
  // else if(syste=="EGPSLow")                                           syst_ST = SystErr::EGPSDOWN;
  // else if(syste=="EGLOWHigh")                                         syst_ST = SystErr::EGLOWUP;
  // else if(syste=="EGLOWLow")                                          syst_ST = SystErr::EGLOWDOWN;
  // else if(syste=="EGRESHigh")                                         syst_ST = SystErr::EGRESUP;
  // else if(syste=="EGRESLow")                                          syst_ST = SystErr::EGRESDOWN;
  // else if(syste=="EEFFHigh")                                          syst_ST = SystErr::EEFFUP;
  // else if(syste=="EEFFLow")                                           syst_ST = SystErr::EEFFDOWN;
  // else if(syste=="MSCALEHigh")                                        syst_ST = SystErr::MSCALEUP;
  // else if(syste=="MSCALELow")                                         syst_ST = SystErr::MSCALELOW;
  // else if(syste=="MMSHigh")                                           syst_ST = SystErr::MMSUP;
  // else if(syste=="MMSLow")                                            syst_ST = SystErr::MMSLOW;
  // else if(syste=="MIDHigh")                                           syst_ST = SystErr::MIDUP;
  // else if(syste=="MIDLow")                                            syst_ST = SystErr::MIDLOW;
  // else if(syste=="MEFFHigh")                                          syst_ST = SystErr::MEFFUP;
  // else if(syste=="MEFFLow")                                           syst_ST = SystErr::MEFFDOWN;
  // else if(syste=="SCALESTHigh")                                       syst_ST = SystErr::SCALESTUP;
  // else if(syste=="SCALESTLow")                                        syst_ST = SystErr::SCALESTDOWN;
  //else if(syste=="RESOST")                                            syst_ST = SystErr::RESOST;
  else if(syste=="BJETHigh")                                          syst_ST = SystErr::BJETUP;
  else if(syste=="BJETLow")                                           syst_ST = SystErr::BJETDOWN;
  else if(syste=="CJETHigh")                                          syst_ST = SystErr::CJETUP;
  else if(syste=="CJETLow")                                           syst_ST = SystErr::CJETDOWN;
  else if(syste=="BMISTAGHigh")                                       syst_ST = SystErr::BMISTAGUP;
  else if(syste=="BMISTAGLow")                                        syst_ST = SystErr::BMISTAGDOWN;

  //-- Pileup
  else if(syste=="PileupHigh") {                                      syst_PU = pileupErr::PileupHigh;}
  else if(syste=="PileupLow")  {                                      syst_PU = pileupErr::PileupLow;}

  //-- JVF
  else if(syste=="JvfUncHigh") {                                      syst_JVF = JvfUncErr::JvfUncHigh;}
  else if(syste=="JvfUncLow")  {                                      syst_JVF = JvfUncErr::JvfUncLow;}
  
  //--- for Z/W+jet Sherpa                                                                                                 
  else if(syste=="ktfacHigh")  {                                      syst_Scale = ScaleVariatioReweighter::ktfacUP;}
  else if(syste=="ktfacLow")   {                                      syst_Scale = ScaleVariatioReweighter::ktfacDOWN;}
  else if(syste=="qfacHigh")   {                                      syst_Scale = ScaleVariatioReweighter::qfacUP;}
  else if(syste=="qfacLow")    {                                      syst_Scale = ScaleVariatioReweighter::qfacDOWN;}
  
  //--- For BCH medium correction                                                                                                
  else if(syste=="BCHCorrHigh") {                                     syst_BCH = BCHCorrMediumErr::BCHCorrHigh; }
  else if(syste=="BCHCorrLow") {                                      syst_BCH = BCHCorrMediumErr::BCHCorrLow; }

  else Error("Systematics::SystTranslate()", Form("No systematic named like this: %s",syste.Data()));

}


