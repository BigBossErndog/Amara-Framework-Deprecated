#ifndef AMARA_BUTTON
#define AMARA_BUTTON

#include "amara.h"

namespace Amara {
    class Button: public Amara::Key {
        public:
            Amara::Buttoncode buttonCode;

            Button(Amara::Buttoncode bcode) {
                buttonCode = bcode;
            }
    };
}

#endif