
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
  
  m_CP_list.push_back("EG_RESOLUTION_ALL__1down"); 
  m_CP_list.push_back("EG_RESOLUTION_ALL__1up");   
  m_CP_list.push_back("EG_SCALE_ALL__1down");     
  m_CP_list.push_back("EG_SCALE_ALL__1up");        
  m_CP_list.push_back("EL_EFF_ID_TotalCorrUncertainty__1down");    
  m_CP_list.push_back("EL_EFF_ID_TotalCorrUncertainty__1up");     
  m_CP_list.push_back("EL_EFF_Iso_TotalCorrUncertainty__1down");   
  m_CP_list.push_back("EL_EFF_Iso_TotalCorrUncertainty__1up");   
  m_CP_list.push_back("EL_EFF_Reco_TotalCorrUncertainty__1down"); 
  m_CP_list.push_back("EL_EFF_Reco_TotalCorrUncertainty__1up"); 
  m_CP_list.push_back("EL_EFF_Trigger_TotalCorrUncertainty__1down");
  m_CP_list.push_back("EL_EFF_Trigger_TotalCorrUncertainty__1up");
  m_CP_list.push_back("FT_EFF_B_systematics__1down");    
  m_CP_list.push_back("FT_EFF_B_systematics__1up");      
  m_CP_list.push_back("FT_EFF_C_systematics__1down");     
  m_CP_list.push_back("FT_EFF_C_systematics__1up");     
  m_CP_list.push_back("FT_EFF_Light_systematics__1down");
  m_CP_list.push_back("FT_EFF_Light_systematics__1up");   
  m_CP_list.push_back("FT_EFF_extrapolation__1down");    
  m_CP_list.push_back("FT_EFF_extrapolation__1up");     
  m_CP_list.push_back("FT_EFF_extrapolation from charm__1down");
  m_CP_list.push_back("FT_EFF_extrapolation from charm__1up"); 
  m_CP_list.push_back("JET_GroupedNP_1__1up");   
  m_CP_list.push_back("JET_GroupedNP_1__1down"); 
  m_CP_list.push_back("JET_GroupedNP_2__1up");   
  m_CP_list.push_back("JET_GroupedNP_2__1down"); 
  m_CP_list.push_back("JET_GroupedNP_3__1up");  
  m_CP_list.push_back("JET_GroupedNP_3__1down"); 
  m_CP_list.push_back("JET_JER_SINGLE_NP__1up"); 
  m_CP_list.push_back("MET_SoftTrk_ResoPara");  
  m_CP_list.push_back("MET_SoftTrk_ResoPerp");   
  m_CP_list.push_back("MET_SoftTrk_ScaleDown");  
  m_CP_list.push_back("MET_SoftTrk_ScaleUp");   
  m_CP_list.push_back("MUONS_ID__1down");
  m_CP_list.push_back("MUONS_ID__1up");  
  m_CP_list.push_back("MUONS_MS__1down"); 
  m_CP_list.push_back("MUONS_MS__1up");  
  m_CP_list.push_back("MUONS_SCALE__1down");
  m_CP_list.push_back("MUONS_SCALE__1up");    
  m_CP_list.push_back("MUON_EFF_STAT__1down");
  m_CP_list.push_back("MUON_EFF_STAT__1up");  
  m_CP_list.push_back("MUON_EFF_SYS__1down"); 
  m_CP_list.push_back("MUON_EFF_SYS__1up");  
  m_CP_list.push_back("MUON_EFF_TrigStatUncertainty__1down");  
  m_CP_list.push_back("MUON_EFF_TrigStatUncertainty__1up");    
  m_CP_list.push_back("MUON_EFF_TrigSystUncertainty__1down"); 
  m_CP_list.push_back("MUON_EFF_TrigSystUncertainty__1up");   
  m_CP_list.push_back("MUON_ISO_STAT__1down"); 
  m_CP_list.push_back("MUON_ISO_STAT__1up");   
  m_CP_list.push_back("MUON_ISO_SYS__1down");  
  m_CP_list.push_back("MUON_ISO_SYS__1up");   
  m_CP_list.push_back("PH_EFF_Uncertainty__1down");  
  m_CP_list.push_back("PH_EFF_Uncertainty__1up");    
  m_CP_list.push_back("PRW_DATASF__1down");   
  m_CP_list.push_back("PRW_DATASF__1up");    
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_ELEOLR_TOTAL__1down"); 
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_ELEOLR_TOTAL__1up");   
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_JETID_TOTAL__1down"); 
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_JETID_TOTAL__1up");    
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_RECO_TOTAL__1down");   
  m_CP_list.push_back("TAUS_TRUEHADTAU_EFF_RECO_TOTAL__1up");   
  m_CP_list.push_back("TAUS_TRUEHADTAU_SME_TES_TOTAL__1down");    
  m_CP_list.push_back("TAUS_TRUEHADTAU_SME_TES_TOTAL__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_B_0__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_0__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_B_1__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_1__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_B_2__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_2__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_B_3__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_3__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_B_4__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_4__1up"); 
  m_CP_list.push_back("FT_EFF_Eigen_B_5__1down");
  m_CP_list.push_back("FT_EFF_Eigen_B_5__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_C_0__1down");
  m_CP_list.push_back("FT_EFF_Eigen_C_0__1up"); 
  m_CP_list.push_back("FT_EFF_Eigen_C_1__1down");
  m_CP_list.push_back("FT_EFF_Eigen_C_1__1up"); 
  m_CP_list.push_back("FT_EFF_Eigen_C_2__1down");
  m_CP_list.push_back("FT_EFF_Eigen_C_2__1up"); 
  m_CP_list.push_back("FT_EFF_Eigen_C_3__1down");
  m_CP_list.push_back("FT_EFF_Eigen_C_3__1up"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_0__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_0__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_1__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_1__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_10__1down");
  m_CP_list.push_back("FT_EFF_Eigen_Light_10__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_Light_11__1down");
  m_CP_list.push_back("FT_EFF_Eigen_Light_11__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_Light_12__1down");
  m_CP_list.push_back("FT_EFF_Eigen_Light_12__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_Light_13__1down");
  m_CP_list.push_back("FT_EFF_Eigen_Light_13__1up");  
  m_CP_list.push_back("FT_EFF_Eigen_Light_2__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_2__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_3__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_3__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_4__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_4__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_5__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_5__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_6__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_6__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_7__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_7__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_8__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_8__1up");   
  m_CP_list.push_back("FT_EFF_Eigen_Light_9__1down"); 
  m_CP_list.push_back("FT_EFF_Eigen_Light_9__1up");   


}

