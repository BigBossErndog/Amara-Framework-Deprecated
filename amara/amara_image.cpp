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

            int frame = 0;

            float originX = 0;
            float originY = 0;

            Image(): Actor() {
                textureKey.clear();
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
            }

            virtual nlohmann::json toData() {
                nlohmann::json config = Amara::Actor::toData();
                if (texture != nullptr) config["texture"] = texture->key;
                config["frame"] = frame;
                config["originX"] = originX;
                config["originY"] = originY;
                return config;
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                bool skipDrawing = false;

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

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

                        SDL_SetTextureBlendMode(tx, blendMode);
				        SDL_SetTextureAlphaMod(tx, alpha * 255);

                        SDL_RenderCopyEx(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect,
                            angle + properties->angle,
                            &origin,
                            SDL_FLIP_NONE
                        );
                    }
                }

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

            void setFrame(int fr) {
                frame = fr;
            }
    };
}

#endif
