import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

import os, subprocess, sys, commands, math, glob
from ROOT import *
from array import array
from optparse import OptionParser, OptionGroup

def loopOnFiles(directory, trueStr):
    listOfFiles = glob.glob(directory+trueStr)
    return listOfFiles
            
def checkSystematics(listOfFiles, systeList, showFound, checkEvents):
    for s in systeList:
        notFound = []
        fileList = []
        found = False
        for l in listOfFiles:
            if s in l:
                found = True
                fileList.append(l)
            else:
	        found = False
		notFound.append(l)

        if showFound or checkEvents:
	   if len(fileList)>0:
              for l in fileList:
                  if showFound:
		      print '--- Systematic %s found for: %s' % (s, l)
		  if checkEvents:
		      chain = TChain('AnalysisTree')
		      chain.Add(l)
		      nEvts = chain.GetEntries()
		      nSim=1
		      for event in chain:
		         #print event.nsim
			 nSim = event.nsim
			 break
		      if ( abs(nSim - nEvts)/nSim > 0.01 ):
		         print '--- Check-Events failed for syst. %s for: %s' % (s, l)
			 print '    Expected number of events: %d, found: %d' % (nSim, nEvts)
		      
        if len(notFound)>0:
           print '--- Not found files for systematic %s:' % (s)
	   for l in notFound:
               print '    Systematic not found for: %s' % (l)

