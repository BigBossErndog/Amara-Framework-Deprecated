#include "amara.h"

class WhatScene: public Amara::Scene {
    public:
        void create() {
            cout << "HELLO WHAT" << endl;
        }
};