namespace Amara {
    class Scene;

    class Camera : public Amara::Actor {
        public:
            std::list<Amara::Camera*>* sceneCameras = nullptr;
            Amara::SceneTransitionBase* transition = nullptr;

            bool definedDimensions = false;

            float width = 0;
            float height = 0;

            float oldScrollX = 0;
            float oldScrollY = 0;
            float scrollX = 0;
            float scrollY = 0;

            float oldZoomX = 0;
            float oldZoomY = 0;
            float zoomX = 1;
            float zoomY = 1;
            float zoomScale = 1;

            float oldCenterX = 0;
            float oldCenterY = 0;
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
            float boundX = 0;
            float boundY = 0;
            float boundW = 0;
            float boundH = 0;

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

                updateValues();
                recordValues();

                if (!definedDimensions) {
                    width = properties->resolution->width;
                    height = properties->resolution->height;
                }

                recordValues();
                entityType = "camera";
                
                run();
            }

            virtual void run() {
				if (!definedDimensions) {
                    x = 0;
                    y = 0;

                    if (width != properties->resolution->width || height != properties->resolution->height) {
                        centerOn(centerX, centerY);
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

                        float nx = (oldCenterX - tx) * (1 - lerpX) + tx;
                        float ny = (oldCenterY - ty) * (1 - lerpY) + ty;
                        centerOn(nx, ny);
                    }
                }

                updateValues();
                recordValues();

                Amara::Actor::run();
			}

            void updateValues() {
                fixValues();
                centerX = scrollX + (width/(zoomX*zoomScale))/2;
                centerY = scrollY + (height/(zoomY*zoomScale))/2;
            }

            void recordValues() {
                oldScrollX = scrollX;
                oldScrollY = scrollY;

                oldZoomX = zoomX;
                oldZoomY = zoomY;

                oldCenterX = centerX;
                oldCenterY = centerY;
            }

            void fixValues() {
                if (zoomX < 0) zoomX = 0.00001;
                if (zoomY < 0) zoomY = 0.00001;

                if (lockedToBounds) {
                    if (width/(zoomX*zoomScale) > boundW) {
                        scrollX = boundX - ((width/(zoomX*zoomScale)) - (boundW))/2;
                    }
                    else if (scrollX < boundX) {
                        scrollX = boundX;
                    }
                    else if (scrollX + width/(zoomX*zoomScale) > boundX + boundW) {
                        scrollX = (boundX + boundW) - (width/(zoomX*zoomScale));
                    }


                    if (height/(zoomY*zoomScale) > boundH) {
                        scrollY = boundY - ((height/(zoomY*zoomScale)) - (boundH))/2;
                    }
                    else if (scrollY < boundY) {
                        scrollY = boundY;
                    }
                    else if (scrollY + height/(zoomY*zoomScale) > boundY + boundH) {
                        scrollY = (boundY + boundH) - (height/(zoomY*zoomScale));
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
                        ++it;
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

            void centerOn(Amara::Entity* entity) {
                scrollX = entity->x - (width/(zoomX*zoomScale))/2;
                scrollY = entity->y - (height/(zoomY*zoomScale))/2;
                updateValues();
            }
            void centerOn(float gx, float gy) {
                scrollX = gx - (width/(zoomX*zoomScale))/2;
                scrollY = gy - (height/(zoomY*zoomScale))/2;
                updateValues();
            }
            void centerOn(float gi) {
                centerOn(gi, gi);
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
                float cx = centerX;
                float cy = centerY;

                setZoomScale(gZoomScale);
                zoomX = gx;
                zoomY = gy;
                centerOn(cx, cy);

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
                boundX = gx;
                boundY = gy;
                boundW = gw;
                boundH = gh;
            }

            void removeBounds() {
                lockedToBounds = false;
            }

            virtual void bringToFront() {
                if (sceneCameras == nullptr) return;
                std::list<Amara::Camera*>& rSceneCameras = *sceneCameras;
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
                    if (width/(zoomX*zoomScale) > boundW) {
                        tx = boundX - ((width/(zoomX*zoomScale)) - (boundW))/2;
                    }
                    else if (tx < boundX) {
                        tx = boundX;
                    }
                    else if (tx + width/(zoomX*zoomScale) > boundX + boundW) {
                        tx = (boundX + boundW) - (width/(zoomX*zoomScale));
                    }

                    if (height/(zoomY*zoomScale) > boundH) {
                        ty = boundY - ((height/(zoomY*zoomScale)) - (boundH))/2;
                    }
                    else if (ty < boundY) {
                        ty = boundY;
                    }
                    else if (ty + height/(zoomY*zoomScale) > boundY + boundH) {
                        ty = (boundY + boundH) - (height/(zoomY*zoomScale));
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

            ~Camera() {}
    };
}
