#pragma once
#ifndef AMARA_TRUETYPEFONT
#define AMARA_TRUETYPEFONT

#include "amara.h"

namespace Amara {
    enum Alignment {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT,
        ALIGN_TOP,
        ALIGN_BOTTOM
    };

    class TrueTypeFont: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;
            Amara::TTFAsset* fontAsset = nullptr;
            std::string fontKey;

            std::string text;
            SDL_Rect viewport;
            Amara::Color color = FC_MakeColor(0, 0, 0, 255);
            Amara::Alignment alignment = ALIGN_LEFT;
            FC_Effect effect;

            float originX = 0;
            float originY = 0;

            float scaleX = 1;
            float scaleY = 1;

            int width = 0;
            int height = 0;

            bool wordWrap = false;
            Uint16 wordWrapWidth = 0;

            TrueTypeFont(): Amara::Actor() {}

            TrueTypeFont(float gx, float gy): TrueTypeFont() {
                x = gx;
                y = gy;
            }

            TrueTypeFont(float gx, float gy, std::string gFontKey): TrueTypeFont(gx, gy) {
                fontKey = gFontKey;
            }

            TrueTypeFont(float gx, float gy, std::string gFontKey, std::string gText): TrueTypeFont(gx, gy, gFontKey) {
                text = gText;
            }

            TrueTypeFont(float gx, float gy, std::string gFontKey, std::string gText, Amara::Color gColor): TrueTypeFont(gx, gy, gFontKey, gText) {
                color = gColor;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!fontKey.empty()) {
                    setFont(fontKey);
                }

                Amara::Actor::init(gameProperties, givenScene, givenParent);
			}

            void setFont(std::string gFontKey) {
                fontAsset = (Amara::TTFAsset*)(load->get(gFontKey));
                if (fontAsset != nullptr) {
                    fontKey = gFontKey;
                    findDimensions();
                }
            }

            void setText(std::string newTxt) {
                text = newTxt;
                const char* txt = text.c_str();
                findDimensions();
            }

            void setColor(int r, int g, int b) {
                color.r = r;
                color.g = g;
                color.b = b;
            }
            void setColor(Amara::Color gColor) {
                color = gColor;
            }

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
                findDimensions();
            }
            void setOrigin(float g) {
                setOrigin(g, g);
            }

            void setScale(float gx, float gy) {
                scaleX = gx;
                scaleY = gy;
                findDimensions();
            }
            void setScale(float g) {
                setScale(g, g);
            }

            void changeScale(float gx, float gy) {
                scaleX += gx;
                scaleY += gy;
                findDimensions();
            }
            void changeScale(float gi) {
                changeScale(gi, gi);
            }

            void setWordWrap() {
                wordWrap = true;
                findDimensions();
            }
            void setWordWrap(int w) {
                wordWrapWidth = w;
                setWordWrap();
            } 
            void setWordWrap(bool g) {
                wordWrap = g;
                findDimensions();
            }

            void findDimensions() {
                const char* txt = text.c_str();
                if (wordWrap) {
                    width = wordWrapWidth * scaleX;
                    height = FC_GetColumnHeight(fontAsset->font, wordWrapWidth, txt) * scaleY;
                }
                else {
                    width = FC_GetWidth(fontAsset->font, txt) * scaleX;
                    height = FC_GetHeight(fontAsset->font, txt) * scaleY;
                }
            }

            void run() {
                if (fontAsset != nullptr) {
                    if (properties->renderTargetsReset) {
                        fontAsset->reloadFontCache(gRenderer);
                    }
                }
                Amara::Actor::run();
            }

            void draw(int vx, int vy, int vw, int vh) {
                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                effect.alignment = (FC_AlignEnum)alignment;
                effect.scale.x = scaleX * nzoomX;
                effect.scale.y = scaleY * nzoomY;
                color.a = alpha * 255;
                effect.color = color;

                const char* txt = text.c_str();

                if (fontAsset != nullptr) {
                    if (wordWrap) {
                        int offsetX = 0;
                        if (alignment == ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == ALIGN_RIGHT) {
                            offsetX = width;
                        }
                        
                        FC_DrawColumnEffect(
                            fontAsset->font,
                            gRenderer,
                            floor((x - properties->scrollX + properties->offsetX - (width * originX) + offsetX) * nzoomX),
                            floor((y-z - properties->scrollY + properties->offsetY - (height * originY)) * nzoomY),
                            wordWrapWidth,
                            effect,
                            txt
                        );
                    }
                    else {
                        int offsetX = 0;
                        if (alignment == ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == ALIGN_RIGHT) {
                            offsetX = width;
                        }
                        FC_DrawEffect(
                            fontAsset->font,
                            gRenderer,
                            floor((x - properties->scrollX + properties->offsetX - (width * originX) + offsetX) * nzoomX),
                            floor((y-z - properties->scrollY + properties->offsetY - (height * originY)) * nzoomY),
                            effect,
                            txt
                        );
                    }
                }

                Amara::Entity::draw(vx, vy, vw, vh);
            }
    };
}

#endif