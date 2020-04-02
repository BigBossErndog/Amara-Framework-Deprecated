#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
    class Interactable {
        private:
            bool recHovered = false;
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::InputManager* input = nullptr;

            bool isInteractable = false;
            bool justClicked = false;
            bool leftClicked = false;
            bool rightClicked = false;
            bool middleClicked = false;

            bool justReleased = false;
            bool leftReleased = false;

            bool isHovered = false;
            bool justHovered = false;

            bool isDraggable = false;
            bool dragged = false;

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
                int mx = properties->input->mouse->dx;
                int my = properties->input->mouse->dy;

                if (!isInteractable) {
                    return;
                }

                if (mx > bx && my > by) {
                    if (mx < bx + bw && my < by + bh) {
                        if (!recHovered) {
                            recHovered = true;
                            justHovered = true;
                            onHover();
                        }
                        isHovered = true;
                        return;
                    }
                }
            }

            virtual void run() {
                if (isInteractable) {
                    justClicked = false;
                    leftClicked = false;
                    rightClicked = false;
                    middleClicked = false;

                    justReleased = false;
                    leftReleased = false;

                    for (Amara::Event* event : events->eventList) {
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (isHovered) {
                                    onClick();
                                    onPointerDown();
                                    if (event->taken) {
                                        event->taken->justClicked = false;
                                        event->taken->leftClicked = false;
                                    }
                                    event->taken = this;

                                    justClicked = true;
                                    leftClicked = true;
                                }
                                break;
                            case OBJECTRIGHTCLICK:
                                if (isHovered) {
                                    onRightClick();
                                    if (event->taken) {
                                        event->taken->justClicked = false;
                                        event->taken->rightClicked = false;
                                    }
                                    event->taken = this;

                                    justClicked = true;
                                    rightClicked = true;
                                }
                                break;
                            case OBJECTMIDDLERELEASE:
                                if (isHovered) {
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
                                if (isHovered) {
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
                        }
                    }
                }
                
                recHovered = isHovered;
                isHovered = false;
                justHovered = false;
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