namespace Amara {
    class TintImage: public TextureContainer {
    public:
        Amara::Image* img = nullptr;
        Amara::Image* copy = nullptr;

        Amara::FillRect* fill = nullptr;

        SDL_Color tint = { 255, 255, 255, 255 };

        bool lockOn = false;

        TintImage(Amara::Image* gImg, SDL_Color gTint): TextureContainer(gImg->x, gImg->y, gImg->imageWidth, gImg->imageHeight) {
            img = gImg;
            tint = gTint;
        }
        TintImage(Amara::Image* gImg, SDL_Color gTint, bool gLock): TintImage(gImg, gTint) {
            lockOn = gLock;
        }

        void init() {
            add(copy = new Image(0, 0));
            copyPosition();

            add(fill = new FillRect(0, 0, width, height, tint));
            fill->blendMode = SDL_BLENDMODE_MOD;
        }

        void copyPosition() {
            if (img->isDestroyed) img = nullptr;
            if (!img) return;
            x = img->x;
            y = img->y;
            width = img->imageWidth;
            height = img->imageHeight;
            depth = img->depth + 0.0001;
            angle = img->angle;

            setOrigin(img->originX, img->originY);
            setScale(img->scaleX, img->scaleY);
            setZoomFactor(img->zoomFactorX, img->zoomFactorY);
            setScrollFactor(img->scrollFactorX, img->scrollFactorY);

            copy->setAlpha(img->alpha);
            copy->setFrame(img->frame);
            if (img->textureKey.compare(copy->textureKey) != 0) {
                copy->setTexture(img->textureKey);
            }
        }

        void run() {
            fill->color = tint;
            if (lockOn) {
                copyPosition();
                if (!img || img->isDestroyed) destroy();
            }
            Amara::TextureContainer::run();
        }
    };
}