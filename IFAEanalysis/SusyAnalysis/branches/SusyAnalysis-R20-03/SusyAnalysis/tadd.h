#include <string.h>
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TFileMerger.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "TSystem.h"
#include "Riostream.h"
#include <vector>

using namespace std;

void AddNewBranch(TString, Float_t);

void ComputeNewBranch(TString, Float_t, bool);

void addAntiWeightToTree(TString fileName, bool isData);

void JoinSplittedFiles(TString fileName);

void MergeFiles(std::vector<TString> files, TString outfile);

void MergeSplittedFiles(TString fileName);

void hadd(std::vector< TString>, TString);

void tadd(std::vector< TString>, std::vector< Double_t>, TString, bool);

void tadd_grid(std::vector< TString>, TString, bool);

void addMetaData(TString rootName, TString histName, TString outName="merged.root");
