
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <TROOT.h>
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include <string>

TString bold(TString t);

TString red(TString t);

std::string getCmdOutput(const std::string& mStr);

std::string tmpdirname();

std::vector<TString> getTokens(TString line, TString delim);

TString stripName(TString name);

float MinimumOf(float, float);

bool checkNonFiltered(int);

bool DoIHaveToKeepIt(int run, float met);

float calib(float, float);

float rapidity(float, float, float);

float deltaPhi(Float_t, Float_t);

float deltaR(Float_t, Float_t, Float_t, Float_t);

bool isBeamHalo(int, int);

bool goodLumiBlock(int, int);
