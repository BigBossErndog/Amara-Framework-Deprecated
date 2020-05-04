#pragma once
#ifndef AMARA_TILE
#define AMARA_TILE

namespace Amara {

    const unsigned long TILED_FLIPPEDHORIZONTALLY = 0x80000000;
    const unsigned long TILED_FLIPPEDVERTICALLY = 0x40000000;
    const unsigned long TILED_FLIPPEDANTIDIAGONALLY = 0x20000000;

    struct Tile {
        int id = 0;
        int x = 0;
        int y = 0;

        bool fhorizontal;
        bool fvertical;
        bool fdiagonal;

        float tileWidth = 0;
        float tileHeight = 0;
        int widthInPixels = 0;
        int heightInPixels = 0;
    };
}

#endif