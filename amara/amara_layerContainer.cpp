#pragma once
#ifndef AMARA_LAYER
#define AMARA_LAYER

#include "amara.h"

namespace Amara {
    class Layer: public Amara::Actor {
    public:
        Layer(): Amara::Actor() {}

        Layer(float gx, float gy): Layer() {
            x = gx;
            y = gy;
        }

        virtual void draw(int vx, int vy, int vw, int vh) {
            Amara::Actor::draw(vx, vy, vw, vh);
        }
    };

    class Container: public Amara::Layer {
    public:
        float width = 0;
        float height = 0;

        float originX = 0;
        float originY = 0;

        Container(): Amara::Layer() {}
        Container(float w, float h): Container() {
            width = w;
            height = h;
        }
        Container(float gx, float gy, float w, float h): Container(w, h) {
            x = gx;
            y = gy;
        }

        virtual void draw(int vx, int vy, int vw, int vh) override {
            int dx = 0, dy = 0, dw = 0, dh = 0, ox = 0, oy = 0;

            float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
            float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY; 
            dx = floor((x - properties->scrollX*scrollFactorX + properties->offsetX - (originX*width*scaleX)) * nzoomX);
            dy = floor((y-z - properties->scrollY*scrollFactorY + properties->offsetY - (originY*height*scaleY)) * nzoomY);
            dw = width * scaleX * nzoomX;
            dh = height * scaleY * nzoomY;

            if (dx < 0) {
                dw += dx;
                ox += dx/(scaleX*nzoomX);
                dx = 0;
            }
            if (dy < 0) {
                dh += dy;
                oy += dy/(scaleY*nzoomY);
                dy = 0;
            }
            if (dx + dw > vw) {
                dw = vw - dx;
            }
            if (dy + dh > vh) {
                dh = vh - dy;
            }

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;
            
            stable_sort(entities.begin(), entities.end(), sortEntities());

            float recZoomX = properties->zoomX;
            float recZoomY = properties->zoomY;
            float recZoomFactorX = properties->zoomFactorX * zoomFactorX;
            float recZoomFactorY = properties->zoomFactorY * zoomFactorY;
            float recAlpha = properties->alpha * alpha;

            for (Amara::Entity* entity : entities) {
                properties->scrollX = 0;
                properties->scrollY = 0;
                properties->offsetX = ox;
                properties->offsetY = oy;
                properties->zoomX = recZoomX * scaleX;
                properties->zoomY = recZoomY * scaleY;
                properties->zoomFactorX = recZoomFactorX;
                properties->zoomFactorY = recZoomFactorY;
                properties->alpha = recAlpha;
                if (entity->isDestroyed || entity->parent != this) continue;
                if (!entity->isVisible) continue;
                entity->draw(vx + dx, vy + dy, dw, dh);
            }
        }

        void setOrigin(float gx, float gy) {
            originX = gx;
            originY = gy;
        }
        void setOrigin(float g) {
            setOrigin(g, g);
        }
    };

    class TextureLayer: public Amara::Layer {
    public:
        SDL_Texture* tx = nullptr;
        int textureWidth;
        int textureHeight;

        SDL_Texture* recTarget;
        SDL_Rect viewport;
        SDL_Rect srcRect;
        SDL_Rect destRect;
        SDL_Point origin = { 0, 0 };

        SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

        TextureLayer(): Layer() {}
        TextureLayer(float gx, float gy): Layer(gx, gy) {}

        virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
            properties = gameProperties;
            createTexture();
            Amara::Layer::init(gameProperties, givenScene, givenParent);
        }

        void createTexture() {
            if (tx) {
                SDL_DestroyTexture(tx);
            }
            tx = SDL_CreateTexture(
                properties->gRenderer,
                SDL_GetWindowPixelFormat(properties->gWindow),
                SDL_TEXTUREACCESS_TARGET,
                properties->resolution->width,
                properties->resolution->height
            );
            textureWidth = properties->resolution->width;
            textureHeight = properties->resolution->height;
        }

        void draw(int vx, int vy, int vw, int vh) {
            if (textureWidth != properties->resolution->width || textureHeight != properties->resolution->height) {
                createTexture();
            }
            if (!tx) return;
            recTarget = SDL_GetRenderTarget(properties->gRenderer);
            SDL_SetRenderTarget(properties->gRenderer, tx);
            SDL_SetRenderDrawColor(properties->gRenderer, 0, 0, 0, 0);
            SDL_RenderClear(properties->gRenderer);
            float recAlpha = properties->alpha;
            
            Amara::Layer::draw(vx, vy, vw, vh);

            SDL_SetRenderTarget(properties->gRenderer, recTarget);

            bool skipDrawing = false;

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;

            viewport.x = 0;
            viewport.y = 0;
            viewport.w = textureWidth;
            viewport.h = textureHeight;
            SDL_RenderSetViewport(properties->gRenderer, &viewport);

            destRect.x = 0;
            destRect.y = 0;
            destRect.w = textureWidth;
            destRect.h = textureHeight;
            
            if (destRect.x + destRect.w <= 0) skipDrawing = true;
            if (destRect.y + destRect.h <= 0) skipDrawing = true;
            if (destRect.x >= properties->resolution->width) skipDrawing = true;
            if (destRect.y >= properties->resolution->height) skipDrawing = true;
            if (destRect.w <= 0) skipDrawing = true;
            if (destRect.h <= 0) skipDrawing = true;

            if (!skipDrawing) {
                if (tx != nullptr) {
                    SDL_SetTextureBlendMode(tx, blendMode);
                    SDL_SetTextureAlphaMod(tx, alpha * recAlpha * 255);

                    SDL_RenderCopyEx(
                        properties->gRenderer,
                        tx,
                        NULL,
                        &destRect,
                        0,
                        &origin,
                        SDL_FLIP_NONE
                    );
                }
            }
        }

        ~TextureLayer() {
            if (tx) {
                SDL_DestroyTexture(tx);
            }
        }
    };
}

#endif