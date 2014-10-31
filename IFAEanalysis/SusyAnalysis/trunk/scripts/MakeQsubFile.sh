#!/bin/sh

# use : source MakeQsubFiles.sh <sampleName>
#
#       <sampleName> is the name appearing in RunsMap.h (ex : AlpGen_Wtaunu...)

export name=$1
shift 1

WDIR=`readlink -f $ANALYSISCODE`                                                                                                                                                                
rm -rf $WDIR/SusyAnalysis/scripts/qsubFiles/${name}.sub
cat >> $WDIR/SusyAnalysis/scripts/qsubFiles/${name}.sub << EOF

if [ -z "\$1" ]
    then

    if [ -z "\$MYSYS" ]
       then
        syst="Nom"
        echo "No systematic given! (setting to Nominal)"
    else
        syst=\$MYSYS
        echo "Systematic : "\$MYSYS
    fi

else
    syst=\$1
    echo "SYS argument passed: "\$1
fi


if [ -z "\$2" ]
    then

    if [ -z "\$MYJO" ]
       then
        syst="METbb"
        echo "Running (default) METbb jopOption"
    else
        joption=\$MYJO
        echo "Running (default) \$MYJO jopOption"
    fi

else
    joption=\$2
    echo "jopOption picked: "\$2
fi



MYTMPDIR=`mktemp -d`
cd \$MYTMPDIR

\$ANALYSISCODE/SusyAnalysis/scripts/RunCode.sh ${name} \$syst \$joption

cd \$ANALYSISCODE
rm -rf \$MYTMPDIR

EOF

chmod +x ${WDIR}/SusyAnalysis/scripts/qsubFiles/${name}.sub


