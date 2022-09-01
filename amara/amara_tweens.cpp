#ifndef AMARA_TWEENS
#define AMARA_TWEENS



namespace Amara {
    class CameraScrollTween: public Tween {
        public: 
            Amara::Camera* cam;

            Amara::Easing easing;

            float startX;
            float startY;
            float targetX;
            float targetY;

            CameraScrollTween(float tx, float ty, float tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
            }

            CameraScrollTween(float tx, float ty, float tt): CameraScrollTween(tx, ty, tt, SINE) {}
            CameraScrollTween(float tx, float ty): CameraScrollTween(tx, ty, 1) {}
            
            void assign(Amara::Tweener* gCam, Amara::GameProperties* gProperties) {
                Amara::Tween::assign(gCam, gProperties);
                cam = (Amara::Camera*)gCam;
                startX = cam->scrollX;
                startY = cam->scrollY;
            }
            
            void run() {
                Amara::Tween::run();
                switch (easing) {
                    case LINEAR:
                        cam->scrollX = linearEase(startX, targetX, progress);
                        cam->scrollY = linearEase(startY, targetY, progress);
                        break;
                    case SINE:
                        cam->scrollX = sineEase(startX, targetX, progress);
                        cam->scrollY = sineEase(startY, targetY, progress);
                        break;
                }
            }
    };

    class CameraZoomTween: public Tween {
        public:

    };
}

#endif