#ifndef AMARA_CONTROL
#define AMARA_CONTROL

#include "amara.h"

namespace Amara {
    class Control {
        public:
            std::string id;
            std::vector<Amara::Key*> keys;

            bool isDown = false;
            bool justDown = false;
            bool tapped = false;
			bool justUp = false;
			bool held = false;
            bool activated = false;

            Amara::Key* lastDown = nullptr;


            Control(std::string givenId) {
                id = givenId;
                keys.clear();
            }

            void addKey(Amara::Key* nKey) {
                keys.push_back(nKey);
            }

            void setKey(Amara::Key* nKey) {
                keys.clear();
                addKey(nKey);
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

                for (Amara::Key* key : keys) {
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
