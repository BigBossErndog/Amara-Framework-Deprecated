#pragma once
#ifndef AMARA_RANDOM
#define AMARA_RANDOM

#include "amara.h"

namespace Amara {
    class RNG {
        public:
            std::random_device rd;
            std::default_random_engine e;

            void randomize() {
                e.seed(rd());
            }
            
            float between(float min, float max) {
                std::uniform_real_distribution<float> dist(0, 1);
                return dist(e);
            }

            float random() {
                return between(0, 1);
            }
    };
}

#endif