#ifndef AMARA_GAMEPADMANAGER
#define AMARA_GAMEPADMANAGER

#include "amara.h"

namespace Amara {
    class GamepadManager {
        public:

            std::deque<Amara::Gamepad*> gamepads;
            std::vector<Amara::Gamepad*> connected;
            std::vector<Amara::Gamepad*> disconnected;

            GamepadManager(Amara::GameProperties* gProperties) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    delete gamepad;
                }
                gamepads.clear();

                for (int i = 0; i < 8; i++) {
                    gamepads.push_back(new Amara::Gamepad());
                }
            }

            void connectGamepad(SDL_GameController* controller) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->controller == nullptr) {
                        gamepad->connect(controller);
                        connected.push_back(gamepad);
				        SDL_Log("Game Info: Controller connected, Name: %s\n", SDL_GameControllerName(controller));
                    }
                }
            }

            void disconnectGamepad(SDL_GameController* controller) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->controller == controller) {
                        gamepad->disconnect();
                        SDL_Log("Game Info: Controller disconnected, Name: %s\n", SDL_GameControllerName(controller));
                        disconnected.push_back(gamepad);
                    }
                }
            }

            Amara::Gamepad* get(int index) {
                if (index < 0 || index >= gamepads.size()) {
                    return nullptr;
                }
                return gamepads[index];
            }

            Amara::Gamepad* get(SDL_GameController* controller) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->controller == controller) {
                        return gamepad;
                    }
                }
                return nullptr;
            }

            std::vector<Amara::Gamepad*> getJustConnected() {
                return connected;
            }

            std::vector<Amara::Gamepad*> getJustDisconnected() {
                return disconnected;
            }

            void manage() {
                connected.clear();
                disconnected.clear();
                for (Amara::Gamepad* gamepad: gamepads) {
                    gamepad->manage();
                }
            }
    };
}

#endif