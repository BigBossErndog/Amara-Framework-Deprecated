namespace Amara {
    class Tween;
    class Actor: public Amara::Entity {
    public:
        std::vector<Amara::Script*> scripts;
        std::vector<Amara::Script*> scriptCopyList;
        bool actingPaused = false;
        bool scriptsCanceled = false;

        Actor(): Amara::Entity() {}

        using Amara::Entity::init;
        void init() {
            scripts.clear();
            Amara::Entity::init();
            entityType = "actor";
        }

        nlohmann::json toData() {
            nlohmann::json config = Amara::Entity::toData();
            config["numberOfScripts"] = scripts.size();
            return config;
        }

        Amara::Actor* getActor(std::string key) {
            return (Amara::Actor*)get(key);
        }

        virtual Amara::Script* recite(Amara::Script* script) {
            if (script == nullptr) {
                SDL_Log("Amara Actor: Null script was given to recite.");
                return nullptr;
            }
            if (script->inQueue) {
                SDL_Log("Amara Actor: Wait until script is dequeued before reciting.");
                return script;
            }
            script->init(properties, this);
            if (isDestroyed) {
                SDL_Log("Amara Actor: A dead actor cannot recite scripts.");
                return script;
            }
            scripts.push_back(script);
            script->prepare();
            script->inQueue = true;
            return script;
        }

        virtual Amara::Script* chain(Amara::Script* script) {
            if (script == nullptr) {
                SDL_Log("Amara Actor: Null script was given to actor chain.");
                return nullptr;
            }
            if (scripts.size() > 0) {
                Amara::Script* lastScript = scripts.back();
                return lastScript->chain(script);
            }
            return recite(script);
        }

        virtual void reciteScripts() {
            if (scripts.size() == 0 || isDestroyed || actingPaused) return;
            debugCopy = "";
            if (debugging) {
                debugID = "";
                for (int i = 0; i < properties->entityDepth; i++) debugID += "\t";
                debugID += id;
                debugCopy = debugID;
                SDL_Log("wtf");
                SDL_Log("%s (%s): Reciting %d Scripts.", debugCopy.c_str(), entityType.c_str(), scripts.size());
            }
            
            Amara::Script* script = nullptr;
            scriptsCanceled = false;
            
            scriptCopyList = scripts;

            for (auto it = scriptCopyList.begin(); it != scriptCopyList.end();) {
                script = *it;
                if (script->isDestroyed) {
                    ++it;
                    continue;
                }
                if (!script->isFinished) {
                    script->receiveMessages();
                    script->updateMessages();
                    if (debugging) SDL_Log("%s (%s): enter script recital", debugCopy.c_str(), entityType.c_str());
                    script->script();
                    if (debugging) SDL_Log("%s (%s): exit script recital", debugCopy.c_str(), entityType.c_str());
                }
                if (isDestroyed || scriptsCanceled) break;
                ++it;
            }

            if (isDestroyed || scriptsCanceled) {
                return;
            }

            std::vector<Script*> chained;
            chained.clear();

            for (auto it = scripts.begin(); it != scripts.end();) {
                script = *it;
                if (script->isDestroyed) {
                    script->inQueue = false;
                    it = scripts.erase(it);
                    continue;
                }
                else if (script->isFinished) {
                    if (!script->endConfig.is_null()) configure(script->endConfig);
                    if (!isDestroyed && !script->isDestroyed && script->chainedScripts.size() > 0) {
                        for (Amara::Script* chainedScript: script->chainedScripts) {
                                chained.push_back(chainedScript);
                        }
                        script->chainedScripts.clear();
                    }
                    script->inQueue = false;
                    if (!script->manualDeletion) {
                        if (debugging) SDL_Log("%s (%s): deleting script", debugCopy.c_str(), entityType.c_str());
                        script->destroyScript();
                    }
                    it = scripts.erase(it);
                    continue;
                }
                ++it;
            }

            if (isDestroyed || scriptsCanceled) {
                return;
            }

            if (debugging && chained.size() > 0) SDL_Log("%s (%s): reciting %d chained scripts", debugCopy.c_str(), entityType.c_str(), chained.size());
            for (Amara::Script* chain: chained) {
                recite(chain);
                if (isDestroyed || scriptsCanceled) break;
            }

            if (debugging) if (debugging) SDL_Log("%s (%s): end reciting scripts", debugCopy.c_str(), entityType.c_str());
        }

        bool stillActing() {
            return (scripts.size() > 0);
        }
        
        bool isActing() { return stillActing(); }

        bool notActing() {
            return !stillActing();
        }

        bool isReciting(Amara::Script* script) {
            for (Amara::Script* check: scripts) {
                if (check == script) return true;
            }
            return false;
        }

        void run() {
			debugID = id;
			debugCopy = "";
			if (debugging) {
				debugID = "";
				for (int i = 0; i < properties->entityDepth; i++) debugID += "\t";
				debugID += id;
				SDL_Log("%s (%s): Running. - depth %d", debugID.c_str(), entityType.c_str(), properties->entityDepth);
				debugCopy = debugID;
			}
            
            receiveMessages();
            updateMessages();
            if (isDestroyed) return;

            Amara::Interactable::run();
            if (isInteractable && interact.isDraggable && interact.isDown) {
				interact.isBeingDragged = true;
				if (physics && !physics->isDestroyed) {
					physics->velocity.x = interact.movementX;
					physics->velocity.y = interact.movementY;
				}
				else {
					x += interact.movementX;
					y += interact.movementY;
				}
			}
			else interact.isBeingDragged = false;

            update();
            if (isDestroyed) return;

            if (physics != nullptr) {
                if (physics->isActive) physics->run();
                
                if (physics->isDestroyed) {
                    removePhysics();
                }
            }

            if (attachedTo != nullptr) {
                if (attachedTo->isDestroyed) {
                    attachedTo = nullptr;
                }
                else {
                    x = attachedTo->x + attachmentOffsetX;
                    y = attachedTo->y + attachmentOffsetY;
                }
            }
            
            reciteScripts();
            if (isDestroyed) return;
            
            runChildren();
            
            if (debugging) SDL_Log("%s (%s): Finished Running.", debugCopy.c_str(), entityType.c_str());
        }

        using Amara::Entity::destroy;
        
        virtual void destroy(bool recursiveDestroy) {
            if (isDestroyed) return;
            clearScripts();
            Amara::Entity::destroy(recursiveDestroy);
        }

        Amara::Script* getScript(std::string gid) {
            for (Amara::Script* script: scripts) {
                if (script->id.compare(gid) == 0) {
                    return script;
                }
            }
            return nullptr;
        }

        Amara::Actor* clearScripts() {
            for (Amara::Script* script: scripts) {
                script->inQueue = false;
                if (!script->manualDeletion) script->destroyScript();
            }
            scripts.clear();
            scriptsCanceled = true;
            return this;
        }

        Amara::Actor* cancelScripts() {
            for (Amara::Script* script: scripts) {
                script->cancel();
                script->inQueue = false;
                if (!script->manualDeletion) script->destroyScript();
            }
            scripts.clear();
            scriptsCanceled = true;
            return this;
        }



        Amara::Actor* pauseActing() {
            actingPaused = true;
            return this;
        }
        Amara::Actor* resumeActing() {
            actingPaused = false;
            return this;
        }
    };
}