#ifndef SYSTEMATICS_H
#define SYSTEMATICS_H

#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <algorithm>  

#include <vector>
#include <map>

#include "TString.h"
#include "TError.h"

//#include "SUSYTools/ScaleVariatioReweighter.hpp"
//#include "SusyAnalysis/ScaleVariatioReweighter.hpp"
#include "PATInterfaces/SystematicSet.h"

using namespace std;

class Systematics {

public:
  Systematics();
  ~Systematics();

 void SystTranslate(   TString syste,
		       CP::SystematicSet& syst_CP );

  void LoadList();
  std::vector<TString> getSystList();
  std::vector<TString> getSystListCP(){return m_CP_list;};

  int     getSystID(TString name);
  TString getSystName(int id);

  void printSystList();

private:
  bool implemented(const TString &syst, const std::vector<TString> &Slist);

  std::vector<TString> m_list;    // the whole list of systematics
  std::vector<TString> m_CP_list; // just the registered ones (recommended by CP groups)

};
#endif  
