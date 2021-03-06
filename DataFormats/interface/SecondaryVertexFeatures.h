// Secondary vertex observables
//
//
//
//

#ifndef nanotron_DataFormats_SecondaryVertexFeatures_h
#define nanotron_DataFormats_SecondaryVertexFeatures_h

#include <cmath>

namespace nanotron {

struct SecondaryVertexFeatures {
    
    float vx; // vertex position
    float vy;
    float vz;
    
    float ptrel;
    float deta;
    float dphi;
    float deltaR;
    float mass;
    
    int   ntracks;
    float chi2;
    int   ndof;
    
    float dxy;
    float dxysig;
    float d3d;
    float d3dsig;
    float costhetasvpv;
    float enratio;
    
    SecondaryVertexFeatures():
        vx(0),
        vy(0),
        vz(0),

        ptrel(0),
        deta(0),
        dphi(0),
        deltaR(0),
        mass(0),

        ntracks(0),
        chi2(0),
        ndof(0),

        dxy(0),
        dxysig(0),
        d3d(0),
        d3dsig(0),
        costhetasvpv(0),
        enratio(0)
    {}
    
    bool operator < (const SecondaryVertexFeatures& other) const {
        if (dxysig > 0 and other.dxysig > 0) {
            if (std::fabs(dxysig - other.dxysig) > std::numeric_limits<float>::epsilon()) {
                return dxysig>other.dxysig;
            }
        }
        return ptrel>other.ptrel; //sort decreasing
    }
};

}

#endif
