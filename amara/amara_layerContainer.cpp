namespace Amara {
    class Layer: public Amara::Actor {
    public:
        Layer(): Amara::Actor() {}

        Layer(float gx, float gy): Layer() {
            x = gx;
            y = gy;
        }

        using Amara::Actor::init;
        void init() {
            Amara::Actor::init();
            entityType = "layer";
        }

        virtual void draw(int vx, int vy, int vw, int vh) {
            Amara::Actor::draw(vx, vy, vw, vh);
        }
    };

    class UnsortedLayer: public Amara::Actor {
    public:
        UnsortedLayer(): Amara::Actor() {}

        UnsortedLayer(float gx, float gy): UnsortedLayer() {
            x = gx;
            y = gy;
        }

        using Amara::Actor::init;
        void init() {
            shouldSortChildren = false;
            Amara::Actor::init();
            entityType = "layer";
        }

        virtual void draw(int vx, int vy, int vw, int vh) {
            Amara::Actor::draw(vx, vy, vw, vh);
        }
    };

    class Container: public Amara::Layer, public Amara::MakeRect {
    public:
        Container(): Amara::Layer() {}
        Container(float w, float h): Container() {
            width = w;
            height = h;
        }
        Container(float gx, float gy, float w, float h): Container(w, h) {
            x = gx;
            y = gy;
        }

        using Amara::Layer::init;
        void init() {
            rectInit(this);
			Amara::Layer::init();
			entityType = "container";
		}

		void configure(nlohmann::json config) {
			Amara::Layer::configure(config);
			rectConfigure(config);
		}

        Amara::FloatRect calculateRect() {
			// Doesn't take into account scrollFactor or zoomFactor.
			return { 
				(x + properties->offsetX - (originX * width * scaleX)),
				(y-z + properties->offsetY - (originY * height * scaleY)),
				(width * scaleX),
				(height * scaleY)
			};
		}

        virtual void draw(int vx, int vy, int vw, int vh) override {
            float dx = 0, dy = 0, dw = 0, dh = 0, ox = 0, oy = 0;

            float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
            float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY; 
            dx = floor((x - properties->scrollX*scrollFactorX + properties->offsetX - (originX*width*scaleX)) * nzoomX);
            dy = floor((y-z - properties->scrollY*scrollFactorY + properties->offsetY - (originY*height*scaleY)) * nzoomY);
            dw = width * scaleX * nzoomX;
            dh = height * scaleY * nzoomY;

            if (dx < 0) {
                dw += dx;
                ox += dx/(scaleX*nzoomX);
                dx = 0;
            }
            if (dy < 0) {
                dh += dy;
                oy += dy/(scaleY*nzoomY);
                dy = 0;
            }
            if (dx + dw > vw) {
                dw = vw - dx;
            }
            if (dy + dh > vh) {
                dh = vh - dy;
            }

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;
            
            checkHover(vx + dx, vy + dy, dw, dh);
            
            if (shouldSortChildren || sortChildrenOnce) {
                sortChildrenOnce = false;
                delayedSorting();
            }

            float recZoomX = properties->zoomX;
            float recZoomY = properties->zoomY;
            float recZoomFactorX = properties->zoomFactorX * zoomFactorX;
            float recZoomFactorY = properties->zoomFactorY * zoomFactorY;
            float recAlpha = properties->alpha * alpha;

            Amara::Entity* entity;
            for (auto it = children.begin(); it != children.end();) {
                entity = *it;
                properties->scrollX = 0;
                properties->scrollY = 0;
                properties->offsetX = ox;
                properties->offsetY = oy;
                properties->zoomX = recZoomX * scaleX;
                properties->zoomY = recZoomY * scaleY;
                properties->zoomFactorX = recZoomFactorX;
                properties->zoomFactorY = recZoomFactorY;
                properties->alpha = recAlpha;
                if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
                    it = children.erase(it);
                    continue;
                }
                if (entity->isVisible) {
                    entity->draw(vx + dx, vy + dy, dw, dh);
                }
                ++it;
            }
        }
    };
    
    class TextureLayer: public Amara::Layer {
    public:
        SDL_Texture* tx = nullptr;
        int textureWidth = -1;
        int textureHeight = -1;

        SDL_Texture* recTarget;
        SDL_Rect viewport;
        SDL_Rect srcRect;
        SDL_FRect destRect;
        SDL_FPoint origin = { 0, 0 };

        SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

        bool textureLocked = false;
		bool pleaseUpdate = false;

        SDL_Color fillColor = { 0, 0, 0, 0 };

        TextureLayer(): Layer() {}
        TextureLayer(float gx, float gy): Layer(gx, gy) {}

        using Amara::Layer::init;
        virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
            properties = gameProperties;
            createTexture();
            Amara::Layer::init(gameProperties, givenScene, givenParent);
            entityType = "textureLayer";
        }

        virtual void configure(nlohmann::json config) {
            Amara::Layer::configure(config);

            if (config.find("textureLocked") != config.end()) {
                setTextureLock(config["textureLocked"]);
            }
        }

        virtual void createTexture() {
            if (tx) {
                tasks->queueTexture(tx);
            }
            tx = SDL_CreateTexture(
                properties->gRenderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_TARGET,
                properties->currentCamera->width,
                properties->currentCamera->height
            );
            textureWidth = properties->currentCamera->width;
            textureHeight = properties->currentCamera->height;
            clearTexture();

            pleaseUpdate = true;
        }

        void clearTexture() {
            recTarget = SDL_GetRenderTarget(properties->gRenderer);
            SDL_SetRenderTarget(properties->gRenderer, tx);
            SDL_SetRenderDrawColor(properties->gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
            SDL_RenderClear(properties->gRenderer);

            SDL_SetRenderTarget(properties->gRenderer, recTarget);
        }

        void setTextureLock(bool gLock) {
            textureLocked = gLock;
            pleaseUpdate = true;
        }

        void drawOnce() {
            textureLocked = true;
            pleaseUpdate = true;
        }

        void draw(int vx, int vy, int vw, int vh) {
            float recAlpha = properties->alpha;

			if (properties->reloadAssets) {
				createTexture();
			}
            else if (textureWidth != properties->currentCamera->width || textureHeight != properties->currentCamera->height) {
                createTexture();
            }
            if (!tx) return;

            if (!textureLocked || pleaseUpdate) {
				pleaseUpdate = false;

                recTarget = SDL_GetRenderTarget(properties->gRenderer);
                SDL_SetRenderTarget(properties->gRenderer, tx);
                SDL_SetRenderDrawColor(properties->gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
                SDL_RenderClear(properties->gRenderer);
                SDL_RenderSetViewport(properties->gRenderer, nullptr);

				properties->interactOffsetX += vx;
				properties->interactOffsetY += vy;

				properties->interactScaleX *= scaleX;
				properties->interactScaleY *= scaleY;
                
                drawContent(vx, vy, vw, vh);

				properties->interactOffsetX -= vx;
				properties->interactOffsetY -= vy;

				properties->interactScaleX /= scaleX;
				properties->interactScaleY /= scaleY;

                SDL_SetRenderTarget(properties->gRenderer, recTarget);
            }

            bool skipDrawing = false;

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;

            viewport.x = 0;
            viewport.y = 0;
            viewport.w = textureWidth;
            viewport.h = textureHeight;
            SDL_RenderSetViewport(properties->gRenderer, &viewport);

            destRect.x = 0;
            destRect.y = 0;
            destRect.w = textureWidth;
            destRect.h = textureHeight;
            
            if (destRect.x + destRect.w <= 0) skipDrawing = true;
            if (destRect.y + destRect.h <= 0) skipDrawing = true;
            if (destRect.w <= 0) skipDrawing = true;
            if (destRect.h <= 0) skipDrawing = true;

            if (!skipDrawing && tx != nullptr) {
                SDL_SetTextureBlendMode(tx, blendMode);
                SDL_SetTextureAlphaMod(tx, alpha * recAlpha * 255);

                SDL_RenderCopyExF(
                    properties->gRenderer,
                    tx,
                    NULL,
                    &destRect,
                    0,
                    &origin,
                    SDL_FLIP_NONE
                );

                checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
            }
        }

        void drawEntities(int vx, int vy, int vw, int vh) {
            if (properties->quit) return;
            if (physics) {
                physics->checkActiveCollisionTargets();
            }

            if (alpha < 0) {
                alpha = 0;
                return;
            }
            if (alpha > 1) alpha = 1;

            float recScrollX = properties->scrollX * scrollFactorX;
            float recScrollY = properties->scrollY * scrollFactorY;
            float recOffsetX = properties->offsetX + x;
            float recOffsetY = properties->offsetY + y;
            float recZoomX = properties->zoomX * scaleX;
            float recZoomY = properties->zoomY * scaleY;
            float recZoomFactorX = properties->zoomFactorX * zoomFactorX;
            float recZoomFactorY = properties->zoomFactorY * zoomFactorY;
            float recAngle = properties->angle + angle;
            float recAlpha = properties->alpha;
            properties->alpha = 1;

            if (shouldSortChildren || sortChildrenOnce) {
                sortChildrenOnce = false;
                delayedSorting();
            }

            Amara::Entity* entity;
            for (auto it = children.begin(); it != children.end();) {
                entity = *it;

                if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
					if (properties->inSceneDrawing) it = children.erase(it);
					else ++it;
					continue;
				}
                if (entity->isVisible) {
                    properties->scrollX = recScrollX;
                    properties->scrollY = recScrollY;
                    properties->offsetX = recOffsetX;
                    properties->offsetY = recOffsetY;
                    properties->zoomX = recZoomX;
                    properties->zoomY = recZoomY;
                    properties->zoomFactorX = recZoomFactorX;
                    properties->zoomFactorY = recZoomFactorY;
                    properties->angle = recAngle;
                    properties->alpha = 1;
                    entity->draw(vx, vy, vw, vh);
                }

                ++it;
            }
            properties->alpha = recAlpha;
        }

        virtual void drawContent(int vx, int vy, int vw, int vh) {
            drawEntities(vx, vy, vw, vh);
        }

        using Amara::Layer::destroy;
        virtual void destroy(bool recursive) {
            if (tx) {
                tasks->queueTexture(tx);
                tx = nullptr;
            }
            Amara::Layer::destroy(recursive);
        }
    };
}