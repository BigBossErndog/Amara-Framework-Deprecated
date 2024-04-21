namespace Amara {
    class Image: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            Amara::ImageTexture* texture = nullptr;
            std::string textureKey;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_FRect destRect;
            SDL_FPoint origin;

            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

            int width = 0;
            int height = 0;
            int imageWidth = 0;
            int imageHeight = 0;

            int cropTop = 0;
            int cropBottom = 0;
            int cropLeft = 0;
            int cropRight = 0;

            int frame = 0;
            int maxFrame = 0;

            float originX = 0;
            float originY = 0;

            float renderOffsetX = 0;
            float renderOffsetY = 0;

            bool flipHorizontal = false;
            bool flipVertical = false;

            bool onCamera = false;

            Image(): Actor() {
                textureKey.clear();
            }
            Image(float gx, float gy) {
                x = gx;
                y = gy;
            }
            Image(std::string gKey): Amara::Actor() {
                textureKey = gKey;
            }
            Image(float gx, float gy, std::string gKey): Image(gKey) {
                x = gx;
                y = gy;
            }

            Image(FloatVector2 v2): Image(v2.x, v2.y) {}
            Image(FloatVector2 v2, std::string gKey): Image(v2.x, v2.y, gKey) {}
            Image(FloatVector3 v3): Image(v3.x, v3.y) {
                z = v3.z;
            }
            Image(FloatVector3 v3, std::string gKey): Image(v3.x, v3.y, gKey) {
                z = v3.z;
            }
            
            using Amara::Actor::init;
            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }

                Amara::Actor::init(gameProperties, givenScene, givenParent);

                entityType = "image";
			}

            virtual void configure(nlohmann::json config) {
                Amara::Actor::configure(config);
                if (config.find("texture") != config.end()) {
                    setTexture(config["texture"]);
                }
                if (config.find("frame") != config.end()) {
                    frame = config["frame"];
                }
                if (config.find("originX") != config.end()) {
                    originX = config["originX"];
                }
                if (config.find("originY") != config.end()) {
                    originY = config["originY"];
                }
                if (config.find("origin") != config.end()) {
                    originX = config["origin"];
                    originY = config["origin"];
                }
                if (config.find("originPositionX") != config.end()) {
                    originX = config["originPositionX"];
                    originX = originX/imageWidth;
                }
                if (config.find("originPositionY") != config.end()) {
                    originY = config["originPositionY"];
                    originY = originY/imageHeight;
                }
                if (config.find("originPosition") != config.end()) {
                    originX = config["originPosition"];
                    setOriginPosition(originX, originX);
                }
                if (config.find("flipHorizontal") != config.end()) {
                    flipHorizontal = config["flipHorizontal"];
                }
                if (config.find("flipVertical") != config.end()) {
                    flipVertical = config["flipVertical"];
                }
                if (config.find("renderOffsetX") != config.end()) {
                    renderOffsetX = config["renderOffsetX"];
                }
                if (config.find("renderOffsetY") != config.end()) {
                    renderOffsetY = config["renderOffsetY"];
                }
                if (config.find("cropLeft") != config.end()) {
                    cropLeft = config["cropLeft"];
                }
                if (config.find("cropRight") != config.end()) {
                    cropRight = config["cropRight"];
                }
                if (config.find("cropTop") != config.end()) {
                    cropTop = config["cropTop"];
                }
                if (config.find("cropBottom") != config.end()) {
                    cropBottom = config["cropBottom"];
                }
            }

            virtual nlohmann::json toData() {
                nlohmann::json config = Amara::Actor::toData();
                if (texture != nullptr) config["texture"] = texture->key;
                config["frame"] = frame;
                config["originX"] = originX;
                config["originY"] = originY;
                config["flipHorizontal"] = flipHorizontal;
                config["flipVertical"] = flipVertical;
				config["cropLeft"] = cropLeft;
				config["cropRight"] = cropRight;
				config["cropTop"] = cropTop;
				config["cropBottom"] = cropBottom;
				config["renderOffsetX"] = renderOffsetX;
				config["renderOffsetY"] = renderOffsetY;
                return config;
            }

            virtual void drawTexture(int vx, int vy, int vw, int vh) {
                bool skipDrawing = false;
                onCamera = false;

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                bool scaleFlipHorizontal = false;
                bool scaleFlipVertical = false;
                float recScaleX = scaleX;
                float recScaleY = scaleY;

                if (scaleX < 0) {
                    scaleFlipHorizontal = true;
                    scaleX = abs(scaleX);
                }
                if (scaleY < 0) {
                    scaleFlipVertical = true;
                    scaleY = abs(scaleY);
                }

                float rotatedX = (x+renderOffsetX+cropLeft - properties->scrollX*scrollFactorX + properties->offsetX - (originX * imageWidth * scaleX));
                float rotatedY = (y-z+renderOffsetY+cropTop - properties->scrollY*scrollFactorY + properties->offsetY - (originY * imageHeight * scaleY));

                destRect.x = (rotatedX * nzoomX);
                destRect.y = (rotatedY * nzoomY);
                destRect.w = (((imageWidth-cropLeft-cropRight) * scaleX) * nzoomX);
                destRect.h = (((imageHeight-cropTop-cropBottom) * scaleY) * nzoomY);

                scaleX = recScaleX;
                scaleY = recScaleY;

                origin.x = destRect.w * originX;
                origin.y = destRect.h * originY;

                if (destRect.x + destRect.w <= 0) skipDrawing = true;
                if (destRect.y + destRect.h <= 0) skipDrawing = true;
                if (destRect.x >= vw) skipDrawing = true;
                if (destRect.y >= vh) skipDrawing = true;
                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;
				
                if (!skipDrawing) {
                    onCamera = true;

                    if (texture != nullptr) {
                        SDL_Texture* tx = (SDL_Texture*)texture->asset;
                        switch (texture->type) {
                            case IMAGE:
                                frame = 0;
                                srcRect.x = cropLeft;
                                srcRect.y = cropTop;
                                srcRect.w = imageWidth - cropLeft - cropRight;
                                srcRect.h = imageHeight - cropTop - cropBottom;
                                break;
                            case SPRITESHEET:
                                setFrame(frame);
                                Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;

                                srcRect.x = (frame % (texture->width / spr->frameWidth)) * spr->frameWidth + cropLeft;
                                srcRect.y = floor(frame / (texture->width / spr->frameWidth)) * spr->frameHeight + cropTop;
                                srcRect.w = spr->frameWidth - cropLeft - cropRight;
                                srcRect.h = spr->frameHeight - cropTop - cropBottom;
                                break;
                        }

                        SDL_SetTextureBlendMode(tx, blendMode);
				        SDL_SetTextureAlphaMod(tx, alpha * properties->alpha * 255);

                        SDL_RendererFlip flipVal = SDL_FLIP_NONE;
                        if (!flipHorizontal != !scaleFlipHorizontal) {
                            flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_HORIZONTAL);
                        }
                        if (!flipVertical != !scaleFlipVertical) {
                            flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_VERTICAL);
                        }

                        SDL_RenderCopyExF(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect,
                            angle + properties->angle,
                            &origin,
                            flipVal
                        );

						checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                drawTexture(vx, vy, vw, vh);
                Amara::Entity::draw(vx, vy, vw, vh);
            }

            virtual bool setTexture(std::string gTextureKey) {
                if (texture) removeTexture();
                if (load == nullptr || properties == nullptr) {
                    textureKey = gTextureKey;
                    return true;
                }

                texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                   textureKey = texture->key;

                    if (texture->type == SPRITESHEET) {
                        width = ((Amara::Spritesheet*)texture)->frameWidth;
                        height = ((Amara::Spritesheet*)texture)->frameHeight;
                        Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                        maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                        setFrame(frame);
                    }
                    else {
                        width = texture->width;
                        height = texture->height;
                        maxFrame = 0;
                        frame = 0;
                    }

                    imageWidth = width;
                    imageHeight = height;
                    
                    return true;
                }
                else {
                    SDL_Log("Texture with key: \"%s\" was not found.", gTextureKey.c_str());
                }
                return false;
            }

            void reloadAssets() {
                setTexture(textureKey);
            }

            bool setTexture(SDL_Texture* tx) {
                if (texture) removeTexture();
                texture = new ImageTexture("temp", IMAGE, tx);
                texture->temp = true;
                width = texture->width;
                height = texture->height;
                imageWidth = texture->width;
                imageWidth = texture->height;
                textureKey = "temp";
                maxFrame = 0;
                frame = 0;

                return true;
            }

            bool setTexture(SDL_Texture* tx, int frw, int frh) {
                if (texture) removeTexture();
                texture = new Spritesheet("temp", SPRITESHEET, tx, frw, frh);
                texture->temp = true;
                width = ((Amara::Spritesheet*)texture)->frameWidth;
                height = ((Amara::Spritesheet*)texture)->frameHeight;
                imageWidth = texture->width;
                imageWidth = texture->height;
                textureKey = "temp";
                maxFrame = 0;
                frame = 0;
                
                return true;
            }

            bool setTexture(ImageTexture* tx) {
                if (texture) removeTexture();
                texture = tx;

                textureKey = texture->key;

                if (texture->type == SPRITESHEET) {
                    width = ((Amara::Spritesheet*)texture)->frameWidth;
                    height = ((Amara::Spritesheet*)texture)->frameHeight;
                    Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                    maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                    setFrame(frame);
                }
                else {
                    width = texture->width;
                    height = texture->height;
                    maxFrame = 0;
                    frame = 0;
                }

                imageWidth = width;
                imageHeight = height;

                return true;
            }

            Amara::Image* removeTexture() {
                textureKey.clear();
                if (texture != nullptr && texture->temp) properties->taskManager->queueDeletion(texture);
                texture = nullptr;
                return this;
            }
            
            Amara::Image* setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
                return this;
            }
            Amara::Image* setOrigin(float g) {
                return setOrigin(g, g);
            }
            Amara::Image* setOriginPosition(float gx, float gy) {
                originX = gx/imageWidth;
                originY = gy/imageHeight;
                return this;
            }
            Amara::Image* setOriginPosition(float g) {
                return setOriginPosition(g, g);
            }

            Amara::Image* setRenderOffset(float gx, float gy) {
                renderOffsetX = gx;
                renderOffsetY = gy;
                return this;
            }

            Amara::Image* setRenderOffset(float gi) {
                return setRenderOffset(gi, gi);
            }

            Amara::Image* setFrame(int fr) {
                if (maxFrame == 0) frame = 0;
                else frame = fr % maxFrame;
                return this;
            }

            Amara::Image* scaleTo(float gw, float gh) {
                scaleX = gw/imageWidth;
                scaleY = gh/imageHeight;
                return this;
            }

            Amara::Image* scaleToWidth(float gw) { 
                scaleX = gw/imageWidth;
                scaleY = scaleX;
                return this;
            }
            Amara::Image* scaleToHeight(float gh) {
                scaleY = gh/imageHeight;
                scaleX = scaleY;
                return this;
            }

            Amara::Image* cropToSection(int gx, int gy, int gw, int gh) {
                cropLeft = gx;
                cropTop = gy;
                cropRight = width - gx - gw;
                cropBottom = height - gy - gh;
                return this;
            }

            using Amara::Actor::destroy;
            virtual void destroy(bool recursive) {
                removeTexture();
                Amara::Actor::destroy(recursive);
            }
    };
}
