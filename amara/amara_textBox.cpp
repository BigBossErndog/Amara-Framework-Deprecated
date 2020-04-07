#ifndef AMARA_TEXTBOX
#define AMARA_TEXTBOX

#include "amara.h"

namespace Amara {
    class TextBox: public Amara::ImageBox {
        public:
            std::string fontKey;
            Amara::TrueTypeFont* txt;

            Amara::StateManager* copySM = nullptr;
            Amara::StateManager sm;

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

            }

            void setFont(std::string gFontKey) {
                fontKey = gFontKey;
                txt->setFont(fontKey);
            }
    };
}

#endif