#pragma once
#ifndef AMARA_INPUTMANAGER
#define AMARA_INPUTMANAGER



namespace Amara {
    class Keyboard;
    class Mouse;
    class GamepadManager;
    class TouchManager;

    enum InputMode {
        InputMode_None = 0,
        InputMode_Mouse = 1,
        InputMode_Keyboard = 2,
        InputMode_Gamepad = 4,
        InputMode_Touch = 8
    };
    
    class InputManager {
        public:
            Amara::Keyboard* keyboard = nullptr;
            Amara::Mouse* mouse = nullptr;
            Amara::GamepadManager* gamepads = nullptr;
            Amara::TouchManager* touches = nullptr;

            int mode = InputMode_None;
            InputMode lastMode = InputMode_None;

            std::string inputText;
            bool textInputOn = false;

            InputManager() {}

            void startTextInput() {
                inputText.clear();
                textInputOn = true;
                SDL_StartTextInput();
            }

            std::string stopTextInput() {
                textInputOn = false;
                SDL_StopTextInput();
                return inputText;
            }

            std::string getClipboardText() {
                inputText = SDL_GetClipboardText();
            }

            void setClipboardText(std::string txt) {
                SDL_SetClipboardText(txt.c_str());
            }
    };
}

#endif