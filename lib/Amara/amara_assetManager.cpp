#ifndef AMARA_ASSETMANAGER
#define AMARA_ASSETMANAGER

#include "amara.h"

namespace Amara {
    class AssetManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* loader = nullptr;

            AssetManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                loader = properties->loader;
            }

            Amara::Asset* get(string key) {
                return loader->get(key);
            }

            Amara::Animation* addAnim(string textureKey, string animKey, vector<int> frames, int frameRate, bool loop) {
                Amara::Asset* asset = get(textureKey);
                if (asset == nullptr) {
                    cout << "Couldn't find animation \"" << textureKey << "\"" << endl;
                    return nullptr;
                }
                if (asset->type == SPRITESHEET) {
                    return ((Amara::Spritesheet*)asset)->addAnim(animKey, frames, frameRate, loop);
                }
            }

            Amara::Animation* addAnim(string textureKey, string animKey, int frame) {
                return addAnim(textureKey, animKey, {frame}, 1, false);
            }
    };
}

#endif