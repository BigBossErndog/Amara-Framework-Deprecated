#pragma once
#ifndef AMARA_ACTORGROUP
#define AMARA_ACTORGROUP

#include "amara.h"

namespace Amara {
    class ActorGroup {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Scene* scene = nullptr;

            std::vector<Amara::Actor*> actors;

            ActorGroup(Amara::GameProperties* gProperties) {
                properties = gProperties;
            }

            ActorGroup(Amara::Actor* gActor) {
                properties = gActor->properties;
            }

            ActorGroup(Amara::Script* script) {
                properties = script->properties;
            }

            Amara::Actor* add(Amara::Actor* actor) {
                actors.push_back(actor);
                return actor;
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
            
            virtual void destroy() {
                destroyActors();
                delete this;
            }
    };
}

#endif