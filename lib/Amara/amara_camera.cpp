#ifndef AMARA_CAMERA
#define AMARA_CAMERA

#include "amara.h"

namespace Amara {
    class Scene;

    class Camera : public Amara::Entity {
        public:
            vector<Amara::Entity*>* sceneEntities = nullptr;

            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;

            int scrollX = 0;
            int scrollY = 0;

            Camera() {
                
            }

            Camera(int gw, int gh) {
                width = gw;
                height = gh;
            }

            Camera(int gx, int gy, int gw, int gh) {
                x = gx;
                y = gy;
                width = gw;
                height = gh;
            }

            virtual void init(Amara::Scene* givenScene, vector<Amara::Entity*>* givenEntities) {
                sceneEntities = givenEntities;
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                properties->currentCamera = this;
                
                int dx, dy, dw, dh = 0;
                dx = vx + x;
                dy = vy + y;
                dw = (x + width > vw) ? (vw - x) : vw;
                dh = (y + height > vh) ? (vh - y) : vh;

                Amara::Entity* entity;
                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    entity->draw(dx, dy, dw, dh);
                }
            }

            ~Camera() {

            }
    };
}

#endif