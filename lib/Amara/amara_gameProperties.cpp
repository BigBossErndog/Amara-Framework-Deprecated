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
    class InputManager;
    class TaskManager;
    class ControlScheme;

    class GameProperties {
        public:
            Amara::Game* game = nullptr;

            SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

            int width = 0;
            int height = 0;

            float scrollX = 0;
            float scrollY = 0;

            float zoomX = 0;
            float zoomY = 0;

            Amara::IntRect* display = nullptr;
			Amara::IntRect* resolution = nullptr;
			Amara::IntRect* window = nullptr;

            bool lagging = false;
            bool dragged = false;
            bool windowFocused = false;
			bool isFullscreen = false;

            Amara::Loader* loader = nullptr;
            Amara::SceneManager* scenes = nullptr;

            Amara::Scene* currentScene = nullptr;
            Amara::Camera* currentCamera = nullptr;

            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::TaskManager* taskManager = nullptr;

            GameProperties() {}
    };
}

#endif
