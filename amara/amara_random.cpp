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
                e.seed(time(0));
            }

			void seed(int s) {
				e.seed(s);
			}

			void seed(std::string s) {
				seed(std::hash<std::string>{}(s));
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
