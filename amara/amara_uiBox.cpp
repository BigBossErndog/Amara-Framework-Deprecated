#pragma once
#ifndef AMARA_UIBOX
#define AMARA_UIBOX

#include "amara.h"

namespace Amara {
    class UIBox: public Amara::Actor {
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

            int minWidth = 0;
            int minHeight = 0;

            int openWidth = 0;
            int openHeight = 0;
            bool lockOpen = false;

            int openSpeedX = 0;
            int openSpeedY = 0;
            int closeSpeedX = 0;
            int closeSpeedY = 0;

            int boxTextureWidth = 0;
            int boxTextureHeight = 0;
            int imageWidth = 0;
            int imageHeight = 0;

            int partitionTop = 0;
            int partitionBottom = 0;
            int partitionLeft = 0;
            int partitionRight = 0;

            int frame = 0;

            float originX = 0;
            float originY = 0;

            Amara::Alignment boxHorizontalAlignment = ALIGN_CENTER;
            Amara::Alignment boxVerticalAlignment = ALIGN_CENTER;

            Amara::StateManager* copySm = nullptr;
            Amara::StateManager mySm;

            bool keepOpen = false;

            UIBox() {}

            UIBox(Amara::StateManager* gsm) {
                copyStateManager(gsm);
                setVisible(false);
            }

            UIBox(float gx, float gy, int gw, int gh, std::string gTextureKey): UIBox() {
                x = gx;
                y = gy;
                width = gw;
                height = gh;
                openWidth = width;
                openHeight = height;
                textureKey = gTextureKey;
            }

            UIBox(int gw, int gh, std::string gTextureKey): UIBox(0, 0, gw, gh, gTextureKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }

                setSize(width, height);

                Amara::Actor::init(gameProperties, givenScene, givenParent);

                data["entityType"] = "uiBox";
            }

            virtual void configure(nlohmann::json config) {
                Amara::Actor::configure(config);

                if (config.find("width") != config.end()) {
                    width = config["width"];
                    openWidth = width;
                }
                if (config.find("height") != config.end()) {
                    height = config["height"];
                    openHeight = height;
                }
                if (config.find("xFromRight") != config.end()) {
                    int xFromRight = config["xFromRight"];
                    x = scene->mainCamera->width - width - xFromRight;
                }
                if (config.find("yFromBottom") != config.end()) {
                    int yFromBottom = config["yFromBottom"];
                    y = scene->mainCamera->height - height - yFromBottom;
                }
				if (config.find("relativeXFromRight") != config.end()) {
					float relativeX = config["relativeXFromRight"];
					x = scene->mainCamera->width - scene->mainCamera->width*relativeX - width;
				}
				if (config.find("relativeYFromBottom") != config.end()) {
					float relativeY = config["relativeYFromBottom"];
					y = scene->mainCamera->height - scene->mainCamera->height*relativeY - height;
				}
				if (config.find("relativeXFromCenter") != config.end()) {
					float relativeX = config["relativeXFromCenter"];
					x = scene->mainCamera->width/2.0 + scene->mainCamera->width*relativeX/2.0 - width/2.0;
				}
				if (config.find("relativeYFromCenter") != config.end()) {
					float relativeY = config["relativeYFromCenter"];
					y = scene->mainCamera->height/2.0 + scene->mainCamera->height*relativeY/2.0 - height/2.0;
				}
                if (config.find("minWidth") != config.end()) {
                    minWidth = config["minWidth"];
                }
                if (config.find("minHeight") != config.end()) {
                    minHeight = config["minHeight"];
                }
                if (config.find("texture") != config.end()) {
                    setTexture(config["texture"]);
                }
                if (config.find("openSpeedX") != config.end()) {
                    openSpeedX = config["openSpeedX"];
                }
                if (config.find("openSpeedY") != config.end()) {
                    openSpeedY = config["openSpeedY"];
                }
                if (config.find("closeSpeedX") != config.end()) {
                    closeSpeedX = config["closeSpeedX"];
                }
                if (config.find("closeSpeedY") != config.end()) {
                    closeSpeedY = config["closeSpeedY"];
                }
                if (config.find("openCloseSpeedX") != config.end()) {
                    openSpeedX = config["openCloseSpeedX"];
                    closeSpeedX = config["openCloseSpeedX"];
                }
                if (config.find("openCloseSpeedY") != config.end()) {
                    openSpeedY = config["openCloseSpeedY"];
                    closeSpeedY = config["openCloseSpeedY"];
                }
                if (config.find("openCloseSpeed") != config.end()) {
                    setOpenCloseSpeed(config["openCloseSpeed"], config["openCloseSpeed"]);
                }
				if (config.find("fixedWithinBounds") != config.end() && config["fixedWithinBounds"]) {
					if (x < 0) x = 0;
					if (y < 0) y = 0;
					if (x + width > scene->mainCamera->width) {
						x = scene->mainCamera->width - width;
					}
					if (y + height > scene->mainCamera->height) {
						y = scene->mainCamera->height - height;
					}
				}
                if (config.find("partitionTop") != config.end()) {
                    partitionTop = config["partitionTop"];
                }
                if (config.find("partitionBottom") != config.end()) {
                    partitionBottom = config["partitionBottom"];
                }
                if (config.find("partitionLeft") != config.end()) {
                    partitionLeft = config["partitionLeft"];
                }
                if (config.find("partitionRight") != config.end()) {
                    partitionRight = config["partitionRight"];
                }

                setOpenSpeed(openSpeedX, openSpeedY);
                setCloseSpeed(closeSpeedX, closeSpeedY);
            }

