#pragma once
#ifndef AMARA_SOUND
#define AMARA_SOUND



namespace Amara {
    class Sound: public Amara::AudioBase {
        public:
            Mix_Chunk* sound = nullptr;
			int channel = -1;

            Sound(std::string givenKey, AssetType givenType, Mix_Chunk* givenAsset): Amara::AudioBase(givenKey, givenType) {
				sound = givenAsset;
				defaultLoops = 0;
            }

			virtual Amara::AudioBase* play(int loops) {
				channel = Mix_PlayChannel(-1, sound, loops);
				if (channel == -1) {
					std::cout << "Error playing sound: \"" << key << "\"" << std::endl;
					return this;
				}
				Mix_Volume(channel, floor(volume * masterVolume * 128));
				isPaused = false;
				isPlaying = true;

				return this;
			}

			virtual Amara::AudioBase* play() {
				return play(defaultLoops);
			}

			virtual Amara::AudioBase* stop() {
				if (isPlaying && channel != -1) {
					Mix_HaltChannel(channel);
					channel = -1;
					isPlaying = false;
					isPaused = false;

					if (parent && parent->currentlyPlaying == this) {
						parent->currentlyPlaying = nullptr;
					}
				}

				return this;
			}

			virtual void chain(std::string nextKey) {
                Amara::AudioBase::chain(nextKey);
				defaultLoops = 0;
            }

			virtual void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

				if (channel != -1) {
					if (Mix_Playing(channel)) Mix_Volume(channel, floor(volume * masterVolume * parentVolume * 128));
					else {
						isPlaying = false;
						channel = -1;
						if (parent && parent->currentlyPlaying == this) {
							parent->currentlyPlaying = nullptr;
							if (!nextInChain.empty()) {
								getRootAudio()->play(nextInChain);
							}
						}
					}
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
