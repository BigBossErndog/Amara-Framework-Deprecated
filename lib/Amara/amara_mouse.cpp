#ifndef AMARA_MOUSE
#define AMARA_MOUSE

#include "amara.h"

namespace Amara {

    class MouseButton: public Amara::Key {
        public:
            MouseButton(): Amara::Key() {}
    };

    class Mouse {
        public:
            Amara::GameProperties* properties = nullptr;

            int x = 0;
            int y = 0;

            Amara::MouseButton* left = nullptr;
			Amara::MouseButton* right = nullptr;
            Amara::MouseButton* middle = nullptr;

			Mouse(Amara::GameProperties* gameProperties) {
                properties = gameProperties;

				left = new Amara::MouseButton();
				right = new Amara::MouseButton();
                middle = new Amara::MouseButton();
			}

			void manage() {
				left->manage();
				right->manage();
                middle->manage();
			}
    };
}

#endif