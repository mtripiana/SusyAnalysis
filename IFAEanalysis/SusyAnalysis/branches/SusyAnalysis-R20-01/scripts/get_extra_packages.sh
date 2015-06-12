#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.3.14
rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-06-10

rc checkout_pkg $SVNOFF/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 
rc checkout_pkg $SVNOFF/PhysicsAnalysis/TileID/TileTripReader/trunk

rc checkout_pkg $SVNOFF/PhysicsAnalysis/D3PDTools/EventLoop/tags/EventLoop-00-01-13
rc checkout_pkg $SVNOFF/PhysicsAnalysis/D3PDTools/EventLoopGrid/tags/EventLoopGrid-00-00-38
rc checkout_pkg $SVNOFF/PhysicsAnalysis/D3PDTools/SampleHandler/tags/SampleHandler-00-00-96


# Offline truth jet tagging for TRUTH1 derivations (no longer needed?)
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/ParticleJetTools/tags/ParticleJetTools-00-03-24 ParticleJetTools


# Boost libraries
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Boost/trunk Asg_Boost

# FastJet
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_FastJet/trunk Asg_FastJet

# PdfReweighting package
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Lhapdf/trunk Asg_Lhapdf


# JetSmearing (IT will come...)
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-01-00-05 JetSmearing

# HistFitter
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/HistFitter/tags/HistFitter-00-00-42 HistFitter



