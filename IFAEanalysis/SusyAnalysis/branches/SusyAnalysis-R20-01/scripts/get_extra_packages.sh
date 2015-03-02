#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.3.5
#rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
#rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-20 # (needs patching)
rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/trunk 

rc checkout_pkg $SVNOFF/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 

# Offline truth jet tagging for MCGN1 derivations
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/ParticleJetTools/tags/ParticleJetTools-00-03-24 ParticleJetTools

# Streamers problems with derivations
#rc checkout_pkg atlasoff/Event/xAOD/xAODCore/trunk    (it should work too)
rc checkout_pkg atlasoff/Event/xAOD/xAODCore/tags/xAODCore-00-00-87-01

# Boost libraries
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Boost/trunk Asg_Boost

# FastJet
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_FastJet/trunk Asg_FastJet

# PdfReweighting package
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Lhapdf/trunk Asg_Lhapdf


# JetSmearing (IT will come...)
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-01-00-03 JetSmearing


# METSmearing
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MET/METSmearing/tags/METSmearing-00-00-02 METSmearing  ## not used yet

# TruthToRecoFunctions
#
# (official package) (comment out if prefered)
# svn co svn+ssh://$USER@svn.cern.ch/reps/atlasgroups/PhysicsAnalysis/EuropeanStrategy/TruthToRecoFunctions/trunk TruthToRecoFunctions
#
# (recommended!) optimized code from Martin's public area:
#cp -r /afs/cern.ch/user/t/tripiana/public/IFAE/SUSY/TruthToRecoFunctions/ .      ## not used yet

# HistFitter
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/HistFitter/tags/HistFitter-00-00-42 HistFitter



