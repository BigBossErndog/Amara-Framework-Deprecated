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
}

#endif