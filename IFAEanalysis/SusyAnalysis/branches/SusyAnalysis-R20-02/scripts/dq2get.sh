#!/bin/bash
## arelycg@cern.ch
## IFAE 18.nov.14
##
## @modified by Martin (tripiana@cern.ch)  June 2015
## @modified by Martin (tripiana@cern.ch)  Nov 2015     -  Moved to RUCIO 
##
## ./dq2get.sh TAG1,TAG2,TAG3,... [output folder] [username] [merge: 0, 1] <PATTERN1,PATTERN2,...> <outputName>

: ${ANALYSISROOTFILES:?" You need to set ANALYSISROOTFILES before !! Please do so and try again..."}

TAG=''
DIRECTORY=$ANALYSISROOTFILES
GRIDUSER=$USER
MERGE=0
KEEPFILES=0

PATTERN=''
OFILE=''

optOFILE=''

#------------------------------------
## Make sure we have grid-stuff up and the proxy available
source $ROOTCOREBIN"/../SusyAnalysis/scripts/grid_up_pwin.sh"
#------------------------------------

#--- Check certificate
timeLeft=$(voms-proxy-info | grep 'timeleft' | awk {'print$3'})
if [ "${timeLeft}" == '' ];
then
   source $ROOTCOREBIN"/../SusyAnalysis/scripts/grid_up_pwin.sh"
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

#--- RUCIO-GET
cd $DIRECTORY
for tag in $TAGLIST ;
do
    
    echo "----------------------------------------------------------------------------"
    echo "   TAG: "$tag
    echo "----------------------------------------------------------------------------"
    
    if [ -z "$VAR" ]; then

	echo "   Doing: rucio list-dids user."$GRIDUSER".*"$tag"*_output.root/ > tmp_ruciols.txt"
	rucio list-dids --short "user."$GRIDUSER".*"$tag"*_output.root/" --filter type=container >> tmp_ruciols.txt
	
	echo " " >> tmp_ruciols.txt
    else
	for pat in $PATLIST ;
	do
	    
	    
	    echo "   Doing: rucio list-dids user."$GRIDUSER".*"$pat"*"$tag"*_output.root/ > tmp_ruciols.txt"
	    rucio list-dids --short "user."$GRIDUSER".*"$pat"*"$tag"*_output.root/" --filter type=container >> tmp_ruciols.txt
	    
	    echo " " >> tmp_ruciols.txt
	done
    fi

    cat tmp_ruciols.txt
    echo " "
    
    for s in $(cat tmp_ruciols.txt | grep -v "hist") ;  
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
	rucio download --dir $tmpDir $s
	rucio download --dir $tmpDir ${s/_output.root/_hist}
	
	#Fix for RUCIO
	ls $tmpDir >> tmp_rucio_dwn.txt
	for d in $(cat tmp_rucio_dwn.txt) ;
	do
	    #move the files
	    mv $tmpDir/$d/* $tmpDir/.
	    rm -r $tmpDir/$d
	done
	rm tmp_rucio_dwn.txt    

	if [ $MERGE == 1 ] 
	then
	    DSid=`echo $sampleName | cut -d'.' -f 4 | cut -d'_' -f 1`
	    echo " "
	    echo "   Proceed to merge files:"
	    echo "   run_weights_grid  -i="$tmpDir" -o="$DIRECTORY"/"$folderName"/  "$optOFILE" -n="$DSid
	    run_weights_grid "$tmpDir" -o="$DIRECTORY"/"$folderName"/  $optOFILE -n=$DSid
	    #copy the files before removing
            #cp $tmpDir/* $DIRECTORY/$folderName/.
	    rm -r $tmpDir
	fi
	echo "----------------------------------------------------------------------------"

    done
    rm tmp_ruciols.txt

    echo " "
done

cd $INITDIR
    
echo " "
echo "... Sayonara!"
