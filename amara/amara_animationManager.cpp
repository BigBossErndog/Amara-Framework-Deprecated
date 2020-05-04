#pragma once
#ifndef Amara_ANIMATIONMANAGER
#define AMARA_ANIMATIONMANAGER

#include "amara.h"

namespace Amara {
    class AnimationManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Image* parent = nullptr;

            Amara::Animation* currentAnim = nullptr;
            bool isFinished = false;
            bool isActive = false;
            bool isPaused = false;
            int currentIndex = 0;
            int currentFrame = 0;

            int frameCounter = 0;

            AnimationManager(Amara::GameProperties* gameProperties, Amara::Image* givenParent) {
                properties = gameProperties;
                parent = givenParent;
            }

            void play(Amara::ImageTexture* texture, std::string animKey) {
                if (texture == nullptr || texture->type != SPRITESHEET) {
                    return;
                }
                Amara::Animation* anim = ((Amara::Spritesheet*)texture)->getAnim(animKey);
                if (anim == nullptr) {
                    std::cout << "Spritesheet \"" << texture->key << "\" does not have the animation \"" << animKey << "\"." << std::endl;
                }
                if (anim != currentAnim || (anim != nullptr && isFinished)) {
                    currentAnim = anim;

                    currentIndex = 0;
                    frameCounter = 0;

                    if (anim != nullptr) {
                        currentFrame = anim->frameAt(currentIndex);
                        parent->frame = currentFrame;
                    }

                    isFinished = false;
                    isActive = true;
                    isPaused = false;
                }
            }

            void stop() {
                currentAnim = nullptr;
            }

            float getProgress() {
                if (currentAnim == nullptr) {
                    return 1;
                }
                float frameDelay = properties->lps/currentAnim->frameRate;
                float p = ((float)currentIndex)/((float)currentAnim->length());
                float e = (1/((float)currentAnim->length()));
                float f = (((float)frameCounter)/frameDelay);

                return p + e*f;
			}


			void setProgress(float p) {
				if (currentAnim == nullptr) {
					return;
				}
				float frameDelay = properties->lps/currentAnim->frameRate;
				currentIndex = floor(p*currentAnim->length());
                float oldP = ((float)currentIndex)/((float)currentAnim->length());
                float e = 1/((float)currentAnim->length());
				frameCounter = floor(((p - oldP)/e)*frameDelay);
				currentFrame = currentAnim->frameAt(currentIndex);
				parent->frame = currentFrame;
			}

            void syncWith(Amara::Animated* other) {
                setProgress(other->anims->getProgress());
            }

            void pause() {
                isPaused = true;
            }
            void resume() {
                isPaused = false;
            }

            void run() {
                if (currentAnim == nullptr || isPaused) {
                    isActive = false;
                    return;
                }
                isActive = true;

                float frameDelay = properties->lps/currentAnim->frameRate;
                frameCounter += 1;
                if (frameCounter >= frameDelay) {
                    currentIndex += 1;
                    if (currentIndex >= currentAnim->length()) {
                        if (currentAnim->loop) {
                            currentIndex = 0;
                            currentFrame = currentAnim->frameAt(currentIndex);
                        }
                        else {
                            currentAnim = nullptr;
                        }
                    }
                    else {
                        currentFrame = currentAnim->frameAt(currentIndex);
                    }
                    parent->frame = currentFrame;
                    frameCounter = 0;
                }
            }
    };
}

#endif
