#ifndef AMARA_CAMERA
#define AMARA_CAMERA

#include "amara.h"

namespace Amara {
    class Camera : public Amara::Entity {
        public:
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;

            int scrollX = 0;
            int scrollY = 0;

            Camera() {

            }
    };
}

#endif