#pragma once
#ifndef AMARA_TEXTUREGENERATION
#define AMARA_TEXTUREGENERATION

#include "amara.h"

namespace Amara {
    void drawRadialGradient(SDL_Renderer* gRenderer, int gx, int gy, int width, int height, SDL_Color innerColor, SDL_Color outerColor, float fadeStart) {
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

        SDL_Color drawColor;
        int smallSide = (width < height) ? width : height;
        int smallSideWhich = (width < height) ? 0 : 1;
        
        float s, xDist, yDist, radius, progress;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                switch (smallSideWhich) {
                    case 0:
                        s = (y/(float)height) * width;
                        xDist = abs((width/2.0) - x);
                        yDist = abs((height/2.0) - s);
                        break;
                    case 1:
                        s = (x/(float)width) * height;
                        xDist = abs((width/2.0) - s);
                        yDist = abs((height/2.0) - y);
                        break;
                }
                radius = sqrt(xDist*xDist + yDist*yDist);
                progress = radius/(smallSide/2.0);
                progress = (progress-fadeStart)/(1-fadeStart);
                if (progress > 0) {
                    if (progress > 1) progress = 1;
                }
                else {
                    progress = 0;
                }
                drawColor.r = innerColor.r + (progress * (outerColor.r - innerColor.r));
                drawColor.g = innerColor.g + (progress * (outerColor.g - innerColor.g));
                drawColor.b = innerColor.b + (progress * (outerColor.b - innerColor.b));
                drawColor.a = innerColor.a + (progress * (outerColor.a - innerColor.a));

                SDL_SetRenderDrawColor(gRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
                SDL_RenderDrawPoint(gRenderer, gx + x, gy + y);
            }
        }
    }
    void drawRadialGradient(Amara::Entity* entity, int gx, int gy, int width, int height, SDL_Color innerColor, SDL_Color outerColor, float fadeStart) {
        drawRadialGradient(entity->properties->gRenderer, gx, gy, width, height, innerColor, outerColor, fadeStart);
    }

    SDL_Texture* createRadialGradient(SDL_Renderer* gRenderer, int width, int height, SDL_Color innerColor, SDL_Color outerColor, float fadeStart) {
        SDL_Texture* texture = SDL_CreateTexture(
                                    gRenderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    width,
                                    height
                                );

        SDL_SetRenderTarget(gRenderer, texture);
        drawRadialGradient(gRenderer, 0, 0, width, height, innerColor, outerColor, fadeStart);

        return texture;
    }

    SDL_Texture* createRadialGradient(Amara::Entity* entity, int width, int height, SDL_Color innerColor, SDL_Color outerColor, float fadeStart) {
        return createRadialGradient(entity->properties->gRenderer, width, height, innerColor, outerColor, fadeStart);
    }
}

#endif