#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.13
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools

svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Tools/PathResolver/tags/PathResolver-00-02-06 PathResolver
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-04 JetCalibTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-08 JetUncertainties
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetInterface/tags/JetInterface-00-00-33 JetInterface
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetCPInterfaces/tags/JetCPInterfaces-00-00-00 JetCPInterfaces
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Control/AthToolSupport/AsgTools/tags/AsgTools-00-00-33 AsgTools


##needed by SUSYTools
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/egamma/egammaEvent/trunk egammaEvent
#this one is the old one! it won't be needed eventually
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/egamma/egammaAnalysis/egammaAnalysisUtils/tags/egammaAnalysisUtils-00-04-59 egammaAnalysisUtils

# Boost libraries
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Boost/trunk Asg_Boost

# JetSmearing
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-00-01-25 JetSmearing

# METSmearing
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MET/METSmearing/tags/METSmearing-00-00-02 METSmearing

# TruthToRecoFunctions
#
# (official package) (comment out if prefered)
# svn co svn+ssh://$USER@svn.cern.ch/reps/atlasgroups/PhysicsAnalysis/EuropeanStrategy/TruthToRecoFunctions/trunk TruthToRecoFunctions
#
# (recommended!) optimized code from Martin's public area:
cp -r /afs/cern.ch/user/t/tripiana/public/IFAE/SUSY/TruthToRecoFunctions/ .

# HistFitter
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/HistFitter/tags/HistFitter-00-00-27 HistFitter

# FastJet
#
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_FastJet/trunk Asg_FastJet

# PdfReweighting package
# Move to AsgExternal/Asg_Lhapdf ?
#svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/D3PDTools/RootCoreLhapdf/trunk RootCoreLhapdf
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Lhapdf/trunk Asg_Lhapdf

