#!/bin/bash

uid=`id -u $USER`
export X509_USER_PROXY=/nfs/at3/scratch/$USER/x509up_u$uid

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
localSetupDQ2Client --skipConfirm

export DQ2_LOCAL_SITE_ID=ROAMING

voms-proxy-init -voms atlas 

localSetupPyAMI
localSetupPandaClient --noAthenaCheck

