#pragma once
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

                for (int i = 0; i < 4; i++) {
                    Amara::Gamepad* gamepad = new Amara::Gamepad();
                    gamepad->index = gamepads.size();
                    gamepads.push_back(gamepad);
                }
            }

            void connectGamepad(SDL_GameController* controller) {
                SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->controller == nullptr && !gamepad->isConnected) {
                        gamepad->connect(controller);
                        connected.push_back(gamepad);
				        SDL_Log("Game Info: Controller connected, Name: %s\n", SDL_GameControllerName(controller));
                        return;
                    }
                    else if (gamepad->id == id) {
                        return;
                    }
                }
                Amara::Gamepad* gamepad = new Amara::Gamepad();
                gamepad->index = gamepads.size();
                gamepads.push_back(gamepad);
                gamepad->connect(controller);
                connected.push_back(gamepad);
                SDL_Log("Game Info: Controller connected, Name: %s\n", SDL_GameControllerName(controller));
            }

            void disconnectGamepad(SDL_GameController* controller) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->controller == controller) {
                        gamepad->disconnect();
                        SDL_Log("Game Info: Controller disconnected, Name: %s\n", SDL_GameControllerName(controller));
                        disconnected.push_back(gamepad);
                        return;
                    }
                }
            }

            int numConnected() {
                int count = 0;
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        count += 1;
                    }
                }
                return count;
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

            bool isDown(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->isDown(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool justDown(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->justDown(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool justUp(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->justUp(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool tapped(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->tapped(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool held(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->held(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool activated(Amara::Buttoncode bcode) {
                for (Amara::Gamepad* gamepad: gamepads) {
                    if (gamepad->isConnected && gamepad->controller != nullptr) {
                        if (gamepad->activated(bcode)) {
                            return true;
                        }
                    }
                }
                return false;
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