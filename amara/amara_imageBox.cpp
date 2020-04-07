#ifndef AMARA_BOXIMAGE
#define AMARA_BOXIMAGE

#include "amara.h"

namespace Amara {
    class ImageBox: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;
            SDL_Texture* canvas = nullptr;

            Amara::ImageTexture* texture = nullptr;
            std::string textureKey;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_Rect destRect;
            SDL_Point origin;

            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

            float recWidth = -1;
            float recHeight = -1;
            int width = 0;
            int height = 0;
            
            int boxTextureWidth = 0;
            int boxTextureHeight = 0;
            int imageWidth = 0;
            int imageHeight = 0;

            int frame = 0;

            float originX = 0;
            float originY = 0;

            ImageBox(float gx, float gy, int gw, int gh, std::string gTextureKey) {
                x = gx;
                y = gy;
                width = gw;
                height = gh;
                textureKey = gTextureKey;
            }

            ImageBox(int gw, int gh, std::string gTextureKey): ImageBox(0, 0, gw, gh, gTextureKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }

                setSize(width, height);

                Amara::Actor::init(gameProperties, givenScene, givenParent);
            }

            virtual void drawBoxPart(int part) {
                bool skipDrawing = false;
                int partX = 0, partY = 0, partWidth = 0, partHeight = 0;
                
                partX = floor((part % 3) * boxTextureWidth/(float)3);
                partY = floor(floor(part/(float)3) * boxTextureHeight/(float)3);
                partWidth = ceil((float)boxTextureWidth/(float)3);
                partHeight = ceil((float)boxTextureHeight/(float)3);

                switch (part % 3) {
                    case 0:
                        destRect.x = 0;
                        destRect.w = partWidth;
                        break;
                    case 1:
                        destRect.x = partWidth;
                        destRect.w = width - partWidth*2;
                        break;
                    case 2:
                        destRect.x = width - partWidth;
                        destRect.w = partWidth;
                        break;
                }
                switch ((int)floor(part/(float)3)) {
                    case 0:
                        destRect.y = 0;
                        destRect.h = partHeight;
                        break;
                    case 1:
                        destRect.y = partHeight;
                        destRect.h = height - partHeight*2;
                        break;
                    case 2:
                        destRect.y = height - partHeight;
                        destRect.h = partHeight;
                        break;
                }

                if (!skipDrawing) {
                    if (texture != nullptr) {
                        SDL_Texture* tx = (SDL_Texture*)texture->asset;
                        switch (texture->type) {
                            case IMAGE:
                                frame = 0;
                                srcRect.x = partX;
                                srcRect.y = partY;
                                srcRect.w = partWidth;
                                srcRect.h = partHeight;
                                break;
                            case SPRITESHEET:
                                Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                                int maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                                frame = frame % maxFrame;

                                srcRect.x = (frame % (texture->width / spr->frameWidth)) * spr->frameWidth + partX;
                                srcRect.y = floor(frame / (texture->width / spr->frameWidth)) * spr->frameHeight + partY;
                                srcRect.w = partWidth;
                                srcRect.h = partHeight;
                                break;
                        }

                        SDL_RenderCopy(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect
                        );
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                if (recWidth != width || recHeight != height) {
                    recWidth = width;
                    recHeight = height;
                    createNewCanvasTexture();
                }

                SDL_SetRenderTarget(properties->gRenderer, canvas);
                SDL_RenderClear(gRenderer);
                for (int i = 0; i < 9; i++) {
                    drawBoxPart(i);
                }
                SDL_SetRenderTarget(properties->gRenderer, NULL);

                bool skipDrawing = false;

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                destRect.x = floor(floor(x*scaleX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * properties->zoomX);
                destRect.y = floor(floor(y*scaleY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * properties->zoomY);
                destRect.w = ceil(ceil(width * scaleX) * properties->zoomX);
                destRect.h = ceil(ceil(height * scaleY) * properties->zoomY);

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
                // SDL_Log("DRAWING: %d, %d, %d, %d - %d, %d", destRect.x, destRect.y, destRect.w, destRect.h, imageWidth, imageHeight);

                Amara::Entity::draw(vx, vy, vw, vh);
            }

            void setSize(int nw, int nh) {
                width = nw;
                height = nh;
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
            }

            bool setTexture(std::string gTextureKey) {
                texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                   textureKey = texture->key;

                    if (texture->type == SPRITESHEET) {
                        boxTextureWidth = ((Amara::Spritesheet*)texture)->frameWidth;
                        boxTextureHeight = ((Amara::Spritesheet*)texture)->frameHeight;
                    }
                    else {
                        boxTextureWidth = texture->width;
                        boxTextureHeight = texture->height;
                    }

                    imageWidth = boxTextureWidth;
                    imageHeight = boxTextureHeight;

                    return true;
                }
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
                }
                return false;
            }
    };
}

#endif