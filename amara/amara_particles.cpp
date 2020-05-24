#pragma once
#ifndef AMARA_PARTICLES
#define AMARA_PARTICLES

#include "amara.h"

namespace Amara {
    class Particle: public Amara::Image, public Amara::StateManager {
        public:
            bool inUse = false;

            bool finish() {
                inUse = false;
            }

            bool finishEvt() {
                if (evt()) {
                    inUse = false;
                }
            }
    };

    class ParticleSystem: public Amara::Actor {
        public:
            std::string textureKey;
            std::vector<Particle> particles;
        
            ParticleSystem(std::string gTextureKey, int gNumber, int startX, int startY) {
                Amara::Particle particle;
                particles.resize(gNumber, particle);
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                Amara::Actor::init(gameProperties, givenScene, givenParent);

                for (Amara::Particle& particle: particles) {
                    particle.init(gameProperties, givenScene, this);
                    particle.setTexture(textureKey);
                }
            }

            int size() {
                return particles.size();
            }

            Amara::Particle* newParticle(float gox, float goy) {
                for (Amara::Particle& particle: particles) {
                    if (!particle.inUse) {
                        particle.inUse = true;
                        particle.x = x + gox;
                        particle.y = y + goy;
                        particle.reset();
                        particle.setTexture(textureKey);
                        createParticle(&particle);
                        return &particle;
                    }
                }
            } 

            Amara::Particle* newParticle() {
                return newParticle(0, 0);
            }

            virtual void createParticle(Amara::Particle* particle) {}
            virtual void updateParticle(Amara::Particle* particle) {}

            void run() {
                for (Amara::Particle& particle: particles) {
                    if (particle.inUse) {
                        updateParticle(&particle);
                    }
                }
            }

            void draw(int vx, int vy, int vw, int vh) {
                for (Amara::Particle& particle: particles) {
                    if (particle.inUse) {
                        particle.draw(vx, vy, vw, vh);
                    }
                }
                Amara::Entity::draw(vx, vy, vw, vh);
            }
    };
}

#endif