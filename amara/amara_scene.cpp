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
            Amara::ScenePlugin* scenes = nullptr;

            Amara::SceneTransitionBase* transition = nullptr;

            Amara::Camera* mainCamera = nullptr;
            std::vector<Amara::Camera*> cameras;

            bool initialLoaded = false;

            Scene(): Actor() {

            }

            virtual void setup(Amara::GameProperties* gameProperties, Amara::ScenePlugin* gScenePlugin) final {
                properties = gameProperties;
				game = properties->game;

				input = properties->input;
				controls = properties->controls;
				audio = properties->audio;
                assets = properties->assets;
                messages = properties->messages;

                scene = this;

                if (loadManager != nullptr) {
                    delete loadManager;
                }
                loadManager = new Amara::LoadManager(properties);
                setLoader(loadManager);

                transition = nullptr;

                scenes = gScenePlugin;
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

                std::vector<Entity*> toDestroy = entities;
                for (Amara::Entity* entity: toDestroy) {
                    if (entity->isDestroyed || entity->scene != this) continue;
                    entity->destroy();
                }
                entities.clear();

                add(mainCamera = new Amara::Camera());
                preload();
                std::cout << "START LOADING TASKS: " << load->numTasks() << " loading tasks." << std::endl;

                entityType = "scene";
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
                receiveMessages();
                updateMessages();

                if (!initialLoaded) {
                    if (transition != nullptr) {
                        transition->run();
                    }

                    load->run();
					
                    if (!load->stillLoading) {
                        if (transition != nullptr) {
                            if (transition->finished) {
                                initialLoaded = true;
                                transition->complete();
                                transition = nullptr;
                            }
                            else if (transition->waitingForPermission) {
                                transition->grantPermission();
                                setLoader(properties->loader);
                                create();
                            }
                            else if (transition->permissionGranted) {
                                updateScene();
                            }
                        }
                        else {
                            initialLoaded = true;

                            setLoader(properties->loader);
                            create();
                        }
                    }
                }
                else {
                    updateScene();
                    if (transition != nullptr) {
                        transition->run();
                        if (transition && transition->fromWake) {
                            if (transition->finished) {
                                transition->complete();
                                transition = nullptr;
                            }
                            else if (transition->waitingForPermission) {
                                transition->grantPermission();
                            }
                        }
                    }
                }
            }

            virtual void updateScene() {
                update();
                reciteScripts();

                Amara::Entity* entity;
                for (auto it = entities.begin(); it != entities.end(); ++it) {
                    entity = *it;
                    if (entity->isDestroyed || entity->parent != this) {
                        entities.erase(it--);
                    }
                    else {
                        entity->run();
                        if (entity->isDestroyed || entity->parent != this) {
                            entities.erase(it--);
                        }
                    }
                }  

                Amara::Camera* cam;
                for (auto it = cameras.begin(); it != cameras.end(); ++it) {
                    cam = *it;
                    if (cam->isDestroyed || cam->parent != this) {
                        cameras.erase(it--);
                    }
                    else {
                        cam->run();
                    }
                }

                afterUpdate();
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
                    cam->transition = transition;
                    cam->draw(vx, vy, properties->resolution->width, properties->resolution->height);
                }

                afterDraw();
            }

            virtual Amara::SceneTransitionBase* startTransition(Amara::SceneTransitionBase* gTransition) {
                if (transition != nullptr) {
                    return nullptr;
                }
                transition = gTransition;
                transition->init(properties, this);
                return transition;
            }

            virtual void removeTransition() {
                if (transition != nullptr && !initialLoaded && !load->stillLoading && transition->permissionGranted) {
                    transition->finish();
                    transition->complete();
                    transition = nullptr;
                    initialLoaded = true;
                }
            }

            virtual void destroyEntities() {
                Amara::Entity::destroyEntities();
                for (Amara::Camera* cam: cameras) {
                    delete cam;
                }
                cameras.clear();
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
