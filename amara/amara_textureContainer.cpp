namespace Amara {
    class TextureContainer: public Amara::Layer, public Amara::MakeRect {
    public:
        SDL_Texture* tx = nullptr;
        int textureWidth = -1;
        int textureHeight = -1;

        bool flipHorizontal = false;
        bool flipVertical = false;

        float renderOffsetX = 0;
		float renderOffsetY = 0;

        SDL_Texture* recTarget;
        SDL_Rect viewport;
        SDL_Rect srcRect;
        SDL_FRect destRect;
        SDL_FPoint origin = { 0, 0 };

        SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;
        
        bool textureLocked = false;
		bool pleaseUpdate = false;

        SDL_Color fillColor = { 0, 0, 0, 0 };

        TextureContainer(): Layer() {}
        TextureContainer(float gw, float gh) {
            width = gw;
            height = gh;
        }
        TextureContainer(float gx, float gy, float gw, float gh) {
            x = gx;
            y = gy;
            width = gw;
            height = gh;
        }

        using Amara::Layer::init;
        virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
            properties = gameProperties;
            createTexture();
            rectInit(this);
            Amara::Layer::init(gameProperties, givenScene, givenParent);
            entityType = "textureContainer";
        }

		void configure(nlohmann::json config) {
			Amara::Layer::configure(config);

			rectConfigure(config);
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
                width,
                height
            );
            textureWidth = width;
            textureHeight = height;

            clearTexture();

            pleaseUpdate = true;
        }

        void clearTexture() {
            if (tx == nullptr) return;
            
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

        Amara::TextureContainer* setRenderOffset(float gx, float gy) {
			renderOffsetX = gx;
			renderOffsetY = gy;
			return this;
		}

        Amara::FloatRect calculateRect() {
			// Doesn't take into account scrollFactor or zoomFactor.
			return { 
				(x+renderOffsetX + properties->offsetX - (originX * width * scaleX)),
				(y-z+renderOffsetY + properties->offsetY - (originY * height * scaleY)),
				(width * scaleX),
				(height * scaleY)
			};
		}

        void draw(int vx, int vy, int vw, int vh) {
            float recAlpha = properties->alpha;
            bool skipDrawing = false;

            if (alpha < 0) {
                alpha = 0;
                return;
            }
            if (alpha > 1) alpha = 1;

            if (properties->reloadAssets || textureWidth != width || textureHeight != height) {
				createTexture();
			}

            float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
            float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;
            
            bool scaleFlipHorizontal = false;
            bool scaleFlipVertical = false;
            float recScaleX = scaleX;
            float recScaleY = scaleY;

            if (scaleX < 0) {
                scaleFlipHorizontal = true;
                scaleX = abs(scaleX);
            }
            if (scaleY < 0) {
                scaleFlipVertical = true;
                scaleY = abs(scaleY);
            }
            scaleX = scaleX * (1 + (nzoomX - 1)*(zoomScaleX - 1));
            scaleY = scaleY * (1 + (nzoomY - 1)*(zoomScaleY - 1));

            destRect.x = ((x+renderOffsetX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
            destRect.y = ((y-z+renderOffsetY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
            destRect.w = ((width * scaleX) * nzoomX);
            destRect.h = ((height * scaleY) * nzoomY);

            scaleX = recScaleX;
            scaleY = recScaleY;

            origin.x = destRect.w * originX;
            origin.y = destRect.h * originY;

            if (destRect.x + destRect.w <= 0) skipDrawing = true;
            if (destRect.y + destRect.h <= 0) skipDrawing = true;
            if (destRect.x >= vw) skipDrawing = true;
            if (destRect.y >= vh) skipDrawing = true;
            if (destRect.w <= 0) skipDrawing = true;
            if (destRect.h <= 0) skipDrawing = true;

            if (!skipDrawing && tx != nullptr) {
                if (!textureLocked || pleaseUpdate) {
                    pleaseUpdate = false;

                    properties->interactOffsetX += vx + destRect.x;
                    properties->interactOffsetY += vy + destRect.y;

                    properties->interactScaleX *= scaleX;
                    properties->interactScaleY *= scaleY;

                    recTarget = SDL_GetRenderTarget(properties->gRenderer);
                    SDL_SetRenderTarget(properties->gRenderer, tx);
                    SDL_SetRenderDrawColor(properties->gRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
                    SDL_RenderClear(properties->gRenderer);
                    SDL_RenderSetViewport(properties->gRenderer, nullptr);

                    drawContent();
                    
                    properties->interactOffsetX -= vx + destRect.x;
                    properties->interactOffsetY -= vy + destRect.y;
                    
                    properties->interactScaleX /= scaleX;
                    properties->interactScaleY /= scaleY;
                    
                    SDL_SetRenderTarget(properties->gRenderer, recTarget);
                }

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);
                
                SDL_SetTextureBlendMode(tx, blendMode);
                SDL_SetTextureAlphaMod(tx, alpha * recAlpha * 255);

                SDL_RendererFlip flipVal = SDL_FLIP_NONE;
                if (!flipHorizontal != !scaleFlipHorizontal) {
                    flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_HORIZONTAL);
                }
                if (!flipVertical != !scaleFlipVertical) {
                    flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_VERTICAL);
                }

                SDL_RenderCopyExF(
                    properties->gRenderer,
                    tx,
                    NULL,
                    &destRect,
                    0,
                    &origin,
                    flipVal
                );

                checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
            }
        }

		virtual void drawContent() {
            drawEntities(0, 0, width, height);
		}

        virtual void drawEntities(int vx, int vy, int vw, int vh) {
            if (properties->quit) return;
            
            if (physics) {
                physics->checkActiveCollisionTargets();
            }

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;

            float recScrollX = properties->scrollX;
            float recScrollY = properties->scrollY;
            float recOffsetX = properties->offsetX;
            float recOffsetY = properties->offsetY;
            float recZoomX = properties->zoomX;
            float recZoomY = properties->zoomY;
            float recZoomFactorX = properties->zoomFactorX;
            float recZoomFactorY = properties->zoomFactorY;
            float recAngle = properties->angle;
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
                    properties->scrollX = 0;
                    properties->scrollY = 0;
                    properties->offsetX = 0;
                    properties->offsetY = 0;
                    properties->zoomX = 1;
                    properties->zoomY = 1;
                    properties->zoomFactorX = 1;
                    properties->zoomFactorY = 1;
                    properties->angle = 0;
                    properties->alpha = 1;
                    entity->draw(vx, vy, vw, vh);
                }
                ++it;
            }
            properties->alpha = recAlpha;
            properties->scrollX = recScrollX;
            properties->scrollY = recScrollY;
            properties->offsetX = recOffsetX;
            properties->offsetY = recOffsetY;
            properties->zoomX = recZoomX;
            properties->zoomY = recZoomY;
            properties->zoomFactorX = recZoomFactorX;
            properties->zoomFactorY = recZoomFactorY;
            properties->angle = recAngle;
        }

        void drawOnce() {
            textureLocked = true;
            pleaseUpdate = true;
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