///////////////////////////////////////////////////////////////////
// chorizoEDM.h:
// This file has EDM includes, EDM collections and CP tools for chorizo
// Keep all this in a separate header file to avoid CINT conflicts:
///////////////////////////////////////////////////////////////////

#ifndef chorizoEDM_H
#define chorizoEDM_H

//EDM includes:
#include "xAODEventInfo/EventInfo.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

#include "CPAnalysisExamples/errorcheck.h"

#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthEvent.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODMissingET/MissingET.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
// #include "xAODCaloEvent/CaloCluster.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/Vertex.h"
//#include "xAODTracking/TrackingPrimitives.h"


// Transient object store. Needed for the CP tools.
xAOD::TStore* m_store;

// Shallow-copy containers for AntiKt4LCTopo selected, recalibrated jets.
// Jet container made in initialize() as view container. Holds selected
// jets but does not own them.
xAOD::JetContainer* m_goodJets;


const xAOD::JetContainer* m_jets;
const xAOD::JetContainer* m_truth_jets;
const xAOD::ElectronContainer* m_electrons;
const xAOD::MuonContainer* m_muons;

const xAOD::TruthEventContainer* m_truthE;
const xAOD::TruthParticleContainer* m_truthP;

#endif //chorizoEDM_H
