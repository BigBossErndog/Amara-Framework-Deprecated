namespace Amara {
    class Canvas: public Actor {
        public:
            SDL_Texture* canvas = nullptr;
            SDL_Texture* recTarget = NULL;
            
            float width = 0;
            float height = 0;
            int imageWidth = 0;
            int imageHeight = 0;
            bool getLogicalDimensions = false;

            Amara::Image drawImage;

            bool clearEveryFrame = false;

            SDL_Color recColor;
            SDL_Rect drawnRect;
            SDL_FRect destRect;
            SDL_Rect viewport;
            SDL_FPoint origin;
            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

            bool pixelLocked = false;

            float originX = 0;
            float originY = 0;

            Canvas(): Actor() {
                getLogicalDimensions = true;
            }

            Canvas(float gx, float gy, float gw, float gh): Actor() {
                x = gx;
                y = gy;
                width = gw;
                height = gh;
            }

            void init(Amara::GameProperties* gameProperties, Amara::Scene* gScene, Amara::Entity* gParent) {
                drawImage.init(gameProperties, gScene, this);
                properties = gameProperties;
                if (canvas == nullptr) {
                    createNewCanvasTexture();
                }
                Amara::Actor::init(gameProperties, gScene, gParent);
                if (getLogicalDimensions) {
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

                entityType = "canvas";
            }

            void beginFill(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode gBlendMode) {
                recTarget = SDL_GetRenderTarget(properties->gRenderer);

                SDL_SetRenderTarget(properties->gRenderer, canvas);
                SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);
                SDL_SetRenderDrawColor(properties->gRenderer, r, g, b, a);
                SDL_SetRenderDrawBlendMode(properties->gRenderer, gBlendMode);

                resetPassOnProperties();
                
            } 
            void beginFill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
                beginFill(r, g, b, a, SDL_BLENDMODE_NONE);
            }
            void beginFill(Uint8 r, Uint8 g, Uint8 b) {
                beginFill(r, g, b, 255);
            }
            void beginFill() {
                beginFill(0, 0, 0);
            }

            void endFill() {
                SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
                SDL_SetRenderTarget(properties->gRenderer, recTarget);
            }

            void clear() {
                beginFill(0, 0, 0, 0);

                drawnRect.x = 0;
                drawnRect.y = 0;
                drawnRect.w = ceil(width);
                drawnRect.h = ceil(height);
                
                SDL_SetRenderDrawBlendMode(properties->gRenderer, SDL_BLENDMODE_NONE);
                SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_NONE);
                SDL_RenderClear(properties->gRenderer);

                endFill();
            }

            void createNewCanvasTexture() {
                if (canvas != nullptr) {
                    SDL_DestroyTexture(canvas);
                }
                canvas = SDL_CreateTexture(
                    properties->gRenderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    floor(width),
                    floor(height)
                );
                SDL_QueryTexture(canvas, NULL, NULL, &imageWidth, &imageHeight);
            }

            void fillRect(int rx, int ry, int rw, int rh) {
                drawnRect.x = rx;
                drawnRect.y = ry;
                drawnRect.w = rw;
                drawnRect.h = rh;
                SDL_RenderFillRect(properties->gRenderer, &drawnRect);
            }

            void fillPixel(int gx, int gy) {
                SDL_RenderDrawPoint(properties->gRenderer, gx, gy);
            }

            void copy(std::string textureKey, int gx, int gy, int gFrame, float originX, float originY, float scaleX, float scaleY, float angle) {
                drawImage.setTexture(textureKey);
                drawImage.x = gx;
                drawImage.y = gy;
                drawImage.frame = gFrame;
                drawImage.originX = originX;
                drawImage.originY = originY;
                drawImage.scaleX = scaleX;
                drawImage.scaleY = scaleY;
                drawImage.draw(0, 0, imageWidth, imageHeight);
            }

            void copy(std::string textureKey, int gx, int gy, int gFrame, float originX, float originY, float scaleX, float scaleY) {
                copy(textureKey, gx, gy, gFrame, originX, originY, scaleX, scaleY, 0);
            }

            void copy(std::string textureKey, int gx, int gy, int gFrame, float originX, float originY) {
                copy(textureKey, gx, gy, gFrame, originX, originY, 1, 1);
            }

            void copy(std::string textureKey, int gx, int gy, int gFrame) {
                copy(textureKey, gx, gy, gFrame, 0, 0);
            }

            void copy(std::string textureKey, int gx, int gy) {
                copy(textureKey, gx, gy, 0);
            }

            void copy(std::string textureKey) {
                copy(textureKey, 0, 0);
            }

            void copy(Amara::Image* img, int gx, int gy, int gFrame, float gOriginX, float gOriginY, float gScaleX, float gScaleY) {
                drawImage.setTexture(img->texture->key);
                drawImage.x = gx;
                drawImage.y = gy;
                drawImage.frame = gFrame;
                drawImage.originX = gOriginX;
                drawImage.originY = gOriginY;
                drawImage.scaleX = gScaleX;
                drawImage.scaleY = gScaleY;
                drawImage.draw(0, 0, imageWidth, imageHeight);
            }

            void copy(Amara::Image* img, int gx, int gy, int gFrame, float gOriginX, float gOriginY) {
                copy(img, gx, gy, gFrame, gOriginX, gOriginY, img->scaleX, img->scaleY);
            }

            void copy(Amara::Image* img, int gx, int gy, int gFrame) {
                copy(img, gx, gy, gFrame, img->originX, img->originY);
            }

            void copy(Amara::Image* img, int gx, int gy) {
                copy(img, gx, gy, img->frame);
            }

            void copy(Amara::Image* img) {
                copy(img, img->x, img->y);
            }

            void run() {
                if (properties->renderDeviceReset) {
                    createNewCanvasTexture();
                }
                Amara::Actor::run();
            }

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
            }
            void setOrigin(float go) {
                setOrigin(go, go);
            }
            void setOriginPosition(float gx, float gy) {
                originX = gx/imageWidth;
                originY = gy/imageHeight;
            }
            void setOriginPosition(float g) {
                setOriginPosition(g, g);
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                bool skipDrawing = false;

				if (properties->renderTargetsReset || properties->renderDeviceReset) {
					createNewCanvasTexture();
				}

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY; 
                destRect.x = ((x - properties->scrollX*scrollFactorX + properties->offsetX - (originX * imageWidth * scaleX)) * nzoomX);
                destRect.y = ((y-z - properties->scrollY*scrollFactorY + properties->offsetY - (originY * imageHeight * scaleY)) * nzoomY);
                destRect.w = ((imageWidth * scaleX) * properties->zoomX);
                destRect.h = ((imageHeight * scaleY) * properties->zoomY);

                if (pixelLocked) {
                    destRect.x = floor(destRect.x);
                    destRect.y = floor(destRect.y);
                    destRect.w = ceil(destRect.w);
                    destRect.h = ceil(destRect.h);
                }

                origin.x = destRect.w * originX;
                origin.y = destRect.h * originY;

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
                    hh -= -(destRect.y);
                    hy = vy;
                }
                if (hx + hw > vx + vw) hw = ((vx + vw) - hx);
                if (hy + hh > vy + vh) hh = ((vy + vh) - hy);
				
                if (destRect.x + destRect.w <= 0) skipDrawing = true;
                if (destRect.y + destRect.h <= 0) skipDrawing = true;
                if (destRect.x >= vw) skipDrawing = true;
                if (destRect.y >= vh) skipDrawing = true;
                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;

				checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);

                if (!skipDrawing) {
                    if (canvas != nullptr) {
                        SDL_SetTextureBlendMode(canvas, blendMode);
				        SDL_SetTextureAlphaMod(canvas, alpha * properties->alpha * 255);

                        SDL_RenderCopyExF(
                            properties->gRenderer,
                            canvas,
                            NULL,
                            &destRect,
                            angle + properties->angle,
                            &origin,
                            SDL_FLIP_NONE
                        );
                    }
                }

                Amara::Actor::draw(vx, vy, vw, vh);

				checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);

                if (clearEveryFrame) clear();
            }

            ~Canvas() {
                SDL_DestroyTexture(canvas);
            }
    };
}