#pragma once
#ifndef AMARA_INPUTMANAGER
#define AMARA_INPUTMANAGER

#include "amara.h"

namespace Amara {
    class Keyboard;
    class Mouse;
    class GamepadManager;
    class TouchManager;

    enum InputMode {
        InputMode_None = 0,
        InputMode_Mouse = 1,
        InputMode_Keyboard = 2,
        InputMode_Gamepad = 4,
        InputMode_Touch = 5
    };
    
    class InputManager {
        public:
            Amara::Keyboard* keyboard = nullptr;
            Amara::Mouse* mouse = nullptr;
            Amara::GamepadManager* gamepads = nullptr;
            Amara::TouchManager* touches = nullptr;

            int mode = InputMode_None;
            InputMode lastMode = InputMode_None;

            InputManager() {}
    };
}

#endif