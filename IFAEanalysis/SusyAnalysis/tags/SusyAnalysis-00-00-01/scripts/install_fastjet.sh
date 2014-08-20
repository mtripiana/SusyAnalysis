##To INSTALL FASTJET YOU JUST DO 
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

#get the pkg
curl -O http://fastjet.fr/repo/fastjet-3.0.6.tar.gz 
tar zxvf fastjet-3.0.6.tar.gz
cd fastjet-3.0.6/

./configure --prefix=$PWD/../fastjet-install
make 
make check  
make install	
cd ..		



##PLEASE MAKE SURE TO ADD THIS TO YOUR .bashrc or .bash_profile (or .profile)
#export FASTJET_DIR=$ANALYSISCODE"/fastjet-install/"
#export PATH="$PATH:$FASTJET_DIR/bin"
#export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$FASTJET_DIR/lib"