bool Systematics::implemented(const TString &syst, const std::vector<TString> &Slist)
{
  return std::find(Slist.begin(), Slist.end(), syst) != Slist.end();
}

void Systematics::SystTranslate( TString syste,
				 CP::SystematicSet &syst_CP,
				 SystErr::Syste &syst_ST,
				 ScaleVariatioReweighter::variation &syst_Scale,
				 pileupErr::pileupSyste &syst_PU )
{
  //Init to Nominal per default
  syst_CP = CP::SystematicSet();
  syst_ST = SystErr::NONE;
  syst_Scale = ScaleVariatioReweighter::None;
  syst_PU = pileupErr::NONE;

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

  //--- for Z/W+jet Sherpa                                                                                                 
  else if(syste=="ktfacHigh")  {                                      syst_Scale = ScaleVariatioReweighter::ktfacUP;}
  else if(syste=="ktfacLow")   {                                      syst_Scale = ScaleVariatioReweighter::ktfacDOWN;}
  else if(syste=="qfacHigh")   {                                      syst_Scale = ScaleVariatioReweighter::qfacUP;}
  else if(syste=="qfacLow")    {                                      syst_Scale = ScaleVariatioReweighter::qfacDOWN;}
  
  else Error("Systematics::SystTranslate()", Form("No systematic named like this: %s",syste.Data()));

}


