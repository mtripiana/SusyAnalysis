#!/bin/bash

syst="Nom"
if [[ $2 != "" ]]; then syst=$2;fi

WHEREFROM=$PWD
 
echo "CURRENT = "$WHEREFROM 
echo "ANALYSISCODE = "$ANALYSISCODE
echo "ROOTCOREDIR = "$ROOTCOREDIR

if [ -z "$ROOTCOREDIR" ]; then
    #this will certainly be the case when running in the batch 
    echo "===============  Setup env  ================"
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    cd $ANALYSISCODE

    RELSET=`cat RootCoreBin/rootcore_config | grep release | grep -oP '(?<=Analysis).*?(?=/Root)' | tr '/' ','`
    source $ANALYSISCODE/rcSetup.sh $RELSET

    echo "ROOTCOREDIR = "$ROOTCOREDIR

    $ROOTCOREDIR/scripts/find_packages.sh
    $ROOTCOREDIR/scripts/compile.sh

    echo " --> back to: "$WHEREFROM
    cd $WHEREFROM
fi  

echo ""
export ROOTENV_NO_HOME=1  #--- To be able to read correctly the TEnv class in ROOT.

echo "===============  Run the code  ================"

source $ANALYSISCODE/SusyAnalysis/scripts/grid_up.sh
run -l $1 $syst


