#ifndef Amara_AnimationManager
#define Amara_AnimationManager

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

            void play(Amara::ImageTexture* texture, string animKey) {
                if (texture == nullptr || texture->type != SPRITESHEET) {
                    return;
                }
                Amara::Animation* anim = ((Amara::Spritesheet*)texture)->getAnim(animKey);
                if (anim == nullptr) {
                    cout << "Spritesheet \"" << texture->key << "\" does not have the animation \"" << animKey << "\"." << endl; 
                }
                if (anim != currentAnim) {
                    currentAnim = anim;

                    currentIndex = 0;
                    frameCounter = 0;

                    if (anim != nullptr) {
                        currentFrame = anim->frameAt(currentIndex);
                        parent->frame = currentFrame;
                    }
                }
                isFinished = false;
                isActive = true;
                isPaused = false;
            }

            void stop() {
                currentAnim = nullptr;
            }

            float getProgress() {
                if (currentAnim == nullptr) {
                    return 1;
                }
                float frameDelay = properties->fps/currentAnim->frameRate;
                return (currentIndex/currentAnim->length()) + (frameCounter/frameDelay); 
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

                float frameDelay = properties->fps/currentAnim->frameRate;
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
                else {
                    frameCounter += 1;
                }
            }
    };
}

#endif
