#ifndef particles_h
#define particles_h

#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include <time.h>
#include "TLorentzVector.h"
#include "SusyAnalysis/utility.h"

using namespace std;

namespace Particles{

class Particle : public TLorentzVector {
  public:
    Particle();
    ~Particle();

    int    index;
    bool   gev;
    int    type;
    int    origin;

    bool   isGood;
    int    id;
    bool   isTight;

    bool   isTrigMatch;

    bool   isIsolated;
    float  isoTight;
    float  isoLoose;
    float  isoGradient;
    float  ptcone20;
    float  etcone20;
    float  ptcone30;
    float  etcone30;

    float  d0_sig;
    float  z0;    
    int    charge;

    float  SF;
    float  SFu;
    float  SFd;

    float  Pt_up;
    float  Pt_down;

    float  Ht;


    TLorentzVector GetVector() const;
    TVector2 GetVector2() const;
    void SetVector(TLorentzVector vec, bool inGeV = false);
    void SetVector(float ,float, float ,float, bool inGeV = false);
    void PrintInfo();

    ClassDef(Particle,1);

};

class Jet : public Particle {
  public:
    Jet();
    ~Jet();
    bool                    isbjet; //--- as from SUSYTools (decoration)
    float                   MV1;
    float                   MV2c20;
    float                   SV1plusIP3D;
    float                   SV1_pb;
    float                   SV1_pc;
    float                   SV1_pu;
    float                   IP3D_pb;
    float                   IP3D_pc;
    float                   IP3D_pu;
    float                   JetFitterCombNN;
    float                   JetFitterCombNNc;
    float                   JetFitterCu;
    float                   JetFitterCb;
    pair<float,float>       TagEffSF; //--- btageffSFx
    pair<float,float>       TagIneffSF;
    pair<float,float>       TagEffMC; //--- btageffMCx
    TString                 bTagOPw;
    int                     FlavorTruth; //--- JetFlavorForTemplate
    float                   nTrk;
    float                   sumPtTrk;    
    float                   chf;
    float                   emf;
    float                   time;
    float                   fsm;
    float                   width;
    float                   n90;
    float                   hecf;
    float                   HECQuality;
    float                   NegativeE;
    float                   LArQuality;
    float                   jvtxf;
    bool                    failBCHTight;
    bool                    failBCHMedium;
    float                   BCH_CORR_CELL;
    float                   BCH_CORR_JET;

    bool isTauJet(float metphi, TString Tagger="MV1");
    bool isBTagged(TString Tagger, float op);
    bool isBTagged_70eff(TString Tagger);
    bool isBTagged_80eff(TString Tagger);    
    float getBweight(TString Tagger);
    void PrintInfo();
    ClassDef(Jet,1);
};

class MET : public TVector2{
  public:
    MET();
    ~MET();
    TVector2  met_imu;   
    TVector2  met_vmu;
    TVector2  met_tst_imu;
    TVector2  met_tst_vmu;  
    TVector2  met_tst_imu_ecorr;
    TVector2  met_tst_vmu_ecorr;
    TVector2  met_tst_vmu_mucorr;  
    TVector2  met_trk;
    TVector2  met_imu_ecorr;
    TVector2  met_vmu_ecorr;
    TVector2  met_lochadtopo;
    TVector2  met_reffinal_imu;
    TVector2  met_reffinal_vmu;
    TVector2  met_vmu_mucorr;    
    TVector2  met_imu_phcorr;
    TVector2  met_vmu_phcorr;
    TVector2  met_truth_imu;
    TVector2  met_truth_vmu;
    bool      m_hasMuons;

    void SetVector(TVector2 vec, TString which="", bool inGeV=false); 

    TVector2 GetVector(TString which="");
    TVector3 GetMET_Razor();

    bool gev;

    void SetHasMuons(bool hasMuons);
    bool GetHasMuons();

    float Phi(TString which="");

    void PrintInfo();
    void Reset();


    ClassDef(MET,1);

};

}

using namespace Particles;

bool operator<(const Particle& Particle1, const Particle& Particle2);
bool operator<(const Jet& Jet1, const Jet& Jet2);

bool bw_MV1_sort(const Jet& Jet1, const Jet& Jet2);
bool bw_MV2c20_sort(const Jet& Jet1, const Jet& Jet2);

#endif
