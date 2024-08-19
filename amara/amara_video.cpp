namespace Amara {
    theora_t video_ctx = { 0 };

    class Video;
    typedef struct VideoData {
        Amara::Video* video = nullptr;
        Amara::Sound* audio = nullptr;
    } VideoData;

    VideoData current_video_data;

    void videoAudioCallback(int gChannel);

    /*
    *   Note: Do not attempt to play more than one video at a time.
    */
    class Video: public Amara::TextureContainer, public Amara::Sound {
    public:
        std::string videoSrc;
        FILE* videoFile = nullptr;

        SDL_Texture* videoStream = nullptr;

        SDL_Rect intDest = { 0, 0, 0, 0 };

        bool audioEnabled = true;

        uint64_t length = 0;

        using Amara::TextureContainer::TextureContainer;
        using Amara::Sound::Sound;
        
        Video(): Amara::Sound() {}

        Video(float gx, float gy): Video() {
            x = gx;
            y = gy;
        }
        Video(std::string path): Video() {
            videoSrc = path;
        }
        Video(float gx, float gy, std::string path): Video(gx, gy) {
            videoSrc = path;
        }

        void init(Amara::GameProperties* gProps, Amara::Scene* gScene, Amara::Entity* gParent) {
            Amara::TextureContainer::init(gProps, gScene, gParent);
            entityType = "video";
        }

        Amara::Video* setAudioGroup(Amara::AudioGroup* group) {
            Amara::Sound::parent = group;
            return this;
        }

        Amara::Video* setAudioGroup(std::string key) {
            return setAudioGroup(audio->getGroup(key));
        }

        virtual void createTexture() {
            Amara::TextureContainer::createTexture();

            if (videoStream) {
                tasks->queueTexture(videoStream);
            }
            videoStream = SDL_CreateTexture(
                properties->gRenderer,
                SDL_PIXELFORMAT_IYUV,
                SDL_TEXTUREACCESS_STREAMING,
                width,
                height
            );
        }

        bool playVideo() {
            if (videoFile) stopVideo();

            videoFile = fopen(videoSrc.c_str(), "rb");
            if (videoFile) {
                current_video_data.video = this;
                clearTexture();

                theora_start(&video_ctx, videoFile);
                if (video_ctx.hasVideo) {
                    width = video_ctx.w;
                    height = video_ctx.h;

                    isPlaying = true;

                    if (audioEnabled) {
                        sound = nullptr;
                        if (video_ctx.hasAudio) {
                            current_video_data.audio = this;
                            sound = theora_audio(&video_ctx);
                            if (sound) {
                                Mix_ChannelFinished(videoAudioCallback);
                                videoAudioCallback(-1);
                            }
                        }
                        if (!sound) {
                            SDL_Log("Video Error: Unable to start sound on video \"%s\".", videoSrc.c_str());
                        }
                    }
                    SDL_Log("Video Started: \"%s\".", videoSrc.c_str());
                    return true;
                }
                else {
                    SDL_Log("Video Error: Could not play video file \"%s\".", videoSrc.c_str());
                }
            }
            else {
                SDL_Log("Video Error: Could not open video file \"%s\".", videoSrc.c_str());
            }
            stopVideo();
            return false;
        }

        bool playVideo(std::string path) {
            videoSrc = path;
            return playVideo();
        }

        bool playVideo(bool withSound) {
            audioEnabled = withSound;
            return playVideo();
        }

        bool playVideo(std::string path, bool withSound) {
            audioEnabled = withSound;
            return playVideo(path);
        }

        void stopVideo() {
            if (videoFile) {
                theora_stop(&video_ctx);
                fclose(videoFile);
            }
            if (Amara::Sound::parent && Amara::Sound::parent->currentlyPlaying == this) {
                Amara::Sound::parent->currentlyPlaying = nullptr;
            }
            if (channel != -1) {
                Mix_ChannelFinished(NULL);
                Amara::Sound::stop();
            }
            clearTexture();

            SDL_Log("Video Stopped: \"%s\"", videoSrc.c_str());
            
            videoFile = nullptr;
            sound = nullptr;
            isPlaying = false;
        }

        void run() {
            if (Amara::Sound::parent) {
                Amara::AudioBase::run(Amara::Sound::parent->calculatedVolume);
            }
            else {
                Amara::AudioBase::run(1);
            }
            SDL_Log("a2");
            if (videoFile) {
                if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx)) {
                    if (channel != -1) {
                        if (Mix_Playing(channel)) {
                            Mix_Volume(channel, floor(calculatedVolume * MIX_MAX_VOLUME));
                        }
                    }
                }
                else {
                    isPlaying = false;
                    stopVideo();
                }
            }
            Amara::TextureContainer::run();
        }

        void draw(int vx, int vy, int vw, int vh) {
            if (videoFile == nullptr) return;

            Amara::TextureContainer::draw(vx, vy, vw, vh);
        } 

        void drawContent() {
            if (videoFile) {
                if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx)) {
                    theora_video(&video_ctx, videoStream);
                    SDL_RenderCopy(properties->gRenderer, videoStream, NULL, NULL);
                }
                else {
                    isPlaying = false;
                    stopVideo();
                }
            }
            Amara::TextureContainer::drawContent();
        }

        bool isFinished() {
            return !isPlaying;
        }

        void destroy() {
            stopVideo();
            if (videoStream) {
                tasks->queueTexture(videoStream);
                videoStream = nullptr;
            }
            Amara::TextureContainer::destroy();
        }
    };

    void videoAudioCallback(int gChannel) {
        Amara::Sound* audio = current_video_data.audio;
        
        audio->sound = theora_audio(&video_ctx);
        if (audio->sound) {
            audio->channel = Mix_PlayChannel(gChannel, audio->sound, 0);
        }
    };
}