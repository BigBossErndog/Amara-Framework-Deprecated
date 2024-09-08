namespace Amara {
    class Scene;

    class Camera : public Amara::Actor, public Amara::MakeRect {
        public:
            std::vector<Amara::Camera*>* sceneCameras = nullptr;
            Amara::SceneTransitionBase* transition = nullptr;

            bool definedDimensions = false;

            float oldScrollX = 0;
            float oldScrollY = 0;
            float scrollX = 0;
            float scrollY = 0;

            float oldZoomX = 0;
            float oldZoomY = 0;
            float zoomX = 1;
            float zoomY = 1;
            float zoomScale = 1;

            float oldFocusX = 0;
            float oldFocusY = 0;
            float focusX = 0;
            float focusY = 0;

            float centerX = 0;
            float centerY = 0;

            float scrollOffsetX = 0;
            float scrollOffsetY = 0;

            float offsetX = 0;
            float offsetY = 0;

            Amara::Entity* followTarget = nullptr;
            bool followEnabled = true;
            float lerpX = 1;
            float lerpY = 1;

            bool lockedToBounds = false;
            FloatRect bounds = { 0, 0, 0, 0 };

            Camera() {
                definedDimensions = false;
            }

            Camera(float gw, float gh) : Camera() {
                definedDimensions = true;
                setSize(gw, gh);
            }

            Camera(float gx, float gy, int gw, int gh): Camera(gw, gh) {
                x = gx;
                y = gy;
            }

            Camera* setSize(int gw, int gh) {
                width = gw;
                height = gh;
                definedDimensions = true;
                return this;
            }
            Camera* setSize(float gx, float gy, float gw, float gh) {
                x = gx;
                y = gy;
                return setSize(gw, gh);
            }
            Camera* setSize(IntRect rect) {
                return setSize(rect.x, rect.y, rect.width, rect.height);
            }
            Camera* setSize(IntVector2 v) {
                return setSize(v.x, v.y);
            }

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* gParent) {
                properties = gameProperties;
                parent = gParent;
                scene = givenScene;

                rectInit(this);

                if (!definedDimensions) {
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

                updateValues();
                recordValues();
                entityType = "camera";
                
                run();
            }

            void configure(nlohmann::json config) {
                Amara::Actor::configure(config);
                rectConfigure(config);
            }

            virtual void run() {
				if (!definedDimensions) {
                    x = 0;
                    y = 0;

                    if (width != properties->resolution->width || height != properties->resolution->height) {
                        focusOn(focusX, focusY);
                    }

                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

				update();
                updateValues();

                if (followTarget != nullptr && followEnabled) {
                    if (followTarget->isDestroyed || !followTarget->isActive) {
                        stopFollow();
                    }
                    else {
                        float tx = followTarget->x + followTarget->cameraOffsetX;
                        float ty = followTarget->y + followTarget->cameraOffsetY;

                        float nx = (oldFocusX - tx) * (1 - lerpX) + tx;
                        float ny = (oldFocusY - ty) * (1 - lerpY) + ty;
                        focusOn(nx, ny);
                    }
                }

                updateValues();
                recordValues();

                Amara::Actor::run();
			}

            void updateValues() {
                fixValues();
                centerX = width/2.0;
                centerY = height/2.0;
                focusX = scrollX + (width/(zoomX*zoomScale))/2.0;
                focusY = scrollY + (height/(zoomY*zoomScale))/2.0;
            }

            void recordValues() {
                oldScrollX = scrollX;
                oldScrollY = scrollY;

                oldZoomX = zoomX;
                oldZoomY = zoomY;

                oldFocusX = focusX;
                oldFocusY = focusY;
            }

            void fixValues() {
                if (zoomX < 0) zoomX = 0.00001;
                if (zoomY < 0) zoomY = 0.00001;

                if (lockedToBounds) {
                    if (width/(zoomX*zoomScale) > bounds.width) {
                        scrollX = bounds.x - ((width/(zoomX*zoomScale)) - (bounds.width))/2;
                    }
                    else if (scrollX < bounds.x) {
                        scrollX = bounds.x;
                    }
                    else if (scrollX + width/(zoomX*zoomScale) > bounds.x + bounds.width) {
                        scrollX = (bounds.x + bounds.width) - (width/(zoomX*zoomScale));
                    }


                    if (height/(zoomY*zoomScale) > bounds.height) {
                        scrollY = bounds.y - ((height/(zoomY*zoomScale)) - (bounds.height))/2;
                    }
                    else if (scrollY < bounds.y) {
                        scrollY = bounds.y;
                    }
                    else if (scrollY + height/(zoomY*zoomScale) > bounds.y + bounds.height) {
                        scrollY = (bounds.y + bounds.height) - (height/(zoomY*zoomScale));
                    }
                }
            }

            virtual void draw(int vx, int vy, int vw, int vh) override {
                if (!isVisible) return;
                int dx = 0, dy = 0, dw = 0, dh = 0, ow = 0, oh = 0;

                dx = vx + floor(x);
                if (dx < vx) {
                    ow = vx - dx;
                    dx = vx;
                }

                dy = vy + floor(y-z);
                if (dy < vy) {
                    oh = vy - dy;
                    dy = vy;
                }

                dw = (x + width > vw) ? ceil(vw - x) : width;
                dw -= ow;
                dh = (y + height > vh) ? ceil(vh - y) : height;
                dh -= oh;

                checkHover(dx, dy, dw, dh, 0, 0, dw, dh);

                Amara::Entity* entity;
                for (auto it = parent->children.begin(); it != parent->children.end();) {
                    entity = *it;
                    if (entity == nullptr || entity->isDestroyed || entity->scene != scene) {
                        if (properties->inSceneDrawing) it = parent->children.erase(it);
					    else ++it;
                        continue;
                    }
                    if (entity->isVisible) {
                        assignAttributes();
                        entity->draw(dx, dy, dw, dh);
                    }
                    ++it;
                }

                if (transition != nullptr) {
                    transition->draw(dx, dy, dw, dh);
                }
            }

            void assignAttributes() {
                resetPassOnProperties();
                properties->currentCamera = this;
                properties->scrollX = scrollX + scrollOffsetX;
                properties->scrollY = scrollY + scrollOffsetY;
                properties->zoomX = zoomX * zoomScale;
                properties->zoomY = zoomY * zoomScale;
                properties->offsetX = offsetX;
                properties->offsetY = offsetY;
            }

            void startFollow(Amara::Entity* entity, float lx, float ly) {
                followTarget = entity;
                followEnabled = true;
                lerpX = lx;
                lerpY = ly;
            }
            void startFollow(Amara::Entity* entity, float l) {
                startFollow(entity, l, l);
            }
            void startFollow(Amara::Entity* entity) {
                startFollow(entity, 1, 1);
            }

            void stopFollow() {
                followTarget = nullptr;
            }

            void focusOn(float gx, float gy) {
                scrollX = gx - (width/(zoomX*zoomScale))/2;
                scrollY = gy - (height/(zoomY*zoomScale))/2;
                updateValues();
            }
            void focusOn(float gi) {
                focusOn(gi, gi);
            }
            void focusOn(Amara::Entity* entity) {
                focusOn(entity->x + entity->cameraOffsetX, entity->y + entity->cameraOffsetY);
                updateValues();
            }

            void setScroll(float gx, float gy) {
                scrollX = gx;
                scrollY = gy;
                updateValues();
            }
            void setScroll(float gi) {
                setScroll(gi, gi);
            }

            void changeScroll(float gx, float gy) {
                scrollX += gx;
                scrollY += gy;
                updateValues();
            }
            void changeScroll(float gi) {
                changeScroll(gi, gi);
            }

            void setZoomScale(float gZoomScale) {
                zoomScale = gZoomScale;
            }

            void setZoom(float gx, float gy, float gZoomScale) {
                float cx = focusX;
                float cy = focusY;

                setZoomScale(gZoomScale);
                zoomX = gx;
                zoomY = gy;
                focusOn(cx, cy);

                updateValues();
            }

            void setZoom(float gx, float gy) {
                setZoom(gx, gy, zoomScale);
            }
            void setZoom(float gi) {
                setZoom(gi, gi);
            }

            void changeZoom(float gx, float gy) {
                setZoom(zoomX + gx, zoomY + gy);
            }
            void changeZoom(float gi) {
                changeZoom(gi, gi);
            }

            void setBounds(float gx, float gy, float gw, float gh) {
                lockedToBounds = true;
                bounds.x = gx;
                bounds.y = gy;
                bounds.width = gw;
                bounds.height = gh;
            }

            void removeBounds() {
                lockedToBounds = false;
            }

            virtual void bringToFront() {
                if (sceneCameras == nullptr) return;
                std::vector<Amara::Camera*>& rSceneCameras = *sceneCameras;
				for (Amara::Camera* cam: rSceneCameras) {
					if (depth <= cam->depth) {
						depth = cam->depth + 1;
					}
				}
            }

            Amara::Script* scrollTo(float gx, float gy, double tt, Amara::Easing gEasing, bool center) {
				float tx = gx;
				float ty = gy;

				stopFollow();

				if (center) {
					tx -= (width/(zoomX*zoomScale))/2;
					ty -= (height/(zoomY*zoomScale))/2;
				}

				if (lockedToBounds) {
                    if (width/(zoomX*zoomScale) > bounds.width) {
                        tx = bounds.x - ((width/(zoomX*zoomScale)) - (bounds.width))/2;
                    }
                    else if (tx < bounds.x) {
                        tx = bounds.x;
                    }
                    else if (tx + width/(zoomX*zoomScale) > bounds.x + bounds.width) {
                        tx = (bounds.x + bounds.width) - (width/(zoomX*zoomScale));
                    }

                    if (height/(zoomY*zoomScale) > bounds.height) {
                        ty = bounds.y - ((height/(zoomY*zoomScale)) - (bounds.height))/2;
                    }
                    else if (ty < bounds.y) {
                        ty = bounds.y;
                    }
                    else if (ty + height/(zoomY*zoomScale) > bounds.y + bounds.height) {
                        ty = (bounds.y + bounds.height) - (height/(zoomY*zoomScale));
                    }
                }

				if (center) {
					tx += (width/(zoomX*zoomScale))/2;
					ty += (height/(zoomY*zoomScale))/2;
				}

                return recite(createTween_ScrollCamera(tx, ty, tt, gEasing, center));
            }
            Amara::Script* scrollTo(float tx, float ty, double tt, Amara::Easing gEasing) {
                return scrollTo(tx, ty, tt, gEasing, true);
            }
            Amara::Script* scrollTo(float tx, float ty, double tt) {
                return scrollTo(tx, ty, tt, LINEAR);
            }
			Amara::Script* scrollTo(Entity* target, double tt, Amara::Easing gEasing, bool center) {
				return scrollTo(
					target->x + target->cameraOffsetX,
					target->y + target->cameraOffsetY,
					tt, gEasing, center
				);
			}
			Amara::Script* scrollTo(Entity* target, double tt, Amara::Easing gEasing) {
				return scrollTo(target, tt, gEasing, true);
			}
			Amara::Script* scrollTo(Entity* target, double tt) {
				return scrollTo(target, tt, LINEAR);
			}

            Amara::Script* zoomTo(float zt, float tt, Amara::Easing gEasing) {
                return recite(createTween_CameraZoom(zt, tt, gEasing));
            }
            Amara::Script* zoomTo(float zt, float tt) {
                return zoomTo(zt, tt, LINEAR);
            }

            float getXAfterScrolling(float gx) {
                float nzoomX = 1 + (zoomX-1)*zoomFactorX;
                return nzoomX * (gx - scrollX);
            }
            float getYAfterScrolling(float gy) {
                float nzoomY = 1 + (zoomY-1)*zoomFactorY;
                return nzoomY * (gy - scrollY);
            }

            float getXAfterScrolling(Amara::Entity* entity) {
                return getXAfterScrolling(entity->x);
            }
            float getYAfterScrolling(Amara::Entity* entity) {
                return getYAfterScrolling(entity->y);
            }

            bool onCamera(FloatRect rect) {
                FloatRect viewRect = { scrollX, scrollY, width/zoomX, height/zoomY };
                return overlapping(&rect, &viewRect);
            }

            bool onCamera(float gx, float gy, float gw, float gh) {
                return onCamera({ gx, gy, gw, gh });
            }

            bool cutOffCamera(FloatRect rect) {
                if (width/zoomX < rect.width) {
                    if (scrollX < rect.x) return true;
                    if (scrollX + width/zoomX > rect.x + rect.width) return true;
                }
                else {
                    if (scrollX > rect.x) return true;
                    if (scrollX + width/zoomX < rect.x + rect.width) return true;
                }

                if (height/zoomY < rect.height) {
                    if (scrollY < rect.y) return true;
                    if (scrollY + height/zoomY > rect.y + rect.height) return true;
                }
                else {
                    if (scrollY > rect.y) return true;
                    if (scrollY + height/zoomY < rect.y + rect.height) return true;
                }

                return false;
            }

            bool fixCameraOn(FloatRect rect) {
                float recX = scrollX;
                float recY = scrollY;

                if (width/zoomX < rect.width) {
                    if (scrollX < rect.x) scrollX = rect.x;
                    if (scrollX + width/zoomX > rect.x + rect.width) {
                        scrollX = rect.x + rect.width - width/zoomX;
                    }
                }
                else {
                    if (scrollX > rect.x) scrollX = rect.x;
                    if (scrollX + width/zoomX < rect.x + rect.width) {
                        scrollX = rect.x + rect.width - width/zoomX;
                    }
                }

                if (height/zoomY < rect.height) {
                    if (scrollY < rect.y) scrollY = rect.y;
                    if (scrollY + height/zoomY > rect.y + rect.height) {
                        scrollY = rect.y + rect.height - height/zoomY;
                    } 
                }
                else {
                    if (scrollY > rect.y) scrollY = rect.y;
                    if (scrollY + height/zoomY < rect.y + rect.height) {
                        scrollY = rect.y + rect.height - height/zoomY;
                    }
                }

                if (recX != scrollX || recY != scrollY) return true;
                return false;
            }
    };
}
