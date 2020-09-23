#pragma once
#ifndef AMARA_CHARATEXTBOX
#define AMARA_CHARATEXTBOX

#include "amara.h"

namespace Amara {
    class CharaTextBox: public Amara::TextBox {
        public:
            Amara::Container* container = nullptr;
            Amara::Sprite* portrait = nullptr;

            Amara::Direction portraitAlignment = Left;
            Amara::Direction portraitVerticalAlignment = Down;

            nlohmann::json defaultConfig;

            CharaTextBox(): Amara::TextBox() {}
            CharaTextBox(float gx, float gy, float gw, float gh, std::string gTextureKey, std::string gFontKey): Amara::TextBox(gx, gy, gw, gh, gTextureKey, gFontKey) {}
            CharaTextBox(Amara::StateManager* gsm): Amara::TextBox(gsm) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				Amara::TextBox::init(gameProperties, givenScene, givenParent);

                add(container = new Container(0, 0));
                container->add(portrait = new Sprite(0, 0));
                container->setVisible(false);
                portrait->setOrigin(0.5);

                data["entityType"] = "charaTextBox";
            }

            void configure(nlohmann::json config) {
                bool noneFound = true;
                if (config.find("textBox") != config.end()) {
                    Amara::TextBox::configure(config["textBox"]);
                    noneFound = false;
                }
                if (config.find("portrait") != config.end()) {
                    configurePortrait(config["portrait"]);
                    noneFound = false;
                }
                if (config.find("default") != config.end()) {
                    defaultConfig = config["default"];
                    configure(defaultConfig);
                    noneFound = false;
                }

                if (noneFound) {
                    Amara::TextBox::configure(config);
                }
            }

            void configurePortrait(nlohmann::json& config) {
                if (config.find("portraitContainerWidth") != config.end()) {
                    container->width = config["portraitContainerWidth"];
                }
                if (config.find("portraitContainerHeight") != config.end()) {
                    container->height = config["portraitContainerHeight"];
                }

                if (config.find("portraitContainerX") != config.end()) {
                    container->x = config["portraitContainerX"];
                }
                if (config.find("portraitContainerY") != config.end()) {
                    container->y = config["portraitContainerY"];
                }
                if (config.find("portraitContainerXFromRight") != config.end()) {
                    int gx = config["portraitContainerXFromRight"];
                    container->x = width - container->width - gx;
                }
                if (config.find("portraitContainerYFromBottom") != config.end()) {
                    int gy = config["portraitContainerYFromBottom"];
                    container->y = height - container->height - gy;
                }

                portrait->x = container->width/2.0;
                portrait->y = container->height/2.0;

                if (config.find("portraitX") != config.end()) {
                    portrait->x = config["portraitX"];
                }
                if (config.find("portraitY") != config.end()) {
                    portrait->y = config["portraitY"];
                }

                if (config.find("portraitOriginX") != config.end()) {
                    portrait->originX = config["portraitOriginX"];
                }
                if (config.find("portraitOriginY") != config.end()) {
                    portrait->originY = config["portraitOriginY"];
                }

                if (config.find("portraitAlignment") != config.end()) {
                    portraitAlignment = config["portraitAlignment"];
                }

                if (config.find("portraitVerticalAlignment") != config.end()) {
                    portraitVerticalAlignment = config["portraitVerticalAlignment"];
                }

                manageMargins();

                if (config.find("portraitTexture") != config.end()) {
                    portrait->setTexture(config["portraitTexture"]);
                }
                if (config.find("portraitFrame") != config.end()) {
                    portrait->setFrame(config["portraitFrame"]);
                }
                if (config.find("portraitAnimation") != config.end()) {
                    portrait->play(config["portraitAnimation"]);
                }
            }

            void manageMargins() {
                switch (portraitAlignment) {
                    case Left:
                        extraMarginLeft = container->x + container->width + marginLeft;
                        break;
                    case Right:
                        extraMarginRight = (width - container->x) - marginRight;
                        break;
                }
            }

            bool say(nlohmann::json& charaData, bool(*showPortrait)(Amara::CharaTextBox*, Amara::Container*), std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (open()) {
                    toReturn = true;
                }

                if (sm.once()) {
                    container->setVisible(true);
                    if (charaData.find("portrait") == charaData.end()) container->setAlpha(0);
                    configure(defaultConfig);
                    configure(charaData);
                    manageMargins();
                    setText("");
                    if (keepOpen) {
                        openWidth = width;
                        openHeight = height;
                    }
                    toReturn = true;
                }

                if (showPortrait != nullptr) {
                    if (showPortrait(this, container)) {
                        toReturn = true;
                    }
                }
                else {
                    if (sm.once()) {
                        if (charaData.find("portrait") != charaData.end()) container->setAlpha(1);
                    }
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
                        if (progressIcon != nullptr) progressIcon->setVisible(false);
                        sm.nextEvt();
                    }
                }

                return toReturn;
            }

            bool say(nlohmann::json& charaData, std::string gText) {
                return say(charaData, nullptr, gText);
            }

            bool say(std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    container->setVisible(false);
                    configure(defaultConfig);
                    clearExtraMargins();
                    setText("");
                    toReturn = true;
                }

                if (Amara::TextBox::say(gText)) {
                    toReturn = true;
                }

                return toReturn;
            }

            bool close() {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    container->setVisible(false);
                    setText("");
                    toReturn = true;
                    keepOpen = false;
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
    };
}

#endif
