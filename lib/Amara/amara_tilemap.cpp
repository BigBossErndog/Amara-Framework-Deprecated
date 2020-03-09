#ifndef AMARA_TILEMAP
#define AMARA_TILEMAP

#include "amara.h"

namespace Amara {
    struct Tile {
        int x;
        int y;
        float tileWidth;
        float tileHeight;
        int widthInPixels;
        int heightInPixels;
    };

    class Tilemap {
        public:
            int width = 0;
            int height = 0;
            
            int widthInPixels = 0;
            int heightInPixels = 0;

            vector<Amara::Tile*> tiles;
            
            Tilemap() {

            }
    };
}

#endif