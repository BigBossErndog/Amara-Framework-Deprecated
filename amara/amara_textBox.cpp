#pragma once
#ifndef AMARA_TEXTBOX
#define AMARA_TEXTBOX

#include "amara.h"

namespace Amara {
    class TextBox: public Amara::UIBox {
        public:
            std::string text;
            std::string wrappedText;
            std::string recText;
            std::string txtProgress;

            std::string progressControl;

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

            int openSpeedX = 0;
            int openSpeedY = 0;
            int closeSpeedX = 0;
            int closeSpeedY = 0;

            Amara::Alignment horizontalAlignment = ALIGN_LEFT;
            Amara::Alignment verticalAlignment = ALIGN_TOP;

            Amara::StateManager* copySm = nullptr;
            Amara::StateManager mySm;

            Amara::Entity* progressIcon = nullptr;

            Amara::Color textColor = {0, 0, 0, 0};

            bool keepOpen = false;

            TextBox() : Amara::UIBox() {}

            TextBox(float gx, float gy, int gw, int gh, std::string gTextureKey, std::string gFontKey): Amara::UIBox(gx, gy, gw, gh, gTextureKey) {
                fontKey = gFontKey;
            }
            TextBox(int gw, int gh, std::string gTextureKey, std::string gFontKey): TextBox(0, 0, gw, gh, gTextureKey, gFontKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				Amara::UIBox::init(gameProperties, givenScene, givenParent);
                
                txt = new TrueTypeFont(0, 0);
                add(txt);
                if (!fontKey.empty()) {
                    setFont(fontKey);
                }

                data["entityType"] = "textBox";
			}

            virtual void configure(nlohmann::json& config) {
                Amara::UIBox::configure(config);
                if (config.find("text") != config.end()) {
                    setText(config["text"]);
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
                if (config.find("progressControl") != config.end()) {
                    progressControl = config["progressControl"];
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

                setOpenSpeed(openSpeedX, openSpeedY);
                setCloseSpeed(closeSpeedX, closeSpeedY);
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

            virtual void update() {
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
                    if (!progressControl.empty() && controls->justDown(progressControl) && allowSkip) {
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
                
                txt->color = textColor;
                txt->alignment = horizontalAlignment;
                txt->setWordWrap(false);
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
                        txt->x = width - nMarginRight;
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
                    
                if (txtProgress.compare(txt->text) != 0) {
                    txt->setText(txtProgress);
                }
            }

            void setText(std::string newText) {
                std::string word;
                int pos = 0;

                int nMarginLeft = marginLeft + extraMarginLeft;
                int nMarginRight = marginRight + extraMarginRight;

                txt->setWordWrap(false);

                text = newText;
                wrappedText = adjustText(newText, width - (nMarginLeft + nMarginRight));
                recText = text;

                finishedProgress = false;

                timeCounter = 0;
                progress = 0;
            }

            std::string adjustText(std::string gText, float wrapWidth) {
                std::string recText = txt->text;
                std::string fText = "";
                std::string word = "";
                std::string pText = "";
                float textWidth = 0;
                char c;

                txt->setWordWrap(false);

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

                        pText = fText + c;
                        txt->setText(pText);
                        if (txt->width > wrapWidth) {
                            fText += '\n';
                        }
                        else {
                            fText += c;
                        }

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

            void setTextColor(int r, int g, int b) {
                textColor.r = r;
                textColor.g = g;
                textColor.b = b;
            }
            void setTextColor(Amara::Color gColor) {
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
            void setProgressive(bool gProgressive) {
                setProgressive(gProgressive, progressDelay);
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

            Amara::StateManager& checkSm() {
                if (copySm != nullptr) {
                    return *copySm;
                }
                else {
                    return mySm;
                }
            }

            virtual bool say(std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (open()) {
                    toReturn = true;
                }

                sm.wait(sayStartDelay);

                if (sm.once()) {
                    setText(gText);
                    toReturn = true;
                }

                if (sm.evt()) {
                    if (finishedProgress) {
                        autoProgressCounter = 0;
                        if (!autoProgress && progressIcon != nullptr) {
                            progressIcon->setVisible(true);
                        }
                        sm.nextEvt();
                    }

                    toReturn = true;
                }

                if (sm.evt()) {
                    if (autoProgress) {
                        autoProgressCounter += 1;
                        if (autoProgressCounter >= autoProgressDelay) {
                            sm.nextEvt();
                        }
                    }
                    else if (progressControl.empty() || controls->justDown(progressControl)) {
                        progressIcon->setVisible(false);
                        sm.nextEvt();
                    }
                }

                return toReturn;
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
                    setText("");
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

                if (sm.once()) {
                    setText("");
                    toReturn = true;
                }

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

            virtual void setProgressControl(std::string gControl) {
                progressControl = gControl;
            }
    };
}

#endif