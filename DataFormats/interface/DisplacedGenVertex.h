// Displaced generator level vertex
// 
// 
//
//

#ifndef nanotron_DATAFORMATS_DISPLACEDGENVERTEX_H
#define nanotron_DATAFORMATS_DISPLACEDGENVERTEX_H


// CMSSW
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"


#include <vector>

namespace nanotron {

struct DisplacedGenVertex {
    
    static constexpr float MIN_DISPLACEMENT = 1e-10;
    static constexpr float MIN_LLP_MASS     = 1e-8;
    
    bool isHardInteraction;
    double sharedMassFraction; // fraction of invariant LLP mass carried by its first daughters in vertex
    
    
    reco::Candidate::Point vertex;
    reco::Candidate::Point hardInteraction;
    
    edm::Ptr<DisplacedGenVertex> motherVertex;
    edm::RefVector<std::vector<DisplacedGenVertex>> daughterVertices;
    
    edm::PtrVector<reco::GenParticle> genParticles;
    edm::PtrVector<reco::GenParticle> lspParticles;
    edm::Ptr<reco::GenParticle> motherLongLivedParticle;
    
    std::vector<reco::GenJet> genJets;
    std::vector<float> jetFractions; 
    
    reco::Candidate::LorentzVector llp_reco;
    
    DisplacedGenVertex():
        isHardInteraction(false),
        sharedMassFraction(0),
        vertex(0,0,0),
        hardInteraction(0,0,0)
    {
    }
    
    double d3d() const;
    double dx()  const;
    double dy()  const;
    double dz()  const;
    double dxy() const;
};

typedef std::vector<DisplacedGenVertex> DisplacedGenVertexCollection;

}

#endif

