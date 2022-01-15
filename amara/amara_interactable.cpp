#pragma once
#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
	class InteractionManager: public Amara::Key {
		public:
			Amara::Key mouseLeft;
			Amara::Key mouseRight;
			Amara::Key mouseMiddle;

			Amara::Key touch;

			Amara::Key mouseHover;
			Amara::Key touchHover;

			float pointX = 0;
			float pointY = 0;
			float movementX = 0;
			float movementY = 0;

			Amara::Mouse* mouse = nullptr;
			Amara::TouchPointer* finger = nullptr;

			void preManage() {
				mouseLeft.manage();
				mouseRight.manage();
				mouseMiddle.manage();

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
				}
			}

			void setInteractPoint(float gx, float gy) {
				if (justDown) {
					movementX = 0;
					movementY = 0;
				}
				else {
					movementX = gx - pointX;
					movementY = gy - pointY;
				}
				pointX = gx;
				pointY = gy;
			}
	};

    class Interactable {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::InputManager* input = nullptr;

			Amara::InteractionManager interact;
			std::vector<Amara::FloatRect> interactBoxes;

			bool isInteractable = false;
			bool isDraggable = false;
            
            virtual void init(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                events = properties->events;
                input = properties->input;
            }

            void setInteractable(bool g) {
                isInteractable = g;
            }
            void setInteractable() {
                setInteractable(true);
            }

            void setDraggable(bool g) {
                isDraggable = g;
            }
            void setDraggable() {
                setDraggable(true);
            }

			void makeInteractBox(int gx, int gy, int gw, int gh) {
				if (!isInteractable) return;
				interactBoxes.push_back({ gx, gy, gw, gh });
			}
			void makeInteractBox(int vx, int vy, int vw, int vh, int gx, int gy, int gw, int gh) {
				if (!isInteractable) return;
				if (gx > vw || gy > vh) return;
				if (gx + gw < 0 || gy + gw < 0) return;

				if (gx < 0) {
					gw += gx;
					gx = 0;
				}
				if (gy < 0) {
					gh += gy;
					gy = 0;
				}
				if (gx + gw > vw) {
					gw -= (gx + gw) - vw;
				}
				if (gy + gh > vh) {
					gh -= (gy + gh) - vh;
				}

				makeInteractBox(vx + gx, vy + gy, gw, gh);
			}

			bool checkHover() {
				Amara::Mouse* mouse = input->mouse;
				Amara::TouchManager* touches = input->touches;
				std::vector<TouchPointer*>& fingers = touches->pointers;

				bool touchHovered = false;
				bool mouseHovered = false;

				interact.mouse = mouse;
				interact.finger = nullptr;
				
				for (Amara::FloatRect& box: interactBoxes) {
					if (overlapping(mouse->dx, mouse->dy, &box)) {
						mouseHovered = true;
					}
					for (TouchPointer* finger: fingers) {
						if (!finger->isDown) {
							continue;
						}
						if (overlapping(finger->dx, finger->dy, &box)) {
							touchHovered = true;
							interact.finger = finger;
						}
					}
				}
				if (mouseHovered) {
					interact.mouseHover.press();
				}
				else {
					interact.mouseHover.release();
					interact.mouseLeft.release();
					interact.mouseRight.release();
					interact.mouseMiddle.release();
				}
				if (touchHovered) {
					interact.touchHover.press();
				}
				else {
					interact.touchHover.release();
					interact.touch.release();
				}

				interactBoxes.clear();
			}

            virtual void run() {
                if (isInteractable) {
					interact.preManage();
					checkHover();
                    for (Amara::Event* event : events->eventList) {
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (interact.mouseHover.isDown) {
									interact.mouseLeft.press();
								}
                                break;
                            case OBJECTRIGHTCLICK:
								if (interact.mouseHover.isDown) {
									interact.mouseRight.press();
								}
                                break;
                            case OBJECTMIDDLECLICK:
								if (interact.mouseHover.isDown) {
									interact.mouseMiddle.press();
								}
                                break;
                            case OBJECTLEFTRELEASE:
                                if (interact.mouseLeft.isDown) {
									interact.mouseLeft.release();
								}
                                break;
							case OBJECTRIGHTRELEASE:
                                if (interact.mouseRight.isDown) {
									interact.mouseRight.release();
								}
                                break;
							case OBJECTMIDDLERELEASE:
                                if (interact.mouseMiddle.isDown) {
									interact.mouseMiddle.release();
								}
                                break;
                            case OBJECTTOUCHDOWN:
								if (interact.touchHover.isDown) {
									interact.touch.press();
								}
                                break;
                            case OBJECTTOUCHUP:
								if (interact.touch.isDown) {
									interact.touch.release();
								}
                                break;
                        }
                    }
					interact.postManage();
                }
            }
    };
}

#endif