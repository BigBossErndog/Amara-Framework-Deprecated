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

        SDL_Rect destRectInt;
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
            rectInit(this);
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
                tasks->queueTexture(canvas);
            }
            canvas = SDL_CreateTexture(
                properties->gRenderer,
                SDL_PIXELFORMAT_ARGB8888,
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

            int ix = cx%Amara::Sprite::imageWidth - Amara::Sprite::imageWidth;
            int iy = cy%Amara::Sprite::imageHeight - Amara::Sprite::imageHeight;
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
                destRectInt.w = srcRect.w;
                destRectInt.h = srcRect.h;
                for (int i = 0; i < pw; i++) {
                    for (int j = 0; j < ph; j++) {
                        destRectInt.x = ix + i*Amara::Sprite::width + cropLeft;
                        destRectInt.y = iy + j*Amara::Sprite::height + cropTop;
                        SDL_RenderCopy(
                            gRenderer,
                            (SDL_Texture*)(texture->asset),
                            &srcRect,
                            &destRectInt
                        );
                    }
                }
            }
        }

        void drawTexture(int vx, int vy, int vw, int vh) {
            if (alpha < 0) {
                alpha = 0;
                return;
            }
            if (alpha > 1) alpha = 1;
            if (texture == nullptr) return;
            if (recWidth != width || recHeight != height) {
                createNewCanvasTexture();
            }
            else if (properties->reloadAssets) {
                createNewCanvasTexture();
            }

            bool skipDrawing = false;

            viewport.x = vx;
            viewport.y = vy;
            viewport.w = vw;
            viewport.h = vh;

            float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
            float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

            destRect.x = ((x*scaleX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
            destRect.y = ((y*scaleY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
            destRect.w = ((width * scaleX) * nzoomX);
            destRect.h = ((height * scaleY) * nzoomY);

            origin.x = destRect.w * originX;
            origin.y = destRect.h * originY;

            if (destRect.x + destRect.w <= 0) skipDrawing = true;
            if (destRect.y + destRect.h <= 0) skipDrawing = true;
            if (destRect.x >= vw) skipDrawing = true;
            if (destRect.y >= vh) skipDrawing = true;
            if (destRect.w <= 0) skipDrawing = true;
            if (destRect.h <= 0) skipDrawing = true;

            if (!skipDrawing && canvas != nullptr) {
                SDL_Texture* recTarget = SDL_GetRenderTarget(properties->gRenderer);
                SDL_SetRenderTarget(properties->gRenderer, canvas);
                SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_NONE);
                SDL_SetTextureAlphaMod(canvas, 255);
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
                SDL_RenderClear(gRenderer);
                SDL_RenderSetViewport(properties->gRenderer, NULL);
                
                drawPattern();

                SDL_SetRenderTarget(properties->gRenderer, recTarget);
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                SDL_SetTextureBlendMode(canvas, blendMode);
                SDL_SetTextureAlphaMod(canvas, alpha * properties->alpha * 255);

                SDL_RenderCopyExF(
                    properties->gRenderer,
                    canvas,
                    NULL,
                    &destRect,
                    angle + properties->angle,
                    &origin,
                    SDL_FLIP_NONE
                );

                checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
            }
        }

        virtual void destroy(bool recursive) {
            if (canvas) {
                tasks->queueTexture(canvas);
                canvas = nullptr;
            }
            Amara::Sprite::destroy(recursive);
        }
    };
};