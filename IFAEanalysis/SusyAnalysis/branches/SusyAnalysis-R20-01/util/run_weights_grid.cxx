//---------------------------------------------------------------
//  
//---------------------------------------------------------------


#include "SusyAnalysis/tadd.h"
#include "SusyAnalysis/XMLReader.h"
#include "SusyAnalysis/utility.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TString.h"
#include "SusyAnalysis/RunsMap.h"
#include "SusyAnalysis/utility.h"

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;


void usage(){

  cout << endl;
  cout << bold("run_weights_grid [options] <inputDir>") << endl;
  cout << endl;
  cout << "       <inputDir>        : where the files to be merged are located" << endl;
  cout << endl;
  cout << "" << endl;
  cout << " [options] : supported option flags" << endl;
  cout << "       -o=<outDir>       : where all the output is saved (if left empty it is saved in $ANALYSISROOTFILES" << endl;
  cout << "       -n=<DSid>         : dataset id " << endl;
  cout << "       -m=<mergedName>   : name to be used for merged sample (so to avoid looking for it in RunsMap)" << endl;
  cout << endl;
}


int main( int argc, char* argv[] ) {

  std::vector<TString> args, opts;

  //parse input arguments                                                                       
  for (int i=1 ; i < argc ; i++) {
    //    if( std::string(argv[i]).find("-") != std::string::npos )// is option 
    //      opts.push_back( TString( strtok(argv[i],"-") ));
    if( std::string(argv[i]).find("-") == 0 )// is option 
      opts.push_back( TString(argv[i]).Remove(0,1) );
    else //is argument
      args.push_back( TString(argv[i]) );
  }
  
  //print usage of code
  if( args.size() < 1 ){
    usage();
    return 0;
  }

  //we want two arguments: DSID (for output file name) and directory name
  int DSid = 0;

  string tmp_dirname =(string)args[0].Data();
  const char *dirname = (const char *)tmp_dirname.c_str(); 

  std::string outDirectory="./";
  std::string mergedName="";

  //config options
  for( unsigned int iop=0; iop < opts.size(); iop++){

    if (opts[iop].BeginsWith("n") )
      DSid = opts[iop].ReplaceAll("n=","").Atoi();
    
    if (opts[iop].BeginsWith("m") )
      mergedName = opts[iop].ReplaceAll("m=","");
	
    if (opts[iop].BeginsWith("o") )
      outDirectory = opts[iop].ReplaceAll("o=","")+"/";
    
  }

  cout << " Merging: DSID\t" << DSid <<"\tfrom inputDir: \t" << TString(dirname) << "\t  to outputDir: \t" << outDirectory << endl;
  
  //add tag with DSid to final sample (if not looked up in RunsMap)
  TString tagDSid = Form("_%d",DSid);  
  
  if(mergedName.empty()){

    RunsMap rmap;
    std::vector<TString> samples = rmap.getKeys();
    std::vector<int> ids;
    std::vector<TString> helpName;
    std::vector<TString> tokens;

    for(unsigned int i=0; i < samples.size(); i++){
      ids = rmap.getIDs(samples[i]);
      helpName = rmap.getPatterns(samples[i]);
      for(unsigned int j=0; j < ids.size(); j++){
	if(ids[j] == DSid){
	  if (helpName[j].BeginsWith("/")) continue;
	  
	  tokens = getTokens(helpName[j], ".");
	  
	  mergedName = tokens[2];
	  tagDSid = "";  // remove tag if name was found
	  break; //once a name is found, we don't need to search anymore
	}
      }
    }
  }
  
  
  std::vector<TString> fileNames;
  const char *ext=".output.root";

  TString DirPath = TString(dirname);

  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();

  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.Contains(ext)) {
	fileNames.push_back(DirPath+"/"+fname);
	addMetaData(DirPath+"/"+fname, DirPath+"/"+fname.Copy().ReplaceAll(".output",".hist-output")); 
      }
    }
  }

  delete files;


  tadd_grid(fileNames, outDirectory+"/Nom_"+mergedName+"_tree"+tagDSid+".root", false);

  //clean ROOTMERGE leftovers  (investigate a bit more anyways...)
  system("ls -lrt1 /tmp/ROOTMERGE* | grep $USER | rev | cut -d' ' -f 1 | rev | while read i; do rm -rf $i; done");

  return 0;
}

