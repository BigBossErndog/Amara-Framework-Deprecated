#ifndef AMARA_TRUETYPEFONT
#define AMARA_TRUETYPEFONT

#include "amara.h"

namespace Amara {
    class TrueTypeFont: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;
            Amara::TTFAsset* fontAsset = nullptr;
            string fontKey;

            string text;
            SDL_Rect viewport;
            SDL_Color color = FC_MakeColor(0, 0, 0, 255);
            FC_AlignEnum alignment = FC_ALIGN_LEFT;
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

            TrueTypeFont(float gx, float gy, string gFontKey): TrueTypeFont(gx, gy) {
                fontKey = gFontKey;
            }

            TrueTypeFont(float gx, float gy, string gFontKey, string gText): TrueTypeFont(gx, gy, gFontKey) {
                text = gText;
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                Amara::Interactable::init(gameProperties);
                
				properties = gameProperties;
				scene = givenScene;
                parent = givenParent;
                load = properties->loader;
                gRenderer = properties->gRenderer;
                input = properties->input;

                if (!fontKey.empty()) {
                    setFont(fontKey);
                }

				create();
			}

            void setFont(string gFontKey) {
                fontAsset = (Amara::TTFAsset*)(load->get(gFontKey));
                if (fontAsset != nullptr) {
                    fontKey = gFontKey;
                }
            }

            void setText(string newTxt) {
                text = newTxt;
                const char* txt = text.c_str();
                width = FC_GetWidth(fontAsset->font, txt);
                height = FC_GetHeight(fontAsset->font, txt);
            }

            void setColor(int r, int g, int b) {
                color.r = r;
                color.g = g;
                color.b = b;
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

            void setWordWrap() {
                wordWrap = true;
            }
            void setWordWrap(int w) {
                setWordWrap();
                wordWrapWidth = w;
            }  
            void removeWordWrap() {
                wordWrap = false;
            }

            void draw(int vx, int vy, int vw, int vh) {
                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

                effect.alignment = alignment;
                effect.scale.x = scaleX * properties->zoomX;
                effect.scale.y = scaleY * properties->zoomY;
                color.a = alpha * 255;
                effect.color = color;

                const char* txt = text.c_str();

                if (fontAsset != nullptr) {
                    if (fontAsset->recFullscreen != properties->isFullscreen) {
                        fontAsset->recFullscreen = properties->isFullscreen;
                        fontAsset->reloadFontCache(gRenderer);
                    }

                    if (wordWrap) {
                        width = wordWrapWidth * scaleX;
                        height = FC_GetColumnHeight(fontAsset->font, wordWrapWidth, txt) * scaleY;

                        int offsetX = 0;
                        if (alignment == FC_ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == FC_ALIGN_RIGHT) {
                            offsetX = -width;
                        }
                        
                        FC_DrawColumnEffect(
                            fontAsset->font,
                            gRenderer,
                            floor(floor(x - properties->scrollX - (width * originX) + offsetX) * properties->zoomX),
                            floor(floor(y - properties->scrollY - (height * originY)) * properties->zoomY),
                            wordWrapWidth,
                            effect,
                            txt
                        );
                    }
                    else {
                        width = FC_GetWidth(fontAsset->font, txt);
                        height = FC_GetHeight(fontAsset->font, txt);
                        int offsetX = 0;
                        if (alignment == FC_ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == FC_ALIGN_RIGHT) {
                            offsetX = width;
                        }
                        FC_DrawEffect(
                            fontAsset->font,
                            gRenderer,
                            floor(floor(x - properties->scrollX - (width * originX) + offsetX) * properties->zoomX),
                            floor(floor(y - properties->scrollY - (height * originY)) * properties->zoomY),
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