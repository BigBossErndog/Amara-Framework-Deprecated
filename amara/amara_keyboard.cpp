namespace Amara {
    class Key;

    class Keyboard {
        public:
            Amara::GameProperties* properties = nullptr;

            std::unordered_map<Amara::Keycode, Amara::Key> keys;
            std::vector<SDL_Keycode> tappedKeycodes;

            Amara::Key* lastKeyDown = nullptr;
			Amara::Keycode lastKeycodeDown;

            bool isActivated = false;
            
            Keyboard() {}
            Keyboard(Amara::GameProperties* gameProperties): Keyboard() {
                properties = gameProperties;
                tappedKeycodes.clear();
            }

            Amara::Key* addKey(SDL_Keycode keyCode) {
                keys[keyCode] = Amara::Key(keyCode);
                return &keys[keyCode];
            }

            void press(Amara::Keycode keyCode) {
                Amara::Key* key;
                for (auto it = keys.begin(); it != keys.end(); it++) {
                    key = &it->second;
                    if (key->keyCode == keyCode) {
                        key->press();
                        lastKeyDown = key;
                    }
                }
                tappedKeycodes.push_back(keyCode);
                lastKeycodeDown = keyCode;
            }

            void release(Amara::Keycode keyCode) {
                Amara::Key* key;
                for (auto it = keys.begin(); it != keys.end(); it++) {
                    key = &it->second;
                    if (key->keyCode == keyCode) {
                        key->release();
                    }
                }
            }

            Amara::Key* get(Amara::Keycode key) {
                if (keys.find(key) != keys.end()) {
                    return &keys[key];
                }
                return nullptr;
            }

            void clearKeys() {
                keys.clear();
            }

            void manage() {
                Amara::Key* key;
                for (auto it = keys.begin(); it != keys.end(); it++) {
                    key = &it->second;
                    key->manage();
                }
                tappedKeycodes.clear();

                isActivated = false;
            }

            ~Keyboard() {
                clearKeys();
            }
    };
}
