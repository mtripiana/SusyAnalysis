#include <string.h>
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "TSystem.h"
#include "Riostream.h"
#include <vector>

void AddNewBranch(TString, Float_t);

void addAntiWeightToTree(TString fileName, bool isData);

void JoinSplittedFiles(TString fileName);

void tadd(std::vector< TString>, vector< Double_t>, TString, bool);
