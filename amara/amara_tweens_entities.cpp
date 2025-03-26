namespace Amara {
    class Tween_XY: public Tween {
        public:
            float startX = 0;
            float startY = 0;
            float targetX = 0;
            float targetY = 0;

            Tween_XY(float tx, float ty, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
            }
            Tween_XY(float tx, float ty, double tt): Tween_XY(tx, ty, tt, EASE_LINEAR) {}

            void prepare() {
                startX = parent->x;
                startY = parent->y;
            }

            void script() {
                Amara::Tween::progressFurther();
                switch (easing) {
                    case EASE_LINEAR:
                        parent->x = linearEase(startX, targetX, progress);
                        parent->y = linearEase(startY, targetY, progress);
                        break;
                    case EASE_SINE_INOUT:
                        parent->x = sineInOutEase(startX, targetX, progress);
                        parent->y = sineInOutEase(startY, targetY, progress);
                        break;
                    case EASE_SINE_IN:
                        parent->x = sineInEase(startX, targetX, progress);
                        parent->y = sineInEase(startY, targetY, progress);
                        break;
                    case EASE_SINE_OUT:
                        parent->x = sineOutEase(startX, targetX, progress);
                        parent->y = sineOutEase(startY, targetY, progress);
                        break;
                }
            }
    };

    class Tween_X: public Tween {
    public:
        float startX = 0;
        float targetX = 0;

        Tween_X(float tx, double tt, Amara::Easing gEasing) {
            targetX = tx;
            time = tt;
            easing = gEasing;
        }
        Tween_X(float tx, double tt): Tween_X(tx, tt, EASE_LINEAR) {}

        void prepare() {
            startX = parent->x;
        }

        void script() {
            Amara::Tween::progressFurther();
            parent->x = ease(startX, targetX, progress, easing);
        }
    };

    class Tween_Y: public Tween {
    public:
        float startY = 0;
        float targetY = 0;

        Tween_Y(float ty, double tt, Amara::Easing gEasing) {
            targetY = ty;
            time = tt;
            easing = gEasing;
        }
        Tween_Y(float ty, double tt): Tween_Y(ty, tt, EASE_LINEAR) {}

        void prepare() {
            startY = parent->y;
        }

        void script() {
            Amara::Tween::progressFurther();
            parent->y = ease(startY, targetY, progress, easing);
        }
    };

    class Tween_RelativeXY: public Tween {
        public:
            float startX = 0;
            float startY = 0;
            float targetX = 0;
            float targetY = 0;

            Tween_RelativeXY(float tx, float ty, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
                time = tt;
                easing = gEasing;
            }
            Tween_RelativeXY(float tx, float ty, double tt): Tween_RelativeXY(tx, ty, tt, EASE_LINEAR) {}

            void prepare() {
                startX = parent->x;
                startY = parent->y;
                targetX += parent->x;
                targetY += parent->y;
            }

            void script() {
                Amara::Tween::progressFurther();
                parent->x = ease(startX, targetX, progress, easing);
                parent->y = ease(startY, targetY, progress, easing);
            }
    };

	class Tween_XYZ: public Tween {
        public:
            float startX = 0;
            float startY = 0;
			float startZ = 0;
            float targetX = 0;
            float targetY = 0;
			float targetZ = 0;

            Tween_XYZ(float tx, float ty, float tz, double tt, Amara::Easing gEasing) {
                targetX = tx;
                targetY = ty;
				targetZ = tz;
                time = tt;
                easing = gEasing;
            }
            Tween_XYZ(float tx, float ty, float tz, double tt): Tween_XYZ(tx, ty, tz, tt, EASE_LINEAR) {}

            void prepare() {
                startX = parent->x;
                startY = parent->y;
				startZ = parent->z;
            }

            void script() {
                Amara::Tween::progressFurther();
                parent->x = ease(startX, targetX, progress, easing);
                parent->y = ease(startY, targetY, progress, easing);
                parent->z = ease(startZ, targetZ, progress, easing);
            }
    };

    class Tween_ScaleXY: public Tween {
        public:
            float startScaleX;
            float startScaleY;
            float targetScaleX;
			float targetScaleY;

            Tween_ScaleXY(float tx, float ty, double tt, Amara::Easing gEasing) {
                targetScaleX = tx;
				targetScaleY = ty;
                time = tt;
                easing = gEasing;
            }
			Tween_ScaleXY(float ts, double tt, Amara::Easing gEasing): Tween_ScaleXY(ts, ts, tt, gEasing) {}
            Tween_ScaleXY(float ts, double tt): Tween_ScaleXY(ts, tt, EASE_LINEAR) {}

            void prepare() {
                startScaleX = parent->scaleX;
                startScaleY = parent->scaleY;
            }

            void script() {
                Amara::Tween::progressFurther();
                parent->scaleX = ease(startScaleX, targetScaleX, progress, easing);
                parent->scaleY = ease(startScaleY, targetScaleY, progress, easing);
            }
    };
	
	class Tween_SnapXY: public Script {
	public:
		float snapX;
		float snapY;

		Tween_SnapXY(float gx, float gy) {
			snapX = gx;
			snapY = gy;
		}

		void finish() {
			parent->x = snapX;
			parent->y = snapY;
			Script::finish();
		}

		void script() {
			finish();
		}
	};

    class Tween_Rotate: public Tween {
    public:
        double rotateAmount = 0;
        double targetAngle = 0;
        double startAngle = 0;

        Tween_Rotate(double amount, double tt, Amara::Easing gEasing) {
            rotateAmount = amount;
            time = tt;
            easing = gEasing;
        }
        Tween_Rotate(double amount, double tt): Tween_Rotate(amount, tt, EASE_LINEAR) {}

        void prepare() {
            startAngle = parent->angle;
            targetAngle = parent->angle + rotateAmount;
        }

        void script() {
            Amara::Tween::progressFurther();
            parent->angle = ease(startAngle, targetAngle, progress, easing);
        }

        void finish() {
            parent->angle = fmod(targetAngle, 360);
            while (parent->angle < 0) parent->angle += 360;
            Tween::finish();
        }
    };

    class Tween_ShakeXY: public Tween {
        public:
            float startX;
            float startY;
            float maxShakeX;
            float maxShakeY;
			float targetX;
			float targetY;

			Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float gTargetX, float gTargetY, float tt, Amara::Easing gEasing) {
				maxShakeX = gMaxShakeX;
                maxShakeY = gMaxShakeY;
				targetX = gTargetX;
				targetY = gTargetY;
                time = tt;
                easing = gEasing;
			}
            Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float tt, Amara::Easing gEasing): Tween_ShakeXY(gMaxShakeX, gMaxShakeY, 0, 0, tt, gEasing)  {}
            Tween_ShakeXY(float gMaxShake, float tt, Amara::Easing gEasing): Tween_ShakeXY(gMaxShake, gMaxShake, tt, gEasing) {}
            Tween_ShakeXY(float gMaxShake, float tt): Tween_ShakeXY(gMaxShake, tt, EASE_LINEAR) {}
			Tween_ShakeXY(float gMaxShakeX, float gMaxShakeY, float gTargetX, float gTargetY, float tt): Tween_ShakeXY(gMaxShakeX, gMaxShakeY, gTargetX, gTargetY, tt, EASE_LINEAR) {}

            void prepare() {
                startX = parent->x;
                startY = parent->y;

				if (targetX == -1) targetX = maxShakeX;
				if (targetY == -1) targetY = maxShakeY;
            }

            void finish() {
                Tween::finish();
                parent->x = startX;
                parent->y = startY;
            }

            void script() {
                Amara::Tween::progressFurther();

                float shakeAmountX;
                float shakeAmountY;

                shakeAmountX = ease(maxShakeX, targetX, progress, easing);
                shakeAmountY = ease(maxShakeY, targetY, progress, easing);
                
                parent->x = startX + properties->rng->random()*shakeAmountX - shakeAmountX/2.0;
                parent->y = startY + properties->rng->random()*shakeAmountY - shakeAmountY/2.0;
            }

			void cancel() {
				parent->x = startX;
				parent->y = startY;
			}
    };

	class Tween_ReverseShakeXY: public Tween {
        public:
            float startX;
            float startY;
            float maxShakeX;
            float maxShakeY;

            float targetX = 0;
            float targetY = 0;

            Tween_ReverseShakeXY(float gMaxShakeX, float gMaxShakeY, float tt, Amara::Easing gEasing) {
                maxShakeX = gMaxShakeX;
                maxShakeY = gMaxShakeY;
                time = tt;
                easing = gEasing;
            }
            Tween_ReverseShakeXY(float gMaxShake, float tt, Amara::Easing gEasing): Tween_ReverseShakeXY(gMaxShake, gMaxShake, tt, gEasing) {}
            Tween_ReverseShakeXY(float gMaxShake, float tt): Tween_ReverseShakeXY(gMaxShake, tt, EASE_LINEAR) {}

            void prepare() {
                startX = parent->x;
                startY = parent->y;
            }

            void finish() {
                Tween::finish();
            }

            void script() {
                Amara::Tween::progressFurther();

                float shakeAmountX;
                float shakeAmountY;

                shakeAmountX = ease(maxShakeX, targetX, (1-progress), easing);
                shakeAmountY = ease(maxShakeY, targetY, (1-progress), easing);

                parent->x = startX + properties->rng->random()*shakeAmountX - shakeAmountX/2.0;
                parent->y = startY + properties->rng->random()*shakeAmountY - shakeAmountY/2.0;
            }

			void cancel() {
				parent->x = startX;
				parent->y = startY;
			}
    };

    class Tween_Alpha: public Tween {
        public:
            float startAlpha = -1;
            float targetAlpha = 0;

            Tween_Alpha(float gTarget, float gTime, Amara::Easing gEasing) {
                targetAlpha = gTarget;
                time = gTime;
                easing = gEasing;
            }
            Tween_Alpha(float gTarget, float gTime): Tween_Alpha(gTarget, gTime, EASE_LINEAR) {}
            Tween_Alpha(float gTime): Tween_Alpha(0, gTime) {}


            Tween_Alpha(float gStart, float gTarget, float gTime, Amara::Easing gEasing): Tween_Alpha(gTarget, gTime, gEasing) {
                startAlpha = gStart;
            }
            Tween_Alpha(float gStart, float gTarget, float gTime): Tween_Alpha(gStart, gTarget, gTime, EASE_LINEAR) {}

            void prepare() {
                if (startAlpha == -1) startAlpha = parent->alpha;
            }

            void script() {
                Amara::Tween::progressFurther();
                parent->alpha = ease(startAlpha, targetAlpha, progress, easing);
            }

            void finish() {
                Tween::finish();
                parent->alpha = targetAlpha;
            }
    };

    class Tween_Jump: public Tween {
    public:
        float targetX;
        float targetY;
        float maxZ;

        float startX;
        float startY;
        float startZ;
        
        Tween_Jump(float gx, float gy, float gz, float gt) {
            targetX = gx;
            targetY = gy;
            maxZ = gz;
            time = gt;
        }

        void prepare() {
            startX = parent->x;
            startY = parent->y;
            startZ = parent->z;
        }

        void script() {
            Tween::progressFurther();
            parent->x = linearEase(startX, targetX, progress);
            parent->y = linearEase(startY, targetY, progress);
            parent->z = circularEase(startZ, maxZ, progress);
        }
    };

    class Script_SetVisible: public Script {
    public:
        bool toSet = true;
        bool justToggle = false;

        Script_SetVisible() {
            justToggle = true;
        }
        Script_SetVisible(bool v) {
            toSet = v;
        }

        void prepare() {
            if (justToggle) toSet = !parent->isVisible;
        }

        void script() {
            finish();
        }

        void finish() {
            parent->setVisible(toSet);
            Script::finish();
        }
    };

    class Tween_Depth: public Tween {
    public:
        float startDepth = 0;
        float targetDepth = 0;

        Tween_Depth(float gTarget, float gTime, Amara::Easing gEasing) {
            targetDepth = gTarget;
            time = gTime;
            easing = gEasing;
        }
        Tween_Depth(float gTarget, float gTime): Tween_Depth(gTarget, gTime, EASE_LINEAR) {}
        Tween_Depth(float gTime): Tween_Depth(-1, gTime) {}
        Tween_Depth(): Tween_Depth(0) {}

        void prepare() {
            startDepth = parent->depth;
            if (targetDepth = -1) {
                Amara::Entity* entity;
                for (auto it = parent->children.begin(); it != parent->children.end();) {
                    entity = *it;
                    if (entity->depth > targetDepth) {
                        targetDepth = entity->depth + 0.1;
                    }
                }
            }
        }

        void script() {
            Amara::Tween::progressFurther();
            parent->depth = ease(startDepth, targetDepth, progress, easing);
        }

        void finish() {
            Tween::finish();
            parent->depth = targetDepth;
        }
    };

    class Tween_FillRectWH: public Tween {
    public:
        FillRect* rect = nullptr;

        float targetW = 0;
        float targetH = 0;
        float startW = 0;
        float startH = 0;

        Tween_FillRectWH(float tw, float th, float tt, Amara::Easing gEasing) {
            targetW = tw;
            targetH = th;
            time = tt;
            easing = gEasing;
        }
        Tween_FillRectWH(float tw, float th, float tt): Tween_FillRectWH(tw, th, tt, EASE_LINEAR) {}

        void prepare() {
            if (rect == nullptr) rect = (FillRect*)parent;
            startW = rect->width;
            startH = rect->height;
        }

        void script() {
            Amara::Tween::progressFurther();
            rect->width = ease(startW, targetW, progress, easing);
            rect->height = ease(startH, targetH, progress, easing);
        }

        void finish() {
            Tween::finish();
            rect->width = targetW;
            rect->height = targetH;
        }
    };

    class Tween_WH: public Tween {
    public:
        MakeRect* rect = nullptr;

        float targetW = 0;
        float targetH = 0;
        float startW = 0;
        float startH = 0;

        Tween_WH(MakeRect* gRect) {
            rect = gRect;
        }

        Tween_WH(MakeRect* gRect, float tw, float th, float tt, Amara::Easing gEasing): Tween_WH(gRect) {
            targetW = tw;
            targetH = th;
            time = tt;
            easing = gEasing;
        }
        Tween_WH(MakeRect* gRect, float tw, float th, float tt): Tween_WH(gRect, tw, th, tt, EASE_LINEAR) {}

        Tween_WH(float tw, float th, float tt, Amara::Easing gEasing): Tween_WH(nullptr, tw, th, tt, gEasing) {}
        Tween_WH(float tw, float th, float tt): Tween_WH(nullptr, tw, th, tt) {}

        void prepare() {
            if (rect == nullptr) rect = (MakeRect*)parent;
            startW = rect->width;
            startH = rect->height;
        }

        void script() {
            Amara::Tween::progressFurther();
            rect->width = ease(startW, targetW, progress, easing);
            rect->height = ease(startH, targetH, progress, easing);
        }

        void finish() {
            Tween::finish();
            rect->width = targetW;
            rect->height = targetH;
        }
    };

    class Tween_WaitForActor: public Amara::Script {
    public:
        Amara::Actor* waitFor = nullptr;

        Tween_WaitForActor(Amara::Actor* gWaitFor) {
            waitFor = gWaitFor;
        }

        void script() {
            if (waitFor->isDestroyed || waitFor->notActing()) {
                finish();
            }
        }
    };

    class Script_TimedBroadcast: public Amara::Script {
    public:
        std::string messageKey;
        nlohmann::json messageData = nullptr;

        float time = 0;

        Script_TimedBroadcast(float gTime, std::string gKey) {
            time = gTime;
            messageKey = gKey;
        }
        Script_TimedBroadcast(float gTime, std::string gKey, nlohmann::json gData): Script_TimedBroadcast(gTime, gKey) {
            messageData = gData;
        }

        void script() {
            start();

            wait(time);

            if (evt()) {
                parent->broadcastMessage(messageKey, messageData);
                finish();
            }
        }
    };

    class Script_Broadcast: public Amara::Script {
    public:
        std::string messageKey;
        nlohmann::json messageData = nullptr;

        Script_Broadcast(std::string gKey) {
            messageKey = gKey;
        }
        Script_Broadcast(std::string gKey, nlohmann::json gData): Script_Broadcast(gKey) {
            messageData = gData;
        }

        void script() {
            start();
            if (once()) broadcastMessage(messageKey, messageData);
            finishEvt();
        }
    };

    class Script_Destroy: public Script {
    public:
        Entity* toDestroy = nullptr;

        Script_Destroy() {}
        Script_Destroy(Entity* gEntity) {
            toDestroy = gEntity;
        }

        void prepare() {
            if (!toDestroy) toDestroy = parent;
        }

        void script() {
            if (toDestroy) toDestroy->destroy();
        }
    };

    class Script_Configure: public Script {
    public:
        Amara::Entity* target = nullptr;
        nlohmann::json config = nullptr;

        Script_Configure() {}
        Script_Configure(nlohmann::json gConfig) {
            config = gConfig;
        }
        Script_Configure(Entity* gTarget, nlohmann::json gConfig): Script_Configure(gConfig) {
            target = gTarget;
        }

        void prepare() {
            if (!target) target = parent;
        }

        void script() {
            target->configure(config);
            finish();
        }
    };

    class Script_SetFrame: public Amara::Script {
    public:
        Amara::Image* image = nullptr;
        int frame = 0;

        Script_SetFrame(int gFrame) {
            frame = gFrame;
        }
        Script_SetFrame(Amara::Image* gImage, int gFrame): Script_SetFrame(gFrame) {
            image = gImage;
        }

        void prepare() {
            if (image == nullptr) image = (Image*)parent;
        }

        void script() {
            image->setFrame(frame);
            finish();
        }
    };

    class Script_Animate: public Amara::Script {
    public:
        Amara::Sprite* sprite = nullptr;
        std::string animKey;

        Script_Animate(std::string gKey) {
            animKey = gKey;
        }
        Script_Animate(Amara::Sprite* gSprite, std::string key): Script_Animate(key) {
            sprite = gSprite;
        }

        void prepare() {
            if (sprite == nullptr) sprite = (Amara::Sprite*)parent;
            sprite->play(animKey);
        }

        void script() {
            start();
            if (evt()) {
                if (!sprite->anims.isActive || (sprite->anims.currentAnim != nullptr && sprite->anims.currentAnim->loop)) {
                    nextEvt();
                }
            }
            finishEvt();
        }
    };

    class Script_BringToFront: public Script {
    public:
        Amara::Entity* target = nullptr;

        Script_BringToFront() {}
        Script_BringToFront(Amara::Entity* gEntity) {
            target = gEntity;
        }

        void prepare() {
            if (target == nullptr) target = parent;
        }

        void script() {
            target->bringToFront();
            finish();
        }
    };
}
