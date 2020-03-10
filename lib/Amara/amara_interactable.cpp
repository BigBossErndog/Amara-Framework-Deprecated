#ifndef AMARA_INTERACTABLE
#define AMARA_INTERACTABLE

#include "amara.h"

namespace Amara {
    class Interactable {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;

            bool interactable = false;
            bool clicked = false;

            bool hovered = false;
            bool justHovered = false;

            bool draggable = false;
            bool dragged = false;

            virtual void init(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                events = properties->events;
            }

            void setInteractable(bool g) {
                interactable = g;
            }
            void setInteractable() {
                setInteractable(true);
            }

            void setDraggable(bool g) {
                draggable = g;
            }
            void setDraggable() {
                setDraggable(true);
            }

            void checkHover(int bx, int by, int bw, int bh) {
                int mx = properties->input->mouse->x;
                int my = properties->input->mouse->y;

                justHovered = false;
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
                hovered = false;
            }

            virtual void run() {
                Amara::Event* event;
                clicked = false;

                for (size_t i = 0; i < events->eventList.size(); i++) {
                    event = events->eventList.at(i);
                    if (event->disabled) continue;
                    switch (event->type) {
                        case OBJECTLEFTCLICK:
                            if (hovered) {
                                onClick();
                                onPointerDown();
                                event->disabled = true;
                            }
                            break;
                        case OBJECTRIGHTCLICK:
                            if (hovered) {
                                onRightClick();
                                event->disabled = true;
                            }
                            break;
                        case OBJECTMIDDLERELEASE:
                            if (hovered) {
                                onMiddleClick();
                                event->disabled = true;
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