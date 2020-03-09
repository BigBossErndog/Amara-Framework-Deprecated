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

            Asset(string givenKey, AssetType givenType, void* givenAsset) {
                key = givenKey;
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

            unordered_map<string, Amara::Animation*> anims;

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

            Amara::Animation* addAnim(string animKey, vector<int> frames, int frameRate, bool loop) {
                Amara::Animation* anim;
                unordered_map<string, Amara::Animation*>::iterator got = anims.find(animKey);
                if (got != anims.end()) {
                    anim = got->second;
                    anim->frames = frames;
                    anim->frameRate = frameRate;
                    anim->loop = loop;
                    return anim;
                }
                
                anim = new Amara::Animation(key, animKey, frames, frameRate, loop);
                anims[animKey] = anim;
                cout << "Added animation \"" << animKey << "\" to spritesheet \"" << key << "." << endl;
                return anim;
            }
            Amara::Animation* addAnim(string animKey, int frame) {
                return addAnim(animKey, {frame}, 1, false);
            }

            Amara::Animation* getAnim(string animKey) {
                unordered_map<string, Amara::Animation*>::iterator got = anims.find(animKey);
                if (got != anims.end()) {
                    return got->second;
                }
                return nullptr;
            }
    };
}

#endif
