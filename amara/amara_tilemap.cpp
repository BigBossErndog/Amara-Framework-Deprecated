#pragma once
#ifndef AMARA_TILEMAP
#define AMARA_TILEMAP



namespace Amara {
    class Tilemap: public Amara::Actor, public Amara::WallFinder {
        public:
            std::string textureKey;
            std::string tiledJsonKey;
            nlohmann::json tiledJson;

            int width = 0;
            int height = 0;
            
            int widthInPixels = 0;
            int heightInPixels = 0;

            int tileWidth = 0;
            int tileHeight = 0;

            bool offMapIsWall = true;

            std::unordered_map<std::string, Amara::TilemapLayer*> layers;
            std::vector<Amara::TilemapLayer*> walls;
            std::unordered_map<int, Amara::Direction> wallTypes;

            Tilemap(): Amara::Actor() {}

            Tilemap(float gx, float gy, std::string gTextureKey) {
                x = gx;
                y = gy;
                textureKey = gTextureKey;
            }
            
            Tilemap(float gx, float gy, std::string gTextureKey, std::string gTiledJsonKey): Tilemap(gx, gy, gTextureKey) {
                tiledJsonKey = gTiledJsonKey;
            }

            Tilemap(std::string gTextureKey, std::string gTiledJsonKey): Tilemap(0, 0, gTextureKey, gTiledJsonKey) {}

            void configure(nlohmann::json mapping) {
                if (mapping.find("texture") != mapping.end()) {
                    setTexture(mapping["texture"]);
                }

				if (mapping.find("json") != mapping.end()) {
					setTiledJson(mapping["json"]);
					createAllLayers();
				}

				if (mapping.find("mapWidth") != mapping.end()) width = mapping["mapWidth"];
                if (mapping.find("mapHeight") != mapping.end()) height =  mapping["mapHeight"];
                if (mapping.find("tileWidth") != mapping.end()) tileWidth = mapping["tileWidth"] ;
                if (mapping.find("tileHeight") != mapping.end()) tileHeight = mapping["tileHeight"];
				widthInPixels = width * tileWidth;
				heightInPixels = height * tileHeight;
				
				if (mapping.find("layers") != mapping.end() && mapping["layers"].is_array()) {
					nlohmann::json& jlayers = mapping["layers"];
					for (nlohmann::json& jlayer: jlayers) {
						std::string layerKey = jlayer["key"];
						TilemapLayer* tilemapLayer = getLayer(layerKey);
						if (tilemapLayer == nullptr) tilemapLayer = createLayer(layerKey, width, height, tileWidth, tileHeight);
						tilemapLayer->configure(jlayer);

						if (jlayer.find("isWall") != jlayer.end() && jlayer["isWall"]) {
							setWall(layerKey);
						}
					}
				}

				if (mapping.find("layer") != mapping.end() && mapping["layer"].is_object()) {
					nlohmann::json& jlayer = mapping["layer"];
					std::string layerKey = jlayer["key"];
					TilemapLayer* tilemapLayer = getLayer(layerKey);
					if (tilemapLayer == nullptr) tilemapLayer = createLayer(layerKey, width, height, tileWidth, tileHeight);
					tilemapLayer->configure(jlayer);

					if (jlayer.find("isWall") != jlayer.end() && jlayer["isWall"]) {
						setWall(layerKey);
					}
				}

				if (mapping.find("walls") != mapping.end() && mapping["walls"].is_array()) {
					setWalls(mapping["walls"]);
				}
				if (mapping.find("wall") != mapping.end() && mapping["wall"].is_string()) {
					setWall(mapping["wall"]);
				}
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                Amara::Actor::init(gameProperties, givenScene, givenParent);
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonFile*)load->get(tiledJsonKey))->jsonObj;

