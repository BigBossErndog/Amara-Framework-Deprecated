#ifndef AMARA_TILEMAPLAYER
#define AMARA_TILEMAPLAYER

#include "amara.h"

namespace Amara {
    class TilemapLayer: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;
            Amara::ImageTexture* texture = nullptr;
            string textureKey;

            bool givenTiledJson = false;
            json tiledJson;
            string tiledJsonKey;

            string tiledLayerKey;

            vector<Amara::Tile> tiles;
            
            int width = 0;
            int height = 0;

            float originX = 0;
            float originY = 0;

            float scaleX = 1;
            float scaleY = 1;

            float angle = 0;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_Rect destRect;
            SDL_Point origin;
            SDL_RendererFlip flip;
            
            int tileWidth = 0;
            int tileHeight = 0;

            int imageWidth = 0;
            int imageHeight = 0;

            int widthInPixels = 0;
            int heightInPixels = 0;

            TilemapLayer(float gw, float gh, float tw, float th): Amara::Actor() {
                width = gw;
                height = gh;
                tileWidth = tw;
                tileHeight = th;

                Amara::Tile tile;
                tiles.resize(width*height, tile);
            }

            TilemapLayer(string gTextureKey, string gTiledJsonKey) {
                textureKey = gTextureKey;
                tiledJsonKey = gTiledJsonKey;
            }

            TilemapLayer(string gTextureKey, string gTiledJsonKey, string gLayerKey): TilemapLayer(gTextureKey, gTiledJsonKey) {
                tiledLayerKey = gLayerKey;
            }

