#ifndef AMARA_TILE
#define AMARA_TILE

namespace Amara {
    struct Tile {
        int index = 0;
        int x = 0;
        int y = 0;
        float tileWidth = 0;
        float tileHeight = 0;
        int widthInPixels = 0;
        int heightInPixels = 0;
    };
}

#endif