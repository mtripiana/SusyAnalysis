
mag='\e[35m'
green='\e[32m'
def='\e[39m'

#list type ("All_Good"(default), "Atlas_Ready", "CombinedPerf", ...)
LTYPE="All_Good"

if [ ! -z $1 ]; then
    LTYPE=$1
fi

echo
echo
printf "${mag} -- Usage :     ./listGRL <Type> "
echo
echo
printf "${mag} -- GRL types available : \n\n"
ls -1 -d /afs/cern.ch/user/a/atlasdqm/grlgen/*/ | while read i; do basename $i; done 
echo 
echo
printf " -- Checking content of : /afs/cern.ch/user/a/atlasdqm/grlgen/${LTYPE}/   ${def}"
echo
echo
ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | head -n -1
printf "${green}"
ls -rt1 /afs/cern.ch/user/a/atlasdqm/grlgen/$LTYPE/ | tail -1
printf "${def}"
echo
