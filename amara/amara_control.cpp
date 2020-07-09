#pragma once
#ifndef AMARA_CONTROL
#define AMARA_CONTROL

#include "amara.h"

namespace Amara {
    class Control {
        public:
            Amara::GameProperties* properties;
            Amara::InputManager* input;
            Amara::GamepadManager* gamepads;

            std::string id;
            std::vector<Amara::Key*> keys;
            std::vector<Amara::Buttoncode> buttons;

            bool isDown = false;
            bool justDown = false;
            bool tapped = false;
			bool justUp = false;
			bool held = false;
            bool activated = false;

            int downTime = 0;

            Amara::Key* lastKeyDown = nullptr;

            Control(Amara::GameProperties* gProperties, std::string givenId) {
                properties = gProperties;
                input = properties->input;
                gamepads = input->gamepads;

                id = givenId;
                keys.clear();
            }

            virtual nlohmann::json toData() {
                nlohmann::json config;
                config["keys"] = nlohmann::json::array();
                for (Amara::Key* key: keys) {
                    config["keys"].push_back(key->keyCode);
                }

                config["buttons"] = nlohmann::json::array();
                for (Amara::Buttoncode button: buttons) {
                    config["buttons"].push_back(button);
                }

                return config;
            }

            void addKey(Amara::Key* nKey) {
                keys.push_back(nKey);
            }

            void setKey(Amara::Key* nKey) {
                keys.clear();
                addKey(nKey);
            }

            void addButton(Amara::Buttoncode bcode) {
                buttons.push_back(bcode);
            }

            void setButton(Amara::Buttoncode bcode) {
                buttons.clear();
                addButton(bcode);
            }

            Amara::Key* remove(Amara::Keycode keyCode) {
                Amara::Key* key;
                for (size_t i = 0; i < keys.size(); i++) {
                    key = keys.at(i);
                    if (key->keyCode == keyCode) {
                        keys.erase(keys.begin() + i);
                        return key;
                    }
                }

                return nullptr;
            }

            Amara::Key* remove(Amara::Key* find) {
                Amara::Key* key;
                for (size_t i = 0; i < keys.size(); i++) {
                    key = keys.at(i);
                    if (key == find) {
                        keys.erase(keys.begin() + i);
                        return key;
                    }
                }

                return nullptr;
            }

			void clearKeys() {
				keys.clear();
			}

            void run() {
                isDown = false;
                justDown = false;
                tapped = false;
                justUp = false;
                held = false;
                activated = false;

                downTime = 0;

                for (Amara::Key* key : keys) {
                    isDown = isDown || key->isDown;
                    justDown = justDown || key->justDown;
                    tapped = tapped || key->tapped;
                    justUp = justUp || key->justUp;
                    held = held || key->held;
                    activated = activated || key->activated;

                    if (justDown) {
                        lastKeyDown = key;
                    }
                    if (key->isDown && key->downTime > downTime) {
                        downTime = key->downTime;
                    }
                }

                for (Amara::Buttoncode bcode: buttons) {
                    isDown = isDown || gamepads->isDown(bcode);
                    justDown = justDown || gamepads->justDown(bcode);
                    tapped = tapped || gamepads->tapped(bcode);
                    justUp = justUp || gamepads->justUp(bcode);
                    held = held || gamepads->held(bcode);
                    activated = activated || gamepads->activated(bcode);

                    if (gamepads->isDown(bcode) && gamepads->downTime(bcode) > downTime) {
                        downTime = gamepads->downTime(bcode);
                    }
                }
            }
    };
}

#endif
