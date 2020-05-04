#pragma once
#ifndef AMARA_LTIMER
#define AMARA_LTIMER

namespace Amara {
    /*
	 * The application time based timer
	 */
	class LTimer {
		public:
			LTimer() {
				mStartTicks = 0;
				mPausedTicks = 0;

				mPaused = false;
				mStarted = false;
			}

			void start() {
				mStarted = true;
				mPaused = false;

				mStartTicks = SDL_GetTicks();
				mPausedTicks = 0;
			}

			void stop() {
				mStarted = false;
				mPaused = false;

				mStartTicks = 0;
				mPausedTicks = 0;
			}

			void pause() {
				if (mStarted && !mPaused) {
					mPaused = true;

					mPausedTicks = SDL_GetTicks() - mStartTicks;
					mStartTicks = 0;
				}
			}

			void unpause() {
				if (mStarted && mPaused) {
					mPaused = false;

					mStartTicks = SDL_GetTicks() - mPausedTicks;

					mPausedTicks = 0;
				}
			}

			Uint32 getTicks() {
				Uint32 time = 0;

				if (mStarted) {
					if (mPaused) {
						time = mPausedTicks;
					}
					else {
						time = SDL_GetTicks() - mStartTicks;
					}
				}

				return time;
			}

			bool isStarted() {
				return mStarted;
			}

			bool isPaused() {
				return mPaused && mStarted;
			}

		private:
			// Clock time when the timer started
			Uint32 mStartTicks;
			// The ticks stored when the timer was paused
			Uint32 mPausedTicks;
			// the timer statues
			bool mPaused;
			bool mStarted;
	};
}
#endif