QUEUE='at3'
SYSTEMATIC='Nom'
MYJOP='METbb'
MYSAMPLE=''

QSUBDIRECTORY=$ANALYSISCODE'/SusyAnalysis/scripts/qsubFiles'

#DATALIST=""

#BKGLIST=""

#QCDLIST=""

#BKGLISTGEN=""

#SIGNALLIST="MadGraphPythia_SM_TT_directCC_200_125 MadGraphPythia_SM_TT_directCC_200_195"

TESTLIST="TestMClocal5"

#------------------------------------
## Make sure we have grid-stuff up and the proxy available
source $ANALYSISCODE"/SusyAnalysis/scripts/grid_up.sh"
source $ANALYSISCODE"/SusyAnalysis/scripts/export_proxy.sh"
#------------------------------------

SAMPLELIST=$TESTLIST 

SYSTLIST="
Nom \
"

if [[ $1 != "" ]]; then QUEUE=$1;fi
if [[ $2 != "" ]]; then SYSTEMATIC=$2;fi
if [[ $3 != "" ]]; then MYJOP=$3; fi
if [[ $4 != "" ]]; then SAMPLELIST=$4; DATALIST="";fi

echo
echo "Samples -------------------------------" 
echo $SAMPLELIST
echo $DATALIST


if [ "$SYSTEMATIC" == "ALL" ]
then
    if [ -f $ANALYSISCODE"/SusyAnalysis/ToRunSusy.txt" ]
    then
        rm $ANALYSISCODE"/code/ToRunSusy.txt*"
    fi

    for syst in $SYSTLIST
    do

        echo "Sendig systematic: "$syst

        if [ "$syst" == "GENERATOR" ]; then
            for sample in $BKGLISTGEN
            do
                f=$QSUBDIRECTORY/$sample
                echo "qsub -q $QUEUE -v MYSYS=Nom,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f.sub " >> $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt"
            done
        else
            if [ "$syst" == "Nom" ]; then
                for sample in $DATALIST
                do
                    f=$QSUBDIRECTORY/$sample
                    echo "qsub -q $QUEUE -v MYSYS=$syst,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f.sub " >> $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt"
                done
            fi
            for sample in $SAMPLELIST
            do 
                f=$QSUBDIRECTORY/$sample
                isequal=0
                for esample in $EXCEPTIONS
                do
                    if [ $sample == $esample ]
                    then
                        isequal=1
                    fi
                done
                if [ $isequal == 0 ]
                then
                    echo "qsub -q $QUEUE -v MYSYS=$syst,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f.sub " >> $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt"
                else
                    echo "qsub -q at3_xxl -v MYSYS=$syst,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f.sub " >> $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt"
                fi
            done
        fi
    done
    split -d -l910 $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt" $ANALYSISSUSYANALYSIS"/SusyAnalysis/ToRunSusy.txt"
else
    echo "Running only in "$SYSTEMATIC
    for sample in $SAMPLELIST
    do 
        f=$QSUBDIRECTORY/$sample
        isequal=0
        for esample in $EXCEPTIONS
        do
            if [ $sample == $esample ]
            then
                isequal=1
            fi
        done
        if [ $isequal == 0 ]
        then
            qsub -q $QUEUE -v MYSYS=$SYSTEMATIC,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f".sub" 
        else
            qsub -q at3_xxl -v MYSYS=$SYSTEMATIC,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f".sub" 
        fi
    done
    if [ "$SYSTEMATIC" == "Nom" ]; then
        for sample in $DATALIST
        do
            f=$QSUBDIRECTORY/$sample
            qsub -q at3 -v MYSYS=$SYSTEMATIC,MYJO=$MYJOP -o $ANALYSISROOTFILES/OutputLogFiles/ -e $ANALYSISROOTFILES/OutputLogFiles/ $f".sub"
        done
    fi
fi
