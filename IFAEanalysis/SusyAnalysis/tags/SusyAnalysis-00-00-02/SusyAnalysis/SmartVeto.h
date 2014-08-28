#include <iostream>
#include <vector>
#include "SUSYTools/FakeMetEstimator.h"

#include <TROOT.h>
#include <map>
#include <TString.h>
#include <math.h>
#include <time.h>

bool IsLArHole(float, float,float,float,float,float);

bool SmartVeto(bool, double, double, int, std::vector<float>*, std::vector<float>*, std::vector<float>*, std::vector<float>*, std::vector<float>*, std::vector<float>*, FakeMetEstimator*);///MEx, MEy is in MeV
