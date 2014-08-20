
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
localSetupDQ2Client --skipConfirm

voms-proxy-init -voms atlas
export DQ2_LOCAL_SITE_ID=ROAMING

#localSetupPandaClient --noAthenaCheck
localSetupPyAMI
