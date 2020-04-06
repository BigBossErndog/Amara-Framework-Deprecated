#ifndef AMARA_EASING
#define AMARA_EASING

#include "amara.h"

namespace Amara {
    enum Easing {
        LINEAR,
        SINE
    };

    float linearEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*progress;
    }   

    double sineEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*sin(progress*M_PI/2);
    }
}

#endif