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

            CharaTextBox(): Amara::TextBox() {}
            CharaTextBox(float gx, float gy, float gw, float gh, std::string gTextureKey, std::string gFontKey): Amara::TextBox(gx, gy, gw, gh, gTextureKey, gFontKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				Amara::TextBox::init(gameProperties, givenScene, givenParent);

                add(container = new Container(0, 0));
                container->add(portrait = new Sprite(0, 0));
                container->setVisible(false);
                portrait->setOrigin(0.5);
                
                data["entityType"] = "charaTextBox";
            }

            void configure(nlohmann::json& config) {
                Amara::TextBox::configure(config);
                configurePortrait(config);
            }

            void configurePortrait(nlohmann::json& config) {
                if (config.find("containerWidth") != config.end()) {
                    container->width = config["containerWidth"];
                }
                if (config.find("containerHeight") != config.end()) {
                    container->height = config["containerHeight"];
                }
                
                if (config.find("containerX") != config.end()) {
                    container->x = config["containerX"];
                }
                if (config.find("containerY") != config.end()) {
                    container->y = config["containerY"];
                }

                if (config.find("containerXFromRight") != config.end()) {
                    int gx = config["xFromRight"];
                    container->x = width - container->width - gx;
                }
                if (config.find("containerYFromBottom") != config.end()) {
                    int gy = config["containerYFromBottom"];
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
            }

            void manageMargins() {
                switch (portraitAlignment) {
                    case Left:
                        extraMarginLeft = container->width + marginLeft;
                        break;
                    case Right:
                        extraMarginRight = container->height + marginRight;
                        break;
                }
            }

            bool say(nlohmann::json& charaData, std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (open()) {
                    toReturn = true;
                }

                if (sm.once()) {
                    container->setVisible(true);
                    configurePortrait(charaData);
                    manageMargins();
                    setText("");
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
                        sm.nextEvt();
                    }
                }

                return toReturn;
            }

            bool say(std::string gText) {
                Amara::StateManager& sm = checkSm();
                bool toReturn = false;

                if (sm.once()) {
                    container->setVisible(false);
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