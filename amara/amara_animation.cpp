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

            Animation(std::string tx, std::string id, std::vector<int> givenFrames, int gFrameRate, bool gLoop)  {
                texture = tx;
                key = id;

                frames = givenFrames;
                frameRate = gFrameRate;
                loop = gLoop;
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

            virtual void play() {}
    };
}

#endif