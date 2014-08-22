#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.6
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MissingETUtility/tags/MissingETUtility-01-02-08 MissingETUtility
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection/tags/ElectronEfficiencyCorrection-00-01-03 ElectronEfficiencyCorrection
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MET/METUtilities/tags/METUtilities-00-00-05 METUtilities
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MET/METInterface/tags/METInterface-00-00-04 METInterface
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Control/xAODRootAccess/trunk xAODRootAccess
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections/tags/MuonMomentumCorrections-01-00-08 MuonMomentumCorrections
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/TauID/TauAnalysisTools/tags/TauAnalysisTools-00-00-06 TauAnalysisTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency/tags/xAODBTaggingEfficiency-00-00-02 xAODBTaggingEfficiency
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-05-00 CalibrationDataInterface
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Tools/PathResolver/trunk PathResolver
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/AsgExternal/Asg_Boost/trunk Asg_Boost
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/CPAnalysisExamples/tags/CPAnalysisExamples-00-00-18/ CPAnalysisExamples

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
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/D3PDTools/RootCoreLhapdf/trunk RootCoreLhapdf


