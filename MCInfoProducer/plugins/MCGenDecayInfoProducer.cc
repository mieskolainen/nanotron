//
//
//
//
//

// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "nanotron/DataFormats/interface/MCGenDecayInfo.h"
#include "nanotron/DataFormats/interface/DisplacedGenVertex.h"

#include <functional>

class MCGenDecayInfoProducer:
    public edm::stream::EDProducer<>
    
{
    private:    
        struct MCDecayChainCfg
        {
            std::string name;
            int llpId;
            std::vector<int> daughterIds;
        };

        /*
        static edm::Ptr<reco::GenParticle> getTauDecayProduct(edm::Ptr<reco::GenParticle> tau, edm::Handle<edm::View<reco::GenParticle>> genParticleCollection)
        {
            auto ghostCandidates = tau->daughterRefVector();
            for (auto const& ghostCandidate: tau->daughterRefVector()){
                edm::Ptr<reco::GenParticle> daughterPtr(genParticleCollection, ghostCandidate.index());
                if (std::abs(daughterPtr->pdgId())==15){
                    return getTauDecayProduct(daughterPtr, genParticleCollection);
                }
                if (std::abs(daughterPtr->pdgId()) == 11 or std::abs(daughterPtr->pdgId()) == 13) {
                    return daughterPtr;
                }
            }
        
            return tau;
        }
        */
        
        static double distance(const reco::Candidate::Point& p1, const reco::Candidate::Point& p2) {
            return std::sqrt((p1 - p2).mag2());
        }

        edm::EDGetTokenT<edm::View<reco::GenParticle>> genParticleToken_;
        
        std::vector<MCDecayChainCfg> llpDecayChains_;
        
        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
            
            
         
        
    public:
        explicit MCGenDecayInfoProducer(const edm::ParameterSet&);
        ~MCGenDecayInfoProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};



//
// constructors and destructor

//
MCGenDecayInfoProducer::MCGenDecayInfoProducer(const edm::ParameterSet& iConfig):
    genParticleToken_(
        consumes<edm::View<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("src"))
    ) {

    const edm::ParameterSet& decaysCfg = iConfig.getParameter<edm::ParameterSet>("decays");
    for (auto const& name: decaysCfg.getParameterNames()) {

        const edm::ParameterSet& decayCfg = decaysCfg.getParameter<edm::ParameterSet>(name);
        
        MCDecayChainCfg decayChainCfg;
        decayChainCfg.name = name;
        decayChainCfg.llpId = decayCfg.getParameter<int>("llpId");
        decayChainCfg.daughterIds = decayCfg.getParameter<std::vector<int>>("daughterIds");
        llpDecayChains_.push_back(decayChainCfg);
    }

    produces<std::vector<nanotron::MCGenDecayInfo>>();
}


MCGenDecayInfoProducer::~MCGenDecayInfoProducer()
{
}


// ------------ method called to produce the data  ------------
void
MCGenDecayInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    edm::Handle<edm::View<reco::GenParticle>> genParticleCollection;
    iEvent.getByToken(genParticleToken_, genParticleCollection);
    
    std::unique_ptr<reco::Candidate::Point> hardInteraction(nullptr);
        
    for (std::size_t igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle) {

        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        if (genParticle.isHardProcess() and genParticle.numberOfMothers()==2) {

            if (!hardInteraction) {
                hardInteraction.reset(new reco::Candidate::Point(genParticle.vertex()));
            } else if (distance(*hardInteraction,genParticle.vertex())>nanotron::DisplacedGenVertex::MIN_DISPLACEMENT) {
                throw cms::Exception("PartonsFromMCSelector: multiple hard interaction vertices found!");
            }
        }
    }
    
    
    auto output = std::make_unique<std::vector<nanotron::MCGenDecayInfo>>();
    
    // Found hard interaction
    if (hardInteraction) {
        for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
        {
            const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
            
            for (auto const& decayChain: llpDecayChains_)
            {
                if (abs(genParticle.pdgId())==decayChain.llpId)
                {
                    nanotron::MCGenDecayInfo llpGenDecayInfo;
                    llpGenDecayInfo.name = decayChain.name;
                    llpGenDecayInfo.llp = genParticleCollection->ptrAt(igenParticle);

                    for (auto const& daughter: genParticle.daughterRefVector()){

                        //const reco::Candidate* daughter = genParticle.daughter(idaughter);
                        //llp decay products need to be displaced wrt hard interaction
                        if (distance(*hardInteraction,daughter->vertex())<nanotron::DisplacedGenVertex::MIN_DISPLACEMENT)
                        {
                            continue;
                        }
                        
                        if (std::find(
                            decayChain.daughterIds.cbegin(),
                            decayChain.daughterIds.cend(),
                            abs(daughter->pdgId())
                        ) != decayChain.daughterIds.cend()) {
                            
                            if (daughter->pdgId()!=genParticleCollection->at(daughter.index()).pdgId()) {
                                throw cms::Exception("GenParticle relations not properly setup!");
                            }
                            

                            // Handle taus
                            /*
                            edm::Ptr<reco::GenParticle> daughterPtr(genParticleCollection, daughter.index());
                            if (abs(daughter->pdgId()) == 15){
                                daughterPtr = getTauDecayProduct(daughterPtr, genParticleCollection);
                            }
                            llpGenDecayInfo.decayProducts.push_back(daughterPtr);
                            */
                            
                        }
                    }
                    if (llpGenDecayInfo.decayProducts.size()>0)
                    {
                        /*
                        std::cout<<" llp decay: "<<llpGenDecayInfo.name<<", "<<llpGenDecayInfo.llp->pdgId()<<std::endl;
                        for (auto const& d: llpGenDecayInfo.decayProducts)
                        {
                            std::cout<<"   -> daughter: "<<d->pdgId()<<std::endl;
                        }
                        */
                        output->emplace_back(llpGenDecayInfo);
                    }
                }
            }
        }
    }
    
    //iEvent.put(std::move(hardInteraction));
    iEvent.put(std::move(output));
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MCGenDecayInfoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(MCGenDecayInfoProducer);


