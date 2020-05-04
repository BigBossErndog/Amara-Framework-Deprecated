#pragma once
#ifndef AMARA_AUDIOBASE
#define AMARA_AUDIOBASE

#include "amara.h"

namespace Amara {
    class AudioGroup;

    enum AudioFade {
        NOFADE,
        FADEIN, 
        FADEOUT
    };

    class AudioBase: public Amara::Asset {
        public:
            float volume = 1;
			float masterVolume = 1;

            Amara::AudioFade fadeDirection = NOFADE;
            float fadeSpeed = 0;
            float fadeEnd = 0;

            bool isPlaying = false;
            bool isPaused = false;
            
            AudioBase(): Amara::Asset() {}
            AudioBase(std::string gKey, Amara::AssetType gType, void* gAsset): Amara::Asset(gKey, gType, gAsset) {}

            Amara::AudioGroup* group = nullptr;
            
            std::string nextInChain = "";
            
            void setVolume(float vol) {
                volume = vol;
            }
            void changeVolume(float c) {
                volume += c;
            }

            void fade(Amara::AudioFade fadeDir, float speed, float end) {
                fadeDirection = fadeDir;
                fadeSpeed = speed;
                fadeEnd = end;
            }

            void fadeIn(float speed, float end) { fade(FADEIN, speed, end); }
            void fadeIn(float speed) { fadeIn(speed, 1); }
            void fadeIn() { fadeIn(0.01); }

            void fadeOut(float speed, float end) { fade(FADEOUT, speed, end); }
            void fadeOut(float speed) { fadeOut(speed, 0); }
            void fadeOut() { fadeOut(0.01); } 

            void stopFade() {
                fadeDirection = NOFADE;
            }


            virtual void play() {}
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
                        }
                        break;
                    case FADEOUT:
                        masterVolume -= fadeSpeed;
                        if (masterVolume <= fadeEnd) {
                            masterVolume = fadeEnd;
                            fadeDirection = NOFADE;
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