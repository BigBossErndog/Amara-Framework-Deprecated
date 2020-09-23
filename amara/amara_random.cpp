#pragma once
#ifndef AMARA_RANDOM
#define AMARA_RANDOM

#include "amara.h"

namespace Amara {
    class RNG {
        public:
            std::random_device rd;
            std::default_random_engine e;

            RNG* randomize() {
                e.seed(time(0));
				return this;
            }

			RNG* seed(int s) {
				e.seed(s);
				return this;
			}

			RNG* seed(std::string s) {
				seed(std::hash<std::string>{}(s));
				return this;
			}

            double between(double min, double max) {
                std::uniform_real_distribution<double> dist(0, 1);
                return dist(e);
            }

            float random() {
                return between(0, 1);
            }
    };
}

#endif
