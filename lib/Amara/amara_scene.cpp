#ifndef AMARA_SCENE
#define AMARA_SCENE

#include "amara.h"

namespace Amara {
    class Game;
    class ScenePlugin;

    class Scene {
        public:
            Amara::Game* game = nullptr;
            Amara::GameProperties* properties = nullptr;
            Amara::LoadManager* load = nullptr;
            Amara::ScenePlugin* scene;

            Amara::Camera* mainCamera = nullptr;
            vector<Amara::Camera*> cameras;

            vector<Amara::Entity*> entities;

            Scene() {
                
            }

            virtual void setup(Amara::GameProperties* gameProperties, Amara::ScenePlugin* scenePlugin) final {
                properties = gameProperties;

                game = properties->game;
                load = properties->loadManager;

                scene = scenePlugin;
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
                entity->init(properties, this);
            }

            Amara::Camera* add(Amara::Camera* cam) {
                cameras.push_back(cam);
                cam->init(this, &entities);
            }

            void run() {
                properties->currentScene = this;

                update();
            }

            void draw() {
                properties->currentScene = this;

                stable_sort(cameras.begin(), cameras.end(), sortEntities());
                stable_sort(entities.begin(), entities.end(), sortEntities());

                Amara::Camera* cam;
                for (size_t i = 0; i < cameras.size(); i++) {
                    cam = cameras.at(i);
                    cam->draw(0, 0, properties->width, properties->height);
                }
            }

            virtual void preload() {}
            virtual void create() {}
            virtual void update() {}
    };
}

#endif