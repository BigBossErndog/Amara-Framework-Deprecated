#pragma once
#ifndef AMARA_SCENETRANSITIONBASE
#define AMARA_SCENETRANSITIONBASE



namespace Amara {
    class Scene;
    class SceneManager;

    class SceneTransitionBase: public Amara::StateManager, public Amara::Actor {
        public:
            Amara::Scene* startScene = nullptr;
            Amara::Scene* endScene = nullptr;
            Amara::SceneManager* sceneManager = nullptr;

            std::string nextSceneKey;

            bool waitingForPermission = false;
            bool permissionGranted = false;
            bool finished = false;

            bool sleepScene = false;
            bool wakeScene = false;

            bool fromWake = false;

            SceneTransitionBase() {}

            SceneTransitionBase(std::string gNextScene) {
                nextSceneKey = gNextScene;
            }

            bool waitForPermission() {
                bool toReturn = false;
                if (once()) {
                    waitingForPermission = true;
                    permissionGranted = false;
                    toReturn = true;
                }
                if (evt()) {
                    if (permissionGranted) {
                        waitingForPermission = false;
                        nextEvt();
                    }
                    toReturn = true;
                }
                return toReturn;
            }

            virtual void grantPermission() {
                permissionGranted = true;
            }

            virtual void finish() {
                finished = true;
            }
            bool finishEvt() {
                if (once()) {
                    finish();
                    return true;
                }
                return false;
            }

            virtual void configure(nlohmann::json config) {
                if (config.find("sleepScene") != config.end()) {
                    sleepScene = config["sleepScene"];
                }
                if (config.find("wakeScene") != config.end()) {
                    wakeScene = config["wakeScene"];
                }
            }

            virtual void init(Amara::GameProperties* gProperties, Amara::Scene* gScene) {}
            virtual void init(Amara::GameProperties* gProperties, Amara::Scene* gScene, Amara::Entity* gParent) {
                permissionGranted = false;
                waitingForPermission = false;
                Amara::Actor::init(gProperties, gScene, gParent);
            }

            virtual bool startNextScene() {}
            virtual void draw(int vx, int vy, int vw, int vh) {
                resetPassOnProperties();
                Amara::Actor::draw(vx, vy, vw, vh);
            }

            virtual void complete() {
                Amara::Actor::properties->taskManager->queueDeletion(this);
				destroyEntities();
            }

            virtual ~SceneTransitionBase() {}
    };
}

#endif
