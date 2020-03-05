#ifndef AMARA_INPUTMANAGER
#define AMARA_INPUTMANAGER

#include "amara.h"

namespace Amara {
    class Keyboard;
    
    class InputManager {
        public:
            Amara::Keyboard* keyboard = nullptr;

            InputManager() {

            }
    };
}

#endif