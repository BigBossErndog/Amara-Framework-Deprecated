#pragma once

#include "amaraWeb.hpp"
#include <emscripten.h>

namespace Amara {
    void webGameLoop(void* arg) {
        Amara::Game* game = (Amara::Game*)arg;
        if (!game->quit) {
            game->gameLoop();
        }
        else {
            emscripten_cancel_main_loop();
        }
    }

    class WebGame: public Amara::Game {
    public:
        WebGame(std::string gName, bool gRendering): Amara::Game(gName, gRendering) {}
        WebGame(std::string gName): Amara::Game(gName) {}

        void start() {
            emscripten_set_main_loop_arg(Amara::webGameLoop, this, -1, 1);
        }

        void start(std::string startKey) {
            // Start a specific scene
            scenes->start(startKey);
            start();
        }
    };
}