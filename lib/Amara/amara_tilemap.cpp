#ifndef AMARA_TILEMAP
#define AMARA_TILEMAP

#include "amara.h"

namespace Amara {
    class Tilemap: public Amara::Actor {
        public:
            int width = 0;
            int height = 0;
            
            int widthInPixels = 0;
            int heightInPixels = 0;

            Tilemap(): Amara::Actor() {}
            
            Tilemap(float gx, float gy): Tilemap() {
                x = gx;
                y = gy;
            }

            Amara::TilemapLayer* add(Amara::TilemapLayer* gLayer) {
                Amara::Actor::add(gLayer);
                gLayer->init(properties, scene, this);
                return gLayer;
            }

            void run() {
                Amara::Actor::run();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}

#endif