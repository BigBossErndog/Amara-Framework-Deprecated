#pragma once
#ifndef AMARA_SOUND
#define AMARA_SOUND



namespace Amara {
    class Sound: public Amara::AudioBase {
        public:
            Mix_Chunk* sound = nullptr;
			int channel = -1;

            Sound(std::string givenKey, AssetType givenType, Mix_Chunk* givenAsset): Amara::AudioBase(givenKey, givenType, givenAsset) {
				sound = givenAsset;
            }

			virtual void play(bool loops) {
				channel = Mix_PlayChannel(-1, sound, loops);
				if (channel == -1) {
					std::cout << "Error playing sound: \"" << key << "\"" << std::endl;
					return;
				}
				Mix_Volume(channel, floor(volume * masterVolume * 128));
				isPaused = false;
				isPlaying = true;
			}

			virtual void play() {
				play(false);
			}

			virtual void stop() {
				if (isPlaying && channel != -1) {
					Mix_HaltChannel(channel);
					channel = -1;
					isPlaying = false;
					isPaused = false;

					if (parent && parent->currentlyPlaying == this) {
						parent->currentlyPlaying = nullptr;
					}
				}
			}

			virtual void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

				if (channel != -1 && Mix_Playing(channel)) {
					Mix_Volume(channel, floor(volume * masterVolume * parentVolume * 128));
				}
				else {
					isPlaying = false;
					channel = -1;
					if (parent && parent->currentlyPlaying == this) {
						parent->currentlyPlaying = nullptr;
					}
				}
			}
    };
}

#endif
