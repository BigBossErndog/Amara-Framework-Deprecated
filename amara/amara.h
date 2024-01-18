#pragma once

#ifndef AMARA_STD
#define AMARA_STD
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
    #include <regex>
    #include <random>
    #include <nlohmann/json.hpp>
    #include <MurmurHash3.cpp>
#endif

#ifndef AMARA_SDL
#define AMARA_SDL
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
    #include <SDL_FontCache.c>
    #ifndef AMARA_WEB
        #include <SDL_net.h>
    #endif
#endif

#ifndef AMARA_DIRECTIONS
#define AMARA_DIRECTIONS

namespace Amara {
    enum Direction {
        NoDir = 0,
        Up = 1,
        UpLeft = 2,
        UpRight = 4,
        Down = 8,
        DownLeft = 16,
        DownRight = 32,
        Left = 64,
        Right = 128
    };

    std::vector<Amara::Direction> DirectionsInOrder = { Right, DownRight, Down, DownLeft, Left, UpLeft, Up, UpRight };
    std::vector<Amara::Direction> CardinalDirections = { Right, Down, Left, Up };
    std::vector<Amara::Direction> DiagonalDirections = { DownLeft, UpLeft, UpRight, DownRight };
}

#endif

#ifndef AMARA_CONSTS
#define AMARA_CONSTS
namespace Amara {
    typedef SDL_Color Color;

	enum EventType {
        OBJECTLEFTCLICK,
        OBJECTRIGHTCLICK,
        OBJECTMIDDLECLICK,

        OBJECTLEFTRELEASE,
        OBJECTRIGHTRELEASE,
        OBJECTMIDDLERELEASE,

        OBJECTTOUCHDOWN,
        OBJECTTOUCHUP
    };
}
#endif

#ifndef AMARA_LIBRARY
#define AMARA_LIBRARY
    #include "amara_string.cpp"
    #include "amara_math.cpp"
    #include "amara_geometry.cpp"
    #include "amara_easing.cpp"
    #include "amara_utility.cpp"
    #include "amara_ltimer.cpp"
    #include "amara_random.cpp"

    #include "amara_fdeclarations.cpp"
    
    #include "amara_fileWriter.cpp"

    #include "amara_gameProperties.cpp"

    #include "amara_messages.cpp"

    #include "amara_stateManager.cpp"
    #include "amara_taskManager.cpp"
    #include "amara_debugging.cpp"

    #include "amara_keyCodes.cpp"
    #include "amara_key.cpp"
    #include "amara_keyboard.cpp"

    #include "amara_buttonCodes.cpp"
    #include "amara_button.cpp"
    #include "amara_stick.cpp"
    #include "amara_gamepad.cpp"
    #include "amara_gamepadManager.cpp"

    #include "amara_pointer.cpp"
    #include "amara_mouse.cpp"
    #include "amara_touchPointer.cpp"
    
    #include "amara_interactionManager.cpp"
    #include "amara_touchManager.cpp"
    
    #include "amara_inputManager.cpp"
    #include "amara_events.cpp"

    #include "amara_animation.cpp"
    #include "amara_asset.cpp"
    #include "amara_audioBase.cpp"
    #include "amara_sound.cpp"

    #include "amara_loader.cpp"
    #include "amara_loadManager.cpp"

    #include "amara_audioGroup.cpp"
    #include "amara_assetManager.cpp"

    #include "amara_wallFinder.cpp"
    #include "amara_interactable.cpp"

    #include "amara_physicsBase.cpp"

    #include "amara_broadcaster.cpp"

    #include "amara_entity.cpp"
    #include "amara_script.cpp"
    #include "amara_actor.cpp"

    #include "amara_timer.cpp"
    #include "amara_textureGeneration.cpp"

    #include "amara_sceneTransitionBase.cpp"
    #include "amara_camera.cpp"

    #include "amara_trueTypeFont.cpp"

    #include "amara_tile.cpp"
    #include "amara_tilemap.cpp"

    #include "amara_pathFinding.cpp"
    
    #include "amara_image.cpp"

    #include "amara_animationManager.cpp"
    #include "amara_sprite.cpp"
    
    #include "amara_repeatedSprite.cpp"

    #include "amara_particles.cpp"

    #include "amara_canvas.cpp"
    #include "amara_layerContainer.cpp"

    #include "amara_uiBar.cpp"

    #include "amara_control.cpp"
    #include "amara_controlScheme.cpp"

    #include "amara_fillRect.cpp"

    #include "amara_tintImage.cpp"

    #include "amara_freeWalker.cpp"

    #include "amara_tween.cpp"
    #include "amara_tweens_camera.cpp"
    #include "amara_tweens_entities.cpp"

    #include "amara_physics.cpp"

    #include "amara_scene.cpp"
    #include "amara_scenePlugin.cpp"
    #include "amara_sceneManager.cpp"

    #include "amara_uiBox.cpp"
    #include "amara_textBox.cpp"

    #include "amara_sceneTransition.cpp"
    #include "amara_sceneTransitions.cpp"

    #include "amara_game.cpp"
#endif