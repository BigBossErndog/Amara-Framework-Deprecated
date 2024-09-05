namespace Amara {
    enum Alignment {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT,
        ALIGN_TOP,
        ALIGN_BOTTOM
    };

    class TrueTypeFont: public Amara::Actor, public Amara::MakeRect {
        public:
            SDL_Renderer* gRenderer = nullptr;
            Amara::TTFAsset* fontAsset = nullptr;
            std::string fontKey;

            std::string text;
            std::string displayText;
            SDL_Rect viewport;
            Amara::Color color = FC_MakeColor(0, 0, 0, 255);
            Amara::Alignment alignment = ALIGN_LEFT;
            FC_Effect effect;

            float renderOffsetX = 0;
            float renderOffsetY = 0;

            bool wordWrap = false;
            Uint16 wordWrapWidth = 0;

            int outline = 0;
            bool outlineCorners = true;
            Amara::Color outlineColor = FC_MakeColor(255, 255, 255, 255);
			float outlineAlphaRate = 1;

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
                rectInit(this);

                if (!fontKey.empty()) {
                    setFont(fontKey);
                }
                if (!text.empty()) setText(text);

                Amara::Actor::init(gameProperties, givenScene, givenParent);

                entityType = "trueTypeFont";
			}

            virtual void configure(nlohmann::json config) {
                Amara::Actor::configure(config);
                rectConfigure(config);

                if (config.find("text") != config.end()) {
                    setText(config["text"]);
                }
                if (config.find("font") != config.end()) {
                    setFont(config["font"]);
                }
                if (config.find("reloadFont") != config.end()) {
                    nlohmann::json check = config["reloadFont"];
                    if (check.is_boolean() && check) setFont(fontKey);
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
                if (config.find("color") != config.end()) {
                    nlohmann::json check = config["color"];
                    if (check.is_array()) {
                        color.r = check[0];
                        color.g = check[1];
                        color.b = check[2];
                        if (check.size() >= 4) color.a = check[3];
                    }
                }
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

            void reloadAssets() {
                setFont(fontKey);
            }

            Amara::TrueTypeFont* setText(std::string newTxt) {
                text = newTxt;
                if (wordWrap) {
                    displayText = StringParser::wrapString(fontAsset->font, text, wordWrapWidth);
                }
                else displayText = text;
                findDimensions();
                return this;
            }

            Amara::TrueTypeFont* setColor(int r, int g, int b, int a) {
                color.r = r;
                color.g = g;
                color.b = b;
                color.a = a;
                return this;
            }
            Amara::TrueTypeFont* setColor(int r, int g, int b) {
                return setColor(r, g, b, 255);
            }
            Amara::TrueTypeFont* setColor(Amara::Color gColor) {
                color = gColor;
                return this;
            }

            Amara::TrueTypeFont* setOutlineColor(int r, int g, int b, int a) {
                outlineColor.r = r;
                outlineColor.g = g;
                outlineColor.b = b;
                outlineColor.a = a;
                return this;
            }
            Amara::TrueTypeFont* setOutlineColor(int r, int g, int b) {
                return setOutlineColor(r, g, b, 255);
            }
            Amara::TrueTypeFont* setOutlineColor(SDL_Color gColor) {
                outlineColor = gColor;
                return this;
            }

            Amara::TrueTypeFont* setOutline(int size, int r, int g, int b, int a) {
                outline = size;
                return setOutlineColor(r, g, b, a);
            }
            Amara::TrueTypeFont* setOutline(int size, int r, int g, int b) {
                return setOutline(size, r, g, b, 255);
            }
            Amara::TrueTypeFont* setOutline(int size, SDL_Color gColor) {
                outline = size;
                return setOutlineColor(gColor);
            }

            void setWordWrap() {
                wordWrap = true;
                setText(text);
            }
            void setWordWrap(int w) {
                wordWrapWidth = w;
                wordWrap = true;
                if (w == 0 || w == false) {
                    wordWrapWidth = 0;
                    wordWrap = false;
                }
                setWordWrap();
            }

            void findDimensions() {
                if (fontAsset == nullptr) return;
                const char* txt = displayText.c_str();

                width = FC_GetWidth(fontAsset->font, txt);
                height = FC_GetHeight(fontAsset->font, txt);
            }

            void drawText(float dx, float dy, bool middle) {
                if (fontAsset == nullptr) return;

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

				const char* txt = displayText.c_str();

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

						float rx = floor((dx+renderOffsetX - properties->scrollX*scrollFactorX + properties->offsetX - (width * originX) + offsetX) * nzoomX);
						float ry = floor((dy-z+renderOffsetY - properties->scrollY*scrollFactorY + properties->offsetY - (height * originY)) * nzoomY);
                        
                        FC_DrawColumnEffect(
                            fontAsset->font,
                            gRenderer,
                            rx, ry,
                            wordWrapWidth,
                            effect,
                            txt
                        );
                        
						if (middle) checkHover(viewbox.x, viewbox.y, viewbox.width, viewbox.height, rx, ry, width * effect.scale.x, height * effect.scale.y);
                    }
                    else {
                        int offsetX = 0;
                        if (alignment == ALIGN_CENTER) {
                            offsetX = width * 0.5;
                        }
                        else if (alignment == ALIGN_RIGHT) {
                            offsetX = width;
                        }

						float rx = floor((dx+renderOffsetX - properties->scrollX*scrollFactorX + properties->offsetX - (width * originX) + offsetX) * nzoomX);
						float ry = floor((dy-z+renderOffsetY - properties->scrollY*scrollFactorY + properties->offsetY - (height * originY)) * nzoomY);
                        
                        FC_DrawEffect(
                            fontAsset->font,
                            gRenderer,
                            rx, ry,
                            effect,
                            txt
                        );

						if (middle) checkHover(viewbox.x, viewbox.y, viewbox.width, viewbox.height, rx, ry, txtWidth * effect.scale.x, txtHeight * effect.scale.y);
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

                if (fontAsset != nullptr) {
                    color.a = alpha * properties->alpha * 255;

                    int dx = floor(x), dy = floor(y);

                    if (outline) {
                        effect.color = outlineColor;
                        effect.color.a = outlineColor.a * pow(alpha, outlineAlphaRate);
                        for (int i = 0; i < outline+1; i++) {
                            drawText(dx+i,dy, false);
                            drawText(dx-i,dy, false);
                            for (int j = 0; j < outline+1; j++) {
                                if (outlineCorners || i != j || i != outline) {
                                    drawText(dx+i,dy+j, false);
                                    drawText(dx-i,dy-j, false);
                                    drawText(dx+i,dy-j, false);
                                    drawText(dx-i,dy+j, false);
                                }
                            }
                        }
                    }
                    effect.color = color;
                    drawText(dx, dy, true);
                }

                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}
