#pragma once
#ifndef AMARA_SCENETRANSITION
#define AMARA_SCENETRANSITION

#include "amara.h"

namespace Amara {
    class SceneTransition: public Amara::SceneTransitionBase {
        public:
            SceneTransition(): Amara::SceneTransitionBase() {}
            SceneTransition(std::string gNextScene): Amara::SceneTransitionBase(gNextScene) {}

            bool startNextScene() {
                if (once()) {
                    if (sleepScene) {
                        startScene->scenes->sleep();
                    }
                    else {
                        startScene->scenes->stop();
                    }

                    if (startScene != endScene) {
                        startScene->transition = nullptr;
                    }
    
                    if (endScene != nullptr) {
                        endScene->transition = this;
                        if (wakeScene) {
                            endScene->scenes->wake();
                            fromWake = true;
                        }
                        else {
                            endScene->scenes->start();
                        }
                    }
                    return true;
                }
                return false;
            }

            virtual void configure(nlohmann::json config) {
                Amara::SceneTransitionBase::configure(config);

                if (config.find("nextScene") != config.end()) {
                    nextSceneKey = config["nextScene"];
                    if (endScene) {
                        endScene->transition = nullptr;
                    }
                    if (sceneManager) {
                        endScene = sceneManager->get(nextSceneKey);
                        if (endScene != nullptr) {
                            endScene->transition = this;
                        }
                    }
                }
            }

            virtual void init(Amara::GameProperties* gProperties,Amara::Scene* gStartScene) {
                Amara::SceneTransitionBase::init(gProperties, gStartScene, this);
                sceneManager = Amara::Actor::properties->scenes;

                startScene = gStartScene;
                startScene->transition = this;

                endScene = sceneManager->get(nextSceneKey);
                fromWake = false;

                finished = false;
                reset();

                create();
            }
    };
}

#endif