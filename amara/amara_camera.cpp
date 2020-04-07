#ifndef AMARA_CAMERA
#define AMARA_CAMERA

#include "amara.h"

namespace Amara {
    class Scene;

    class Camera : public Amara::Actor {
        public:
            std::vector<Amara::Entity*>* sceneEntities = nullptr;

            bool definedDimensions = false;

            float width = 0;
            float height = 0;

            float oldScrollX = 0;
            float oldScrollY = 0;
            float scrollX = 0;
            float scrollY = 0;

            float oldZoomX = 0;
            float oldZoomY = 0;
            float zoomX = 1;
            float zoomY = 1;

            float oldCenterX = 0;
            float oldCenterY = 0;
            float centerX = 0;
            float centerY = 0;

            float offsetX = 0;
            float offsetY = 0;

            Amara::Entity* followTarget = nullptr;
            float lerpX = 1;
            float lerpY = 1;

            bool lockedToBounds = false;
            float boundX = 0;
            float boundY = 0;
            float boundW = 0;
            float boundH = 0;

            Camera() {
                definedDimensions = false;
            }

            Camera(float gw, float gh) : Camera() {
                definedDimensions = true;
                width = gw;
                height = gh;
            }

            Camera(float gx, float gy, int gw, int gh): Camera(gw, gh) {
                x = gx;
                y = gy;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* gParent, std::vector<Amara::Entity*>* givenEntities) {
                properties = gameProperties;
                sceneEntities = givenEntities;
                parent = gParent;

                if (!definedDimensions) {
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

                recordValues();
            }

            virtual void run() {
				if (!definedDimensions) {
                    x = 0;
                    y = 0;
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

				update();
                updateValues();

                if (followTarget != nullptr) {
                    if (followTarget->isDestroyed) {
                        stopFollow();
                    }
                    else {
                        float tx = (float)followTarget->x;
                        float ty = (float)followTarget->y;

                        float cx = (width/zoomX)/2 - (width/oldZoomX)/2;
                        float cy = (height/zoomY)/2 - (height/oldZoomY)/2;

                        float nx = (oldCenterX - tx) * (1 - lerpX) + tx;
                        float ny = (oldCenterY - ty) * (1 - lerpY) + ty;
                        
                        centerOn(nx, ny);
                    }
                }

                updateValues();
                recordValues();

                Amara::Actor::run();
			}

            void updateValues() {
                fixValues();
                centerX = scrollX + (width/zoomX)/2;
                centerY = scrollY + (height/zoomY)/2;
            }

            void recordValues() {
                oldScrollX = scrollX;
                oldScrollY = scrollY;

                oldZoomX = zoomX;
                oldZoomY = zoomY;

                oldCenterX = centerX;
                oldCenterY = centerY;
            }

            void fixValues() {
                if (zoomX < 0) zoomX = 0.00001;
                if (zoomY < 0) zoomY = 0.00001;

                if (lockedToBounds) {
                    if (width/zoomX > boundX + boundW) {
                        scrollX = boundX - ((width/zoomX) - (boundX + boundW))/2;
                    }
                    else if (scrollX < boundX) {
                        scrollX = boundX;
                    }
                    else if (scrollX + width/zoomX > boundX + boundW) {
                        scrollX = (boundX + boundW) - (width/zoomX);
                    }

                    
                    if (height/zoomY > boundY + boundH) {
                        scrollY = boundY - ((height/zoomY) - (boundY + boundH))/2;
                    }
                    else if (scrollY < boundY) {
                        scrollY = boundY;
                    }
                    else if (scrollY + height/zoomY > boundY + boundH) {
                        scrollY = (boundY + boundH) - (height/zoomY);
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                int dx = 0, dy = 0, dw = 0, dh = 0, ow = 0, oh = 0;

                dx = vx + floor(x);
                if (dx < vx) {
                    ow = vx - dx;
                    dx = vx;
                }

                dy = vy + floor(y);
                if (dy < vy) {
                    oh = vy - dy;
                    dy = vy;
                }

                dw = (x + width > vw) ? ceil(vw - x) : width;
                dw -= ow;
                dh = (y + height > vh) ? ceil(vh - y) : height;
                dh -= oh;

                if (sceneEntities != nullptr) {
                    std::vector<Amara::Entity*>& rSceneEntities = *sceneEntities;
                    for (Amara::Entity* entity : rSceneEntities) {
                        assignAttributes();
                        if (entity->isDestroyed) continue;
                        entity->draw(dx, dy, dw, dh);
                    }
                }
            }

            void assignAttributes() {
                properties->currentCamera = this;
                properties->scrollX = scrollX + offsetX/zoomX;
                properties->scrollY = scrollY + offsetY/zoomY;
                properties->zoomX = zoomX;
                properties->zoomY = zoomY;
                properties->offsetX = 0;
                properties->offsetY = 0;
                properties->angle = 0;
            }

            void startFollow(Amara::Entity* entity, float lx, float ly) {
                followTarget = entity;
                lerpX = lx;
                lerpY = ly;
            }
            void startFollow(Amara::Entity* entity, float l) {
                startFollow(entity, l, l);
            }
            void startFollow(Amara::Entity* entity) {
                startFollow(entity, 1, 1);
            }

            void stopFollow() {
                followTarget = nullptr;
            }

            void centerOn(Amara::Entity* entity) {
                scrollX = entity->x - (width/zoomX)/2;
                scrollY = entity->y - (height/zoomY)/2;
                updateValues();
            }
            void centerOn(float gx, float gy) {
                scrollX = gx - (width/zoomX)/2;
                scrollY = gy - (height/zoomY)/2;
                updateValues();
            }
            void centerOn(float gi) {
                centerOn(gi, gi);
            }

            void setScroll(float gx, float gy) {
                scrollX = gx;
                scrollY = gy;
                updateValues();
            }
            void setScroll(float gi) {
                setScroll(gi, gi);
            }

            void changeScroll(float gx, float gy) {
                scrollX += gx;
                scrollY += gy;
                updateValues();
            }
            void changeScroll(float gi) {
                changeScroll(gi, gi);
            }

            void setZoom(float gx, float gy) {
                float cx = centerX;
                float cy = centerY;

                zoomX = gx;
                zoomY = gy;

                centerOn(centerX, centerY);
                updateValues();
            }
            void setZoom(float gi) {
                setZoom(gi, gi);
            }

            void changeZoom(float gx, float gy) {
                setZoom(zoomX + gx, zoomY + gy);
            }
            void changeZoom(float gi) {
                changeZoom(gi, gi);
            }

            void setBounds(float gx, float gy, float gw, float gh) {
                lockedToBounds = true;
                boundX = gx;
                boundY = gy;
                boundW = gw;
                boundH = gh;
            }

            void removeBounds() {
                lockedToBounds = false;
            }

            Amara::Script* scrollTo(float tx, float ty, double tt, Amara::Easing gEasing, bool center) {
                return recite(createTween_ScrollCamera(tx, ty, tt, gEasing, center));
            }
            Amara::Script* scrollTo(float tx, float ty, double tt, Amara::Easing gEasing) {
                return scrollTo(tx, ty, tt, gEasing, true);
            }
            Amara::Script* scrollTo(float tx, float ty, double tt) {
                return scrollTo(tx, ty, tt, LINEAR, true);
            }

            Amara::Script* zoomTo(float zt, float tt, Amara::Easing gEasing) {
                return recite(createTween_CameraZoom(zt, tt, gEasing));
            }
            Amara::Script* zoomTo(float zt, float tt) {
                return zoomTo(zt, tt, LINEAR);
            }

            ~Camera() {

            }
    };
}

#endif
