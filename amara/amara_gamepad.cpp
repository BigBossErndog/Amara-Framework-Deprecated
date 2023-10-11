namespace Amara {
    enum Gamepad_Scheme {
        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        GAMEPAD_A,
        GAMEPAD_B,
        GAMEPAD_X,
        GAMEPAD_Y
    };

    class Gamepad {
        public:
            SDL_GameController* controller = nullptr;
            SDL_JoystickID id;
            int index = 0;

            std::unordered_map<Amara::Buttoncode, Amara::Button*> buttons;
            std::unordered_map<Amara::Axiscode, Amara::Trigger*> triggers;
            
            bool isConnected = false;
            bool justConnected = false;
            bool justDisconnected = false;

            bool isActivated = false;
            
            Gamepad() {}
            Gamepad(int gIndex): Gamepad() {
                index = gIndex;
            }

            void setup() {
                buttons.clear();
                triggers.clear();

                addButton(BUTTON_BACK);
                addButton(BUTTON_GUIDE);
                addButton(BUTTON_START);

                addButton(BUTTON_A);
                addButton(BUTTON_B);
                addButton(BUTTON_X);
                addButton(BUTTON_Y);
                addButton(BUTTON_DPAD_LEFT);
                addButton(BUTTON_DPAD_RIGHT);
                addButton(BUTTON_DPAD_UP);
                addButton(BUTTON_DPAD_DOWN);

                addButton(BUTTON_LEFTSHOULDER);
                addButton(BUTTON_RIGHTSHOULDER);

                addStick(BUTTON_LEFTSTICK);
                addStick(BUTTON_RIGHTSTICK);

                addTrigger(AXIS_TRIGGERLEFT);
                addTrigger(AXIS_TRIGGERRIGHT);
            }

            Amara::Button* addButton(Amara::Buttoncode bcode) {
                if (getButton(bcode) == nullptr) {
                    buttons[bcode] = new Amara::Button(bcode);
                }
                return buttons[bcode];
            }

            Amara::Stick* addStick(Amara::Buttoncode bcode) {
                if (getButton(bcode) == nullptr) {
                    Amara::Stick* stick = new Amara::Stick(bcode);
                    buttons[bcode] = stick;
                    switch (bcode) {
                        case BUTTON_LEFTSTICK:
                            buttons[LEFTSTICK_UP] = stick->up;
                            buttons[LEFTSTICK_DOWN] = stick->down;
                            buttons[LEFTSTICK_LEFT] = stick->left;
                            buttons[LEFTSTICK_RIGHT] = stick->right;
                            break;
                        case BUTTON_RIGHTSTICK:
                            buttons[RIGHTSTICK_UP] = stick->up;
                            buttons[RIGHTSTICK_DOWN] = stick->down;
                            buttons[RIGHTSTICK_LEFT] = stick->left;
                            buttons[RIGHTSTICK_RIGHT] = stick->right;
                            break;
                    }
                }
                return (Amara::Stick*)buttons[bcode];
            }

            Amara::Trigger* addTrigger(Amara::Axiscode acode) {
                if (getTrigger(acode) != nullptr) {
                    Amara::Trigger* trigger = new Amara::Trigger(acode);
                    triggers[acode] = trigger;
                    switch (acode) {
                        case Amara::AXIS_TRIGGERLEFT:
                            buttons[BUTTON_TRIGGERLEFT] = trigger;
                            trigger->buttonCode = BUTTON_TRIGGERLEFT;
                            break;
                        case Amara::AXIS_TRIGGERRIGHT:
                            buttons[BUTTON_TRIGGERRIGHT] = trigger;
                            trigger->buttonCode = BUTTON_TRIGGERRIGHT;
                            break;
                    }
                }
                return triggers[acode];
            }

            Amara::Button* getButton(Amara::Buttoncode bcode) {
                if (buttons.find(bcode) != buttons.end()) {
                    return buttons[bcode];
                }
                return nullptr;
            }

            Amara::Stick* getStick(Amara::Buttoncode bcode) {
                return (Amara::Stick*)getButton(bcode);
            }

            Amara::Trigger* getTrigger(Amara::Buttoncode bcode) {
                return (Amara::Trigger*)getButton(bcode);
            }

            bool isDown(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->isDown;
                }
                return false;
            }

            bool justDown(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->justDown;
                }
                return false;
            }

            bool justUp(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->justUp;
                }
                return false;
            }

            bool tapped(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->tapped;
                }
                return false;
            }

            bool held(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->held;
                }
                return false;
            }

            int downTime(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->downTime;
                }
                return 0;
            }

            bool activated(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->held;
                }
                return false;
            }
            
            void connect(SDL_GameController* gController) {
                controller = gController;
                id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                isConnected = true;
                justConnected = true;
            }

            void disconnect() {
                isConnected = false;
                justDisconnected = true;
                controller = nullptr;
            }

            void manage() {
                justConnected = false;
                justDisconnected = false;
                Amara::Button* btn;
                for (auto it = buttons.begin(); it != buttons.end(); it++) {
                    btn = it->second;
                    btn->manage();
                }
                isActivated = false;
            }

            void press(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    isActivated = true;
                    return button->press();
                }
            }

            void release(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->release();
                }
            }

            void push(Amara::Axiscode acode, Sint16 value) {
                Amara::Stick* stick;
                Amara::Trigger* trigger;

                switch (acode) {
                    case AXIS_LEFTX:
                        stick = getStick(BUTTON_LEFTSTICK);
                        if (stick) stick->pushX(value);
                        break;
                    case AXIS_LEFTY:
                        stick = getStick(BUTTON_LEFTSTICK);
                        if (stick) stick->pushY(value);
                        break;
                    case AXIS_RIGHTX:
                        stick = getStick(BUTTON_RIGHTSTICK);
                        if (stick) stick->pushX(value);
                        break;
                    case AXIS_RIGHTY:
                        stick = getStick(BUTTON_RIGHTSTICK);
                        if (stick) stick->pushY(value);
                        break;
                    case AXIS_TRIGGERLEFT:
                        trigger = getTrigger(BUTTON_TRIGGERLEFT);
                        if (trigger) trigger->push(value);
                        break;
                    case AXIS_TRIGGERRIGHT:
                        trigger = getTrigger(BUTTON_TRIGGERRIGHT);
                        if (trigger) trigger->push(value);
                        break;
                }
            }

            ~Gamepad() {
                std::unordered_map<Uint8, Amara::Button*>::iterator it;
                Amara::Button* btn;
                for (it = buttons.begin(); it != buttons.end(); it++) {
                    btn = it->second;
                    delete btn;
                }
                buttons.clear();
            }
    };
}