#ifndef AMARA_KEYBOARD
#define AMARA_KEYBOARD

#include "amara.h"

namespace Amara {
    class Key;

    class Keyboard {
        public:
            unordered_map<string, Amara::Key*> keys;

            Keyboard() {

            }

            Amara::Key* addKey(string key, SDL_Keycode keyCode) {
                Amara::Key* newKey = new Amara::Key(key, keyCode);
                keys[key] = newKey;
                return newKey;
            }

            void press(SDL_Keycode keyCode) {
                unordered_map<string, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    if (key->keyCode == keyCode) {
                        key->press();
                    }
                }
            }

            void release(SDL_Keycode keyCode) {
                unordered_map<string, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    if (key->keyCode == keyCode) {
                        key->release();
                    }
                }
            }

            Amara::Key* get(string key) {
                if (keys.find(key) != keys.end()) {
                    return keys[key];
                }
                return nullptr;
            }

            void manage() {
                unordered_map<string, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    key->manage();
                }
            }
    };
}

#endif