                    if (tiledJson != nullptr) {
                        if (tiledJson["width"] > width) width = tiledJson["width"];
                        if (tiledJson["height"] > height) height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];
                    }
                }
                
                entityType = "tilemap";
            }

            void setTiledJson(std::string gTiledJsonKey) {
                tiledJsonKey = gTiledJsonKey;
				walls.clear();
                if (!tiledJsonKey.empty()) {
                    Amara::JsonFile* jsonAsset = (Amara::JsonFile*)load->get(tiledJsonKey);
					
                    if (jsonAsset != nullptr) {
                        tiledJson = jsonAsset->getJSON();
                        if (tiledJson["width"] > width) width = tiledJson["width"];
                        if (tiledJson["height"] > height) height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];
                        widthInPixels = tileWidth * width;
                        heightInPixels = tileHeight * height;
                    }
                    else {
                        SDL_Log("Tiled JSON \"%s\" was not found.", gTiledJsonKey.c_str());
                    }
                }
            }

            bool setTexture(std::string gTextureKey) {
                Amara::Loader* load = properties->loader;
                Amara::ImageTexture* texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                    textureKey = texture->key;
                    return true;
                }
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
                }
                return false;
            }

            Amara::TilemapLayer* add(std::string gid, Amara::TilemapLayer* gLayer) {
                ((Amara::Entity*)scene)->add(gLayer);
                gLayer->setTexture(textureKey);
                gLayer->setTilemap(this, this);

                tileWidth = gLayer->tileWidth;
                tileHeight = gLayer->tileHeight;
                widthInPixels = gLayer->tileWidth*gLayer->width;
                heightInPixels = gLayer->tileHeight*gLayer->height;

                gLayer->id = gid;
                layers[gid] = gLayer;
                return gLayer;
            }

            Amara::TilemapLayer* createLayer(std::string layerKey, int mapWidth, int mapHeight, int tileWidth, int tileHeight) {
                Amara::TilemapLayer* newLayer;
                ((Amara::Entity*)scene)->add(newLayer = new Amara::TilemapLayer(mapWidth, mapHeight, tileWidth, tileHeight));

                newLayer->id = layerKey;
                newLayer->setTexture(textureKey);
                newLayer->setTilemap(this, this);

                tileWidth = newLayer->tileWidth;
                tileHeight = newLayer->tileHeight;
                widthInPixels = tileWidth * width;
                heightInPixels = tileHeight * height;

                layers[layerKey] = newLayer;
                return newLayer;
            }

            Amara::TilemapLayer* createLayer(std::string layerKey, float gx, float gy) {
                if (tiledJsonKey.empty()) return nullptr;
                Amara::TilemapLayer* newLayer;
                parent->add(newLayer = new Amara::TilemapLayer(textureKey, tiledJsonKey, layerKey));
                layers[layerKey] = newLayer;
                if (newLayer->width > width) width = newLayer->width;
                if (newLayer->height > height) height = newLayer->height;

                newLayer->id = layerKey;

                newLayer->x = x;
                newLayer->y = y;
                newLayer->setTilemap(this, this);

                tileWidth = newLayer->tileWidth;
                tileHeight = newLayer->tileHeight;
                widthInPixels = tileWidth * width;
                heightInPixels = tileHeight * height;
                return newLayer;
            }

            Amara::TilemapLayer* createLayer(std::string layerKey) {
                return createLayer(layerKey, 0, 0);
            }

            Amara::TilemapLayer* createEmptyLayer(std::string layerKey) {
                Amara::TilemapLayer* newLayer;
                parent->add(newLayer = new Amara::TilemapLayer(width, height, tileWidth, tileHeight));
                layers[layerKey] = newLayer;

                if (!textureKey.empty()) {
                    newLayer->setTexture(textureKey);
                }
                
                newLayer->id = layerKey;

                newLayer->x = x;
                newLayer->y = y;
                newLayer->setTilemap(this, this);

                return newLayer;
            }

            void createAllLayers() {
                if (tiledJsonKey.empty()) return;
                int numLayers = tiledJson["layers"].size();
                std::string layerKey;
                std::string layerType;
                for (size_t l = 0; l < numLayers; l++) {
                    layerKey = tiledJson["layers"][l]["name"];
                    layerType = tiledJson["layers"][l]["type"];
                    if (layerType.compare("tilelayer") == 0) {
                        createLayer(layerKey);
                    }
                }
            }

            void createObjectLayer(bool check, nlohmann::json layerData, void(*createFunc)(Amara::Tilemap*, nlohmann::json, int)) {
                std::string layerType = layerData["type"];
                if (layerType.compare("objectgroup") != 0) return;
                int dataSize = layerData["objects"].size();
                for (int i = 0; i < dataSize; i++) {
                    nlohmann::json data = layerData["objects"][i];
                    int gid = data["gid"];
                    int id = convertGID(gid);
                    createFunc(this, data, id);
                }
            }

            void createObjectLayer(std::string fLayerKey, void(*createFunc)(Amara::Tilemap*, nlohmann::json, int)) {
                if (tiledJsonKey.empty()) return;
                int numLayers = tiledJson["layers"].size();
                std::string layerKey;
                for (size_t l = 0; l < numLayers; l++) {
                    layerKey = tiledJson["layers"][l]["name"];
                    if (layerKey.compare(fLayerKey) == 0) {
                        createObjectLayer(true, tiledJson["layers"][l], createFunc);
                    }
                }
            }

            void createAllObjectLayers(void(*createFunc)(Amara::Tilemap*, nlohmann::json, int)) {
                if (tiledJsonKey.empty()) return;
                int numLayers = tiledJson["layers"].size();
                for (size_t l = 0; l < numLayers; l++) {
                    createObjectLayer(true, tiledJson["layers"][l], createFunc);
                }
            }

            nlohmann::json getTiledLayerData(std::string fLayerKey) {
                nlohmann::json toReturn;
                if (tiledJsonKey.empty()) return toReturn;
                int numLayers = tiledJson["layers"].size();
                std::string layerKey;
                for (size_t l = 0; l < numLayers; l++) {
                    layerKey = tiledJson["layers"][l]["name"];
                    if (layerKey.compare(fLayerKey) == 0) {
                        toReturn  = tiledJson["layers"][l];
                    }
                }
                return toReturn;
            } 

            Amara::TilemapLayer* getLayer(std::string layerKey) {
                if (layers.find(layerKey) != layers.end()) {
                    return layers[layerKey];
                }
                return nullptr;
            }

            void destroyLayer(std::string layerKey) {
                TilemapLayer* layer = getLayer(layerKey);
                if (layer) {
                    layers.erase(layerKey);
                    layer->destroy();
                }
            }

            std::vector<Amara::TilemapLayer*> setWalls(std::vector<std::string> wallKeys) {
                Amara::TilemapLayer* layer;
                for (std::string layerKey: wallKeys) {
                    layer = getLayer(layerKey);
                    if (layer) {
                        walls.push_back(layer);
                    }
                }
                return walls;
            }

			std::vector<Amara::TilemapLayer*> setWall(std::string wallKey) {
				Amara::TilemapLayer* layer = getLayer(wallKey);
				if (layer) {
					walls.push_back(layer);
				}
				return walls;
			}

            virtual bool isWall(int gx, int gy) {
                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAt(gx, gy);
                    if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
            }

            bool isWallAtXY(int gx, int gy) {
                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAtXY(gx, gy);
                    if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
            }

            virtual bool isWall(int gx, int gy, Amara::Direction dir) {
                if (gx < 0 || gy < 0) return (offMapIsWall) ? true : false;
                if (gx >= width || gy >= height) return (offMapIsWall) ? true : false;

                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAt(gx, gy);
                    if (wallTypes.find(tile.id) != wallTypes.end()) {
                        if (wallTypes[tile.id] == dir) {
                            return true;
                        }
                    }
                    else if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
            }
            
            void setWallId(int id, Amara::Direction dir) {
                wallTypes[id] = dir;
            }

            void run() {
                Amara::Actor::run();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::Actor::draw(vx, vy, vw, vh);
            }

            virtual int getMapWidth() {
                return width;
            }
            virtual int getMapHeight() {
                return height;
            }

            void setCameraBounds(Amara::Camera* cam) {
                cam->setBounds(x, y, widthInPixels, heightInPixels);
            }

            int convertGID(unsigned long id) {
                if (!tiledJson.empty() && tiledJson.find("tilesets") != tiledJson.end()) {
                    int firstgid = tiledJson["tilesets"][0]["firstgid"];
                    return tiledGID2ID(id, firstgid);
                }
                return -1;
            }

            float getTileX(int tx) {
                return tx * tileWidth;
            }
            float getTileY(int ty) {
                return ty * tileHeight;
            }
            float getMidTileX(int tx) {
                return (tx + 0.5) * tileWidth;
            }
            float getMidTileY(int ty) {
                return (ty + 0.5) * tileHeight; 
            }

            void destroy() {
                Amara::TilemapLayer* layer;
                for (auto it = layers.begin(); it != layers.end(); it++) {
                    layer = it->second;
                    layer->tilemap = nullptr;
                }
                layers.clear();
                walls.clear();
                Amara::Actor::destroy();
            }
    };

    void Amara::TilemapLayer::destroy() {
        if (tilemap != nullptr) {
            if (tilemap->layers.find(id) != tilemap->layers.end()) {
                tilemap->layers.erase(id);
            }
            Amara::TilemapLayer* layer;
            for (auto it = tilemap->walls.begin(); it != tilemap->walls.begin(); ++it) {
                layer = *it;
                if (layer == this) {
                    tilemap->walls.erase(it--);
                    continue;
                }
            }
        }
        
        Amara::Actor::destroy();
    }
}

#endif