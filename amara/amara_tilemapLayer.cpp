#ifndef AMARA_TILEMAPLAYER
#define AMARA_TILEMAPLAYER

#include "amara.h"

namespace Amara {
    enum TILEDFLAG {
        FlippedHorizontally = 0x80000000,
        FlippedVertically = 0x400000000,
        FlippedAntiDiagonally = 0x20000000
    };
    
    class TilemapLayer: public Amara::Actor {
        public:
            Amara::ImageTexture* texture = nullptr;
            string textureKey;

            vector<vector<Amara::Tile>> tiles;
            
            int width = 0;
            int height = 0;
            
            int tileWidth = 0;
            int tileHeight = 0;

            int imageWidth = 0;
            int imageHeight = 0;

            TilemapLayer(float gw, float gh, float tw, float th, string gTexture): Amara::Actor() {
                textureKey = gTexture;

                width = gw;
                height = gh;
                tileWidth = tw;
                tileHeight = th;

                Amara::Tile tile;
                tiles.resize(height, vector<Amara::Tile>(width, tile));
            }

            void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                if (!textureKey.empty()) {
                    setTexture(textureKey);
                    textureKey.clear();
                }

                Amara::Actor::init(gameProperties, givenScene, givenParent);
            }

            bool setTexture(string textureKey) {
                Amara::Loader* load = properties->loader;
                texture = (Amara::ImageTexture*)(load->get(textureKey));
                if (texture != nullptr) {
                    textureKey = texture->key;

                    if (texture->type == SPRITESHEET) {
                        width = ((Amara::Spritesheet*)texture)->frameWidth;
                        height = ((Amara::Spritesheet*)texture)->frameHeight;
                    }
                    else {
                        width = texture->width;
                        height = texture->height;
                    }

                    imageWidth = width;
                    imageHeight = height;
                    return true;
                }
                return false;
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