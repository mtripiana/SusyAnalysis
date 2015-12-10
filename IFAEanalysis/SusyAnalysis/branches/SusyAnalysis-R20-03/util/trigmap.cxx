#include "SusyAnalysis/utility.h"
#include "TString.h"
#include "TFile.h"
#include <iostream>
#include <utility>      // std::pair
#include <iostream>     // std::cout
#include <iomanip>     // std::cout
#include <map> 

using namespace std;

void usage(){

  cout << endl;
  cout << bold("trigmap <sample>") << endl;
  cout << endl;
  cout << "<sample>   :  the root file you want to check the trigger map for" << endl;
  cout << endl;
}

std::vector<TString> getTokens(TString line, TString delim){
  std::vector<TString> vtokens;
  TObjArray* tokens = line.Tokenize(delim); //delimiters                                                                                                                             
  if(tokens->GetEntriesFast()) {
    TIter iString(tokens);
    TObjString* os=0;
    while ((os=(TObjString*)iString())) {
      vtokens.push_back( os->GetString() );
    }
  }
  delete tokens;

  return vtokens;
}

int main( int argc, char* argv[] ) {

  //check if enough arguments
  if( argc < 1 ){
    usage();
    return 0;
  }

  TString sample = TString(argv[1]);

  //load trigger map
  std::vector<TString> trigchains;                                                         std::map<int,TString> trigmap{};


  try{
    TString strig = (TString)TFile::Open(sample)->Get("Triggers")->GetTitle();
    trigchains = getTokens(strig, ",");
    auto ic=0;
    cout << "Trigger MAP " << endl;
    cout << "------------------------------------------" << endl;
    for(const auto& s : trigchains){
      
      cout << ic << "\t : \t" << s << endl;
      ic++;

    }
  }
  catch(...){
    cout << "No trigger metadata saved for this file!" << endl;
  }
}
