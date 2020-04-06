#ifndef AMARA_TWEENS_ENTITIES
#define AMARA_TWEENS_ENTITIES

#include "amara.h"

namespace Amara {
    class Tween_XY: public Tween {
        public:
            float startX = 0;
            float startY = 0;
            float targetX = 0;
            float targetY = 0;

            Tween_XY(float tx, int ty, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
            }
            Tween_XY(float tx, float ty, double tt): Tween_XY(tx, ty, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::script();
                switch (easing) {
                    case LINEAR:
                        actor->x = linearEase(startX, targetX, progress);
                        actor->y = linearEase(startY, targetY, progress);
                        break;
                    case SINE:
                        actor->x = sineEase(startX, targetX, progress);
                        actor->y = sineEase(startY, targetY, progress);
                        break;
                }
            }
    };
}

#endif