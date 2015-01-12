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
    float  Pt_up;
    float  Pt_down;
    float  Ht;
    bool   isGood;
    bool   isIsolated;
    bool   isTight;
    float  ptcone20;
    float  etcone20;
    float  ptcone30;
    float  etcone30;
    int    charge;
    float  SF;
    float  SFu;
    float  SFd;

    int    id;
    bool   gev;
    int    type;
    int    origin;

    TLorentzVector GetVector();
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
    bool isBTagged(TString Tagger);
    bool isBTagged_80eff(TString Tagger);    
    float getBweight(TString Tagger);
    void PrintInfo();
    ClassDef(Jet,1);
};

class MET : public TVector2{
  public:
    MET();
    ~MET();
    TVector2  met;
    TVector2  met_trk;
    TVector2  met_mu;
    TVector2  met_lochadtopo;
    TVector2  met_reffinal;
    TVector2  met_reffinal_mu;
    TVector2  met_ecorr;
    TVector2  met_phcorr;
    bool      m_hasMuons;

    void SetVector(TVector2 vec, TString which="", bool inGeV=false); 

    TVector2 GetVector(TString which="");
    TVector3 GetMET_Razor(TString met_flavour="");

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


#endif
