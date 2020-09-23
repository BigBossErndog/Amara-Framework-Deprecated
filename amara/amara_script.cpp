#pragma once
#ifndef AMARA_SCRIPT
#define AMARA_SCRIPT

#include "amara.h"

namespace Amara {
    class Game;
    class Scene;
    class Actor;

    class Script: public Amara::StateManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Game* game = nullptr;
            Amara::Scene* scene = nullptr;
            Amara::Actor* parent = nullptr;
            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::AudioGroup* audio = nullptr;
            Amara::AssetManager* assets = nullptr;
            Amara::Loader* load = nullptr;

			std::string id;

            bool deleteOnFinish = true;

            Amara::Script* chainedScript = nullptr;

            Script(bool deleteWhenDone): Amara::StateManager() {
                deleteOnFinish = deleteWhenDone;
            }

            Script(): Script(true) {}

            Amara::Script* chain(Amara::Script* gScript) {
                chainedScript = gScript;
                return chainedScript;
            }

            Amara::Script* unchain() {
                Amara::Script* recScript = chainedScript;
                chainedScript = nullptr;
                return recScript;
            }

            virtual void init(Amara::GameProperties* gameProperties) {
                Amara::StateManager::properties = gameProperties;
                properties = gameProperties;
                game = properties->game;
                scene = properties->currentScene;
                input = properties->input;
                controls = properties->controls;
                audio = properties->audio;
                assets = properties->assets;
                load = properties->loader;

                finished = false;
                reset();
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Actor* parentActor) {
                init(gameProperties);
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

            virtual void prepare() {}
            virtual void prepare(Amara::Actor* actor) {}

            virtual void script() {}
            virtual void script(Amara::Actor* actor) {}

			virtual void cancel() {}
			virtual void cancel(Amara::Actor* actor) {}
    };
}

#endif
