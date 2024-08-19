namespace Amara {
    class AudioGroup;

    enum AudioFade {
        AudioFade_NOFADE = 0,
        AudioFade_FADEIN = 1,
        AudioFade_FADEOUT = 2
    };

    class AudioBase: public Amara::Asset {
    public:
        float volume = 1;
        float masterVolume = 1;
        float calculatedVolume = 1;

        Amara::AudioFade fadeDirection = AudioFade_NOFADE;
        float fadeSpeed = 0;
        float fadeEnd = 0;
        bool fadeStopOnEnd = false;
        bool fadePauseOnEnd = false;

        bool isPlaying = false;
        bool isPaused = false;

        int defaultLoops = -1;

        Amara::AudioGroup* group = nullptr;
        Amara::AudioBase* parent = nullptr;

        Amara::AudioBase* currentlyPlaying = nullptr;
        Amara::AudioBase* lastPlayed = nullptr;

        std::string nextInChain = "";

        AudioBase(): Amara::Asset() {}
        AudioBase(std::string gKey, Amara::AssetType gType): Amara::Asset(gKey, gType) {}

        AudioBase* setVolume(float vol) {
            volume = vol;
            return this;
        }
        AudioBase* setMasterVolume(float vol) {
            masterVolume = vol;
            return this;
        }
        AudioBase* changeVolume(float c) {
            volume += c;
            return this;
        }

        AudioBase* fade(Amara::AudioFade fadeDir, float speed, float end, bool stopOnEnd) {
            fadeDirection = fadeDir;
            fadeSpeed = speed;
            fadeEnd = end;
            fadeStopOnEnd = stopOnEnd;
            return this;
        }

        AudioBase* fadeIn(float speed, float end, bool stopOnEnd) { return fade(AudioFade_FADEIN, speed, end, stopOnEnd); }
        AudioBase* fadeIn(float speed, float end) { return fadeIn(speed, end, false); }
        AudioBase* fadeIn(float speed) { return fadeIn(speed, 1); }
        AudioBase* fadeIn() { return fadeIn(0.01); }

        AudioBase* fadeOut(float speed, float end, bool stopOnEnd) { return fade(AudioFade_FADEOUT, speed, end, stopOnEnd); }
        AudioBase* fadeOut(float speed, float end) { return fadeOut(speed, end, false); }
        AudioBase* fadeOut(float speed) { return fadeOut(speed, 0, true); }
        AudioBase* fadeOut() { return fadeOut(0.01); }

        AudioBase* transferFade(AudioBase* other) {
            other->masterVolume = masterVolume;
            other->fadeDirection = fadeDirection;
            other->fadeSpeed = fadeSpeed;
            other->fadeEnd = fadeEnd;
            other->fadeStopOnEnd = fadeStopOnEnd;
            other->fadePauseOnEnd = fadePauseOnEnd;
            return this;
        }

        AudioBase* stopFade() {
            fadeDirection = AudioFade_NOFADE;
            fadeStopOnEnd = false;
            return this;
        }
        AudioBase* pauseOnEnd() {
            fadeStopOnEnd = false;
            fadePauseOnEnd = true;
            return this;
        }

        virtual AudioBase* play() { return play(defaultLoops); }
        virtual AudioBase* play(int loops) { }
        virtual AudioBase* play(std::string) {}
        virtual AudioBase* pause() {}
        virtual AudioBase* resume() {}
        virtual AudioBase* stop() {}

        virtual void onStop() {}

        virtual void chain(std::string nextKey) {
            nextInChain = nextKey;
        }

        virtual AudioBase* getRootAudio() {
            if (parent) return parent->getRootAudio();
            return this;
        }

        void calculateVol(float parentVolume) {
            calculatedVolume = volume * masterVolume * parentVolume;
        }

        virtual void run(float parentVolume) {
            switch (fadeDirection) {
                case AudioFade_FADEIN:
                    masterVolume += fadeSpeed;
                    if (masterVolume >= fadeEnd) {
                        masterVolume = fadeEnd;
                        fadeDirection = AudioFade_NOFADE;
                        if (fadeStopOnEnd) {
                            fadeStopOnEnd = false;
                            masterVolume = 1;
                            stop();
                        }
                    }
                    break;
                case AudioFade_FADEOUT:
                    masterVolume -= fadeSpeed;
                    if (masterVolume <= fadeEnd) {
                        masterVolume = fadeEnd;
                        fadeDirection = AudioFade_NOFADE;
                        if (fadeStopOnEnd) {
                            fadeStopOnEnd = false;
                            masterVolume = 1;
                            stop();
                        }
                        else if (fadePauseOnEnd) {
                            fadePauseOnEnd = false;
                            masterVolume = 1;
                            pause();
                        }
                    }
                    break;
            }
            if (volume < 0) {
                volume = 0;
            }
            if (volume > 1) {
                volume = 1;
            }
            
            calculateVol(parentVolume);
        }
    };
}
