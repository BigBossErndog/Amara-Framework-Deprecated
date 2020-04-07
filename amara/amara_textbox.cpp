#ifndef AMARA_TEXTBOX
#define AMARA_TEXTBOX

#include "amara.h"

namespace Amara {
    class Textbox: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            Amara::ImageTexture* texture = nullptr;
            std::string textureKey;

            SDL_Rect viewport;
            SDL_Rect srcRect;
            SDL_Rect destRect;
            SDL_Point origin;

            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

            int width = 0;
            int height = 0;
            int imageWidth = 0;
            int imageHeight = 0;

            int frame = 0;

            float originX = 0;
            float originY = 0;
            
            std::string fontKey;
            Amara::TrueTypeFont* txt;

            Amara::StateManager sm;

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
				properties = gameProperties;
                load = properties->loader;
                gRenderer = properties->gRenderer;

                if (!textureKey.empty()) {
                    setTexture(textureKey);
                }

                Amara::Actor::init(gameProperties, givenScene, givenParent);
                
                txt = new TrueTypeFont(0, 0);
                add(txt);
                if (!fontKey.empty()) {
                    setFont(fontKey);
                }
			}

            virtual void update() {

            }

            virtual void drawBoxPart(int vx, int vy, int vw, int vh) {
                
            }

            virtual void draw(int vx, int vy, int vw, int vh) {

                Amara::Entity::draw(vx, vy, vw, vh);
            }

            bool setTexture(std::string gTextureKey) {
                texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                   textureKey = texture->key;

                    if (texture->type == SPRITESHEET) {
                        width = ((Amara::Spritesheet*)texture)->frameWidth;
                        height = ((Amara::Spritesheet*)texture)->frameHeight;
                    }
                    else {
                        width = texture->width;
                        height = texture->height;
                    }

                    imageWidth = width;
                    imageHeight = height;

                    return true;
                }
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
                }
                return false;
            }

            void setFont(std::string gFontKey) {
                fontKey = gFontKey;
                txt->setFont(fontKey);
            }
    };
}

#endif