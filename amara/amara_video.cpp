namespace Amara {
    theora_t video_ctx = { 0 };

    class Video;
    typedef struct VideoData {
        Amara::GameProperties* properties = nullptr;
        Amara::Video* video = nullptr;
        Amara::Sound* audio = nullptr;
        SDL_mutex* mutex = nullptr;
        SDL_Thread* thread = nullptr;
    } VideoData;

    VideoData current_video_data;

    int videoRenderThread(void* data);
    void videoAudioCallback(int gChannel);

    /*
    *   Note: Do not attempt to play more than one video at a time.
    */
    class Video: public Amara::TextureContainer, public Amara::Sound {
    public:
        std::string videoSrc;
        FILE* videoFile = nullptr;

        SDL_Texture* videoStream = nullptr;
        int streamWidth = 0;
        int streamHeight = 0;

        SDL_Rect intDest = { 0, 0, 0, 0 };

        bool audioEnabled = true;

        uint64_t length = 0;
        int fps = 0;

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
            fps = properties->fps;
        }
        
        Amara::Video* setAudioGroup(Amara::AudioGroup* group) {
            Amara::Sound::parent = group;
            return this;
        }

        Amara::Video* setAudioGroup(std::string key) {
            return setAudioGroup(audio->getGroup(key));
        }

        void createStreamTexture() {
            destroyStreamTexture();
            if (current_video_data.mutex) SDL_LockMutex(current_video_data.mutex);
            videoStream = SDL_CreateTexture(
                properties->gRenderer,
                SDL_PIXELFORMAT_IYUV,
                SDL_TEXTUREACCESS_STREAMING,
                width,
                height
            );
            streamWidth = width;
            streamHeight = height;
            if (current_video_data.mutex) SDL_UnlockMutex(current_video_data.mutex);
        }

        void destroyStreamTexture() {
            if (!videoStream) return;
            if (current_video_data.mutex) SDL_LockMutex(current_video_data.mutex);
            SDL_DestroyTexture(videoStream);
            videoStream = nullptr;
            if (current_video_data.mutex) SDL_UnlockMutex(current_video_data.mutex);
        }

        bool playVideo() {
            if (videoFile) stopVideo();

            videoFile = fopen(videoSrc.c_str(), "rb");
            if (videoFile) {
                current_video_data.video = this;
                current_video_data.properties = properties;
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

                    if (!videoStream  || streamWidth != width || streamHeight != height) {
                        createStreamTexture();
                    }
                    
                    current_video_data.mutex = SDL_CreateMutex();
                    current_video_data.thread = SDL_CreateThread(videoRenderThread, NULL, NULL);
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
            if (current_video_data.mutex) {
                SDL_LockMutex(current_video_data.mutex);
            }
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
            
            sound = nullptr;
            isPlaying = false;
            videoFile = nullptr;
            
            if (current_video_data.mutex) {
                SDL_UnlockMutex(current_video_data.mutex);
                SDL_DestroyMutex(current_video_data.mutex);
                current_video_data.mutex = nullptr;
            }
        }

        void run() {
            if (Amara::Sound::parent) {
                Amara::AudioBase::run(Amara::Sound::parent->calculatedVolume);
            }
            else {
                Amara::AudioBase::run(1);
            }
            if (videoFile) {
                if (!properties->quit && video_ctx.hasVideo && isPlaying) {
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
                SDL_LockMutex(current_video_data.mutex);
                if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx)) {
                    SDL_RenderCopy(properties->gRenderer, videoStream, NULL, NULL);
                    SDL_UnlockMutex(current_video_data.mutex);
                }
                else {
                    SDL_UnlockMutex(current_video_data.mutex);
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
            destroyStreamTexture();
            Amara::TextureContainer::destroy();
        }
    };

    int videoRenderThread(void* data) {
        Amara::Video* video = current_video_data.video;
        Amara::GameProperties* properties = current_video_data.properties;

        LTimer timer;
        int frameTicks, totalWait, tps;

        SDL_Texture* tx = video->videoStream;

        while (true) {
            SDL_LockMutex(current_video_data.mutex);
            timer.start();
            if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx) && video->isPlaying) {
                theora_video(&video_ctx, tx);
            }
            else {
                SDL_UnlockMutex(current_video_data.mutex);
                break;
            }
            SDL_UnlockMutex(current_video_data.mutex);
            totalWait = 0;
            tps = 1000 / video->fps;
            frameTicks = timer.getTicks();
            if (frameTicks < tps) {
                totalWait += (tps - frameTicks);
            }
            if (totalWait > 0) SDL_Delay(totalWait);
        }

        return 0;
    }

    void videoAudioCallback(int gChannel) {
        Amara::Sound* audio = current_video_data.audio;
        
        audio->sound = theora_audio(&video_ctx);
        if (audio->sound) {
            audio->channel = Mix_PlayChannel(gChannel, audio->sound, 0);
        }
    };
}