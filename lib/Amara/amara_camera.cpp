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

            virtual void init(Amara::Scene* givenScene, vector<Amara::Entity*>* givenEntities) {
                sceneEntities = givenEntities;
            }

            virtual void drawSelf() {
                
            }

            virtual void draw() override {
                Amara::Entity* entity;
                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    entity->draw();
                }
            }

            ~Camera() {

            }
    };
}

#endif