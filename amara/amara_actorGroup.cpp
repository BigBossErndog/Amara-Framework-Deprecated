#pragma once
#ifndef AMARA_ACTORGROUP
#define AMARA_ACTORGROUP

#include "amara.h"

namespace Amara {
    class ActorGroup: public Amara::Script {
        public:
            std::vector<Amara::Actor*> actors;

            ActorGroup() {}

			int size() {
				return actors.size();
			}

			Actor* get(int index) {
				return actors[index];
			}

            Amara::Actor* add(Amara::Actor* actor) {
                actors.push_back(actor);
                return actor;
            }
            Amara::Actor* add(bool sceneAdd, Amara::Actor* actor) {
                if (sceneAdd && scene) {
                    scene->add(actor);
                }
                return add(actor);
            }

            virtual void move(int gx, int gy) {
                for (Amara::Actor* actor: actors) {
                    actor->x += gx;
                    actor->y += gy;
                }
            }

            virtual void apply(void(*callback)(Amara::Actor* actor)) {
                for (Amara::Actor* actor: actors) {
                    callback(actor);
                }
            }

            virtual void recite(Amara::Script* script) {
                for (Amara::Actor* actor: actors) {
                    actor->recite(script);
                }
            }

            void destroyActors() {
                for (Amara::Actor* actor: actors) {
                    actor->destroy();
                }
                actors.clear();
            }

            void destroy() {
                finish();
            }

            void script() {
                Amara::Actor* actor;
                for (auto it = actors.begin(); it != actors.end(); ++it) {
                    actor = *it;
                    if (actor->isDestroyed) {
                        actors.erase(it--);
                    }
                }
            }

            void finish() {
                Amara::Script::finish();
                destroyActors();
            }
    };
}

#endif
