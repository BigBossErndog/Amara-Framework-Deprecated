#pragma once
#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
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
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTRIGHTCLICK:
								if (interact.mouseHover.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTMIDDLECLICK:
								if (interact.mouseHover.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTLEFTRELEASE:
                                if (interact.mouseLeft.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
							case OBJECTRIGHTRELEASE:
                                if (interact.mouseRight.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
							case OBJECTMIDDLERELEASE:
                                if (interact.mouseMiddle.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTTOUCHDOWN:
								if (interact.touchHover.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTTOUCHUP:
								if (interact.touch.isDown) {
									//events->recordObject(event->type, &interact);
									interact.executeEvent(event->type);
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