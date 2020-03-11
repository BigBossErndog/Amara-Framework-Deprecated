#ifndef AMARA_MUSIC
#define AMARA_MUSIC

#include "amara.h"

namespace Amara {
    class Music: public Amara::AudioBase {
        public:
            Amara::GameProperties* properties = nullptr;
            Mix_Music* music = nullptr;
            int channel = -1;
            bool isPaused = false;

            Music(string givenKey, AssetType givenType, Mix_Music* givenAsset, Amara::GameProperties* gameProperties): Amara::AudioBase(givenKey, givenType, givenAsset) {
                properties = gameProperties;
				music = givenAsset;
            }

			void play(int loops) {
				if (Mix_PlayingMusic()) {
					if (properties->music == this) {
						if (Mix_PausedMusic()) {
							Mix_ResumeMusic();
						}
					}
					else {
						Mix_HaltMusic();
					}
				}
				Mix_PlayMusic(music, loops);
				properties->music = this;
				isPaused = false;
			}

			void play() {
				play(0);
			}

			void pause() {
				if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
					if (properties->music == this) {
						Mix_PauseMusic();
						isPaused = true;
					}
				}
			}

			void resume() {
				if (Mix_PlayingMusic() && Mix_PausedMusic()) {
					if (properties->music == this) {
						Mix_ResumeMusic();
						isPaused = false;
					}
				}
			}

			bool finished() {

			}

			void stop() {
				if (Mix_PlayingMusic()) {
					if (properties->music == this) {
						Mix_HaltMusic();
						isPaused = true;
					}
				}
			}

            void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

                if (Mix_PlayingMusic() && properties->music == this) {
					if (volume < 0) {
						volume = 0;
					}
					if (volume > 1) {
						volume = 1;
					}

					Mix_VolumeMusic(floor(volume * masterVolume * parentVolume * 128));
				}
            }
    };
}

#endif