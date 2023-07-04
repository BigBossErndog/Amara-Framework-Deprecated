#pragma once
#ifndef AMARA_TRUETYPEFONT
#define AMARA_TRUETYPEFONT



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

            int outline = 0;
            bool outlineCorners = true;
            Amara::Color outlineColor = FC_MakeColor(255, 255, 255, 255);
			float outlineAlpha = 1;
			bool outlineAlphaLocked = 1;
			float outlineAlphaPow = 1;

			IntRect viewbox;

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

                entityType = "trueTypeFont";
			}

            virtual void configure(nlohmann::json config) {
                Amara::Actor::configure(config);
                if (config.find("text") != config.end()) {
                    setText(config["text"]);
                }
                if (config.find("font") != config.end()) {
                    setFont(config["font"]);
                }
                if (config.find("outline") != config.end()) {
                    outline = config["outline"];
                }
                if (config.find("outlineCorners") != config.end()) {
                    outlineCorners = config["outlineCorners"];
                }
				if (config.find("align") != config.end()) {
					int a = config["align"];
					switch (a) {
						case 0:
							alignment = ALIGN_LEFT;
							break;
						case 1:
							alignment = ALIGN_RIGHT;
							break;
						case 2:
							alignment = ALIGN_CENTER;
							break;
					}
				}
                if (config.find("r") != config.end()) color.r = config["r"];
                if (config.find("g") != config.end()) color.g = config["g"];
                if (config.find("b") != config.end()) color.b = config["b"];
                if (config.find("a") != config.end()) color.a = config["a"];
            }

            bool setFont(std::string gFontKey) {
                fontAsset = (Amara::TTFAsset*)(load->get(gFontKey));
                if (fontAsset != nullptr) {
                    fontKey = gFontKey;
                    findDimensions();
                    return true;
                }
                else {
                    SDL_Log("Font with key: \"$s\" was not found.", gFontKey.c_str());
                }
                return false;
            }

            void setText(std::string newTxt) {
                text = newTxt;
                const char* txt = text.c_str();
                findDimensions();
            }

            std::string getWrappedText(std::string gText, float wrapWidth) {
                int testSize = gText.size() * 2;
                char wrapping[testSize];

                FC_GetWrappedText(fontAsset->font, wrapping, testSize, wrapWidth,gText.c_str());

                return wrapping;
            }

            void setWrappedText(std::string gText, float wrapWidth) {
                setText(getWrappedText(gText, wrapWidth));
            }

            void setColor(int r, int g, int b, int a) {
                color.r = r;
                color.g = g;
                color.b = b;
                color.a = a;
            }
            void setColor(int r, int g, int b) {
                setColor(r, g, b, 255);
            }
            void setColor(Amara::Color gColor) {
                color = gColor;
            }

            void setOutlineColor(int r, int g, int b, int a) {
                outlineColor.r = r;
                outlineColor.g = g;
                outlineColor.b = b;
                outlineColor.a = a;
            }
            void setOutlineColor(int r, int g, int b) {
                setOutlineColor(r, g, b, 255);
            }
            void setOutlineColor(SDL_Color gColor) {
                outlineColor = gColor;
            }

            void setOutline(int size, int r, int g, int b, int a) {
                outline = size;
                setOutlineColor(r, g, b, a);
            }
            void setOutline(int size, int r, int g, int b) {
                setOutline(size, r, g, b, 255);
            }
            void setOutline(int size, SDL_Color gColor) {
                outline = size;
                setOutlineColor(gColor);
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
                wordWrap = true;
                if (w == 0 || w == false) {
                    wordWrapWidth = 0;
                    wordWrap = false;
                }
                findDimensions();
            }

            void findDimensions() {
                const char* txt = text.c_str();
                if (fontAsset == nullptr) return;

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
                Amara::Actor::run();
            }

            void drawText(float dx, float dy) {
				switch (alignment) {
					case ALIGN_LEFT:
						effect.alignment = FC_ALIGN_LEFT;
						break;
					case ALIGN_CENTER:
						effect.alignment = FC_ALIGN_CENTER;
						break;
					case ALIGN_RIGHT:
						effect.alignment = FC_ALIGN_RIGHT;
						break;
				}

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

                effect.scale.x = scaleX * nzoomX;
                effect.scale.y = scaleY * nzoomY;

				const char* txt = text.c_str();

				int txtWidth, txtHeight;
				if (wordWrap) {
                    txtWidth = wordWrapWidth;
                    txtHeight = FC_GetColumnHeight(fontAsset->font, wordWrapWidth, txt);
                }
                else {
                    txtWidth = FC_GetWidth(fontAsset->font, txt);
                    txtHeight = FC_GetHeight(fontAsset->font, txt);
                }

                if (fontAsset != nullptr) {
                    if (wordWrap) {
                        int offsetX = 0;
                        if (alignment == ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == ALIGN_RIGHT) {
                            offsetX = width;
                        }

						int rx = floor((dx - properties->scrollX + properties->offsetX - (width * originX) + offsetX) * nzoomX);
						int ry = floor((dy-z - properties->scrollY + properties->offsetY - (height * originY)) * nzoomY);
                        
                        FC_DrawColumnEffect(
                            fontAsset->font,
                            gRenderer,
                            rx, ry,
                            wordWrapWidth,
                            effect,
                            txt
                        );

						checkHover(viewbox.x, viewbox.y, viewbox.width, viewbox.height, rx, ry, width * effect.scale.x, height * effect.scale.y);
                    }
                    else {
                        int offsetX = 0;
                        if (alignment == ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == ALIGN_RIGHT) {
                            offsetX = width;
                        }

						int rx = floor((dx - properties->scrollX + properties->offsetX - (width * originX) + offsetX) * nzoomX);
						int ry = floor((dy-z - properties->scrollY + properties->offsetY - (height * originY)) * nzoomY);

                        FC_DrawEffect(
                            fontAsset->font,
                            gRenderer,
                            rx, ry,
                            effect,
                            txt
                        );

						checkHover(viewbox.x, viewbox.y, viewbox.width, viewbox.height, rx, ry, txtWidth * effect.scale.x, txtHeight * effect.scale.y);
                    }
                }
            }

            void draw(int vx, int vy, int vw, int vh) {
                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(gRenderer, &viewport);

				viewbox = { vx, vy, vw, vh };

                color.a = alpha * properties->alpha * 255;

                if (outline) {
                    effect.color = outlineColor;
					effect.color.a = outlineColor.a * pow(alpha, 1) * outlineAlpha;
                    for (int i = 0; i < outline+1; i++) {
                        drawText(x+i,y);
                        drawText(x-i,y);
                        for (int j = 0; j < outline+1; j++) {
                            if (outlineCorners || i != j || i != outline) {
                                drawText(x+i,y+j);
                                drawText(x-i,y-j);
                                drawText(x+i,y-j);
                                drawText(x-i,y+j);
                            }
                        }
                    }
                }
                effect.color = color;
                drawText(x, y);

                Amara::Entity::draw(vx, vy, vw, vh);
            }
    };
}

#endif
