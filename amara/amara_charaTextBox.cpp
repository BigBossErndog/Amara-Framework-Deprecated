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

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				Amara::TextBox::init(gameProperties, givenScene, givenParent);

                add(container = new Container(0, 0));
                portrait->add(portrait = new Sprite(0, 0));
                
                data["entityType"] = "charaTextBox";
            }

            void configure(nlohmann::json& config) {
                Amara::TextBox::configure(config);
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

                if (config.find("containerAlign") != config.end()) {
                    portraitAlignment = config["align"];
                    switch (portraitAlignment) {
                        case Left:
                            marginLeft = marginLeft + container->x + container->width;
                            break;
                        case Right:
                            marginRight = marginRight + (width - (container->x + container->width));
                            break;
                    }
                }

                if (config.find("portraitTexture") != config.end()) {
                    portrait->setTexture("portraitTexture");
                }

            }
    };
}

#endif