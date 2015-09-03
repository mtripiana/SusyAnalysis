#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.1.33
#rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-31

rc checkout_pkg $SVNOFF/Reconstruction/EventShapes/EventShapeTools/tags/EventShapeTools-00-01-09
rc checkout_pkg $SVNOFF/Reconstruction/EventShapes/EventShapeInterface/tags/EventShapeInterface-00-00-09
rc checkout_pkg $SVNOFF/PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection/tags/ElectronEfficiencyCorrection-00-01-19
rc checkout_pkg $SVNOFF/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 
rc checkout_pkg $SVNOFF/PhysicsAnalysis/ElectronPhotonID/PhotonEfficiencyCorrection/tags/PhotonEfficiencyCorrection-00-01-05
rc checkout_pkg $SVNPERF/CombPerf/FlavorTag/FlavorTagEfficiencyMaps/tags/FlavorTagEfficiencyMaps-00-00-13
mv FlavorTagEfficiencyMaps BTagEfficiencyReader

rc checkout_pkg $SVNOFF/PhysicsAnalysis/TileID/TileTripReader/trunk
rc checkout_pkg $SVNOFF/PhysicsAnalysis/ElectronPhotonID/ElectronIsolationSelection/tags/ElectronIsolationSelection-00-01-02


# Offline truth jet tagging for MCGN1 derivations
rc checkout_pkg $SVNOFF/PhysicsAnalysis/AnalysisCommon/ParticleJetTools/tags/ParticleJetTools-00-03-24 

#
# Apply needed patches:
#
#cd $ANALYSISCODE


# Boost libraries
#
rc checkout_pkg $SVNOFF/AsgExternal/Asg_Boost/trunk

# FastJet
#
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_FastJet/trunk Asg_FastJet
rc checkout_pkg $SVNOFF/AsgExternal/Asg_FastJet/tags/Asg_FastJet-03-06-14

# PdfReweighting package
rc checkout_pkg $SVNOFF/AsgExternal/Asg_Lhapdf/trunk


# JetSmearing (IT will come...)
rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-01-00-03 JetSmearing


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


