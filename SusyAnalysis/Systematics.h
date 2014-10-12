#ifndef SYSTEMATICS_H
#define SYSTEMATICS_H

#include <stdio.h>
#include <stdarg.h>
#include <algorithm>  

#include <vector>
#include <map>

#include "TString.h"
#include "TError.h"

#include "SUSYTools/ScaleVariatioReweighter.hpp"
#include "PATInterfaces/SystematicSet.h"

namespace pileupErr
{
  typedef enum  {
    NONE,
    PileupHigh,
    PileupLow
  } pileupSyste;
}

namespace JvfUncErr
{
  typedef enum  {
    NONE,
    JvfUncHigh,
    JvfUncLow
  } JvfSyste;
}

namespace BCHCorrMediumErr
{
  typedef enum  {
    NONE,
    BCHCorrHigh,
    BCHCorrLow
  } BCHSyste;
}

//ported (partially) from old SUSYObjDef class 
//@todo get rid of this once we have the correspendent implementation in CP
namespace SystErr
{
  typedef enum  {
    NONE,                          
    BJETDOWN, BJETUP, CJETDOWN, CJETUP, BMISTAGDOWN, BMISTAGUP  
  } Syste;
}

class Systematics {

public:
  Systematics();
  ~Systematics();

 void SystTranslate(   TString syste,
		       CP::SystematicSet& syst_CP,
		       SystErr::Syste &syst_ST,
		       ScaleVariatioReweighter::variation &syst_Scale,
		       pileupErr::pileupSyste &syst_PU,
		       JvfUncErr::JvfSyste &syst_JVF,
		       BCHCorrMediumErr::BCHSyste &syst_BCH );
 

  void LoadList();
  std::vector<TString> getSystList();
  std::vector<TString> getSystListUser(){return m_user_list;};
  std::vector<TString> getSystListST(){return m_ST_list;};
  std::vector<TString> getSystListCP(){return m_CP_list;};

  void printSystList();

private:
  bool implemented(const TString &syst, const std::vector<TString> &Slist);

  std::vector<TString> m_list;    // the whole list of systematics
  std::vector<TString> m_CP_list; // just the registered ones (recommended by CP groups)
  std::vector<TString> m_ST_list; // just the SUSYTools ones (old, deprecated!)
  std::vector<TString> m_user_list; // just the user defined ones (PU, scale, BCH, JVF)

};
#endif  
