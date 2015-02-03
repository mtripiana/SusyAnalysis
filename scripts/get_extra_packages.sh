#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.22
#rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-16 # (needs patching)

rc checkout_pkg $SVNOFF/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 

rc checkout_pkg $SVNOFF/Event/xAOD/xAODMissingET/tags/xAODMissingET-00-01-13
rc checkout_pkg $SVNOFF/Reconstruction/MET/METInterface/tags/METInterface-00-01-02
rc checkout_pkg $SVNOFF/Reconstruction/MET/METUtilities/tags/METUtilities-00-01-11-01
#rc checkout_pkg $SVNOFF/Reconstruction/MET/METUtilities/tags/METUtilities-00-01-18
rc checkout_pkg $SVNOFF/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-20
rc checkout_pkg $SVNOFF/PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection/tags/ElectronEfficiencyCorrection-00-01-13
rc checkout_pkg $SVNOFF/InnerDetector/InDetRecTools/InDetTrackSelectionTool/tags/InDetTrackSelectionTool-00-01-10
rc checkout_pkg $SVNOFF/Reconstruction/EventShapes/EventShapeTools/tags/EventShapeTools-00-01-03 # (needs patching)
rc checkout_pkg $SVNOFF/PhysicsAnalysis/AnalysisCommon/AssociationUtils/tags/AssociationUtils-01-00-10


rc checkout_pkg $SVNOFF/Trigger/TrigConfiguration/TrigConfxAOD/tags/TrigConfxAOD-00-00-10
rc checkout_pkg $SVNOFF/Trigger/TrigAnalysis/TrigDecisionTool/tags/TrigDecisionTool-00-02-18-14

#
# Apply needed patches:
#
patch -p0 -i SusyAnalysis/patches/patch_EventShapeCopier.cxx.diff
patch -p0 -i SusyAnalysis/patches/patch_SUSYObjDef_xAOD.cxx.diff  # add photon stuff
patch -p0 -i SusyAnalysis/patches/patch_SUSYObjDef_xAOD.h.diff
patch -p0 -i SusyAnalysis/patches/patch_SUSYToolsInit.cxx.diff    # lower msg output of some tools!
patch -p0 -i SusyAnalysis/patches/patch_DecisionObjectHandleStandalone.cxx.diff  # fix trigger dec access
patch -p0 -i SusyAnalysis/patches/patch_DecisionUnpackerStandalone.cxx.diff
patch -p0 -i SusyAnalysis/patches/patch_prepareTriggerMenu.cxx.diff


# Boost libraries
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Boost/trunk Asg_Boost

# FastJet
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_FastJet/trunk Asg_FastJet

# PdfReweighting package
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Lhapdf/trunk Asg_Lhapdf


# JetSmearing (IT will come...)
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-01-00-03 JetSmearing

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



