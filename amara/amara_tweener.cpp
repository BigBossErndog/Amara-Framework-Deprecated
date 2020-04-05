#ifndef AMARA_TWEENER
#define AMARA_TWEENER

#include "amara.h"

namespace Amara {
    class Tweener {
        public:
            Tweener() {}

            std::vector<Amara::Tween*> tweens;

            virtual void tween(Amara::Tween* tween) {
                tween->assign(this);
                tweens.push_back(tween);
            }

            virtual void run() {
                Amara::Tween* tween;
                for (auto it = tweens.begin(); it != tweens.end(); it++) {
                    tween = *it;
                    tween->run();
                    if (tween->finished) {
                        tweens.erase(it--);
                        if (tween->deleteOnFinish) {
                            delete tween;
                        }
                    }
                }
            }
    };
}

#endif