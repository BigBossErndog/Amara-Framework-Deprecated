#pragma once
#ifndef AMARA_SCRIPT
#define AMARA_SCRIPT



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
            Amara::Entity* parentEntity = nullptr;
            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::AudioGroup* audio = nullptr;
            Amara::AssetManager* assets = nullptr;
            Amara::Loader* load = nullptr;
            Amara::MessageQueue* messages = nullptr;

			std::string id;

            bool deleteOnFinish = true;
            bool deleteChainOnDelete = true;

			bool initiated = false;

            Amara::Script* chainedScript = nullptr;

            Script(bool deleteWhenDone): Amara::StateManager() {
                deleteOnFinish = deleteWhenDone;
            }

            Script(): Script(true) {}

            Amara::Script* chain(Amara::Script* gScript) {
                if (chainedScript) chainedScript->chain(gScript);
                else chainedScript = gScript;
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
                if (parentEntity) scene = parentEntity->scene;
                else scene = properties->currentScene;
                input = properties->input;
                controls = properties->controls;
                audio = properties->audio;
                assets = properties->assets;
                load = properties->loader;
                messages = properties->messages;

                isFinished = false;
                reset();
                init();
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Actor* parentActor) {
                parent = parentActor;
                parentEntity = (Amara::Entity*)parent;
                initiated = true;
                init(gameProperties);
            }

            virtual void init() {}

            bool isFinished = false;

            virtual void finish() {
                isFinished = true;
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

            bool pushedMessages = false;

            void updateMessages() {
				if (pushedMessages) {
					pushedMessages = false;
					Amara::MessageQueue& messages = *(properties->messages);
					for (auto it = messages.begin(); it != messages.end();) {
						Message msg = *it;
						if (msg.parent == this) {
							if (msg.skip) {
								msg.skip = false;
								pushedMessages = true;
							}
							else {
								it = messages.queue.erase(it);
								continue;
							}
						}
						++it;
					}
				}
			}
			Message& broadcastMessage(std::string key, nlohmann::json gData) {
				pushedMessages = true;
				return properties->messages->broadcast(this, key, gData);
			}
			Message& broadcastMessage(std::string key) {
				return broadcastMessage(key, nullptr);
			}
			Message& getMessage(std::string key) {
				return properties->messages->get(key);
			}
			virtual void receiveMessages() {}

            ~Script() {
                if (deleteChainOnDelete && chainedScript) {
                    delete chainedScript;
                }
            }
    };
}

#endif
