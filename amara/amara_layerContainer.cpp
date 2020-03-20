#ifndef AMARA_LAYER
#define AMARA_LAYER

#include "amara.h"

namespace Amara {
    class Layer: public Amara::Actor {
        public:
            Layer(): Amara::Actor() {}

            Layer(float gx, float gy): Layer() {
                x = gx;
                y = gy;
            }

            virtual void draw(int vx, int vy, int vw, int vh) {
                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };

    class Container: public Amara::Layer {
        public:
            float width = 0;
            float height = 0;

            float originX = 0;
            float originY = 0;

            Container(): Amara::Layer() {}
            Container(float w, float h): Container() {
                width = w;
                height = h;
            }
            Container(float gx, float gy, float w, float h): Container(w, h) {
                x = gx;
                y = gy;
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                int dx = 0, dy = 0, dw = 0, dh = 0, ox = 0, oy = 0;

                dx = floor(floor(x - properties->scrollX*scrollFactorX + properties->offsetX - (originX*width*scaleX)) * properties->zoomX);
                dy = floor(floor(y - properties->scrollY*scrollFactorY + properties->offsetY - (originY*height*scaleY)) * properties->zoomY);
                dw = width * scaleX * properties->zoomX;
                dh = height * scaleY * properties->zoomY;

                if (dx < 0) {
                    dw += dx;
                    ox += dx/(scaleX*properties->zoomX);
                    dx = 0;
                }
                if (dy < 0) {
                    dh += dy;
                    oy += dy/(scaleY*properties->zoomY);
                    dy = 0;
                }
                if (dx + dw > vw) {
                    dw = vw - dx;
                }
                if (dy + dh > vh) {
                    dh = vh - dy;
                }

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;
				
				stable_sort(entities.begin(), entities.end(), sortEntities());

                float recZoomX = properties->zoomX;
                float recZoomY = properties->zoomY;

				for (Amara::Entity* entity : entities) {
                    properties->scrollX = 0;
                    properties->scrollY = 0;
                    properties->offsetX = ox;
                    properties->offsetY = oy;
                    properties->zoomX = recZoomX * scaleX;
                    properties->zoomY = recZoomY * scaleY;
					if (entity->isDestroyed || entity->parent != this) continue;
					entity->draw(vx + dx, vy + dy, dw, dh);
				}
            }

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
            }
            void setOrigin(float g) {
                setOrigin(g, g);
            }
    };
}

#endif