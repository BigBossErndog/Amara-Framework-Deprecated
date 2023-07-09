namespace Amara {
    class Tween;
    class Actor: public Amara::Entity {
        private:
            bool inRecital = false;
            std::list<Amara::Script*> scriptBuffer;
        public:
            std::list<Amara::Script*> scripts;
            bool actingPaused = false;

            Actor(): Amara::Entity() {}

            using Amara::Entity::init;
            void init() {
                Amara::Entity::init();
                entityType = "actor";
            }

            Amara::Actor* getActor(std::string key) {
                return (Amara::Actor*)get(key);
            }

			void destroyScript(Amara::Script* script) {
				if (script != nullptr) {
					Script* check;
					for (auto it = scripts.begin(); it != scripts.end();) {
						check = *it;
						if (check == script) {
							it = scripts.erase(it);
							continue;
						}
                        ++it;
					}
					if (script->deleteOnFinish) properties->taskManager->queueDeletion(script);
					if (script->chainedScript) destroyScript(script->chainedScript);
				} 
			}

            virtual Amara::Script* recite(Amara::Script* script) {
				if (script == nullptr) {
					SDL_Log("Null script was given.");
					return nullptr;
				}
				if (isDestroyed) {
					destroyScript(script);
					return script;
				}
                if (!inRecital) {
                    scripts.push_back(script);
                }
                else {
                    scriptBuffer.push_back(script);
                }
                script->init(properties, this);
                script->prepare();
                script->prepare(this);
                return script;
            }

            virtual Amara::Script* chain(Amara::Script* script) {
                if (scriptBuffer.size() > 0) {
                    Amara::Script* lastScript = scriptBuffer.back();
                    return lastScript->chain(script);
                }
                if (scripts.size() > 0) {
                    Amara::Script* lastScript = scripts.back();
                    return lastScript->chain(script);
                }
                return recite(script);
            }

            virtual void reciteScripts() {
                if (scripts.size() == 0 || actingPaused) return;
                
                inRecital = true;

                for (Amara::Script* script: scripts) {
                    if (!script->isFinished) {
                        script->receiveMessages();
                        script->updateMessages();
                        script->script();
                        script->script(this);
                    }
					if (isDestroyed) {
						clearScripts();
                        inRecital = false;
						return;
					}
                }

                std::vector<Script*> chained;
                chained.clear();
                
                Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end();) {
                    script = *it;
                    if (script->isFinished) {
                        if (script->chainedScript != nullptr) {
                            chained.push_back(script->chainedScript);
                            script->chainedScript = nullptr;
                        }
                        it = scripts.erase(it);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
                        continue;
                    }
                    ++it;
                }
                for (Amara::Script* chain: chained) {
                    recite(chain);
                }
                inRecital = false;
                pipeScriptBuffer();
            }

            void pipeScriptBuffer() {
                for (Amara::Script* script: scriptBuffer) {
                    scripts.push_back(script);
                }
                scriptBuffer.clear();
            }

            bool stillActing() {
                return (scripts.size() > 0 || scriptBuffer.size() > 0);
            }

            bool isActing() { return stillActing(); }

			bool notActing() {
				return !stillActing();
			}

            bool isReciting(Amara::Script* script) {
                for (Amara::Script* check: scripts) {
                    if (check == script) return true;
                }
                for (Amara::Script* check: scriptBuffer) {
                    if (check == script) return true;
                }
                return false;
            }

            void run() {
                receiveMessages();
				updateMessages();

				Amara::Interactable::run();
				if (isInteractable && isDraggable && interact.isDown) {
					if (physics) {
						physics->velocityX = interact.movementX;
						physics->velocityY = interact.movementY;
					}
					else {
						x += interact.movementX;
						y += interact.movementY;
					}
				}

				update();

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

				runChildren();
				checkChildren();
            }

            Amara::Actor* clearScripts() {
                for (Amara::Script* script: scripts) {
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scripts.clear();
                for (Amara::Script* script: scriptBuffer) {
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scriptBuffer.clear();
                return this;
            }

			Amara::Actor* clearScript(std::string gid) {
				Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end();) {
                    script = *it;
                    if (script->id.compare(gid) == 0) {
                        if (script->chainedScript != nullptr) {
                            recite(script->chainedScript);
                        }
                        it = scripts.erase(it);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
						break;
                    }
                    ++it;
                }
                for (auto it = scriptBuffer.begin(); it != scriptBuffer.end();) {
                    script = *it;
                    if (script->id.compare(gid) == 0) {
                        if (script->chainedScript != nullptr) {
                            recite(script->chainedScript);
                        }
                        it = scripts.erase(it);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
						break;
                    }
                    ++it;
                }
                return this;
			}

			Amara::Script* getScript(std::string gid) {
				for (Amara::Script* script: scripts) {
					if (script->id.compare(gid) == 0) {
						return script;
					}
				}
				return nullptr;
			}

			Amara::Actor* cancelScripts() {
				for (Amara::Script* script: scripts) {
					script->cancel();
					script->cancel(this);
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scripts.clear();
                for (Amara::Script* script: scriptBuffer) {
					script->cancel();
					script->cancel(this);
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scriptBuffer.clear();

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

            void destroy() {
                clearScripts();
                Amara::Entity::destroy();
            }
    };
}