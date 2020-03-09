#ifndef AMARA_ANIMATION
#define AMARA_ANIMATION

#include "amara.h"

namespace Amara {
    class Animation {
        public:
            string texture;
            string key;

            vector<int> frames;
            int frameRate = 1;
            bool loop;

            Animation(string tx, string id, vector<int> givenFrames, int gFrameRate, bool gLoop)  {
                texture = tx;
                key = id;

                frames = givenFrames;
                frameRate = gFrameRate;
                loop = gLoop;
            }

            Animation(string tx, string id, int frame): Animation(tx, id, {frame}, 1, false) {}

            int frameAt(int index) {
                return frames.at(index);
            }

            int length() {
                return frames.size();
            }
    };
}

#endif