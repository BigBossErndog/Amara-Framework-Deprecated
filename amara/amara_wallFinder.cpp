#pragma once
#ifndef AMARA_WALLFINDER
#define AMARA_WALLFINDER

namespace Amara {
    class WallFinder {
        public:
            virtual bool isWall(int gx, int gy) {
                return false;
            }

            virtual bool isWall(int gx, int gy, Amara::Direction dir) {
                return false;
            }

            virtual int getMapWidth() {
                return 0;
            }

            virtual int getMapHeight() {
                return 0;
            }
    };
}

#endif