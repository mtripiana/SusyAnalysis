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
		Rpair run11=pdummy,  Rpair run12=pdummy,  Rpair run13=pdummy,  Rpair run14=pdummy,  Rpair run15=pdummy, 
		Rpair run16=pdummy,  Rpair run17=pdummy,  Rpair run18=pdummy,  Rpair run19=pdummy,  Rpair run20=pdummy); 

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

  // new 13 TeV derivations
  
//SBOTTOM   ---------------------------
m_map["13TEV_Sbottom_300_290"] = rlist( Rpair(202249, "mc14_13TeV.202249.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_300_290_MET50.merge.DAOD_SUSY1.e3064_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_Sbottom_800_1"] = rlist( Rpair(202266, "mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.DAOD_SUSY1.e3064_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_Sbottom_900_1"] = rlist( Rpair(202270, "mc14_13TeV.202270.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_900_1_MET50.merge.DAOD_SUSY1.e3460_a266_a265_r5853_p1872/"));
m_map["13TEV_Sbottom_1000_1"] = rlist( Rpair(202277, "mc14_13TeV.202277.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_1000_1_MET50.merge.DAOD_SUSY1.e3460_a266_a265_r5853_p1872/"));


//SINGLETOP ---------------------------
m_map["13TEV_Part1singleTop_PowhegPythia"] = rlist( Rpair(110070,"mc14_13TeV.110070.PowhegPythia_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_Part2singleTop_PowhegPythia"] = rlist( Rpair(110071,"mc14_13TeV.110071.PowhegPythia_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_Part3singleTop_PowhegPythia"] = rlist( Rpair(110302,"mc14_13TeV.110302.PowhegPythia_P2012_st_schan_lep.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_Part4singleTop_PowhegPythia"] = rlist( Rpair(110305,"mc14_13TeV.110305.PowhegPythia_P2012_st_Wtchan_incl_DR.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1872/"));

//TTBAR ---------------------------
m_map["13TEV_ttbar_PowhegPythia"] = rlist( Rpair(110401,"mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.DAOD_SUSY1.e2928_s1982_s2008_r5787_r5853_p1872/"));

// TTBAR+V ---------------------------
m_map["13TEV_ttbarW_MadGraphPythia"]	   = rlist( Rpair(119353,"mc14_13TeV.119353.MadGraphPythia_AUET2BCTEQ6L1_ttbarW.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarWjExcl_MadGraphPythia"]  = rlist( Rpair(174830,"mc14_13TeV.174830.MadGraphPythia_AUET2BCTEQ6L1_ttbarWjExcl.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarWjjIncl_MadGraphPythia"] = rlist( Rpair(174831,"mc14_13TeV.174831.MadGraphPythia_AUET2BCTEQ6L1_ttbarWjjIncl.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarZ_MadGraphPythia"]       = rlist( Rpair(119355,"mc14_13TeV.119355.MadGraphPythia_AUET2BCTEQ6L1_ttbarZ.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarZjExcl_MadGraphPythia"]  = rlist( Rpair(174832,"mc14_13TeV.174832.MadGraphPythia_AUET2BCTEQ6L1_ttbarZjExcl.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarZjjIncl_MadGraphPythia"] = rlist( Rpair(174833,"mc14_13TeV.174833.MadGraphPythia_AUET2BCTEQ6L1_ttbarZjjIncl.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_ttbarWWIncl_MadGraphPythia"]  = rlist( Rpair(119583,"mc14_13TeV.119583.MadgraphPythia_AUET2B_CTEQ6L1_ttbarWW.merge.DAOD_SUSY1.e3214_s1982_s2008_r5787_r5853_p1872/"));



// W+JETS ---------------------------
m_map["Wenu167740_13TeVgrid"]	   = rlist( Rpair(167740,"mc14_13TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167741_13TeVgrid"]	   = rlist( Rpair(167741,"mc14_13TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167742_13TeVgrid"]	   = rlist( Rpair(167742,"mc14_13TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167743_13TeVgrid"]	   = rlist( Rpair(167743,"mc14_13TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167744_13TeVgrid"]	   = rlist( Rpair(167744,"mc14_13TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167745_13TeVgrid"]	   = rlist( Rpair(167745,"mc14_13TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167746_13TeVgrid"]	   = rlist( Rpair(167746,"mc14_13TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167747_13TeVgrid"]	   = rlist( Rpair(167747,"mc14_13TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167748_13TeVgrid"]	   = rlist( Rpair(167748,"mc14_13TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

m_map["Wenu180534_13TeVgrid"]      = rlist( Rpair(180534,"mc14_13TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu180535_13TeVgrid"]      = rlist( Rpair(180535,"mc14_13TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu180536_13TeVgrid"]      = rlist( Rpair(180536,"mc14_13TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu180537_13TeVgrid"]     = rlist( Rpair(180537,"mc14_13TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu180538_13TeVgrid"]     = rlist( Rpair(180538,"mc14_13TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu180539_13TeVgrid"]     = rlist( Rpair(180539,"mc14_13TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu180540_13TeVgrid"]     = rlist( Rpair(180540,"mc14_13TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu180541_13TeVgrid"]     = rlist( Rpair(180541,"mc14_13TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu180542_13TeVgrid"]     = rlist( Rpair(180542,"mc14_13TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

m_map["Wenu167761_13TeVgrid"]	   = rlist( Rpair(167761,"mc14_13TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167762_13TeVgrid"]	   = rlist( Rpair(167762,"mc14_13TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167763_13TeVgrid"]	   = rlist( Rpair(167763,"mc14_13TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167764_13TeVgrid"]	   = rlist( Rpair(167764,"mc14_13TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167765_13TeVgrid"]	   = rlist( Rpair(167765,"mc14_13TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167766_13TeVgrid"]	   = rlist( Rpair(167766,"mc14_13TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167767_13TeVgrid"]	   = rlist( Rpair(167767,"mc14_13TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167768_13TeVgrid"]	   = rlist( Rpair(167768,"mc14_13TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167769_13TeVgrid"]	   = rlist( Rpair(167769,"mc14_13TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

m_map["Wenu167770_13TeVgrid"]	   = rlist( Rpair(167770,"mc14_13TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167771_13TeVgrid"]	   = rlist( Rpair(167771,"mc14_13TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167772_13TeVgrid"]	   = rlist( Rpair(167772,"mc14_13TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167773_13TeVgrid"]	   = rlist( Rpair(167773,"mc14_13TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167774_13TeVgrid"]	   = rlist( Rpair(167774,"mc14_13TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167775_13TeVgrid"]	   = rlist( Rpair(167775,"mc14_13TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167776_13TeVgrid"]	   = rlist( Rpair(167776,"mc14_13TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167777_13TeVgrid"]	   = rlist( Rpair(167777,"mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167778_13TeVgrid"]	   = rlist( Rpair(167778,"mc14_13TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

m_map["Wenu167779_13TeVgrid"]	   = rlist( Rpair(167779,"mc14_13TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167780_13TeVgrid"]	   = rlist( Rpair(167780,"mc14_13TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167781_13TeVgrid"]	   = rlist( Rpair(167781,"mc14_13TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167782_13TeVgrid"]	   = rlist( Rpair(167782,"mc14_13TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167783_13TeVgrid"]	   = rlist( Rpair(167783,"mc14_13TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167784_13TeVgrid"]	   = rlist( Rpair(167784,"mc14_13TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167785_13TeVgrid"]	   = rlist( Rpair(167785,"mc14_13TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167786_13TeVgrid"]	   = rlist( Rpair(167786,"mc14_13TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167787_13TeVgrid"]	   = rlist( Rpair(167787,"mc14_13TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

m_map["Wenu167788_13TeVgrid"]	   = rlist( Rpair(167788,"mc14_13TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167789_13TeVgrid"]	   = rlist( Rpair(167789,"mc14_13TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wenu167790_13TeVgrid"]	   = rlist( Rpair(167790,"mc14_13TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167791_13TeVgrid"]	   = rlist( Rpair(167791,"mc14_13TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167792_13TeVgrid"]	   = rlist( Rpair(167792,"mc14_13TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wmunu167793_13TeVgrid"]	   = rlist( Rpair(167793,"mc14_13TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167794_13TeVgrid"]	   = rlist( Rpair(167794,"mc14_13TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167795_13TeVgrid"]	   = rlist( Rpair(167795,"mc14_13TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));
m_map["Wtaunu167796_13TeVgrid"]	   = rlist( Rpair(167796,"mc14_13TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1872/"));

// Z+JETS  ---------------------------
m_map["Zee167749_13TeVgrid"]	   = rlist( Rpair(167749,"mc14_13TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167750_13TeVgrid"]	   = rlist( Rpair(167750,"mc14_13TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167751_13TeVgrid"]	   = rlist( Rpair(167751,"mc14_13TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167752_13TeVgrid"]	   = rlist( Rpair(167752,"mc14_13TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167753_13TeVgrid"]	   = rlist( Rpair(167753,"mc14_13TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167754_13TeVgrid"]	   = rlist( Rpair(167754,"mc14_13TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167755_13TeVgrid"]	   = rlist( Rpair(167755,"mc14_13TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167756_13TeVgrid"]	   = rlist( Rpair(167756,"mc14_13TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167757_13TeVgrid"]	   = rlist( Rpair(167757,"mc14_13TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167758_13TeVgrid"]	   = rlist( Rpair(167758,"mc14_13TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167759_13TeVgrid"]	   = rlist( Rpair(167759,"mc14_13TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167760_13TeVgrid"]	   = rlist( Rpair(167760,"mc14_13TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));

m_map["Zee167797_13TeVgrid"]	   = rlist( Rpair(167797,"mc14_13TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167798_13TeVgrid"]	   = rlist( Rpair(167798,"mc14_13TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167799_13TeVgrid"]	   = rlist( Rpair(167799,"mc14_13TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167800_13TeVgrid"]	   = rlist( Rpair(167800,"mc14_13TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167801_13TeVgrid"]	   = rlist( Rpair(167801,"mc14_13TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167802_13TeVgrid"]	   = rlist( Rpair(167802,"mc14_13TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167803_13TeVgrid"]	   = rlist( Rpair(167803,"mc14_13TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167804_13TeVgrid"]	   = rlist( Rpair(167804,"mc14_13TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167805_13TeVgrid"]	   = rlist( Rpair(167805,"mc14_13TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167806_13TeVgrid"]	   = rlist( Rpair(167806,"mc14_13TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167807_13TeVgrid"]	   = rlist( Rpair(167807,"mc14_13TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167808_13TeVgrid"]	   = rlist( Rpair(167808,"mc14_13TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));

m_map["Zee167809_13TeVgrid"]	   = rlist( Rpair(167809,"mc14_13TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167810_13TeVgrid"]	   = rlist( Rpair(167810,"mc14_13TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167811_13TeVgrid"]	   = rlist( Rpair(167811,"mc14_13TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167812_13TeVgrid"]	   = rlist( Rpair(167812,"mc14_13TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167813_13TeVgrid"]	   = rlist( Rpair(167813,"mc14_13TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167814_13TeVgrid"]	   = rlist( Rpair(167814,"mc14_13TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167815_13TeVgrid"]	   = rlist( Rpair(167815,"mc14_13TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167816_13TeVgrid"]	   = rlist( Rpair(167816,"mc14_13TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167817_13TeVgrid"]	   = rlist( Rpair(167817,"mc14_13TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167818_13TeVgrid"]	   = rlist( Rpair(167818,"mc14_13TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167819_13TeVgrid"]	   = rlist( Rpair(167819,"mc14_13TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167820_13TeVgrid"]	   = rlist( Rpair(167820,"mc14_13TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));

m_map["Zee167821_13TeVgrid"]	   = rlist( Rpair(167821,"mc14_13TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167822_13TeVgrid"]     = rlist( Rpair(167822,"mc14_13TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167823_13TeVgrid"]     = rlist( Rpair(167823,"mc14_13TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167824_13TeVgrid"]     = rlist( Rpair(167824,"mc14_13TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167825_13TeVgrid"]     = rlist( Rpair(167825,"mc14_13TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167826_13TeVgrid"]     = rlist( Rpair(167826,"mc14_13TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167827_13TeVgrid"]     = rlist( Rpair(167827,"mc14_13TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167828_13TeVgrid"]     = rlist( Rpair(167828,"mc14_13TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167829_13TeVgrid"]     = rlist( Rpair(167829,"mc14_13TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167830_13TeVgrid"]	   = rlist( Rpair(167830,"mc14_13TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167831_13TeVgrid"]	   = rlist( Rpair(167831,"mc14_13TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167832_13TeVgrid"]	   = rlist( Rpair(167832,"mc14_13TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));

m_map["Zee167833_13TeVgrid"]	= rlist( Rpair(167833,"mc14_13TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167834_13TeVgrid"]	= rlist( Rpair(167834,"mc14_13TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zee167835_13TeVgrid"]	= rlist( Rpair(167835,"mc14_13TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167836_13TeVgrid"]	= rlist( Rpair(167836,"mc14_13TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167837_13TeVgrid"]	= rlist( Rpair(167837,"mc14_13TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Zmumu167838_13TeVgrid"]	= rlist( Rpair(167838,"mc14_13TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167839_13TeVgrid"]	= rlist( Rpair(167839,"mc14_13TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167840_13TeVgrid"]	= rlist( Rpair(167840,"mc14_13TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Ztautau167841_13TeVgrid"]	= rlist( Rpair(167841,"mc14_13TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167842_13TeVgrid"]	   = rlist( Rpair(167842,"mc14_13TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167843_13TeVgrid"]	   = rlist( Rpair(167843,"mc14_13TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));
m_map["Znunu167844_13TeVgrid"]	   = rlist( Rpair(167844,"mc14_13TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1872/"));


// GAMMAJETS   ---------------------------
m_map["13TEV_SinglePhotonMassiveCBPt140_280_CVetoBVeto"] = rlist( Rpair(177575,"mc14_13TeV.177575.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt280_500_CVetoBVeto"] = rlist( Rpair(177576,"mc14_13TeV.177576.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt500_CVetoBVeto"] = rlist( Rpair(177577,"mc14_13TeV.177577.Sherpa_CT10_SinglePhotonMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt140_280_CFilterBVeto"] = rlist( Rpair(177579,"mc14_13TeV.177579.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt280_500_CFilterBVeto"] = rlist( Rpair(177580,"mc14_13TeV.177580.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt500_CFilterBVeto"] = rlist( Rpair(177581,"mc14_13TeV.177581.Sherpa_CT10_SinglePhotonMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt140_280_BFilter"] = rlist( Rpair(177583,"mc14_13TeV.177583.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt280_500_BFilter"] = rlist( Rpair(177584,"mc14_13TeV.177584.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));
m_map["13TEV_SinglePhotonMassiveCBPt500_BFilter"] = rlist( Rpair(177585,"mc14_13TeV.177585.Sherpa_CT10_SinglePhotonMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));


  //======================================================================================================================
  //                                                 Grid sbottom analysis
  //======================================================================================================================

  // 8 TEV

  //SIGNAL
  m_map["8TEV_Sbottom_800_1"] = rlist( Rpair(202266, "mc14_8TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.AOD.e3063_s1933_s1911_r5591_r5625/"));
  m_map["8TEV_Sbottom_300_290"] = rlist( Rpair(202249, "mc14_8TeV.202249.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_300_290_MET50.merge.AOD.e3063_s1933_s1911_r5591_r5625/"));
  
  //BACKGROUND
  m_map["8TEV_ttbar_PowhegPythia"] = rlist( Rpair(117050,"mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.DAOD_SUSY1.e1727_s1933_s1911_r5591_r5625_p1784/"));
  m_map["8TEV_singleTop_PowhegPythia"] = rlist( Rpair(110101,"mc14_8TeV.110101.AcerMCPythia_P2011CCTEQ6L1_singletop_tchan_l.merge.DAOD_SUSY1.e2096_s1933_s1911_r5591_r5625_p1784/"),
						Rpair(110119,"mc14_8TeV.110119.PowhegPythia_P2011C_st_schan_lep.merge.DAOD_SUSY1.e1720_s1933_s1911_r5591_r5625_p1784/"),
						Rpair(110140,"mc14_8TeV.110140.PowhegPythia_P2011C_st_Wtchan_incl_DR.merge.DAOD_SUSY1.e1743_s1933_s1911_r5591_r5625_p1784/"));

  m_map["8TEV_ttbarG_MadGraphPythia"] = rlist( Rpair(177998,"mc14_8TeV.177998.MadGraphPythia_AUET2BCTEQ6L1_ttbargammaPt80_noAllHad_fixed.merge.AOD.e2189_s1933_s1911_r5591_r5625/"));

  m_map["8TEV_ttbarZ_MadGraphPythia"] = rlist( Rpair(119355,"mc14_8TeV.119355.MadGraphPythia_AUET2BCTEQ6L1_ttbarZ.merge.AOD.e1352_s1933_s1911_r5591_r5625/"));
  
  m_map["8TEV_Wenu_Sherpa"] = rlist( Rpair(167740,"mc14_8TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167741,"mc14_8TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167742,"mc14_8TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167761,"mc14_8TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167762,"mc14_8TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167763,"mc14_8TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167770,"mc14_8TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167771,"mc14_8TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167772,"mc14_8TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167779,"mc14_8TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167780,"mc14_8TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167781,"mc14_8TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167788,"mc14_8TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167789,"mc14_8TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(167790,"mc14_8TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(180534,"mc14_8TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(180535,"mc14_8TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				     Rpair(180536,"mc14_8TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"));
  
  
  m_map["8TEV_Wmunu_Sherpa"] = rlist( Rpair(167743,"mc14_8TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167744,"mc14_8TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167745,"mc14_8TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167764,"mc14_8TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167765,"mc14_8TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167766,"mc14_8TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167773,"mc14_8TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167774,"mc14_8TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167775,"mc14_8TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167782,"mc14_8TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167783,"mc14_8TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167784,"mc14_8TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167791,"mc14_8TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167792,"mc14_8TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167793,"mc14_8TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(180537,"mc14_8TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(180538,"mc14_8TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(180539,"mc14_8TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"));
  
  
  m_map["8TEV_Wtaunu_Sherpa"] = rlist( Rpair(167746,"mc14_8TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167747,"mc14_8TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167748,"mc14_8TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167767,"mc14_8TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167768,"mc14_8TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"),
				       Rpair(167769,"mc14_8TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167776,"mc14_8TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167777,"mc14_8TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167778,"mc14_8TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167785,"mc14_8TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167786,"mc14_8TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167787,"mc14_8TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167794,"mc14_8TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167795,"mc14_8TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(167796,"mc14_8TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(180540,"mc14_8TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(180541,"mc14_8TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"),
				       Rpair(180542,"mc14_8TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e1867_s1933_s1911_r5591_r5625_p1784/"));
  
  
  
  m_map["8TEV_Zee_Sherpa"] = rlist( Rpair(167749,"mc14_8TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167749,"mc14_8TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167749,"mc14_8TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167797,"mc14_8TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167798,"mc14_8TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"),
				    Rpair(167799,"mc14_8TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167809,"mc14_8TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167810,"mc14_8TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167811,"mc14_8TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167821,"mc14_8TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167822,"mc14_8TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167823,"mc14_8TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167833,"mc14_8TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167834,"mc14_8TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				    Rpair(167835,"mc14_8TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"));


  m_map["8TEV_Zmumu_Sherpa"] = rlist( Rpair(167752,"mc14_8TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167753,"mc14_8TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167754,"mc14_8TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167800,"mc14_8TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167801,"mc14_8TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167802,"mc14_8TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167812,"mc14_8TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167813,"mc14_8TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167814,"mc14_8TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167824,"mc14_8TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167825,"mc14_8TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167826,"mc14_8TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167836,"mc14_8TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167837,"mc14_8TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167838,"mc14_8TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"));
  
  m_map["8TEV_Ztautau_Sherpa"] = rlist( Rpair(167755,"mc14_8TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167756,"mc14_8TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e1587_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167757,"mc14_8TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e1587_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167803,"mc14_8TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167804,"mc14_8TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167805,"mc14_8TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167815,"mc14_8TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167816,"mc14_8TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167817,"mc14_8TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167827,"mc14_8TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167828,"mc14_8TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167829,"mc14_8TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167839,"mc14_8TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167840,"mc14_8TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167841,"mc14_8TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"));
  
  m_map["8TEV_Znunu_Sherpa"] = rlist( Rpair(167758,"mc14_8TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e1585_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167759,"mc14_8TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e1587_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167760,"mc14_8TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e1587_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167806,"mc14_8TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167807,"mc14_8TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167808,"mc14_8TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167818,"mc14_8TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167819,"mc14_8TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167820,"mc14_8TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e1620_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167830,"mc14_8TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167831,"mc14_8TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"),
				      Rpair(167832,"mc14_8TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167842,"mc14_8TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e1741_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167843,"mc14_8TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"),
				      Rpair(167844,"mc14_8TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e1714_s1933_s1911_r5591_r5625_p1784/"));
  
  
  m_map["8TEV_diboson_Sherpa"] = rlist( Rpair(167799,"mc14_8TeV.161985.Sherpa_CT10_llnunu_WW_EW6_noHiggs.merge.DAOD_SUSY1.e1434_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167811,"mc14_8TeV.161986.Sherpa_CT10_llnunu_ZZ_EW6_noHiggs.merge.DAOD_SUSY1.e1434_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167823,"mc14_8TeV.161987.Sherpa_CT10_lllnu_WZ_EW6_noHiggs.merge.DAOD_SUSY1.e1434_s1933_s1911_r5591_r5625_p1784/"),
					Rpair(167835,"mc14_8TeV.161988.Sherpa_CT10_llll_ZZ_EW6_noHiggs.merge.DAOD_SUSY1.e1434_s1933_s1911_r5591_r5625_p1784/"));

  m_map["TestdijetSUSY1"]=rlist(Rpair(147911,"/nfs/at3/scratch/arodriguez/"));  


  //kt-matching study (SBOTTOM)
  m_map["13TEV_Sbottom_300_290_msb1"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb1/"));
  m_map["13TEV_Sbottom_300_290_msb1.5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb1.5/"));
  m_map["13TEV_Sbottom_300_290_msb2"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb2/"));
  m_map["13TEV_Sbottom_300_290_msb2.5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb2.5/"));
  m_map["13TEV_Sbottom_300_290_msb3"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb3/"));
  m_map["13TEV_Sbottom_300_290_msb4"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb4/"));
  m_map["13TEV_Sbottom_300_290_msb5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb5/"));
  m_map["13TEV_Sbottom_300_290_msb6"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb6/"));
  m_map["13TEV_Sbottom_300_290_msb7"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb7/"));
  m_map["13TEV_Sbottom_300_290_msb8"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb8/"));
  m_map["13TEV_Sbottom_300_290_msb9"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb9/"));
  m_map["13TEV_Sbottom_300_290_msb10"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb10/"));
  m_map["13TEV_Sbottom_300_290_msb12"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_300_290/msb12/"));

  m_map["13TEV_Sbottom_500_490_msb1"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb1/"));
  m_map["13TEV_Sbottom_500_490_msb1.5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb1.5/"));
  m_map["13TEV_Sbottom_500_490_msb2"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb2/"));
  m_map["13TEV_Sbottom_500_490_msb2.5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb2.5/"));
  m_map["13TEV_Sbottom_500_490_msb3"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb3/"));
  m_map["13TEV_Sbottom_500_490_msb4"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb4/"));
  m_map["13TEV_Sbottom_500_490_msb5"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb5/"));
  m_map["13TEV_Sbottom_500_490_msb6"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb6/"));
  m_map["13TEV_Sbottom_500_490_msb7"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb7/"));
  m_map["13TEV_Sbottom_500_490_msb8"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb8/"));
  m_map["13TEV_Sbottom_500_490_msb9"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb9/"));
  m_map["13TEV_Sbottom_500_490_msb10"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb10/"));
  m_map["13TEV_Sbottom_500_490_msb12"] = rlist( Rpair(202249, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_500_490/msb12/"));

  m_map["13TEV_Sbottom_700_690_msb1"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb1/"));
  m_map["13TEV_Sbottom_700_690_msb1.5"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb1.5/"));
  m_map["13TEV_Sbottom_700_690_msb2"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb2/"));
  m_map["13TEV_Sbottom_700_690_msb2.5"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb2.5/"));
  m_map["13TEV_Sbottom_700_690_msb3"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb3/"));
  m_map["13TEV_Sbottom_700_690_msb4"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb4/"));
  m_map["13TEV_Sbottom_700_690_msb5"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb5/"));
  m_map["13TEV_Sbottom_700_690_msb6"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb6/"));
  m_map["13TEV_Sbottom_700_690_msb7"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb7/"));
  m_map["13TEV_Sbottom_700_690_msb8"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb8/"));
  m_map["13TEV_Sbottom_700_690_msb9"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb9/"));
  m_map["13TEV_Sbottom_700_690_msb10"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb10/"));
  m_map["13TEV_Sbottom_700_690_msb12"] = rlist( Rpair(300373, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/ktmatching/BB_700_690/msb12/"));

  //EWK directHG study
  m_map["directHG"] = rlist( Rpair(300604,"/nfs/at3/scratch/tripiana/xAOD/truth/directHG/"));


  //SIGNAL
    m_map["test_stop"] = rlist( Rpair(204422, "/nfs/at3/scratch/sfracchia/SUSY/test_stop/"));    
  //m_map["test_stop"] = rlist( Rpair(204422, "mc14_13TeV.204422.Herwigpp_UEEE4_CTEQ6L1_Tt_T800_L1.merge.DAOD_SUSY1.e3064_s1982_s2008_r5787_r5853_p1872/"));    //CutFlow check Stop0L pre-WT (p1872)

  m_map["13TEV_Stop_800_1"] = rlist( Rpair(204422, "mc14_13TeV.204422.Herwigpp_UEEE4_CTEQ6L1_Tt_T800_L1.merge.AOD.e3064_s1982_s2008_r5787_r5853/"));  
  m_map["13TEV_Stop_800_1_af"] = rlist( Rpair(204422, "mc14_13TeV.204422.Herwigpp_UEEE4_CTEQ6L1_Tt_T800_L1.merge.AOD.e3064_a266_a265_r5853/"));

  //m_map["13TEV_Sbottom_800_1"]   = rlist( Rpair(202266, "mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.AOD.e3064_s1982_s2008_r5787_r5853/"));
  m_map["13TEV_Sbottom_300_290"] = rlist( Rpair(202249, "mc14_13TeV.202249.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_300_290_MET50.merge.AOD.e3064_s1982_s2008_r5787_r5853/"));

  m_map["13TEV_Sbottom_800_1_af"]   = rlist( Rpair(202266, "mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.AOD.e3064_a266_a265_r5853/"));
  m_map["13TEV_Sbottom_300_290_af"] = rlist( Rpair(202249, ""));
  
  m_map["13TEV_Sbottom_800_1_SUSY1"] = rlist( Rpair(202266, "/nfs/atlas-data06/scratch/tripiana/xAOD/mc14/sbottom/SUSY1/"));
  m_map["13TEV_Sbottom_800_1_SUSY1_WT"] = rlist( Rpair(202266, "mc14_13TeV:mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.DAOD_SUSY1.e3064_s1982_s2008_r5787_r5853_p1872/"));
  m_map["test_sbottom"] = rlist( Rpair(202266, "mc14_13TeV:mc14_13TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50.merge.DAOD_SUSY1.e3064_s1982_s2008_r5787_r5853_p1872/"));

  m_map["13TEV_Sbottom_800_1_LOCAL"] = rlist( Rpair(202266, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/SM_BB_direct_800_1_202266_13TeV/"));
  m_map["13TEV_Sbottom_300_290_LOCAL"] = rlist( Rpair(202249, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/SM_BB_direct_300_290_202249_13TeV/"));

  //GGM
  m_map["TestGGM"] = rlist( Rpair(179483, "/nfs/at3/scratch/tripiana/xAOD/truth/GGM/"));

  //BACKGROUND
//   //  m_map["13TEV_ttbar_PowhegPythia"] = rlist( Rpair(110401,"mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.AOD.e2928_s1982_s2008_r5787_r5853/"));
//   m_map["13TEV_ttbar_PowhegPythia"] = rlist( Rpair(110401,"mc14_13TeV:mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.DAOD_SUSY1.e2928_s1982_s2008_r5787_r5853_p1816/"));
//   m_map["13TEV_ttbar_PowhegPythia_af"] = rlist( Rpair(110401,"mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.AOD.e2928_a266_a265_r5853/"));
// 
//   m_map["13TEV_singleTop_PowhegPythia"] = rlist( Rpair(110070,"mc14_13TeV.110070.PowhegPythia_P2012_singletop_tchan_lept_top.merge.AOD.e3049_s1982_s2008_r5787_r5853/"),
// 						 Rpair(110071,"mc14_13TeV.110071.PowhegPythia_P2012_singletop_tchan_lept_antitop.merge.AOD.e3049_s1982_s2008_r5787_r5853/"),
// 						 Rpair(110305,"mc14_13TeV.110305.PowhegPythia_P2012_st_Wtchan_incl_DR.merge.AOD.e3049_s1982_s2008_r5787_r5853/"),
// 						 Rpair(110302,"mc14_13TeV.110302.PowhegPythia_P2012_st_schan_lep.merge.AOD.e3049_s1982_s2008_r5787_r5853/"));
//   
// 
//   //  m_map["13TEV_Part1singleTop_PowhegPythia"] = rlist( Rpair(110302,"mc14_13TeV.110302.PowhegPythia_P2012_st_schan_lep.merge.AOD.e3049_s1982_s2008_r5787_r5853/"));
//   m_map["13TEV_Part1singleTop_PowhegPythia"] = rlist( Rpair(110302,"mc14_13TeV.110302.PowhegPythia_P2012_st_schan_lep.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1816/"));
// 
//   //  m_map["13TEV_Part2singleTop_PowhegPythia"] = rlist( Rpair(110071,"mc14_13TeV.110071.PowhegPythia_P2012_singletop_tchan_lept_antitop.merge.AOD.e3049_s1982_s2008_r5787_r5853/"));
//   m_map["13TEV_Part2singleTop_PowhegPythia"] = rlist( Rpair(110071,"mc14_13TeV.110071.PowhegPythia_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["13TEV_Part3singleTop_PowhegPythia"] = rlist( Rpair(110070,"mc14_13TeV.110070.PowhegPythia_P2012_singletop_tchan_lept_top.merge.AOD.e3049_s1982_s2008_r5787_r5853/"));
//   m_map["13TEV_Part3singleTop_PowhegPythia"] = rlist( Rpair(110070,"mc14_13TeV.110070.PowhegPythia_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1816/"));
//   m_map["13TEV_Part4singleTop_PowhegPythia"] = rlist( Rpair(110305,"mc14_13TeV.110305.PowhegPythia_P2012_st_Wtchan_incl_DR.merge.DAOD_SUSY1.e3049_s1982_s2008_r5787_r5853_p1816/"));
// 
//   m_map["13TEV_ttbarV_MadGraphPythia"] = rlist( Rpair(119353,"mc14_13TeV.119353.MadGraphPythia_AUET2BCTEQ6L1_ttbarW.merge.AOD.e3214_s1982_s2008_r5787_r5853/"),
// 						Rpair(119355,"mc14_13TeV.119355.MadGraphPythia_AUET2BCTEQ6L1_ttbarZ.merge.AOD.e3214_s1982_s2008_r5787_r5853/"));
// 				 /* Rpair(174830,"mc14_13TeV.174830.MadGraphPythia_AUET2BCTEQ6L1_ttbarWjExcl.merge.AOD.e3214_s1982_s2008_r5787_r5853/"), */
// 				 /* Rpair(174832,"mc14_13TeV.174832.MadGraphPythia_AUET2BCTEQ6L1_ttbarZjExcl.merge.AOD.e3214_s1982_s2008_r5787_r5853/"),				  */
// 				 /* Rpair(174831,"mc14_13TeV.174831.MadGraphPythia_AUET2BCTEQ6L1_ttbarWjjIncl.merge.AOD.e3214_s1982_s2008_r5787_r5853/"), */
// 				 /* Rpair(174833,"mc14_13TeV.174833.MadGraphPythia_AUET2BCTEQ6L1_ttbarZjjIncl.merge.AOD.e3214_s1982_s2008_r5787_r5853/"));				  */
// 				 
//   //  m_map["13TEV_ttbarG_MadGraphPythia"] = rlist( Rpair(177998,"mc14_8TeV.177998.MadGraphPythia_AUET2BCTEQ6L1_ttbargammaPt80_noAllHad_fixed.merge.AOD.e2189_s1933_s1911_r5591_r5625/"));
// 				 
//   m_map["13TEV_ttbarVV_MadGraphPythia"] = rlist( Rpair(119583,"mc14_13TeV.119583.MadgraphPythia_AUET2B_CTEQ6L1_ttbarWW.merge.AOD.e3214_s1982_s2008_r5787_r5853/"));


  m_map["13TEV_gjetDP_Pythia8"] = rlist( Rpair(129200,"mc14_13TeV.129200.Pythia8_AU2CTEQ6L1_gammajet_DP17_35.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129201,"mc14_13TeV.129201.Pythia8_AU2CTEQ6L1_gammajet_DP35_50.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129202,"mc14_13TeV.129202.Pythia8_AU2CTEQ6L1_gammajet_DP50_70.merge.AOD.e3023_s1982_s2008_r5787_r5853/"),
					 Rpair(129203,"mc14_13TeV.129203.Pythia8_AU2CTEQ6L1_gammajet_DP70_140.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129204,"mc14_13TeV.129204.Pythia8_AU2CTEQ6L1_gammajet_DP140_280.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129205,"mc14_13TeV.129205.Pythia8_AU2CTEQ6L1_gammajet_DP280_500.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129206,"mc14_13TeV.129206.Pythia8_AU2CTEQ6L1_gammajet_DP500_800.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129207,"mc14_13TeV.129207.Pythia8_AU2CTEQ6L1_gammajet_DP800_1000.merge.AOD.e2981_s1982_s2008_r5787_r5853/"),
					 Rpair(129208,"mc14_13TeV.129208.Pythia8_AU2CTEQ6L1_gammajet_DP1000_inf.merge.AOD.e2981_s1982_s2008_r5787_r5853/"));
  
  
//   m_map["13TEV_Znunu_Sherpa"] = rlist( Rpair(167758,"mc14_13TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167759,"mc14_13TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167760,"mc14_13TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167806,"mc14_13TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167807,"mc14_13TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167808,"mc14_13TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167818,"mc14_13TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167819,"mc14_13TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167820,"mc14_13TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167830,"mc14_13TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167831,"mc14_13TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167832,"mc14_13TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167842,"mc14_13TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167843,"mc14_13TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167844,"mc14_13TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Znunu_hf_Sherpa"] = rlist( Rpair(167758,"mc14_13TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167759,"mc14_13TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167806,"mc14_13TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167807,"mc14_13TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167818,"mc14_13TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167819,"mc14_13TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167830,"mc14_13TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167831,"mc14_13TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167842,"mc14_13TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167843,"mc14_13TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
// 
//   m_map["13TEV_Znunu_lf_Sherpa"] = rlist( Rpair(167760,"mc14_13TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167808,"mc14_13TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167820,"mc14_13TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167832,"mc14_13TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167844,"mc14_13TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
// 
//   m_map["13TEV_Zee_Sherpa"] = rlist( Rpair(167749,"mc14_13TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167750,"mc14_13TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167751,"mc14_13TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167797,"mc14_13TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167798,"mc14_13TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167799,"mc14_13TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167809,"mc14_13TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167810,"mc14_13TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167811,"mc14_13TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167821,"mc14_13TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167822,"mc14_13TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167823,"mc14_13TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167833,"mc14_13TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167834,"mc14_13TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167835,"mc14_13TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Zee_hf_Sherpa"] = rlist( Rpair(167749,"mc14_13TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167750,"mc14_13TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167797,"mc14_13TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167798,"mc14_13TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167809,"mc14_13TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167810,"mc14_13TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167821,"mc14_13TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167822,"mc14_13TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167833,"mc14_13TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167834,"mc14_13TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Zee_lf_Sherpa"] = rlist( Rpair(167751,"mc14_13TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167799,"mc14_13TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167811,"mc14_13TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167823,"mc14_13TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				     Rpair(167835,"mc14_13TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
// 
//   m_map["13TEV_Zmumu_Sherpa"] = rlist(Rpair(167752,"mc14_13TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167753,"mc14_13TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167754,"mc14_13TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167800,"mc14_13TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167801,"mc14_13TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167802,"mc14_13TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167812,"mc14_13TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167813,"mc14_13TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167814,"mc14_13TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167824,"mc14_13TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167825,"mc14_13TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167826,"mc14_13TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167836,"mc14_13TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167837,"mc14_13TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167838,"mc14_13TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Zmumu_hf_Sherpa"] = rlist(Rpair(167752,"mc14_13TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167753,"mc14_13TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167800,"mc14_13TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167801,"mc14_13TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167812,"mc14_13TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167813,"mc14_13TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167824,"mc14_13TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167825,"mc14_13TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167836,"mc14_13TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167837,"mc14_13TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/")); 
//  
//   m_map["13TEV_Zmumu_lf_Sherpa"] = rlist(Rpair(167754,"mc14_13TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167802,"mc14_13TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167814,"mc14_13TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167826,"mc14_13TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167838,"mc14_13TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));  
//   
//   m_map["13TEV_Ztautau_Sherpa"] = rlist( Rpair(167755,"mc14_13TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167756,"mc14_13TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167757,"mc14_13TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167803,"mc14_13TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167804,"mc14_13TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167805,"mc14_13TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167815,"mc14_13TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167816,"mc14_13TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167817,"mc14_13TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167827,"mc14_13TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167828,"mc14_13TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167829,"mc14_13TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167839,"mc14_13TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167840,"mc14_13TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167841,"mc14_13TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Ztautau_hf_Sherpa"] = rlist( Rpair(167755,"mc14_13TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167756,"mc14_13TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167803,"mc14_13TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167804,"mc14_13TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167815,"mc14_13TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167816,"mc14_13TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167827,"mc14_13TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167828,"mc14_13TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167839,"mc14_13TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167840,"mc14_13TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Ztautau_lf_Sherpa"] = rlist( Rpair(167757,"mc14_13TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167805,"mc14_13TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167817,"mc14_13TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167829,"mc14_13TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"),
// 					 Rpair(167841,"mc14_13TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
// 
//   m_map["13TEV_Wenu_Sherpa"] = rlist( Rpair(167740,"mc14_13TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167741,"mc14_13TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167742,"mc14_13TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167761,"mc14_13TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167762,"mc14_13TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167763,"mc14_13TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167770,"mc14_13TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167771,"mc14_13TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167772,"mc14_13TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167779,"mc14_13TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167780,"mc14_13TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167781,"mc14_13TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167788,"mc14_13TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167789,"mc14_13TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167790,"mc14_13TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180534,"mc14_13TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180535,"mc14_13TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180536,"mc14_13TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   
//   m_map["13TEV_Wenu_hf_Sherpa"] = rlist( Rpair(167740,"mc14_13TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167741,"mc14_13TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167761,"mc14_13TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167762,"mc14_13TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167770,"mc14_13TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167771,"mc14_13TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167779,"mc14_13TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167780,"mc14_13TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167788,"mc14_13TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167789,"mc14_13TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180534,"mc14_13TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180535,"mc14_13TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//     
//   m_map["13TEV_Wenu_lf_Sherpa"] = rlist( Rpair(167742,"mc14_13TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167763,"mc14_13TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167772,"mc14_13TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167781,"mc14_13TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(167790,"mc14_13TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				      Rpair(180536,"mc14_13TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   
//   
//   m_map["13TEV_Wmunu_Sherpa"] = rlist( Rpair(167743,"mc14_13TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167744,"mc14_13TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167745,"mc14_13TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167764,"mc14_13TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167765,"mc14_13TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167766,"mc14_13TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167773,"mc14_13TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167774,"mc14_13TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167775,"mc14_13TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167782,"mc14_13TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167783,"mc14_13TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167784,"mc14_13TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167791,"mc14_13TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167792,"mc14_13TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167793,"mc14_13TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180537,"mc14_13TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180538,"mc14_13TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180539,"mc14_13TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//  
//   m_map["13TEV_Wmunu_hf_Sherpa"] = rlist( Rpair(167743,"mc14_13TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167744,"mc14_13TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167764,"mc14_13TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167765,"mc14_13TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167773,"mc14_13TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167774,"mc14_13TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167782,"mc14_13TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167783,"mc14_13TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167791,"mc14_13TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167792,"mc14_13TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180537,"mc14_13TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180538,"mc14_13TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//  
//   m_map["13TEV_Wmunu_lf_Sherpa"] = rlist( Rpair(167745,"mc14_13TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167766,"mc14_13TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167775,"mc14_13TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167784,"mc14_13TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(167793,"mc14_13TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 				       Rpair(180539,"mc14_13TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
// 
//   
//   m_map["13TEV_Wtaunu_Sherpa"] = rlist( Rpair(167746,"mc14_13TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167747,"mc14_13TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167748,"mc14_13TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167767,"mc14_13TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167768,"mc14_13TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167769,"mc14_13TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167776,"mc14_13TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167777,"mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167778,"mc14_13TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167785,"mc14_13TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167786,"mc14_13TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167787,"mc14_13TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167794,"mc14_13TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167795,"mc14_13TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167796,"mc14_13TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180540,"mc14_13TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180541,"mc14_13TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180542,"mc14_13TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
// 					
//   m_map["13TEV_Wtaunu_hf_Sherpa"] = rlist( Rpair(167746,"mc14_13TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167747,"mc14_13TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167767,"mc14_13TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167768,"mc14_13TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167776,"mc14_13TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167777,"mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167785,"mc14_13TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167786,"mc14_13TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167794,"mc14_13TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167795,"mc14_13TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180540,"mc14_13TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180541,"mc14_13TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));					
// 					
//    m_map["13TEV_Wtaunu_lf_Sherpa"] = rlist( Rpair(167748,"mc14_13TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167769,"mc14_13TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167778,"mc14_13TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167787,"mc14_13TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(167796,"mc14_13TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"),
// 					Rpair(180542,"mc14_13TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/")); 

   m_map["TestZnunu167758Pt0"] = rlist( Rpair(167758, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Znunu_167758/"));



   m_map["13TEV_GJet_test"] = rlist( Rpair(177576, "mc14_13TeV.177576.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_CVetoBVeto.merge.AOD.e3083_s1982_s2008_r5787_r5853/"));

  //======================================================================================================================
  //                                                 Grid Data
  //======================================================================================================================

  //DC14 data reproc (repro16_v04) //Updated : this reproc includes trigger info, accesible via TDT
  //note: Run numbers for data containers follow this convention:   '9900XY'  , X=period, Y=sub-period (0 for whole container)
  m_map["8TEV_data12_B"] = rlist( Rpair(990020, "data12_8TeV.periodB.physics_JetTauEtmiss.PhysCont.AOD.repro16_v04/")); //period B
  
  
  //======================================================================================================================
  //                                                 Grid tests
  //======================================================================================================================
  
  
  m_map["TestGrid1"] = rlist( Rpair(167752, "mc14_8TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  
  m_map["TestGrid2"] = rlist( Rpair(167836, "mc14_8TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  
  m_map["ttbar13"] = rlist( Rpair(110401,"mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.AOD.e2928_s1982_s2008_r5787_r5853/"));
  
  m_map["data12_Egamma_202798"] = rlist( Rpair(202798, "data12_8TeV.00202798.physics_Egamma.merge.AOD.r5723_p1751/"));
  m_map["data12_Muons_202798"] = rlist( Rpair(202798, "data12_8TeV.00202798.physics_Muons.merge.AOD.r5723_p1751/"));
  

  //8TeV background samples

  m_map["Wenu167740_8TeVgrid"] = rlist( Rpair(167740, "mc14_8TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167741_8TeVgrid"] = rlist( Rpair(167741, "mc14_8TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167742_8TeVgrid"] = rlist( Rpair(167742, "mc14_8TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167770_8TeVgrid"] = rlist( Rpair(167770, "mc14_8TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167771_8TeVgrid"] = rlist( Rpair(167771, "mc14_8TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167772_8TeVgrid"] = rlist( Rpair(167772, "mc14_8TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167779_8TeVgrid"] = rlist( Rpair(167779, "mc14_8TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167780_8TeVgrid"] = rlist( Rpair(167780, "mc14_8TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167781_8TeVgrid"] = rlist( Rpair(167781, "mc14_8TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wenu180534_8TeVgrid"] = rlist( Rpair(180534, "mc14_8TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wenu180535_8TeVgrid"] = rlist( Rpair(180535, "mc14_8TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wenu180536_8TeVgrid"] = rlist( Rpair(180536, "mc14_8TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167788_8TeVgrid"] = rlist( Rpair(167788, "mc14_8TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167789_8TeVgrid"] = rlist( Rpair(167789, "mc14_8TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167790_8TeVgrid"] = rlist( Rpair(167790, "mc14_8TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167761_8TeVgrid"] = rlist( Rpair(167761, "mc14_8TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167762_8TeVgrid"] = rlist( Rpair(167762, "mc14_8TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wenu167763_8TeVgrid"] = rlist( Rpair(167763, "mc14_8TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167743_8TeVgrid"] = rlist( Rpair(167743, "mc14_8TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167744_8TeVgrid"] = rlist( Rpair(167744, "mc14_8TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167745_8TeVgrid"] = rlist( Rpair(167745, "mc14_8TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167773_8TeVgrid"] = rlist( Rpair(167773, "mc14_8TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167774_8TeVgrid"] = rlist( Rpair(167774, "mc14_8TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167775_8TeVgrid"] = rlist( Rpair(167775, "mc14_8TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167782_8TeVgrid"] = rlist( Rpair(167782, "mc14_8TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167783_8TeVgrid"] = rlist( Rpair(167783, "mc14_8TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167784_8TeVgrid"] = rlist( Rpair(167784, "mc14_8TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu180537_8TeVgrid"] = rlist( Rpair(180537, "mc14_8TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu180538_8TeVgrid"] = rlist( Rpair(180538, "mc14_8TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu180539_8TeVgrid"] = rlist( Rpair(180539, "mc14_8TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167791_8TeVgrid"] = rlist( Rpair(167791, "mc14_8TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167792_8TeVgrid"] = rlist( Rpair(167792, "mc14_8TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167793_8TeVgrid"] = rlist( Rpair(167793, "mc14_8TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167764_8TeVgrid"] = rlist( Rpair(167764, "mc14_8TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167765_8TeVgrid"] = rlist( Rpair(167765, "mc14_8TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wmunu167766_8TeVgrid"] = rlist( Rpair(167766, "mc14_8TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167746_8TeVgrid"] = rlist( Rpair(167746, "mc14_8TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167747_8TeVgrid"] = rlist( Rpair(167747, "mc14_8TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167748_8TeVgrid"] = rlist( Rpair(167748, "mc14_8TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167776_8TeVgrid"] = rlist( Rpair(167776, "mc14_8TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167777_8TeVgrid"] = rlist( Rpair(167777, "mc14_8TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167778_8TeVgrid"] = rlist( Rpair(167778, "mc14_8TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167785_8TeVgrid"] = rlist( Rpair(167785, "mc14_8TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167786_8TeVgrid"] = rlist( Rpair(167786, "mc14_8TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167787_8TeVgrid"] = rlist( Rpair(167787, "mc14_8TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu180540_8TeVgrid"] = rlist( Rpair(180540, "mc14_8TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu180541_8TeVgrid"] = rlist( Rpair(180541, "mc14_8TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu180542_8TeVgrid"] = rlist( Rpair(180542, "mc14_8TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e1867_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167794_8TeVgrid"] = rlist( Rpair(167794, "mc14_8TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167795_8TeVgrid"] = rlist( Rpair(167795, "mc14_8TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167796_8TeVgrid"] = rlist( Rpair(167796, "mc14_8TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167767_8TeVgrid"] = rlist( Rpair(167767, "mc14_8TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167768_8TeVgrid"] = rlist( Rpair(167768, "mc14_8TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Wtaunu167769_8TeVgrid"] = rlist( Rpair(167769, "mc14_8TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee147770_8TeVgrid"] = rlist( Rpair(147770, "mc14_8TeV.147770.Sherpa_CT10_Zee.merge.AOD.e1434_s1933_s1911_r5591_r5625/"));
  m_map["Zee167749_8TeVgrid"] = rlist( Rpair(167749, "mc14_8TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zee167750_8TeVgrid"] = rlist( Rpair(167750, "mc14_8TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zee167751_8TeVgrid"] = rlist( Rpair(167751, "mc14_8TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zee167809_8TeVgrid"] = rlist( Rpair(167809, "mc14_8TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167810_8TeVgrid"] = rlist( Rpair(167810, "mc14_8TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167811_8TeVgrid"] = rlist( Rpair(167811, "mc14_8TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167821_8TeVgrid"] = rlist( Rpair(167821, "mc14_8TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167822_8TeVgrid"] = rlist( Rpair(167822, "mc14_8TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Zee167823_8TeVgrid"] = rlist( Rpair(167823, "mc14_8TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167833_8TeVgrid"] = rlist( Rpair(167833, "mc14_8TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167834_8TeVgrid"] = rlist( Rpair(167834, "mc14_8TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167835_8TeVgrid"] = rlist( Rpair(167835, "mc14_8TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167798_8TeVgrid"] = rlist( Rpair(167798, "mc14_8TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zee167799_8TeVgrid"] = rlist( Rpair(167799, "mc14_8TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu147771_8TeVgrid"] = rlist( Rpair(147771, "mc14_8TeV.147771.Sherpa_CT10_Zmumu.merge.AOD.e1434_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167752_8TeVgrid"] = rlist( Rpair(167752, "mc14_8TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167753_8TeVgrid"] = rlist( Rpair(167753, "mc14_8TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167754_8TeVgrid"] = rlist( Rpair(167754, "mc14_8TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167812_8TeVgrid"] = rlist( Rpair(167812, "mc14_8TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167813_8TeVgrid"] = rlist( Rpair(167813, "mc14_8TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167814_8TeVgrid"] = rlist( Rpair(167814, "mc14_8TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167824_8TeVgrid"] = rlist( Rpair(167824, "mc14_8TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167825_8TeVgrid"] = rlist( Rpair(167825, "mc14_8TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167826_8TeVgrid"] = rlist( Rpair(167826, "mc14_8TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167836_8TeVgrid"] = rlist( Rpair(167836, "mc14_8TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167837_8TeVgrid"] = rlist( Rpair(167837, "mc14_8TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167838_8TeVgrid"] = rlist( Rpair(167838, "mc14_8TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167800_8TeVgrid"] = rlist( Rpair(167800, "mc14_8TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167801_8TeVgrid"] = rlist( Rpair(167801, "mc14_8TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Zmumu167802_8TeVgrid"] = rlist( Rpair(167802, "mc14_8TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167758_8TeVgrid"] = rlist( Rpair(167758, "mc14_8TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167759_8TeVgrid"] = rlist( Rpair(167759, "mc14_8TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.AOD.e1587_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167760_8TeVgrid"] = rlist( Rpair(167760, "mc14_8TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.AOD.e1587_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167818_8TeVgrid"] = rlist( Rpair(167818, "mc14_8TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167819_8TeVgrid"] = rlist( Rpair(167819, "mc14_8TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167820_8TeVgrid"] = rlist( Rpair(167820, "mc14_8TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167830_8TeVgrid"] = rlist( Rpair(167830, "mc14_8TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167831_8TeVgrid"] = rlist( Rpair(167831, "mc14_8TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167832_8TeVgrid"] = rlist( Rpair(167832, "mc14_8TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167842_8TeVgrid"] = rlist( Rpair(167842, "mc14_8TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167843_8TeVgrid"] = rlist( Rpair(167843, "mc14_8TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167844_8TeVgrid"] = rlist( Rpair(167844, "mc14_8TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167806_8TeVgrid"] = rlist( Rpair(167806, "mc14_8TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167807_8TeVgrid"] = rlist( Rpair(167807, "mc14_8TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Znunu167808_8TeVgrid"] = rlist( Rpair(167808, "mc14_8TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e1620_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167755_8TeVgrid"] = rlist( Rpair(167755, "mc14_8TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167756_8TeVgrid"] = rlist( Rpair(167756, "mc14_8TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.AOD.e1587_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167757_8TeVgrid"] = rlist( Rpair(167757, "mc14_8TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.AOD.e1587_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167815_8TeVgrid"] = rlist( Rpair(167815, "mc14_8TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167816_8TeVgrid"] = rlist( Rpair(167816, "mc14_8TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167817_8TeVgrid"] = rlist( Rpair(167817, "mc14_8TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167827_8TeVgrid"] = rlist( Rpair(167827, "mc14_8TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167828_8TeVgrid"] = rlist( Rpair(167828, "mc14_8TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.AOD.e1741_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167829_8TeVgrid"] = rlist( Rpair(167829, "mc14_8TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167839_8TeVgrid"] = rlist( Rpair(167839, "mc14_8TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167840_8TeVgrid"] = rlist( Rpair(167840, "mc14_8TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167841_8TeVgrid"] = rlist( Rpair(167841, "mc14_8TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167803_8TeVgrid"] = rlist( Rpair(167803, "mc14_8TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167804_8TeVgrid"] = rlist( Rpair(167804, "mc14_8TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));
  m_map["Ztautau167805_8TeVgrid"] = rlist( Rpair(167805, "mc14_8TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.AOD.e1714_s1933_s1911_r5591_r5625/"));


//   // 13 TeV xAODs
//   //  m_map["Wenu167740_13TeVgrid"] = rlist( Rpair(167740, "mc14_13TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167740_13TeVgrid"] = rlist( Rpair(167740, "mc14_13TeV.167740.Sherpa_CT10_WenuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167741_13TeVgrid"] = rlist( Rpair(167741, "mc14_13TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167741_13TeVgrid"] = rlist( Rpair(167741, "mc14_13TeV.167741.Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167742_13TeVgrid"] = rlist( Rpair(167742, "mc14_13TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167742_13TeVgrid"] = rlist( Rpair(167742, "mc14_13TeV.167742.Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //m_map["Wenu167770_13TeVgrid"] = rlist( Rpair(167770, "mc14_13TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167770_13TeVgrid"] = rlist( Rpair(167770, "mc14_13TeV.167770.Sherpa_CT10_WenuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816"));
//   //  m_map["Wenu167771_13TeVgrid"] = rlist( Rpair(167771, "mc14_13TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167771_13TeVgrid"] = rlist( Rpair(167771, "mc14_13TeV.167771.Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167772_13TeVgrid"] = rlist( Rpair(167772, "mc14_13TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167772_13TeVgrid"] = rlist( Rpair(167772, "mc14_13TeV.167772.Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167779_13TeVgrid"] = rlist( Rpair(167779, "mc14_13TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167779_13TeVgrid"] = rlist( Rpair(167779, "mc14_13TeV.167779.Sherpa_CT10_WenuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167780_13TeVgrid"] = rlist( Rpair(167780, "mc14_13TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167780_13TeVgrid"] = rlist( Rpair(167780, "mc14_13TeV.167780.Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167781_13TeVgrid"] = rlist( Rpair(167781, "mc14_13TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167781_13TeVgrid"] = rlist( Rpair(167781, "mc14_13TeV.167781.Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu180534_13TeVgrid"] = rlist( Rpair(180534, "mc14_13TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu180534_13TeVgrid"] = rlist( Rpair(180534, "mc14_13TeV.180534.Sherpa_CT10_WenuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu180535_13TeVgrid"] = rlist( Rpair(180535, "mc14_13TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu180535_13TeVgrid"] = rlist( Rpair(180535, "mc14_13TeV.180535.Sherpa_CT10_WenuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu180536_13TeVgrid"] = rlist( Rpair(180536, "mc14_13TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu180536_13TeVgrid"] = rlist( Rpair(180536, "mc14_13TeV.180536.Sherpa_CT10_WenuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167788_13TeVgrid"] = rlist( Rpair(167788, "mc14_13TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167788_13TeVgrid"] = rlist( Rpair(167788, "mc14_13TeV.167788.Sherpa_CT10_WenuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167789_13TeVgrid"] = rlist( Rpair(167789, "mc14_13TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167789_13TeVgrid"] = rlist( Rpair(167789, "mc14_13TeV.167789.Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167790_13TeVgrid"] = rlist( Rpair(167790, "mc14_13TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167790_13TeVgrid"] = rlist( Rpair(167790, "mc14_13TeV.167790.Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167761_13TeVgrid"] = rlist( Rpair(167761, "mc14_13TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167761_13TeVgrid"] = rlist( Rpair(167761, "mc14_13TeV.167761.Sherpa_CT10_WenuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167762_13TeVgrid"] = rlist( Rpair(167762, "mc14_13TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167762_13TeVgrid"] = rlist( Rpair(167762, "mc14_13TeV.167762.Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wenu167763_13TeVgrid"] = rlist( Rpair(167763, "mc14_13TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wenu167763_13TeVgrid"] = rlist( Rpair(167763, "mc14_13TeV.167763.Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167743_13TeVgrid"] = rlist( Rpair(167743, "mc14_13TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167743_13TeVgrid"] = rlist( Rpair(167743, "mc14_13TeV.167743.Sherpa_CT10_WmunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167744_13TeVgrid"] = rlist( Rpair(167744, "mc14_13TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167744_13TeVgrid"] = rlist( Rpair(167744, "mc14_13TeV.167744.Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167745_13TeVgrid"] = rlist( Rpair(167745, "mc14_13TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167745_13TeVgrid"] = rlist( Rpair(167745, "mc14_13TeV.167745.Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167773_13TeVgrid"] = rlist( Rpair(167773, "mc14_13TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167773_13TeVgrid"] = rlist( Rpair(167773, "mc14_13TeV.167773.Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167774_13TeVgrid"] = rlist( Rpair(167774, "mc14_13TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167774_13TeVgrid"] = rlist( Rpair(167774, "mc14_13TeV.167774.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167775_13TeVgrid"] = rlist( Rpair(167775, "mc14_13TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167775_13TeVgrid"] = rlist( Rpair(167775, "mc14_13TeV.167775.Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167782_13TeVgrid"] = rlist( Rpair(167782, "mc14_13TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167782_13TeVgrid"] = rlist( Rpair(167782, "mc14_13TeV.167782.Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167783_13TeVgrid"] = rlist( Rpair(167783, "mc14_13TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167783_13TeVgrid"] = rlist( Rpair(167783, "mc14_13TeV.167783.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167784_13TeVgrid"] = rlist( Rpair(167784, "mc14_13TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167784_13TeVgrid"] = rlist( Rpair(167784, "mc14_13TeV.167784.Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu180537_13TeVgrid"] = rlist( Rpair(180537, "mc14_13TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu180537_13TeVgrid"] = rlist( Rpair(180537, "mc14_13TeV.180537.Sherpa_CT10_WmunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu180538_13TeVgrid"] = rlist( Rpair(180538, "mc14_13TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu180538_13TeVgrid"] = rlist( Rpair(180538, "mc14_13TeV.180538.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu180539_13TeVgrid"] = rlist( Rpair(180539, "mc14_13TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu180539_13TeVgrid"] = rlist( Rpair(180539, "mc14_13TeV.180539.Sherpa_CT10_WmunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167791_13TeVgrid"] = rlist( Rpair(167791, "mc14_13TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167791_13TeVgrid"] = rlist( Rpair(167791, "mc14_13TeV.167791.Sherpa_CT10_WmunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167792_13TeVgrid"] = rlist( Rpair(167792, "mc14_13TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167792_13TeVgrid"] = rlist( Rpair(167792, "mc14_13TeV.167792.Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167793_13TeVgrid"] = rlist( Rpair(167793, "mc14_13TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167793_13TeVgrid"] = rlist( Rpair(167793, "mc14_13TeV.167793.Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167764_13TeVgrid"] = rlist( Rpair(167764, "mc14_13TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167764_13TeVgrid"] = rlist( Rpair(167764, "mc14_13TeV.167764.Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167765_13TeVgrid"] = rlist( Rpair(167765, "mc14_13TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167765_13TeVgrid"] = rlist( Rpair(167765, "mc14_13TeV.167765.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wmunu167766_13TeVgrid"] = rlist( Rpair(167766, "mc14_13TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wmunu167766_13TeVgrid"] = rlist( Rpair(167766, "mc14_13TeV.167766.Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167746_13TeVgrid"] = rlist( Rpair(167746, "mc14_13TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167746_13TeVgrid"] = rlist( Rpair(167746, "mc14_13TeV.167746.Sherpa_CT10_WtaunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167747_13TeVgrid"] = rlist( Rpair(167747, "mc14_13TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167747_13TeVgrid"] = rlist( Rpair(167747, "mc14_13TeV.167747.Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167748_13TeVgrid"] = rlist( Rpair(167748, "mc14_13TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167748_13TeVgrid"] = rlist( Rpair(167748, "mc14_13TeV.167748.Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167776_13TeVgrid"] = rlist( Rpair(167776, "mc14_13TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167776_13TeVgrid"] = rlist( Rpair(167776, "mc14_13TeV.167776.Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167777_13TeVgrid"] = rlist( Rpair(167777, "mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167777_13TeVgrid"] = rlist( Rpair(167777, "mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167778_13TeVgrid"] = rlist( Rpair(167778, "mc14_13TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167778_13TeVgrid"] = rlist( Rpair(167778, "mc14_13TeV.167778.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167785_13TeVgrid"] = rlist( Rpair(167785, "mc14_13TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167785_13TeVgrid"] = rlist( Rpair(167785, "mc14_13TeV.167785.Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167786_13TeVgrid"] = rlist( Rpair(167786, "mc14_13TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167786_13TeVgrid"] = rlist( Rpair(167786, "mc14_13TeV.167786.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167787_13TeVgrid"] = rlist( Rpair(167787, "mc14_13TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167787_13TeVgrid"] = rlist( Rpair(167787, "mc14_13TeV.167787.Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu180540_13TeVgrid"] = rlist( Rpair(180540, "mc14_13TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu180540_13TeVgrid"] = rlist( Rpair(180540, "mc14_13TeV.180540.Sherpa_CT10_WtaunuMassiveCBPt40_70_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu180541_13TeVgrid"] = rlist( Rpair(180541, "mc14_13TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu180541_13TeVgrid"] = rlist( Rpair(180541, "mc14_13TeV.180541.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu180542_13TeVgrid"] = rlist( Rpair(180542, "mc14_13TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu180542_13TeVgrid"] = rlist( Rpair(180542, "mc14_13TeV.180542.Sherpa_CT10_WtaunuMassiveCBPt40_70_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167794_13TeVgrid"] = rlist( Rpair(167794, "mc14_13TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167794_13TeVgrid"] = rlist( Rpair(167794, "mc14_13TeV.167794.Sherpa_CT10_WtaunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167795_13TeVgrid"] = rlist( Rpair(167795, "mc14_13TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167795_13TeVgrid"] = rlist( Rpair(167795, "mc14_13TeV.167795.Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167796_13TeVgrid"] = rlist( Rpair(167796, "mc14_13TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167796_13TeVgrid"] = rlist( Rpair(167796, "mc14_13TeV.167796.Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167767_13TeVgrid"] = rlist( Rpair(167767, "mc14_13TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167767_13TeVgrid"] = rlist( Rpair(167767, "mc14_13TeV.167767.Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167768_13TeVgrid"] = rlist( Rpair(167768, "mc14_13TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167768_13TeVgrid"] = rlist( Rpair(167768, "mc14_13TeV.167768.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Wtaunu167769_13TeVgrid"] = rlist( Rpair(167769, "mc14_13TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.AOD.e2822_s1982_s2008_r5787_r5853/"));
//   m_map["Wtaunu167769_13TeVgrid"] = rlist( Rpair(167769, "mc14_13TeV.167769.Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167749_13TeVgrid"] = rlist( Rpair(167749, "mc14_13TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167749_13TeVgrid"] = rlist( Rpair(167749, "mc14_13TeV.167749.Sherpa_CT10_ZeeMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167750_13TeVgrid"] = rlist( Rpair(167750, "mc14_13TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167750_13TeVgrid"] = rlist( Rpair(167750, "mc14_13TeV.167750.Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167751_13TeVgrid"] = rlist( Rpair(167751, "mc14_13TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167751_13TeVgrid"] = rlist( Rpair(167751, "mc14_13TeV.167751.Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167809_13TeVgrid"] = rlist( Rpair(167809, "mc14_13TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167809_13TeVgrid"] = rlist( Rpair(167809, "mc14_13TeV.167809.Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167810_13TeVgrid"] = rlist( Rpair(167810, "mc14_13TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167810_13TeVgrid"] = rlist( Rpair(167810, "mc14_13TeV.167810.Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167811_13TeVgrid"] = rlist( Rpair(167811, "mc14_13TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167811_13TeVgrid"] = rlist( Rpair(167811, "mc14_13TeV.167811.Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167821_13TeVgrid"] = rlist( Rpair(167821, "mc14_13TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167821_13TeVgrid"] = rlist( Rpair(167821, "mc14_13TeV.167821.Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167822_13TeVgrid"] = rlist( Rpair(167822, "mc14_13TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167822_13TeVgrid"] = rlist( Rpair(167822, "mc14_13TeV.167822.Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167823_13TeVgrid"] = rlist( Rpair(167823, "mc14_13TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167823_13TeVgrid"] = rlist( Rpair(167823, "mc14_13TeV.167823.Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167833_13TeVgrid"] = rlist( Rpair(167833, "mc14_13TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167833_13TeVgrid"] = rlist( Rpair(167833, "mc14_13TeV.167833.Sherpa_CT10_ZeeMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167834_13TeVgrid"] = rlist( Rpair(167834, "mc14_13TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167834_13TeVgrid"] = rlist( Rpair(167834, "mc14_13TeV.167834.Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167835_13TeVgrid"] = rlist( Rpair(167835, "mc14_13TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167835_13TeVgrid"] = rlist( Rpair(167835, "mc14_13TeV.167835.Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167797_13TeVgrid"] = rlist( Rpair(167797, "mc14_13TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167797_13TeVgrid"] = rlist( Rpair(167797, "mc14_13TeV.167797.Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167798_13TeVgrid"] = rlist( Rpair(167798, "mc14_13TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167798_13TeVgrid"] = rlist( Rpair(167798, "mc14_13TeV.167798.Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zee167799_13TeVgrid"] = rlist( Rpair(167799, "mc14_13TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zee167799_13TeVgrid"] = rlist( Rpair(167799, "mc14_13TeV.167799.Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167752_13TeVgrid"] = rlist( Rpair(167752, "mc14_13TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167752_13TeVgrid"] = rlist( Rpair(167752, "mc14_13TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167753_13TeVgrid"] = rlist( Rpair(167753, "mc14_13TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167753_13TeVgrid"] = rlist( Rpair(167753, "mc14_13TeV.167753.Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167754_13TeVgrid"] = rlist( Rpair(167754, "mc14_13TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167754_13TeVgrid"] = rlist( Rpair(167754, "mc14_13TeV.167754.Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167812_13TeVgrid"] = rlist( Rpair(167812, "mc14_13TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167812_13TeVgrid"] = rlist( Rpair(167812, "mc14_13TeV.167812.Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167813_13TeVgrid"] = rlist( Rpair(167813, "mc14_13TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167813_13TeVgrid"] = rlist( Rpair(167813, "mc14_13TeV.167813.Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167814_13TeVgrid"] = rlist( Rpair(167814, "mc14_13TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167814_13TeVgrid"] = rlist( Rpair(167814, "mc14_13TeV.167814.Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167824_13TeVgrid"] = rlist( Rpair(167824, "mc14_13TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167824_13TeVgrid"] = rlist( Rpair(167824, "mc14_13TeV.167824.Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167825_13TeVgrid"] = rlist( Rpair(167825, "mc14_13TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167825_13TeVgrid"] = rlist( Rpair(167825, "mc14_13TeV.167825.Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167826_13TeVgrid"] = rlist( Rpair(167826, "mc14_13TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167826_13TeVgrid"] = rlist( Rpair(167826, "mc14_13TeV.167826.Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167836_13TeVgrid"] = rlist( Rpair(167836, "mc14_13TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167836_13TeVgrid"] = rlist( Rpair(167836, "mc14_13TeV.167836.Sherpa_CT10_ZmumuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167837_13TeVgrid"] = rlist( Rpair(167837, "mc14_13TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167837_13TeVgrid"] = rlist( Rpair(167837, "mc14_13TeV.167837.Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167838_13TeVgrid"] = rlist( Rpair(167838, "mc14_13TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167838_13TeVgrid"] = rlist( Rpair(167838, "mc14_13TeV.167838.Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167800_13TeVgrid"] = rlist( Rpair(167800, "mc14_13TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167800_13TeVgrid"] = rlist( Rpair(167800, "mc14_13TeV.167800.Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167801_13TeVgrid"] = rlist( Rpair(167801, "mc14_13TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167801_13TeVgrid"] = rlist( Rpair(167801, "mc14_13TeV.167801.Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Zmumu167802_13TeVgrid"] = rlist( Rpair(167802, "mc14_13TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Zmumu167802_13TeVgrid"] = rlist( Rpair(167802, "mc14_13TeV.167802.Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167758_13TeVgrid"] = rlist( Rpair(167758, "mc14_13TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167758_13TeVgrid"] = rlist( Rpair(167758, "mc14_13TeV.167758.Sherpa_CT10_ZnunuMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167759_13TeVgrid"] = rlist( Rpair(167759, "mc14_13TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167759_13TeVgrid"] = rlist( Rpair(167759, "mc14_13TeV.167759.Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167760_13TeVgrid"] = rlist( Rpair(167760, "mc14_13TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167760_13TeVgrid"] = rlist( Rpair(167760, "mc14_13TeV.167760.Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167818_13TeVgrid"] = rlist( Rpair(167818, "mc14_13TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167818_13TeVgrid"] = rlist( Rpair(167818, "mc14_13TeV.167818.Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167819_13TeVgrid"] = rlist( Rpair(167819, "mc14_13TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167819_13TeVgrid"] = rlist( Rpair(167819, "mc14_13TeV.167819.Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167820_13TeVgrid"] = rlist( Rpair(167820, "mc14_13TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167820_13TeVgrid"] = rlist( Rpair(167820, "mc14_13TeV.167820.Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167830_13TeVgrid"] = rlist( Rpair(167830, "mc14_13TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167830_13TeVgrid"] = rlist( Rpair(167830, "mc14_13TeV.167830.Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167831_13TeVgrid"] = rlist( Rpair(167831, "mc14_13TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167831_13TeVgrid"] = rlist( Rpair(167831, "mc14_13TeV.167831.Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167832_13TeVgrid"] = rlist( Rpair(167832, "mc14_13TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167832_13TeVgrid"] = rlist( Rpair(167832, "mc14_13TeV.167832.Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167842_13TeVgrid"] = rlist( Rpair(167842, "mc14_13TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167842_13TeVgrid"] = rlist( Rpair(167842, "mc14_13TeV.167842.Sherpa_CT10_ZnunuMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/")); 
//   //  m_map["Znunu167843_13TeVgrid"] = rlist( Rpair(167843, "mc14_13TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167843_13TeVgrid"] = rlist( Rpair(167843, "mc14_13TeV.167843.Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167844_13TeVgrid"] = rlist( Rpair(167844, "mc14_13TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167844_13TeVgrid"] = rlist( Rpair(167844, "mc14_13TeV.167844.Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167806_13TeVgrid"] = rlist( Rpair(167806, "mc14_13TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167806_13TeVgrid"] = rlist( Rpair(167806, "mc14_13TeV.167806.Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167807_13TeVgrid"] = rlist( Rpair(167807, "mc14_13TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167807_13TeVgrid"] = rlist( Rpair(167807, "mc14_13TeV.167807.Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Znunu167808_13TeVgrid"] = rlist( Rpair(167808, "mc14_13TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Znunu167808_13TeVgrid"] = rlist( Rpair(167808, "mc14_13TeV.167808.Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167755_13TeVgrid"] = rlist( Rpair(167755, "mc14_13TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167755_13TeVgrid"] = rlist( Rpair(167755, "mc14_13TeV.167755.Sherpa_CT10_ZtautauMassiveCBPt0_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167756_13TeVgrid"] = rlist( Rpair(167756, "mc14_13TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167756_13TeVgrid"] = rlist( Rpair(167756, "mc14_13TeV.167756.Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816"));
//   //  m_map["Ztautau167757_13TeVgrid"] = rlist( Rpair(167757, "mc14_13TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167757_13TeVgrid"] = rlist( Rpair(167757, "mc14_13TeV.167757.Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167815_13TeVgrid"] = rlist( Rpair(167815, "mc14_13TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167815_13TeVgrid"] = rlist( Rpair(167815, "mc14_13TeV.167815.Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167816_13TeVgrid"] = rlist( Rpair(167816, "mc14_13TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167816_13TeVgrid"] = rlist( Rpair(167816, "mc14_13TeV.167816.Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167817_13TeVgrid"] = rlist( Rpair(167817, "mc14_13TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167817_13TeVgrid"] = rlist( Rpair(167817, "mc14_13TeV.167817.Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167827_13TeVgrid"] = rlist( Rpair(167827, "mc14_13TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167827_13TeVgrid"] = rlist( Rpair(167827, "mc14_13TeV.167827.Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167828_13TeVgrid"] = rlist( Rpair(167828, "mc14_13TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167828_13TeVgrid"] = rlist( Rpair(167828, "mc14_13TeV.167828.Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167829_13TeVgrid"] = rlist( Rpair(167829, "mc14_13TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167829_13TeVgrid"] = rlist( Rpair(167829, "mc14_13TeV.167829.Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167839_13TeVgrid"] = rlist( Rpair(167839, "mc14_13TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167839_13TeVgrid"] = rlist( Rpair(167839, "mc14_13TeV.167839.Sherpa_CT10_ZtautauMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816"));
//   //  m_map["Ztautau167840_13TeVgrid"] = rlist( Rpair(167840, "mc14_13TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167840_13TeVgrid"] = rlist( Rpair(167840, "mc14_13TeV.167840.Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167841_13TeVgrid"] = rlist( Rpair(167841, "mc14_13TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167841_13TeVgrid"] = rlist( Rpair(167841, "mc14_13TeV.167841.Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167803_13TeVgrid"] = rlist( Rpair(167803, "mc14_13TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167803_13TeVgrid"] = rlist( Rpair(167803, "mc14_13TeV.167803.Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167804_13TeVgrid"] = rlist( Rpair(167804, "mc14_13TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167804_13TeVgrid"] = rlist( Rpair(167804, "mc14_13TeV.167804.Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));
//   //  m_map["Ztautau167805_13TeVgrid"] = rlist( Rpair(167805, "mc14_13TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.AOD.e2798_s1982_s2008_r5787_r5853/"));
//   m_map["Ztautau167805_13TeVgrid"] = rlist( Rpair(167805, "mc14_13TeV.167805.Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto.merge.DAOD_SUSY1.e2798_s1982_s2008_r5787_r5853_p1816/"));

  //======================================================================================================================
  //                                                 Local afs tests --> 50ns samples
  //======================================================================================================================
  
  m_map["Znunu167818_13TeV_50ns"] = rlist( Rpair(167818, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167818/"));
  m_map["Znunu167819_13TeV_50ns"] = rlist( Rpair(167819, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167819/"));
  m_map["Znunu167820_13TeV_50ns"] = rlist( Rpair(167820, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167820/"));
  m_map["Znunu167830_13TeV_50ns"] = rlist( Rpair(167830, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167830/"));
  m_map["Znunu167831_13TeV_50ns"] = rlist( Rpair(167831, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167831/"));
  m_map["Znunu167832_13TeV_50ns"] = rlist( Rpair(167832, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167832/"));
  m_map["Znunu167842_13TeV_50ns"] = rlist( Rpair(167842, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167842/")); 
  m_map["Znunu167843_13TeV_50ns"] = rlist( Rpair(167843, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167843/"));
  m_map["Znunu167844_13TeV_50ns"] = rlist( Rpair(167844, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/50ns_Znunu_xAOD/Znunu167844/"));


  //======================================================================================================================
  //                                                 Local afs/nfs tests
  //======================================================================================================================

  //Test procID  
  m_map["TestProcID"] = rlist( Rpair(204556,"/tmp/tripiana/procTest/"));

  //Test SUSY1
  m_map["TestSUSY1"] = rlist( Rpair(167784,"/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Wmunu167784_13TeV_SUSY1/"));

  //Test Smart slimming
  m_map["TestSmart"] = rlist( Rpair(117050, "/nfs/at3/scratch/tripiana/xAOD/smart/"));

  //Test cutflow (vs Kerim's)
  m_map["CF_Zee"] =  rlist( Rpair(167750, "/nfs/at3/scratch/tripiana/xAOD/mc14/Kerim/Zee/"));
  m_map["CF_Zmumu"] =  rlist( Rpair(167754, "/nfs/at3/scratch/tripiana/xAOD/mc14/Kerim/Zmumu/"));

  //Test truth
  m_map["TestTruth"] = rlist( Rpair(202266, "/nfs/at3/scratch/tripiana/xAOD/truth/sbottom/"));
  m_map["TestTruth2"] = rlist( Rpair(202266, "/nfs/at3/scratch/tripiana/xAOD/truth/stop/"));
  m_map["TestTruth3"] = rlist( Rpair(202266, "/nfs/at3/scratch/tripiana/xAOD/truth/Gtt/"));
  
  //Test trigger
  m_map["TestTrig"] = rlist( Rpair(147912, "/nfs/at3/scratch/tripiana/Trigger/test/"));
  m_map["TestTrig2"] = rlist( Rpair(147912, "/nfs/at3/scratch/tripiana/xAOD/test/"));
  m_map["TestTrig3"] = rlist( Rpair(147912, "/nfs/at3/scratch/tripiana/xAOD/test/"),
			      Rpair(147912, "/nfs/at3/scratch/tripiana/Trigger/test/"));
  m_map["TestTrig4"] = rlist( Rpair(147912, "/nfs/at3/scratch/tripiana/Trigger/METmon/datasets/"));

  //Data 
  m_map["TestDataB"] = rlist( Rpair(203432, "/nfs/at3/scratch/tripiana/xAOD/data12/JetTauEtmiss/"));
  
  //New DAOD_IFAE3G derivation
  m_map["TestIFAE3G"] = rlist( Rpair(117050, "/nfs/at3users/users/tripiana/Derivations/WorkArea/run/test/"));
  
  //New test xAOD mc14 (SUSY1 derivation)
  m_map["TestDF"] = rlist( Rpair(117050, "/nfs/at3/scratch/tripiana/xAOD/mc14/ttbarD/"));
  m_map["TestDF13"] = rlist( Rpair(117050, "/nfs/at3/scratch/tripiana/xAOD/mc14/ttbarD13/"));
  m_map["TestTT13"] = rlist( Rpair(117050, "/tmp/tripiana/ttbar13/"));

  //TTbargamma
  m_map["TestTTbarG"] = rlist( Rpair(177998,"/tmp/tripiana/ttbargamma/"));

  //TTbargamma
  m_map["TestTTbarV"] = rlist( Rpair(177998,"/tmp/tripiana/ttbarV/"));

  //New test on 8TeV Znunu xAODs
  m_map["TestZnunu167818"] = rlist( Rpair(167818, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Znunu_167818/"));
  m_map["TestZnunu167819"] = rlist( Rpair(167819, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Znunu_167819/"));
  m_map["TestZnunu167820"] = rlist( Rpair(167820, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Znunu_167820/"));

  m_map["TestZmumu167812"] = rlist( Rpair(167812, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Zmumu_167812/"));
  m_map["TestZmumu167813"] = rlist( Rpair(167813, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Zmumu_167813/"));
  m_map["TestZmumu167814"] = rlist( Rpair(167814, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Zmumu_167814/"));

  m_map["TestWenu167770"] = rlist( Rpair(167770, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wenu_167770/"));
  m_map["TestWenu167771"] = rlist( Rpair(167771, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wenu_167771/"));
  m_map["TestWenu167772"] = rlist( Rpair(167772, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wenu_167772/"));
  m_map["TestWenu167781"] = rlist( Rpair(167781, "/nfs/atlas-data06/scratch/cfischer/SusyAnalysis/XAODSamples/Wenu167781/"));
  m_map["TestWmunu167784"] = rlist( Rpair(167784, "/nfs/atlas-data06/scratch/cfischer/SusyAnalysis/XAODSamples/Wmunu167784/"));

  m_map["TestZnunu167820_13TeV"] = rlist( Rpair(167820, "/nfs/atlas-data06/scratch/cfischer/SusyAnalysis/XAODSamples/Znunu167820_13TeV/"));
  m_map["TestSMBB202249_13TeV"] = rlist( Rpair(202249, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/SM_BB_direct_300_290_202249_13TeV/"));
  m_map["TestSMBB202266_13TeV"] = rlist( Rpair(202266, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/SM_BB_direct_800_1_202266_13TeV/"));

  m_map["TestSMBB202249_8TeV"] = rlist( Rpair(202249, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/SM_BB_direct_300_290_202249_8TeV/"));
  m_map["TestSMBB202266_8TeV"] = rlist( Rpair(202266, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/SM_BB_direct_800_1_202266_8TeV/"));

  m_map["TestZnunu167758Pt0"] = rlist( Rpair(167758, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Znunu_167758/"));
  m_map["TestWmunu167743Pt0"] = rlist( Rpair(167743, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Wmunu_167743/"));

  m_map["TestWmunu167773"] = rlist( Rpair(167773, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wmunu_167773/"));
  m_map["TestWmunu167774"] = rlist( Rpair(167774, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wmunu_167774/"));
  m_map["TestWmunu167775"] = rlist( Rpair(167775, "/nfs/at3/scratch2/arelycg/SUSYANALYSIS_13TeV/xAOD_samples_Testing_oct2014/mc14/Wmunu_167775/"));

  m_map["Wmunu167775_13TeV"] = rlist( Rpair(167775, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Wmunu167775_13TeV/"));
  m_map["Wmunu167784_13TeV"] = rlist( Rpair(167784, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/Wmunu167784_13TeV/"));

  m_map["TestD5_DM50_MS1000_pt150"] = rlist( Rpair(100001, "/nfs/at3/scratch/arelycg/SUSYANALYSIS_13TeV/xAOD_wimp_Valerio_8dec14/D5_dm50_ms1000_pt150/"));
  m_map["TestD5_DM50_MS1000_pt400"] = rlist( Rpair(100002, "/nfs/at3/scratch/arelycg/SUSYANALYSIS_13TeV/xAOD_wimp_Valerio_8dec14/D5_dm50_ms1000_pt400/"));  

  m_map["D5_mDM50_MET100"] = rlist( Rpair(191044, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/D5_mDM50_MET100/"));
  m_map["D5_mDM50_MET300"] = rlist( Rpair(191044, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/D5_mDM50_MET300/"));
  m_map["D5_mDM50_MET500"] = rlist( Rpair(191044, "/nfs/at3/scratch/cfischer/SusyAnalysis/XAODSamples/D5_mDM50_MET500/"));

  //New test xAOD mc14 
  m_map["TestMClocal"] = rlist( Rpair(117050, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/"));

  m_map["TestMCData"] = rlist( Rpair(117050, "/afs/cern.ch/atlas/project/PAT/xAODs/r5591/"),
			       Rpair(900001, "/afs/cern.ch/atlas/project/PAT/xAODs/r5597/"));


  m_map["TestData"] = rlist( Rpair(900001, "/afs/cern.ch/atlas/project/PAT/xAODs/r5597/"));  //we could decode this as (900010-->A, 900011-->A1, etc... if needed)

  m_map["TestMClocal1"] = rlist( Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/Zmumu/"));

  m_map["TestMClocal2"] = rlist( Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/Wmunu/"));

  m_map["TestMClocal3"] = rlist( Rpair(117050, "/nfs/at3/scratch/tripiana/xAOD/mc14/ttbar/"),
				 Rpair(167764, "/nfs/at3/scratch/tripiana/xAOD/mc14/singletop/"));
				 

  m_map["TestMClocal4"] = rlist( Rpair(110101,"/nfs/at3/scratch/tripiana/xAOD/mc14/singletop/"));

  m_map["TestMClocal5"] = rlist( Rpair(117050,"/nfs/at3/scratch/tripiana/xAOD/mc14/ttbar/"));

  m_map["Wtaunu167777_13TeV"] = rlist( Rpair(167777, "mc14_13TeV.167777.Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto.merge.DAOD_SUSY1.e2822_s1982_s2008_r5787_r5853_p1816*"));
  //======================================================================================================================
  //                                                 List of backgrounds for the Baseline analysis
  //======================================================================================================================

  //Zmumu (test Sherpa)
  m_map["TestMCpic"] = rlist( Rpair(167752, "user.tripiana.mc14_8TeV.167752.Sherpa_CT10_ZmumuMassiveCBPt0_BFilter.merge.AOD.e1585_s1933_s1911_r5591_r5625_tid01512435*"));

  //Baseline_ttbar
  m_map["PowhegPythia_P2011C_ttbar"] = rlist( Rpair(117050, "mc14_8TeV.117050.PowhegPythia_P2011C_ttbar*") );

  m_map["D5_mDM50_Mstar1000_MET300"] = rlist( Rpair(191044, "user.vippolit.TestxAOD.191044..AOD.s1982_r5787.test_04_EXT0.13565184"));
  m_map["D5_mDM50_Mstar1000_MET100"] = rlist( Rpair(191043, "user.vippolit.TestxAOD.191043..AOD.s1982_r5787.test_04_EXT0.13565187"));
  m_map["D5_mDM50_Mstar1000_MET500"] = rlist( Rpair(191045, "user.vippolit.TestxAOD.191045..AOD.s1982_r5787.test_04_EXT0.13566771"));
  m_map["D5_mDM400_Mstar1000_MET100"] = rlist( Rpair(191040, "user.vippolit.TestxAOD.191040..AOD.s1982_r5787.test_04_EXT0.13565115"));
  m_map["D5_mDM400_Mstar1000_MET300"] = rlist( Rpair(191041, "user.vippolit:user.vippolit.TestxAOD.191041..AOD.s1982_r5787.test_04_EXT0.13565128"));
  m_map["D5_mDM400_Mstar1000_MET500"] = rlist( Rpair(191042, "user.vippolit:user.vippolit.TestxAOD.191042..AOD.s1982_r5787.test_04_EXT0.13565181"));

  //======================================================================================================================
  //                                                 Signal grids
  //======================================================================================================================
  //BB grid
  m_map["MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50"] = rlist( Rpair(202266, "mc14_8TeV.202266.MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50*"));

  //...

  //GammaJet

  m_map["mc14_13TeV_Gammajet"] = rlist( Rpair(177575,"mc14_13TeV.177575.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"), Rpair(177576,"mc14_13TeV.177576.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"), Rpair(177577, "mc14_13TeV.177577.Sherpa_CT10_SinglePhotonMassiveCBPt500_CVetoBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"), Rpair(177579, "mc14_13TeV.177579.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"), Rpair(177580,"mc14_13TeV.177580.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"),Rpair(177581,"mc14_13TeV.177581.Sherpa_CT10_SinglePhotonMassiveCBPt500_CFilterBVeto.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"),  Rpair(177583,"mc14_13TeV.177583.Sherpa_CT10_SinglePhotonMassiveCBPt140_280_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"),    Rpair(177584,"mc14_13TeV.177584.Sherpa_CT10_SinglePhotonMassiveCBPt280_500_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"),    Rpair(177585,"mc14_13TeV.177585.Sherpa_CT10_SinglePhotonMassiveCBPt500_BFilter.merge.DAOD_SUSY1.e3083_s1982_s2008_r5787_r5853_p1872/"));

};

VRpair RunsMap :: rlist( Rpair run1, Rpair run2, Rpair run3, Rpair run4, Rpair run5,
			 Rpair run6, Rpair run7, Rpair run8, Rpair run9, Rpair run10,
			 Rpair run11, Rpair run12, Rpair run13, Rpair run14, Rpair run15,
			 Rpair run16,  Rpair run17,  Rpair run18,  Rpair run19,  Rpair run20){

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
  if(run16.first > 0) runList.push_back( run16 );
  if(run17.first > 0) runList.push_back( run17 );
  if(run18.first > 0) runList.push_back( run18 );
  if(run19.first > 0) runList.push_back( run19 );
  if(run20.first > 0) runList.push_back( run20 );

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
