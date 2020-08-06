#pragma once
#ifndef AMARA_TWEEN
#define AMARA_TWEEN

#include "amara.h"

namespace Amara {
    class Tween: public Script {
        public:
            Amara::Easing easing;
            double progress = 0;
            double time = 0;

            virtual void progressFurther() {
                progress += 1/(time*properties->lps);
                if (progress >= 1) {
                    progress = 1;
                    finish();
                }
            }
    };
}

#endif
