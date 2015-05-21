#include "SusyAnalysis/particles.h"

ClassImp(Particle);
ClassImp(Jet);
ClassImp(MET);

using namespace Particles;

// Default constructor for Particle
Particle::Particle(){

  gev=false; //4mom in GeV?
  index = -1;

  type = 0; //as defined in MCTruthClassifier::ParticleType
  origin = 0; //as defined in MCTruthClassifier::ParticleOrigin

  isGood = true;
  id = -1;
  isTight = false;

  isTrigMatch = false;

  isIsolated = false;
  isoTight = 0.;
  isoLoose = 0.;
  isoGradient = 0.;
  ptcone20 = 0;
  etcone20 = 0;
  ptcone30 = 0;
  etcone30 = 0;

  d0_sig = 0;
  z0 = 0;
  charge = 0;

  Pt_up = 0;
  Pt_down = 0;
  SF = 1;
  SFu = 0;
  SFd = 0;
}

Particle::~Particle(){}

TLorentzVector Particle::GetVector() const{
  return TLorentzVector(*this);
}

TVector2 Particle::GetVector2() const{
  return TVector2(this->Px(), this->Py());
}

void Particle::SetVector(TLorentzVector vec, bool inGeV){ //inGeV=true if it is already in GeV!
  if(inGeV)
    this->SetPtEtaPhiE(vec.Pt(), vec.Eta(), vec.Phi(), vec.E());
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
  isbjet = false;
  MV1 = -1;
  MV2c20 = -1;
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

bool Jet::isBTagged(TString Tagger, float op){ 
  //from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks
  if      (Tagger=="MV1")            return (this->MV1 > op);
  else if (Tagger=="MV2c20")         return (this->MV2c20 > op);
  else if (Tagger=="IP3DSV1")        return (this->SV1plusIP3D > op);
  else if (Tagger=="Truth")          return (abs(this->FlavorTruth)==5);
  // else if (Tagger=="JetFitterCombNN"  && (this->JetFitterCombNN > op && this->JetFitterCombNNc < 1.0)) {return true;}//57-80% b eff 
  // else if (Tagger=="JetFitterCombNNc" && (this->JetFitterCombNNc > -3.8 && this->JetFitterCombNNc < 2.2))  {return true;}
 
  return false;
}


bool Jet::isBTagged_70eff(TString Tagger){ 
  //from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks
  if      (Tagger=="MV1")              return (this->MV1 > 0.7892); //70% b eff  
  else if (Tagger=="MV2c20")           return (this->MV2c20 > 0.0314 ); //70% b eff  
  else if (Tagger=="IP3DSV1")          return (this->SV1plusIP3D > 1.85); //70% b eff 
  else if (Tagger=="Truth")            return (abs(this->FlavorTruth)==5); 
  return false;
}

bool Jet::isBTagged_80eff(TString Tagger){ 
  if      (Tagger=="MV1")              return (this->MV1 > 0.3511); //80% b eff  
  else if (Tagger=="MV2c20")           return (this->MV2c20 > -0.5517); //80% b eff  
  else if (Tagger=="IP3DSV1")          return (this->SV1plusIP3D > -0.70); //80% b eff 
  else if (Tagger=="Truth")            return (abs(this->FlavorTruth)==5); 
  return false;
}

bool Jet::isTauJet(float metphi, TString Tagger){ //--- Check!!!

  if (this->isBTagged_70eff(Tagger)) return false;
  if (this->nTrk >= 5) return false;
  if (deltaPhi(this->Phi(), metphi)>0.2) return false;

  return true;
}


float Jet::getBweight(TString Tagger){
  if (Tagger=="MV1")              return (this->MV1);
  if (Tagger=="MV2c20")           return (this->MV2c20);
  if (Tagger=="IP3DSV1")          return (this->SV1plusIP3D);
  else if (Tagger=="Truth")       return (abs(this->FlavorTruth)); 
  
  return -1; //not valid Tagger!
}

void Jet::PrintInfo(){
  cout<<"Pt: "<<this->Pt()<<" Pt_up: "<<this->Pt_up<<" Pt_down: "<<this->Pt_down<<endl;
  cout<<"Eta: "<<this->Eta()<<" Phi: "<<this->Phi()<<endl;
  cout<<"Chf: "<<this->chf<<" Emf: "<<this->emf<<" time: "<<this->time<<" fsm: "<<this->fsm<<" width: "<<this->width<<" n90: "<<this->n90<<" hecf: "<<this->hecf<<" HECQuality: "<<this->HECQuality<<" NegativeE: "<<this->NegativeE<<" LArQuality: "<<this->LArQuality<<" jvtxf: "<<this->jvtxf<<"  failBCHTight: "<<this->failBCHTight<<"  failBCHMedium: "<<this->failBCHMedium<<endl;
  cout<<" MV1: "<<this->MV1<<" MV2c20: "<<this->MV2c20<<" EffSF: "<<this->TagEffSF.first<<" IneffSF: "<<this->TagIneffSF.first<<" EffMC: "<<TagEffMC.first<<endl;
  cout<<endl;
}



MET::MET(){
  met_imu.Set(0., 0.);
  met_vmu.Set(0., 0.);  
  met_tst_imu.Set(0., 0.);
  met_tst_vmu.Set(0., 0.);  
  met_tst_imu_ecorr.Set(0., 0.);
  met_tst_vmu_ecorr.Set(0., 0.);
  met_tst_vmu_mucorr.Set(0., 0.);
  met_trk.Set(0., 0.);
  met_vmu_mucorr.Set(0., 0.);
  met_vmu_ecorr.Set(0., 0.);
  met_reffinal_imu.Set(0., 0.);
  met_reffinal_vmu.Set(0., 0.);  
  met_lochadtopo.Set(0., 0.);
  met_imu_ecorr.Set(0., 0.);
  met_imu_phcorr.Set(0., 0.);
  met_vmu_phcorr.Set(0., 0.);  
  met_truth_imu.Set(0., 0.);
  met_truth_vmu.Set(0., 0.);    

  m_hasMuons = false;
  gev=false;
}

MET::~MET(){}

void MET::SetVector(TVector2 vec, TString which, bool inGeV){
  
  float sf = (inGeV ? 1. : 0.001); 

  this->gev=true; //book it always in GeV
  
  if(which=="met_imu"){
    this->met_imu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_vmu"){
    this->met_vmu.Set(vec.X() * sf, vec.Y() * sf);
  }  
  else if(which=="met_tst_imu"){
    this->met_tst_imu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_tst_vmu"){
    this->met_tst_vmu.Set(vec.X() * sf, vec.Y() * sf);
  }  
  else if(which=="met_tst_vmu_mucorr"){
    this->met_tst_vmu_mucorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_tst_vmu_ecorr"){
    this->met_tst_vmu_ecorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_tst_imu_ecorr"){
    this->met_tst_imu_ecorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_trk"){
    this->met_trk.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_vmu_mucorr"){
    this->met_vmu_mucorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_vmu_ecorr"){
    this->met_vmu_ecorr.Set(vec.X() * sf, vec.Y() * sf);
  }    
  else if(which=="met_refFinal_imu"){
    this->met_reffinal_imu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_refFinal_vmu"){
    this->met_reffinal_vmu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_locHadTopo"){
    this->met_lochadtopo.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_imu_ecorr"){
    this->met_imu_ecorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_imu_phcorr"){
    this->met_imu_phcorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_vmu_phcorr"){
    this->met_vmu_phcorr.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_truth_imu"){
    this->met_truth_imu.Set(vec.X() * sf, vec.Y() * sf);
  }
  else if(which=="met_truth_vmu"){
    this->met_truth_vmu.Set(vec.X() * sf, vec.Y() * sf);
  }  

}

TVector2 MET::GetVector(TString which){

  if(which=="met_imu"){
    return TVector2(this->met_imu);
  }
  if(which=="met_vmu"){
    return TVector2(this->met_vmu);
  }    
  if(which=="met_tst_imu"){
    return TVector2(this->met_tst_imu);
  }
  if(which=="met_tst_vmu"){
    return TVector2(this->met_tst_vmu);
  }  
  if(which=="met_tst_vmu_mucorr"){
    return TVector2(this->met_tst_vmu_mucorr);
  }
  if(which=="met_tst_vmu_ecorr"){
    return TVector2(this->met_tst_vmu_ecorr);
  }
  if(which=="met_tst_imu_ecorr"){
    return TVector2(this->met_tst_imu_ecorr);
  }
  if(which=="met_trk"){
    return TVector2(this->met_trk);
  }
  if(which=="met_vmu_mucorr"){
    return TVector2(this->met_vmu_mucorr);
  }
  if(which=="met_vmu_ecorr"){
    return TVector2(this->met_vmu_ecorr);
  }  
  if(which=="met_refFinal_vmu"){
    return TVector2(this->met_reffinal_vmu);
  }
  if(which=="met_refFinal_imu"){
    return TVector2(this->met_reffinal_imu);
  }
  if(which=="met_locHadTopo"){
    return TVector2(this->met_lochadtopo);
  }
  if(which=="met_imu_ecorr"){
    return TVector2(this->met_imu_ecorr);
  }
  if(which=="met_imu_phcorr"){
    return TVector2(this->met_imu_phcorr);
  }
  if(which=="met_vmu_phcorr"){
    return TVector2(this->met_vmu_phcorr);
  }  
  if(which=="met_truth_imu"){
    return TVector2(this->met_truth_imu);
  }
  if(which=="met_truth_vmu"){
    return TVector2(this->met_truth_vmu);
  }      
  
}

float MET::Phi(TString which){

  return TVector2::Phi_mpi_pi( GetVector(which).Phi() );

};

//--------------------------------- 3D MET -----------------------------
TVector3 MET::GetMET_Razor(){
  
  TVector2 met_2D(this->met_imu);
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
  cout<<"MET: "<<this->met_imu.Mod()<<" MET_TST: "<<this->met_tst_imu.Mod()<<" MET_Trk: "<<this->met_trk.Mod()<<" MET_mu: "<<this->met_vmu.Mod()<<endl;
}

void MET::Reset(){
  //--- Probably there's a better way
  met_imu.Set(0., 0.);
  met_vmu.Set(0., 0.);  
  met_tst_imu.Set(0., 0.);
  met_tst_vmu.Set(0., 0.);  
  met_tst_imu_ecorr.Set(0., 0.);
  met_tst_vmu_mucorr.Set(0., 0.);
  met_tst_vmu_ecorr.Set(0., 0.);
  met_trk.Set(0., 0.);
  met_imu_ecorr.Set(0., 0.);
  met_vmu_mucorr.Set(0., 0.);  
  met_vmu_ecorr.Set(0., 0.);  
  met_lochadtopo.Set(0., 0.);
  met_reffinal_vmu.Set(0., 0.);
  met_reffinal_imu.Set(0., 0.);
  met_vmu_phcorr.Set(0., 0.);
  met_imu_phcorr.Set(0., 0.);  
  met_truth_vmu.Set(0., 0.);
  met_truth_imu.Set(0., 0.);  
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

bool bw_MV2c20_sort(const Jet& Jet1, const Jet& Jet2){
  return Jet1.MV2c20 > Jet2.MV2c20;
};
