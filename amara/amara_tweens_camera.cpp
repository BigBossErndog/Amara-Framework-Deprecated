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
                Amara::Tween::script();
                float nx = 0, ny = 0;
                
                switch (easing) {
                    case LINEAR:
                        nx = linearEase(startX, targetX, progress);
                        ny = linearEase(startY, targetY, progress);
                        break;
                    case SINE:
                        nx = sineEase(startX, targetX, progress);
                        ny = sineEase(startY, targetY, progress);
                        break;
                }

                if (center) {
                    cam->centerOn(nx, ny);
                }
                else {
                    cam->setScroll(nx, ny);
                }
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
                Amara::Tween::script();
                float nzx = 0, nzy = 0;
                switch (easing) {
                    case LINEAR:
                        nzx = linearEase(zStartX, zTarget, progress);
                        nzy = linearEase(zStartY, zTarget, progress);
                        break;
                    case SINE:
                        nzx = sineEase(zStartX, zTarget, progress);
                        nzy = sineEase(zStartY, zTarget, progress);
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
}

#endif