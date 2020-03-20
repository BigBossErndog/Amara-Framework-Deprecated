#ifndef AMARA_INTRECT
#define AMARA_INTRECT

#include "amara.h"

namespace Amara {
    class IntVector2 {
        public:
            int x = 0;
            int y = 0;

            IntVector2() {}

            IntVector2(int nx, int ny) {
                x = nx;
                y = ny;
            } 
    };

    class FloatVector2 {
        public:
            float x = 0;
            float y = 0;

            FloatVector2() {}

            FloatVector2(float nx, float ny) {
                x = nx;
                y = ny;
            } 
    };

    class IntRect {
        public:
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;

            IntRect() {}

            IntRect(int nx, int ny, int nw, int nh) {
                x = nx;
                y = ny;
                width = nw;
                height = nh;
            }

            bool overlaps(IntRect rect) {
                bool overlapX = Amara::valueInRange(x, rect.x, rect.x + rect.width) || Amara::valueInRange(rect.x, x, x + width);
                bool overlapY = Amara::valueInRange(y, rect.y, rect.y + rect.height) || Amara::valueInRange(rect.y, y, y + height);
                return overlapX && overlapY;
            }
    };

    class FloatRect {
        public:
            float x = 0;
            float y = 0;
            float width = 0;
            float height = 0;

            FloatRect() {}

            FloatRect(float nx, float ny, float nw, float nh) {
                x = nx;
                y = ny;
                width = nw;
                height = nh;
            }
    };
}

#endif