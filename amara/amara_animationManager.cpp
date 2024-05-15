namespace Amara {
    class AnimationManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::AssetManager* assets = nullptr;
            Amara::Image* parent = nullptr;

            Amara::Animation* currentAnim = nullptr;
            bool isFinished = true;
            bool isActive = false;
            bool isPaused = false;
            int currentIndex = 0;
            int currentFrame = 0;

            float frameCounter = 0;

            AnimationManager() {}
            AnimationManager(Amara::GameProperties* gameProperties, Amara::Image* givenParent) {
                properties = gameProperties;
                parent = givenParent;
                assets = properties->assets;
            }

            bool play(Amara::ImageTexture* texture, std::string animKey) {
                if (texture == nullptr || texture->type != SPRITESHEET) {
                    return false;
                }
                Amara::Animation* anim = assets->getAnimation(parent->texture->key, animKey);
                if (anim == nullptr) {
                    SDL_Log("Spritesheet \"%s\" does not have the animation \"%s\".", texture->key.c_str(), animKey.c_str());
                }
                if (anim != currentAnim || (anim != nullptr && isFinished)) {
					if (currentAnim != nullptr && currentAnim != anim && currentAnim->deleteOnFinish) {
                        properties->taskManager->queueObject(currentAnim);
                    }
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
					else {
						isFinished = true;
						isActive = false;
						isPaused = false;
					}
                }
                return false;
            }
			void play(Amara::ImageTexture* texture, Amara::Animation* anim) {
				if (anim != currentAnim || (anim != nullptr && isFinished)) {
					if (currentAnim != nullptr && currentAnim != anim && currentAnim->deleteOnFinish && currentAnim != anim) {
                        properties->taskManager->queueObject(currentAnim);
                    }
                    currentAnim = anim;

                    currentIndex = 0;
                    frameCounter = 0;

                    if (anim != nullptr) {
						currentAnim->deleteOnFinish = true;
                        currentFrame = anim->frameAt(currentIndex);
                        parent->frame = currentFrame;

                        isFinished = false;
                        isActive = true;
                        isPaused = false;
                    }
					else {
						isFinished = true;
						isActive = false;
						isPaused = false;
					}
                }
			}

            Amara::Animation* get(std::string animKey) {
                if (parent && parent->texture && parent->texture->type == SPRITESHEET) {
                    return assets->getAnimation(parent->texture->key, animKey);
                }
                return nullptr;
            }

            bool isPlayingAnimation(std::string gKey) {
                if (isActive && currentAnim) {
                    if (gKey.compare(currentAnim->key) == 0) {
                        return true;
                    }
                }
                return false;
            }

            void stop() {
				if (currentAnim != nullptr && currentAnim->deleteOnFinish) {
                    properties->taskManager->queueObject(currentAnim);
                }
                currentAnim = nullptr;
				isFinished = true;
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

            void syncWith(AnimationManager& other) {
                setProgress(other.getProgress());
            }

            void pause() {
                isPaused = true;
                isActive = false;
            }
            void resume() {
                isPaused = false;
                if (currentAnim != nullptr) isActive = true;
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
                    currentIndex += floor(frameCounter/frameDelay);
                    if (currentIndex >= currentAnim->length()) {
                        if (currentAnim->loop) {
                            currentIndex = 0;
                            currentFrame = currentAnim->frameAt(currentIndex);
                        }
                        else {
                            currentFrame = currentAnim->frameAt(currentAnim->length() - 1);
							if (currentAnim != nullptr && currentAnim->deleteOnFinish) {
                                properties->taskManager->queueObject(currentAnim);
                            }
                            currentAnim = nullptr;
							isFinished = true;
							isActive = false;
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

    class Animated {
        public:
            Amara::AnimationManager anims;

            Animated() {}

            virtual bool play(std::string key) {}
			virtual void play(Amara::Animation* anim) {}
    };
}
