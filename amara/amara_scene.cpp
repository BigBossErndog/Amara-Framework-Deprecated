#pragma once
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
            Amara::LoadManager* loadManager = nullptr;
            Amara::ScenePlugin* scenePlugin = nullptr;

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
				audio = properties->audio;
                assets = properties->assets;

                scene = this;

                if (loadManager != nullptr) {
                    delete loadManager;
                }
                loadManager = new Amara::LoadManager(properties);
                setLoader(loadManager);

                scenePlugin = gScenePlugin;
                isActive = false;
            }

            virtual void init() {
                initialLoaded = false; 

                setLoader(loadManager);
                load->reset();

                for (Amara::Camera* cam : cameras) {
                    delete cam;
                }
                cameras.clear();
                mainCamera = nullptr;

                for (Amara::Entity* entity : entities) {
                    entity->destroy();
                }
                entities.clear();
                
                add(mainCamera = new Amara::Camera());
                preload();
                std::cout << "START LOADING TASKS: " << load->numTasks() << " loading tasks." << std::endl;
            }

            virtual Amara::Entity* add(Amara::Entity* entity) {
                entities.push_back(entity);
                entity->init(properties, this, this);
                return entity;
            }

            virtual Amara::Camera* add(Amara::Camera* cam) {
                cameras.push_back(cam);
                cam->init(properties, this, this);
                cam->sceneCameras = &cameras;
                return cam;
            }

            virtual Amara::Entity* removeCamera(size_t index) {
				Amara::Entity* child = entities.at(index);
				child->parent = nullptr;
				cameras.erase(cameras.begin() + index);
				return child;
			}

			virtual Amara::Entity* removeCamera(Amara::Camera* entity) {
				Amara::Camera* child;
				int numChildren = cameras.size();
				for (size_t i = 0; i < numChildren; i++) {
					child = cameras.at(i);
					if (child == entity) {
						return removeCamera(i);
					}
				}
				return nullptr;
			}

            virtual void run() {
                properties->currentScene = this;

                if (!initialLoaded) {
                    load->run();
                    if (!load->stillLoading) {
                        initialLoaded = true;
                        setLoader(properties->loader);
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

                    afterUpdate();
                }
            }

            virtual void draw() {
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

                Amara::Camera* cam;
                for (std::vector<Amara::Camera*>::iterator it = cameras.begin(); it != cameras.end(); it++) {
                    cam = *it;
                    if (cam->isDestroyed || cam->parent != this) {
                        cameras.erase(it--);
                    }
                    cam->draw(vx, vy, properties->resolution->width, properties->resolution->height);
                }

                afterDraw();
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

            virtual void afterUpdate() {}
            virtual void afterDraw() {}

            ~Scene() {
                delete load;
            }
    };
}

#endif
