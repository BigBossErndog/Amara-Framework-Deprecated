#ifndef AMARA_SCENEPLUGIN
#define AMARA_SCENEPLUGIN

#include "amara.h"

namespace Amara {
    class Game;

    enum SceneTask {
        STOP,
        START,
        PAUSE,
        RESUME,
        SLEEP,
        WAKE
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

            void start() {
                tasks.push_back(START);
            }

            void start(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    stop(key);
                    got->second->scenePlugin->start();
                }
            }

            void stop(std::string key) {
                std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap->find(key);
                if (got != sceneMap->end()) {
                    got->second->scenePlugin->stop();
                }
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

            void sleep() {
                tasks.push_back(SLEEP);
            }

            void wake() {
                tasks.push_back(WAKE);
            }

            void manageTasks() {
                for (size_t i = 0; i < tasks.size(); i++) {
                    SceneTask curTask = tasks.at(i);
                    switch (curTask) {
                        case START:
                            isActive = true;
                            isPaused = false;
                            isSleeping = false;
                            scene->init();
                            break;
                        case STOP:
                            isActive = false;
                            isPaused = false;
                            isSleeping = false;
                            break;
                        case PAUSE:
                            if (isActive && !isPaused) {
                                isPaused = true;
                                isSleeping = false;
                            }
                            break;
                        case RESUME:
                            if (isActive && isPaused) {
                                isPaused = false;
                                isSleeping = false;
                            }
                            break;
                        case SLEEP:
                            if (isActive && !isSleeping) {
                                isPaused = false;
                                isSleeping = true;
                            }
                            break;
                        case WAKE:
                            if (isActive && isSleeping) {
                                isPaused = false;
                                isSleeping = false;
                            }
                            break;
                    }
                }
                tasks.clear();
            }
    };
}

#endif
