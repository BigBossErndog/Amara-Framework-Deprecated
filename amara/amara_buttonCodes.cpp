#ifndef AMARA_BUTTONCODES
#define AMARA_BUTTONCODES

namespace Amara {
    typedef Uint8 Buttoncode;
    /**
     *  The list of buttons available from a controller
     */
    enum ButtonCode {
        BUTTON_INVALID = -1,
        BUTTON_A,
        BUTTON_B,
        BUTTON_X,
        BUTTON_Y,
        BUTTON_BACK,
        BUTTON_GUIDE,
        BUTTON_START,
        BUTTON_LEFTSTICK,
        BUTTON_RIGHTSTICK,
        BUTTON_LEFTSHOULDER,
        BUTTON_RIGHTSHOULDER,
        BUTTON_DPAD_UP,
        BUTTON_DPAD_DOWN,
        BUTTON_DPAD_LEFT,
        BUTTON_DPAD_RIGHT,
        BUTTON_MAX
    };
}

#endif