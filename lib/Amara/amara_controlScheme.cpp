#ifndef AMARA_CONTROLSCHEME
#define AMARA_CONTROLSCHEME

#include "amara.h"

namespace Amara {
    class ControlScheme {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::InputManager* input = nullptr;

            unordered_map<string, Amara::Control*> controls;
            vector<Amara::Control*> controlList;

            ControlScheme(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                input = gameProperties->input;
                controlList.clear();
            }

            Amara::Control* newControl(string key) {
                if (get(key) != nullptr) {
                    cout << "Control \"" << key << "\" already exists." << endl;
                    return get(key);
                }

                Amara::Control* newControl = new Amara::Control(key);
                controls[key] = newControl;
                controlList.push_back(newControl);

                return newControl;
            }

            Amara::Control* get(string key) {
                unordered_map<string, Amara::Control*>::iterator got = controls.find(key);
                if (got != controls.end()) {
                    return got->second;
                }
                return nullptr;
            }

            Amara::Control* addKey(string id, Amara::Key* key) {
                Amara::Control* control = get(id);
                if (get(id) != nullptr) {
                    control->addKey(key);
                    return control;
                }
                
                cout << "Key \"" << id << "\" has not been initialized!" << endl;
                Amara::breakGame();
            }

            Amara::Control* addKey(string id, Amara::Keycode keyCode) {
                Amara::Key* key = input->keyboard->get(keyCode);
                if (key != nullptr) {
                    return addKey(id, key);
                }
                cout << "Key with KeyCode \"" << ((char)keyCode) << "\" has not yet been created." << endl;
                cout << "Key with KeyCode \"" << ((char)keyCode) << "\" now exists." << endl;
                key = input->keyboard->addKey(keyCode);
                return addKey(id, key);
            }

            Amara::Control* setKey(string id, Amara::Key* key) {
                Amara::Control* control = get(id);
                if (get(id) != nullptr) {
                    control->setKey(key);
                    return control;
                }
                
                cout << "Key \"" << id << "\" has not been initialized!" << endl;
                Amara::breakGame();
            }

            Amara::Control* setKey(string id, Amara::Keycode keyCode) {
                Amara::Key* key = input->keyboard->get(keyCode);
                if (key != nullptr) {
                    return setKey(id, key);
                }
                cout << "Key with KeyCode \"" << ((char)keyCode) << "\" has not yet been created." << endl;
                cout << "Key with KeyCode \"" << ((char)keyCode) << "\" now exists." << endl;
                key = input->keyboard->addKey(keyCode);
                return setKey(id, key);
            }

            void run() {
                Amara::Control* control;
                for (size_t i = 0; i < controlList.size(); i++) {
                    control = controlList.at(i);
                    control->run();
                }
            }
    };
}

#endif