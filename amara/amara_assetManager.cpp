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

            Amara::Asset* get(std::string key) {
                return loader->get(key);
            }

            Amara::Animation* addAnim(std::string textureKey, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                Amara::Asset* asset = get(textureKey);
                if (asset == nullptr) {
                    std::cout << "Couldn't find animation \"" << textureKey << "\"" << std::endl;
                    return nullptr;
                }
                if (asset->type == SPRITESHEET) {
                    return ((Amara::Spritesheet*)asset)->addAnim(animKey, frames, frameRate, loop);
                }
            }

            Amara::Animation* addAnim(std::string textureKey, std::string animKey, int frame) {
                return addAnim(textureKey, animKey, {frame}, 1, false);
            }

            void addAnim(std::vector<std::string> textureKeys, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                for (std::string tKey : textureKeys) {
                    addAnim(tKey, animKey, frames, frameRate, loop);
                }
            }

            void addAnim(std::vector<std::string> textureKeys, std::string animKey, int frame) {
                addAnim(textureKeys, animKey, {frame}, 1, false);
            }
    };
}

#endif