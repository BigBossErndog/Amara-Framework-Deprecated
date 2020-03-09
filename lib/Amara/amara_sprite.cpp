#ifndef AMARA_SPRITE
#define AMARA_SPRITE

#include "amara.h"

namespace Amara {
    class Sprite: public Amara::Image {
        public:
            Amara::AnimationManager anims;
            Sprite(int gx, int gy, string tx): Amara::Image(gx, gy, tx) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				properties = gameProperties;
				scene = givenScene;
                parent = givenParent;
                gRenderer = properties->gRenderer;

                anims.properties = properties;
                anims.parent = this;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                    textureKey.clear();
                }

				create();
			}

            virtual void play(string animKey) {
                anims.play(texture, animKey);
            }

            virtual void run() {
                anims.run();
                Amara::Entity::run();
            }
    };
}

#endif