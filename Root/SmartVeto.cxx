#define smartveto_cxx

#include "SusyAnalysis/SmartVeto.h"

bool IsLArHole(float eta, float phi,
               float etamin = -0.1, float etamax = 1.5,
               float phimin = -0.9, float phimax = -0.5) 
{
    if (eta < etamin || eta > etamax) return false;
    if (phi < phimin || phi > phimax) return false;
    return true;
}

bool SmartVeto(bool IsData, double MEx, double MEy , int jet_AntiKt4TopoNewEM_n, std::vector<float>* jet_AntiKt4TopoNewEM_pt, std::vector<float>* jet_AntiKt4TopoNewEM_eta, std::vector<float>* jet_AntiKt4TopoNewEM_phi, std::vector<float>* jet_AntiKt4TopoNewEM_BCH_CORR_JET, std::vector<float>* jet_AntiKt4TopoNewEM_BCH_CORR_CELL, std::vector<float>* jet_AntiKt4TopoNewEM_BCH_CORR_DOTX, FakeMetEstimator* m_fakeMetEstim)///MEx, MEy is in MeV
{
 bool checkSmartVeto=false;
 for(int i=0;i<jet_AntiKt4TopoNewEM_n;i++)
 {
     if((*jet_AntiKt4TopoNewEM_pt)[i]<= 20000.){continue;}

     bool isClose = IsLArHole((*jet_AntiKt4TopoNewEM_eta)[i],
                              (*jet_AntiKt4TopoNewEM_phi)[i]);

     if(isClose && m_fakeMetEstim->isBad((*jet_AntiKt4TopoNewEM_pt)[i],
                                         (*jet_AntiKt4TopoNewEM_BCH_CORR_JET)[i],
                                         (*jet_AntiKt4TopoNewEM_BCH_CORR_CELL)[i],
                                         (*jet_AntiKt4TopoNewEM_BCH_CORR_DOTX)[i],
                                         (*jet_AntiKt4TopoNewEM_phi)[i],
                                         MEx,MEy,10000.,10.,-1.,-1.))
     {
        checkSmartVeto=true;
     }
  }
  return(checkSmartVeto);
}
