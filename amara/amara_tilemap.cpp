namespace Amara {
    class Tilemap;

    int tiledGID2ID(unsigned long tilegid, unsigned long firstgid) {
        bool fhorizontal = (tilegid & Amara::TILED_FLIPPEDHORIZONTALLY) != 0;
        bool fvertical = (tilegid & Amara::TILED_FLIPPEDVERTICALLY) != 0;
        bool fdiagonal = (tilegid & Amara::TILED_FLIPPEDANTIDIAGONALLY) != 0;
        
        tilegid = tilegid & ~(Amara::TILED_FLIPPEDHORIZONTALLY | Amara::TILED_FLIPPEDVERTICALLY | Amara::TILED_FLIPPEDANTIDIAGONALLY);
        int tileId = (int)(tilegid - firstgid);

        return tileId;
    }

    class TilemapLayer: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;
            Amara::ImageTexture* texture = nullptr;
            SDL_Texture* drawTexture = nullptr;
            std::string textureKey;

            bool givenTiledJson = false;
            nlohmann::json tiledJson;
            std::string tiledJsonKey;

            std::string tiledLayerKey;

            std::vector<Amara::Tile> tiles;

            std::unordered_map<int, Amara::PhysicsProperties> tilePhysics;

            Amara::Tilemap* tilemap = nullptr;
            Amara::Entity* tilemapEntity = nullptr;
            
            int width = 0;
            int height = 0;

            float originX = 0;
            float originY = 0;

            float scaleX = 1;
            float scaleY = 1;

            SDL_Rect viewport;
            SDL_Rect srcRect;
			SDL_Rect destRect;
            SDL_FRect destRectF;
            SDL_FPoint origin;
			SDL_Point originN;
            SDL_RendererFlip flip;

            bool pixelLocked = false;

            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;
            
            int tileWidth = 0;
            int tileHeight = 0;

            int imageWidth = 0;
            int imageHeight = 0;

            int widthInPixels = 0;
            int heightInPixels = 0;

            bool extruded = false;

            bool pleaseUpdate = true;
            bool textureLocked = false;

            Amara::Tile nullTile = Amara::Tile();

            std::unordered_map<int, Amara::TileAnimation> animations;

            TilemapLayer(float gw, float gh, float tw, float th): Amara::Actor() {
                width = gw;
                height = gh;
                tileWidth = tw;
                tileHeight = th;

                widthInPixels = width * tileWidth;
                heightInPixels = height * tileHeight;
                imageWidth = widthInPixels;
                imageHeight = heightInPixels;

                Amara::Tile tile;
                tiles.resize(width*height, tile);
                for (int t = 0; t < tiles.size(); t++) {
                    Tile& tile = tiles[t];
                    tile.x = (t % width);
                    tile.y = floor(((float)t) / (float)width);
                }
            }

            TilemapLayer(std::string gTextureKey, std::string gTiledJsonKey) {
                textureKey = gTextureKey;
                tiledJsonKey = gTiledJsonKey;
            }

            TilemapLayer(std::string gTextureKey, std::string gTiledJsonKey, std::string gLayerKey): TilemapLayer(gTextureKey, gTiledJsonKey) {
                tiledLayerKey = gLayerKey;
            }

            void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                textureLocked = textureLockDefault;

                Amara::Actor::init(gameProperties, givenScene, givenParent);
                gRenderer = properties->gRenderer;

                tilePhysics.clear();

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }
                if (!tiledJsonKey.empty()) {
                    setTiledJson(tiledJsonKey);
                }
                else {
                    createDrawTexture();
                }
                if (!tiledLayerKey.empty()) {
                    setupTiledLayer(tiledLayerKey);
                }

                entityType = "tilemapLayer";
            }

            void configure(nlohmann::json config) {
                Amara::Actor::configure(config);

                if (config.find("texture") != config.end()) {
                    setTexture(config["texture"]);
                }
                
                if (config.find("tiles") != config.end()) {
                    nlohmann::json& jtiles = config["tiles"];
                    for (int i = 0; i < jtiles.size(); i++) {
                        setTile(i, jtiles[i]);
                    }
                }
                if (config.find("extruded") != config.end()) {
                    extruded = config["extruded"];
                }
                if (config.find("textureLocked") != config.end()) {
                    setTextureLock(config["textureLocked"]);
                }
            }

            void setupLayer(std::string gTextureKey, std::string gTiledJsonKey, std::string gLayerKey) {
                textureKey = gTextureKey;
                tiledJsonKey = gTiledJsonKey;
                tiledLayerKey = gLayerKey;
                
                setTexture(textureKey);
                setTiledJson(tiledJsonKey);
                setupTiledLayer(gLayerKey);
            }

            void setTilemap(Amara::Tilemap* gTilemap, Amara::Entity* gTilemapEntity) {
                tilemap = gTilemap;
                tilemapEntity = gTilemapEntity;
            }

            void setTiledJson(std::string tiledJsonKey) {
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonFile*) load->get(tiledJsonKey))->jsonObj;
                    if (!tiledJson.empty()) {
                        givenTiledJson = true;

                        width = tiledJson["width"];
                        height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];

                        widthInPixels = width * tileWidth;
                        heightInPixels = height * tileHeight;
                        imageWidth = widthInPixels;
                        imageHeight = heightInPixels;

                        Amara::Tile tile;
                        tiles.resize(width*height, tile);
                        createDrawTexture();

                        animations.clear();
                        if (tiledJson.find("tilesets") != tiledJson.end()) {
                            for (nlohmann::json& tilesetJson: tiledJson["tilesets"]) {
                                if (tilesetJson.find("tiles") != tilesetJson.end()) {
                                    for (nlohmann::json& animData: tilesetJson["tiles"]) {
                                        int tileId = animData["id"];
                                        animations.insert(std::make_pair(tileId, Amara::TileAnimation(properties, animData)));
                                    }
                                }
                            }
                        }
                    }
                    else {
                        SDL_Log("JSON with key: \"%s\" was not found.", tiledJsonKey.c_str());
                    }
                }
            }

            void setupTiledLayer(std::string layerKey) {
                if (!givenTiledJson) return; 
                nlohmann::json layers = tiledJson["layers"];

                unsigned long tileId;
                int firstgid = tiledJson["tilesets"][0]["firstgid"];

                bool fhorizontal;
                bool fvertical;
                bool fdiagonal;

                int numLayers = layers.size();
                for (size_t l = 0; l < numLayers; l++) {
                    if (tiledLayerKey.compare(layers[l]["name"]) != 0) continue;
                    alpha = layers[l]["opacity"];
                    isVisible = layers[l]["visible"];

                    for (size_t t = 0; t < layers[l]["data"].size(); t++) {
                        tileId = (unsigned long)layers[l]["data"][t];

                        bool fhorizontal = (tileId & Amara::TILED_FLIPPEDHORIZONTALLY) != 0;
                        bool fvertical = (tileId & Amara::TILED_FLIPPEDVERTICALLY) != 0;
                        bool fdiagonal = (tileId & Amara::TILED_FLIPPEDANTIDIAGONALLY) != 0;
                        
                        Amara::Tile& tile = tiles.at(t);
                        tileId = tileId & ~(Amara::TILED_FLIPPEDHORIZONTALLY | Amara::TILED_FLIPPEDVERTICALLY | Amara::TILED_FLIPPEDANTIDIAGONALLY);
                        tile.id = (int)(tileId - firstgid);
                        tile.x = (t % width);
                        tile.y = floor(((float)t) / (float)width);
                        tile.fhorizontal = fhorizontal;
                        tile.fvertical = fvertical;
                        tile.fdiagonal = fdiagonal;
                    }
                }
            }

            void setupTiledLayer(std::string tiledJsonKey, std::string gLayerKey) {
                setTiledJson(tiledJsonKey);
                setupTiledLayer(gLayerKey);
            }

            void setupTiledLayer(std::vector<int> gTiles) {
                for (size_t j = 0; j < gTiles.size(); j++) {
                    tiles.at(j).id = gTiles.at(j);
                }
            }

            bool setTexture(std::string gTextureKey) {
                Amara::Loader* load = properties->loader;
                texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                    textureKey = texture->key;
                    return true;
                }
                else {
                    SDL_Log("Texture with key: \"%s\" was not found.", gTextureKey);
                }
                return false;
            }

            void setTextureLock(bool gLock) {
                textureLocked = gLock;
                pleaseUpdate = true;
            }

            void reloadAssets() {
                setTexture(textureKey);
            }

            int getTileIdAt(int gx, int gy) {
                return getTileAt(gx, gy).id;
            }

            Amara::Tile& getTileAt(int gx, int gy) {
                if (gx < 0) gx  = 0;
                if (gy < 0) gy = 0;
                if (gx >= width) gx = width - 1;
                if (gy >= height) gy = height - 1;
                int index = (gy * width) + gx;
                return tiles[index];
            }

            Amara::Tile& setTileAt(int gx, int gy, int nid) {
                if (gx < 0 || gx >= width) return nullTile;
                if (gy < 0 || gy >= height) return nullTile;
                Amara::Tile& tile = getTileAt(gx, gy);
                tile.id = nid;
                return tile;
            }

            Amara::Tile& getTileAtXY(float gx, float gy) {
                float px = 0;
                float py = 0;
                if (tilemapEntity) {
                    px = tilemapEntity->x;
                    py = tilemapEntity->y;
                }

                int fx = floor((gx - x - px)/tileWidth);
                int fy = floor((gy - y - py)/tileHeight);
                if (fx < 0) fx = 0;
                if (fx >= width) fx = width - 1;
                if (fy < 0) fy = 0;
                if (fy >= height) fy = height - 1;
                return getTileAt(fx, fy);
            }

            Amara::Tile& setTile(int index, int nid) {
                Amara::Tile& tile = tiles[index];
                tile.id = nid;
                return tile;
            }

            void setTiles(std::vector<int> given, int gx, int gy, int gw, int gh) {
                for (int i = gx; i < gx+gw; i++) {
                    for (int j = gy; j < gy+gh; j++) {
                        setTileAt(i, j, given[j*gw + i]);
                    }
                }
            }
            void setTiles(std::vector<int> given, int gw, int gh) {
                setTiles(given, 0, 0, gw, gh);
            }
            void setTiles(std::vector<int> given) {
                setTiles(given, width, height);
            }

            void fillTiles(int nid) {
                for (Amara::Tile& tile: tiles) {
                    tile.id = nid;
                    tile.fhorizontal = false;
                    tile.fvertical = false;
                    tile.fdiagonal = false;
                }
            }

            void clearTiles() {
                for (Amara::Tile& tile: tiles) {
                    tile.id = -1;
                    tile.fhorizontal = false;
                    tile.fvertical = false;
                    tile.fdiagonal = false;
                }
            }

            void clear() {
                for (Amara::Tile& tile: tiles) {
                    tile.id = -1;
                }
            }

            std::vector<int> getTileArray() {
                std::vector<int> arr;
                arr.clear();
                for (Tile& tile: tiles) {
                    arr.push_back(tile.id);
                }
                return arr;
            }

            std::vector<int> toVector(int* setW, int* setH) {
                std::vector<int> list;
                list.resize(width * height);
                for (int i = 0; i < tiles.size(); i++) {
                    list[i] = tiles[i].id;
                }
                if (setW != nullptr) *setW = width;
                if (setH != nullptr) *setH = height;
                return list;
            }
            std::vector<int> toVector() {
                return toVector(nullptr, nullptr);
            }

            void createDrawTexture() {
                if (drawTexture) SDL_DestroyTexture(drawTexture);
                drawTexture = SDL_CreateTexture(
                    properties->gRenderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    widthInPixels,
                    heightInPixels
                );
                pleaseUpdate = true;
            }

            void run() {
                std::unordered_map<int, Amara::TileAnimation>::iterator it = animations.begin();

                while(it != animations.end()) {
                    it->second.update();
                    it++;
                }
                Amara::Actor::run();
            }

            void drawTiles(int vx, int vy, int vw, int vh) {
                int frame, maxFrame = 0;
                float tileAngle = 0;
                float tx, ty;

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                SDL_SetTextureBlendMode(texture->asset, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(texture->asset, 255);
                SDL_SetRenderDrawColor(properties->gRenderer, 0, 0, 0, 0);
                SDL_RenderClear(properties->gRenderer);

                Amara::Camera* cam = properties->currentCamera;
                Amara::Tile& startTile = getTileAtXY(properties->scrollX, properties->scrollY);
                Amara::Tile& endTile = getTileAtXY(properties->scrollX+(vw/nzoomX), properties->scrollY+(vh/nzoomY));
                int startX = startTile.x;
                int startY = startTile.y;
                int endX = endTile.x;
                int endY = endTile.y;

                if (startX < 0) startX = 0;
                if (startY < 0) startY = 0;
                if (endX >= width) endX = width - 1;
                if (endY >= height) endY = height - 1;

                origin.x = 0;
                origin.y = 0;
                destRect.w = tileWidth;
                destRect.h = tileHeight;
                
                for (int i = startX; i <= endX; i++) {
                    for (int j = startY; j <= endY ; j++) {
                        Amara::Tile& tile = getTileAt(i, j);
                        frame = tile.id;

                        if (frame == -1) {
                            continue;
                        }

                        tileAngle = 0;
                        flip = SDL_FLIP_NONE;
                        if (tile.fhorizontal) {
                            if (tile.fvertical) {
                                if (tile.fdiagonal) {
                                    tileAngle = 90;
                                    flip = SDL_FLIP_VERTICAL;
                                }
                                else {
                                    tileAngle = 180;
                                }
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
                        
                        destRect.x = tx;
                        destRect.y = ty;

                        if (destRect.x + destRect.w <= 0) skipDrawing = true;
                        if (destRect.y + destRect.h <= 0) skipDrawing = true;
                        if (destRect.w <= 0) skipDrawing = true;
                        if (destRect.h <= 0) skipDrawing = true;

                        if (!skipDrawing) {
                            auto got = animations.find(frame);
                            if (got != animations.end()) {
                                frame = got->second.currentTileId;
                            }

                            if (texture != nullptr) {
                                SDL_Texture* tex = (SDL_Texture*)texture->asset;
                                maxFrame = ((texture->width / tileWidth) * (texture->height / tileHeight));
                                frame = frame % maxFrame;

                                srcRect.x = (frame % (texture->width / tileWidth)) * tileHeight;
                                srcRect.y = floor(frame / (texture->width / tileWidth)) * tileHeight;
                                srcRect.w = tileWidth;
                                srcRect.h = tileHeight;

                                SDL_SetTextureAlphaMod(tex, properties->alpha * tile.alpha * 255);
								
                                SDL_RenderCopyEx(
                                    gRenderer,
                                    tex,
                                    &srcRect,
                                    &destRect,
                                    angle + tileAngle,
                                    &originN,
                                    flip
                                );
                            }
                        }
                    }
                }
            }

            void drawAllTiles(int vx, int vy, int vw, int vh) {
                int frame, maxFrame = 0;
                float tileAngle = 0;
                float tx, ty;

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                SDL_SetTextureBlendMode(texture->asset, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(texture->asset, 255);
                SDL_SetRenderDrawColor(properties->gRenderer, 0, 0, 0, 0);
                SDL_RenderClear(properties->gRenderer);

                Amara::Camera* cam = properties->currentCamera;
                Amara::Tile& startTile = tiles[0];
                Amara::Tile& endTile = tiles[tiles.size() - 1];
                int startX = startTile.x;
                int startY = startTile.y;
                int endX = endTile.x;
                int endY = endTile.y;

                if (startX < 0) startX = 0;
                if (startY < 0) startY = 0;
                if (endX >= width) endX = width - 1;
                if (endY >= height) endY = height - 1;

                origin.x = 0;
                origin.y = 0;
                destRect.w = tileWidth;
                destRect.h = tileHeight;
                
                for (int i = startX; i <= endX; i++) {
                    for (int j = startY; j <= endY ; j++) {
                        Amara::Tile& tile = getTileAt(i, j);
                        frame = tile.id;

                        if (frame == -1) {
                            continue;
                        }

                        tileAngle = 0;
                        flip = SDL_FLIP_NONE;
                        if (tile.fhorizontal) {
                            if (tile.fvertical) {
                                if (tile.fdiagonal) {
                                    tileAngle = 90;
                                    flip = SDL_FLIP_VERTICAL;
                                }
                                else {
                                    tileAngle = 180;
                                }
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
                        
                        destRect.x = tx;
                        destRect.y = ty;

                        if (destRect.x + destRect.w <= 0) skipDrawing = true;
                        if (destRect.y + destRect.h <= 0) skipDrawing = true;
                        if (destRect.w <= 0) skipDrawing = true;
                        if (destRect.h <= 0) skipDrawing = true;

                        if (!skipDrawing) {
                            auto got = animations.find(frame);
                            if (got != animations.end()) {
                                frame = got->second.currentTileId;
                            }

                            if (texture != nullptr) {
                                SDL_Texture* tex = (SDL_Texture*)texture->asset;
                                maxFrame = ((texture->width / tileWidth) * (texture->height / tileHeight));
                                frame = frame % maxFrame;

                                srcRect.x = (frame % (texture->width / tileWidth)) * tileHeight;
                                srcRect.y = floor(frame / (texture->width / tileWidth)) * tileHeight;
                                srcRect.w = tileWidth;
                                srcRect.h = tileHeight;

                                SDL_SetTextureAlphaMod(tex, properties->alpha * tile.alpha * 255);
								
                                SDL_RenderCopyEx(
                                    gRenderer,
                                    tex,
                                    &srcRect,
                                    &destRect,
                                    angle + tileAngle,
                                    &originN,
                                    flip
                                );
                            }
                        }
                    }
                }
            }

            void draw(int vx, int vy, int vw, int vh) {
                if (!isVisible) return;
                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;
				
				if (properties->renderTargetsReset || properties->renderDeviceReset) {
					createDrawTexture();
				}

                SDL_Texture* recTarget = SDL_GetRenderTarget(properties->gRenderer);
                SDL_SetRenderTarget(properties->gRenderer, drawTexture);
                SDL_RenderSetViewport(gRenderer, NULL);
                

                if (!textureLocked || pleaseUpdate) {
                    pleaseUpdate = false;

                    if (textureLocked) drawAllTiles(vx, vy, vw, vh);
                    else drawTiles(vx, vy, vw, vh);
                }

                float px = 0;
                float py = 0;
                if (tilemapEntity) {
                    px = tilemapEntity->x;
                    py = tilemapEntity->y;
                }

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                SDL_SetRenderTarget(properties->gRenderer, recTarget);
                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                destRectF.x = ((x+px - properties->scrollX*scrollFactorX + properties->offsetX - (originX * imageWidth * scaleX)) * nzoomX);
                destRectF.y = ((y-z+py - properties->scrollY*scrollFactorY + properties->offsetY - (originY * imageHeight * scaleY)) * nzoomY);
                destRectF.w = (widthInPixels*scaleX*nzoomX);
                destRectF.h = (heightInPixels*scaleY*nzoomY);

                if (pixelLocked) {
                    destRectF.x = floor(destRectF.x);
                    destRectF.y = floor(destRectF.y);
                    destRectF.w = ceil(destRectF.w);
                    destRectF.h = ceil(destRectF.h);
                }

                origin.x = destRectF.w * originX;
                origin.y = destRectF.h * originY;
				
                SDL_SetTextureBlendMode(drawTexture, blendMode);
                SDL_SetTextureAlphaMod(drawTexture, properties->alpha * alpha * 255);

                SDL_RenderCopyExF(
                    properties->gRenderer,
                    drawTexture,
                    NULL,
                    &destRectF,
                    0,
                    &origin,
                    SDL_FLIP_NONE
                );

				checkHover(vx, vy, vw, vh, destRectF.x, destRectF.y, destRectF.w, destRectF.h);
                
                Amara::Actor::draw(vx, vy, vw, vh);
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

            bool isBlockingPath(int gx, int gy, bool allowDiagonal) {
                if (getTileIdAt(gx-1,gy) != -1 && getTileIdAt(gx+1,gy) != -1) return true;
                if (getTileIdAt(gx,gy-1) != -1 && getTileIdAt(gx,gy+1) != -1) return true;

                if (!allowDiagonal) {
                    if (getTileIdAt(gx-1,gy-1) != -1 && getTileIdAt(gx+1,gy-1) != -1) return true;
                    if (getTileIdAt(gx+1,gy-1) != -1 && getTileIdAt(gx+1,gy+1) != -1) return true;
                    if (getTileIdAt(gx+1,gy+1) != -1 && getTileIdAt(gx-1,gy+1) != -1) return true;
                    if (getTileIdAt(gx-1,gy+1) != -1 && getTileIdAt(gx-1,gy-1) != -1) return true;
                }
                return false;
            }

            bool isBlockingPath(int gx, int gy) {
                return isBlockingPath(gx, gy, false);
            }

            void destroy();

            static bool textureLockDefault;
            static void setTextureLockDefault(bool gLock) {
                textureLockDefault = gLock;
            }
    };
    bool TilemapLayer::textureLockDefault = true;
    
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
            std::list<Amara::TilemapLayer*> walls;
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
                    SDL_Log("Texture with key: \"%s\" was not found.", gTextureKey.c_str());
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

            std::list<Amara::TilemapLayer*> setWalls(std::vector<std::string> wallKeys) {
                Amara::TilemapLayer* layer;
                for (std::string layerKey: wallKeys) {
                    layer = getLayer(layerKey);
                    if (layer) {
                        walls.push_back(layer);
                    }
                }
                return walls;
            }

			std::list<Amara::TilemapLayer*> setWall(std::string wallKey) {
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
            for (auto it = tilemap->walls.begin(); it != tilemap->walls.begin();) {
                layer = *it;
                if (layer == this) {
                    it = tilemap->walls.erase(it);
                    continue;
                }
                ++it;
            }
        }

        if (drawTexture) SDL_DestroyTexture(drawTexture);
        
        Amara::Actor::destroy();
    }
}