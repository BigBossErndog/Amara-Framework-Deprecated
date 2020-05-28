#pragma once
#ifndef AMARA_LIGHTING
#define AMARA_LIGHTING

#include "amara.h"

namespace Amara {
    class Light {
        public:
            int x = 0;
            int y = 0;
            int z = 0;
            int width;
            int height;
            SDL_Color innerColor;
            SDL_Color outerColor;
            float fadeStart = 0;

            float originX = 0.5;
            float originY = 0.5;

            int offsetX = 0;
            int offsetY = 0;

            float zoomFactorX = 1;
            float zoomFactorY = 1;
            float scrollFactorX = 1;
            float scrollFactorY = 1;

            float scaleX = 1;
            float scaleY = 1;

            SDL_Rect destRect;

            bool destroyed = false;

            Amara::Entity* attachedTo = nullptr;

            Light(int gx, int gy, int gWidth, int gHeight, SDL_Color gInnerColor, SDL_Color gOuterColor, float gFadeStart) {
                x = gx;
                y = gy;
                width = gWidth;
                height = gHeight;
                innerColor = gInnerColor;
                outerColor = gOuterColor;
                fadeStart = gFadeStart;
            }

            Light(int gx, int gy, int gWidth, int gHeight, SDL_Color gInnerColor, SDL_Color gOuterColor) : Light(gx, gy, gWidth, gHeight, gInnerColor, gOuterColor, 0) {}

            void attachTo(Amara::Entity* entity) {
                attachedTo = entity;
                if (entity == nullptr) return;
                x = attachedTo->x;
                y = attachedTo->y;
            }
            void dettach() {
                attachedTo = nullptr;
            }

            void draw(Amara::GameProperties* properties, SDL_Renderer* gRenderer, int vx, int vy, int vw, int vh) {
                bool skipDrawing = false;

                float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
                float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;
                
                destRect.x = floor((x + offsetX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
                destRect.y = floor((y-z + offsetY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
                destRect.w = ceil((width * scaleX) * nzoomX);
                destRect.h = ceil((height * scaleY) * nzoomY);

                if (destRect.x + destRect.w <= 0) skipDrawing = true;
                if (destRect.y + destRect.h <= 0) skipDrawing = true;
                if (destRect.x >= vw) skipDrawing = true;
                if (destRect.y >= vh) skipDrawing = true;
                if (destRect.w <= 0) skipDrawing = true;
                if (destRect.h <= 0) skipDrawing = true;
                
                if (!skipDrawing) {
                    Amara::drawRadialGradient(
                        gRenderer,
                        destRect.x, destRect.y, destRect.w, destRect.h,
                        innerColor, outerColor,
                        fadeStart
                    );
                }
            }

            virtual void update() {
                if (attachedTo != nullptr) {
                    if (attachedTo->isDestroyed) {
                        attachedTo = nullptr;
                    }
                    else {
                        x = attachedTo->x;
                        y = attachedTo->y;
                    }
                }
            }

            void setOrigin(float gx, float gy) {
                originX = gx;
                originY = gy;
            }
            void setOrigin(float gi) {
                setOrigin(gi, gi);
            }

            void destroy() {
                destroyed = true;
            }
    };

    class LightLayer: public Amara::Actor {
        public:
            SDL_Renderer* gRenderer = nullptr;

            SDL_Texture* lightTexture = nullptr;
            SDL_Color fillColor;
            SDL_Color recColor;
            SDL_Rect destRect;
            SDL_Rect viewport;

            std::vector<Amara::Light*> lights;

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
                destroyLights();

                destroyTexture();
            }

            Amara::Light* add(Amara::Light* light) {
                lights.push_back(light);
                return light;
            }

            void destroyTexture() {
                if (lightTexture != nullptr) {
                    SDL_DestroyTexture(lightTexture);
                    lightTexture = nullptr;
                }
            }

            void destroyLights() {
                for (Amara::Light* light: lights) {
                    delete light;
                }
                lights.clear();
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

            void run() {
                Amara::Light* light;
                for (auto it = lights.begin(); it != lights.end(); it++) {
                    light = *it;
                    if (light->destroyed) {
                        lights.erase(it--);
                        delete light;
                    }
                    else {
                        light->update();
                    }
                }

                
                Amara::Actor::run();
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

                for (Amara::Light* light: lights) {
                    light->draw(properties, gRenderer, 0, 0, vw, vh);
                }

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
                destroyLights();
            }
    };
}

#endif