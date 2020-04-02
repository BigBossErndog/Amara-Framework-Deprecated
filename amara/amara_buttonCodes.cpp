#ifndef AMARA_BUTTONCODES
#define AMARA_BUTTONCODES

namespace Amara {
    typedef Uint8 Buttoncode;
    typedef Uint8 Axiscode;
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
        BUTTON_MAX,

        BUTTON_TRIGGERLEFT,
        BUTTON_TRIGGERRIGHT,

        LEFTSTICK_UP,
        LEFTSTICK_DOWN,
        LEFTSTICK_LEFT,
        LEFTSTICK_RIGHT,
        
        RIGHTSTICK_UP,
        RIGHTSTICK_DOWN,
        RIGHTSTICK_LEFT,
        RIGHTSTICK_RIGHT
    };

    enum AxisCode {
        AXIS_INVALID = -1,
        AXIS_LEFTX,
        AXIS_LEFTY,
        AXIS_RIGHTX,
        AXIS_RIGHTY,
        AXIS_TRIGGERLEFT,
        AXIS_TRIGGERRIGHT,
        AXIS_MAX
    };
}

#endif