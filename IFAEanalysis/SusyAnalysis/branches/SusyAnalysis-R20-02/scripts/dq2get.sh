#!/bin/bash
## arelycg@cern.ch
## IFAE 18.nov.14
##
## @modified by Martin (tripiana@cern.ch)  June 2015
##
## ./dq2get.sh TAG1,TAG2,TAG3,... [output folder] [username] [merge: 0, 1] <PATTERN1,PATTERN2,...> <outputName>

: ${ANALYSISROOTFILES:?" You need to set ANALYSISROOTFILES before !! Please do so and try again..."}

TAG=''
DIRECTORY=$ANALYSISROOTFILES
GRIDUSER=$USER
MERGE=0

PATTERN=''
OFILE=''

optOFILE=''

#------------------------------------
## Make sure we have grid-stuff up and the proxy available
source $ROOTCOREBIN"/../SusyAnalysis/scripts/grid_up.sh"
#------------------------------------

#--- Check certificate
timeLeft=$(voms-proxy-info | grep 'timeleft' | awk {'print$3'})
if [ "${timeLeft}" == '' ];
then
   source $ROOTCOREBIN"/../SusyAnalysis/scripts/grid_up.sh"
elif [ "${timeLeft}" == '0:00:00' ];
then
   voms-proxy-init -voms atlas
   clear
else
   echo
fi

INITDIR=$PWD

#--- Read input info
if [[ $1 != "" ]]; then TAG=$1;fi
if [[ $2 != "" ]]; then DIRECTORY=$2;fi
if [[ $3 != "" ]]; then GRIDUSER=$3;fi
if [[ $4 != "" ]]; then MERGE=$4;fi

if [[ $5 != "" ]]; 
then 
    PATTERN=$5
else
    PATTERN=""
fi

if [[ $6 != "" ]]; then 
    OFILE=$6;
    optOFILE=" -m="$6
fi

TAGLIST="${TAG//,/ }"
PATLIST="${PATTERN//,/ }"


#--- Input info
echo "----------------------------------------------------------------------------"
echo "   Looking for samples from user: "$GRIDUSER"."
echo "----------------------------------------------------------------------------"

#--- DQ2-GET
cd $DIRECTORY
for tag in $TAGLIST ;
do
    
    echo "----------------------------------------------------------------------------"
    echo "   TAG: "$tag
    echo "----------------------------------------------------------------------------"
    
    if [ -z "$VAR" ]; then

	echo "   Doing: dq2-ls user."$GRIDUSER".*"$tag"*_output.root/ > tmp_dq2ls.txt"
	dq2-ls "user."$GRIDUSER".*"$tag"*_output.root/" >> tmp_dq2ls.txt
	
	echo " " >> tmp_dq2ls.txt
    else
	for pat in $PATLIST ;
	do
	    
	    
	    echo "   Doing: dq2-ls user."$GRIDUSER".*"$pat"*"$tag"*_output.root/ > tmp_dq2ls.txt"
	    dq2-ls "user."$GRIDUSER".*"$pat"*"$tag"*_output.root/" >> tmp_dq2ls.txt
	    
	    echo " " >> tmp_dq2ls.txt
	done
    fi

    cat tmp_dq2ls.txt
    echo " "
    
    for s in $(cat tmp_dq2ls.txt | grep -v "hist-") ;  
    do 
	#create output Folder
	sampleName=`echo $s | cut -d':' -f 2 | cut -d'_' -f 1`
	folderName="output_"$sampleName"_v"$tag
	mkdir -p $folderName

        #create temporal directory
        #    tmpDir=`mktemp -d`
	tmpDir=$DIRECTORY"/"$sampleName".tmp/"
	mkdir -p $tmpDir
	
	echo "   Downloading: "$s
	#get all samples from the grid
	#dq2-get -H $folderName $s
	#dq2-get -H $folderName ${s/_output/_hist-output}
	dq2-get -H $tmpDir $s
	dq2-get -H $tmpDir ${s/_output/_hist-output}
	
	if [ $MERGE == 1 ] 
	then
	    DSid=`echo $sampleName | cut -d'.' -f 4 | cut -d'_' -f 1`
	    echo " "
	    echo "   Proceed to merge files:"
	    echo "   run_weights_grid  -i="$tmpDir" -o="$DIRECTORY"/"$folderName"/  "$optOFILE" -n="$DSid
	    run_weights_grid "$tmpDir" -o="$DIRECTORY"/"$folderName"/  $optOFILE -n=$DSid
	    rm -r $tmpDir
	fi
	echo "----------------------------------------------------------------------------"

    done
    rm tmp_dq2ls.txt

    echo " "
done

cd $INITDIR
    
echo " "
echo "... Sayonara!"





