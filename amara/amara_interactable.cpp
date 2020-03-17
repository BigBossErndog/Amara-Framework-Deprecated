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
                justClicked = false;
                leftClicked = false;
                rightClicked = false;
                middleClicked = false;

                if (isInteractable) {
                    for (Amara::Event* event : events->eventList) {
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (isHovered) {
                                    onClick();
                                    onPointerDown();
                                    event->disabled = true;
                                    justClicked = true;
                                    leftClicked = true;
                                }
                                break;
                            case OBJECTRIGHTCLICK:
                                if (isHovered) {
                                    onRightClick();
                                    event->disabled = true;
                                    rightClicked = true;
                                }
                                break;
                            case OBJECTMIDDLERELEASE:
                                if (isHovered) {
                                    onMiddleClick();
                                    event->disabled = true;
                                    middleClicked = true;
                                }
                                break;
                            case OBJECTLEFTRELEASE:
                                if (isHovered) {
                                    onRelease();
                                    event->disabled = true;
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