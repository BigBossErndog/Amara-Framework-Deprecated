#pragma once
#ifndef AMARA_ANIMATION
#define AMARA_ANIMATION

#include "amara.h"

namespace Amara {
    class AnimationManager;

    class Animation {
        public:
            std::string texture;
            std::string key;

            std::vector<int> frames;
            int frameRate = 1;
            bool loop;

			bool deleteOnFinish = false;

			Animation(std::vector<int> givenFrames, int gFrameRate, bool gLoop) {
				frames = givenFrames;
                frameRate = gFrameRate;
                loop = gLoop;
			}
			Animation(int frame): Animation({frame}, 1, false) {}
			Animation(int startFrame, int numFrames, int gFrameRate, bool gLoop) {
				frameRate = gFrameRate;
				loop = gLoop;
				frames.clear();
				int lastFrame = startFrame + numFrames;
				for (int i = startFrame; i < lastFrame; i++) {
					frames.push_back(i);
				}
			}

            Animation(std::string tx, std::string id, std::vector<int> givenFrames, int gFrameRate, bool gLoop): Animation(givenFrames, gFrameRate, gLoop)  {
                texture = tx;
                key = id;
            }

            Animation(std::string tx, std::string id, int frame): Animation(tx, id, {frame}, 1, false) {}

            int frameAt(int index) {
                return frames.at(index);
            }

            int length() {
                return frames.size();
            }
    };

    class Animated {
        public:
            Amara::AnimationManager* anims = nullptr;

            virtual bool play(std::string key) {}
			virtual void play(Amara::Animation* anim) {}
    };
}

#endif