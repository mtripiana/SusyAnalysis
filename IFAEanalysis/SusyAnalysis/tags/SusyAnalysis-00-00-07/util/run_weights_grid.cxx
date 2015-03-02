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

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;


void usage(){

  cout << endl;
  cout << bold("run_weights_grid [options] <DSID> <InputDirectory>") << endl;
  cout << endl;
  cout << " <DSID> : the data set ID " << endl;
  cout << " <InputDirectory> : directory where the files are to be merged " << endl;
  cout << endl;
  cout << "" << endl;
  cout << " [options] : supported option flags" << endl;
  cout << "       -j=<jOption>  : choose which analysis you want to run over. ( = 'METbb'(default), 'Stop', 'Monojet')  " << endl;
  cout << "       -o=<outDir>   : where all the output is saved (if left empty it reads the path from the xml jobOption (FinalPath))." << endl;
  cout << endl;
}



int main( int argc, char* argv[] ) {

  std::vector<TString> args, opts;

  //parse input arguments                                                                                                                                                                                                                   
  for (int i=1 ; i < argc ; i++) {
    //cout << argv[i] << endl;
    if( std::string(argv[i]).find("-") != std::string::npos )// is option 
      opts.push_back( TString( strtok(argv[i],"-") ));
    else //is argument
      args.push_back( argv[i] );
  }

  //print usage of code
  if( args.size() < 1 ){
    usage();
    return 0;
  }

  //we want two arguments: DSID (for output file name) and directory name
  int DataSetID = TString(args[0]).Atoi();
  //  cout << TString(args[1]).Data() << "\t" << args[1].Data() << "\t" << (const char *)(args[1].Data()) << endl;
  string tmp_dirname =(string)args[1].Data();
  const char *dirname = (const char *)tmp_dirname.c_str(); //(args[1].Data());

  cout << "Merging: DSID\t" << DataSetID <<"\tDirName\t" << TString(dirname).Data() << endl;

  std::string jOption = "Stop_JobOption.xml";
  std::string outDirectory;

  for( unsigned int iop=0; iop < opts.size(); iop++){

    if (opts[iop].BeginsWith("j") ){
      jOption = opts[iop].ReplaceAll("j=","")+"_JobOption.xml";
    }
    if (opts[iop].BeginsWith("o") ){
      outDirectory = opts[iop].ReplaceAll("o=","")+"/";
    }
  }


  RunsMap rmap;
  std::vector<TString> samples = rmap.getKeys();
  std::vector<int> ids;
  std::vector<TString> helpName;
  std::string mergedName;
  for(unsigned int i=0; i<samples.size(); i++){
    ids = rmap.getIDs(samples[i]);
    helpName = rmap.getPatterns(samples[i]);
    for(unsigned int j=0; j<ids.size(); j++){
      if(ids[j]== DataSetID){
	if (helpName[j].BeginsWith("/")) continue;
	// now: akward string manipulation: goal: get name between 2nd dot and 3rd dot in sample name
	helpName[j].Replace(helpName[j].Index("."),1,"_"); //eliminate first dot
	Ssiz_t secondPoint = helpName[j].Index(".");
	helpName[j].Replace(secondPoint,1,"_"); //change 2nd dot
	Ssiz_t thirdPoint = helpName[j].Index(".");
	helpName[j].Remove(thirdPoint);// remove the end
	helpName[j].Remove(0,secondPoint+1);// remove the beginning
	mergedName = helpName[j]; 
	break; //once a name is found, we don't need to search anymore
      }

    }
  }



  //*** Read some input options
  //  std::string DirectoryPath=gSystem->Getenv("ANALYSISCODE");
  std::string maindir = getenv("ROOTCOREBIN");

  std::string xmlPath=maindir+"/data/SusyAnalysis/"+jOption;

  XMLReader *xmlJobOption = new XMLReader();
  xmlJobOption->readXML(xmlPath);

  TString FinalPath      = TString(xmlJobOption->retrieveChar("AnalysisOptions$GeneralSettings$Path/name/RootFilesFolder").c_str());
  if( outDirectory!="") FinalPath = TString(outDirectory);    // Take the submit directory from the input if provided:


  std::vector<TString> fileNames;

  //  const char *dirname="/nfs/at3/scratch/cfischer/SusyAnalysis/FullSamples_17nov14/Wenu/";
  const char *ext=".root";

  TString DirPath = TString(dirname);

  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  if (files) {
    cout << "found files"<<endl;
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.Contains(ext)) {
	cout << fname.Data() << endl;
	fileNames.push_back(DirPath+"/"+fname);
      }
    }
  }

  tadd_grid(fileNames, FinalPath+"/Nom_"+mergedName+"_tree.root", false);

  return 0;
}

