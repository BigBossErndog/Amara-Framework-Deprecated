#pragma once
#ifndef AMARA_TWEENS_ENTITIES
#define AMARA_TWEENS_ENTITIES



namespace Amara {
    class Tween_XY: public Tween {
        public:
            float startX = 0;
            float startY = 0;
            float targetX = 0;
            float targetY = 0;

            Tween_XY(float tx, float ty, double tt, Amara::Easing gEasing) {
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

    class Tween_RelativeXY: public Tween {
        public:
            float startX = 0;
            float startY = 0;
            float targetX = 0;
            float targetY = 0;

            Tween_RelativeXY(float tx, float ty, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
            }
            Tween_RelativeXY(float tx, float ty, double tt): Tween_RelativeXY(tx, ty, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
                targetX += actor->x;
                targetY += actor->y;
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

    class Tween_ScaleXY: public Tween {
        public:
            float startScaleX;
            float startScaleY;
            float targetScaleX;
			float targetScaleY;

            Tween_ScaleXY(float tx, float ty, double tt, Amara::Easing gEasing) {
                targetScaleX = tx;
				targetScaleY = ty;
                time = tt;
                easing = gEasing;
            }
			Tween_ScaleXY(float ts, double tt, Amara::Easing gEasing): Tween_ScaleXY(ts, ts, tt, gEasing) {}
            Tween_ScaleXY(float ts, double tt): Tween_ScaleXY(ts, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startScaleX = actor->scaleX;
                startScaleY = actor->scaleY;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                switch (easing) {
                    case LINEAR:
                        actor->scaleX = linearEase(startScaleX, targetScaleX, progress);
                        actor->scaleY = linearEase(startScaleY, targetScaleY, progress);
                        break;
                    case SINE_INOUT:
                        actor->scaleX = sineInOutEase(startScaleX, targetScaleX, progress);
                        actor->scaleY = sineInOutEase(startScaleY, targetScaleY, progress);
                        break;
                    case SINE_IN:
                        actor->scaleX = sineInEase(startScaleX, targetScaleX, progress);
                        actor->scaleY = sineInEase(startScaleY, targetScaleY, progress);
                        break;
                    case SINE_OUT:
                        actor->scaleX = sineOutEase(startScaleX, targetScaleX, progress);
                        actor->scaleY = sineOutEase(startScaleY, targetScaleY, progress);
                        break;
                }
            }
    };
	
	class Tween_SnapXY: public Script {
	public:
		float snapX;
		float snapY;

		Tween_SnapXY(float gx, float gy) {
			snapX = gx;
			snapY = gy;
		}

		void finish() {
			parent->x = snapX;
			parent->y = snapY;
			Script::finish();
		}

		void script() {
			finish();
		}
	};

    class Tween_ShakeXY: public Tween {
        public:
            float startX;
            float startY;
            float maxShakeX;
            float maxShakeY;
			float targetX;
			float targetY;

            Actor* ent;

            RNG rng;
			Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float gTargetX, float gTargetY, float tt, Amara::Easing gEasing) {
				maxShakeX = gMaxShakeX;
                maxShakeY = gMaxShakeY;
				targetX = gTargetX;
				targetY = gTargetY;
                time = tt;
                easing = gEasing;
			}
            Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float tt, Amara::Easing gEasing): Tween_ShakeXY(gMaxShakeX, gMaxShakeY, 0, 0, tt, gEasing)  {}
            Tween_ShakeXY(float gMaxShake, float tt, Amara::Easing gEasing): Tween_ShakeXY(gMaxShake, gMaxShake, tt, gEasing) {}
            Tween_ShakeXY(float gMaxShake, float tt): Tween_ShakeXY(gMaxShake, tt, LINEAR) {}
			Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float gTargetX, float gTargetY, float tt): Tween_ShakeXY(gMaxShakeX, gMaxShakeY, gTargetX, gTargetY, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
                ent = actor;

				if (targetX == -1) targetX = maxShakeX;
				if (targetY == -1) targetY = maxShakeY;

                rng.randomize();
            }

            void finish() {
                Tween::finish();
                ent->x = startX;
                ent->y = startY;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                float shakeAmountX;
                float shakeAmountY;
                switch (easing) {
                    case LINEAR:
                        shakeAmountX = linearEase(maxShakeX, targetX, progress);
                        shakeAmountY = linearEase(maxShakeY, targetY, progress);
                        break;
                    case SINE_INOUT:
                        shakeAmountX = sineInOutEase(maxShakeX, targetX, progress);
                        shakeAmountY = sineInOutEase(maxShakeY, targetY, progress);
                        break;
                    case SINE_IN:
                        shakeAmountX = sineInEase(maxShakeX, targetX, progress);
                        shakeAmountY = sineInEase(maxShakeY, targetY, progress);
                        break;
                    case SINE_OUT:
                        shakeAmountX = sineOutEase(maxShakeX, targetX, progress);
                        shakeAmountY = sineOutEase(maxShakeY, targetY, progress);
                        break;
                }
                actor->x = startX + rng.random()*shakeAmountX - shakeAmountX/2.0;
                actor->y = startY + rng.random()*shakeAmountY - shakeAmountY/2.0;
            }

			void cancel(Amara::Actor* actor) {
				actor->x = startX;
				actor->y = startY;
			}
    };

