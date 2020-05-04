#pragma once
#ifndef AMARA_INPUTMANAGER
#define AMARA_INPUTMANAGER

#include "amara.h"

namespace Amara {
    class Keyboard;
    class Mouse;
    class GamepadManager;
    
    class InputManager {
        public:
            Amara::Keyboard* keyboard = nullptr;
            Amara::Mouse* mouse = nullptr;
            Amara::GamepadManager* gamepads = nullptr;

            InputManager() {}
    };
}

#endif