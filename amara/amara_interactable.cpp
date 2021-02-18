#pragma once
#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
    class Interactable {
        private:
            bool recHovered = false;
            bool recMouseHovered = false;
            bool recTouchHovered = false;
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::InputManager* input = nullptr;

            bool isMouseDown = false;
            bool isTouchDown = false;

            bool isInteractable = false;
            bool justClicked = false;
            bool leftClicked = false;
            bool rightClicked = false;
            bool middleClicked = false;

            bool justReleased = false;
            bool leftReleased = false;

            bool justTouchDown = false;
            bool justTouchUp = false;

            bool isHovered = false;
            bool justHovered = false;

            bool isMouseHovered = false;
            bool isTouchHovered = false;

            bool justMouseHovered = false;
            bool justTouchHovered = false;

            bool justTouchTapped = false;

            bool isDraggable = false;
            bool dragged = false;

            bool checkTouchDown = false;

            int downTime = 0;
            int tapTime = 12;

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

            void checkForHover(int bx, int by, int bw, int bh) {
                if (!isInteractable) {
                    return;
                }

                Amara::Mouse* mouse = properties->input->mouse;
                int mx = mouse->dx;
                int my = mouse->dy;
                if (mx > bx && my > by) {
                    if (mx < bx + bw && my < by + bh) {
                        if (!recHovered) {
                            recHovered = true;
                            justHovered = true;
                            onHover();
                        }
                        if (!recMouseHovered) {
                            recMouseHovered = true;
                            justMouseHovered = true;
                        }
                        if (isMouseDown) {
                            downTime += 1;
                        }
                        isHovered = true;
                        isMouseHovered = true;
                    }
                }
                if (!isMouseHovered) {
                    isMouseDown = false;
                }
                
                std::vector<Amara::TouchPointer*> fingers = input->touches->pointers;
                for (Amara::TouchPointer* finger: fingers) {
                    if (!finger->inUse) continue;
                    int fx = finger->dx;
                    int fy = finger->dy;
                    if (fx > bx && fy > by) {
                        if (fx < bx + bw && fy < by + bh) {
                            if (!recHovered) {
                                recHovered = true;
                                justHovered = true;
                                onHover();
                            }
                            if (!recTouchHovered) {
                                recTouchHovered = true;
                                justTouchHovered = true;
                            }
                            isHovered = true;
                            isTouchHovered = true;
                            
                            downTime += 1;
                            if (checkTouchDown) {
                                justTouchDown = true;
                                downTime = 0;
                                isTouchDown = true;
                            }
                        }
                    }
                }
                if (!isTouchHovered) {
                    isTouchDown = false;
                }

                checkTouchDown = false;
            }

            virtual void run() {
                if (isInteractable) {
                    justClicked = false;
                    leftClicked = false;
                    rightClicked = false;
                    middleClicked = false;

                    justReleased = false;
                    leftReleased = false;

                    justTouchDown = false;
                    justTouchUp = false;

                    justTouchTapped = false;

                    for (Amara::Event* event : events->eventList) {
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (isMouseHovered) {
                                    onClick();
                                    onPointerDown();
                                    if (event->taken) {
                                        event->taken->justClicked = false;
                                        event->taken->leftClicked = false;
                                    }
                                    event->taken = this;

                                    justClicked = true;
                                    leftClicked = true;

                                    downTime = 0;
                                    isMouseDown = true;
                                }
                                break;
                            case OBJECTRIGHTCLICK:
                                if (isMouseHovered) {
                                    onRightClick();
                                    if (event->taken) {
                                        event->taken->justClicked = false;
                                        event->taken->rightClicked = false;
                                    }
                                    event->taken = this;

                                    justClicked = true;
                                    rightClicked = true;

                                    downTime = 0;
                                }
                                break;
                            case OBJECTMIDDLERELEASE:
                                if (isMouseHovered) {
                                    onMiddleClick();
                                    if (event->taken) {
                                        event->taken->justClicked = false;
                                        event->taken->middleClicked = false;
                                    }
                                    event->taken = this;

                                    justClicked = true;
                                    middleClicked = true;
                                }
                                break;
                            case OBJECTLEFTRELEASE:
                                if (isMouseHovered) {
                                    onRelease();
                                    if (event->taken) {
                                        event->taken->justReleased = false;
                                        event->taken->leftReleased = false;
                                    }
                                    event->taken = this;

                                    justReleased = true;
                                    leftReleased = true;
                                }
                                break;
                            case OBJECTTOUCHDOWN:
                                checkTouchDown = true;
                                break;
                            case OBJECTTOUCHUP:
                                if (isTouchHovered) {
                                    justTouchUp = true;
                                    isTouchDown = false;
                                    if (downTime < tapTime) {
                                        justTouchTapped = true;
                                    }
                                    isTouchHovered = false;
                                }
                                break;
                        }
                    }
                }
                
                recHovered = isHovered;
                recMouseHovered = isMouseHovered;
                recTouchHovered = isTouchHovered;
                
                isHovered = false;
                justHovered = false;

                isMouseHovered = false;
                justMouseHovered = false;

                isTouchHovered = false;
                justTouchHovered = false;
            }

            virtual void onHover() {}
            virtual void onClick() {}
            virtual void onLeftClick() {}
            virtual void onRightClick() {}
            virtual void onMiddleClick() {}
            virtual void onPointerDown() {}
            virtual void onRelease() {}
    };
}

#endif