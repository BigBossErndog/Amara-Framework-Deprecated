#pragma once
#ifndef AMARA_AUDIOBASE
#define AMARA_AUDIOBASE



namespace Amara {
    class AudioGroup;

    enum AudioFade {
        NOFADE = 0,
        FADEIN = 1,
        FADEOUT = 2
    };

    class AudioBase: public Amara::Asset {
        public:
            float volume = 1;
			float masterVolume = 1;

            Amara::AudioFade fadeDirection = NOFADE;
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

			AudioBase* fadeIn(float speed, float end, bool stopOnEnd) { return fade(FADEIN, speed, end, stopOnEnd); }
            AudioBase* fadeIn(float speed, float end) { return fadeIn(speed, end, false); }
            AudioBase* fadeIn(float speed) { return fadeIn(speed, 1); }
            AudioBase* fadeIn() { return fadeIn(0.01); }

			AudioBase* fadeOut(float speed, float end, bool stopOnEnd) { return fade(FADEOUT, speed, end, stopOnEnd); }
            AudioBase* fadeOut(float speed, float end) { return fadeOut(speed, end, false); }
            AudioBase* fadeOut(float speed) { return fadeOut(speed, 0, true); }
            AudioBase* fadeOut() { return fadeOut(0.01); }

            AudioBase* stopFade() {
                fadeDirection = NOFADE;
				fadeStopOnEnd = false;
                return this;
            }
            AudioBase* pauseOnEnd() {
                fadeStopOnEnd = false;
                fadePauseOnEnd = true;
                return this;
            }

            virtual AudioBase* play() { play(defaultLoops); }
			virtual AudioBase* play(int loops) { }
            virtual AudioBase* play(std::string) {}
            virtual AudioBase* pause() {}
            virtual AudioBase* resume() {}
            virtual AudioBase* stop() {}

            virtual void chain(std::string nextKey) {
                nextInChain = nextKey;
            }

            virtual AudioBase* getRootAudio() {
                if (parent) return parent->getRootAudio();
                return this;
            }

            virtual void run(float parentVolume) {
                switch (fadeDirection) {
                    case FADEIN:
                        masterVolume += fadeSpeed;
                        if (masterVolume >= fadeEnd) {
                            masterVolume = fadeEnd;
                            fadeDirection = NOFADE;
							if (fadeStopOnEnd) {
								fadeStopOnEnd = false;
								masterVolume = 1;
								stop();
							}
                        }
                        break;
                    case FADEOUT:
                        masterVolume -= fadeSpeed;
                        if (masterVolume <= fadeEnd) {
                            masterVolume = fadeEnd;
                            fadeDirection = NOFADE;
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
            }
    };
}

#endif
