//////////////////////////
// SusyAnalysis Utilities
//////////////////////////

#include "SusyAnalysis/MCUtils.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

#include <limits>

//==== MC ==========//
//WIMP signal
bool isSignal_WIMP(int mc_id){
   
   if((mc_id>=159565 && mc_id<=159684) || (mc_id>=182328 && mc_id<=182431))
     return true;
   
   return false;      
};
 
//ADD signal
bool isSignal_ADD(int mc_id){
  
  if (mc_id>=158382 && mc_id<=158396)
    return true;
  
  return false;      
};

/// Sherpa MassiveCB W/Z reweighting used in the stop to charm analysis: reweighting of the boson pT to data (section 8.1 of the supporting note ATL-COM-PHYS-2013-064 
float boson_sherpa_stop_charm(const Int_t mc_n,
			      std::vector<int> mc_status,
			      std::vector<int> mc_pdgId,
			      std::vector<float> mc_pt,
			      std::vector<float> mc_eta,
			      std::vector<float> mc_phi,
			      std::vector<float> mc_m){
  float weight = 1.;
  float pt = 0.;
  
  /// Calculate the boson pt
  TLorentzVector V;
  
  TLorentzVector l1;
  TLorentzVector l2;
  
  bool foundFirst = false;
  bool foundSecond = false; 
  
  for ( int j = 0; j < mc_n; ++j){
    if (mc_status[j] == 3 && fabs(mc_pdgId[j]) >= 11 && fabs(mc_pdgId[j]) <= 16){
      if (!foundFirst){
	l1.SetPtEtaPhiM(mc_pt[j],mc_eta[j], mc_phi[j], mc_m[j]);
	foundFirst = true;
      } else if(!foundSecond){
	l2.SetPtEtaPhiM(mc_pt[j],mc_eta[j], mc_phi[j], mc_m[j]);
	foundSecond = true;
      } else {
	std::cout << "Error in boson_sherpa_stop_charm : more than 2 leptons" << std::endl;
	return weight;
      }
    }
  }
  
  if (!foundSecond) {
    std::cout << "Error in boson_sherpa_stop_charm : unable to find 2 leptons" << std::endl;
    return weight;
  }
  else {
    V = l1 + l2;
    pt = V.Pt()*0.001;
  }
  
  if(pt<50)       weight = 2.456;
  else if(pt<100) weight = 1.351;
  else if(pt<150) weight = 0.946;
  else if(pt<200) weight = 1.018;
  else if(pt<250) weight = 0.954;
  else if(pt<300) weight = 0.995;
  else if(pt<350) weight = 0.791;
  else if(pt<400) weight = 0.908;
  else            weight = 0.712;
  
  return weight;
}

//==== AUX ==========//


//==== TRUTH ==========//

// Implement HepMC isGenStable
bool isStable(const xAOD::TruthParticle* p){ 
  //*** from CPAnalysisExamples::isGenStable( p );
  if( p->barcode() > 200000 ) return false;
  if( p->pdgId() == 21 && p->e() == 0 ) return false;
  if( p->status()%1000 == 1 ) return true;
  if( p->hasDecayVtx() ){
    const xAOD::TruthVertex* dvtx = p->decayVtx();
    if( dvtx ){
      if( p->status() == 2 && dvtx->barcode() < -200000 ) return true;
    }
  }
  if( p->status()%1000 == 2 && p->status() > 1000 ) return true;
  return false;
    
  //  return isStableP( p->status() ); };
}

bool  is_Electron(int pid){ return (fabs(pid)==11); };
bool  is_Muon(int pid){ return (fabs(pid)==13); };
bool  is_Tau(int pid){ return (fabs(pid)==15); };
bool  is_Lepton(int pid){ return (is_Electron(pid) || is_Muon(pid) || is_Tau(pid)); };
bool  is_Neutrino(int pid){ return (fabs(pid)==12 || fabs(pid)==14 || fabs(pid)==16); };
bool  is_LepNeut(int pid){ return (is_Lepton(pid) || is_Neutrino(pid)); };
bool  is_HF(int pid){ return (fabs(pid)==4 || fabs(pid)==5); };
bool  is_Stable(int status){ return (status==1); };
bool  is_Hard(int status){ return (status==3); };

bool  isHard(const xAOD::TruthParticle* p){ return is_Hard( p->status() ); };
bool  isLepton(const xAOD::TruthParticle *p){ return is_Lepton( p->pdgId() ); };
bool  isElectron(const xAOD::TruthParticle *p){ return is_Electron( p->pdgId() ); };
bool  isMuon(const xAOD::TruthParticle *p){ return is_Muon( p->pdgId() ); };
bool  isTau(const xAOD::TruthParticle *p){ return is_Tau( p->pdgId() ); };
bool  isNeutrino(const xAOD::TruthParticle* p){ return is_Neutrino( p->pdgId() ); };
bool  isLepNeut(const xAOD::TruthParticle* p){ return is_LepNeut( p->pdgId() ); };
bool  isHF(const xAOD::TruthParticle* p){ return is_HF( p->pdgId() ); };
