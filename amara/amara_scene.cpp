#ifndef AMARA_SCENE
#define AMARA_SCENE

#include "amara.h"

namespace Amara {
    class Game;
    class ScenePlugin;
    class AssetManager;

    class Scene: public Amara::Actor {
        public:
            std::string key;
            Amara::Game* game = nullptr;
            Amara::LoadManager* load = nullptr;
            Amara::ScenePlugin* scenePlugin = nullptr;
            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::AssetManager* assets = nullptr;

            Amara::Camera* mainCamera = nullptr;
            std::vector<Amara::Camera*> cameras;

            bool initialLoaded = false;
            bool initialCreated = false;

            Scene(): Actor() {

            }

            virtual void setup(Amara::GameProperties* gameProperties, Amara::ScenePlugin* gScenePlugin) final {
                properties = gameProperties;

                game = properties->game;
                input = properties->input;
                controls = properties->controls;
                assets = properties->assets;
                audio = properties->audio;

                if (load != nullptr) {
                    delete load;
                }
                load = new Amara::LoadManager(properties);

                scenePlugin = gScenePlugin;
            }

            void init() {
                initialLoaded = false;
                
                load->reset();

                for (Amara::Camera* cam : cameras) {
                    delete cam;
                }
                cameras.clear();
                mainCamera = nullptr;

                for (Amara::Entity* entity : entities) {
                    delete entity;
                }
                entities.clear();

                mainCamera = new Amara::Camera();
                cameras.push_back(mainCamera);
                mainCamera->init(properties, this, this, &entities);

                preload();
                std::cout << "START LOADING TASKS: " << load->tasks.size() << " loading tasks." << std::endl;
            }

            Amara::Entity* add(Amara::Entity* entity) {
                entities.push_back(entity);
                entity->init(properties, this, this);
                return entity;
            }

            Amara::Camera* add(Amara::Camera* cam) {
                cameras.push_back(cam);
                cam->init(properties, this, this, &entities);
                return cam;
            }

            void run() {
                properties->currentScene = this;

                if (!initialLoaded) {
                    load->run();
                    if (!load->stillLoading) {
                        initialLoaded = true;
                        create();
                    }
                }
                else {
                    update();
                    reciteScripts();
                    
                    for (Amara::Entity* entity : entities) {
                        if (entity->isDestroyed || entity->parent != this) continue;
                        entity->run();
                    }

                    for (Amara::Camera* cam : cameras) {
                        if (cam->isDestroyed || cam->parent != this) continue;
                        cam->run();
                    }
                }
            }

            void draw() {
                properties->currentScene = this;
				properties->scrollX = 0;
				properties->scrollY = 0;

                stable_sort(cameras.begin(), cameras.end(), sortEntities());
                stable_sort(entities.begin(), entities.end(), sortEntities());

                float offset, upScale;
                int vx, vy = 0;
                float ratioRes = ((float)properties->resolution->width / (float)properties->resolution->height);
                float ratioWin = ((float)properties->window->width / (float)properties->window->height);
                
                if (ratioRes < ratioWin) {
                    upScale = ((float)properties->window->height/(float)properties->resolution->height);
                    offset = ((float)properties->window->width - ((float)properties->resolution->width * upScale))/2;
                    vx += offset/upScale;
                }
                else if (ratioRes > ratioWin) {
                    upScale = ((float)properties->window->width/(float)properties->resolution->width);
                    offset = ((float)properties->window->height - ((float)properties->resolution->height * upScale))/2;
                    vy += offset/upScale;
                }
                
                for (Amara::Camera* cam : cameras) {
                    if (cam->isDestroyed || cam->parent != this) continue;
                    using namespace std;
                    cam->draw(vx, vy, properties->resolution->width, properties->resolution->height);
                }
            }

            virtual void preload() {}
            virtual void create() {}
            virtual void update() {}

            virtual void onStart() {}
            virtual void onStop() {}
            virtual void onPause() {}
            virtual void onResume() {}
            virtual void onSleep() {}
            virtual void onWake() {}

            ~Scene() {
                delete load;
            }
    };
}

#endif
