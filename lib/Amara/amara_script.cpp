#ifndef AMARA_SCRIPT
#define AMARA_SCRIPT

#include "amara.h"

namespace Amara {
    class Game;
    class Scene;

    class Script: Amara::StateManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Entity* parent = nullptr;
            Amara::Scene* scene = nullptr;
            Amara::Game* game = nullptr;
            Amara::InputManager* input = nullptr;

            bool deleteOnFinish = true;

            Script(bool deleteWhenDone): Amara::StateManager() {
                deleteOnFinish = deleteWhenDone;
            }

            Script(): Script(true) {}

            void init(Amara::GameProperties* gameProperties, Amara::Entity* parentActor) {
                properties = gameProperties;
                game = properties->game;
                scene = properties->currentScene;
                input = properties->input;

                parent = parentActor;
            }

            bool finished = false;
            
            virtual void finish() {
                finished = true;
            }
            void finishEvt() {
                if (evt()) {
                    finish();
                }
            }

            virtual void prepare(Amara::Entity* entity) {}
            virtual void script(Amara::Entity* entity) {}
    };
}

#endif