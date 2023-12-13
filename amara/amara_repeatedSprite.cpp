namespace Amara {
    class RepeatedSprite: public Amara::Sprite {
    public:
        float patternBaseX = 0;
        float patternBaseY = 0;
        float patternOffsetX = 0;
        float patternOffsetY = 0;
        float patternMovementX = 0;
        float patternMovementY = 0;

        int width = 0;
        int height = 0;
        float recWidth = -1;
        float recHeight = -1;

        bool pixelLocked = false;

        SDL_FRect destRectF;
        SDL_Texture* canvas = nullptr;

        using Amara::Sprite::Sprite;
        RepeatedSprite(float gx, float gy, float gw, float gh, std::string tx): Amara::Sprite(gx, gy, tx) {
            width = gw;
            height = gh;
        }
        RepeatedSprite(float gw, float gh, std::string tx): Amara::Sprite(0, 0, tx) {
            width = gw;
            height = gh;
        }
        RepeatedSprite(float gx, float gy, float gw, float gh): Amara::Sprite(gx, gy) {
            width = gw;
            height = gh;
        }
        RepeatedSprite(float gw, float gh): Amara::Sprite(0, 0) {
            width = gw;
            height = gh;
        }

        using Amara::Image::init;
        virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
            Amara::Sprite::init(gameProperties, givenScene, givenParent);
            entityType = "repeatedSprite";
        }

        void configure(nlohmann::json config) {
            Amara::Sprite::configure(config);
            if (config.find("patternOffsetX") != config.end()) {
                patternOffsetX = config["patternOffsetX"];
            }
            if (config.find("patternOffsetY") != config.end()) {
                patternOffsetY = config["patternOffsetY"];
            }
        }

        void createNewCanvasTexture() {
            recWidth = width;
            recHeight = height;
            if (canvas != nullptr) {
                SDL_DestroyTexture(canvas);
            }
            canvas = SDL_CreateTexture(
                properties->gRenderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                floor(width),
                floor(height)
            );
        }

        void run() {
            Amara::Sprite::run();
            patternOffsetX += patternMovementX;
            patternOffsetY += patternMovementY;
        }

        virtual void drawPattern() {
            int cx = floor(patternBaseX + patternOffsetX);
            while (cx < 0) cx += Amara::Sprite::width;
            int cy = floor(patternBaseY + patternOffsetY);
            while (cy < 0) cy += Amara::Sprite::height;

            int ix = cx%Amara::Sprite::width - Amara::Sprite::width;
            int iy = cy%Amara::Sprite::height - Amara::Sprite::height;
            int pw = floor(width/Amara::Sprite::width)+2;
            int ph = floor(height/Amara::Sprite::height)+2;

            if (texture != nullptr) {
                SDL_Texture* tx = (SDL_Texture*)texture->asset;
                switch (texture->type) {
                    case IMAGE:
                        frame = 0;
                        srcRect.x = cropLeft;
                        srcRect.y = cropTop;
                        srcRect.w = imageWidth - cropLeft - cropRight;
                        srcRect.h = imageHeight - cropTop - cropBottom;
                        break;
                    case SPRITESHEET:
                        Amara::Spritesheet* spr = (Amara::Spritesheet*)texture;
                        int maxFrame = ((texture->width / spr->frameWidth) * (texture->height / spr->frameHeight));
                        frame = frame % maxFrame;

                        srcRect.x = (frame % (texture->width / spr->frameWidth)) * spr->frameWidth + cropLeft;
                        srcRect.y = floor(frame / (texture->width / spr->frameWidth)) * spr->frameHeight + cropTop;
                        srcRect.w = spr->frameWidth - cropLeft - cropRight;
                        srcRect.h = spr->frameHeight - cropTop - cropBottom;
                        break;
                }
                destRect.w = srcRect.w;
                destRect.h = srcRect.h;
                for (int i = 0; i < pw; i++) {
                    for (int j = 0; j < ph; j++) {
                        destRect.x = ix + i*Amara::Sprite::width + cropLeft;
                        destRect.y = iy + j*Amara::Sprite::height + cropTop;
                        SDL_RenderCopyF(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRect
                        );
                    }
                }
            }
        }

        void drawTexture(int vx, int vy, int vw, int vh) {
            if (!isVisible) return;
            if (alpha <= 0) {
                alpha = 0;
                return;
            }
            if (texture == nullptr) return;
            if (recWidth != width || recHeight != height) {
                createNewCanvasTexture();
            }
            else if (properties->renderTargetsReset || properties->renderDeviceReset) {
                createNewCanvasTexture();
            }

            SDL_Texture* recTarget = SDL_GetRenderTarget(properties->gRenderer);
            SDL_SetRenderTarget(properties->gRenderer, canvas);
            SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(canvas, 255);
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
            SDL_RenderClear(gRenderer);
            SDL_RenderSetViewport(properties->gRenderer, NULL);
            
            drawPattern();

            SDL_SetRenderTarget(properties->gRenderer, recTarget);
        
            bool skipDrawing = false;

            if (alpha < 0) alpha = 0;
            if (alpha > 1) alpha = 1;  

            viewport.x = vx;
            viewport.y = vy;
            viewport.w = vw;
            viewport.h = vh;
            SDL_RenderSetViewport(properties->gRenderer, &viewport);

            float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
            float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

            destRectF.x = ((x*scaleX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
            destRectF.y = ((y*scaleY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
            destRectF.w = ((width * scaleX) * nzoomX);
            destRectF.h = ((height * scaleY) * nzoomY);

            if (pixelLocked) {
                destRectF.x = floor(destRectF.x);
                destRectF.y = floor(destRectF.y);
                destRectF.w = ceil(destRectF.w);
                destRectF.h = ceil(destRectF.h);
            }

            origin.x = destRectF.w * originX;
            origin.y = destRectF.h * originY;

            int hx, hy, hw, hh = 0;
            hw = destRectF.w;
            hh = destRectF.h;

            if (destRectF.x + destRectF.w <= 0) skipDrawing = true;
            if (destRectF.y + destRectF.h <= 0) skipDrawing = true;
            if (destRectF.x >= vw) skipDrawing = true;
            if (destRectF.y >= vh) skipDrawing = true;
            if (destRectF.w <= 0) skipDrawing = true;
            if (destRectF.h <= 0) skipDrawing = true;

            if (!skipDrawing) {
                if (destRectF.x >= 0) {
                    hx = destRectF.x + vx;
                }
                else {
                    hw -= -(destRectF.x);
                    hx = vx;
                }
                if (destRectF.y >= 0) {
                    hy = destRectF.y + vy;
                }
                else {
                    hh -= -(destRectF.y);
                    hy = vy;
                }
                if (hx + hw > vx + vw) hw = ((vx + vw) - hx);
                if (hy + hh > vy + vh) hh = ((vy + vh) - hy);

                if (canvas != nullptr) {
                    SDL_SetTextureBlendMode(canvas, blendMode);
                    SDL_SetTextureAlphaMod(canvas, alpha * properties->alpha * 255);

                    SDL_RenderCopyExF(
                        properties->gRenderer,
                        canvas,
                        NULL,
                        &destRectF,
                        angle + properties->angle,
                        &origin,
                        SDL_FLIP_NONE
                    );

                    checkHover(vx, vy, vw, vh, destRectF.x, destRectF.y, destRectF.w, destRectF.h);
                }
            }
        }

        virtual void destroy(bool recursive) {
            if (canvas) {
                SDL_DestroyTexture(canvas);
                canvas = nullptr;
            }
            Amara::Sprite::destroy(recursive);
        }
    };
};