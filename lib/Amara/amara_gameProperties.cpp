#ifndef AMARA_GAME_PROPERTIES
#define AMARA_GAME_PROPERTIES

#include "amara.h"

namespace Amara {
    class Game;
    class LoadManager;
    class Loader;
    class Keyboard;
    class SceneManager;
    class Scene;
    class Camera;

    class GameProperties {
        public:
            Amara::Game* game = nullptr;

            SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

            int width = 0;
            int height = 0;

            Amara::IntRect* display = nullptr;
			Amara::IntRect* resolution = nullptr;
			Amara::IntRect* window = nullptr;
            
            Amara::Loader* loader = nullptr;
            Amara::SceneManager* scenes = nullptr;

            Amara::Scene* currentScene = nullptr;
            Amara::Camera* currentCamera = nullptr;

            Amara::InputManager* input = nullptr;

            GameProperties() {
                
            }
    };
}

#endif