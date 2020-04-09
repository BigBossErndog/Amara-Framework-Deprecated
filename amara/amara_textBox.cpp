#ifndef AMARA_TEXTBOX
#define AMARA_TEXTBOX

#include "amara.h"

namespace Amara {
    class TextBox: public Amara::ImageBox {
        public:
            std::string text;
            std::string wrappedText;
            std::string recText;
            std::string txtProgress;

            unsigned int progress = 0;
            unsigned int timeCounter = 0;
            unsigned int progressDelay = 16;

            bool isProgressive = false;

            std::string fontKey;
            Amara::TrueTypeFont* txt;

            int marginTop = 16;
            int marginBottom = 16;
            int marginLeft = 16;
            int marginRight = 16;

            Amara::Alignment horizontalAlignment = ALIGN_LEFT;
            Amara::Alignment verticalAlignment = ALIGN_TOP;

            Amara::StateManager* copySm = nullptr;
            Amara::StateManager sm;

            Amara::Color textColor = {0, 0, 0, 0};

            TextBox(float gx, float gy, int gw, int gh, std::string gTextureKey, std::string gFontKey): ImageBox(gx, gy, gw, gh, gTextureKey) {
                fontKey = gFontKey;
            }
            TextBox(int gw, int gh, std::string gTextureKey, std::string gFontKey): TextBox(0, 0, gw, gh, gTextureKey, gFontKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				Amara::ImageBox::init(gameProperties, givenScene, givenParent);
                
                txt = new TrueTypeFont(0, 0);
                add(txt);
                if (!fontKey.empty()) {
                    setFont(fontKey);
                }
			}

            virtual void update() {
                if (recText.compare(text) != 0) {
                    setText(text);
                }

                if (!isProgressive) {
                    txtProgress = wrappedText;
                }
                else {
                    timeCounter += 1;
                    if (timeCounter >= progressDelay) {
                        timeCounter = 0;
                        progress += 1;
                    }
                    if (progress >= wrappedText.length()) {
                        progress = wrappedText.length();
                        txtProgress = text;
                    }
                    else {
                        txtProgress = wrappedText.substr(0, progress);
                    }
                }
                
                txt->color = textColor;
                txt->alignment = horizontalAlignment;
                txt->setWordWrap(width - (marginLeft + marginRight));
                txt->setText(wrappedText);

                switch (horizontalAlignment) {
                    case ALIGN_LEFT:
                        txt->originX = 0;
                        txt->x = marginLeft;
                        break;
                    case ALIGN_CENTER:
                        txt->originX = 0;
                        txt->x = (marginLeft + (width - marginRight))/2 - txt->width/2;
                        break;
                    case ALIGN_RIGHT:
                        txt->originX = 1;
                        txt->x = width - marginRight;
                        break;
                }
                txt->x -= width*originX;

                switch (verticalAlignment) {
                    case ALIGN_TOP:
                        txt->y = marginTop;
                        break;
                    case ALIGN_CENTER:
                        txt->y = (marginTop + (height - marginBottom))/2 - txt->height/2;
                        break;
                    case ALIGN_BOTTOM:
                        txt->y = height - marginBottom - txt->height;
                        break;
                }
                txt->y -= height*originY;
                    
                if (txtProgress.compare(txt->text) != 0) {
                    txt->setText(txtProgress);
                }
            }

            void setText(std::string newText) {
                std::string word;
                int pos = 0;

                text = newText;
                wrappedText = adjustText(newText, width - marginLeft - marginRight);
                recText = text;

                timeCounter = 0;
                progress = 0;
            }

            std::string adjustText(std::string gText, int wrapWidth) {
                std::string recText = txt->text;
                std::string fText = "";
                std::string word = "";
                std::string pText = "";
                int textWidth = 0;
                char c;

                for (int i = 0; i < gText.length(); i++) {
                    c = gText.at(i);
                    if (c == ' ') {
                        pText = fText + word;
                        txt->setText(pText);
                        if (txt->width > wrapWidth) {
                            fText += '\n';
                            fText += word;
                        }
                        else {
                            fText += word;
                        }
                        fText += ' ';
                        word = "";
                    }
                    else {
                        word += c;
                    }
                }
                pText = fText + word;
                txt->setText(pText);
                if (txt->width > wrapWidth) {
                    fText += '\n';
                    fText += word;
                }
                else {
                    fText += word;
                }
                
                txt->setText(recText);
                
                return fText;
            }

            void setFont(std::string gFontKey) {
                fontKey = gFontKey;
                txt->setFont(fontKey);
            }

            void setColor(int r, int g, int b) {
                textColor.r = r;
                textColor.g = g;
                textColor.b = b;
            }
            void setColor(Amara::Color gColor) {
                textColor = gColor;
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
            void setProgressive() {
                setProgressive(true);
            }

            void copyStateManager(Amara::StateManager* gsm) {
                copySm = gsm;
            }
            void copyStateManager(Amara::StateManager& gsm) {
                copySm = &gsm;
            }

            void checkSm() {
                if (copySm != nullptr) {
                    sm = *copySm;
                }
            }
    };
}

#endif