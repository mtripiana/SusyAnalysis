#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${$ROOTCOREBIN:?" You need to set an analysis release first! (check 'rcSetup -r' for available options) Please do so and get back..."}

cd $ROOTCOREBIN/..

# Extra tags needed for Base,2.3.16
rc checkout $ROOTCOREBIN/../SusyAnalysis/scripts/packages.txt 


