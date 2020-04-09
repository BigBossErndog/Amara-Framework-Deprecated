#ifndef AMARA_CANVAS
#define AMARA_CANVAS

#include "amara.h"

namespace Amara {
    class Canvas: public Actor {
        public:
            SDL_Texture* canvas = nullptr;
            
            float width = 0;
            float height = 0;
            int imageWidth = 0;
            int imageHeight = 0;
            bool getLogicalDimensions = false;

            Amara::Image drawImage;

            bool clearEveryFrame = true;

            SDL_Color recColor;
            SDL_Rect drawnRect;
            SDL_Rect destRect;
            SDL_Rect viewport;
            SDL_Point origin;
            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

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
                Amara::Actor::init(gameProperties, gScene, gParent);
                if (getLogicalDimensions) {
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

                if (canvas == nullptr) {
                    createNewCanvasTexture();
                }

                drawImage.init(properties, scene, this);
            }

            void beginFill(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode gBlendMode) {
                SDL_SetRenderTarget(properties->gRenderer, canvas);
                SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);
                SDL_SetRenderDrawColor(properties->gRenderer, r, g, b, a);
                SDL_SetRenderDrawBlendMode(properties->gRenderer, gBlendMode);

                properties->zoomX = 1;
                properties->zoomY = 1;
                properties->offsetX = 0;
                properties->offsetY = 0;
                properties->scrollX = 0;
                properties->scrollY = 0;
                
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
                SDL_SetRenderTarget(properties->gRenderer, NULL);
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
                    SDL_GetWindowPixelFormat(properties->gWindow),
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
                drawImage.setTexture(img->textureKey);
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

            virtual void draw(int vx, int vy, int vw, int vh) override {
                bool skipDrawing = false;

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                destRect.x = floor(floor(x*scaleX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * imageWidth * scaleX)) * properties->zoomX);
                destRect.y = floor(floor(y*scaleY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * imageHeight * scaleY)) * properties->zoomY);
                destRect.w = ceil(ceil(imageWidth * scaleX) * properties->zoomX);
                destRect.h = ceil(ceil(imageHeight * scaleY) * properties->zoomY);

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

                checkForHover(hx, hy, hw, hh);

                if (destRect.x + destRect.w <= 0) skipDrawing = true;
                if (destRect.y + destRect.h <= 0) skipDrawing = true;
                if (destRect.x >= vw) skipDrawing = true;
                if (destRect.y >= vh) skipDrawing = true;
                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;

                if (!skipDrawing) {
                    if (canvas != nullptr) {
                        SDL_SetTextureBlendMode(canvas, blendMode);
				        SDL_SetTextureAlphaMod(canvas, alpha * 255);

                        SDL_RenderCopyEx(
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

                Amara::Entity::draw(vx, vy, vw, vh);

                if (clearEveryFrame) clear();
            }
    };
}

#endif