namespace Amara {
    class Tween_ScrollCamera: public Tween {
        public:
            Amara::Camera* cam;

            bool center = false;

            float startX;
            float startY;
            float targetX;
            float targetY;

            FloatRect targetRect;

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

            void prepare() {
                cam = (Amara::Camera*)parent;
				cam->stopFollow();

                if (center) {
                    startX = cam->centerX;
                    startY = cam->centerY;

                    if (cam->lockedToBounds) {
                        targetRect = {
                            targetX - (cam->width/cam->zoomX)/2.0,
                            targetY - (cam->height/cam->zoomY)/2.0,
                            cam->width/cam->zoomX,
                            cam->height/cam->zoomY
                        };
                        if (targetRect.x < cam->boundX) targetRect.x = cam->boundX;
                        else if (targetRect.x + targetRect.width > cam->boundX + cam->boundW) targetRect.x = cam->boundX + cam->boundW - targetRect.width;
                        if (targetRect.y < cam->boundY) targetRect.y = cam->boundY;
                        else if (targetRect.y + targetRect.height > cam->boundY + cam->boundH) targetRect.y = cam->boundY + cam->boundW - targetRect.height;
                        targetX = targetRect.x + targetRect.width/2.0;
                        targetY = targetRect.y + targetRect.height/2.0;
                    }
                }
                else {
                    startX = cam->scrollX;
                    startY = cam->scrollY;

                    if (cam->lockedToBounds) {
                        targetRect = {
                            targetX,
                            targetY,
                            cam->width/cam->zoomX,
                            cam->height/cam->zoomY
                        };
                        if (targetRect.x < cam->boundX) targetRect.x = cam->boundX;
                        else if (targetRect.x + targetRect.width > cam->boundX + cam->boundW) targetRect.x = cam->boundX + cam->boundW - targetRect.width;
                        if (targetRect.y < cam->boundY) targetRect.y = cam->boundY;
                        else if (targetRect.y + targetRect.height > cam->boundY + cam->boundH) targetRect.y = cam->boundY + cam->boundW - targetRect.height;
                        targetX = targetRect.x;
                        targetY = targetRect.y;
                    }
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

            void prepare() {
                cam = (Amara::Camera*)parent;
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

	class Tween_ShakeCamera: public Amara::Tween {
	public:
		Amara::Camera* cam;

		float shakeStartX;
		float shakeStartY;
		float shakeEndX;
		float shakeEndY;

		bool center = false;

		RNG rng;

		Tween_ShakeCamera(float sx, float sy, float ex, float ey, float tt, Amara::Easing gEasing) {
			shakeStartX = sx;
			shakeStartY = sy;
			shakeEndX = ex;
			shakeEndY = ey;
			time = tt;
			easing = gEasing;
		}
		Tween_ShakeCamera(float sx, float sy, float ex, float ey, float tt): Tween_ShakeCamera(sx, sy, ex, ey, tt, LINEAR) {}

		Tween_ShakeCamera(float tx, float ty, float tt, Amara::Easing gEasing): Tween_ShakeCamera(tx, ty, 0, 0, tt, gEasing) {}
		Tween_ShakeCamera(float tx, float ty, float tt): Tween_ShakeCamera(tx, ty, tt, LINEAR) {}
		Tween_ShakeCamera(float tx, float ty): Tween_ShakeCamera(tx, ty, 1) {}

		void prepare() {
			cam = (Amara::Camera*)parent;

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

			cam->scrollOffsetX = shakeX;
			cam->scrollOffsetY = shakeY;
		}

		void finish() {
			Amara::Tween::finish();
			cam->offsetX = 0;
			cam->offsetY = 0;
		}
	};

    class Tween_ResizeCamera: public Amara::Tween {
    public:
        Amara::Camera* cam = nullptr;

        IntRect startRect;
        IntRect targetRect;

        Tween_ResizeCamera(IntRect rect, double tt, Amara::Easing gEase) {
            time = tt;
            easing = gEase;
        }

        void prepare() {
            if (cam == nullptr) cam = (Amara::Camera*)parent;

            startRect = { cam->x, cam->y, cam->width, cam->height };
        }

        void script() {
            Amara::Tween::progressFurther();

            cam->setSize(
                ease(startRect.x, targetRect.x, progress, easing),
                ease(startRect.y, targetRect.y, progress, easing),
                ease(startRect.width, targetRect.width, progress, easing),
                ease(startRect.height, targetRect.height, progress, easing)
            );
        }

        void finish() {
            Amara::Tween::finish();
            cam->setSize(targetRect);
        }
    };
}
