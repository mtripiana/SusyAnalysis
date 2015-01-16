#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.22
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-15 SUSYTools

svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 JVFUncertaintyTool

svn co svn+ssh://svn.cern.ch/reps/atlasoff/Event/xAOD/xAODMissingET/tags/xAODMissingET-00-01-12 xAODMissingET         
svn co svn+ssh://svn.cern.ch/reps/atlasoff/Reconstruction/MET/METInterface/tags/METInterface-00-01-02 METInterface    
svn co svn+ssh://svn.cern.ch/reps/atlasoff/Reconstruction/MET/METUtilities/tags/METUtilities-00-01-08 METUtilities    
svn co svn+ssh://svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-20 JetCalibTools                             
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection/tags/ElectronEfficiencyCorrection-00-01-13 ElectronEfficiencyCorrection
svn co svn+ssh://svn.cern.ch/reps/atlasoff/InnerDetector/InDetRecTools/InDetTrackSelectionTool/tags/InDetTrackSelectionTool-00-00-02 InDetTrackSelectionTool # (needs patching)
svn co svn+ssh://svn.cern.ch/reps/atlasoff/Reconstruction/EventShapes/EventShapeTools/tags/EventShapeTools-00-01-03 EventShapeTools # (needs patching)

#
# Apply needed patches:
#
patch -p0 -i /nfs/pic.es/user/t/tripiana/public/IFAE/SusyAnalysis_patches/patch_IInDetTrackSelectionTool.diff
patch -p0 -i /nfs/pic.es/user/t/tripiana/public/IFAE/SusyAnalysis_patches/patch_EventShapeCopier.cxx.diff
patch -p0 -i /nfs/pic.es/user/t/tripiana/public/IFAE/SusyAnalysis_patches/patch_SUSYObjDef_xAOD.cxx.diff  # add photon stuff
patch -p0 -i /nfs/pic.es/user/t/tripiana/public/IFAE/SusyAnalysis_patches/patch_SUSYObjDef_xAOD.h.diff
 

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



