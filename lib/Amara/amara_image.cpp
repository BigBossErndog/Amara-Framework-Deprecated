#ifndef AMARA_IMAGE
#define AMARA_IMAGE

#include "amara.h"

namespace Amara {
    class Image: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            Amara::ImageTexture* texture = nullptr;
            string textureKey;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_Rect destRect;
            SDL_Point origin;

            int width = 0;
            int height = 0;
            int imageWidth = 0;
            int imageHeight = 0;

            int frame = 0;

            float originX = 0;
            float originY = 0;

            float scaleX = 1;
            float scaleY = 1;

            float angle = 0;

            Image(): Actor() {
                textureKey.clear();
            }

            Image(string givenKey): Amara::Actor() {
                textureKey = givenKey;
            }

            Image(float gx, float gy, string givenKey): Image(givenKey) {
                x = gx;
                y = gy;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                Amara::Interactable::init(gameProperties);
                
				properties = gameProperties;
				scene = givenScene;
                parent = givenParent;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                    textureKey.clear();
                }

				create();
			}

            virtual void draw(int vx, int vy, int vw, int vh) override {
                bool skipDrawing = false;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                destRect.x = floor(floor(x - properties->scrollX - (originX * imageWidth)) * properties->zoomX);
                destRect.y = floor(floor(y - properties->scrollY - (originY * imageHeight)) * properties->zoomY);
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
                    hh -= -(destRect.h);
                    hy = vy;
                }
                if (hx + hw > vx + vw) hw = (vx - hx);
                if (hy + hh > vy + vh) hh = (vy - hy);


                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;

                if (!skipDrawing) {
                    if (texture != nullptr) {
                        SDL_Texture* tx = (SDL_Texture*)texture->asset;
                        switch (texture->type) {
                            case IMAGE:
                                frame = 0;
                                srcRect.x = 0;
                                srcRect.y = 0;
                                srcRect.w = imageWidth;
                                srcRect.h = imageHeight;
                                break;
                            case SPRITESHEET:
                                Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                                int maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                                frame = frame % maxFrame;

                                srcRect.x = (frame % (texture->width / spr->frameWidth)) * spr->frameWidth;
                                srcRect.y = floor(frame / (texture->width / spr->frameWidth)) * spr->frameHeight;
                                srcRect.w = spr->frameWidth;
                                srcRect.h = spr->frameHeight;
                                break;
                        }

                        SDL_RenderCopyEx(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect,
                            angle,
                            &origin,
                            SDL_FLIP_NONE
                        );
                    }
                }

                Amara::Entity::draw(vx, vy, vw, vh);
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

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
            }
            void setOrigin(float g) {
                setOrigin(g, g);
            }

            void setScale(float gx, float gy) {
                scaleX = gx;
                scaleY = gy;
            }
            void setScale(float g) {
                setScale(g, g);
            }

            void changeScale(float gx, float gy) {
                scaleX += gx;
                scaleY += gy;
            }
            void changeScale(float gi) {
                changeScale(gi, gi);
            }

            void setFrame(int fr) {
                frame = fr;
            }
    };
}

#endif
