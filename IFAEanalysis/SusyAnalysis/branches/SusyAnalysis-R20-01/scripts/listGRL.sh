#!/bin/sh

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Colors
mag='\e[35m'
green='\e[32m'
def='\e[39m'

#list type ("All_Good"(default), "Atlas_Ready", "CombinedPerf", ...)
LTYPE="All_Good"

# options
docopy=0
doview=0

function show_help() {
echo
echo
printf "${mag} -- Usage :     ./listGRL [options] <Type> "
echo
echo
printf "   options:\n"
printf "              -v to print latest GRL\n"
printf "              -c to copy latest GRL over"
echo
echo
printf "${mag} -- GRL types available : \n\n"
ls -1 -d /afs/cern.ch/user/a/atlasdqm/grlgen/*/ | while read i; do basename $i; done 
echo 
}


while getopts "h?vc" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    v)  doview=1
        ;;
    c)  docopy=1
        ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift


if [ ! -z $1 ]; then
    LTYPE=$1
else
    echo
    echo
    show_help
fi

echo
echo
if [ $doview -eq 1 ]; then
    printf " -- Showing " 
    printf "${green}"
    ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1
    echo 
    printf "${def}\n"
    cat  /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/`ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1`
    echo 
    exit 0
fi

if [ $docopy -eq 1 ]; then
    printf " -- Getting " 
    printf "${green}"
    ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1
    echo 
    printf "${def}\n"
    cp /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/`ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1` .
    printf " done!\n\n" 
    exit 0
fi

printf "${green}"
printf " -- Checking content of : /afs/cern.ch/user/a/atlasdqm/grlgen/${LTYPE}/   ${def}"
echo
echo
ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | head -n -1
printf "${green}"
ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1
printf "${def}"
echo
