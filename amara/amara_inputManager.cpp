#ifndef AMARA_INPUTMANAGER
#define AMARA_INPUTMANAGER

#include "amara.h"

namespace Amara {
    class Keyboard;
    class Mouse;
    
    class InputManager {
        public:
            Amara::Keyboard* keyboard = nullptr;
            Amara::Mouse* mouse = nullptr;

            InputManager() {}
    };
}

#endif