namespace Amara {
    class TextBox: public Amara::UIBox {
        public:
            std::string text;
            std::string wrappedText;
            std::string recText;
            std::string txtProgress;

            unsigned int progress = 0;
            unsigned int timeCounter = 0;
            unsigned int progressDelay = 2;
            unsigned int progressSpeed = 1;
            bool finishedProgress = false;

            float sayStartDelay = 0.2;

            bool isProgressive = false;
            bool allowSkip = true;
            bool autoProgress = false;
            int autoProgressDelay = 60;
            int autoProgressCounter = 0;
			bool autoProgressSkip = false;

            std::string fontKey;
            Amara::TrueTypeFont* txt;

            int marginTop = 16;
            int marginBottom = 16;
            int marginLeft = 16;
            int marginRight = 16;

            int extraMarginTop = 0;
            int extraMarginBottom = 0;
            int extraMarginLeft = 0;
            int extraMarginRight = 0;

            int recWrapWidth = -1;

            Amara::Alignment horizontalAlignment = ALIGN_LEFT;
            Amara::Alignment verticalAlignment = ALIGN_TOP;

            Amara::Entity* progressIcon = nullptr;

            TextBox() : Amara::UIBox() {}

            TextBox(float gx, float gy, int gw, int gh, std::string gTextureKey, std::string gFontKey): Amara::UIBox(gx, gy, gw, gh, gTextureKey) {
                fontKey = gFontKey;
            }
            TextBox(int gw, int gh, std::string gTextureKey, std::string gFontKey): TextBox(0, 0, gw, gh, gTextureKey, gFontKey) {}

            TextBox(Amara::StateManager* gsm): UIBox(gsm) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                properties = gameProperties;
                scene = givenScene;
                parent = givenParent;
                assets = properties->assets;

                txt = new TrueTypeFont(0, 0);
                add(txt);
                if (!fontKey.empty()) {
                    setFont(fontKey);
                }
                
				Amara::UIBox::init(gameProperties, givenScene, givenParent);

                entityType = "textBox";
			}

            virtual void configure(nlohmann::json config) {
                Amara::UIBox::configure(config);
                if (config.find("text") != config.end()) {
                    setText(config["text"]);
                }
                if (config.find("textConfig") != config.end()) {
                    txt->configure(config["textConfig"]);
                }
                if (config.find("progressDelay") != config.end()) {
                    progressDelay = config["progressDelay"];
                }
                if (config.find("progressSpeed") != config.end()) {
                    progressSpeed = config["progressSpeed"];
                }
                if (config.find("isProgressive") != config.end()) {
                    isProgressive = config["isProgressive"];
                }
                if (config.find("allowSkip") != config.end()) {
                    allowSkip = config["allowSkip"];
                }
                if (config.find("autoProgress") != config.end()) {
                    autoProgress = config["autoProgress"];
                }
                if (config.find("autoProgressDelay") != config.end()) {
                    autoProgressDelay = config["autoProgressDelay"];
                }
                if (config.find("margin") != config.end()) {
                    setMargin(config["margin"]);
                }
                if (config.find("marginTop") != config.end()) {
                    marginTop = config["marginTop"];
                }
                if (config.find("marginBottom") != config.end()) {
                    marginBottom = config["marginBottom"];
                }
                if (config.find("marginLeft") != config.end()) {
                    marginLeft = config["marginLeft"];
                }
                if (config.find("marginRight") != config.end()) {
                    marginRight = config["marginRight"];
                }
                if (config.find("font") != config.end()) {
                    setFont(config["font"]);
                }
				if (config.find("textAlignHorizontal") != config.end()) {
					int a = config["textAlignHorizontal"];
					switch (a) {
						case 0:
							horizontalAlignment = ALIGN_LEFT;
							break;
						case 1:
							horizontalAlignment = ALIGN_RIGHT;
							break;
						case 2:
							horizontalAlignment = ALIGN_CENTER;
							break;
					}
				}
				if (config.find("textAlignVertical") != config.end()) {
					int a = config["textAlignVertical"];
					switch (a) {
						case 0:
							verticalAlignment = ALIGN_TOP;
							break;
						case 1:
							verticalAlignment = ALIGN_BOTTOM;
							break;
						case 2:
							verticalAlignment = ALIGN_CENTER;
							break;
					}
				}
            }

            void clearExtraMargins() {
                extraMarginTop = 0;
                extraMarginBottom = 0;
                extraMarginLeft = 0;
                extraMarginRight = 0;
            }

            Amara::Entity* addProgressIcon(Amara::Entity* icon) {
                progressIcon = icon;
                add(icon);
                icon->setVisible(false);
                return icon;
            }

            virtual void showProgressIcon() {
                if (progressIcon) progressIcon->setVisible(true);
            }
            virtual void hideProgressIcon() {
                if (progressIcon) progressIcon->setVisible(false);
            }

            virtual void textboxUpdate() {
                int nMarginTop = marginTop + extraMarginTop;
                int nMarginBottom = marginBottom + extraMarginBottom;
                int nMarginLeft = marginLeft + extraMarginLeft;
                int nMarginRight = marginRight + extraMarginRight;

                int wrapWidth = width - nMarginLeft - nMarginRight;

                if (recText.compare(text) != 0 || wrapWidth != recWrapWidth) {
                    recWrapWidth = wrapWidth;
                    setText(text);
                }

                if (!isProgressive) {
                    txtProgress = wrappedText;
                    finishedProgress = true;
                }
                else {
                    if (progressControl() && allowSkip) {
                        progress = wrappedText.length();
                    }

                    timeCounter += 1;
                    if (timeCounter >= progressDelay) {
                        timeCounter = 0;
                        progress += progressSpeed;

                    }
                    if (progress >= wrappedText.length()) {
                        progress = wrappedText.length();
                        txtProgress = wrappedText;
                        finishedProgress = true;

                    }
                    else {
                        txtProgress = wrappedText.substr(0, progress);
                    }
                }

                if (txtProgress.compare(txt->text) != 0) {
                    txt->setText(txtProgress);
                }
            }

