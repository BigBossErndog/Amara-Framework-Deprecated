#pragma once
#ifndef AMARA_AUDIOBASE
#define AMARA_AUDIOBASE

#include "amara.h"

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

            bool isPlaying = false;
            bool isPaused = false;

			Amara::AudioGroup* group = nullptr;
			Amara::AudioBase* parent = nullptr;

			Amara::AudioBase* currentlyPlaying = nullptr;
			Amara::AudioBase* lastPlayed = nullptr;

            std::string nextInChain = "";

            AudioBase(): Amara::Asset() {}
            AudioBase(std::string gKey, Amara::AssetType gType, void* gAsset): Amara::Asset(gKey, gType, gAsset) {}

            void setVolume(float vol) {
                volume = vol;
            }
            void changeVolume(float c) {
                volume += c;
            }

            void fade(Amara::AudioFade fadeDir, float speed, float end, bool stopOnEnd) {
                fadeDirection = fadeDir;
                fadeSpeed = speed;
                fadeEnd = end;
				fadeStopOnEnd = stopOnEnd;
            }

			void fadeIn(float speed, float end, bool stopOnEnd) { fade(FADEIN, speed, end, stopOnEnd); }
            void fadeIn(float speed, float end) { fadeIn(speed, end, false); }
            void fadeIn(float speed) { fadeIn(speed, 1); }
            void fadeIn() { fadeIn(0.01); }

			void fadeOut(float speed, float end, bool stopOnEnd) { fade(FADEOUT, speed, end, stopOnEnd); }
            void fadeOut(float speed, float end) { fadeOut(speed, end, false); }
            void fadeOut(float speed) { fadeOut(speed, 0, true); }
            void fadeOut() { fadeOut(0.01); }

            void stopFade() {
                fadeDirection = NOFADE;
				fadeStopOnEnd = false;
            }

            virtual void play() {}
			virtual void play(int loops) {}
            virtual void pause() {}
            virtual void resume() {}
            virtual void stop() {}
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
