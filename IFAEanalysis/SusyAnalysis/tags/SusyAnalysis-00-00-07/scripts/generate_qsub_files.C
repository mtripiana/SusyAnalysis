
void generate_qsub_files(){

  TString croot = gSystem->GetFromPipe("readlink -f $ANALYSISCODE");

  gSystem->mkdir(croot+"/SusyAnalysis/scripts/qsubFiles/");

  gSystem->Exec("rm -f AutoDict_RunsMap*");
  gInterpreter->GenerateDictionary("RunsMap",croot+"/SusyAnalysis/SusyAnalysis/RunsMap.h");
  std::vector<TString> samples;
  RunsMap loc_map;
 
  samples = loc_map.getKeys();
  for(unsigned int s=0; s < samples.size(); s++){
    cout << "Generating script for sample (" << s << "/" << samples.size() << ")= " << samples[s] << endl; 
    gSystem->Exec(croot+"/SusyAnalysis/scripts/MakeQsubFile.sh "+samples[s]);
  }
}
