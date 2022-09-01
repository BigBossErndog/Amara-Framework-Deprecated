#pragma once
#ifndef AMARA_DEBUGGING
#define AMARA_DEBUGGING



namespace Amara {
    class Error {
        public:
            void breakGame() {
                Amara::Error* e = nullptr;
                e->breakGame();
            }
    };

    void log(std::string msg) {
        SDL_Log(msg.c_str());
    }

    void error(std::string msg) {
        std::cout << "Amara::Error: " << msg << std::endl;
        Amara::Error* e = nullptr;
        e->breakGame();
    }

    void breakGame() {
        Amara::Error* e = nullptr;
        e->breakGame();
    }
}

#endif
