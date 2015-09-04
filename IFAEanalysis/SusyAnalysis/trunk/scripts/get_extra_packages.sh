#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${$ROOTCOREBIN:?" You need to set an analysis release first! (check 'rcSetup -r' for available options) Please do so and get back..."}

cd $ROOTCOREBIN/..

# Extra tags needed for SUSY,2.3.24a
rc checkout $ROOTCOREBIN/../SusyAnalysis/scripts/packages.txt 

## patch SUSYTools to allow for FlatBEff WPs while we wait for their calibrations
patch -p0 -i SusyAnalysis/patches/patch_SUSYTools_06-23-03.diff

