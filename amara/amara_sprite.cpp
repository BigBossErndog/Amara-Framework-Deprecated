#ifndef AMARA_SPRITE
#define AMARA_SPRITE

#include "amara.h"

namespace Amara {
    class Sprite: public Amara::Image, public Amara::Animated {
        public:
            Sprite(int gx, int gy, std::string tx): Amara::Image(gx, gy, tx) {}
            Sprite(): Amara::Image() {};

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                anims = new Amara::AnimationManager(gameProperties, this);
                Amara::Image::init(gameProperties, givenScene, givenParent);
			}

            virtual void play(std::string animKey) {
                anims->play(texture, animKey);
            }

            virtual void run() {
                Amara::Image::run();
                anims->run();
            }
    };
}

#endif
