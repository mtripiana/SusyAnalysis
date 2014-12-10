#!/bin/bash
## arelycg@cern.ch
## IFAE 18.nov.14
## ./dq2get.sh TAG1,TAG2,TAG3,... [output folder] [username] [merge: 0, 1]

: ${ANALYSISROOTFILES:?" You need to set ANALYSISCODE before !! Please do so and try again..."}

TAG=''
DIRECTORY=$ANALYSISROOTFILES
GRIDUSER=$USER
MERGE=0
JOBOPT="METbb"

#------------------------------------
## Make sure we have grid-stuff up and the proxy available
source $ANALYSISCODE"/SusyAnalysis/scripts/grid_up.sh"
#------------------------------------

#--- Check certificate
timeLeft=$(voms-proxy-info | grep 'timeleft' | awk {'print$3'})
if [ "${timeLeft}" = '' ];
then
   source $ANALYSISCODE"/SusyAnalysis/scripts/grid_up.sh"
elif [ "${timeLeft}" = '0:00:00' ];
then
   voms-proxy-init -voms atlas
   clear
else
   echo
fi

#--- Read input info
if [[ $1 != "" ]]; then TAG=$1;fi
if [[ $2 != "" ]]; then DIRECTORY=$2;fi
if [[ $3 != "" ]]; then GRIDUSER=$3;fi
if [[ $4 != "" ]]; then MERGE=$4;fi

TAGLIST="${TAG//,/ }"

#--- Input info
echo "----------------------------------------------------------------------------"
echo "   Looking for samples from user: "$GRIDUSER"."
echo "----------------------------------------------------------------------------"

#--- DQ2-GET
cd $DIRECTORY
TAGLIST="${TAG//,/ }"
for tag in $TAGLIST ;
do
   echo "----------------------------------------------------------------------------"
   echo "   TAG: "$tag
   echo "----------------------------------------------------------------------------"

   echo "   Doing: dq2-ls user."$GRIDUSER".*"$tag"*_output.root/ | grep -v \"hist-output\" > tmp_dq2ls.txt"
   dq2-ls "user."$GRIDUSER".*"$tag"*_output.root/" | grep -v hist-output  > tmp_dq2ls.txt
   
   echo " " >> tmp_dq2ls.txt

   cat tmp_dq2ls.txt
   echo " "

   for s in $(cat tmp_dq2ls.txt);  
   do 
      echo "   Downloading: "$s
      sampleName=`echo $s | cut -d':' -f 2 | cut -d'_' -f 1`
      folderName="output_"$sampleName"_v"$tag
      mkdir $folderName
      dq2-get -H $folderName $s
      if [ $MERGE == 1 ] 
      then
         DSid=`echo $sampleName | cut -d'.' -f 4 | cut -d'_' -f 1`
	 echo " "
         echo "   Proceed to merge files:"
	 echo "   run_weights_grid "$DSid" "$DIRECTORY"/"$folderName" -j="$JOBOPT
	 run_weights_grid "$DSid" "$DIRECTORY"/"$folderName"/ -j="$JOBOPT"
      fi
      echo "----------------------------------------------------------------------------"
   done
   rm tmp_dq2ls.txt
   echo " "
done
cd -

echo " "
echo "... Sayonara!"





