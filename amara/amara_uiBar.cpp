namespace Amara {
    class UIBar: public Actor {
    public:
        float value = 0;
        float minValue = 0;
        float maxValue = 1;
        float displayValue = 0;

        float changeSpeed = -1;
        int pauseTime = 0;
        int pauseCount = 0;

        Amara::Image* backBar = nullptr;
        Amara::Image* frontBar = nullptr;

        Amara::TrueTypeFont* textDisplay = nullptr;

        std::string textureKey;

        int frontPaddingLeft = 0;
        int frontPaddingRight = 0;
        int frontPaddingTop = 0;
        int frontPaddingBottom = 0;

        bool isBarHeld = false;
        bool hasValueChanged = false;
        
        Amara::Direction drainDirection = Left; 

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
            if (config.find("minValue") != config.end()) {
                minValue = config["minValue"];
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
            if (config.find("frontPaddingTop") != config.end()) {
                frontPaddingTop = config["frontPaddingTop"];
            }
            if (config.find("frontPaddingBottom") != config.end()) {
                frontPaddingBottom = config["frontPaddingBottom"];
            }
            if (config.find("drainDirection") != config.end()) {
                drainDirection = config["drainDirection"];
            }
        }

        void init() {
            add(backBar = new Image(0, 0));
            add(frontBar = new Image(0, 0));
            setTexture(textureKey);
            entityType = "uiBar";
        }

        bool setTexture(std::string gKey) {
            if (gKey.size() == 0) return false;
            if (backBar->setTexture(gKey) && frontBar->setTexture(gKey)) {
                return true;
            }
            return false;
        }

        void reloadAssets() {
            backBar->reloadAssets();
            frontBar->reloadAssets();
            if (textDisplay) textDisplay->reloadAssets();
        }

        void fixSetValue(float gVal) {
            if (gVal < minValue) gVal = minValue;
            if (gVal > maxValue) gVal = maxValue;
            setValue(gVal);
        }

        void setValue(float gVal) {
            float recVal = value;
            value = gVal;
            if (value > maxValue) maxValue = value;
            if (value < minValue) minValue = value;

            if (value != recVal) hasValueChanged = true;
        }
        void setValueInstantly(float gVal) {
            displayValue = gVal;
            setValue(gVal);
        }

        void setPercentage(float val, bool roundIt) {
            if (roundIt) setValue(round(val*maxValue));
            else setValue(val*maxValue);
        }

        TrueTypeFont* addTextDisplay(TrueTypeFont* gTxt) {
            add(textDisplay = gTxt);
            return textDisplay;
        }

        void removeTextDisplay() {
            if (textDisplay) {
                textDisplay->destroy();
                textDisplay = nullptr;
            }
        }

        void setSpeed(float gSpeed, int gDelay) {
            changeSpeed = gSpeed;
            pauseTime = gDelay;
        }
        void setSpeed(int gDelay) {
            setSpeed(1, gDelay);
        }

        void setInteractable(bool g) {
            Actor::setInteractable(g);
            backBar->setInteractable(g);
        }

        void run() {
            hasValueChanged = false;
            if (isBarHeld) {
                isBarHeld = false;
                if (isInteractable) {
                    if (input->lastMode == InputMode_Touch && backBar->interact.finger && backBar->interact.finger->isDown) isBarHeld = true;
                    if (input->lastMode == InputMode_Mouse && backBar->interact.mouse && backBar->interact.mouse->isDown) isBarHeld = true; 
                }
            }
            if (isInteractable) {
                if (backBar->interact.isDown) isBarHeld = true;
                if (isBarHeld) {
                    float pixelWidth = frontBar->imageWidth - frontPaddingLeft - frontPaddingRight;
                    float pixelOffset = backBar->interact.pointOffsetX - frontPaddingLeft;
                    float newValue = maxValue*(pixelOffset/pixelWidth);
                    fixSetValue(newValue);
                }
            }

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
                textDisplay->setText(std::to_string((int)round(displayValue)));
            }
            
            if (drainDirection == Left) {
                frontBar->cropLeft = frontPaddingLeft;
                frontBar->cropRight = frontBar->imageWidth - floor((frontBar->imageWidth - frontPaddingLeft - frontPaddingRight)*(displayValue/maxValue)) - frontPaddingLeft;
                if (frontBar->cropRight < frontPaddingRight) frontBar->cropRight = frontPaddingRight;
            }
            else if (drainDirection == Right) {
                frontBar->cropRight = frontPaddingRight;
                frontBar->cropLeft = frontBar->imageWidth - floor((frontBar->imageWidth - frontPaddingLeft - frontPaddingRight)*(displayValue/maxValue)) - frontPaddingRight;
                if (frontBar->cropLeft < frontPaddingLeft) frontBar->cropLeft = frontPaddingLeft;
            }
            else if (drainDirection == Up) {
                frontBar->cropTop = frontPaddingTop;
                frontBar->cropBottom = frontBar->imageHeight - floor((frontBar->imageHeight - frontPaddingTop - frontPaddingBottom)*(displayValue/maxValue)) - frontPaddingTop;
                if (frontBar->cropBottom < frontPaddingBottom) frontBar->cropBottom = frontPaddingBottom;
            }
            else if (drainDirection == Down) {
                frontBar->cropBottom = frontPaddingBottom;
                frontBar->cropTop = frontBar->imageHeight - floor((frontBar->imageHeight - frontPaddingTop - frontPaddingBottom)*(displayValue/maxValue)) - frontPaddingBottom;
                if (frontBar->cropTop < frontPaddingTop) frontBar->cropTop = frontPaddingTop;
            }

            Amara::Actor::run();
        }
    };
}