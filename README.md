
SETUP
------------------------------------------------------------------------------------------------------------------------------------------------------


1) Analysis release:
   =====================

The first thing you need to do is to set the Analysis release, so to have access to all packages used by our code.

For that you do simply (after 'setupATLAS' if not in at3), from your Analysis folder:

     >  rcSetup SUSY,2.3.24a
 
     Version might change in the future though. Please check https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AnalysisRelease        	 	      

     Then everytime you login, you just need to do:

     > rcSetup


2) Env variables:
   =====================

   Please make sure to add these variables to your environment (.bash_profile or .bashrc or .profile), so the code knows where to fund some stuff:

   > export ANALYSISCODE="<your analysis folder>"      # where your code will be, from where you did the rcSetup above.
   > export HISTFITTERDIR="<your HistFitter folder>"   # where you have (or gonna place) the HistFitter code.


3) Getting the analysis code:
   =====================
   cd $ANALYSISCODE
   svn co svn+ssh://$USER@svn.cern.ch/reps/IfaeAnaRepo/IFAEanalysis/SusyAnalysis/trunk SusyAnalysis
   svn co svn+ssh://$USER@svn.cern.ch/reps/IfaeAnaRepo/IFAEanalysis/SusyAnalysis/branches/SusyAnalysis-Rel20-01 SusyAnalysis


4) Getting extra packages (not included in the Analysis release):
   =====================
   source $ANALYSISCODE/SusyAnalysis/scripts/get_extra_packages.sh


5) Compile everything:
   =====================
   rc find_packages	
   rc compile

   For a quick compile of our package simply do:  'rc compile_pkg SusyAnalysis' 

NOTE:   before running please check the path for the pdfrw tool is properly set. 
        --> do : 
             echo $LHAPDF_DATA_PATH

        This should look like this:
	"$ROOTCOREBIN/data/Asg_Lhapdf_LHAPDF:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:"
         
	 where $ROOTCOREBIN = $ANALYSISCODE/RootCoreBin

	If it is not the case please set it up, soing something like :
 
        echo "export LHAPDF_DATA_PATH="$ANALYSISCODE"/RootCoreBin/data/Asg_Lhapdf_LHAPDF:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:" >> $HOME/.bashrc

        (and source your .bashrc . Or just start a new session and re-do 'rcsetup'). 


6) To run the analysis:
   =====================

   *** grid-stuff setup
       
       As the sample metadata is fetched from AMI, we need a voms-proxy beforehand. To do so please run (after rcSetup):

       	  source SusyAnalysis/scripts/grid_up.sh


   *NOTE( (30/10/2014):  (in case you don't use the script above to setup the grid)  
                         There is a new default version of PyAMI in at3. It is a major update (v5.0.0) which is not backward compatible! (and so the fetch tool won't work)
   	                 Please make sure you set the previous version, unless until SampleHandler gets updated:
                       
                       -->  localSetupPyAMI pyAMI-4.1.3a-x86_64-slc6
  
           
   *** to run the code you do:

          run [options] <Sample> 

       where

		<Sample>  : The sample name to run over (comma-sep list). 

		            To list the implemented samples do:      'run samples'
			  
	        [options] : supported option flags

			    	 -j=<jOption>   : choose which analysis you want to run over. ( = METbb, Stop, Monojet)	
			         -l             : run locally (default) 
			         -b             : run in batch          
				 -p             : run on the grid    (prun)   
				 -g             : run on the grid    (ganga)     
				 -u             : generate pileup file (overrides jOption config)
				 -x             : switch to 'at3_xxl' queue (when running in batch mode) (default='at3') 
				 -t             : run over truth xAODs
				 -a             : is AFII reconstruction? It is guessed from the sample name otherwise.
				 -n=<N>         : run over N events
				 -i=<ID>        : specify a specific dataset ID inside the container
				 -v=<V>         : output version. To tag output files. (adds a '_vV' suffix)
				 -s=<S>         : systematics to be considered (comma-separated list) [optional]. 
		            	 	          Just nominal is taken by default (Nom).

 						  To list the available (recommended) systematic variations do:     'run slist'

						  or well run './SusyAnalysis/scripts/list_systematics.sh'

 				 -o=<outDir>    : where all the output is saved (if left empty is reads the path from the xml jobOption (FinalPath)).
                                 -e=<eventList> : provide list of run & event numbers you want to keep. ("Run EventNumber" formatted file. One per line)
 				 -d=<inputDir>  : run over specific directory, over-passing RunsMap (mainly for tests)


       or just type:  'run'  to get this help.

   ***
   ** Grid submission

   a) with Prun: 
          nothing to do here. Do simply:
           
              run -p MySample

   b) with Ganga:
      Before running for the first time: The GridDriver uses ganga, which currently needs to create a few configuration defaults before it can be run for the first time. If you have never used ganga before, 
      start it once in interactive mode before using the GridDriver:
 
	export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
	source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
	localSetupGanga
	ganga

      Answer the questions, then press ctrl+d to exit. You only need to do this once on each computer where you want to use the GridDriver.    

      From then on, you can also simply do:

      	      run -g MySample

      and you can add a tag to your output samples in case you run several times on the same input, with the -v option.

   *** 
   ** Batch submission **

   a) Create batch submission scripts (only once! Might take some time though...):

      root generate_qsub_files.C

   b) Run
      e.g. to produce the mini-ntuples for X-sample:

      run -b X-sample -j=Stop -s=Nom,MUONS_ID__1up,MUONS_ID__1down


   Or you could run the script:

      submit_batch.sh

   after modifying it accordingly. Put the samples and systematics you would like to run there.



   ** NOTE:  for the moment there is no nice to get the proxy running on the batch. 
             So we need to re-set the grid stuff locally again on the running machine, for what we need the password :)
	     A way to do it (not nice I know but ...) is to place your grid certificate password in a file a call 
             voms-init-proxy with it. 

             In short...you need to:
      
                 - create a $ANALYSISCODE/.gridpsd file with just your password in it
		 - make sure its permissions are correct (so you are minimally protected :) ) :   
  		   	chmod 600 $ANALYSISCODE/.gridsp
		 - voila! 
             


---------------- Caveats & Warnings --------------------------
- it is not possible to mix samples at diff e.c.m.  ( I think it is now! thanks to the wrapper. CHECK! (Martin))
- it is not possible to put MC and data within the same sample (= runs map key) (code prevents this anyways)


---------------- EXTRAS --------------------------

ROOT version:
=============

If you have problems setting ROOT (although the rcSetup should do it for you) you can write this in your .bash_profile:

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
echo "ATLAS_LOCAL_ROOT_BASE "$ATLAS_LOCAL_ROOT_BASE
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
localSetupROOT





