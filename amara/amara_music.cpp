#pragma once
#ifndef AMARA_MUSIC
#define AMARA_MUSIC



namespace Amara {
    class Music: public Amara::AudioBase {
        public:
            Amara::GameProperties* properties = nullptr;
            Mix_Music* music = nullptr;

            int channel = -1;
			int loops = -1;

			double position = 0;

            Music(std::string givenKey, AssetType givenType, Mix_Music* givenAsset, Amara::GameProperties* gameProperties): Amara::AudioBase(givenKey, givenType, givenAsset) {
                properties = gameProperties;
				music = givenAsset;
            }

			void play(int gLoops) {
				loops = gLoops;
				if (Mix_PlayingMusic()) {
					if (properties->music != nullptr && !properties->music->isPaused) {
						properties->music->stop();
					}
				}
				Mix_PlayMusic(music, loops);
				if (Mix_PausedMusic()) Mix_ResumeMusic();
				properties->music = this;
				isPaused = false;
				isPlaying = true;

				if (parent) {
					parent->lastPlayed = this;
					parent->currentlyPlaying == this;
				}
			}

			void play() {
				play(-1);
			}

			void pause() {
				if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
					if (properties->music == this) {
						Mix_PauseMusic();
						isPaused = true;
						isPlaying = false;
						position = Mix_GetMusicPosition(music);
					}
				}
			}

			void resume() {
				if (properties->music == this) {
					Mix_ResumeMusic();
				}
				else {
					Mix_PlayMusic(music, loops);
					Mix_SetMusicPosition(position);
					properties->music = this;
				}
				isPaused = false;
				isPlaying = true;
			}

			bool finished() {
				return !isPlaying;
			}

			void stop() {
				if (Mix_PlayingMusic()) {
					if (properties->music == this) {
						Mix_HaltMusic();
						isPaused = false;
						isPlaying = false;
						if (parent && parent->currentlyPlaying == this) {
							parent->currentlyPlaying = nullptr;
						}
					}
				}
			}

            void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

                if (Mix_PlayingMusic() && properties->music == this) {
					Mix_VolumeMusic(floor(volume * masterVolume * parentVolume * MIX_MAX_VOLUME));
					position = Mix_GetMusicPosition(music);
				}
				else {
					isPlaying = false;
					isPaused = false;
					if (parent && parent->currentlyPlaying == this) {
						parent->currentlyPlaying = nullptr;
					}
				}
            }
    };
}

#endif
