#pragma once
#ifndef AMARA_INTERACTIONMANAGER
#define AMARA_INTERACTIONMANAGER



namespace Amara {
	class InteractionManager: public Amara::Key {
		public:
			Amara::Key mouseLeft;
			Amara::Key mouseRight;
			Amara::Key mouseMiddle;

			Amara::Key touch;

			Amara::Key mouseHover;
			Amara::Key touchHover;

			bool isHovered = false;

			float pointX = 0;
			float pointY = 0;
			float movementX = 0;
			float movementY = 0;
			float moveDistance = 0;

			bool moved = false;

			float interactScaleX = 1;
			float interactScaleY = 1;

			Amara::Mouse* mouse = nullptr;
			Amara::TouchPointer* finger = nullptr;

			void preManage() {
				mouseLeft.manage();
				mouseRight.manage();
				mouseMiddle.manage();

				mouseHover.manage();
				touchHover.manage();

				touch.manage();
				Amara::Key::manage();
			}
			void postManage() {
				if (mouseLeft.isDown || touch.isDown) {
					press();
				}
				else {
					release();
				}
				tapped = mouseLeft.tapped || touch.tapped;
				isHovered = mouseHover.isDown || touchHover.isDown;

				if (isDown) {
					if (finger) {
						setInteractPoint(finger->x, finger->y);
					}
					else if (mouse) {
						setInteractPoint(mouse->x, mouse->y);
					}
				}
				else {
					movementX = 0;
					movementY = 0;
					moved = false;
				}
			}

			void setInteractPoint(float gx, float gy) {
				if (justDown) {
					movementX = 0;
					movementY = 0;
					moved = false;
					moveDistance = 0;
				}
				else {
					movementX = (gx - pointX) / interactScaleX;
					movementY = (gy - pointY) / interactScaleY;
					moveDistance = distanceBetween(0, 0, movementX, movementY);
				}
				pointX = gx;
				pointY = gy;
				if (movementX != 0 || movementY != 0) {
					moved = true;
				}
			}

			void executeEvent(EventType type) {
				switch (type) {
					case OBJECTLEFTCLICK:
						mouseLeft.press();
						break;
					case OBJECTRIGHTCLICK:
						mouseRight.press();
						break;
					case OBJECTMIDDLECLICK:
						mouseMiddle.press();
						break;
					case OBJECTLEFTRELEASE:
						mouseLeft.release();
						break;
					case OBJECTRIGHTRELEASE:
						mouseRight.release();
						break;
					case OBJECTMIDDLERELEASE:
						mouseMiddle.release();
						break;
					case OBJECTTOUCHDOWN:
						touch.press();
						break;
					case OBJECTTOUCHUP:
						touch.release();
						break;
				}
			}
	};
}

#endif