namespace Amara {
    class Key {
        public:
            Amara::Keycode keyCode;

            bool isDown = false;
            bool justDown = false;
            bool tapped = false;
			bool justUp = false;
			bool held = false;
            bool activated = false;

            int downTime = 0;
			int tapTime = 15;

            Key() {
                downTime = 0;
            }

            Key(SDL_Keycode givenKeyCode): Key() {
                keyCode = givenKeyCode;
            }

			void reset() {
				isDown = false;
				justDown = false;
				tapped = false;
				justUp = false;
				held = false;
				activated = false;
				downTime = 0;
			}

            void press() {
				if (!isDown) {
					downTime = 0;
					justDown = true;
					held = false;
				}
				isDown = true;
				activated = true;
			}

			void release() {
				if (isDown) {
					justUp = true;
					if (downTime < tapTime) {
						tapped = true;
					}
				}
				isDown = false;
				activated = false;
				held = false;
			}

            virtual void manage() {
                justUp = false;
				justDown = false;
				tapped = false;
				if (isDown) {
					downTime += 1;
					if (downTime > tapTime) {
						held = true;
					}
				}
                activated = false;
            }
    };
}
