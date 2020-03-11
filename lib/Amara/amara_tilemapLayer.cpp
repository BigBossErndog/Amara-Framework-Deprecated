#ifndef AMARA_TILEMAPLAYER
#define AMARA_TILEMAPLAYER

#include "amara.h"

namespace Amara {
    class TilemapLayer: Amara::Actor {
        public:
            vector<vector<Amara::Tile*>> tiles;

            TilemapLayer() {}

            void run() {
                Amara::Actor::run();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}

#endif