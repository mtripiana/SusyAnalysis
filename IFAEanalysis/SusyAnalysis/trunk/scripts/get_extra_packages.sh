#
# Get extra packages needed on top of AnalysisSUSY release
#
#
: ${ANALYSISCODE:?" You need to set ANALYSISCODE before !! Please do so and get back..."}

cd $ANALYSISCODE

# Extra tags needed for Base,2.0.4
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/branches/SUSYTools-00-05-00-branch SUSYTools
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/JetAnalysisTools/BCHCleaningTool/tags/BCHCleaningTool-00-00-10 BCHCleaningTool
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/ElectronPhotonID/PhotonEfficiencyCorrection/tags/PhotonEfficiencyCorrection-00-00-04 PhotonEfficiencyCorrection
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/PileupReweighting/tags/PileupReweighting-00-02-12 PileupReweighting
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/TileID/TileTripReader/tags/TileTripReader-00-00-19 TileTripReader
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Trigger/TrigAnalysis/TrigMuonEfficiency/tags/TrigMuonEfficiency-00-02-47 TrigMuonEfficiency
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections/tags/MuonMomentumCorrections-00-09-23 MuonMomentumCorrections
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/Jet/ApplyJetCalibration/tags/ApplyJetCalibration-00-03-20 ApplyJetCalibration
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/Reconstruction/MissingETUtility/tags/MissingETUtility-01-02-08 MissingETUtility
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections/tags/MuonEfficiencyCorrections-02-01-20 MuonEfficiencyCorrections


# JetSmearing
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/JetSmearing/tags/JetSmearing-00-01-25 JetSmearing

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
source $ANALYSISCODE/code/scripts/install_fastjet.sh 

# PdfReweighting package
# Move to AsgExternal/Asg_Lhapdf ?
svn co svn+ssh://$USER@svn.cern.ch/reps/atlasoff/PhysicsAnalysis/D3PDTools/RootCoreLhapdf/trunk RootCoreLhapdf


