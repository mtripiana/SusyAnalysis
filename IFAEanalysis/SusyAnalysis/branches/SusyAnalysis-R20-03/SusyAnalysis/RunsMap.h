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

m_map["test_slist"] = rlist( Rpair(410026, "/nfs/at3/scratch/sfracchia/SUSY/test_SUSY5/"));

m_map["test_ttbar_martin"] = rlist( Rpair(410000, "/nfs/at3/scratch2/tripiana/xAOD/ttbar/SUSY1_new/"));

m_map["signal_sbottom_cutflow"] = rlist( Rpair(387063, "/nfs/at3/scratch/sfracchia/SUSY/sbottom_800_1/"));
m_map["Wenu_cutflow"] = rlist( Rpair(361308, "/nfs/at3/scratch/sfracchia/SUSY/Wenu_cutflow/"));
m_map["Zmumu_cutflow"] = rlist( Rpair(361404, "/nfs/at3/scratch/sfracchia/SUSY/Zmumu_cutflow/"));
m_map["Zee_cutflow"] = rlist( Rpair(361404, "/nfs/at3/scratch/sfracchia/SUSY/zee_cutflow/"));

m_map["ttbar_cutflow"] = rlist( Rpair(410000, "/nfs/at3/scratch/sfracchia/SUSY/ttbar_cutflow/"));
m_map["singletop_cutflow"] = rlist( Rpair(410013, "/nfs/at3/scratch/sfracchia/SUSY/singletop_cutflow/"));


  m_map["ttbar_SUSY1"]       = rlist( Rpair(410000,"mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY1.e3698_s2608_s2183_r7267_r6282_p2452/"));
  m_map["ttbarMET200_SUSY1"] = rlist( Rpair(407012,"mc15_13TeV.407012.PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH.merge.DAOD_SUSY1.e4023_s2608_r6765_r6282_p2452/"));


  m_map["ttZnnqq_SUSY1"]  = rlist( Rpair(410073,"mc15_13TeV.410073.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np0.merge.DAOD_SUSY1.e4111_s2608_s2183_r6869_r6282_p2419/"),
				   Rpair(410074,"mc15_13TeV.410074.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np1.merge.DAOD_SUSY1.e4143_s2608_s2183_r6869_r6282_p2419/"),
				   Rpair(410075,"mc15_13TeV.410075.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np2.merge.DAOD_SUSY1.e4111_s2608_s2183_r6869_r6282_p2419/"));

  //DATA (Stop)
  m_map["data15_periodD_SUSY1"] = rlist( Rpair(000001, "data15_13TeV.periodD.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));
  m_map["data15_periodE_SUSY1"] = rlist( Rpair(000002, "data15_13TeV.periodE.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));
  m_map["data15_periodF_SUSY1"] = rlist( Rpair(000003, "data15_13TeV.periodF.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));
  m_map["data15_periodG_SUSY1"] = rlist( Rpair(000004, "data15_13TeV.periodG.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));
  m_map["data15_periodH_SUSY1"] = rlist( Rpair(000005, "data15_13TeV.periodH.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));
  m_map["data15_periodJ_SUSY1"] = rlist( Rpair(000006, "data15_13TeV.periodJ.physics_Main.PhysCont.DAOD_SUSY1.grp15_v01_p2425/"));


//DEBUG STREAM
 m_map["data15_25ns_SUSY7_debugrec"] = rlist(Rpair(100000, "/nfs/atlas-data07/tripiana/xAOD/data/13TeV/debugrec/"));
 m_map["data15_25ns_SUSY7_test"] = rlist(Rpair(284154, "/nfs/at3/scratch/sfracchia/SUSY/data_test/"));


// 25ns
  m_map["data15_25ns_SUSY7_276262"] = rlist( Rpair(276262, "data15_13TeV.00276262.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276329"] = rlist( Rpair(276329, "data15_13TeV.00276329.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276336"] = rlist( Rpair(276336, "data15_13TeV.00276336.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276416"] = rlist( Rpair(276416, "data15_13TeV.00276416.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276511"] = rlist( Rpair(276511, "data15_13TeV.00276511.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276689"] = rlist( Rpair(276689, "data15_13TeV.00276689.physics_Main.merge.DAOD_SUSY7.f623_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276778"] = rlist( Rpair(276778, "data15_13TeV.00276778.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276790"] = rlist( Rpair(276790, "data15_13TeV.00276790.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276952"] = rlist( Rpair(276952, "data15_13TeV.00276952.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));
  m_map["data15_25ns_SUSY7_276954"] = rlist( Rpair(276954, "data15_13TeV.00276954.physics_Main.merge.DAOD_SUSY7.f620_m1480_p2425/"));  
  m_map["data15_25ns_SUSY7_278880"] = rlist( Rpair(278880, "data15_13TeV.00278880.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_278912"] = rlist( Rpair(278912, "data15_13TeV.00278912.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_278968"] = rlist( Rpair(278968, "data15_13TeV.00278968.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279169"] = rlist( Rpair(279169, "data15_13TeV.00279169.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279259"] = rlist( Rpair(279259, "data15_13TeV.00279259.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279279"] = rlist( Rpair(279279, "data15_13TeV.00279279.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279284"] = rlist( Rpair(279284, "data15_13TeV.00279284.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279345"] = rlist( Rpair(279345, "data15_13TeV.00279345.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279515"] = rlist( Rpair(279515, "data15_13TeV.00279515.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279598"] = rlist( Rpair(279598, "data15_13TeV.00279598.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279685"] = rlist( Rpair(279685, "data15_13TeV.00279685.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279764"] = rlist( Rpair(279764, "data15_13TeV.00279764.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279813"] = rlist( Rpair(279813, "data15_13TeV.00279813.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279867"] = rlist( Rpair(279867, "data15_13TeV.00279867.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));  
  m_map["data15_25ns_SUSY7_279928"] = rlist( Rpair(279928, "data15_13TeV.00279928.physics_Main.merge.DAOD_SUSY7.f628_m1497_p2425/"));
  m_map["data15_25ns_SUSY7_279932"] = rlist( Rpair(279932, "data15_13TeV.00279932.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_279984"] = rlist( Rpair(279984, "data15_13TeV.00279984.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));  
  m_map["data15_25ns_SUSY7_280231"] = rlist( Rpair(280231, "data15_13TeV.00280231.physics_Main.merge.DAOD_SUSY7.f630_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280319"] = rlist( Rpair(280319, "data15_13TeV.00280319.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/")); 
  m_map["data15_25ns_SUSY7_280368"] = rlist( Rpair(280368, "data15_13TeV.00280368.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2436/"));  
  m_map["data15_25ns_SUSY7_280423"] = rlist( Rpair(280423, "data15_13TeV.00280423.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280464"] = rlist( Rpair(280464, "data15_13TeV.00280464.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280500"] = rlist( Rpair(280500, "data15_13TeV.00280500.physics_Main.merge.DAOD_SUSY7.f631_m1504_p2425/"));  
  m_map["data15_25ns_SUSY7_280520"] = rlist( Rpair(280520, "data15_13TeV.00280520.physics_Main.merge.DAOD_SUSY7.f632_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280614"] = rlist( Rpair(280614, "data15_13TeV.00280614.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280673"] = rlist( Rpair(280673, "data15_13TeV.00280673.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280753"] = rlist( Rpair(280753, "data15_13TeV.00280753.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));   
  m_map["data15_25ns_SUSY7_280853"] = rlist( Rpair(280853, "data15_13TeV.00280853.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280862"] = rlist( Rpair(280862, "data15_13TeV.00280862.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));  
  m_map["data15_25ns_SUSY7_280950"] = rlist( Rpair(280950, "data15_13TeV.00280950.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_280977"] = rlist( Rpair(280977, "data15_13TeV.00280977.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_281070"] = rlist( Rpair(281070, "data15_13TeV.00281070.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_281075"] = rlist( Rpair(281075, "data15_13TeV.00281075.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_281074"] = rlist( Rpair(281074, "data15_13TeV.00281074.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_281317"] = rlist( Rpair(281317, "data15_13TeV.00281317.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));  
  m_map["data15_25ns_SUSY7_281385"] = rlist( Rpair(281385, "data15_13TeV.00281385.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_281411"] = rlist( Rpair(281411, "data15_13TeV.00281411.physics_Main.merge.DAOD_SUSY7.f629_m1504_p2425/"));
  m_map["data15_25ns_SUSY7_282625"] = rlist( Rpair(282625, "data15_13TeV.00282625.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));  
  m_map["data15_25ns_SUSY7_282631"] = rlist( Rpair(282631, "data15_13TeV.00282631.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_282712"] = rlist( Rpair(282712, "data15_13TeV.00282712.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_282784"] = rlist( Rpair(282784, "data15_13TeV.00282784.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_282992"] = rlist( Rpair(282992, "data15_13TeV.00282992.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_283074"] = rlist( Rpair(283074, "data15_13TeV.00283074.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_283155"] = rlist( Rpair(283155, "data15_13TeV.00283155.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_283270"] = rlist( Rpair(283270, "data15_13TeV.00283270.physics_Main.merge.DAOD_SUSY7.f640_m1511_p2425/"));
  m_map["data15_25ns_SUSY7_283429"] = rlist( Rpair(283429, "data15_13TeV.00283429.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_283608"] = rlist( Rpair(283608, "data15_13TeV.00283608.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_283780"] = rlist( Rpair(283780, "data15_13TeV.00283780.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284006"] = rlist( Rpair(284006, "data15_13TeV.00284006.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284154"] = rlist( Rpair(284154, "data15_13TeV.00284154.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284213"] = rlist( Rpair(284213, "data15_13TeV.00284213.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284285"] = rlist( Rpair(284285, "data15_13TeV.00284285.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284420"] = rlist( Rpair(284420, "data15_13TeV.00284420.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284427"] = rlist( Rpair(284427, "data15_13TeV.00284427.physics_Main.merge.DAOD_SUSY7.f643_m1518_p2425/"));
  m_map["data15_25ns_SUSY7_284484"] = rlist( Rpair(284484, "data15_13TeV.00284484.physics_Main.merge.DAOD_SUSY7.f644_m1518_p2425/"));


  //======================================================================================================================
  //                                                 Grid sbottom analysis
  //======================================================================================================================


  //Test EOS (data) 900GeV
  m_map["TestEOS"] = rlist( Rpair(264034, "/eos/atlas/atlastier0/rucio/data15_comm/physics_MinBias/00264034/data15_comm.00264034.physics_MinBias.merge.AOD.f577_m1420"));
  
  //======================================================================================================================
  //                                                 Grid tests
  //======================================================================================================================
  
  
  m_map["TestGrid0"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY1.e3698_s2608_s2183_r6630_r6264_p2353/"));

  m_map["25ns_Sherpa_CT10_SinglePhotonPt35_70_BFilter"] = rlist( Rpair(361041, "mc15_13TeV.361041.Sherpa_CT10_SinglePhotonPt35_70_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));   
  m_map["25ns_Sherpa_CT10_SinglePhotonPt35_70_CVetoBVeto"] = rlist( Rpair(361039, "mc15_13TeV.361039.Sherpa_CT10_SinglePhotonPt35_70_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt35_70_CFilterBVeto"] = rlist( Rpair(361040, "mc15_13TeV.361040.Sherpa_CT10_SinglePhotonPt35_70_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));    
  m_map["25ns_Sherpa_CT10_SinglePhotonPt70_140_CVetoBVeto"] = rlist( Rpair(361042, "mc15_13TeV.361042.Sherpa_CT10_SinglePhotonPt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));       
  m_map["25ns_Sherpa_CT10_SinglePhotonPt70_140_CFilterBVeto"] = rlist( Rpair(361043, "mc15_13TeV.361043.Sherpa_CT10_SinglePhotonPt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt70_140_BFilter"] = rlist( Rpair(361044, "mc15_13TeV.361044.Sherpa_CT10_SinglePhotonPt70_140_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt140_280_CVetoBVeto"] = rlist( Rpair(361045, "mc15_13TeV.361045.Sherpa_CT10_SinglePhotonPt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));    
  m_map["25ns_Sherpa_CT10_SinglePhotonPt140_280_CFilterBVeto"] = rlist( Rpair(361046, "mc15_13TeV.361046.Sherpa_CT10_SinglePhotonPt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));       
  m_map["25ns_Sherpa_CT10_SinglePhotonPt140_280_BFilter"] = rlist( Rpair(361047, "mc15_13TeV.361047.Sherpa_CT10_SinglePhotonPt140_280_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt280_500_CVetoBVeto"] = rlist( Rpair(361048, "mc15_13TeV.361048.Sherpa_CT10_SinglePhotonPt280_500_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));    
  m_map["25ns_Sherpa_CT10_SinglePhotonPt280_500_CFilterBVeto"] = rlist( Rpair(361049, "mc15_13TeV.361049.Sherpa_CT10_SinglePhotonPt280_500_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));       
  m_map["25ns_Sherpa_CT10_SinglePhotonPt280_500_BFilter"] = rlist( Rpair(361050, "mc15_13TeV.361050.Sherpa_CT10_SinglePhotonPt280_500_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt500_1000_CVetoBVeto"] = rlist( Rpair(361051, "mc15_13TeV.361051.Sherpa_CT10_SinglePhotonPt500_1000_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt500_1000_CFilterBVeto"] = rlist( Rpair(361052, "mc15_13TeV.361052.Sherpa_CT10_SinglePhotonPt500_1000_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt500_1000_BFilter"] = rlist( Rpair(361053, "mc15_13TeV.361053.Sherpa_CT10_SinglePhotonPt500_1000_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt1000_2000_CVetoBVeto"] = rlist( Rpair(361054, "mc15_13TeV.361054.Sherpa_CT10_SinglePhotonPt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt1000_2000_CFilterBVeto"] = rlist( Rpair(361055, "mc15_13TeV.361055.Sherpa_CT10_SinglePhotonPt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt1000_2000_BFilter"] = rlist( Rpair(361056, "mc15_13TeV.361056.Sherpa_CT10_SinglePhotonPt1000_2000_BFilter.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt2000_4000_CVetoBVeto"] = rlist( Rpair(361057, "mc15_13TeV.361057.Sherpa_CT10_SinglePhotonPt2000_4000_CVetoBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt2000_4000_CFilterBVeto"] = rlist( Rpair(361058, "mc15_13TeV.361058.Sherpa_CT10_SinglePhotonPt2000_4000_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_SinglePhotonPt4000_CFilterBVeto"] = rlist( Rpair(361061, "mc15_13TeV.361061.Sherpa_CT10_SinglePhotonPt4000_CFilterBVeto.merge.DAOD_SUSY7.e3587_s2608_s2183_r6869_r6282_p2419/"));


  m_map["25ns_Sherpa_CT10_WplvWmqq"] = rlist( Rpair(361081, "mc15_13TeV.361081.Sherpa_CT10_WplvWmqq.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_WpqqWmlv"] = rlist( Rpair(361082, "mc15_13TeV.361082.Sherpa_CT10_WpqqWmlv.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_WlvZqq"] = rlist( Rpair(361083, "mc15_13TeV.361083.Sherpa_CT10_WlvZqq.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_WqqZll"] = rlist( Rpair(361084, "mc15_13TeV.361084.Sherpa_CT10_WqqZll.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_WqqZvv"] = rlist( Rpair(361085, "mc15_13TeV.361085.Sherpa_CT10_WqqZvv.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_ZqqZll"] = rlist( Rpair(361086, "mc15_13TeV.361086.Sherpa_CT10_ZqqZll.merge.DAOD_SUSY7.e3926_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_ZqqZvv"] = rlist( Rpair(361087, "mc15_13TeV.361087.Sherpa_CT10_ZqqZvv.merge.DAOD_SUSY7.e3926_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_llll"] = rlist( Rpair(361063, "mc15_13TeV.361063.Sherpa_CT10_llll.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_lllvSFMinus"] = rlist( Rpair(361064, "mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_lllvOFMinus"] = rlist( Rpair(361065, "mc15_13TeV.361065.Sherpa_CT10_lllvOFMinus.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_lllvSFPlus"] = rlist( Rpair(361066, "mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_lllvOFPlus"] = rlist( Rpair(361067, "mc15_13TeV.361067.Sherpa_CT10_lllvOFPlus.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));
  m_map["25ns_Sherpa_CT10_llvv"] = rlist( Rpair(361068, "mc15_13TeV.361068.Sherpa_CT10_llvv.merge.DAOD_SUSY7.e3836_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Wenu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361306, "mc15_13TeV.361306.Sherpa_CT10_Wenu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361307, "mc15_13TeV.361307.Sherpa_CT10_Wenu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361315, "mc15_13TeV.361315.Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361316, "mc15_13TeV.361316.Sherpa_CT10_Wenu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361327, "mc15_13TeV.361327.Sherpa_CT10_Wmunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361330, "mc15_13TeV.361330.Sherpa_CT10_Wmunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361363, "mc15_13TeV.361363.Sherpa_CT10_Wtaunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt500_700_BFilter"] = rlist( Rpair(361386, "mc15_13TeV.361386.Sherpa_CT10_Zee_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt280_500_BFilter"] = rlist( Rpair(361455, "mc15_13TeV.361455.Sherpa_CT10_Znunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361457, "mc15_13TeV.361457.Sherpa_CT10_Znunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt500_700_BFilter"] = rlist( Rpair(361458, "mc15_13TeV.361458.Sherpa_CT10_Znunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt0_70_BFilter"] = rlist( Rpair(361422, "mc15_13TeV.361422.Sherpa_CT10_Ztautau_Pt0_70_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361417, "mc15_13TeV.361417.Sherpa_CT10_Zmumu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361459, "mc15_13TeV.361459.Sherpa_CT10_Znunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));


    m_map["25ns_Sherpa_CT10_Wenu_Pt0_70_BFilter"] = rlist( Rpair(361302, "mc15_13TeV.361302.Sherpa_CT10_Wenu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361309, "mc15_13TeV.361309.Sherpa_CT10_Wenu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361315, "mc15_13TeV.361315.Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361333, "mc15_13TeV.361333.Sherpa_CT10_Wmunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361334, "mc15_13TeV.361334.Sherpa_CT10_Wmunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361337, "mc15_13TeV.361337.Sherpa_CT10_Wmunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt500_700_BFilter"] = rlist( Rpair(361338, "mc15_13TeV.361338.Sherpa_CT10_Wmunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361345, "mc15_13TeV.361345.Sherpa_CT10_Wmunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt280_500_BFilter"] = rlist( Rpair(361359, "mc15_13TeV.361359.Sherpa_CT10_Wtaunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt500_700_CVetoBVeto"] = rlist( Rpair(361384, "mc15_13TeV.361384.Sherpa_CT10_Zee_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt500_700_CFilterBVeto"] = rlist( Rpair(361385, "mc15_13TeV.361385.Sherpa_CT10_Zee_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361397, "mc15_13TeV.361397.Sherpa_CT10_Zmumu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361408, "mc15_13TeV.361408.Sherpa_CT10_Zmumu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361409, "mc15_13TeV.361409.Sherpa_CT10_Zmumu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361417, "mc15_13TeV.361417.Sherpa_CT10_Zmumu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361405, "mc15_13TeV.361405.Sherpa_CT10_Zmumu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361406, "mc15_13TeV.361406.Sherpa_CT10_Zmumu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt280_500_BFilter"] = rlist( Rpair(361407, "mc15_13TeV.361407.Sherpa_CT10_Zmumu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt0_70_BFilter"] = rlist( Rpair(361422, "mc15_13TeV.361422.Sherpa_CT10_Ztautau_Pt0_70_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt280_500_CVetoBVeto"] = rlist( Rpair(361429, "mc15_13TeV.361429.Sherpa_CT10_Ztautau_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt280_500_CFilterBVeto"] = rlist( Rpair(361430, "mc15_13TeV.361430.Sherpa_CT10_Ztautau_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt280_500_BFilter"] = rlist( Rpair(361431, "mc15_13TeV.361431.Sherpa_CT10_Ztautau_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361453, "mc15_13TeV.361453.Sherpa_CT10_Znunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361466, "mc15_13TeV.361466.Sherpa_CT10_Znunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH"] = rlist( Rpair(407012, "mc15_13TeV.407012.PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r6765_r6282_p2419/"));
    
    m_map["25ns_ttbarMET2001_PowhegPythia"] = rlist( Rpair(407012, "mc15_13TeV.407012.PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r6765_r6282_p2419/"));
    m_map["25ns_ttbarMET2002_PowhegPythia"] = rlist( Rpair(407012, "mc15_13TeV.407012.PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r6765_r6282_p2419/"));
    m_map["25ns_ttbarMET2003_PowhegPythia"] = rlist( Rpair(407012, "mc15_13TeV.407012.PowhegPythiaEvtGen_P2012CT10_ttbarMET200_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r6765_r6282_p2419/"));
    
    m_map["25ns_PowhegPythiaEvtGen_P2012_SingleTopSchan_noAllHad_antitop"] = rlist( Rpair(407026, "mc15_13TeV.410026.PowhegPythiaEvtGen_P2012_SingleTopSchan_noAllHad_antitop.merge.DAOD_SUSY7.e3998_s2608_s2183_r6630_r6264_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_SingleTopSchan_noAllHad_top"] = rlist( Rpair(407025, "mc15_13TeV.410025.PowhegPythiaEvtGen_P2012_SingleTopSchan_noAllHad_top.merge.DAOD_SUSY7.e3998_s2608_s2183_r6630_r6264_p2419/"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np2"] = rlist( Rpair(407075, "mc15_13TeV.410075.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np2.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513554_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513554_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513561_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513561_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513555_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513555_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513565_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513565_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513560_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513560_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513553_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513553_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513564_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513564_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513557_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513557_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513562_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513562_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad_tid06513558_00"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06513558_00"));
    
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511395_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511395_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511398_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511398_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511393_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511393_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511389_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511389_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511387_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511387_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511391_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511391_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511388_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511388_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511386_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511386_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511397_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511397_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad_tid06511394_00"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419_tid06511394_00"));       
    m_map["25ns_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_a766_a767_r6264_p2419/"));
    
    m_map["25ns_aMcAtNloHerwigppEvtGen_ttbar_nonallhad"] = rlist( Rpair(410003, "mc15_13TeV.410003.aMcAtNloHerwigppEvtGen_ttbar_nonallhad.merge.DAOD_SUSY7.e3964_a766_a767_r6264_p2419/"));

    m_map["25ns_Sherpa_CT10_Wenu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361300, "mc15_13TeV.361300.Sherpa_CT10_Wenu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361301, "mc15_13TeV.361301.Sherpa_CT10_Wenu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361303, "mc15_13TeV.361303.Sherpa_CT10_Wenu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361304, "mc15_13TeV.361304.Sherpa_CT10_Wenu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt70_140_BFilter"] = rlist( Rpair(361305, "mc15_13TeV.361305.Sherpa_CT10_Wenu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt70_140_BFilter_bis"] = rlist( Rpair(361305, "mc15_13TeV.361305.Sherpa_CT10_Wenu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt140_280_BFilter"] = rlist( Rpair(361308, "mc15_13TeV.361308.Sherpa_CT10_Wenu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361310, "mc15_13TeV.361310.Sherpa_CT10_Wenu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt280_500_BFilter"] = rlist( Rpair(361311, "mc15_13TeV.361311.Sherpa_CT10_Wenu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361312, "mc15_13TeV.361312.Sherpa_CT10_Wenu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361313, "mc15_13TeV.361313.Sherpa_CT10_Wenu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt500_700_BFilter"] = rlist( Rpair(361314, "mc15_13TeV.361314.Sherpa_CT10_Wenu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt700_1000_BFilter"] = rlist( Rpair(361317, "mc15_13TeV.361317.Sherpa_CT10_Wenu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361318, "mc15_13TeV.361318.Sherpa_CT10_Wenu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361319, "mc15_13TeV.361319.Sherpa_CT10_Wenu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt1000_2000_BFilter"] = rlist( Rpair(361320, "mc15_13TeV.361320.Sherpa_CT10_Wenu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361321, "mc15_13TeV.361321.Sherpa_CT10_Wenu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361322, "mc15_13TeV.361322.Sherpa_CT10_Wenu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wenu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361323, "mc15_13TeV.361323.Sherpa_CT10_Wenu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Wmunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361324, "mc15_13TeV.361324.Sherpa_CT10_Wmunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361325, "mc15_13TeV.361325.Sherpa_CT10_Wmunu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt0_70_BFilter"] = rlist( Rpair(361326, "mc15_13TeV.361326.Sherpa_CT10_Wmunu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361328, "mc15_13TeV.361328.Sherpa_CT10_Wmunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt70_140_BFilter"] = rlist( Rpair(361329, "mc15_13TeV.361329.Sherpa_CT10_Wmunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361331, "mc15_13TeV.361331.Sherpa_CT10_Wmunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt140_280_BFilter"] = rlist( Rpair(361332, "mc15_13TeV.361332.Sherpa_CT10_Wmunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt280_500_BFilter"] = rlist( Rpair(361335, "mc15_13TeV.361335.Sherpa_CT10_Wmunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361336, "mc15_13TeV.361336.Sherpa_CT10_Wmunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361339, "mc15_13TeV.361339.Sherpa_CT10_Wmunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361340, "mc15_13TeV.361340.Sherpa_CT10_Wmunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt700_1000_BFilter"] = rlist( Rpair(361341, "mc15_13TeV.361341.Sherpa_CT10_Wmunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361342, "mc15_13TeV.361342.Sherpa_CT10_Wmunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361343, "mc15_13TeV.361343.Sherpa_CT10_Wmunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt1000_2000_BFilter"] = rlist( Rpair(361344, "mc15_13TeV.361344.Sherpa_CT10_Wmunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361346, "mc15_13TeV.361346.Sherpa_CT10_Wmunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wmunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361347, "mc15_13TeV.361347.Sherpa_CT10_Wmunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Wtaunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361348, "mc15_13TeV.361348.Sherpa_CT10_Wtaunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361349, "mc15_13TeV.361349.Sherpa_CT10_Wtaunu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt0_70_BFilter"] = rlist( Rpair(361350, "mc15_13TeV.361350.Sherpa_CT10_Wtaunu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361351, "mc15_13TeV.361351.Sherpa_CT10_Wtaunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361352, "mc15_13TeV.361352.Sherpa_CT10_Wtaunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt70_140_BFilter"] = rlist( Rpair(361353, "mc15_13TeV.361353.Sherpa_CT10_Wtaunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361354, "mc15_13TeV.361354.Sherpa_CT10_Wtaunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361355, "mc15_13TeV.361355.Sherpa_CT10_Wtaunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt140_280_BFilter"] = rlist( Rpair(361356, "mc15_13TeV.361356.Sherpa_CT10_Wtaunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361357, "mc15_13TeV.361357.Sherpa_CT10_Wtaunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361358, "mc15_13TeV.361358.Sherpa_CT10_Wtaunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361360, "mc15_13TeV.361360.Sherpa_CT10_Wtaunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361361, "mc15_13TeV.361361.Sherpa_CT10_Wtaunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt500_700_BFilter"] = rlist( Rpair(361362, "mc15_13TeV.361362.Sherpa_CT10_Wtaunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361364, "mc15_13TeV.361364.Sherpa_CT10_Wtaunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt700_1000_BFilter"] = rlist( Rpair(361365, "mc15_13TeV.361365.Sherpa_CT10_Wtaunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361366, "mc15_13TeV.361366.Sherpa_CT10_Wtaunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361367, "mc15_13TeV.361367.Sherpa_CT10_Wtaunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt1000_2000_BFilter"] = rlist( Rpair(361368, "mc15_13TeV.361368.Sherpa_CT10_Wtaunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361369, "mc15_13TeV.361369.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361370, "mc15_13TeV.361370.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361371, "mc15_13TeV.361371.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Zee_Pt0_70_CVetoBVeto"] = rlist( Rpair(361372, "mc15_13TeV.361372.Sherpa_CT10_Zee_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt0_70_CFilterBVeto"] = rlist( Rpair(361373, "mc15_13TeV.361373.Sherpa_CT10_Zee_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt0_70_BFilter"] = rlist( Rpair(361374, "mc15_13TeV.361374.Sherpa_CT10_Zee_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt70_140_CVetoBVeto"] = rlist( Rpair(361375, "mc15_13TeV.361375.Sherpa_CT10_Zee_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt70_140_CFilterBVeto"] = rlist( Rpair(361376, "mc15_13TeV.361376.Sherpa_CT10_Zee_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt70_140_BFilter"] = rlist( Rpair(361377, "mc15_13TeV.361377.Sherpa_CT10_Zee_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt140_280_CVetoBVeto"] = rlist( Rpair(361378, "mc15_13TeV.361378.Sherpa_CT10_Zee_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt140_280_CFilterBVeto"] = rlist( Rpair(361379, "mc15_13TeV.361379.Sherpa_CT10_Zee_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt140_280_BFilter"] = rlist( Rpair(361380, "mc15_13TeV.361380.Sherpa_CT10_Zee_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt280_500_CVetoBVeto"] = rlist( Rpair(361381, "mc15_13TeV.361381.Sherpa_CT10_Zee_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt280_500_CFilterBVeto"] = rlist( Rpair(361382, "mc15_13TeV.361382.Sherpa_CT10_Zee_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt280_500_BFilter"] = rlist( Rpair(361383, "mc15_13TeV.361383.Sherpa_CT10_Zee_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361387, "mc15_13TeV.361387.Sherpa_CT10_Zee_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361388, "mc15_13TeV.361388.Sherpa_CT10_Zee_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt700_1000_BFilter"] = rlist( Rpair(361389, "mc15_13TeV.361389.Sherpa_CT10_Zee_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361390, "mc15_13TeV.361390.Sherpa_CT10_Zee_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361391, "mc15_13TeV.361391.Sherpa_CT10_Zee_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt1000_2000_BFilter"] = rlist( Rpair(361392, "mc15_13TeV.361392.Sherpa_CT10_Zee_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361393, "mc15_13TeV.361393.Sherpa_CT10_Zee_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361394, "mc15_13TeV.361394.Sherpa_CT10_Zee_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zee_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361395, "mc15_13TeV.361395.Sherpa_CT10_Zee_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Zmumu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361396, "mc15_13TeV.361396.Sherpa_CT10_Zmumu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt0_70_BFilter"] = rlist( Rpair(361398, "mc15_13TeV.361398.Sherpa_CT10_Zmumu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361399, "mc15_13TeV.361399.Sherpa_CT10_Zmumu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361499, "mc15_13TeV.361400.Sherpa_CT10_Zmumu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt70_140_BFilter"] = rlist( Rpair(361401, "mc15_13TeV.361401.Sherpa_CT10_Zmumu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361402, "mc15_13TeV.361402.Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));    
    m_map["25ns_Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto_bis"] = rlist( Rpair(361402, "mc15_13TeV.361402.Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));    
    m_map["25ns_Sherpa_CT10_Zmumu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361403, "mc15_13TeV.361403.Sherpa_CT10_Zmumu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt140_280_BFilter"] = rlist( Rpair(361404, "mc15_13TeV.361404.Sherpa_CT10_Zmumu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt500_700_BFilter"] = rlist( Rpair(361410, "mc15_13TeV.361410.Sherpa_CT10_Zmumu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361411, "mc15_13TeV.361411.Sherpa_CT10_Zmumu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361412, "mc15_13TeV.361412.Sherpa_CT10_Zmumu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt700_1000_BFilter"] = rlist( Rpair(361413, "mc15_13TeV.361413.Sherpa_CT10_Zmumu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361414, "mc15_13TeV.361414.Sherpa_CT10_Zmumu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361415, "mc15_13TeV.361415.Sherpa_CT10_Zmumu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt1000_2000_BFilter"] = rlist( Rpair(361416, "mc15_13TeV.361416.Sherpa_CT10_Zmumu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361418, "mc15_13TeV.361418.Sherpa_CT10_Zmumu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Zmumu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361419, "mc15_13TeV.361419.Sherpa_CT10_Zmumu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Ztautau_Pt0_70_CVetoBVeto"] = rlist( Rpair(361420, "mc15_13TeV.361420.Sherpa_CT10_Ztautau_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));    
    m_map["25ns_Sherpa_CT10_Ztautau_Pt0_70_CVetoBVeto_bis"] = rlist( Rpair(361420, "mc15_13TeV.361420.Sherpa_CT10_Ztautau_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2586_s2174_r6869_r6282_p2419/"));    
    m_map["25ns_Sherpa_CT10_Ztautau_Pt0_70_CFilterBVeto"] = rlist( Rpair(361421, "mc15_13TeV.361421.Sherpa_CT10_Ztautau_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt70_140_CVetoBVeto"] = rlist( Rpair(361423, "mc15_13TeV.361423.Sherpa_CT10_Ztautau_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt70_140_CFilterBVeto"] = rlist( Rpair(361424, "mc15_13TeV.361424.Sherpa_CT10_Ztautau_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt70_140_BFilter"] = rlist( Rpair(361425, "mc15_13TeV.361425.Sherpa_CT10_Ztautau_Pt70_140_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt140_280_CVetoBVeto"] = rlist( Rpair(361426, "mc15_13TeV.361426.Sherpa_CT10_Ztautau_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt140_280_CFilterBVeto"] = rlist( Rpair(361427, "mc15_13TeV.361427.Sherpa_CT10_Ztautau_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt140_280_BFilter"] = rlist( Rpair(361428, "mc15_13TeV.361428.Sherpa_CT10_Ztautau_Pt140_280_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt500_700_CVetoBVeto"] = rlist( Rpair(361432, "mc15_13TeV.361432.Sherpa_CT10_Ztautau_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt500_700_CFilterBVeto"] = rlist( Rpair(361433, "mc15_13TeV.361433.Sherpa_CT10_Ztautau_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt500_700_BFilter"] = rlist( Rpair(361434, "mc15_13TeV.361434.Sherpa_CT10_Ztautau_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361435, "mc15_13TeV.361435.Sherpa_CT10_Ztautau_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361436, "mc15_13TeV.361436.Sherpa_CT10_Ztautau_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt700_1000_BFilter"] = rlist( Rpair(361437, "mc15_13TeV.361437.Sherpa_CT10_Ztautau_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361438, "mc15_13TeV.361438.Sherpa_CT10_Ztautau_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361439, "mc15_13TeV.361439.Sherpa_CT10_Ztautau_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt1000_2000_BFilter"] = rlist( Rpair(361440, "mc15_13TeV.361440.Sherpa_CT10_Ztautau_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361441, "mc15_13TeV.361441.Sherpa_CT10_Ztautau_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361442, "mc15_13TeV.361442.Sherpa_CT10_Ztautau_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Ztautau_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361443, "mc15_13TeV.361443.Sherpa_CT10_Ztautau_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_Sherpa_CT10_Znunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361444, "mc15_13TeV.361444.Sherpa_CT10_Znunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361445, "mc15_13TeV.361445.Sherpa_CT10_Znunu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt0_70_BFilter"] = rlist( Rpair(361446, "mc15_13TeV.361446.Sherpa_CT10_Znunu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361447, "mc15_13TeV.361447.Sherpa_CT10_Znunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361448, "mc15_13TeV.361448.Sherpa_CT10_Znunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt70_140_BFilter"] = rlist( Rpair(361449, "mc15_13TeV.361449.Sherpa_CT10_Znunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361450, "mc15_13TeV.361450.Sherpa_CT10_Znunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361451, "mc15_13TeV.361451.Sherpa_CT10_Znunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt140_280_BFilter"] = rlist( Rpair(361452, "mc15_13TeV.361452.Sherpa_CT10_Znunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361454, "mc15_13TeV.361454.Sherpa_CT10_Znunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361456, "mc15_13TeV.361456.Sherpa_CT10_Znunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361460, "mc15_13TeV.361460.Sherpa_CT10_Znunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt700_1000_BFilter"] = rlist( Rpair(361461, "mc15_13TeV.361461.Sherpa_CT10_Znunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361462, "mc15_13TeV.361462.Sherpa_CT10_Znunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361463, "mc15_13TeV.361463.Sherpa_CT10_Znunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt1000_2000_BFilter"] = rlist( Rpair(361464, "mc15_13TeV.361464.Sherpa_CT10_Znunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361465, "mc15_13TeV.361465.Sherpa_CT10_Znunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_Sherpa_CT10_Znunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361467, "mc15_13TeV.361467.Sherpa_CT10_Znunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_DS_dilepton_top"] = rlist( Rpair(410064, "mc15_13TeV.410064.PowhegPythiaEvtGen_P2012_Wt_DS_dilepton_top.merge.DAOD_SUSY7.e4132_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_DS_dilepton_antitop"] = rlist( Rpair(410065, "mc15_13TeV.410065.PowhegPythiaEvtGen_P2012_Wt_DS_dilepton_antitop.merge.DAOD_SUSY7.e4132_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np1"] = rlist( Rpair(410067, "mc15_13TeV.410067.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np1.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np2"] = rlist( Rpair(410068, "mc15_13TeV.410068.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np2.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np0"] = rlist( Rpair(410073, "mc15_13TeV.410073.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np0.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np1"] = rlist( Rpair(410074, "mc15_13TeV.410074.MadGraphPythia8EvtGen_A14NNPDF23LO_ttZnnqq_Np1.merge.DAOD_SUSY7.e4143_s2608_s2183_r6869_r6282_p2419/"));

    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419/"));    
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451172_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451172_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451178_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451178_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451180_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451180_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451188_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451188_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451171_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451171_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451184_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451184_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451182_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451182_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451190_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451190_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451186_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451186_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad_tid06451176_00"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2419_tid06451176_00"));   
           
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top"] = rlist( Rpair(410011, "mc15_13TeV.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop"] = rlist( Rpair(410012, "mc15_13TeV.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419/"));
    
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top_tid06510753_00"] = rlist( Rpair(410011, "mc15_13TeV.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06510753_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top_tid06510754_00"] = rlist( Rpair(410011, "mc15_13TeV.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06510754_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top_tid06510755_00"] = rlist( Rpair(410011, "mc15_13TeV.410011.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_top.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06510755_00"));  

    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop_tid06515057_00"] = rlist( Rpair(410012, "mc15_13TeV.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06515057_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop_tid06515059_00"] = rlist( Rpair(410012, "mc15_13TeV.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06515059_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop_tid06515060_00"] = rlist( Rpair(410012, "mc15_13TeV.410012.PowhegPythiaEvtGen_P2012_singletop_tchan_lept_antitop.merge.DAOD_SUSY7.e3824_s2608_s2183_r6869_r6282_p2419_tid06515060_00"));

    
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop"] = rlist( Rpair(410014, "mc15_13TeV.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop_tid06511137_00"] = rlist( Rpair(410014, "mc15_13TeV.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06511137_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop_tid06511132_00"] = rlist( Rpair(410014, "mc15_13TeV.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06511132_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop_tid06511135_00"] = rlist( Rpair(410014, "mc15_13TeV.410014.PowhegPythiaEvtGen_P2012_Wt_inclusive_antitop.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06511135_00"));

    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_top"] = rlist( Rpair(410013, "mc15_13TeV.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419/")); 
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_top_tid06513748_00"] = rlist( Rpair(410013, "mc15_13TeV.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06513748_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_top_tid06513746_00"] = rlist( Rpair(410013, "mc15_13TeV.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06513746_00"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_inclusive_top_tid06513745_00"] = rlist( Rpair(410013, "mc15_13TeV.410013.PowhegPythiaEvtGen_P2012_Wt_inclusive_top.merge.DAOD_SUSY7.e3753_s2608_s2183_r6869_r6282_p2419_tid06513745_00"));


    m_map["25ns_PowhegPythiaEvtGen_P2012CT10_Wt_inclusive_top_MET200"] = rlist( Rpair(407019, "mc15_13TeV.407019.PowhegPythiaEvtGen_P2012CT10_Wt_inclusive_top_MET200.merge.DAOD_SUSY7.e4024_s2608_r6765_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012CT10_Wt_inclusive_tbar_MET200"] = rlist( Rpair(407021, "mc15_13TeV.407021.PowhegPythiaEvtGen_P2012CT10_Wt_inclusive_tbar_MET200.merge.DAOD_SUSY7.e4024_s2608_r6765_r6282_p2419/"));
   
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_DS_inclusive_top"] = rlist( Rpair(410062, "mc15_13TeV.410062.PowhegPythiaEvtGen_P2012_Wt_DS_inclusive_top.merge.DAOD_SUSY7.e4132_s2608_s2183_r6869_r6282_p2419/"));
    m_map["25ns_PowhegPythiaEvtGen_P2012_Wt_DS_inclusive_antitop"] = rlist( Rpair(410063, "mc15_13TeV.410063.PowhegPythiaEvtGen_P2012_Wt_DS_inclusive_antitop.merge.DAOD_SUSY7.e4132_s2608_s2183_r6869_r6282_p2419/"));
   
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419/"));
    
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad_tid06510479_00"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06510479_00"));
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad_tid06920931_00"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06920931_00"));
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad_tid06920933_00"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06920933_00"));
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad_tid06920935_00"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06920935_00"));
    m_map["25ns_PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad_tid06920937_00"] = rlist( Rpair(410004, "mc15_13TeV.410004.PowhegHerwigppEvtGen_UEEE5_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06920937_00"));
    
        
    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419/"));

    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad_tid06514748_00"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06514748_00"));
    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad_tid06921158_00"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06921158_00"));
    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad_tid06921162_00"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06921162_00"));
    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad_tid06921165_00"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06921165_00"));
    m_map["25ns_PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad_tid06921169_00"] = rlist( Rpair(410006, "mc15_13TeV.410006.PowhegPythia8EvtGen_A14_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3836_a766_a777_r6282_p2419_tid06921169_00"));

    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np0"] = rlist( Rpair(410066, "mc15_13TeV.410066.MadGraphPythia8EvtGen_A14NNPDF23LO_ttW_Np0.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23_4topSM"] = rlist( Rpair(410080, "mc15_13TeV.410080.MadGraphPythia8EvtGen_A14NNPDF23_4topSM.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW"] = rlist( Rpair(410081, "mc15_13TeV.410081.MadGraphPythia8EvtGen_A14NNPDF23_ttbarWW.merge.DAOD_SUSY7.e4111_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np0"] = rlist( Rpair(410111, "mc15_13TeV.410111.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np0.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np1"] = rlist( Rpair(410112, "mc15_13TeV.410112.MadGraphPythia8EvtGen_A14NNPDF23LO_ttee_Np1.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np0"] = rlist( Rpair(410113, "mc15_13TeV.410113.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np0.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np1"] = rlist( Rpair(410114, "mc15_13TeV.410114.MadGraphPythia8EvtGen_A14NNPDF23LO_ttmumu_Np1.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np0"] = rlist( Rpair(410115, "mc15_13TeV.410115.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np0.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));
    m_map["25ns_MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np1"] = rlist( Rpair(410116, "mc15_13TeV.410116.MadGraphPythia8EvtGen_A14NNPDF23LO_tttautau_Np1.merge.DAOD_SUSY7.e4265_s2608_s2183_r6869_r6282_p2419"));

    m_map["25ns_MGPy8EG_A14N_BB_direct_250_230_MET100"] = rlist( Rpair(387003, "mc15_13TeV.387003.MGPy8EG_A14N_BB_direct_250_230_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_250_245_MET100"] = rlist( Rpair(387004, "mc15_13TeV.387004.MGPy8EG_A14N_BB_direct_250_245_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_300_150_MET100"] = rlist( Rpair(387005, "mc15_13TeV.387005.MGPy8EG_A14N_BB_direct_300_150_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_300_280_MET100"] = rlist( Rpair(387006, "mc15_13TeV.387006.MGPy8EG_A14N_BB_direct_300_280_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_300_295_MET100"] = rlist( Rpair(387007, "mc15_13TeV.387007.MGPy8EG_A14N_BB_direct_300_295_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_350_330_MET100"] = rlist( Rpair(387008, "mc15_13TeV.387008.MGPy8EG_A14N_BB_direct_350_330_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_350_345_MET100"] = rlist( Rpair(387009, "mc15_13TeV.387009.MGPy8EG_A14N_BB_direct_350_345_MET100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_1"] = rlist( Rpair(387010, "mc15_13TeV.387010.MGPy8EG_A14N_BB_direct_400_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_100"] = rlist( Rpair(387011, "mc15_13TeV.387011.MGPy8EG_A14N_BB_direct_400_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_200"] = rlist( Rpair(387012, "mc15_13TeV.387012.MGPy8EG_A14N_BB_direct_400_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_300"] = rlist( Rpair(387013, "mc15_13TeV.387013.MGPy8EG_A14N_BB_direct_400_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_380"] = rlist( Rpair(387014, "mc15_13TeV.387014.MGPy8EG_A14N_BB_direct_400_380.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_400_395"] = rlist( Rpair(387015, "mc15_13TeV.387015.MGPy8EG_A14N_BB_direct_400_395.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_450_400"] = rlist( Rpair(387016, "mc15_13TeV.387016.MGPy8EG_A14N_BB_direct_450_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_450_430"] = rlist( Rpair(387017, "mc15_13TeV.387017.MGPy8EG_A14N_BB_direct_450_430.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_450_445"] = rlist( Rpair(387018, "mc15_13TeV.387018.MGPy8EG_A14N_BB_direct_450_445.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_1"] = rlist( Rpair(387019, "mc15_13TeV.387019.MGPy8EG_A14N_BB_direct_500_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_100"] = rlist( Rpair(387020, "mc15_13TeV.387020.MGPy8EG_A14N_BB_direct_500_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_200"] = rlist( Rpair(387021, "mc15_13TeV.387021.MGPy8EG_A14N_BB_direct_500_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_300"] = rlist( Rpair(387022, "mc15_13TeV.387022.MGPy8EG_A14N_BB_direct_500_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_400"] = rlist( Rpair(387023, "mc15_13TeV.387023.MGPy8EG_A14N_BB_direct_500_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_480"] = rlist( Rpair(387024, "mc15_13TeV.387024.MGPy8EG_A14N_BB_direct_500_480.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_500_495"] = rlist( Rpair(387025, "mc15_13TeV.387025.MGPy8EG_A14N_BB_direct_500_495.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_1"] = rlist( Rpair(387026, "mc15_13TeV.387026.MGPy8EG_A14N_BB_direct_550_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_100"] = rlist( Rpair(387027, "mc15_13TeV.387027.MGPy8EG_A14N_BB_direct_550_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_200"] = rlist( Rpair(387028, "mc15_13TeV.387028.MGPy8EG_A14N_BB_direct_550_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_300"] = rlist( Rpair(387029, "mc15_13TeV.387029.MGPy8EG_A14N_BB_direct_550_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_400"] = rlist( Rpair(387030, "mc15_13TeV.387030.MGPy8EG_A14N_BB_direct_550_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_500"] = rlist( Rpair(387031, "mc15_13TeV.387031.MGPy8EG_A14N_BB_direct_550_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_530"] = rlist( Rpair(387032, "mc15_13TeV.387032.MGPy8EG_A14N_BB_direct_550_530.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_550_545"] = rlist( Rpair(387033, "mc15_13TeV.387033.MGPy8EG_A14N_BB_direct_550_545.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_1"] = rlist( Rpair(387034, "mc15_13TeV.387034.MGPy8EG_A14N_BB_direct_600_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_100"] = rlist( Rpair(387035, "mc15_13TeV.387035.MGPy8EG_A14N_BB_direct_600_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_200"] = rlist( Rpair(387036, "mc15_13TeV.387036.MGPy8EG_A14N_BB_direct_600_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_300"] = rlist( Rpair(387037, "mc15_13TeV.387037.MGPy8EG_A14N_BB_direct_600_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_400"] = rlist( Rpair(387038, "mc15_13TeV.387038.MGPy8EG_A14N_BB_direct_600_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_500"] = rlist( Rpair(387039, "mc15_13TeV.387039.MGPy8EG_A14N_BB_direct_600_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_580"] = rlist( Rpair(387040, "mc15_13TeV.387040.MGPy8EG_A14N_BB_direct_600_580.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_600_595"] = rlist( Rpair(387041, "mc15_13TeV.387041.MGPy8EG_A14N_BB_direct_600_595.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_1"] = rlist( Rpair(387042, "mc15_13TeV.387042.MGPy8EG_A14N_BB_direct_650_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_100"] = rlist( Rpair(387043, "mc15_13TeV.387043.MGPy8EG_A14N_BB_direct_650_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_200"] = rlist( Rpair(387044, "mc15_13TeV.387044.MGPy8EG_A14N_BB_direct_650_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_300"] = rlist( Rpair(387045, "mc15_13TeV.387045.MGPy8EG_A14N_BB_direct_650_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_400"] = rlist( Rpair(387046, "mc15_13TeV.387046.MGPy8EG_A14N_BB_direct_650_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_500"] = rlist( Rpair(387047, "mc15_13TeV.387047.MGPy8EG_A14N_BB_direct_650_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_650_600"] = rlist( Rpair(387048, "mc15_13TeV.387048.MGPy8EG_A14N_BB_direct_650_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_1"] = rlist( Rpair(387049, "mc15_13TeV.387049.MGPy8EG_A14N_BB_direct_700_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_100"] = rlist( Rpair(387050, "mc15_13TeV.387050.MGPy8EG_A14N_BB_direct_700_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_200"] = rlist( Rpair(387051, "mc15_13TeV.387051.MGPy8EG_A14N_BB_direct_700_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_300"] = rlist( Rpair(387052, "mc15_13TeV.387052.MGPy8EG_A14N_BB_direct_700_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_400"] = rlist( Rpair(387053, "mc15_13TeV.387053.MGPy8EG_A14N_BB_direct_700_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_500"] = rlist( Rpair(387054, "mc15_13TeV.387054.MGPy8EG_A14N_BB_direct_700_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_700_600"] = rlist( Rpair(387055, "mc15_13TeV.387055.MGPy8EG_A14N_BB_direct_700_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_1"] = rlist( Rpair(387056, "mc15_13TeV.387056.MGPy8EG_A14N_BB_direct_750_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_100"] = rlist( Rpair(387057, "mc15_13TeV.387057.MGPy8EG_A14N_BB_direct_750_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_200"] = rlist( Rpair(387058, "mc15_13TeV.387058.MGPy8EG_A14N_BB_direct_750_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_300"] = rlist( Rpair(387059, "mc15_13TeV.387059.MGPy8EG_A14N_BB_direct_750_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_400"] = rlist( Rpair(387060, "mc15_13TeV.387060.MGPy8EG_A14N_BB_direct_750_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_500"] = rlist( Rpair(387061, "mc15_13TeV.387061.MGPy8EG_A14N_BB_direct_750_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_750_600"] = rlist( Rpair(387062, "mc15_13TeV.387062.MGPy8EG_A14N_BB_direct_750_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_1"] = rlist( Rpair(387063, "mc15_13TeV.387063.MGPy8EG_A14N_BB_direct_800_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_100"] = rlist( Rpair(387064, "mc15_13TeV.387064.MGPy8EG_A14N_BB_direct_800_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_150"] = rlist( Rpair(387065, "mc15_13TeV.387065.MGPy8EG_A14N_BB_direct_800_150.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_200"] = rlist( Rpair(387066, "mc15_13TeV.387066.MGPy8EG_A14N_BB_direct_800_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_250"] = rlist( Rpair(387067, "mc15_13TeV.387067.MGPy8EG_A14N_BB_direct_800_250.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_300"] = rlist( Rpair(387068, "mc15_13TeV.387068.MGPy8EG_A14N_BB_direct_800_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_350"] = rlist( Rpair(387069, "mc15_13TeV.387069.MGPy8EG_A14N_BB_direct_800_350.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_400"] = rlist( Rpair(387070, "mc15_13TeV.387070.MGPy8EG_A14N_BB_direct_800_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_450"] = rlist( Rpair(387071, "mc15_13TeV.387071.MGPy8EG_A14N_BB_direct_800_450.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_50"] = rlist( Rpair(387072, "mc15_13TeV.387072.MGPy8EG_A14N_BB_direct_800_50.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_500"] = rlist( Rpair(387073, "mc15_13TeV.387073.MGPy8EG_A14N_BB_direct_800_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_550"] = rlist( Rpair(387074, "mc15_13TeV.387074.MGPy8EG_A14N_BB_direct_800_550.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_800_600"] = rlist( Rpair(387075, "mc15_13TeV.387075.MGPy8EG_A14N_BB_direct_800_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_1"] = rlist( Rpair(387076, "mc15_13TeV.387076.MGPy8EG_A14N_BB_direct_850_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_100"] = rlist( Rpair(387077, "mc15_13TeV.387077.MGPy8EG_A14N_BB_direct_850_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_150"] = rlist( Rpair(387078, "mc15_13TeV.387078.MGPy8EG_A14N_BB_direct_850_150.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_200"] = rlist( Rpair(387079, "mc15_13TeV.387079.MGPy8EG_A14N_BB_direct_850_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_250"] = rlist( Rpair(387080, "mc15_13TeV.387080.MGPy8EG_A14N_BB_direct_850_250.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_300"] = rlist( Rpair(387081, "mc15_13TeV.387081.MGPy8EG_A14N_BB_direct_850_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_350"] = rlist( Rpair(387082, "mc15_13TeV.387082.MGPy8EG_A14N_BB_direct_850_350.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_400"] = rlist( Rpair(387083, "mc15_13TeV.387083.MGPy8EG_A14N_BB_direct_850_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_450"] = rlist( Rpair(387084, "mc15_13TeV.387084.MGPy8EG_A14N_BB_direct_850_450.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_50"] = rlist( Rpair(387085, "mc15_13TeV.387085.MGPy8EG_A14N_BB_direct_850_50.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_500"] = rlist( Rpair(387086, "mc15_13TeV.387086.MGPy8EG_A14N_BB_direct_850_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_550"] = rlist( Rpair(387087, "mc15_13TeV.387087.MGPy8EG_A14N_BB_direct_850_550.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_850_600"] = rlist( Rpair(387088, "mc15_13TeV.387088.MGPy8EG_A14N_BB_direct_850_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_1"] = rlist( Rpair(387089, "mc15_13TeV.387089.MGPy8EG_A14N_BB_direct_900_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_100"] = rlist( Rpair(387090, "mc15_13TeV.387090.MGPy8EG_A14N_BB_direct_900_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_150"] = rlist( Rpair(387091, "mc15_13TeV.387091.MGPy8EG_A14N_BB_direct_900_150.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_200"] = rlist( Rpair(387092, "mc15_13TeV.387092.MGPy8EG_A14N_BB_direct_900_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_250"] = rlist( Rpair(387093, "mc15_13TeV.387093.MGPy8EG_A14N_BB_direct_900_250.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_300"] = rlist( Rpair(387094, "mc15_13TeV.387094.MGPy8EG_A14N_BB_direct_900_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_350"] = rlist( Rpair(387095, "mc15_13TeV.387095.MGPy8EG_A14N_BB_direct_900_350.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_400"] = rlist( Rpair(387096, "mc15_13TeV.387096.MGPy8EG_A14N_BB_direct_900_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_450"] = rlist( Rpair(387097, "mc15_13TeV.387097.MGPy8EG_A14N_BB_direct_900_450.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_50"] = rlist( Rpair(387098, "mc15_13TeV.387098.MGPy8EG_A14N_BB_direct_900_50.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_500"] = rlist( Rpair(387099, "mc15_13TeV.387099.MGPy8EG_A14N_BB_direct_900_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_550"] = rlist( Rpair(387100, "mc15_13TeV.387100.MGPy8EG_A14N_BB_direct_900_550.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_900_600"] = rlist( Rpair(387101, "mc15_13TeV.387101.MGPy8EG_A14N_BB_direct_900_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_1"] = rlist( Rpair(387102, "mc15_13TeV.387102.MGPy8EG_A14N_BB_direct_950_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_100"] = rlist( Rpair(387103, "mc15_13TeV.387103.MGPy8EG_A14N_BB_direct_950_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_150"] = rlist( Rpair(387104, "mc15_13TeV.387104.MGPy8EG_A14N_BB_direct_950_150.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_200"] = rlist( Rpair(387105, "mc15_13TeV.387105.MGPy8EG_A14N_BB_direct_950_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_250"] = rlist( Rpair(387106, "mc15_13TeV.387106.MGPy8EG_A14N_BB_direct_950_250.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_300"] = rlist( Rpair(387107, "mc15_13TeV.387107.MGPy8EG_A14N_BB_direct_950_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_350"] = rlist( Rpair(387108, "mc15_13TeV.387108.MGPy8EG_A14N_BB_direct_950_350.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_400"] = rlist( Rpair(387109, "mc15_13TeV.387109.MGPy8EG_A14N_BB_direct_950_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_450"] = rlist( Rpair(387110, "mc15_13TeV.387110.MGPy8EG_A14N_BB_direct_950_450.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_50"] = rlist( Rpair(387111, "mc15_13TeV.387111.MGPy8EG_A14N_BB_direct_950_50.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_500"] = rlist( Rpair(387112, "mc15_13TeV.387112.MGPy8EG_A14N_BB_direct_950_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_550"] = rlist( Rpair(387113, "mc15_13TeV.387113.MGPy8EG_A14N_BB_direct_950_550.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_950_600"] = rlist( Rpair(387114, "mc15_13TeV.387114.MGPy8EG_A14N_BB_direct_950_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_1"] = rlist( Rpair(387115, "mc15_13TeV.387115.MGPy8EG_A14N_BB_direct_1000_1.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));  
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_100"] = rlist( Rpair(387116, "mc15_13TeV.387116.MGPy8EG_A14N_BB_direct_1000_100.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_150"] = rlist( Rpair(387117, "mc15_13TeV.387117.MGPy8EG_A14N_BB_direct_1000_150.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_200"] = rlist( Rpair(387118, "mc15_13TeV.387118.MGPy8EG_A14N_BB_direct_1000_200.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_250"] = rlist( Rpair(387119, "mc15_13TeV.387119.MGPy8EG_A14N_BB_direct_1000_250.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_300"] = rlist( Rpair(387120, "mc15_13TeV.387120.MGPy8EG_A14N_BB_direct_1000_300.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_350"] = rlist( Rpair(387121, "mc15_13TeV.387121.MGPy8EG_A14N_BB_direct_1000_350.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_400"] = rlist( Rpair(387122, "mc15_13TeV.387122.MGPy8EG_A14N_BB_direct_1000_400.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_450"] = rlist( Rpair(387123, "mc15_13TeV.387123.MGPy8EG_A14N_BB_direct_1000_450.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_50"] = rlist( Rpair(387124, "mc15_13TeV.387124.MGPy8EG_A14N_BB_direct_1000_50.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/")); 
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_500"] = rlist( Rpair(387125, "mc15_13TeV.387125.MGPy8EG_A14N_BB_direct_1000_500.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_550"] = rlist( Rpair(387126, "mc15_13TeV.387126.MGPy8EG_A14N_BB_direct_1000_550.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));
    m_map["25ns_MGPy8EG_A14N_BB_direct_1000_600"] = rlist( Rpair(387127, "mc15_13TeV.387127.MGPy8EG_A14N_BB_direct_1000_600.merge.DAOD_SUSY7.e3994_a766_a768_r6282_p2419/"));

//mc15b

    m_map["mc15b_Sherpa_CT10_Wenu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361303, "mc15_13TeV.361303.Sherpa_CT10_Wenu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361304, "mc15_13TeV.361304.Sherpa_CT10_Wenu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt70_140_BFilter"] = rlist( Rpair(361305, "mc15_13TeV.361305.Sherpa_CT10_Wenu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361306, "mc15_13TeV.361306.Sherpa_CT10_Wenu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361307, "mc15_13TeV.361307.Sherpa_CT10_Wenu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt140_280_BFilter"] = rlist( Rpair(361308, "mc15_13TeV.361308.Sherpa_CT10_Wenu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361309, "mc15_13TeV.361309.Sherpa_CT10_Wenu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361310, "mc15_13TeV.361310.Sherpa_CT10_Wenu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt280_500_BFilter"] = rlist( Rpair(361311, "mc15_13TeV.361311.Sherpa_CT10_Wenu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361312, "mc15_13TeV.361312.Sherpa_CT10_Wenu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361313, "mc15_13TeV.361313.Sherpa_CT10_Wenu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt500_700_BFilter"] = rlist( Rpair(361314, "mc15_13TeV.361314.Sherpa_CT10_Wenu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361315, "mc15_13TeV.361315.Sherpa_CT10_Wenu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361316, "mc15_13TeV.361316.Sherpa_CT10_Wenu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt700_1000_BFilter"] = rlist( Rpair(361317, "mc15_13TeV.361317.Sherpa_CT10_Wenu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361318, "mc15_13TeV.361318.Sherpa_CT10_Wenu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361319, "mc15_13TeV.361319.Sherpa_CT10_Wenu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt1000_2000_BFilter"] = rlist( Rpair(361320, "mc15_13TeV.361320.Sherpa_CT10_Wenu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361321, "mc15_13TeV.361321.Sherpa_CT10_Wenu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361322, "mc15_13TeV.361322.Sherpa_CT10_Wenu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wenu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361323, "mc15_13TeV.361323.Sherpa_CT10_Wenu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361324, "mc15_13TeV.361324.Sherpa_CT10_Wmunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361327, "mc15_13TeV.361327.Sherpa_CT10_Wmunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361328, "mc15_13TeV.361328.Sherpa_CT10_Wmunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt70_140_BFilter"] = rlist( Rpair(361329, "mc15_13TeV.361329.Sherpa_CT10_Wmunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361330, "mc15_13TeV.361330.Sherpa_CT10_Wmunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361331, "mc15_13TeV.361331.Sherpa_CT10_Wmunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt140_280_BFilter"] = rlist( Rpair(361332, "mc15_13TeV.361332.Sherpa_CT10_Wmunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361333, "mc15_13TeV.361333.Sherpa_CT10_Wmunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361334, "mc15_13TeV.361334.Sherpa_CT10_Wmunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt280_500_BFilter"] = rlist( Rpair(361335, "mc15_13TeV.361335.Sherpa_CT10_Wmunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361336, "mc15_13TeV.361336.Sherpa_CT10_Wmunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361337, "mc15_13TeV.361337.Sherpa_CT10_Wmunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt500_700_BFilter"] = rlist( Rpair(361338, "mc15_13TeV.361338.Sherpa_CT10_Wmunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361339, "mc15_13TeV.361339.Sherpa_CT10_Wmunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361340, "mc15_13TeV.361340.Sherpa_CT10_Wmunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt700_1000_BFilter"] = rlist( Rpair(361341, "mc15_13TeV.361341.Sherpa_CT10_Wmunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361342, "mc15_13TeV.361342.Sherpa_CT10_Wmunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361343, "mc15_13TeV.361343.Sherpa_CT10_Wmunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt1000_2000_BFilter"] = rlist( Rpair(361344, "mc15_13TeV.361344.Sherpa_CT10_Wmunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361345, "mc15_13TeV.361345.Sherpa_CT10_Wmunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361346, "mc15_13TeV.361346.Sherpa_CT10_Wmunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wmunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361347, "mc15_13TeV.361347.Sherpa_CT10_Wmunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361348, "mc15_13TeV.361348.Sherpa_CT10_Wtaunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361351, "mc15_13TeV.361351.Sherpa_CT10_Wtaunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361352, "mc15_13TeV.361352.Sherpa_CT10_Wtaunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt70_140_BFilter"] = rlist( Rpair(361353, "mc15_13TeV.361353.Sherpa_CT10_Wtaunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361354, "mc15_13TeV.361354.Sherpa_CT10_Wtaunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361355, "mc15_13TeV.361355.Sherpa_CT10_Wtaunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt140_280_BFilter"] = rlist( Rpair(361356, "mc15_13TeV.361356.Sherpa_CT10_Wtaunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361357, "mc15_13TeV.361357.Sherpa_CT10_Wtaunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361358, "mc15_13TeV.361358.Sherpa_CT10_Wtaunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt280_500_BFilter"] = rlist( Rpair(361359, "mc15_13TeV.361359.Sherpa_CT10_Wtaunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361360, "mc15_13TeV.361360.Sherpa_CT10_Wtaunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361361, "mc15_13TeV.361361.Sherpa_CT10_Wtaunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt500_700_BFilter"] = rlist( Rpair(361362, "mc15_13TeV.361362.Sherpa_CT10_Wtaunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361363, "mc15_13TeV.361363.Sherpa_CT10_Wtaunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361364, "mc15_13TeV.361364.Sherpa_CT10_Wtaunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt700_1000_BFilter"] = rlist( Rpair(361365, "mc15_13TeV.361365.Sherpa_CT10_Wtaunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361366, "mc15_13TeV.361366.Sherpa_CT10_Wtaunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361367, "mc15_13TeV.361367.Sherpa_CT10_Wtaunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt1000_2000_BFilter"] = rlist( Rpair(361368, "mc15_13TeV.361368.Sherpa_CT10_Wtaunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361369, "mc15_13TeV.361369.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361370, "mc15_13TeV.361370.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Wtaunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361371, "mc15_13TeV.361371.Sherpa_CT10_Wtaunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt0_70_CVetoBVeto"] = rlist( Rpair(361372, "mc15_13TeV.361372.Sherpa_CT10_Zee_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt0_70_CFilterBVeto"] = rlist( Rpair(361373, "mc15_13TeV.361373.Sherpa_CT10_Zee_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt0_70_BFilter"] = rlist( Rpair(361374, "mc15_13TeV.361374.Sherpa_CT10_Zee_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt70_140_CVetoBVeto"] = rlist( Rpair(361375, "mc15_13TeV.361375.Sherpa_CT10_Zee_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt70_140_CFilterBVeto"] = rlist( Rpair(361376, "mc15_13TeV.361376.Sherpa_CT10_Zee_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt70_140_BFilter"] = rlist( Rpair(361377, "mc15_13TeV.361377.Sherpa_CT10_Zee_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt140_280_CVetoBVeto"] = rlist( Rpair(361378, "mc15_13TeV.361378.Sherpa_CT10_Zee_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt140_280_CFilterBVeto"] = rlist( Rpair(361379, "mc15_13TeV.361379.Sherpa_CT10_Zee_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt140_280_BFilter"] = rlist( Rpair(361380, "mc15_13TeV.361380.Sherpa_CT10_Zee_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt280_500_CVetoBVeto"] = rlist( Rpair(361381, "mc15_13TeV.361381.Sherpa_CT10_Zee_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt280_500_CFilterBVeto"] = rlist( Rpair(361382, "mc15_13TeV.361382.Sherpa_CT10_Zee_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt280_500_BFilter"] = rlist( Rpair(361383, "mc15_13TeV.361383.Sherpa_CT10_Zee_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt500_700_CVetoBVeto"] = rlist( Rpair(361384, "mc15_13TeV.361384.Sherpa_CT10_Zee_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt500_700_CFilterBVeto"] = rlist( Rpair(361385, "mc15_13TeV.361385.Sherpa_CT10_Zee_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt500_700_BFilter"] = rlist( Rpair(361386, "mc15_13TeV.361386.Sherpa_CT10_Zee_Pt500_700_BFilter.merge.DAOD_SUSY7.e3741_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361387, "mc15_13TeV.361387.Sherpa_CT10_Zee_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361388, "mc15_13TeV.361388.Sherpa_CT10_Zee_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt700_1000_BFilter"] = rlist( Rpair(361389, "mc15_13TeV.361389.Sherpa_CT10_Zee_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361390, "mc15_13TeV.361390.Sherpa_CT10_Zee_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361391, "mc15_13TeV.361391.Sherpa_CT10_Zee_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt1000_2000_BFilter"] = rlist( Rpair(361392, "mc15_13TeV.361392.Sherpa_CT10_Zee_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361393, "mc15_13TeV.361393.Sherpa_CT10_Zee_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361394, "mc15_13TeV.361394.Sherpa_CT10_Zee_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zee_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361395, "mc15_13TeV.361395.Sherpa_CT10_Zee_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361396, "mc15_13TeV.361396.Sherpa_CT10_Zmumu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361397, "mc15_13TeV.361397.Sherpa_CT10_Zmumu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt0_70_BFilter"] = rlist( Rpair(361398, "mc15_13TeV.361398.Sherpa_CT10_Zmumu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361399, "mc15_13TeV.361399.Sherpa_CT10_Zmumu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt70_140_BFilter"] = rlist( Rpair(361401, "mc15_13TeV.361401.Sherpa_CT10_Zmumu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361402, "mc15_13TeV.361402.Sherpa_CT10_Zmumu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361403, "mc15_13TeV.361403.Sherpa_CT10_Zmumu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt140_280_BFilter"] = rlist( Rpair(361404, "mc15_13TeV.361404.Sherpa_CT10_Zmumu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361405, "mc15_13TeV.361405.Sherpa_CT10_Zmumu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361406, "mc15_13TeV.361406.Sherpa_CT10_Zmumu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e3741_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt280_500_BFilter"] = rlist( Rpair(361407, "mc15_13TeV.361407.Sherpa_CT10_Zmumu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361408, "mc15_13TeV.361408.Sherpa_CT10_Zmumu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361409, "mc15_13TeV.361409.Sherpa_CT10_Zmumu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt500_700_BFilter"] = rlist( Rpair(361410, "mc15_13TeV.361410.Sherpa_CT10_Zmumu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361411, "mc15_13TeV.361411.Sherpa_CT10_Zmumu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361412, "mc15_13TeV.361412.Sherpa_CT10_Zmumu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt700_1000_BFilter"] = rlist( Rpair(361413, "mc15_13TeV.361413.Sherpa_CT10_Zmumu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361414, "mc15_13TeV.361414.Sherpa_CT10_Zmumu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361415, "mc15_13TeV.361415.Sherpa_CT10_Zmumu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt1000_2000_BFilter"] = rlist( Rpair(361416, "mc15_13TeV.361416.Sherpa_CT10_Zmumu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361418, "mc15_13TeV.361418.Sherpa_CT10_Zmumu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Zmumu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361419, "mc15_13TeV.361419.Sherpa_CT10_Zmumu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt0_70_CFilterBVeto"] = rlist( Rpair(361421, "mc15_13TeV.361421.Sherpa_CT10_Ztautau_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt0_70_BFilter"] = rlist( Rpair(361422, "mc15_13TeV.361422.Sherpa_CT10_Ztautau_Pt0_70_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt70_140_CVetoBVeto"] = rlist( Rpair(361423, "mc15_13TeV.361423.Sherpa_CT10_Ztautau_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt70_140_CFilterBVeto"] = rlist( Rpair(361424, "mc15_13TeV.361424.Sherpa_CT10_Ztautau_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt70_140_BFilter"] = rlist( Rpair(361425, "mc15_13TeV.361425.Sherpa_CT10_Ztautau_Pt70_140_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt140_280_CVetoBVeto"] = rlist( Rpair(361426, "mc15_13TeV.361426.Sherpa_CT10_Ztautau_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt140_280_CFilterBVeto"] = rlist( Rpair(361427, "mc15_13TeV.361427.Sherpa_CT10_Ztautau_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt140_280_BFilter"] = rlist( Rpair(361428, "mc15_13TeV.361428.Sherpa_CT10_Ztautau_Pt140_280_BFilter.merge.DAOD_SUSY7.e3733_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt280_500_CVetoBVeto"] = rlist( Rpair(361429, "mc15_13TeV.361429.Sherpa_CT10_Ztautau_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt280_500_CFilterBVeto"] = rlist( Rpair(361430, "mc15_13TeV.361430.Sherpa_CT10_Ztautau_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt280_500_BFilter"] = rlist( Rpair(361431, "mc15_13TeV.361431.Sherpa_CT10_Ztautau_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt500_700_CVetoBVeto"] = rlist( Rpair(361432, "mc15_13TeV.361432.Sherpa_CT10_Ztautau_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt500_700_CFilterBVeto"] = rlist( Rpair(361433, "mc15_13TeV.361433.Sherpa_CT10_Ztautau_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt500_700_BFilter"] = rlist( Rpair(361434, "mc15_13TeV.361434.Sherpa_CT10_Ztautau_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361435, "mc15_13TeV.361435.Sherpa_CT10_Ztautau_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361436, "mc15_13TeV.361436.Sherpa_CT10_Ztautau_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e3741_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt700_1000_BFilter"] = rlist( Rpair(361437, "mc15_13TeV.361437.Sherpa_CT10_Ztautau_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361438, "mc15_13TeV.361438.Sherpa_CT10_Ztautau_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361439, "mc15_13TeV.361439.Sherpa_CT10_Ztautau_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt1000_2000_BFilter"] = rlist( Rpair(361440, "mc15_13TeV.361440.Sherpa_CT10_Ztautau_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361441, "mc15_13TeV.361441.Sherpa_CT10_Ztautau_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361442, "mc15_13TeV.361442.Sherpa_CT10_Ztautau_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Ztautau_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361443, "mc15_13TeV.361443.Sherpa_CT10_Ztautau_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt0_70_CVetoBVeto"] = rlist( Rpair(361444, "mc15_13TeV.361444.Sherpa_CT10_Znunu_Pt0_70_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2608_s2183_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt0_70_CFilterBVeto"] = rlist( Rpair(361445, "mc15_13TeV.361445.Sherpa_CT10_Znunu_Pt0_70_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt0_70_BFilter"] = rlist( Rpair(361446, "mc15_13TeV.361446.Sherpa_CT10_Znunu_Pt0_70_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt70_140_CVetoBVeto"] = rlist( Rpair(361447, "mc15_13TeV.361447.Sherpa_CT10_Znunu_Pt70_140_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt70_140_CFilterBVeto"] = rlist( Rpair(361448, "mc15_13TeV.361448.Sherpa_CT10_Znunu_Pt70_140_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt70_140_BFilter"] = rlist( Rpair(361449, "mc15_13TeV.361449.Sherpa_CT10_Znunu_Pt70_140_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt140_280_CVetoBVeto"] = rlist( Rpair(361450, "mc15_13TeV.361450.Sherpa_CT10_Znunu_Pt140_280_CVetoBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt140_280_CFilterBVeto"] = rlist( Rpair(361451, "mc15_13TeV.361451.Sherpa_CT10_Znunu_Pt140_280_CFilterBVeto.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt140_280_BFilter"] = rlist( Rpair(361452, "mc15_13TeV.361452.Sherpa_CT10_Znunu_Pt140_280_BFilter.merge.DAOD_SUSY7.e3651_s2586_s2174_r7267_r6282_p2452/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt280_500_CVetoBVeto"] = rlist( Rpair(361453, "mc15_13TeV.361453.Sherpa_CT10_Znunu_Pt280_500_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt280_500_CFilterBVeto"] = rlist( Rpair(361454, "mc15_13TeV.361454.Sherpa_CT10_Znunu_Pt280_500_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt280_500_BFilter"] = rlist( Rpair(361455, "mc15_13TeV.361455.Sherpa_CT10_Znunu_Pt280_500_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt500_700_CVetoBVeto"] = rlist( Rpair(361456, "mc15_13TeV.361456.Sherpa_CT10_Znunu_Pt500_700_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt500_700_CFilterBVeto"] = rlist( Rpair(361457, "mc15_13TeV.361457.Sherpa_CT10_Znunu_Pt500_700_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt500_700_BFilter"] = rlist( Rpair(361458, "mc15_13TeV.361458.Sherpa_CT10_Znunu_Pt500_700_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt700_1000_CVetoBVeto"] = rlist( Rpair(361459, "mc15_13TeV.361459.Sherpa_CT10_Znunu_Pt700_1000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt700_1000_CFilterBVeto"] = rlist( Rpair(361460, "mc15_13TeV.361460.Sherpa_CT10_Znunu_Pt700_1000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt700_1000_BFilter"] = rlist( Rpair(361461, "mc15_13TeV.361461.Sherpa_CT10_Znunu_Pt700_1000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt1000_2000_CVetoBVeto"] = rlist( Rpair(361462, "mc15_13TeV.361462.Sherpa_CT10_Znunu_Pt1000_2000_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt1000_2000_CFilterBVeto"] = rlist( Rpair(361463, "mc15_13TeV.361463.Sherpa_CT10_Znunu_Pt1000_2000_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt1000_2000_BFilter"] = rlist( Rpair(361464, "mc15_13TeV.361464.Sherpa_CT10_Znunu_Pt1000_2000_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt2000_E_CMS_CVetoBVeto"] = rlist( Rpair(361465, "mc15_13TeV.361465.Sherpa_CT10_Znunu_Pt2000_E_CMS_CVetoBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt2000_E_CMS_CFilterBVeto"] = rlist( Rpair(361466, "mc15_13TeV.361466.Sherpa_CT10_Znunu_Pt2000_E_CMS_CFilterBVeto.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    m_map["mc15b_Sherpa_CT10_Znunu_Pt2000_E_CMS_BFilter"] = rlist( Rpair(361467, "mc15_13TeV.361467.Sherpa_CT10_Znunu_Pt2000_E_CMS_BFilter.merge.DAOD_SUSY7.e4133_s2608_s2183_r7326_r6282_p2470/"));
    
    m_map["mc15b_PowhegPythiaEvtGen_P2012CT10_ttbarHT1k_1k5_hdamp172p5_nonAH"] = rlist( Rpair(407010, "mc15_13TeV.407010.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k_1k5_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r7326_r6282_p2470/"));
    m_map["mc15b_PowhegPythiaEvtGen_P2012CT10_ttbarHT1k5_hdamp172p5_nonAH"] = rlist( Rpair(407011, "mc15_13TeV.407011.PowhegPythiaEvtGen_P2012CT10_ttbarHT1k5_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r7326_r6282_p2470/"));

    m_map["mc15b_PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad"] = rlist( Rpair(410000, "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_SUSY7.e3698_s2608_s2183_r6765_r6282_p2452"));

    m_map["mc15b_PowhegPythiaEvtGen_P2012CT10_ttbarHT6c_1k_hdamp172p5_nonAH"] = rlist( Rpair(407009, "mc15_13TeV.407009.PowhegPythiaEvtGen_P2012CT10_ttbarHT6c_1k_hdamp172p5_nonAH.merge.DAOD_SUSY7.e4023_s2608_r7326_r6282_p2470"));
    m_map["mc15b_PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad"] = rlist( Rpair(410001, "mc15_13TeV.410001.PowhegPythiaEvtGen_P2012radHi_ttbar_hdamp345_down_nonallhad.merge.DAOD_SUSY7.e3783_s2608_r7326_r6282_p2470"));
    m_map["mc15b_PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad"] = rlist( Rpair(410002, "mc15_13TeV.410002.PowhegPythiaEvtGen_P2012radLo_ttbar_hdamp172_up_nonallhad.merge.DAOD_SUSY7.e3783_s2608_r7326_r6282_p2470"));
    m_map["mc15b_aMcAtNloHerwigppEvtGen_ttbar_nonallhad"] = rlist( Rpair(410003, "mc15_13TeV.410003.aMcAtNloHerwigppEvtGen_ttbar_nonallhad.merge.DAOD_SUSY7.e3964_a766_a810_r6282_p2470"));
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
