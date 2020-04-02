#ifndef AMARA_GAMEPAD
#define AMARA_GAMEPAD

#include "amara.h"

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
            std::vector<Amara::Buttoncode> buttonCodes;
            
            bool isConnected = false;
            bool justConnected = false;
            bool justDisconnected = false;
            
            Gamepad() {
                buttons.clear();
                addButton(BUTTON_A);
                addButton(BUTTON_B);
                addButton(BUTTON_X);
                addButton(BUTTON_Y);
                addButton(BUTTON_DPAD_LEFT);
                addButton(BUTTON_DPAD_RIGHT);
                addButton(BUTTON_DPAD_UP);
                addButton(BUTTON_DPAD_DOWN);
            }

            Amara::Button* addButton(Amara::Buttoncode bcode) {
                if (getButton(bcode) == nullptr) {
                    buttons[bcode] = new Amara::Button(bcode);
                    buttonCodes.push_back(bcode);
                }
                return buttons[bcode];
            }

            Amara::Button* getButton(Amara::Buttoncode bcode) {
                if (buttons.find(bcode) != buttons.end()) {
                    return buttons[bcode];
                }
                return nullptr;
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
                std::unordered_map<Uint8, Amara::Button*>::iterator it;
                Amara::Button* btn;
                for (it = buttons.begin(); it != buttons.end(); it++) {
                    btn = it->second;
                    btn->manage();
                }
            }

            void press(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->press();
                }
            }

            void release(Amara::Buttoncode bcode) {
                Amara::Button* button = getButton(bcode);
                if (button != nullptr) {
                    return button->release();
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
                buttonCodes.clear();
            }
    };
}

#endif