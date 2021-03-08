#pragma once
#include "amara.h"

namespace Amara {
    class UIBar: public Actor {
    public:
        int value = 0;
        int maxValue = 1;
        int displayValue = 0;

        int changeSpeed = -1;
        int pauseTime = 0;
        int pauseCount = 0;

        Amara::Image* backBar = nullptr;
        Amara::Image* frontBar = nullptr;

        Amara::TrueTypeFont* textDisplay = nullptr;

        std::string textureKey;

        int frontPaddingLeft = 0;
        int frontPaddingRight = 0;

        UIBar() {}

        UIBar(float gx, float gy) {
            x = gx;
            y = gy;
        }

        UIBar(float gx, float gy, std::string gkey): UIBar(gx, gy) {
            textureKey = gkey;
        }

        void configure(nlohmann::json config) {
            Amara::Actor::configure(config);
            if (config.find("value") != config.end()) {
                setValueInstantly(config["value"]);
            }
            if (config.find("displayValue") != config.end()) {
                displayValue = config["displayValue"];
            }
            if (config.find("maxValue") != config.end()) {
                maxValue = config["maxValue"];
            }
            if (config.find("frontPaddingLeft") != config.end()) {
                frontPaddingLeft = config["frontPaddingLeft"];
            }
            if (config.find("frontPaddingRight") != config.end()) {
                frontPaddingLeft = config["frontPaddingRight"];
            }
        }

        void init() {
            add(backBar = new Image(0, 0));
            add(frontBar = new Image(0, 0));
            setTexture(textureKey);
        }

        bool setTexture(std::string gKey) {
            if (gKey.size() == 0) return false;
            if (backBar->setTexture(gKey) && frontBar->setTexture(gKey)) {
                return true;
            }
            return false;
        }

        void setValue(int gVal) {
            value = gVal;
            if (value > maxValue) {
                maxValue = value;
            }
        }
        void setValueInstantly(int gVal) {
            displayValue = gVal;
            setValue(gVal);
        }

        TrueTypeFont* addTextDisplay(TrueTypeFont* gTxt) {
            add(textDisplay = gTxt);
            return textDisplay;
        }

        void run() {
            if (changeSpeed == -1) {
                displayValue = value;
            }
            else {
                pauseCount += 1;
                if (pauseCount >= pauseTime) {
                    pauseCount = 0;
                    changeSpeed = abs(changeSpeed);
                    if (displayValue < value) {
                        displayValue += changeSpeed;
                        if (displayValue > value) displayValue = value;
                    }
                    else if (displayValue > value) {
                        displayValue -= changeSpeed;
                        if (displayValue < value) displayValue = value;
                    }
                }
            }
            if (textDisplay) {
                textDisplay->setText(std::to_string(displayValue));
            }

            frontBar->cropLeft = frontPaddingLeft;
            frontBar->cropRight = frontBar->imageWidth - ceil((frontBar->imageWidth - frontPaddingLeft - frontPaddingRight)*(displayValue/(float)maxValue)) - frontPaddingLeft;
            if (frontBar->cropRight < frontPaddingRight) frontBar->cropRight = frontPaddingRight;

            Amara::Actor::run();
        }
    };
}