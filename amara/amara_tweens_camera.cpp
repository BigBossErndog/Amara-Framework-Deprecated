#pragma once
#ifndef AMARA_TWEENS
#define AMARA_TWEENS

#include "amara.h"

namespace Amara {
    class Tween_ScrollCamera: public Tween {
        public:
            Amara::Camera* cam;

            bool center = false;

            float startX;
            float startY;
            float targetX;
            float targetY;

            Tween_ScrollCamera(float tx, float ty, float tt, Amara::Easing gEasing, bool gCenter) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
                center = gCenter;
            }

            Tween_ScrollCamera(float tx, float ty, float tt, Amara::Easing gEasing): Tween_ScrollCamera(tx, ty, tt, gEasing, true) {}
            Tween_ScrollCamera(float tx, float ty, float tt): Tween_ScrollCamera(tx, ty, tt, LINEAR) {}
            Tween_ScrollCamera(float tx, float ty): Tween_ScrollCamera(tx, ty, 1) {}

            void prepare(Amara::Actor* gCam) {
                cam = (Amara::Camera*)gCam;
				cam->stopFollow();
                if (center) {
                    startX = cam->centerX;
                    startY = cam->centerY;
                }
                else {
                    startX = cam->scrollX;
                    startY = cam->scrollY;
                }
            }

            void script() {
                Amara::Tween::progressFurther();
                float nx = 0, ny = 0;

                switch (easing) {
                    case LINEAR:
                        nx = linearEase(startX, targetX, progress);
                        ny = linearEase(startY, targetY, progress);
                        break;
                    case SINE_INOUT:
                        nx = sineInOutEase(startX, targetX, progress);
                        ny = sineInOutEase(startY, targetY, progress);
                        break;
                    case SINE_IN:
                        nx = sineInEase(startX, targetX, progress);
                        ny = sineInEase(startY, targetY, progress);
                        break;
                    case SINE_OUT:
                        nx = sineOutEase(startX, targetX, progress);
                        ny = sineOutEase(startY, targetY, progress);
                        break;
                }

                if (center) {
                    cam->centerOn(nx, ny);
                }
                else {
                    cam->setScroll(nx, ny);
                }
            }

			void finish() {
				if (center) {
                    cam->centerOn(targetX, targetY);
                }
                else {
                    cam->setScroll(targetX, targetY);
                }
				Amara::Tween::finish();
			}
    };

    class Tween_CameraZoom: public Amara::Tween {
        public:
            Amara::Camera* cam;
            float zStartX = 1;
            float zStartY = 1;
            float zTarget = 1;

            Tween_CameraZoom(float gTarget, double tt, Amara::Easing gEasing) {
                zTarget = gTarget;
                time = tt;
                easing = gEasing;
            }
            Tween_CameraZoom(float gTarget, double tt): Tween_CameraZoom(gTarget, tt, LINEAR) {}

            void prepare(Amara::Actor* actor) {
                cam = (Amara::Camera*)actor;
                zStartX = cam->zoomX;
                zStartY = cam->zoomY;
            }

            void script() {
                Amara::Tween::progressFurther();
                float nzx = 0, nzy = 0;
                switch (easing) {
                    case LINEAR:
                        nzx = linearEase(zStartX, zTarget, progress);
                        nzy = linearEase(zStartY, zTarget, progress);
                        break;
                    case SINE_INOUT:
                        nzx = sineInOutEase(zStartX, zTarget, progress);
                        nzy = sineInOutEase(zStartY, zTarget, progress);
                        break;
                    case SINE_IN:
                        nzx = sineInEase(zStartX, zTarget, progress);
                        nzy = sineInEase(zStartY, zTarget, progress);
                        break;
                    case SINE_OUT:
                        nzx = sineOutEase(zStartX, zTarget, progress);
                        nzy = sineOutEase(zStartY, zTarget, progress);
                        break;
                }
                cam->setZoom(nzx, nzy);
            }
    };

    Amara::Script* createTween_ScrollCamera(float tx, float ty, double tt, Amara::Easing gEasing, bool center) {
        return (new Tween_ScrollCamera(tx, ty, tt, gEasing, center));
    };
    Amara::Script* createTween_CameraZoom(float tz, double tt, Amara::Easing gEasing) {
        return (new Tween_CameraZoom(tz, tt, gEasing));
    };

	class Tween_CameraShake: public Amara::Tween {
	public:
		Amara::Camera* cam;

		float shakeStartX;
		float shakeStartY;
		float shakeEndX;
		float shakeEndY;

		bool center = false;

		RNG rng;

		Tween_CameraShake(float sx, float sy, float ex, float ey, float tt, Amara::Easing gEasing) {
			shakeStartX = sx;
			shakeStartY = sy;
			shakeEndX = ex;
			shakeEndY = ey;
			time = tt;
			easing = gEasing;
		}
		Tween_CameraShake(float sx, float sy, float ex, float ey, float tt): Tween_CameraShake(sx, sy, ex, ey, tt, LINEAR) {}

		Tween_CameraShake(float tx, float ty, float tt, Amara::Easing gEasing): Tween_CameraShake(tx, ty, 0, 0, tt, gEasing) {}
		Tween_CameraShake(float tx, float ty, float tt): Tween_CameraShake(tx, ty, tt, LINEAR) {}
		Tween_CameraShake(float tx, float ty): Tween_CameraShake(tx, ty, 1) {}

		void prepare(Amara::Actor* actor) {
			cam = (Amara::Camera*)actor;

			if (shakeEndX == -1) shakeEndX = shakeStartX;
			if (shakeEndY == -1) shakeEndY = shakeStartY;

			rng.randomize();
		}

		void script() {
			Amara::Tween::progressFurther();
			float nx = 0, ny = 0;

			switch (easing) {
				case LINEAR:
					nx = linearEase(shakeStartX, shakeEndX, progress);
					ny = linearEase(shakeStartY, shakeEndY, progress);
					break;
				case SINE_INOUT:
					nx = sineInOutEase(shakeStartX, shakeEndX, progress);
					ny = sineInOutEase(shakeStartY, shakeEndY, progress);
					break;
				case SINE_IN:
					nx = sineInEase(shakeStartX, shakeEndX, progress);
					ny = sineInEase(shakeStartY, shakeEndY, progress);
					break;
				case SINE_OUT:
					nx = sineOutEase(shakeStartX, shakeEndX, progress);
					ny = sineOutEase(shakeStartY, shakeEndY, progress);
					break;
			}

			float shakeX = rng.random()*nx - nx/2.0;
			float shakeY = rng.random()*ny - ny/2.0;

			cam->offsetX = shakeX;
			cam->offsetY = shakeY;
		}

		void finish() {
			Amara::Tween::finish();
			cam->offsetX = 0;
			cam->offsetY = 0;
		}
	};
}

#endif
