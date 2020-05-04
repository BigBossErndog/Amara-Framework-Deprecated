#pragma once
#ifndef AMARA_ACTOR
#define AMARA_ACTOR

#include "amara.h"

namespace Amara {
    class Tween;
    class Actor: public Amara::Entity {
        public:
            std::vector<Amara::Script*> scripts;

            Actor(): Amara::Entity() {}

            virtual Amara::Script* recite(Amara::Script* script) {
                scripts.push_back(script);
                script->init(properties, this);
                script->prepare();
                script->prepare(this);
            }

            void reciteScripts() {
                Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end(); it++) {
                    script = *it;
                    script->script();
                    script->script(this);
                    if (script->finished) {
                        scripts.erase(it--);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
                    }
                }
            }

            bool stillActing() {
                return (scripts.size() > 0);
            }

            void run() {
                reciteScripts();
                Amara::Entity::run();
            }

            Amara::Entity* add(Amara::Entity* entity) {
                return Amara::Entity::add(entity);
            }

            ~Actor() {
                for (Amara::Script* script: scripts) {
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scripts.clear();
            }
    };
}

#endif
