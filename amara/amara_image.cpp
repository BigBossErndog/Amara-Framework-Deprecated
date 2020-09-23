#pragma once
#ifndef AMARA_IMAGE
#define AMARA_IMAGE

#include "amara.h"

namespace Amara {
    class Image: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            Amara::ImageTexture* texture = nullptr;
            std::string textureKey;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_Rect destRect;
            SDL_Point origin;

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

            float originX = 0;
            float originY = 0;

            float renderOffsetX = 0;
            float renderOffsetY = 0;

            bool flipHorizontal = false;
            bool flipVertical = false;

            Image(): Actor() {
                textureKey.clear();
            }

            Image(float gx, float gy) {
                x = gx;
                y = gy;
            }

            Image(std::string givenKey): Amara::Actor() {
                textureKey = givenKey;
            }

            Image(float gx, float gy, std::string givenKey): Image(givenKey) {
                x = gx;
                y = gy;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }

                Amara::Actor::init(gameProperties, givenScene, givenParent);

                data["entityType"] = "image";
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
                if (config.find("flipHorizontal") != config.end()) {
                    flipHorizontal = config["flipHorizontal"];
                }
                if (config.find("flipVertical") != config.end()) {
                    flipVertical = config["flipVertical"];
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
                return config;
            }

            virtual void drawTexture(int vx, int vy, int vw, int vh) {
                bool skipDrawing = false;

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

                destRect.x = floor((x+renderOffsetX+cropLeft - properties->scrollX*scrollFactorX + properties->offsetX - (originX * imageWidth * scaleX)) * nzoomX);
                destRect.y = floor((y-z+renderOffsetY+cropTop - properties->scrollY*scrollFactorY + properties->offsetY - (originY * imageHeight * scaleY)) * nzoomY);
                destRect.w = ceil(((imageWidth-cropLeft-cropRight) * scaleX) * nzoomX);
                destRect.h = ceil(((imageHeight-cropTop-cropBottom) * scaleY) * nzoomY);

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
                                Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                                int maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                                frame = frame % maxFrame;

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

                        SDL_RenderCopyEx(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect,
                            angle + properties->angle,
                            &origin,
                            flipVal
                        );
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                drawTexture(vx, vy, vw, vh);

                Amara::Entity::draw(vx, vy, vw, vh);
            }

            bool setTexture(std::string gTextureKey) {
                texture = (Amara::ImageTexture*)(load->get(gTextureKey));
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
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
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

            void setRenderOffset(float gx, float gy) {
                renderOffsetX = gx;
                renderOffsetY = gy;
            }

            void setRenderOffset(float gi) {
                setRenderOffset(gi, gi);
            }

            void setFrame(int fr) {
                frame = fr;
            }

            void scaleTo(float gw, float gh) {
                scaleX = gw/imageWidth;
                scaleY = gh/imageHeight;
            }
    };
}

#endif
