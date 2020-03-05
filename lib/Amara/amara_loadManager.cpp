#ifndef AMARA_LOADMANAGER
#define AMARA_LOADMANAGER

#include "amara.h"

namespace Amara {
    struct LoadTask {
        Amara::AssetType type;
        bool replace;

        string key;
        string path;
        
        int frameWidth;
        int frameHeight;

        int size;
        int style;
        SDL_Color color;
    };  

    class LoadManager {
        public:
            Amara::Game* game = nullptr;
            Amara::GameProperties* properties = nullptr;

            LoadManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                game = properties->game;
            }
    };
}

#endif