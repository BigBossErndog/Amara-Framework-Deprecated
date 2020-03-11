#ifndef AMARA_SPRITE
#define AMARA_SPRITE

#include "amara.h"

namespace Amara {
    class Sprite: public Amara::Image, public Amara::Animated {
        public:
            Sprite(int gx, int gy, string tx): Amara::Image(gx, gy, tx) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                Amara::Interactable::init(gameProperties);
                
				properties = gameProperties;
				scene = givenScene;
                parent = givenParent;
                gRenderer = properties->gRenderer;
                
                anims = new Amara::AnimationManager(properties, this);

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                    textureKey.clear();
                }

				create();
			}

            virtual void play(string animKey) {
                anims->play(texture, animKey);
            }

            virtual void run() {
                Amara::Image::run();
                anims->run();
            }
    };
}

#endif
