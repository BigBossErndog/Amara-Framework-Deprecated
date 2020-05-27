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
            SDL_Rect destRect;
            SDL_Rect viewport;

            SDL_BlendMode blendMode = SDL_BLENDMODE_MOD;

            SDL_Texture* recTarget = nullptr;

            float width;
            float height;
            int imageWidth = 0;
            int imageHeight = 0;

            LightLayer(int r, int g, int b, int a) {
                fillColor = {r, g, b, a};
            }

            LightLayer(int r, int g, int b): LightLayer(r, g, b, 255) {}

            LightLayer(SDL_Color gColor) {
                fillColor = gColor;
            }

            void init(Amara::GameProperties* gProperties, Amara::Scene* gScene, Amara::Entity* gParent) {
                Amara::Actor::init(gProperties, gScene, gParent);
                gRenderer = properties->gRenderer;

                blendMode = SDL_BLENDMODE_MOD;

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

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);
                
                destRect.x = 0;
                destRect.y = 0;
                destRect.w = vw;
                destRect.h = vh;

                SDL_GetRenderDrawColor(gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);
                recTarget = SDL_GetRenderTarget(gRenderer);

                SDL_SetRenderTarget(gRenderer, lightTexture);
                SDL_SetRenderDrawColor(gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
                SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_NONE);
                SDL_RenderFillRect(gRenderer, NULL);

                Amara::Actor::draw(0, 0, vw, vh);

                SDL_SetRenderDrawColor(gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
                SDL_SetRenderTarget(gRenderer, recTarget);

                if (lightTexture != nullptr) {
                    SDL_SetTextureBlendMode(lightTexture, blendMode);
                    SDL_SetTextureAlphaMod(lightTexture, 255);
                    SDL_RenderCopyEx(
                        properties->gRenderer,
                        lightTexture,
                        NULL,
                        &destRect,
                        NULL,
                        NULL,
                        SDL_FLIP_NONE
                    );
                }
            }

            ~LightLayer() {
                SDL_DestroyTexture(lightTexture);
            }
    };
}

#endif