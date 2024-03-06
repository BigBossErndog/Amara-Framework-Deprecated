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
        SDL_Log("Amara::Error: ", msg.c_str());
        Amara::Error* e = nullptr;
        e->breakGame();
    }

    void breakGame() {
        Amara::Error* e = nullptr;
        e->breakGame();
    }
}