	class Tween_ReverseShakeXY: public Tween {
        public:
            float startX;
            float startY;
            float maxShakeX;
            float maxShakeY;

            Actor* ent;

            RNG rng;

            Tween_ReverseShakeXY(float gMaxShakeX, float gMaxShakeY, float tt, Amara::Easing gEasing) {
                maxShakeX = gMaxShakeX;
                maxShakeY = gMaxShakeY;
                time = tt;
                easing = gEasing;
            }
            Tween_ReverseShakeXY(float gMaxShake, float tt, Amara::Easing gEasing): Tween_ReverseShakeXY(gMaxShake, gMaxShake, tt, gEasing) {}
            Tween_ReverseShakeXY(float gMaxShake, float tt): Tween_ReverseShakeXY(gMaxShake, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                startX = actor->x;
                startY = actor->y;
                ent = actor;

                rng.randomize();
            }

            void finish() {
                Tween::finish();
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                float shakeAmountX;
                float shakeAmountY;
                switch (easing) {
                    case LINEAR:
                        shakeAmountX = linearEase(maxShakeX, 0, (1-progress));
                        shakeAmountY = linearEase(maxShakeY, 0, (1-progress));
                        break;
                    case SINE_INOUT:
                        shakeAmountX = sineInOutEase(maxShakeX, 0, (1-progress));
                        shakeAmountY = sineInOutEase(maxShakeY, 0, (1-progress));
                        break;
                    case SINE_IN:
                        shakeAmountX = sineInEase(maxShakeX, 0, (1-progress));
                        shakeAmountY = sineInEase(maxShakeY, 0, (1-progress));
                        break;
                    case SINE_OUT:
                        shakeAmountX = sineOutEase(maxShakeX, 0, (1-progress));
                        shakeAmountY = sineOutEase(maxShakeY, 0, (1-progress));
                        break;
                }
                actor->x = startX + rng.random()*shakeAmountX - shakeAmountX/2.0;
                actor->y = startY + rng.random()*shakeAmountY - shakeAmountY/2.0;
            }

			void cancel(Amara::Actor* actor) {
				actor->x = startX;
				actor->y = startY;
			}
    };

    class Tween_Alpha: public Tween {
        public:
            float startAlpha = 0;
            float targetAlpha = 0;

            Tween_Alpha(float gTarget, float gTime, Amara::Easing gEasing) {
                targetAlpha = gTarget;
                time = gTime;
                easing = gEasing;
            }
            Tween_Alpha(float gTarget, float gTime): Tween_Alpha(gTarget, gTime, LINEAR) {}
            Tween_Alpha(float gTime): Tween_Alpha(0, gTime) {}

            void prepare(Amara::Actor* actor) {
                startAlpha = actor->alpha;
            }

            void script(Amara::Actor* actor) {
                Amara::Tween::progressFurther();
                switch (easing) {
                    case LINEAR:
                        actor->alpha = linearEase(startAlpha, targetAlpha, progress);
                        break;
                    case SINE_INOUT:
                        actor->alpha = sineInOutEase(startAlpha, targetAlpha, progress);
                        break;
                    case SINE_IN:
                        actor->alpha = sineInEase(startAlpha, targetAlpha, progress);
                        break;
                    case SINE_OUT:
                        actor->alpha = sineOutEase(startAlpha, targetAlpha, progress);
                        break;
                }
            }

            void finish() {
                Tween::finish();
                parent->alpha = targetAlpha;
            }
    };

    class Tween_Jump: public Tween {
    public:
        float targetX;
        float targetY;
        float maxZ;

        float startX;
        float startY;
        float startZ;
        
        Tween_Jump(float gx, float gy, float gz, float gt) {
            targetX = gx;
            targetY = gy;
            maxZ = gz;
            time = gt;
        }

        void prepare(Actor* actor) {
            startX = actor->x;
            startY = actor->y;
            startZ = actor->z;
        }

        void script(Actor* actor) {
            Tween::progressFurther();
            actor->x = linearEase(startX, targetX, progress);
            actor->y = linearEase(startY, targetY, progress);
            actor->z = sineHardInOutEase(startZ, maxZ, progress);
        }
    };

    class Script_SetVisible: public Script {
    public:
        bool toSet = true;
        bool justToggle = false;

        Script_SetVisible() {
            justToggle = true;
        }
        Script_SetVisible(bool v) {
            toSet = v;
        }

        void prepare() {
            if (justToggle) toSet = !parent->isVisible;
        }

        void script() {
            parent->setVisible(toSet);
            finish();
        }
    };
}

#endif
