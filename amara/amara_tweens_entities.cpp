#pragma once
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
                Amara::Tween::progressFurther();
                switch (easing) {
                    case LINEAR:
                        actor->x = linearEase(startX, targetX, progress);
                        actor->y = linearEase(startY, targetY, progress);
                        break;
                    case SINE_INOUT:
                        actor->x = sineInOutEase(startX, targetX, progress);
                        actor->y = sineInOutEase(startY, targetY, progress);
                        break;
                    case SINE_IN:
                        actor->x = sineInEase(startX, targetX, progress);
                        actor->y = sineInEase(startY, targetY, progress);
                        break;
                    case SINE_OUT:
                        actor->x = sineOutEase(startX, targetX, progress);
                        actor->y = sineOutEase(startY, targetY, progress);
                        break;
                }
            }
    };

	class Tween_XYZ: public Tween {
        public:
            float startX = 0;
            float startY = 0;
			float startZ = 0;
            float targetX = 0;
            float targetY = 0;
			float targetZ = 0;

            Tween_XYZ(float tx, float ty, float tz, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
				targetZ = tz;
                time = tt;
                easing = gEasing;
            }
            Tween_XYZ(float tx, float ty, float tz, double tt): Tween_XYZ(tx, ty, tz, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
				startZ = actor->z;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                switch (easing) {
                    case LINEAR:
                        actor->x = linearEase(startX, targetX, progress);
                        actor->y = linearEase(startY, targetY, progress);
						actor->z = linearEase(startZ, targetZ, progress);
                        break;
                    case SINE_INOUT:
                        actor->x = sineInOutEase(startX, targetX, progress);
                        actor->y = sineInOutEase(startY, targetY, progress);
						actor->z = sineInOutEase(startZ, targetZ, progress);
                        break;
                    case SINE_IN:
                        actor->x = sineInEase(startX, targetX, progress);
                        actor->y = sineInEase(startY, targetY, progress);
						actor->z = sineInEase(startZ, targetZ, progress);
                        break;
                    case SINE_OUT:
                        actor->x = sineOutEase(startX, targetX, progress);
                        actor->y = sineOutEase(startY, targetY, progress);
						actor->z = sineOutEase(startZ, targetZ, progress);
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
                Amara::Tween::progressFurther();
                switch (easing) {
                    case LINEAR:
                        actor->scaleX = linearEase(startScaleX, targetScale, progress);
                        actor->scaleY = linearEase(startScaleY, targetScale, progress);
                        break;
                    case SINE_INOUT:
                        actor->scaleX = sineInOutEase(startScaleX, targetScale, progress);
                        actor->scaleY = sineInOutEase(startScaleY, targetScale, progress);
                        break;
                    case SINE_IN:
                        actor->scaleX = sineInEase(startScaleX, targetScale, progress);
                        actor->scaleY = sineInEase(startScaleY, targetScale, progress);
                        break;
                    case SINE_OUT:
                        actor->scaleX = sineOutEase(startScaleX, targetScale, progress);
                        actor->scaleY = sineOutEase(startScaleY, targetScale, progress);
                        break;
                }
            }
    };

    class Tween_Shake: public Tween {
        public:
            float startX;
            float startY;
            float maxShake;

            Actor* ent;

            RNG rng;

            Tween_Shake(float gMaxShake, float tt, Amara::Easing gEasing) {
                maxShake = gMaxShake;
                time = tt;
                easing = gEasing;
            }
            Tween_Shake(float gMaxShake, float tt): Tween_Shake(gMaxShake, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
                ent = actor;

                rng.randomize();
            }

            void finish() {
                Tween::finish();
                ent->x = startX;
                ent->y = startY;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                float shakeAmount;
                switch (easing) {
                    case LINEAR:
                        shakeAmount = linearEase(maxShake, 0, progress);
                        break;
                    case SINE_INOUT:
                        shakeAmount = sineInOutEase(maxShake, 0, progress);
                        break;
                    case SINE_IN:
                        shakeAmount = sineInEase(maxShake, 0, progress);
                        break;
                    case SINE_OUT:
                        shakeAmount = sineOutEase(maxShake, 0, progress);
                        break;
                }
                actor->x = startX + rng.random()*shakeAmount - shakeAmount/2.0;
                actor->y = startY + rng.random()*shakeAmount - shakeAmount/2.0;
            }

			void cancel(Amara::Actor* actor) {
				actor->x = startX;
				actor->y = startY;
			}
    };
}

#endif
