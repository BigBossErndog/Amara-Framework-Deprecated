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

            vector<Amara::TilemapLayer*> layers;

            Tilemap(): Amara::Actor() {}
            
            Tilemap(float gx, float gy): Tilemap() {
                x = gx;
                y = gy;
            }

            Amara::TilemapLayer* add(Amara::TilemapLayer* gLayer) {
                layers.push_back(gLayer);
                return gLayer;
            }

            void run() {
                Amara::Actor::run();

                Amara::TilemapLayer* layer;
                for (size_t i = 0; i < layers.size(); i++) {
                    layer = layers.at(i);
                    layer->run();
                }
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::TilemapLayer* layer;
                for (size_t i = 0; i < layers.size(); i++) {
                    layer = layers.at(i);
                    layer->draw(vx, vy, vw, vh);
                }

                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}

#endif