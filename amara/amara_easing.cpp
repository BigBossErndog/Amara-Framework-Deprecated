#pragma once
#ifndef AMARA_EASING
#define AMARA_EASING

#include "amara.h"

namespace Amara {
    enum Easing {
        LINEAR,
        SINE_INOUT,
        SINE_IN,
        SINE_OUT,
        SINE_INOUT_BACKROUND
    };

    double linearEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*progress;
    }   

    double sineInOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI) + 1)/2;
    }

    double sineOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(progress*M_PI/2));
    }

    double sineInEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI/2) + 1);
    }

    double sineInOutBackRoundEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI*2) + 1)/2;
    }

    double sineHardInOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(M_PI * progress));
    }
}

#endif