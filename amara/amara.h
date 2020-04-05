#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <time.h>
#include <limits>
#include <bitset>
#include <map>
#include <unordered_map>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>
#include <math.h>
#include <nlohmann/json.hpp>

#ifndef AMARA_SDL
#define AMARA_SDL
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
    #include <SDL_net.h>
    #include <SDL_FontCache.c>
#endif

#ifndef AMARA_DIRECTIONS
#define AMARA_DIRECTIONS

namespace Amara {
    enum Direction {
        NoDir,
        Up,
        UpLeft,
        UpRight,
        Down,
        DownLeft,
        DownRight,
        Left,
        Right
    };

    std::vector<Direction> DirectionsInOrder = { Down, DownLeft, Left, UpLeft, Up, UpRight, Right, DownRight };
    std::vector<Direction> FourDirections = { Down, Left, Up, Right };
    std::vector<Direction> DiagonalDirections = { DownLeft, UpLeft, UpRight, DownRight };
}

#endif

#include "amara_game.cpp"
#include "amara_sceneManager.cpp"
#include "amara_scenePlugin.cpp"
#include "amara_scene.cpp"
#include "amara_ltimer.cpp"

#include "amara_controlScheme.cpp"
#include "amara_control.cpp"

#include "amara_scene.cpp"

#include "amara_layerContainer.cpp"
#include "amara_canvas.cpp"

#include "amara_sprite.cpp"
#include "amara_animationManager.cpp"
#include "amara_image.cpp"

#include "amara_pathFinding.cpp"

#include "amara_tilemap.cpp"
#include "amara_tilemapLayer.cpp"
#include "amara_tile.cpp"

#include "amara_trueTypeFont.cpp"

#include "amara_camera.cpp"
#include "amara_actor.cpp"
#include "amara_script.cpp"
#include "amara_entity.cpp"

#include "amara_tweener.cpp"
#include "amara_tweenBase.cpp"

#include "amara_interactable.cpp"

#include "amara_assetManager.cpp"
#include "amara_audioGroup.cpp"

#include "amara_loadManager.cpp"
#include "amara_loader.cpp"

#include "amara_music.cpp"
#include "amara_sound.cpp"
#include "amara_audioBase.cpp"
#include "amara_asset.cpp"
#include "amara_animation.cpp"

#include "amara_events.cpp"
#include "amara_mouse.cpp"
#include "amara_pointer.cpp"

#include "amara_gamepadManager.cpp"
#include "amara_gamepad.cpp"
#include "amara_stick.cpp"
#include "amara_button.cpp"
#include "amara_buttonCodes.cpp"

#include "amara_keyboard.cpp"
#include "amara_key.cpp"
#include "amara_keyCodes.cpp"

#include "amara_inputManager.cpp"

#include "amara_debugging.cpp"
#include "amara_taskManager.cpp"
#include "amara_stateManager.cpp"

#include "amara_gameProperties.cpp"

#include "amara_geometry.cpp"
#include "amara_math.cpp"
