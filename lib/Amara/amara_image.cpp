#ifndef AMARA_IMAGE
#define AMARA_IMAGE

#include "amara.h"

namespace Amara {
    class Image : Amara::Entity {
        public:
            SDL_Rect viewport;
            Amara::Asset* asset = nullptr;

            virtual void draw(int vx, int vy, int vw, int vh) override {
                // dx = vx + x;
                // dy = vy + y;
                // dw = (x + width > vw) ? (vw - x) : vw;
                // dh = (y + height > vh) ? (vh - y) : vh;
                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;

                Amara::Entity::draw(vx, vy, vw, vh);
            }
    };
}

#endif