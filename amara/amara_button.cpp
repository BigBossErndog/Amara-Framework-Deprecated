#ifndef AMARA_BUTTON
#define AMARA_BUTTON

#include "amara.h"

namespace Amara {
    class Button: public Amara::Key {
        public:
            Amara::Buttoncode buttonCode;
            Button() {}
            Button(Amara::Buttoncode bcode) {
                buttonCode = bcode;
            }
    };

    int TRIGGER_DEADZONE = 8000;

    class Trigger: public Amara::Button {
        public:
            Amara::Axiscode axisCode;
            Sint16 pressure = 0;

            Trigger(Amara::Axiscode acode) {
                axisCode = acode;
            }

            void push(Sint16 value) {
                if (value > TRIGGER_DEADZONE && pressure < TRIGGER_DEADZONE) {
                    press();
                }
                else if (value < TRIGGER_DEADZONE && pressure > TRIGGER_DEADZONE) {
                    release();
                }
                pressure = value;
            }
    };
}

#endif