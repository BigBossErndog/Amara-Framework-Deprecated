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
            std::list<Amara::Camera*> cameras;

            bool initialLoaded = false;

            bool sortCameras = false;
            bool stableSortCameras = false;

            using Actor::Actor;

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
                    properties->taskManager->queueDeletion(loadManager);
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

                destroyEntities();
                mainCamera = nullptr;

                add(mainCamera = new Amara::Camera());
                preload();
                SDL_Log("START LOADING TASKS: %d loading tasks.", load->numberOfTasks);

                entityType = "scene";
            }

            using Amara::Actor::add;
            virtual Amara::Camera* add(Amara::Camera* cam) {
                cam->depth = cameras.size() * 0.001;
                cameras.push_back(cam);
                cam->init(properties, this, this);
                cam->sceneCameras = &cameras;
                return cam;
            }

			virtual Amara::Entity* removeCamera(Amara::Camera* rem) {
				Amara::Camera* cam;
                if (mainCamera == rem) mainCamera = nullptr;
				for (auto it = cameras.begin(); it != cameras.end();) {
                    cam = *it;
                    if (cam == rem) {
                        it = cameras.erase(it);
                        return cam;
                    }
                    ++it;
                }
				return nullptr;
			}

            virtual void manageScene() {
                if (transition != nullptr) {
                    transition->run();

                    if (transition != nullptr && transition->endScene == this) {
                        if (transition->isFinished) {
                            initialLoaded = true;
                            transition->complete();
                            transition = nullptr;
                        }
                        else if (transition->waitingForPermission) {
                            transition->grantPermission();
                        }
                    }

                    if (transition == nullptr || 
                        (transition->startScene == this && !transition->waitingForPermission && !transition->permissionGranted) || 
                        (transition->endScene == this && transition->permissionGranted)
                    ) {
                        if (!initialLoaded) {
                            load->run();
                            if (!load->stillLoading) {
                                initialLoaded = true;
                                setLoader(properties->loader);
                                create();
                            }
                            else whileLoading();
                        }
                        else updateScene();
                    }
                }
                else if (!initialLoaded) {
                    load->run();
                    if (!load->stillLoading) {
                        initialLoaded = true;
                        setLoader(properties->loader);
                        create();
                    }
                    else whileLoading();
                }
                else updateScene();
            }

            virtual void run() {
                properties->currentScene = this;
				properties->currentCamera = mainCamera;

                manageScene();
            }

            virtual void updateScene() {
                debugID = id;
                std::string debugCopy;
                if (debugging) {
                    debugID = "";
                    for (int i = 0; i < properties->entityDepth; i++) debugID += "\t";
                    debugID += id;
                    SDL_Log("%s (%s): Running. - depth %d", debugID.c_str(), entityType.c_str(), properties->entityDepth);
                    debugCopy = debugID;
                }

                receiveMessages();
                updateMessages();
                
                properties->entityDepth = 0;
                properties->scrollX = 0;
                properties->scrollY = 0;
                properties->offsetX = 0;
                properties->offsetY = 0;
                properties->zoomX = 1;
                properties->zoomY = 1;
                properties->zoomFactorX = 1;
                properties->zoomFactorY = 1;
                properties->angle = 0;
                properties->alpha = 1;
                
                update();

                if (debugging) SDL_Log("%s (%s): Reciting Scripts (%d).", debugCopy.c_str(), entityType.c_str(), scripts.size());
                reciteScripts();
                
                runChildren();
                checkChildren();

                Amara::Camera* cam;
                for (auto it = cameras.begin(); it != cameras.end(); ++it) {
                    cam = *it;
                    if (cam == nullptr || cam->isDestroyed || cam->parent != this) {
                        continue;
                    }
                    else {
                        cam->run();
                    }
                }

                if (debugging) SDL_Log("%s (%s): Finished Running.", debugCopy.c_str(), entityType.c_str());
            }

            virtual void draw() {
                properties->currentScene = this;
				properties->currentCamera = mainCamera;
				properties->scrollX = 0;
				properties->scrollY = 0;

                if (sortCameras) cameras.sort(sortEntitiesByDepth());

                if (shouldSortChildren || sortChildrenOnce) {
                    sortChildrenOnce = false;
                    delayedSorting();
                }

                float offset, upScale;
                int vx = 0, vy = 0;
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
                for (auto it = cameras.begin(); it != cameras.end();) {
                    cam = *it;
                    if (cam == nullptr || cam->isDestroyed || cam->parent != this) {
                        it = cameras.erase(it);
                        continue;
                    }
                    cam->transition = transition;
					SDL_SetRenderTarget(properties->gRenderer, NULL);

					properties->interactOffsetX = 0;
					properties->interactOffsetY = 0;
                    properties->interactScaleX = 1;
                    properties->interactScaleY = 1;

                    cam->draw(vx, vy, properties->resolution->width, properties->resolution->height);
                    ++it;
                }
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
                for (Amara::Camera* cam: cameras) {
                    if (cam == nullptr || cam->isDestroyed || cam->parent != this) {
                        continue;
                    }
                    cam->destroy();
                }
                cameras.clear();
                mainCamera = nullptr;
                Amara::Actor::destroyEntities();
            }

            virtual void preload() {}
            virtual void whileLoading() {}
            virtual void create() {}
            virtual void update() {}

            virtual void onStart() {}
            virtual void onStop() {}
            virtual void onPause() {}
            virtual void onResume() {}
            virtual void onSleep() {}
            virtual void onWake() {}

            void bringToFront();
    };
}