            void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                Amara::Actor::init(gameProperties, givenScene, givenParent);
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }
                if (!tiledJsonKey.empty()) {
                    setTiledJson(tiledJsonKey);
                }
                if (!tiledLayerKey.empty()) {
                    setupTiledLayer(tiledLayerKey);
                }
            }

            void setupLayer(string gTextureKey, string gTiledJsonKey, string gLayerKey) {
                textureKey = gTextureKey;
                tiledJsonKey = gTiledJsonKey;
                tiledLayerKey = gLayerKey;
                
                setTexture(textureKey);
                setTiledJson(tiledJsonKey);
                setupTiledLayer(gLayerKey);
            }

            void setTiledJson(string tiledJsonKey) {
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonFile*) load->get(tiledJsonKey))->jsonObj;
                    if (tiledJson != nullptr) {
                        tiledJsonKey.clear();
                        givenTiledJson = true;

                        width = tiledJson["width"];
                        height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];

                        widthInPixels = width * tileWidth;
                        heightInPixels = height * tileHeight;

                        Amara::Tile tile;
                        tiles.resize(width*height, tile);
                    }
                }
            }

            void setupTiledLayer(string layerKey) {
                if (!givenTiledJson) return; 
                json layers = tiledJson["layers"];

                unsigned int tileId;
                int firstgid = tiledJson["tilesets"][0]["firstgid"];

                bool fhorizontal;
                bool fvertical;
                bool fdiagonal;

                int numLayers = layers.size();
                for (size_t l = 0; l < numLayers; l++) {
                    if (tiledLayerKey.compare(layers[l]["name"]) != 0) continue;

                    for (size_t t = 0; t < layers[l]["data"].size(); t++) {
                        tileId = (unsigned int)layers[l]["data"][t];

                        fhorizontal = (tileId & Amara::TILED_FLIPPEDHORIZONTALLY) != 0;
                        fvertical = (tileId & Amara::TILED_FLIPPEDVERTICALLY) != 0;
                        fdiagonal = (tileId & Amara::TILED_FLIPPEDANTIDIAGONALLY) != 0;
                        
                        tileId = tileId & ~(Amara::TILED_FLIPPEDHORIZONTALLY | Amara::TILED_FLIPPEDVERTICALLY | Amara::TILED_FLIPPEDANTIDIAGONALLY);
                        tiles.at(t).id = (int)(tileId - firstgid);
                        tiles.at(t).x = (t % width);
                        tiles.at(t).y = floor(((float)t) / (float)width);
                        tiles.at(t).fhorizontal = fhorizontal;
                        tiles.at(t).fvertical = fvertical;
                        tiles.at(t).fdiagonal = fdiagonal;
                    }
                }
            }

            void setupTiledLayer(string tiledJsonKey, string gLayerKey) {
                setTiledJson(tiledJsonKey);
                setupTiledLayer(gLayerKey);
            }

            void setupTiledLayer(vector<int> gTiles) {
                for (size_t j = 0; j < gTiles.size(); j++) {
                    tiles.at(j).id = gTiles.at(j);
                }
            }

            bool setTexture(string textureKey) {
                Amara::Loader* load = properties->loader;
                texture = (Amara::ImageTexture*)(load->get(textureKey));
                if (texture != nullptr) {
                    textureKey = texture->key;

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
                Amara::Tile tile;
                int tx, ty, frame, maxFrame = 0;
                float tileAngle = 0;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                for (Amara::Tile tile : tiles) {
                    frame = tile.id;

                    tileAngle = 0;
                    if (tile.fhorizontal) {
                        if (tile.fvertical) {
                            tileAngle = 180;
                        }
                        else if (tile.fdiagonal) {
                            tileAngle = 90;
                        }
                        else {
                            flip = SDL_FLIP_HORIZONTAL;
                        }
                    }
                    else if (tile.fvertical) {
                        if (tile.fdiagonal) {
                            tileAngle = -90;
                        }
                        else {
                            flip = SDL_FLIP_VERTICAL;
                        }
                    }
                    
                    tx = tile.x * tileWidth;
                    ty = tile.y * tileHeight;

                    bool skipDrawing = false;

                    destRect.x = floor(floor(x + tx - properties->scrollX - (originX * imageWidth)) * properties->zoomX);
                    destRect.y = floor(floor(y + ty - properties->scrollY - (originY * imageHeight)) * properties->zoomY);
                    destRect.w = ceil(ceil(tileWidth * scaleX) * properties->zoomX);
                    destRect.h = ceil(ceil(tileHeight * scaleY) * properties->zoomY);

                    origin.x = destRect.w * originX + destRect.w/2;
                    origin.y = destRect.h * originY + destRect.h/2;

                    int hx, hy, hw, hh = 0;
                    hw = destRect.w;
                    hh = destRect.h;

                    if (destRect.x >= 0) {
                        hx = destRect.x + vx;
                    }
                    else {
                        hw -= -(destRect.x);
                        hx = vx;
                    }
                    if (destRect.y >= 0) {
                        hy = destRect.y + vy;
                    }
                    else {
                        hh -= -(destRect.h);
                        hy = vy;
                    }
                    if (hx + hw > vx + vw) hw = (vx - hx);
                    if (hy + hh > vy + vh) hh = (vy - hy);

                    checkForHover(hx, hy, hw, hh);

                    if (destRect.w <= 0) skipDrawing = true;
                    if (destRect.h <= 0) skipDrawing = true;

                    if (!skipDrawing) {
                        if (texture != nullptr) {
                            SDL_Texture* tx = (SDL_Texture*)texture->asset;
                            maxFrame = ((texture->width / tileWidth) * (texture->height / tileHeight));
                            frame = frame % maxFrame;

                            srcRect.x = (frame % (texture->width / tileWidth)) * tileHeight;
                            srcRect.y = floor(frame / (texture->width / tileWidth)) * tileHeight;
                            srcRect.w = tileWidth;
                            srcRect.h = tileHeight;

                            SDL_RenderCopyEx(
                                gRenderer,
                                (SDL_Texture*)(texture->asset),
                                &srcRect,
                                &destRect,
                                angle + tileAngle,
                                &origin,
                                flip
                            );
                        }
                    }
                }

                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}

#endif