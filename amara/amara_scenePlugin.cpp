namespace Amara {
    class Game;

    enum SceneTask {
        SCENETASK_RUN,
        SCENETASK_STOP,
        SCENETASK_START,
        SCENETASK_RESTART,
        SCENETASK_PAUSE,
        SCENETASK_RESUME,
        SCENETASK_SLEEP,
        SCENETASK_WAKE,
		SCENETASK_BRINGTOFRONT
    };

    class ScenePlugin {
        public:
            Amara::Game* game = nullptr;
            Amara::GameProperties* properties = nullptr;
            Amara::Scene* scene = nullptr;

            std::string key;

            std::unordered_map<std::string, Amara::Scene*>* sceneMap = nullptr;
            std::list<Amara::Scene*>* sceneList = nullptr;

            std::vector<SceneTask> tasks;

            bool isActive = false;
            bool isPaused = false;
            bool isSleeping = false;

            bool reloadAssets = false;

            ScenePlugin(std::string givenKey, Amara::GameProperties* gameProperties, Amara::Scene* givenScene, std::unordered_map<std::string, Amara::Scene*>* givenSceneMap, std::list<Amara::Scene*>* givenSceneList) {
                key = givenKey;
                givenScene->id = key;

                properties = gameProperties;
                game = properties->game;

                scene = givenScene;
                scene->key = key;

                sceneMap = givenSceneMap;
                sceneList = givenSceneList;

                tasks.clear();
            }

            Amara::Scene* get(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    return got->second;
                }
                return nullptr;
            }

            bool has(std::string key) {
                return (get(key) != nullptr);
            }

            void run(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->run();
                }
                else {
					SDL_Log("Scene with key \"%s\" not found. Could not be run.", key.c_str());
				}
            }

            void start(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    stop();
                    got->second->scenes->start();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be started.", key.c_str());
				}
            }

            void stop(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->stop();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be stopped.", key.c_str());
				}
            }

			void pause(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->pause();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be paused.", key.c_str());
				}
            }

            void resume(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->resume();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be resumed.", key.c_str());
				}
            }

            void restart(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->restart();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be restarted.", key.c_str());
				}
            }

            void sleep(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->sleep();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be snoozed.", key.c_str());
				}
            }

            void wake(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->wake();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be woken.", key.c_str());
				}
            }

			void bringToFront(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->bringToFront();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be brought to front.", key.c_str());
				}
            }

			void run() {
                tasks.push_back(SCENETASK_RUN);
            }

			void start() {
                tasks.push_back(SCENETASK_START);
            }

            void stop() {
                tasks.push_back(SCENETASK_STOP);
            }

            void pause() {
                tasks.push_back(SCENETASK_PAUSE);
            }

            void resume() {
                tasks.push_back(SCENETASK_RESUME);
            }

            void restart() {
                tasks.push_back(SCENETASK_RESTART);
            }

            void sleep() {
                tasks.push_back(SCENETASK_SLEEP);
            }

            void wake() {
                tasks.push_back(SCENETASK_WAKE);
            }

			void bringToFront() {
				tasks.push_back(SCENETASK_BRINGTOFRONT);
			}
            
            void manageTasks() {
                for (SceneTask curTask: tasks) {
                    switch (curTask) {
                        case SCENETASK_RUN:
                            if (!isActive) {
                                scene->init();
                                scene->onStart();

                                if (scene->transition && scene->transition->endScene != scene) {
                                    scene->transition = nullptr;
                                }

                                if (properties->testing) SDL_Log("SCENE RUN: %s", key.c_str());
                            }
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            break;
                        case SCENETASK_START:
                            if (!isActive) {
                                scene->init();
                                scene->onStart();
                                properties->messages->clearMessagesOfScene(scene);

                                if (scene->transition && scene->transition->endScene != scene) {
                                    scene->transition = nullptr;
                                }
                                
                                if (properties->testing) SDL_Log("SCENE STARTED: %s", key.c_str());
                            }
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            break;
                        case SCENETASK_STOP:
                            if (isActive) {
                                scene->onStop();
                                properties->messages->clearMessagesOfScene(scene);
                                scene->destroyEntities();
                                scene->children.clear();
                                scene->clearScripts();

                                if (scene->transition && scene->transition->endScene != scene) {
                                    scene->transition = nullptr;
                                }

                                if (properties->testing) SDL_Log("SCENE STOPPED: %s", key.c_str());
                            }
                            isActive = false;
                            isPaused = false;
                            isSleeping = false;
                            break;
                        case SCENETASK_RESTART:
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            scene->clearScripts();
                            scene->onStop();
                            scene->destroyEntities();
                            scene->children.clear();
                            scene->init();
                            scene->onStart();
                            properties->messages->clearMessagesOfScene(scene);
                            if (properties->testing) SDL_Log("SCENE RESTARTED: %s", key.c_str());
                            break;
                        case SCENETASK_PAUSE:
                            if (isActive && !isPaused) {
                                isPaused = true;
                                isSleeping = false;
                                scene->onPause();
                                if (properties->testing) SDL_Log("SCENE PAUSED: %s", key.c_str());
                            }
                            break;
                        case SCENETASK_RESUME:
                            if (isActive && isPaused) {
                                isPaused = false;
                                isSleeping = false;
                                scene->onResume();
                                if (properties->testing) SDL_Log("SCENE RESUMED: %s", key.c_str());
                            }
                            break;
                        case SCENETASK_SLEEP:
                            if (isActive && !isSleeping) {
                                isPaused = false;
                                isSleeping = true;
                                scene->onSleep();

                                if (scene->transition && scene->transition->endScene != scene) {
                                    scene->transition = nullptr;
                                }

                                if (properties->testing) SDL_Log("SCENE PUT TO SLEEP: %s", key.c_str());
                            }
                            break;
                        case SCENETASK_WAKE:
                            if (isActive && isSleeping) {
                                isPaused = false;
                                isSleeping = false;
                                scene->onWake();
                                if (properties->testing) SDL_Log("SCENE WOKEN UP: %s", key.c_str());
                            }
                            break;
						case SCENETASK_BRINGTOFRONT:
							for (Scene* s: *sceneList) {
								if (scene->depth <= s->depth) {
									scene->depth = s->depth + 0.1;
								}
							}
							break;
                    }
                }
                tasks.clear();
                scene->isActive = isActive;
            }
    };

    void Scene::bringToFront() {
        scenes->bringToFront();
    }
}