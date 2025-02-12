#ifndef AMARA_NOVIDEO

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
    *   Do not play any other audio while video is playing is audioPlayback either.
    */
    class Video: public Amara::TextureContainer, public Amara::Sound {
    public:
        std::string videoSrc;
        FILE* videoFile = nullptr;
        SDL_Texture* videoStream = nullptr;

        uint64_t duration = 0;

        bool audioEnabled = true;

        uint64_t length = 0;

        using Amara::TextureContainer::TextureContainer;
        using Amara::Sound::Sound;

        int frameSkip = 0;
        int frameSkipCount = 0;
        
        bool startedStreaming = false;

        double renderDelayTime = 0;
        int renderDelayCounter = 0;

        bool audioPlayback = true;
        
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
            renderDelayTime = 1/60;
        }

        Amara::Video* setAudioGroup(Amara::AudioGroup* group) {
            Amara::Sound::parent = group;
            return this;
        }

        Amara::Video* setAudioGroup(std::string key) {
            return setAudioGroup(audio->getGroup(key));
        }

        virtual void createTexture() {
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
            tx = videoStream;
            textureWidth = width;
            textureHeight = height;
        }

        Video* setAudioPlayback(bool gPlayback) {
            audioPlayback = gPlayback;
            return this;
        }

        bool playVideo() {
            if (videoFile) stopVideo();

            videoFile = fopen(videoSrc.c_str(), "rb");
            if (videoFile) {
                current_video_data.video = this;
                startedStreaming = false;
                clearTexture();

                duration = theora_getduration(videoFile);
                theora_start(&video_ctx, videoFile);

                if (video_ctx.hasVideo) {
                    width = video_ctx.w;
                    height = video_ctx.h;

                    isPlaying = true;
                    renderDelayCounter = 0;

                    if (audioEnabled) {
                        sound = nullptr;
                        if (audioPlayback && video_ctx.hasAudio) {
                            current_video_data.audio = this;
                            sound = theora_audio(&video_ctx);
                            if (sound) {
                                SDL_LockAudio();
                                    Mix_ChannelFinished(videoAudioCallback);
                                SDL_UnlockAudio();
                                videoAudioCallback(0);
                                Amara::Sound::isPlaying = true;
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

        double getProgress() {
            if (!isPlaying) return 0;
            return (SDL_GetTicks() - video_ctx.baseticks)/(double)duration;
        }

        bool renderDelayed() {
            if (isPlaying && renderDelayCounter < renderDelayTime*properties->fps) return true;
            else return false;
        }

        Video* setRenderDelay(double amount) {
            renderDelayTime = amount;
            return this;
        } 

        void stopVideo() {
            if (Amara::Sound::parent && Amara::Sound::parent->currentlyPlaying == this) {
                Amara::Sound::parent->currentlyPlaying = nullptr;
            }
            
            if (Amara::Sound::isPlaying && Mix_Playing(-1)) {
                SDL_LockAudio();
                    Mix_ChannelFinished(NULL);
                    Mix_HaltChannel(-1);
                SDL_UnlockAudio();
            }
            channel = -1;

            if (videoFile) {
                theora_stop(&video_ctx);
                fclose(videoFile);
                videoFile = nullptr;
            }

            duration = 0;

            SDL_Log("Video Stopped: \"%s\"", videoSrc.c_str());
            
            sound = nullptr;
            Amara::Sound::isPlaying = false;
            Amara::Sound::isPaused = false;
            Amara::Entity::isPaused = false;
        }

        void run() {
            if (Amara::Sound::parent) {
                Amara::AudioBase::run(Amara::Sound::parent->calculatedVolume);
            }
            else {
                Amara::AudioBase::run(1);
            }

            if (videoFile) {
                if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx)) {
                    if (channel != -1 && Mix_Playing(channel)) {
                        Mix_Volume(channel, floor(calculatedVolume * MIX_MAX_VOLUME));
                    }
                }
                else {
                    stopVideo();
                }
            }
            Amara::TextureContainer::run();
        }

        void draw(int vx, int vy, int vw, int vh) {
            if (!videoFile) return;
            
            float recAlpha = properties->alpha;
            bool skipDrawing = false;

            if (alpha < 0) {
                alpha = 0;
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
                    drawContent();
                }

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                SDL_RendererFlip flipVal = SDL_FLIP_NONE;
                if (!flipHorizontal != !scaleFlipHorizontal) {
                    flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_HORIZONTAL);
                }
                if (!flipVertical != !scaleFlipVertical) {
                    flipVal = (SDL_RendererFlip)(flipVal | SDL_FLIP_VERTICAL);
                }

                if (startedStreaming && !renderDelayed()) {
                    SDL_SetTextureBlendMode(tx, blendMode);
                    SDL_SetTextureAlphaMod(tx, alpha * recAlpha * 255);

                    SDL_RenderCopyExF(
                        properties->gRenderer,
                        tx,
                        NULL,
                        &destRect,
                        0,
                        &origin,
                        flipVal
                    );
                }

                checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
            }
            if (isPlaying && renderDelayCounter < renderDelayTime*properties->lps) {
                renderDelayCounter += 1;
            }
        }

        void drawContent() {
            if (videoFile) {
                if (!properties->quit && video_ctx.hasVideo && theora_playing(&video_ctx)) {
                    if (frameSkipCount % (frameSkip + 1) == 0) {
                        startedStreaming = true;
                        theora_video(&video_ctx, videoStream);
                        frameSkipCount = 0;
                    }
                    frameSkipCount += 1;
                }
                else {
                    stopVideo();
                }
            }
        }

        bool isFinished() {
            return !isPlaying;
        }

        void destroy() {
            stopVideo();
            videoStream = nullptr;
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

#endif