#ifndef AMARA_CAMERA
#define AMARA_CAMERA

#include "amara.h"

namespace Amara {
    class Scene;

    class Camera : public Amara::Actor {
        public:
            std::vector<Amara::Camera*>* sceneCameras = nullptr;

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
            float zoomScale = 1;

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

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* gParent) {
                properties = gameProperties;
                parent = gParent;
                scene = givenScene;

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
                    if (followTarget->isDestroyed || !followTarget->isActive) {
                        stopFollow();
                    }
                    else {
                        float tx = (float)followTarget->x;
                        float ty = (float)followTarget->y;

                        float cx = (width/(zoomX*zoomScale))/2 - (width/(oldZoomX*zoomScale))/2;
                        float cy = (height/(zoomY*zoomScale))/2 - (height/(oldZoomY*zoomScale))/2;

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
                centerX = scrollX + (width/(zoomX*zoomScale))/2;
                centerY = scrollY + (height/(zoomY*zoomScale))/2;
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
                    if (width/(zoomX*zoomScale) > boundX + boundW) {
                        scrollX = boundX - ((width/(zoomX*zoomScale)) - (boundX + boundW))/2;
                    }
                    else if (scrollX < boundX) {
                        scrollX = boundX;
                    }
                    else if (scrollX + width/(zoomX*zoomScale) > boundX + boundW) {
                        scrollX = (boundX + boundW) - (width/(zoomX*zoomScale));
                    }

                    
                    if (height/(zoomY*zoomScale) > boundY + boundH) {
                        scrollY = boundY - ((height/(zoomY*zoomScale)) - (boundY + boundH))/2;
                    }
                    else if (scrollY < boundY) {
                        scrollY = boundY;
                    }
                    else if (scrollY + height/(zoomY*zoomScale) > boundY + boundH) {
                        scrollY = (boundY + boundH) - (height/(zoomY*zoomScale));
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                if (!isVisible) return;
                int dx = 0, dy = 0, dw = 0, dh = 0, ow = 0, oh = 0;

                dx = vx + floor(x);
                if (dx < vx) {
                    ow = vx - dx;
                    dx = vx;
                }

                dy = vy + floor(y-z);
                if (dy < vy) {
                    oh = vy - dy;
                    dy = vy;
                }

                dw = (x + width > vw) ? ceil(vw - x) : width;
                dw -= ow;
                dh = (y + height > vh) ? ceil(vh - y) : height;
                dh -= oh;

                std::vector<Amara::Entity*>& rSceneEntities = parent->entities;
                Amara::Entity* entity;
                for (std::vector<Amara::Entity*>::iterator it = rSceneEntities.begin(); it != rSceneEntities.end(); it++) {
                    entity = *it;
                    if (entity->isDestroyed || entity->scene != scene) {
                        rSceneEntities.erase(it--);
                    }
                    if (!entity->isVisible) continue;
                    assignAttributes();
                    entity->draw(dx, dy, dw, dh);
                }
            }

            void assignAttributes() {
                properties->currentCamera = this;
                properties->scrollX = scrollX + offsetX/(zoomX*zoomScale);
                properties->scrollY = scrollY + offsetY/(zoomY*zoomScale);
                properties->zoomX = zoomX * zoomScale;
                properties->zoomY = zoomY * zoomScale;
                properties->offsetX = 0;
                properties->offsetY = 0;
                properties->angle = 0;
                properties->zoomFactorX = 1;
                properties->zoomFactorY = 1;
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
                scrollX = entity->x - (width/(zoomX*zoomScale))/2;
                scrollY = entity->y - (height/(zoomY*zoomScale))/2;
                updateValues();
            }
            void centerOn(float gx, float gy) {
                scrollX = gx - (width/(zoomX*zoomScale))/2;
                scrollY = gy - (height/(zoomY*zoomScale))/2;
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

            void setZoomScale(float gZoomScale) {
                zoomScale = gZoomScale;
            }

            void setZoom(float gx, float gy, float gZoomScale) {
                float cx = centerX;
                float cy = centerY;

                setZoomScale(gZoomScale);
                zoomX = gx;
                zoomY = gy;

                centerOn(centerX, centerY);
                updateValues();
            }

            void setZoom(float gx, float gy) {
                setZoom(gx, gy, zoomScale);
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

            virtual void bringToFront() {
                if (sceneCameras == nullptr) return;
                std::vector<Amara::Camera*>& rSceneCameras = *sceneCameras;
				for (Amara::Camera* cam: rSceneCameras) {
					if (depth <= cam->depth) {
						depth = cam->depth + 1;
					}
				}
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
