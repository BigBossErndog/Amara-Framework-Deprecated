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

            void copy(IntRect rect) {
                x = rect.x;
                y = rect.y;
                width = rect.width;
                height = rect.height;
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

            bool overlaps(FloatRect rect) {
                bool overlapX = Amara::valueInRange(x, rect.x, rect.x + rect.width) || Amara::valueInRange(rect.x, x, x + width);
                bool overlapY = Amara::valueInRange(y, rect.y, rect.y + rect.height) || Amara::valueInRange(rect.y, y, y + height);
                return overlapX && overlapY;
            }

            void copy(FloatRect rect) {
                x = rect.x;
                y = rect.y;
                width = rect.width;
                height = rect.height;
            }
    };

    int getOffsetX(Amara::Direction dir) {
        switch (dir) {
            case Up:
                return 0;
                break;
            case UpLeft:
                return -1;
                break;
            case UpRight:
                return 1;
                break;
            case Down:
                return 0;
                break;
            case DownLeft:
                return -1;
                break;
            case DownRight:
                return 1;
                break;
            case Left:
                return -1;
                break;
            case Right:
                return 1;
                break;
        }

        return 0;
    }

    int getOffsetY(Amara::Direction dir) {
        switch (dir) {
            case Up:
                return -1;
                break;
            case UpLeft:
                return -1;
                break;
            case UpRight:
                return -1;
                break;
            case Down:
                return 1;
                break;
            case DownLeft:
                return 1;
                break;
            case DownRight:
                return 1;
                break;
            case Left:
                return 0;
                break;
            case Right:
                return 0;
                break;
        }
        return 0;
    }
}

#endif