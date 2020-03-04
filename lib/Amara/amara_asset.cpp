#ifndef AMARA_ASSET
#define AMARA_ASSET

#include "amara.h"

namespace Amara {
    enum AssetType {
        SURFACE,
        TEXTURE,
        SPRITESHEET,
        TTF,
        SOUND,
        MUSIC
    };

    class Asset {
        public:
            AssetType type;
            void* asset;

            Asset(AssetType givenType, void* givenAsset) {
                type = givenType;
				asset = givenAsset;
            }
    };

    class Spritesheet : public Amara::Asset {
        public:
            int framewidth = 0;
            int frameheight = 0;

            Spritesheet(SDL_Texture* newtexture, int newwidth, int newheight): Amara::Asset(SPRITESHEET, newtexture) {
                type = SPRITESHEET;
                asset = newtexture;
                framewidth = newwidth;
                frameheight = newheight;
            }
    };
}

#endif