if __name__=="__main__":


    usage = "usage: %prog [options]"
    parser = OptionParser(usage=usage)

    parser.add_option("-v","--verbose", action="store_true", dest="doVerbose", default=False, help="Set the program in verbose mode")
    parser.add_option("--directory", type="string", action="store", dest="directory", default=os.getenv('ANALYSISROOTFILES'), help="Set the directory where to look for")
    #parser.add_option("--systematics", type="string", action="store", dest="systematics", default="Nom,JESHigh,JESLow,JER,EGZEEHigh,EGZEELow,EGMATHigh,EGMATLow,EGPSHigh,EGPSLow,EGLOWHigh,EGLOWLow,EGRESHigh,EGRESLow,EEFFHigh,EEFFLow,MMSHigh,MMSLow,MIDHigh,MIDLow,MEFFHigh,MEFFLow,MSCALEHigh,MSCALELow,SCALESTHigh,SCALESTLow,RESOST,PileupHigh,PileupLow,JvfUncHigh,JvfUncLow,ktfacHigh,ktfacLow,qfacHigh,qfacLow,BCHCorrHigh,BCHCorrLow", help="Name of the systematics separated by commas")
    parser.add_option("--systematics", type="string", action="store", dest="systematics", default="Nom", help="Name of the systematics separated by commas")
    parser.add_option('--showFound', action="store_true", default=False, help="Show the found files.")
    parser.add_option('--checkEvents', action="store_true", default=False, help="Check the total number of events in found files.")
    
    (options, args) = parser.parse_args()

    
    directory   = options.directory
    systematics = options.systematics.split(',')
    showFound   = options.showFound
    checkEvents   = options.checkEvents
    
    trueStr = ['Sherpa_CT10_WenuMassiveCBPt0_BFilter', \
               'Sherpa_CT10_WenuMassiveCBPt0_CJetFilterBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt0_CJetVetoBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_WenuMassiveCBPt140_280_CJetFilterBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt140_280_CJetVetoBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_WenuMassiveCBPt280_500_CJetFilterBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt280_500_CJetVetoBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt500_BFilter', \
               'Sherpa_CT10_WenuMassiveCBPt500_CJetFilterBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt500_CJetVetoBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_WenuMassiveCBPt70_140_CJetFilterBVeto', \
               'Sherpa_CT10_WenuMassiveCBPt70_140_CJetVetoBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt0_BFilter', \
               'Sherpa_CT10_WmunuMassiveCBPt0_CJetFilterBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt0_CJetVetoBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_WmunuMassiveCBPt140_280_CJetFilterBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt140_280_CJetVetoBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_WmunuMassiveCBPt280_500_CJetFilterBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt280_500_CJetVetoBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt500_BFilter', \
               'Sherpa_CT10_WmunuMassiveCBPt500_CJetFilterBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt500_CJetVetoBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_WmunuMassiveCBPt70_140_CJetFilterBVeto', \
               'Sherpa_CT10_WmunuMassiveCBPt70_140_CJetVetoBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt0_BFilter', \
               'Sherpa_CT10_WtaunuMassiveCBPt0_CJetFilterBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt0_CJetVetoBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetFilterBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt140_280_CJetVetoBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetFilterBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt280_500_CJetVetoBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt500_BFilter', \
               'Sherpa_CT10_WtaunuMassiveCBPt500_CJetFilterBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt500_CJetVetoBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetFilterBVeto', \
               'Sherpa_CT10_WtaunuMassiveCBPt70_140_CJetVetoBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt0_BFilter', \
               'Sherpa_CT10_ZeeMassiveCBPt0_CFilterBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt0_CVetoBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_ZeeMassiveCBPt140_280_CFilterBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt140_280_CVetoBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_ZeeMassiveCBPt280_500_CFilterBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt280_500_CVetoBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt500_BFilter', \
               'Sherpa_CT10_ZeeMassiveCBPt500_CFilterBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt500_CVetoBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_ZeeMassiveCBPt70_140_CFilterBVeto', \
               'Sherpa_CT10_ZeeMassiveCBPt70_140_CVetoBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt0_BFilter', \
               'Sherpa_CT10_ZmumuMassiveCBPt0_CFilterBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt0_CVetoBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_ZmumuMassiveCBPt140_280_CFilterBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt140_280_CVetoBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_ZmumuMassiveCBPt280_500_CFilterBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt280_500_CVetoBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt500_BFilter', \
               'Sherpa_CT10_ZmumuMassiveCBPt500_CFilterBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt500_CVetoBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_ZmumuMassiveCBPt70_140_CFilterBVeto', \
               'Sherpa_CT10_ZmumuMassiveCBPt70_140_CVetoBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt0_BFilter', \
               'Sherpa_CT10_ZnunuMassiveCBPt0_CFilterBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt0_CVetoBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_ZnunuMassiveCBPt140_280_CFilterBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt140_280_CVetoBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_ZnunuMassiveCBPt280_500_CFilterBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt280_500_CVetoBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt500_BFilter', \
               'Sherpa_CT10_ZnunuMassiveCBPt500_CFilterBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt500_CVetoBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_ZnunuMassiveCBPt70_140_CFilterBVeto', \
               'Sherpa_CT10_ZnunuMassiveCBPt70_140_CVetoBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt0_BFilter', \
               'Sherpa_CT10_ZtautauMassiveCBPt0_CFilterBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt0_CVetoBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt140_280_BFilter', \
               'Sherpa_CT10_ZtautauMassiveCBPt140_280_CFilterBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt140_280_CVetoBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt280_500_BFilter', \
               'Sherpa_CT10_ZtautauMassiveCBPt280_500_CFilterBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt280_500_CVetoBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt500_BFilter', \
               'Sherpa_CT10_ZtautauMassiveCBPt500_CFilterBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt500_CVetoBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt70_140_BFilter', \
               'Sherpa_CT10_ZtautauMassiveCBPt70_140_CFilterBVeto', \
               'Sherpa_CT10_ZtautauMassiveCBPt70_140_CVetoBVeto', \
               'PowhegPythia_P2012_ttbar_nonallhad', \
               'PowhegPythia_P2012_singletop_tchan_lept_top', \
	       'PowhegPythia_P2012_singletop_tchan_lept_antitop', \
               'PowhegPythia_P2012_st_Wtchan_incl_DR', \
               'PowhegPythia_P2012_st_schan_lep', \
               'MadGraphPythia_AUET2BCTEQ6L1_SM_TT_directCC_300_290_MET80', \
	       'MadGraphPythia_AUET2BCTEQ6L1_SM_TT_directCC_300_240_MET80', \
               'MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_300_290_MET50', \
               'MadGraphPythia_AUET2BCTEQ6L1_SM_BB_direct_800_1_MET50', \
              ]

    for t in trueStr:
        listOfFiles = loopOnFiles(directory=directory, trueStr='*'+t+'_tree.root')
	if len(listOfFiles) == 0:
	   print '*** No matches for: %s' % (t)
	checkSystematics(listOfFiles=listOfFiles, systeList=systematics, showFound=showFound, checkEvents=checkEvents)