            void setText(std::string newText) {
                int pos = 0;

                int nMarginLeft = marginLeft + extraMarginLeft;
                int nMarginRight = marginRight + extraMarginRight;
                int wrapWidth = width - nMarginLeft - nMarginRight;

                txt->setWordWrap((isProgressive) ? false : wrapWidth);

                text = newText;
                wrappedText = adjustText(newText, width - (nMarginLeft + nMarginRight));
                recText = text;

                finishedProgress = false;

                timeCounter = 0;
                progress = 0;

				fixText();
                txt->setText("");
            }

            std::string adjustText(std::string gText, float wrapWidth) {
                txt->setWordWrap((isProgressive) ? false : wrapWidth);
                if (!isProgressive) {
                    return gText;
                }

                return StringParser::wrapString(txt->fontAsset->font, gText, wrapWidth);
            }

			void fixText() {
				int nMarginTop = marginTop + extraMarginTop;
                int nMarginBottom = marginBottom + extraMarginBottom;
                int nMarginLeft = marginLeft + extraMarginLeft;
                int nMarginRight = marginRight + extraMarginRight;

                int wrapWidth = width - nMarginLeft - nMarginRight;
                
                txt->setWordWrap((isProgressive) ? false : wrapWidth);
                txt->setText(wrappedText);

                switch (horizontalAlignment) {
                    case ALIGN_LEFT:
                        txt->originX = 0;
                        txt->x = nMarginLeft;
                        break;
                    case ALIGN_CENTER:
                        txt->originX = 0;
                        txt->x = (nMarginLeft + (width - nMarginRight))/2 - txt->width/2;
                        break;
                    case ALIGN_RIGHT:
                        txt->originX = 1;
                        txt->x = width - nMarginRight - txt->width;
                        break;
                }
                txt->x -= width*originX;

                switch (verticalAlignment) {
                    case ALIGN_TOP:
                        txt->y = nMarginTop;
                        break;
                    case ALIGN_CENTER:
                        txt->y = (nMarginTop + (height - nMarginBottom))/2 - txt->height/2;
                        break;
                    case ALIGN_BOTTOM:
                        txt->y = height - nMarginBottom - txt->height;
                        break;
                }
                txt->y -= height*originY;
			}

            void setFont(std::string gFontKey) {
                fontKey = gFontKey;
                txt->setFont(fontKey);
            }

            void setMargin(int t, int b, int l, int r) {
                marginTop = t;
                marginBottom = b;
                marginLeft = l;
                marginRight = r;
            }
            void setMargin(int g) {
                setMargin(g, g, g, g);
            }

            void setProgressive(int gDelay) {
                isProgressive = true;
                progressDelay = gDelay;
                progress = 0;
                timeCounter = 0;
            }
            void setProgressive(bool gProgressive, int gDelay) {
                setProgressive(gDelay);
                isProgressive = gProgressive;
            }
            void setProgressive(bool gProgressive) {
                setProgressive(gProgressive, progressDelay);
            }
            void setProgressive() {
                setProgressive(true);
            }

            virtual bool say(std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    setText("");
                    txt->setText("");
                }

                if (open()) {
                    toReturn = true;
                }

                sm.wait(sayStartDelay);

                if (sm.once()) {
                    setText(gText);
                    toReturn = true;
                }

                if (sm.evt()) {
                    textboxUpdate();
                    if (finishedProgress) {
                        autoProgressCounter = 0;
                        if (!autoProgress && progressIcon != nullptr) {
                            showProgressIcon();
                        }
                        sm.nextEvt();
                    }

                    toReturn = true;
                }

                if (sm.evt()) {
                    if (autoProgress) {
                        autoProgressCounter += 1;
                        if (autoProgressCounter >= autoProgressDelay || (autoProgressSkip && progressControl())) {
							onProgress();
                            sm.nextEvt();
                        }
                    }
                    else if (progressControl()) {
                        if (progressIcon != nullptr) hideProgressIcon();
						onProgress();
                        sm.nextEvt();
                    }
                }

                return toReturn;
            }

            void setTextColor(int r, int g, int b, int a) {
                if (txt) txt->setColor(r, g, b, a);
            }
            void setTextColor(int r, int g, int b) {
                setTextColor(r, g, b, 255);
            }
            void setTextOutline(int o, int r, int g, int b, int a) {
                if (txt) txt->setOutline(o, r, g, b, a);
            }
            void setTextOutline(int o, int r, int g, int b) {
                setTextOutline(o, r, g, b, 255);
            }

            virtual bool close() {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    setText("");
                    txt->setText("");
                    toReturn = true;
                }

                if (Amara::UIBox::close()) {
                    toReturn = true;
                }

                return toReturn;
            }

            std::string inputKey = "confirm";
            virtual bool progressControl() {
                if (controls->justDown(inputKey) || interact.tapped) {
                    return true;
                }
                return false;
            }

			void setAutoProgressDelay(float s) {
				autoProgress = true;
				autoProgressDelay = ceil(properties->lps*s);
			}

			virtual void onProgress() {}
    };
}