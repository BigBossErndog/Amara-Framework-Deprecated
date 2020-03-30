#ifndef AMARA_MUSIC
#define AMARA_MUSIC

#include "amara.h"

namespace Amara {
    class Music: public Amara::AudioBase {
        public:
            Amara::GameProperties* properties = nullptr;
            Mix_Music* music = nullptr;
            int channel = -1;

            Music(std::string givenKey, AssetType givenType, Mix_Music* givenAsset, Amara::GameProperties* gameProperties): Amara::AudioBase(givenKey, givenType, givenAsset) {
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
						if (properties->music != nullptr) {
							properties->music->stop();
						}
					}
				}
				Mix_PlayMusic(music, loops);
				properties->music = this;
				isPaused = false;
				isPlaying = true;
			}

			void play() {
				play(0);
			}

			void pause() {
				if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
					if (properties->music == this) {
						Mix_PauseMusic();
						isPaused = true;
						isPlaying = false;
					}
				}
			}

			void resume() {
				if (Mix_PlayingMusic() && Mix_PausedMusic()) {
					if (properties->music == this) {
						Mix_ResumeMusic();
						isPaused = false;
						isPlaying = true;
					}
				}
			}

			bool finished() {

			}

			void stop() {
				if (Mix_PlayingMusic()) {
					if (properties->music == this) {
						Mix_HaltMusic();
						isPaused = false;
						isPlaying = false;
					}
				}
			}

            void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

                if (Mix_PlayingMusic() && properties->music == this) {
					Mix_VolumeMusic(floor(volume * masterVolume * parentVolume * 128));
				}
				else {
					isPlaying = false;
				}
            }
    };
}

#endif