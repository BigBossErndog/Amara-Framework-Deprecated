#pragma once
#ifndef Amara_ANIMATIONMANAGER
#define Amara_ANIMATIONMANAGER

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

            float frameCounter = 0;

            AnimationManager(Amara::GameProperties* gameProperties, Amara::Image* givenParent) {
                properties = gameProperties;
                parent = givenParent;
            }

            bool play(Amara::ImageTexture* texture, std::string animKey) {
                if (texture == nullptr || texture->type != SPRITESHEET) {
                    return false;
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

                        isFinished = false;
                        isActive = true;
                        isPaused = false;

                        return true;
                    }
                }
                return false;
            }

            Amara::Animation* get(std::string animKey) {
                if (parent && parent->texture && parent->texture->type == SPRITESHEET) {
                    return ((Amara::Spritesheet*)parent->texture)->getAnim(animKey);
                }
                return nullptr;
            }

            bool playingAnim(std::string gKey) {
                if (isActive && currentAnim) {
                    if (gKey.compare(currentAnim->key) == 0) {
                        return true;
                    }
                }
                return false;
            }

            void stop() {
                currentAnim = nullptr;
            }

            float getProgress() {
                if (currentAnim == nullptr) {
                    return 1;
                }
                float frameDelay = properties->lps/(float)currentAnim->frameRate;
                float p = ((float)currentIndex)/((float)currentAnim->length());
                float e = (1/((float)currentAnim->length()));
                float f = (((float)frameCounter)/frameDelay);

                return p + e*f;
			}


			void setProgress(float p) {
				if (currentAnim == nullptr) {
					return;
				}
				float frameDelay = properties->lps/(float)currentAnim->frameRate;
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

                float frameDelay = properties->lps/(float)currentAnim->frameRate;
                frameCounter += 1;
                if (frameCounter >= frameDelay) {
                    currentIndex += 1;
                    if (currentIndex >= currentAnim->length()) {
                        if (currentAnim->loop) {
                            currentIndex = 0;
                            currentFrame = currentAnim->frameAt(currentIndex);
                        }
                        else {
                            currentFrame = currentAnim->frameAt(currentAnim->length() - 1);
                            currentAnim = nullptr;
                        }
                    }
                    else {
                        currentFrame = currentAnim->frameAt(currentIndex);
                    }
                    parent->frame = currentFrame;
                    frameCounter -= frameDelay;
                }
            }
    };
}

#endif
