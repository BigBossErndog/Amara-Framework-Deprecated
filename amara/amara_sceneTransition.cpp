#pragma once
#ifndef AMARA_SCENETRANSITION
#define AMARA_SCENETRANSITION

#include "amara.h"

namespace Amara {
    class SceneTransition: public Amara::SceneTransitionBase {
        public:
            SceneTransition(std::string gNextScene): Amara::SceneTransitionBase(gNextScene) {}

            bool startNextScene() {
                if (once()) {
                    if (sleepScene) {
                        startScene->scenePlugin->sleep();
                    }
                    else {
                        startScene->scenePlugin->stop();
                    }

                    if (startScene != endScene) {
                        startScene->transition = nullptr;
                    }
    
                    if (endScene != nullptr) {
                        if (wakeScene) {
                            endScene->scenePlugin->wake();
                        }
                        else {
                            endScene->scenePlugin->start();
                        }
                    }
                    return true;
                }
                return false;
            }

            virtual void init(Amara::GameProperties* gProperties,Amara::Scene* gStartScene) {
                Amara::SceneTransitionBase::init(gProperties, gStartScene, this);
                sceneManager = Amara::Actor::properties->scenes;

                startScene = gStartScene;
                startScene->transition = this;

                endScene = sceneManager->get(nextSceneKey);
                if (endScene != nullptr) {
                    endScene->transition = this;
                }

                sleepScene = false;
                wakeScene = false;

                finished = false;
                reset();

                create();
            }
    };
}

#endif