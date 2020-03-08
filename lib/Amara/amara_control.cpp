#ifndef AMARA_CONTROL
#define AMARA_CONTROL

#include "amara.h"

namespace Amara {
    class Control {
        public:
            string id;
            vector<Amara::Key*> keys;

            bool isDown = false;
            bool justDown = false;
            bool tapped = false;
			bool justUp = false;
			bool held = false;
            bool activated = false;

            Amara::Key* lastDown = nullptr;

            
            Control(string givenId) {
                id = givenId;
                keys.clear();
            }

            void add(Amara::Key* nKey) {
                keys.push_back(nKey);
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

            void run() {
                isDown = false;
                justDown = false;
                tapped = false;
                justUp = false;
                held = false;
                activated = false;

                Amara::Key* key;
                for (size_t i = 0; i < keys.size(); i++) {
                    key = keys.at(i);
                    isDown = isDown || key->isDown;
                    justDown = justDown || key->justDown;
                    tapped = tapped || key->tapped;
                    justUp = justUp || key->justUp;
                    held = held || key->held;
                    activated = activated || key->activated;
                    
                    if (justDown) {
                        lastDown = key;
                    }
                }
            }
    };
}

#endif