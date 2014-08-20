: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and try again..."}

: ${ROOTCOREBIN:?" You need to set an AnalysisRelease before (rcSetup)!! Please do so and try again..."}

cd $ANALYSISCODE 

echo 'Loading...'
echo 

tmpFile='tmp.tmpKj1234'

run_chorizo slist > $tmpFile

grep 'List of recommended' $tmpFile
sed -n '/~~~~~~~~~~~~~~~~~~~~~/,/~~~~~~~~~~~~~~~~/p' $tmpFile

rm $tmpFile