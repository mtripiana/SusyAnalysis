#!/bin/bash

syst="Nom"
if [[ $2 != "" ]]; then syst=$2;fi

jop="METbb"
if [[ $3 != "" ]]; then jop=$3;fi

WHEREFROM=$PWD
 
echo "CURRENT = "$WHEREFROM 
echo "ANALYSISCODE = "$ANALYSISCODE
echo "ROOTCOREBIN = "$ROOTCOREBIN
echo "ROOTCOREDIR = "$ROOTCOREDIR

if [ -z "$ROOTCOREDIR" ]; then
    #this will certainly be the case when running in the batch 
    echo "===============  Setup env  ================"
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

    #setup RootCore from your workarea
    source $ANALYSISCODE/RootCoreBin/local_setup.sh

    echo "new ROOTCOREBIN = "$ROOTCOREBIN
    echo "new ROOTCOREDIR = "$ROOTCOREDIR

    #no need to re-compile (save time) . But make sure you do so before submission...
    #    $ROOTCOREDIR/scripts/find_packages.sh
    #    $ROOTCOREDIR/scripts/compile.sh

    if [ -z "$LHAPDF_DATA_PATH" ]; then
	export LHAPDF_DATA_PATH=$ROOTCOREBIN/data/Asg_Lhapdf_LHAPDF:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:
    fi
fi  

echo ""
export ROOTENV_NO_HOME=1  #--- To be able to read correctly the TEnv class in ROOT.

echo "===============  Run the code  ================"

source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh
run -l -j=$jop -s=$syst $1


