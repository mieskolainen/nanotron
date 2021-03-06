// Charged track observables
//
// 
//
//

#ifndef nanotron_DataFormats_ChargedCandidateFeatures_h
#define nanotron_DataFormats_ChargedCandidateFeatures_h

#include <cmath>

namespace nanotron {

struct ChargedCandidateFeatures {

    // Absolute
    float px;
    float py;
    float pz;

    // Relative
    float ptrel;
    float deta;
    float dphi;
    float deltaR;
    

    float trackEtaRel;
    float trackPtRel;
    float trackPPar;
    float trackDeltaR;
    float trackPParRatio;
    float trackPtRatio;
    float trackSip2dVal;
    float trackSip2dSig;
    float trackSip3dVal;
    float trackSip3dSig;
    float trackJetDistVal;
    float trackJetDistSig;
    float drminsv;

    int vertex_association;
    
    float fromPV;
    float puppi_weight;
    float track_chi2;
    float track_quality;
    
    int track_numberOfValidPixelHits;
    int track_pixelLayersWithMeasurement;
    int track_numberOfValidStripHits;
    int track_stripLayersWithMeasurement; 
    
    float relmassdrop;
    
    float trackSip2dValSV;
    float trackSip2dSigSV;
    float trackSip3dValSV;
    float trackSip3dSigSV;

    float trackSip2dValSV_adapted;
    float trackSip2dSigSV_adapted;
    float trackSip3dValSV_adapted;
    float trackSip3dSigSV_adapted;

    int matchedMuon;
    int matchedElectron;
    int matchedSV;
    int matchedSV_adapted;
    int track_ndof;

    float dZmin;
    
    ChargedCandidateFeatures():
        px(0),
        py(0),
        pz(0),

        ptrel(-1),
        deta(1),
        dphi(1),
        deltaR(1),

        trackEtaRel(0),
        trackPtRel(0),
        trackPPar(0),
        trackDeltaR(0),
        trackPParRatio(0),
        trackPtRatio(0),
        trackSip2dVal(0),
        trackSip2dSig(0),
        trackSip3dVal(0),
        trackSip3dSig(0),
        trackJetDistVal(0),
        trackJetDistSig(0),
        drminsv(0),
        vertex_association(0),
        fromPV(0),
        puppi_weight(0),
        track_chi2(0),
        track_quality(0),
        track_numberOfValidPixelHits(0),
        track_pixelLayersWithMeasurement(0),
        track_numberOfValidStripHits(0),
        track_stripLayersWithMeasurement(0),
        relmassdrop(0),
        
        trackSip2dValSV(1),
        trackSip2dSigSV(0),
        trackSip3dValSV(1),
        trackSip3dSigSV(0),

        trackSip2dValSV_adapted(1),
        trackSip2dSigSV_adapted(0),
        trackSip3dValSV_adapted(1),
        trackSip3dSigSV_adapted(0),

        matchedMuon(0),
        matchedElectron(0),
        matchedSV(0),
        matchedSV_adapted(0),
        
        track_ndof(0),
        dZmin(100)
    {}
    
    bool operator < (const ChargedCandidateFeatures& other) const {

        if (trackSip2dSig > 0 and other.trackSip2dSig > 0) {
            //sort decreasing
            return std::fabs(trackSip2dSig) > std::fabs(other.trackSip2dSig);
        }
        else if (trackSip2dSig < 0 and other.trackSip2dSig > 0) {
            return false;
        }
        else if (trackSip2dSig > 0 and other.trackSip2dSig < 0) {
            return true;
        }
        else if (std::fabs(drminsv - other.drminsv) > std::numeric_limits<float>::epsilon()) {
            // sort increasing
            return drminsv < other.drminsv;
        }
        else {
            // sort decreasing
            return ptrel > other.ptrel;
        }
        
        return false;
    }

};

}

#endif
