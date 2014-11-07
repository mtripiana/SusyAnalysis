#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.16
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools

svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/TauID/TauAnalysisTools/tags/TauAnalysisTools-00-00-13 TauAnalysisTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-04 JetCalibTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04 JVFUncertaintyTool


svn co svn+ssh://svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetSubStructureMomentTools/trunk JetSubStructureMomentTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetSubStructureUtils/trunk JetSubStructureUtils


##needed by SUSYTools
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/egamma/egammaEvent/trunk egammaEvent
#this one is the old one! it won't be needed eventually
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/egamma/egammaAnalysis/egammaAnalysisUtils/tags/egammaAnalysisUtils-00-04-59 egammaAnalysisUtils

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



