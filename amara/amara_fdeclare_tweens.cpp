#ifndef AMARA_FDECLARE_TWEENS
#define AMARA_FDECLARE_TWEENS

#include "amara.h"

namespace Amara {
    Amara::Script* createTween_ScrollCamera(float, float, double, Amara::Easing, bool);
    Amara::Script* createTween_CameraZoom(float, double, Amara::Easing);
}

#endif