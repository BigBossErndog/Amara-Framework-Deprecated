#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
    class Interactable {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::InputManager* input = nullptr;

            bool isInteractable = false;
            bool clicked = false;
            bool leftClicked = false;
            bool rightClicked = false;
            bool middleClicked = false;

            bool hovered = false;
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
                int mx = properties->input->mouse->x;
                int my = properties->input->mouse->y;

                justHovered = false;
                hovered = false;

                if (!isInteractable) {
                    return;
                }

                if (mx > bx && my > by) {
                    if (mx < bx + bw && my < by + bh) {
                        if (!hovered) {
                            justHovered = true;
                            onHover();
                        }
                        hovered = true;
                        return;
                    }
                }
            }

            virtual void run() {
                Amara::Event* event;
                clicked = false;
                leftClicked = false;
                rightClicked = false;
                middleClicked = false;

                if (isInteractable) {
                    for (size_t i = 0; i < events->eventList.size(); i++) {
                        event = events->eventList.at(i);
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (hovered) {
                                    onClick();
                                    onPointerDown();
                                    event->disabled = true;
                                    clicked = true;
                                    leftClicked = true;
                                }
                                break;
                            case OBJECTRIGHTCLICK:
                                if (hovered) {
                                    onRightClick();
                                    event->disabled = true;
                                    rightClicked = true;
                                }
                                break;
                            case OBJECTMIDDLERELEASE:
                                if (hovered) {
                                    onMiddleClick();
                                    event->disabled = true;
                                    middleClicked = true;
                                }
                                break;
                            case OBJECTLEFTRELEASE:
                                if (hovered) {
                                    onRelease();
                                    event->disabled = true;
                                }
                                break;
                        }
                    }
                }
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