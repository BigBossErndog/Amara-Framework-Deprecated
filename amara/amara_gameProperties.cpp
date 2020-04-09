#ifndef AMARA_GAME_PROPERTIES
#define AMARA_GAME_PROPERTIES

#include "amara.h"

namespace Amara {
    class Game;
    class AssetManager;
    class LoadManager;
    class Loader;
    class Keyboard;
    class SceneManager;
    class Scene;
    class Camera;
    class InputManager;
    class TaskManager;
    class ControlScheme;
    class EventManager;
    class Music;
    class AudioGroup;

    class GameProperties {
        public:
            Amara::Game* game = nullptr;
            bool quit = false;

            SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

            SDL_Color backgroundColor;

            int width = 0;
            int height = 0;

            int scrollX = 0;
            int scrollY = 0;

            float zoomFactorX = 1;
            float zoomFactorY = 1;

            float offsetX = 0;
            float offsetY = 0;

            float zoomX = 0;
            float zoomY = 0;

            float angle = 0;

            Amara::IntRect* display = nullptr;
			Amara::IntRect* resolution = nullptr;
			Amara::IntRect* window = nullptr;

            bool lagging = false;
            bool dragged = false;
            bool windowFocused = false;
			bool isFullscreen = false;

            bool renderTargetsReset = false;
            bool renderDeviceReset = false;

            int fps = 60;
            int lps = fps;
            int realFPS = fps;

            Amara::Loader* loader = nullptr;
            Amara::AssetManager* assets = nullptr;
            Amara::SceneManager* scenes = nullptr;

            Amara::Music* music = nullptr;

            Amara::Scene* currentScene = nullptr;
            Amara::Camera* currentCamera = nullptr;

            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::TaskManager* taskManager = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::AudioGroup* audio = nullptr;

            GameProperties() {}
    };
}

#endif
