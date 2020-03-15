#ifndef AMARA_TILEMAP
#define AMARA_TILEMAP

#include "amara.h"

namespace Amara {
    class Tilemap: public Amara::Actor {
        public:
            string textureKey;
            string tiledJsonKey;
            json tiledJson;

            int width = 0;
            int height = 0;
            
            int widthInPixels = 0;
            int heightInPixels = 0;

            int tileWidth = 0;
            int tileHeight = 0;

            unordered_map<string, Amara::TilemapLayer*> layers;

            Tilemap(): Amara::Actor() {}

            Tilemap(float gx, float gy, string gTextureKey) {
                x = gx;
                y = gy;
                textureKey = gTextureKey;
            }
            
            Tilemap(float gx, float gy, string gTextureKey, string gTiledJsonKey): Tilemap(gx, gy, gTextureKey) {
                tiledJsonKey = gTiledJsonKey;
            }

            Tilemap(string gTextureKey, string gTiledJsonKey): Tilemap(0, 0, gTextureKey, gTiledJsonKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                Amara::Actor::init(gameProperties, givenScene, givenParent);
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonAsset*)load->get(tiledJsonKey))->jsonObj;

                    if (tiledJson != nullptr) {
                        width = tiledJson["width"];
                        height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];
                    }
                }
            }

            Amara::TilemapLayer* add(Amara::TilemapLayer* gLayer) {
                Amara::Actor::add(gLayer);
                return gLayer;
            }

            Amara::TilemapLayer* createLayer(int mapWidth, int mapHeight, int tileWidth, int tileHeight) {
                Amara::TilemapLayer* newLayer;
                ((Amara::Entity*)scene)->add(newLayer = new Amara::TilemapLayer(mapWidth, mapHeight, tileWidth, tileHeight));
                newLayer->setTexture(textureKey);
                return newLayer;
            }

            Amara::TilemapLayer* createLayer(string layerKey) {
                if (tiledJsonKey.empty()) return nullptr;
                cout << "Creating Layer: \"" << layerKey << "\" with tilemap \"" << tiledJsonKey << "\"" << endl;
                Amara::TilemapLayer* newLayer;
                ((Amara::Entity*)scene)->add(newLayer = new Amara::TilemapLayer(textureKey, tiledJsonKey, layerKey));
                layers[layerKey] = newLayer;
                return newLayer;
            }

            Amara::TilemapLayer* getLayer(string layerKey) {
                if (layers.find(layerKey) != layers.end()) {
                    return layers[layerKey];
                }
                return nullptr;
            }

            void createAllLayers() {
                if (tiledJsonKey.empty()) return;
                int numLayers = tiledJson["layers"].size();
                for (size_t l = 0; l < numLayers; l++) {
                    createLayer(tiledJson["layers"][l]["name"]);
                }
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