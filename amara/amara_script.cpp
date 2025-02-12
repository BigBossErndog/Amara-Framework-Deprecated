namespace Amara {
    class Game;
    class Scene;
    class Actor;

    class Script: public Amara::StateManager, public Amara::Broadcaster {
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
            Amara::TaskManager* tasks = nullptr;
            Amara::Loader* load = nullptr;

			std::string id;

            bool manualDeletion = false;
            bool deleteChained = true;
            bool inQueue = false;
            bool isDestroyed = false;

			bool initiated = false;
            
            nlohmann::json endConfig = nullptr;

            std::vector<Amara::Script*> chainedScripts;

            Script(bool gManual): Amara::StateManager() {
                manualDeletion = gManual;
            }

            Script(): Script(false) {}

            Amara::Script* chain(Amara::Script* gScript, bool parallel) {
                if (gScript == nullptr) {
                    SDL_Log("Amara Actor: Null script was given to script chain.");
                    return nullptr;
                }
                if (parallel || chainedScripts.size() == 0) chainedScripts.push_back(gScript);
                else {
                    Amara:Script* last = chainedScripts.back();
                    if (last) last->chain(gScript);
                }
                return gScript;
            }

            Amara::Script* chain(Amara::Script* gScript) {
                return chain(gScript, false);
            }

            Amara::Script* parallel(Amara::Script* gScript) {
                chain(gScript, true);
                return this;
            }

            std::vector<Amara::Script*> unchain() {
                std::vector<Amara::Script*> recScripts = chainedScripts;
                chainedScripts.clear();
                return recScripts;
            }

            void destroyChains() {
                if (chainedScripts.size() > 0) {
                    for (Amara::Script* chainedScript: chainedScripts) {
                        chainedScript->properties = properties;
                        if (!chainedScript->manualDeletion) chainedScript->destroyScript();
                    }
                    chainedScripts.clear();
                }
            }

            Amara::Script* thenConfigure(nlohmann::json config) {
                if (endConfig.is_null()) endConfig = nlohmann::json::object();
                endConfig.update(config);
                return this;
            }

            Amara::Script* thenConfigure(std::string key, nlohmann::json val) {
                nlohmann::json config = nlohmann::json::object();
                config[key] = val;
                return thenConfigure(config);
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
                tasks = properties->taskManager;

                initiated = true;
                isFinished = false;
                
                reset();
                init();
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Actor* parentActor) {
                parent = parentActor;
                parentEntity = (Amara::Entity*)parent;
                Amara::StateManager::init(gameProperties);
                init(gameProperties);
            }

            virtual void init() {}

            bool notActing() {
                return isFinished && !inQueue;
            }
            bool isActing() {
                return !notActing();
            }

            bool isFinished = false;

            virtual void finish() {
                if (pushedMessages) fixMessages();
                isFinished = true;
            }
            bool finishEvt() {
                if (evt()) {
                    finish();
                    return true;
                }
                return false;
            }

            virtual void prepare() {}
            virtual void script() {}
			virtual void cancel() {}

            virtual void destroyScript() {
                if (isDestroyed) return;
                if (pushedMessages) fixMessages();
                isDestroyed = true;
                if (deleteChained) {
                    destroyChains();
                }
                if (properties) properties->taskManager->queueScript(this);
            }

            void fixMessages() {
                if (!messages) return;
                pushedMessages = false;
                for (Amara::Message& msg: messages->queue) {
                    if (msg.parent == this) {
                        msg.parent = (Amara::Broadcaster*)parentEntity;
                        parentEntity->pushedMessages = true;
                    }
                }
            }
    };

    bool StateManager::waitOnScript(Amara::Script* script) {
        if (evt()) {
            if (script) {
                script->manualDeletion = true;
                if (nextEvtOn(script->notActing())) {
                    script->destroyScript();
                }
            }
            else nextEvt();
            return true;
        }
        return false;
    }

    void TaskManager::queueScript(Amara::Script* script) {
		if (script == nullptr) return;
		script->isDestroyed = true;
		scriptBuffer.push_back(script);
	}
}
