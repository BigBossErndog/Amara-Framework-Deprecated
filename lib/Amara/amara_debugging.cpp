#ifndef AMARA_DEBUGGING
#define AMARA_DEBUGGING

#include "amara.h"

namespace Amara {
    class Error {
        public:
            breakGame() {
                Amara::Error* e = nullptr;
                e->breakGame();
            }
    };

    void log(string msg) {
        cout << msg << endl;
    }

    void error(string msg) {
        cout << "Amara::Error: " << msg << endl;
        Amara::Error* e = nullptr;
        e->breakGame();
    }

    void breakGame() {
        Amara::Error* e = nullptr;
        e->breakGame();
    }
}

#endif
