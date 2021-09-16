#pragma once
#ifndef AMARA_FDECLARE_TWEENS
#define AMARA_FDECLARE_TWEENS

#include "amara.h"

namespace Amara {
    class Script;
    enum Easing;

    Amara::Script* createTween_ScrollCamera(float, float, double, Amara::Easing, bool);
    Amara::Script* createTween_CameraZoom(float, double, Amara::Easing);

    SDL_Texture* createRadialGradientTexture(SDL_Renderer*, int, int, SDL_Color, SDL_Color, float);
	SDL_Texture* createCircleTexture(SDL_Renderer*, float, SDL_Color);
}

#endif