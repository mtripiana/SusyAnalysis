#ifndef RUNSMAP_H
#define RUNSMAP_H

#include <stdio.h>
#include <stdarg.h>

#include <vector>
#include <map>

#include "TString.h"

typedef std::pair<int, TString> Rpair;
typedef std::vector< Rpair > VRpair;
typedef std::map<TString, VRpair > RMap;

#define mp std::make_pair 

Rpair pdummy(-1,TString(""));

class RunsMap {

public:
  RunsMap();
  ~RunsMap();

  void LoadMap();
  RMap getMap(){return m_map;};

  std::vector<TString> getKeys();
  std::vector<int> getIDs(TString runname);
  std::vector<TString> getPatterns(TString runname);

private:

  //  VRpair rlist(unsigned int nruns, ...); //not working with pairs! (no-trivially-copyable struct)
  VRpair rlist( Rpair run1,  Rpair run2=pdummy,  Rpair run3=pdummy,  Rpair run4=pdummy,  Rpair run5=pdummy, 
		Rpair run6=pdummy,  Rpair run7=pdummy,  Rpair run8=pdummy,  Rpair run9=pdummy,  Rpair run10=pdummy, 
		Rpair run11=pdummy,  Rpair run12=pdummy,  Rpair run13=pdummy,  Rpair run14=pdummy,  Rpair run15=pdummy); 

  RMap m_map;

};

RunsMap::RunsMap(){
  LoadMap();
}

RunsMap::~RunsMap(){}

std::vector<TString> RunsMap::getKeys(){
  std::vector<TString> v;
  for(RMap::iterator it = m_map.begin(); it != m_map.end(); ++it) 
    v.push_back(it->first);
  
  return v; 
}

std::vector<int> RunsMap::getIDs(TString runname){
  std::vector<int> v;
  for(std::vector<Rpair>::iterator it = m_map[runname].begin(); it != m_map[runname].end(); ++it) {
    v.push_back( (*it).first );
  }
  return v; 
}

std::vector<TString> RunsMap::getPatterns(TString runname){
  std::vector<TString> v;
  for(std::vector<Rpair>::iterator it = m_map[runname].begin(); it != m_map[runname].end(); ++it) {
    v.push_back( (*it).second );
  }
  return v; 
}


void RunsMap::LoadMap(){

  //======================================================================================================================
  //                                                 Local afs/nfs tests
  //======================================================================================================================

  //New test xAOD mc14 
  m_map["TestMClocal"] = rlist( Rpair(117050, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/"));

  m_map["TestMCData"] = rlist( Rpair(117050, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/"),
			       Rpair(900001, "/afs/cern.ch/atlas/project/PAT/xAODs/r5597/"));


  m_map["TestData"] = rlist( Rpair(900001, "/afs/cern.ch/atlas/project/PAT/xAODs/r5597/"));  //we could decode this as (900001-->A, 900011-->A1, etc... if needed)

  m_map["TestMClocal1"] = rlist( Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/Zmumu/"));

  m_map["TestMClocal2"] = rlist( Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/Wmunu/"));

  m_map["TestMClocal3"] = rlist( Rpair(117050, "/nfs/at3/scratch/tripiana/xAOD/mc14/ttbar/"),
				 Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/singletop/"));
				 

  m_map["TestMClocal4"] = rlist( Rpair(110101,"/nfs/at3/scratch/tripiana/xAOD/mc14/singletop/"));

  //======================================================================================================================
  //                                                 List of backgrounds for the Baseline analysis
  //======================================================================================================================

  //Baseline_ttbar
  m_map["PowhegPythia_P2011C_ttbar"] = rlist( Rpair(117050, "mc14_8TeV.117050.PowhegPythia_P2011C_ttbar*") );

  //======================================================================================================================
  //                                                 Signal grids
  //======================================================================================================================
  //BB grid
  m_map["MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50"] = rlist( Rpair(202266, "mc14_8TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50*"));

  //...
};

VRpair RunsMap :: rlist( Rpair run1, Rpair run2, Rpair run3, Rpair run4, Rpair run5,
			 Rpair run6, Rpair run7, Rpair run8, Rpair run9, Rpair run10,
			 Rpair run11, Rpair run12, Rpair run13, Rpair run14, Rpair run15){ 
  
  VRpair runList;
  if(run1.first > 0) runList.push_back( run1 );
  if(run2.first > 0) runList.push_back( run2 );
  if(run3.first > 0) runList.push_back( run3 );
  if(run4.first > 0) runList.push_back( run4 );
  if(run5.first > 0) runList.push_back( run5 );
  if(run6.first > 0) runList.push_back( run6 );
  if(run7.first > 0) runList.push_back( run7 );
  if(run8.first > 0) runList.push_back( run8 );
  if(run9.first > 0) runList.push_back( run9 );
  if(run10.first > 0) runList.push_back( run10 );
  if(run11.first > 0) runList.push_back( run11 );
  if(run12.first > 0) runList.push_back( run12 );
  if(run13.first > 0) runList.push_back( run13 );
  if(run14.first > 0) runList.push_back( run14 );
  if(run15.first > 0) runList.push_back( run15 );

  return runList;
}


/* VRpair RunsMap::rlist( unsigned int nruns, ... ){ */

/*   VRpair runList; */

/*   va_list vap;  */
/*   va_start( vap, nruns ); */
  
/*   for(unsigned int i=0; i<nruns; i++){ */
/*     Rpair rp = va_arg(vap, Rpair ); */
/*     runList.push_back( rp ); */
/*   } */
/*   va_end(vap); */

/*   return runList; */
/* }   */

#endif  
