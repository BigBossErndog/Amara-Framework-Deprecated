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
                    case SINE_INOUT:
                        actor->x = sineInOutEase(startX, targetX, progress);
                        actor->y = sineInOutEase(startY, targetY, progress);
                        break;
                }
            }
    };

    class Tween_Scale: public Tween {
        public:
            float startScaleX;
            float startScaleY;
            float targetScale;

            Tween_Scale(float ts, double tt, Amara::Easing gEasing) {
                targetScale = ts;
                time = tt;
                easing = gEasing;
            }
            Tween_Scale(float ts, double tt): Tween_Scale(ts, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startScaleX = actor->scaleX;
                startScaleY = actor->scaleY;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::script();
                switch (easing) {
                    case LINEAR:
                        actor->scaleX = linearEase(startScaleX, targetScale, progress);
                        actor->scaleY = linearEase(startScaleY, targetScale, progress);
                        break;
                    case SINE_INOUT:
                        actor->scaleX = sineInOutEase(startScaleX, targetScale, progress);
                        actor->scaleY = sineInOutEase(startScaleY, targetScale, progress);
                        break;
                }
            }
    };
}

#endif