#pragma once
#ifndef AMARA_TILEANIMATION
#define AMARA_TILEANIMATION

#include "amara.h"

namespace Amara {
    struct TileFrame {
        int tileId;
        int duration;
    };

    class TileAnimation {
        public:
            Amara::GameProperties* properties = nullptr;

            int id = 0;

            std::vector<TileFrame> frames;
            int numFrames = 0;
            int frame = 0;
            int currentTileId = 0;

            float frameCounter = 0;

            TileAnimation(Amara::GameProperties* gameProperties, nlohmann::json& animData) {
                properties = gameProperties;

                id = animData["id"];
                frames.clear();
                frame = 0;

                numFrames = animData["animation"].size();
                for (nlohmann::json& frameData: animData["animation"]) {
                    TileFrame tf;
                    tf.tileId = frameData["tileid"];
                    tf.duration = frameData["duration"];
                    frames.push_back(tf);
                }

                currentTileId = (frames.empty()) ? id : frames[0].tileId;
            }

            void update() {
                if (frames.empty()) return;
                double factor = 1000.0/properties->lps;
                int duration = frames[frame].duration;

                frameCounter += (1000.0/properties->lps);

                if (frameCounter >= duration) {
                    frameCounter -= duration;
                    frame += 1;
                    frame = frame % numFrames;
                    currentTileId = frames[frame].tileId;
                }
            }
    };
}

#endif