#include <iostream>
#include <math.h>
#include <TROOT.h>
#include "TMath.h"

float MinimumOf(float, float);

bool checkNonFiltered(int);

bool DoIHaveToKeepIt(int run, float met);

float calib(float, float);

float rapidity(float, float, float);

float deltaPhi(Float_t, Float_t);

float deltaR(Float_t, Float_t, Float_t, Float_t);

bool isBeamHalo(int, int);

bool goodLumiBlock(int, int);
