#include "SusyAnalysis/Systematics.h"
#include "SusyAnalysis/utility.h"
#include <iostream>

void usage(){

  cout << endl;
  cout << bold("sysname <ID>") << endl;
  cout << endl;
  cout << "<ID>   :  the systematics identifier in your sample (e.g. SYS5 --> <ID>=5)" << endl;
  cout << endl;
}


int main( int argc, char* argv[] ) {

  //check if enough arguments
  if( argc < 1 ){
    usage();
    return 0;
  }

  int sid = TString(argv[1]).Atoi();

  Systematics Sobj;
  Sobj.LoadList();
  cout << "SYS" << sid << "\t:\t " << Sobj.getSystName(sid) << endl;
  
}
