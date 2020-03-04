#ifndef AMARA_INTRECT
#define AMARA_INTRECT

namespace Amara {
    class IntRect {
        public:
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;

            IntRect(int nx, int ny, int nw, int nh) {
                x = nx;
                y = ny;
                width = nw;
                height = nh;
            }
    };
}

#endif