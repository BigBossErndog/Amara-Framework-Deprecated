#ifndef AMARA_CAMERA
#define AMARA_CAMERA

#include "amara.h"

namespace Amara {
    class Scene;

    class Camera : public Amara::Entity {
        public:
            vector<Amara::Entity*>* sceneEntities = nullptr;

            float width = 0;
            float height = 0;

            float scrollX = 0;
            float scrollY = 0;

            float zoomX = 1;
            float zoomY = 1;

            Amara::Entity* followTarget = nullptr;
            float lerpX = 0;
            float lerpY = 0; 

            Camera() {

            }

            Camera(float gw, float gh) {
                width = gw;
                height = gh;
            }

            Camera(float gx, float gy, int gw, int gh) {
                x = gx;
                y = gy;
                width = gw;
                height = gh;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, vector<Amara::Entity*>* givenEntities) {
                properties = gameProperties;
                sceneEntities = givenEntities;
                width = properties->resolution->width;
                height = properties->resolution->height;
            }

            virtual void run() {
                if (followTarget != nullptr) {
                    float tx = followTarget->x - width/2;
                    float ty = followTarget->y - height/2;
                    scrollX = (scrollX - tx) * lerpX + tx;
                    scrollY = (scrollY - ty) * lerpY + ty;
                }

				update();

				Amara::Entity* entity;
				for (size_t i = 0; i < entities.size(); i++) {
					entity = entities.at(i);
					entity->run();
				}
			}

            virtual void draw(int vx, int vy, int vw, int vh) override {
                properties->currentCamera = this;

                int dx, dy, dw, dh, ow, oh = 0;

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

                Amara::Entity* entity;

                for (size_t i = 0; i < sceneEntities->size(); i++) {
                    entity = sceneEntities->at(i);
                    entity->draw(dx, dy, dw, dh);
                }

                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    entity->draw(vx, vy, vw, vh);
                }
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
                startFollow(entity, 0, 0);
            }

            void stopFollow() {
                followTarget = nullptr;
            }

            void centerOn(Amara::Entity* entity) {
                scrollX = entity->x - width/2;
                scrollY = entity->y - height/2;
            }

            ~Camera() {

            }
    };
}

#endif