            virtual void drawBoxPart(int part) {
                bool skipDrawing = false;
                int partX = 0, partY = 0, partWidth = 0, partHeight = 0;

                float horizontalAlignmentFactor = 0.5;
                float verticalAlignmentFactor = 0.5;

                switch (boxHorizontalAlignment) {
                    case ALIGN_LEFT:
                        horizontalAlignmentFactor = 0;
                        break;
                    case ALIGN_RIGHT:
                        horizontalAlignmentFactor = 1;
                        break;
                }
                switch(boxVerticalAlignment) {
                    case ALIGN_TOP:
                        verticalAlignmentFactor = 0;
                        break;
                    case ALIGN_BOTTOM:
                        verticalAlignmentFactor = 1;
                        break;
                }

                switch (part % 3) {
                    case 0:
                        partX = 0;
                        partWidth = partitionLeft;
                        destRect.x = (width - openWidth)*horizontalAlignmentFactor;
                        destRect.w = partWidth;
                        break;
                    case 1:
                        partX = partitionLeft;
                        partWidth = boxTextureWidth - partitionLeft - partitionRight;
                        destRect.x = (width - openWidth)*horizontalAlignmentFactor + partitionLeft;
                        destRect.w = openWidth - partitionLeft - partitionRight;
                        break;
                    case 2:
                        partX = boxTextureWidth - partitionRight;
                        partWidth = partitionRight;
                        destRect.x = (width - openWidth)*horizontalAlignmentFactor + openWidth - partitionRight;
                        destRect.w = partWidth;
                        break;
                }
                switch ((int)floor(part/(float)3)) {
                    case 0:
                        partY = 0;
                        partHeight = partitionTop;
                        destRect.y = (height - openHeight)*verticalAlignmentFactor;
                        destRect.h = partHeight;
                        break;
                    case 1:
                        partY = partitionTop;
                        partHeight = boxTextureHeight - partitionTop - partitionBottom;
                        destRect.y = (height - openHeight)*verticalAlignmentFactor + partitionTop;
                        destRect.h = openHeight - partitionTop - partitionBottom;
                        break;
                    case 2:
                        partY = boxTextureHeight - partitionBottom;
                        partHeight = partitionBottom;
                        destRect.y = (height - openHeight)*verticalAlignmentFactor + openHeight - partitionBottom;
                        destRect.h = partHeight;
                        break;
                }

                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;

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
                if (!isVisible) return;
                if (width < minWidth) width = minWidth;
                if (height < minHeight) height = minHeight;

                if (recWidth != width || recHeight != height) {
                    recWidth = width;
                    recHeight = height;
                    if (openWidth > width) openWidth = width;
                    if (openHeight > height) openHeight = height;
                    createNewCanvasTexture();
                }

                if (lockOpen) {
                    openWidth = width;
                    openHeight = height;
                }

                SDL_SetRenderTarget(properties->gRenderer, canvas);
                SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
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

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                destRect.x = floor((x*scaleX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
                destRect.y = floor((y*scaleY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
                destRect.w = ceil((width * scaleX) * nzoomX);
                destRect.h = ceil((height * scaleY) * nzoomY);

                origin.x = destRect.w * originX;
                origin.y = destRect.h * originY;

                int hx, hy, hw, hh = 0;
                hw = destRect.w;
                hh = destRect.h;

                if (destRect.x + destRect.w <= 0) skipDrawing = true;
                if (destRect.y + destRect.h <= 0) skipDrawing = true;
                if (destRect.x >= vw) skipDrawing = true;
                if (destRect.y >= vh) skipDrawing = true;
                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;

                if (!skipDrawing) {
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

                    if (canvas != nullptr) {
                        SDL_SetTextureBlendMode(canvas, blendMode);
				        SDL_SetTextureAlphaMod(canvas, alpha * properties->alpha * 255);

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

                if (openWidth == width && openHeight == height) {
                    Amara::Entity::draw(vx, vy, vw, vh);
                }
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

                    partitionLeft = imageWidth/3.0;
                    partitionRight = imageWidth/3.0;
                    partitionTop = imageHeight/3.0;
                    partitionBottom = imageHeight/3.0;

                    return true;
                }
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
                }
                return false;
            }

            void setSize(int nw, int nh) {
                width = nw;
                height = nh;
            }

            void setOpenSize(int nw, int nh) {
                openWidth = nw;
                openHeight = nh;
                if (openWidth > width) openWidth = width;
                if (openHeight > height) openHeight = height;
                if (openWidth < minWidth) openWidth = minWidth;
                if (openHeight < minHeight) openHeight = minHeight;
            }
            void changeOpenSize(int gx, int gy) {
                setOpenSize(openWidth + gx, openHeight + gy);
            }

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
            }
            void setOrigin(float gi) {
                setOrigin(gi, gi);
            }

            void copyStateManager(Amara::StateManager* gsm) {
                copySm = gsm;
            }
            void copyStateManager(Amara::StateManager& gsm) {
                copySm = &gsm;
            }

            Amara::StateManager& checkSm() {
                if (copySm != nullptr) {
                    return *copySm;
                }
                else {
                    return mySm;
                }
            }

            virtual bool show() {
                Amara::StateManager& sm = checkSm();
                if (sm.once()) {
                    setVisible(true);
                    return true;
                }
                return false;
            }

            virtual bool hide() {
                Amara::StateManager& sm = checkSm();
                if (sm.once()) {
                    setVisible(false);
                    return true;
                }
                return false;
            }

            virtual bool open() {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    if (!keepOpen) {
                        resetOpenSize();
                    }
                }

                if (show()) {
                    toReturn = true;
                }

                if (sm.evt()) {
                    bool complete = true;

                    if (openSpeedX > 0) {
                        openWidth += openSpeedX;
                        if (openWidth >= width) {
                            openWidth = width;
                        }
                        else {
                            complete = false;
                        }
                    }

                    if (openSpeedY > 0) {
                        openHeight += openSpeedY;
                        if (openHeight >= height) {
                            openHeight = height;
                        }
                        else {
                            complete = false;
                        }
                    }

                    if (complete) {
                        keepOpen = true;
                        sm.nextEvt();
                    }

                    toReturn = true;
                }
            }

            virtual bool close() {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.evt()) {
                    bool complete = true;
                    if (closeSpeedX > 0) {
                        openWidth -= closeSpeedX;
                        if (openWidth <= minWidth) {
                            openWidth = minWidth;
                        }
                        else {
                            complete = false;
                        }
                    }

                    if (closeSpeedY > 0) {
                        openHeight -= closeSpeedY;
                        if (openHeight <= minHeight) {
                            openHeight = minHeight;
                        }
                        else {
                            complete = false;
                        }
                    }

                    if (complete) {
                        keepOpen = false;
                        sm.nextEvt();
                    }

                    toReturn = true;
                }

                if (hide()) {
                    toReturn = true;
                }

                return toReturn;
            }

            void setOpenSpeed(int gx, int gy) {
                openSpeedX = gx;
                openSpeedY = gy;

                if (openSpeedX < 0) openSpeedX = 0;
                if (openSpeedY < 0) openSpeedY = 0;

                resetOpenSize();

                lockOpen = false;
            }
            void setOpenSpeed(int gy) {
                setOpenSpeed(0, gy);
            }
            void setOpenSpeed() {
                setOpenSpeed(0);
            }

            void setCloseSpeed(int gx, int gy) {
                closeSpeedX = gx;
                closeSpeedY = gy;

                if (closeSpeedX < 0) closeSpeedX = 0;
                if (closeSpeedY < 0) closeSpeedY = 0;

                lockOpen = false;
            }
            void setCloseSpeed(int gy) {
                setCloseSpeed(0, gy);
            }
            void setCloseSpeed() {
                setCloseSpeed(0);
            }

            void setOpenCloseSpeed(int gx, int gy) {
                setOpenSpeed(gx, gy);
                setCloseSpeed(gx, gy);
            }
            void setOpenCloseSpeed(int gy) {
                setOpenCloseSpeed(0, gy);
            }
            void setOpenCloseSpeed() {
                setOpenCloseSpeed(0);
            }

            void resetOpenSize() {
                if (openSpeedX > 0) setOpenSize(0, openHeight);
                if (openSpeedY > 0) setOpenSize(openWidth, 0);
            }

            ~UIBox() {
                SDL_DestroyTexture(canvas);
            }
    };
}

#endif
