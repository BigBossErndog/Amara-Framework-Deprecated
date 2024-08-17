namespace Amara {
    theora_t video_ctx = { 0 };

    class Video: public Amara::TextureContainer, Amara::Sound {
    public:
        std::string videoSrc;
        FILE* videoFile = nullptr;

        bool isPlaying = false;

        SDL_Rect intDest = { 0, 0, 0, 0 };

        int imageWidth = 0;
        int imageHeight = 0;

        bool autoSize = false;
        bool maintainAspectRatio = false;

        Video(std::string path, int gx, int gy, int gw, int gh): Amara::TextureContainer(), Amara::Sound() {
            videoSrc = path;
        }

        void init(Amara::GameProperties* gProps, Amara::Scene* gScene, Amara::Entity* gParent) {
            Amara::TextureContainer::init(gProps, gScene, gParent);
        }

        void setAudioGroup(std::string key) {
            Amara::Sound::parent = audio->getGroup(key);
        }

        void run() {
            if (videoFile) {
                if (Amara::Sound::parent) Amara::Sound::run(Amara::Sound::parent->calculatedVolume);
                if (video_ctx.hasVideo && theora_playing(&video_ctx)) {

                }
                else {
                    isPlaying = false;
                    stop();
                }
            }
        }

        void drawContent(int vx, int vy, int vw, int vh) {
            if (video_ctx.hasVideo && theora_playing(&video_ctx)) {

            }
            else {
                isPlaying = false;
                stop();
            }
            Amara::TextureContainer::drawContent();
        }

        bool playVideo() {
            videoFile = fopen(videoSrc.c_str(), "rb");
            if (videoFile) {
                theora_start(&video_ctx, videoFile);
                imageWidth = video_ctx.w;
                imageHeight = video_ctx.h;

                isPlaying = true;
            }
        }

        bool playVideo(std::string path) {
            videoSrc = path;
            playVideo();
        }

        void stopVideo() {
            if (videoFile) {
                if (Amara::Sound::parent && Amara::Sound::parent->currentlyPlaying == this) {
                    Amara::Sound::parent->currentlyPlaying = nullptr;
                } 
                theora_stop(&video_ctx);
                fclose(videoFile);
            }
            isPlaying = false;
        }

        bool isFinished() {
            return !isPlaying;
        }

        void destroy() {
            stop();
            Amara::TextureContainer::destroy();
        }
    };
}