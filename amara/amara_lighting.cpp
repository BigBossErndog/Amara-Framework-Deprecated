#pragma once
#ifndef AMARA_LIGHTING
#define AMARA_LIGHTING

#include "amara.h"

namespace Amara {
    class LightLayer: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            SDL_Texture* lightTexture = nullptr;
            SDL_Color fillColor;
            SDL_Color recColor;

            SDL_BlendMode blendMode = SDL_BLENDMODE_MOD;

            SDL_Texture* recTarget = nullptr;

            float width;
            float height;
            int imageWidth = 0;
            int imageHeight = 0;

            void init(Amara::GameProperties* gProperties, Amara::Scene* gScene, Amara::Entity* gParent) {
                Amara::Actor::init(gProperties, gScene, gParent);
                gRenderer = properties->gRenderer;

                blendMode = SDL_BLENDMODE_BLEND;

                destroyTexture();
            }

            void destroyTexture() {
                if (lightTexture != nullptr) {
                    SDL_DestroyTexture(lightTexture);
                    lightTexture = nullptr;
                }
            }

            SDL_Texture* createTexture() {
                destroyTexture();
                lightTexture = SDL_CreateTexture(
                    properties->gRenderer,
                    SDL_GetWindowPixelFormat(properties->gWindow),
                    SDL_TEXTUREACCESS_TARGET,
                    floor(width),
                    floor(height)
                );
                SDL_QueryTexture(lightTexture, NULL, NULL, &imageWidth, &imageHeight);
            }

            void draw(int vx, int vy, int vw, int vh) {
                if (lightTexture == nullptr || width != vw || height != vh) {
                    width = vw;
                    height = vh;
                    createTexture();
                }
                recTarget = SDL_GetRenderTarget(gRenderer);
                resetPassOnProperties();

                SDL_SetRenderTarget(gRenderer, lightTexture);
                SDL_SetRenderDrawColor(properties->gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);

                Amara::Actor::draw(vx, vy, vw, vh);

                SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
                SDL_SetRenderTarget(gRenderer, recTarget);

                SDL_SetTextureBlendMode(lightTexture, blendMode);
			    SDL_SetTextureAlphaMod(lightTexture, alpha * properties->alpha * 255);
            }
    };
}

#endif