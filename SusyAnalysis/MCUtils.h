#ifndef MCUTILS_H
#define MCUTILS_H

#include <vector>
#include "TLorentzVector.h"

//fwd declares
namespace xAOD{
  class TruthParticle_v1;
  typedef TruthParticle_v1 TruthParticle;
  class TruthVertex_v1;
  typedef TruthVertex_v1 TruthVertex;
  /* class TruthParticle; */
  /* class TruthVertex; */
}

//---MC
bool isSignal_WIMP(int mc_id);
bool isSignal_ADD(int mc_id);

float boson_sherpa_stop_charm(const Int_t mc_n,
			      std::vector<int> mc_status,
			      std::vector<int> mc_pdgId,
			      std::vector<float> mc_pt,
			      std::vector<float> mc_eta,
			      std::vector<float> mc_phi,
			      std::vector<float> mc_m);

//---aux
float BtagEta(float eta); //saturates eta to 2.5 (btag calib defined region)

float getAsymmetry(float a, float b);


//---truth 
bool  is_Electron(int pid);
bool  is_Muon(int pid);
bool  is_Tau(int pid);
bool  is_Lepton(int pid);
bool  is_Neutrino(int pid);
bool  is_LepNeut(int pid);
bool  is_HF(int pid);
bool  is_Stable(int status);
bool  is_Hard(int status);

bool  isHard(const xAOD::TruthParticle* p);
bool  isStable(const xAOD::TruthParticle* p);

bool  isLepton(const xAOD::TruthParticle* p);
bool  isElectron(const xAOD::TruthParticle* p);
bool  isMuon(const xAOD::TruthParticle* p);
bool  isTau(const xAOD::TruthParticle* p);
bool  isNeutrino(const xAOD::TruthParticle* p);
bool  isLepNeut(const xAOD::TruthParticle* p);
bool  isHF(const xAOD::TruthParticle* p);


#endif //MCUTILS_H
