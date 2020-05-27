#pragma once
#ifndef AMARA_CONTROLSCHEME
#define AMARA_CONTROLSCHEME

#include "amara.h"

namespace Amara {
    class ControlScheme {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::InputManager* input = nullptr;

            std::unordered_map<std::string, Amara::Control*> controls;
            std::vector<Amara::Control*> controlList;

            ControlScheme(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                input = gameProperties->input;
                controlList.clear();
            }

            virtual void configure(nlohmann::json config) {

            }

            virtual nlohmann::json toData() {
                nlohmann::json config;
                for (Amara::Control* control: controlList) {
                    config[control->id] = control->toData();
                }
                return config;
            }

            Amara::Control* newControl(std::string key) {
                if (get(key) != nullptr) {
                    std::cout << "Control \"" << key << "\" already exists." << std::endl;
                    return get(key);
                }

                Amara::Control* newControl = new Amara::Control(properties, key);
                controls[key] = newControl;
                controlList.push_back(newControl);

                return newControl;
            }

            Amara::Control* get(std::string key) {
                std::unordered_map<std::string, Amara::Control*>::iterator got = controls.find(key);
                if (got != controls.end()) {
                    return got->second;
                }
                return nullptr;
            }

            Amara::Control* addKey(std::string id, Amara::Key* key) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                    control->addKey(key);
                    return control;
                }
                
                control = newControl(id);
                return addKey(id, key);
            }

            Amara::Control* addKey(std::string id, Amara::Keycode keyCode) {
                Amara::Key* key = input->keyboard->get(keyCode);
                if (key != nullptr) {
                    return addKey(id, key);
                }

                key = input->keyboard->addKey(keyCode);
                return addKey(id, key);
            }

            Amara::Control* setKey(std::string id, Amara::Key* key) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                    control->setKey(key);
                    return control;
                }

                control = newControl(id);
                return setKey(id, key);
            }

            Amara::Control* setKey(std::string id, Amara::Keycode keyCode) {
                Amara::Key* key = input->keyboard->get(keyCode);
                if (key != nullptr) {
                    return setKey(id, key);
                }

                key = input->keyboard->addKey(keyCode);
                return setKey(id, key);
            }

            Amara::Control* addButton(std::string id, Amara::Buttoncode bcode) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                    control->addButton(bcode);
                    return control;
                }
                
                control = newControl(id);
                return addButton(id, bcode);
            }

            Amara::Control* addButton(std::string id, Amara::Button* button) {
                return addKey(id, button);
            } 

            void run() {
                for (Amara::Control* control : controlList) {
                    control->run();
                }
            }

            bool isDown(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->isDown;
                }
                return false;
            }

            bool justDown(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->justDown;
                }
                return false;
            }

            bool justUp(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->justUp;
                }
                return false;
            }

            bool tapped(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->tapped;
                }
                return false;
            }

            bool held(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->held;
                }
                return false;
            }

            bool activated(std::string id) {
                Amara::Control* control = get(id);
                if (control != nullptr) {
                   return control->activated;
                }
                return false;
            }
    };
}

#endif