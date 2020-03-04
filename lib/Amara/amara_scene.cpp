#ifndef AMARA_SCENE
#define AMARA_SCENE

#include "amara.h"

namespace Amara {
    class Game;
    class ScenePlugin;

    class Scene {
        public:
            Amara::Game* game = nullptr;
            Amara::Loader* load = nullptr;
            Amara::ScenePlugin* scene;

            Amara::Camera* mainCamera = nullptr;
            vector<Amara::Camera*> cameras;

            vector<Amara::Entity*> entities;

            Scene() {
                
            }

            virtual void setup(Amara::Game* givenGame, Amara::Loader* givenLoad, Amara::ScenePlugin* scenePlugin) final {
                game = givenGame;
                scene = scenePlugin;
                load = givenLoad;
            }

            void init() {
                Amara::Camera* cam;
                for (size_t i = 0; i < cameras.size(); i++) {
                    cam = cameras.at(i);
                    delete cam;
                }
                cameras.clear();
                mainCamera = nullptr;

                Amara::Entity* entity;
                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    delete entity;
                }
                entities.clear();

                mainCamera = new Amara::Camera();
                cameras.push_back(mainCamera);
                mainCamera->init(this, &entities);

                create();
            }

            Amara::Entity* add(Amara::Entity* entity) {
                entities.push_back(entity);
                entity->init(this);
            }

            void run() {
                update();
            }

            void draw() {
                sort(cameras.begin(), cameras.end(), sortEntities());
                sort(entities.begin(), entities.end(), sortEntities());

                Amara::Camera* cam;
                for (size_t i = 0; i < cameras.size(); i++) {
                    cam = cameras.at(i);
                    cam->draw();
                }
            }

            virtual void preload() {}
            virtual void create() {}
            virtual void update() {}
    };
}

#endif