#ifndef AMARA_ASSET
#define AMARA_ASSET

#include "amara.h"

namespace Amara {
    enum AssetType {
        SURFACE,
        IMAGE,
        SPRITESHEET,
        TTF,
        SOUND,
        MUSIC
    };

    class Asset {
        public:
            AssetType type;
            void* asset;
            string key;

            Asset(string key, AssetType givenType, void* givenAsset) {
                key = key;
                type = givenType;
				asset = givenAsset;
            }
    };

    class ImageTexture : public Amara::Asset {
        public:
            int width = 0;
            int height = 0;

            ImageTexture(string key, AssetType givenType, SDL_Texture* givenAsset): Amara::Asset(key, givenType, givenAsset) {
                SDL_QueryTexture(givenAsset, NULL, NULL, &width, &height);
            }
    };

    class Spritesheet : public Amara::ImageTexture {
        public:
            int frameWidth = 0;
            int frameHeight = 0;

            Spritesheet(string key, AssetType givenType, SDL_Texture* newtexture, int newwidth, int newheight): Amara::ImageTexture(key, givenType, newtexture) {
                asset = newtexture;
                frameWidth = newwidth;
                frameHeight = newheight;
                if (frameWidth > width) {
                    frameWidth = width;
                }
                if (frameHeight > height) {
                    frameHeight = height;
                }
            }
    };
}

#endif
