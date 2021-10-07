#pragma once
#ifndef AMARA_SCENEPLUGIN
#define AMARA_SCENEPLUGIN

#include "amara.h"

namespace Amara {
    class Game;

    enum SceneTask {
        RUN,
        STOP,
        START,
        RESTART,
        PAUSE,
        RESUME,
        SLEEP,
        WAKE,
		BRINGTOFRONT
    };

    class ScenePlugin {
        public:
            Amara::Game* game = nullptr;
            Amara::GameProperties* properties = nullptr;
            Amara::Scene* scene = nullptr;

            std::string key;

            std::unordered_map<std::string, Amara::Scene*>* sceneMap = nullptr;
            std::vector<Amara::Scene*>* sceneList = nullptr;

            std::vector<SceneTask> tasks;

            bool isActive = false;
            bool isPaused = false;
            bool isSleeping = false;

            ScenePlugin(std::string givenKey, Amara::GameProperties* gameProperties, Amara::Scene* givenScene, std::unordered_map<std::string, Amara::Scene*>* givenSceneMap, std::vector<Amara::Scene*>* givenSceneList) {
                key = givenKey;
                givenScene->id = key;

                properties = gameProperties;
                game = properties->game;

                scene = givenScene;

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

            void run(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->run();
                }
                else {
					SDL_Log("Scene with key \"%s\" not found. Could not be run.", key);
				}
            }

            void start(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    stop(this->key);
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
					SDL_Log("Scene with key \"%s\" not found. Could not be paused.", key);
				}
            }

            void resume(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->resume();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be resumed.", key);
				}
            }

            void restart(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->restart();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be restarted.", key);
				}
            }

            void sleep(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->sleep();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be snoozed.", key);
				}
            }

            void wake(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->wake();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be woken.", key);
				}
            }

			void bringToFront(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenes->bringToFront();
                }
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be brought to front.", key);
				}
            }

			void run() {
                tasks.push_back(RUN);
            }

			void start() {
                tasks.push_back(START);
            }

            void stop() {
                tasks.push_back(STOP);
            }

            void pause() {
                tasks.push_back(PAUSE);
            }

            void resume() {
                tasks.push_back(RESUME);
            }

            void restart() {
                tasks.push_back(RESTART);
            }

            void sleep() {
                tasks.push_back(SLEEP);
            }

            void wake() {
                tasks.push_back(WAKE);
            }

			void bringToFront() {
				tasks.push_back(BRINGTOFRONT);
			}

            void manageTasks() {
                for (size_t i = 0; i < tasks.size(); i++) {
                    SceneTask curTask = tasks.at(i);
                    switch (curTask) {
                        case RUN:
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            scene->init();
                            scene->onStart();
                            break;
                        case START:
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            scene->init();
                            scene->onStart();
                            break;
                        case STOP:
                            isActive = false;
                            isPaused = false;
                            isSleeping = false;
                            scene->clearScripts();
                            scene->onStop();
                            scene->destroyEntities();
                            break;
                        case RESTART:
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            scene->clearScripts();
                            scene->onStop();
                            scene->init();
                            scene->onStart();
                            break;
                        case PAUSE:
                            if (isActive && !isPaused) {
                                isPaused = true;
                                isSleeping = false;
                                scene->onStart();
                            }
                            break;
                        case RESUME:
                            if (isActive && isPaused) {
                                isPaused = false;
                                isSleeping = false;
                                scene->onResume();
                            }
                            break;
                        case SLEEP:
                            if (isActive && !isSleeping) {
                                isPaused = false;
                                isSleeping = true;
                                scene->onSleep();
                            }
                            break;
                        case WAKE:
                            if (isActive && isSleeping) {
                                isPaused = false;
                                isSleeping = false;
                                scene->onWake();
                            }
                            break;
						case BRINGTOFRONT:
							for (int i = 0; i < sceneList->size(); i++) {
								Scene* s = sceneList->at(i);
								if (s == scene) {
									sceneList->erase(sceneList->begin() + i);
									sceneList->push_back(s);
									break;
								}
							}
							break;
                    }
                }
                tasks.clear();
                scene->isActive = isActive;
            }
    };
}

#endif
