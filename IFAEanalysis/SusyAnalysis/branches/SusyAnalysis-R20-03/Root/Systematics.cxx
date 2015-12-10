#include "SusyAnalysis/Systematics.h"

Systematics::Systematics(){
  LoadList();
}

Systematics::~Systematics(){}

std::vector<TString> Systematics::getSystList(){
  std::vector<TString> fList;
  fList.reserve( m_CP_list.size() ); // preallocate memory
  fList.insert( fList.end(), m_CP_list.begin(), m_CP_list.end() );

  return fList;
}

int Systematics::getSystID(TString name){

  auto it = std::find(m_CP_list.begin(), m_CP_list.end(), name);
  if(it != m_CP_list.end())
    return std::distance(m_CP_list.begin(), it);

  return 999; //not implemented!

}

TString Systematics::getSystName(int id){

  if(id<0 || id>m_CP_list.size()) return "Unknown";

  return m_CP_list.at(id);
}

void Systematics::printSystList(){
  
  std::vector<TString>::const_iterator itr;
  
  std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  std::cout << "\n List of CP recommended systematics" << std::endl; 
  std::cout << "--------------------------------------------------" << std::endl;
  for(itr = m_CP_list.begin(); itr != m_CP_list.end(); ++itr)
    std::cout << *itr << std::endl;
  
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n" << std::endl;
}

 
void Systematics::LoadList(){

  //CP registry
  //***  PLEASE DO NOT MODIFY THE ORDER OF THIS LIST!  JUST APPEND WHAT YOU NEED ***
  //TODO:  move to map (to avoid this risk)
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
				 CP::SystematicSet &syst_CP)
{
  //Init to Nominal per default
  syst_CP = CP::SystematicSet();

  //Check implementation
  if(syste=="Nom")                                                    
    return;

  else if( implemented(syste, this->m_CP_list) )                      
    syst_CP.insert( CP::SystematicVariation(std::string(syste.Data())));

  else if( implemented(syste.Copy().ReplaceAll("__1up",""), this->m_CP_list) )  
    syst_CP.insert( CP::SystematicVariation(std::string(syste.Copy().ReplaceAll("__continuous__1up","").Data()), 1));
  
  else if( implemented(syste.Copy().ReplaceAll("__1down",""), this->m_CP_list) ) 
    syst_CP.insert( CP::SystematicVariation(std::string(syste.Copy().ReplaceAll("__continuous__1down","").Data()), 1));
  
  else Error("Systematics::SystTranslate()", Form("No systematic named like this: %s",syste.Data()));
  
}


