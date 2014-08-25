#include "SusyAnalysis/particles.h"

using namespace Particles;

// Default constructor for Particle
Particle::Particle(){
  Pt_up = 0;
  Pt_down = 0;
  Ht = 0;
  isGood = true;
  isIsolated = false;
  isTight = false;
  ptcone20 = 0;
  etcone20 = 0;
  ptcone30 = 0;
  etcone30 = 0;
  charge = 0;
  SF = 1;
  SFu = 0;
  SFd = 0;
  gev=false; //4mom in GeV?
  id = -1;
}

Particle::~Particle(){}

TLorentzVector Particle::GetVector(){
  return TLorentzVector(*this);
}

void Particle::SetVector(TLorentzVector vec, bool inGeV){
  if(inGeV)
    this->SetPtEtaPhiE(vec.Pt()*0.001, vec.Eta(), vec.Phi(), vec.E()*0.001);
  else
    this->SetPtEtaPhiE(vec.Pt()*0.001, vec.Eta(), vec.Phi(), vec.E()*0.001);

  this->gev=true;
}

void Particle::SetVector(float pt, float eta, float phi, float e, bool inGeV){
  TLorentzVector tlv;
  tlv.SetPtEtaPhiE(pt, eta, phi, e);
  this->SetVector(tlv, inGeV);
}

void Particle::PrintInfo(){
  cout<<"Pt: "<<this->Pt()<<" Pt_up: "<<this->Pt_up<<" Pt_down: "<<this->Pt_down<<endl;
  cout<<"Eta: "<<this->Eta()<<" Phi: "<<this->Phi()<<endl;
  cout<<"SF: "<<this->SF<<" SFu: "<<this->SFu<<" SFd: "<<this->SFd<<endl;
  cout<<endl;
}

// Default constructor for Jet
Jet::Jet(){
  MV1 = -1;
  SV1plusIP3D = -1;
  SV1_pb = -1;
  SV1_pc = -1;
  SV1_pu = -1;
  IP3D_pb = -1;
  IP3D_pc = -1;
  IP3D_pu = -1;
  JetFitterCombNN = -100;
  JetFitterCombNNc = -100;
  JetFitterCu = -100;
  JetFitterCb = -100;
  TagEffSF.first = 1.;
  TagEffSF.second = 0.;
  TagIneffSF.first = 1.;
  TagIneffSF.second = 0.;
  TagEffMC.first = 1.;
  TagEffMC.second = 0.;
  bTagOPw = "";
  FlavorTruth = -1;
  chf=-100.;
  nTrk=-100.;
  sumPtTrk=-100.;  
  emf=-100.;
  time=-100.;
  fsm=-100.;
  width=-100.;
  n90=-100.;
  hecf=-100.;
  HECQuality=-100.;
  NegativeE=-100.;
  LArQuality=-100.;
  jvtxf=-100.;
  failBCHTight=false;
  failBCHMedium=false;
  BCH_CORR_CELL=-100;
  BCH_CORR_JET=-100;
}

Jet::~Jet(){}

bool Jet::isBTagged(TString Tagger){ //--- Check!!!
  if      (Tagger=="MV1"              && (this->MV1 > 0.7892))              {return true;}
  else if (Tagger=="JetFitterCombNN" && (this->JetFitterCombNN > -2.55 && this->JetFitterCombNNc < 1.0)) {return true;}
  else if (Tagger=="JetFitterCombNNc" && (this->JetFitterCombNNc > -3.8 && this->JetFitterCombNNc < 2.2))  {return true;}
 
  return false;
}

bool Jet::isBTagged_80eff(TString Tagger){ //--- Check!!!
  if      (Tagger=="MV1"              && (this->MV1 > 0.3511))              {return true;}

  return false;
}

bool Jet::isTauJet(float metphi, TString Tagger){ //--- Check!!!

  if (this->isBTagged(Tagger)) return false;
  if (this->nTrk >= 5) return false;
  if (deltaPhi(this->Phi(), metphi)>0.2) return false;

  return true;
}


float Jet::getBweight(TString Tagger){
  if (Tagger=="MV1")              return (this->MV1);
  if (Tagger=="JetFitterCOMBNN")  return (this->JetFitterCombNN);
  if (Tagger=="JetFitterCOMBNNc") return (this->JetFitterCombNNc); 
  
  return -1; //not valid Tagger!
}

