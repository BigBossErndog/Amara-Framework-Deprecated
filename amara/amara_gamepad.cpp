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

            std::unordered_map<Uint8, Amara::Button*> buttons;
            
            bool isConnected = false;
            bool justConnected = false;
            bool justDisconnected = false;
            
            Gamepad() {
                
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
            }

            void press(Uint8 btn) {

            }

            void release(Uint8 btn) {
                
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

#endif