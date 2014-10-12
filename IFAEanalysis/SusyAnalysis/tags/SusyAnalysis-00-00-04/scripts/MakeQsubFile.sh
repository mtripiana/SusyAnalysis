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
    if [ -z "\$SYS" ]
       then
        syst="Nom"
        echo "No systematic given! (setting to Nominal)"
    else
        syst=\$SYS
        echo "Systematic : "\$SYS
    fi
else
    syst=\$1
    echo "SYS argument passed: "\$1
fi

MYTMPDIR=`mktemp -d`
cd \$MYTMPDIR

\$ANALYSISCODE/SusyAnalysis/scripts/RunCode.sh ${name} \$syst

cd \$ANALYSISCODE
rm -rf \$MYTMPDIR

EOF

chmod +x ${WDIR}/SusyAnalysis/scripts/qsubFiles/${name}.sub