void Jet::PrintInfo(){
  cout<<"Pt: "<<this->Pt()<<" Pt_up: "<<this->Pt_up<<" Pt_down: "<<this->Pt_down<<endl;
  cout<<"Eta: "<<this->Eta()<<" Phi: "<<this->Phi()<<endl;
  cout<<"Chf: "<<this->chf<<" Emf: "<<this->emf<<" time: "<<this->time<<" fsm: "<<this->fsm<<" width: "<<this->width<<" n90: "<<this->n90<<" hecf: "<<this->hecf<<" HECQuality: "<<this->HECQuality<<" NegativeE: "<<this->NegativeE<<" LArQuality: "<<this->LArQuality<<" jvtxf: "<<this->jvtxf<<"  failBCHTight: "<<this->failBCHTight<<"  failBCHMedium: "<<this->failBCHMedium<<endl;
  cout<<" MV1: "<<this->MV1<<" JetFitterCOMBNN: "<<this->JetFitterCombNN<<" JetFitterCOMBNNc: "<<this->JetFitterCombNNc<<" EffSF: "<<this->TagEffSF.first<<" IneffSF: "<<this->TagIneffSF.first<<" EffMC: "<<TagEffMC.first<<endl;
  cout<<endl;
}



MET::MET(){
  met.Set(0., 0.);
  met_trk.Set(0., 0.);
  met_mu.Set(0., 0.);
  met_reffinal.Set(0., 0.);
  met_lochadtopo.Set(0., 0.);
  met_ecorr.Set(0., 0.);
  m_hasMuons = false;
}

MET::~MET(){}

void MET::SetVector(TVector2 vec, TString which, bool inGeV){
  
  float sf = (inGeV ? 1. : 0.001); 

  if(which=="met_trk"){
    this->met_trk.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_mu"){
    this->met_mu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_refFinal"){
    this->met_reffinal.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_refFinal_mu"){
    this->met_reffinal_mu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_locHadTopo"){
    this->met_lochadtopo.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_ecorr"){
    this->met_ecorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else { //re-computed flavor
    this->met.Set(vec.X() * sf, vec.Y() * sf);
  }
}

TVector2 MET::GetVector(TString which){
  if(which=="met_trk"){
    return TVector2(this->met_trk);
  }
  if(which=="met_mu"){
    return TVector2(this->met_mu);
  }
  if(which=="met_refFinal"){
    return TVector2(this->met_reffinal);
  }
  if(which=="met_refFinal_mu"){
    return TVector2(this->met_reffinal_mu);
  }
  if(which=="met_locHadTopo"){
    return TVector2(this->met_lochadtopo);
  }
  if(which=="met_ecorr"){
    return TVector2(this->met_ecorr);
  }
  return TVector2(this->met);
}

float MET::Phi(TString which){

  return TVector2::Phi_mpi_pi( GetVector(which).Phi() );

};

//--------------------------------- 3D MET -----------------------------
TVector3 MET::GetMET_Razor(){
  
  TVector2 met_2D(this->met);
  double met_x = met_2D.X();
  double met_y = met_2D.Y();
  
  return TVector3(met_x,met_y,0);   
  
}
//----------------------------------------------------------------------

void MET::SetHasMuons(bool hasMuons){
  m_hasMuons = hasMuons;
}

bool MET::GetHasMuons(){
  return m_hasMuons;
}

void MET::PrintInfo(){
  cout<<"MET: "<<this->met.Mod()<<" MET_Trk: "<<this->met_trk.Mod()<<" MET_mu: "<<this->met_mu.Mod()<<endl;
}

void MET::Reset(){
  //--- Probably there's a better way
  met.Set(0., 0.);
  met_trk.Set(0., 0.);
  met_mu.Set(0., 0.);
  met_lochadtopo.Set(0., 0.);
  met_reffinal.Set(0., 0.);
  met_reffinal_mu.Set(0., 0.);
}

bool operator<(const Jet& Jet1, const Jet& Jet2){
    return Jet1.Pt() > Jet2.Pt();
}

bool operator<(const Particle& Particle1, const Particle& Particle2){
    return Particle1.Pt() > Particle2.Pt();
}

bool bw_MV1_sort(const Jet& Jet1, const Jet& Jet2){
  return Jet1.MV1 > Jet2.MV1;
};
