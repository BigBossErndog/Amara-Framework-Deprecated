#ifndef AMARA_TWEENBASE
#define AMARA_TWEENBASE

#include "amara.h"

namespace Amara {
    class Tweener;

    class Tween: public Amara::StateManager {
        public:
            bool finished = false;
            bool deleteOnFinish = true;

            virtual void assign(Amara::Tweener* gTweener) {}
            virtual void run() {}
    };
}

#endif