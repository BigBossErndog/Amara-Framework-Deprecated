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
				SDL_Log("Error playing sound: \"%s\"", key.c_str());
				return this;
			}
			Mix_Volume(channel, floor(volume * masterVolume * MIX_MAX_VOLUME));
			isPaused = false;
			isPlaying = true;

			return this;
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
				if (Mix_Playing(channel)) Mix_Volume(channel, floor(volume * masterVolume * parentVolume * MIX_MAX_VOLUME));
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

	class Music: public Amara::AudioBase {
	public:
		Amara::GameProperties* properties = nullptr;
		Mix_Music* music = nullptr;

		int loops = -1;

		bool forceStopped = false;

		double position = 0;

		static Music* currentlyPlayingMusic;

		Music(std::string givenKey, AssetType givenType, Mix_Music* givenAsset, Amara::GameProperties* gameProperties): Amara::AudioBase(givenKey, givenType) {
			properties = gameProperties;
			music = givenAsset;
		}

		Amara::AudioBase* play(int gLoops) {
			loops = gLoops;
			if (Mix_PlayingMusic()) {
				if (properties->music != nullptr && !properties->music->isPaused) {
					properties->music->stop();
				}
			}
			Mix_PlayMusic(music, loops);
			if (Mix_PausedMusic()) Mix_ResumeMusic();
			Mix_VolumeMusic(floor(volume * masterVolume * MIX_MAX_VOLUME));
			properties->music =  this;
			currentlyPlayingMusic = this;
			isPaused = false;
			isPlaying = true;

			if (parent) {
				parent->lastPlayed = this;
				parent->currentlyPlaying == this;
			}
			
			forceStopped = false;
			Mix_HookMusicFinished(&hookMusicStopped);

			return this;
		}

		Amara::AudioBase* pause() {
			if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
				if (properties->music == this) {
					Mix_PauseMusic();
					isPaused = true;
					isPlaying = false;
					position = Mix_GetMusicPosition(music);
				}
			}
			
			return this;
		}

		Amara::AudioBase* resume() {
			if (properties->music == this) {
				currentlyPlayingMusic = this;
				Mix_ResumeMusic();
			}
			else {
				Mix_PlayMusic(music, loops);
				Mix_SetMusicPosition(position);
				properties->music = this;
				currentlyPlayingMusic = this;
			}
			isPaused = false;
			isPlaying = true;
			forceStopped = false;

			return this;
		}

		virtual void chain(std::string nextKey) {
			Amara::AudioBase::chain(nextKey);
			defaultLoops = 0;
		}

		bool finished() {
			return !isPlaying;
		}

		Amara::AudioBase* stop() {
			if (Mix_PlayingMusic()) {
				if (properties->music == this) {
					Mix_HaltMusic();
					isPaused = false;
					isPlaying = false;
					forceStopped = true;
					position = 0;
					if (parent && parent->currentlyPlaying == this) {
						parent->currentlyPlaying = nullptr;
					}
					if (properties->music == this) {
						properties->music = nullptr;
					}
				}
			}
			return this;
		}

		void onFinish() {
			isPlaying = false;
			isPaused = false;
			if (parent && parent->currentlyPlaying == this) {
				parent->currentlyPlaying = nullptr;
			}
			if (properties->music == this) {
				properties->music = nullptr;
			}
			if (!forceStopped && !nextInChain.empty()) {
				Amara::AudioBase* nextMusic = getRootAudio()->play(nextInChain);
				transferFade(nextMusic);
			}
		}

		static void hookMusicStopped() {
			if (Music::currentlyPlayingMusic) {
				Music::currentlyPlayingMusic->onFinish();
			}
		}

		void run(float parentVolume) {
			Amara::AudioBase::run(parentVolume);

			if (properties->music == this) {
				if (Mix_PlayingMusic()) {
					Mix_VolumeMusic(floor(volume * masterVolume * parentVolume * MIX_MAX_VOLUME));
					position = Mix_GetMusicPosition(music);
				}
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

	Amara::Music* Amara::Music::currentlyPlayingMusic = nullptr;
}
