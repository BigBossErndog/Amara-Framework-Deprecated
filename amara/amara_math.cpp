#pragma once
#ifndef AMARA_MATH
#define AMARA_MATH

#include "amara.h"

namespace Amara {
    bool valueInRange(int val, int minVal, int maxVal) {
        return (val >= minVal) && (val <= maxVal);
    }

    bool valueInRange(float val, float minVal, float maxVal) {
        return (val >= minVal) && (val <= maxVal);
    }
    
    float Random() {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_real_distribution<float> dist(0, 1);
        return dist(e);
    }
}

#endif