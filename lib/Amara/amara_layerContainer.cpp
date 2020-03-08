#ifndef AMARA_LAYER
#define AMARA_LAYER

#include "amara.h"

namespace Amara {
    class Layer: public Amara::Actor {
        public:
            Layer(): Amara::Actor() {}
    }

    class Container: public Amara::Camera {
        public:
            float width = 0;
            float height = 0;

            Container(): Amara::Container() {}
            Container(float w, float h): Container() {
                width = w;
                height = h;
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                int dx, dy, dw, dh, ow, oh = 0;

                dx = vx + floor(x);
                if (dx < vx) {
                    ow = vx - dx;
                    dx = vx;
                }

                dy = vy + floor(y);
                if (dy < vy) {
                    oh = vy - dy;
                    dy = vy;
                }

                dw = (x + width > vw) ? ceil(vw - x) : width;
                dw -= ow;
                dh = (y + height > vh) ? ceil(vh - y) : height;
                dh -= oh;

                Amara::Entity* entity;
                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    entity->draw(dx, dy, dw, dh);
                }
            }
    }
}

#endif