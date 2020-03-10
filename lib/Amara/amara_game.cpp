#ifndef AMARA_GAME
#define AMARA_GAME

#include "amara.h"

namespace Amara {
	class Game {
		public:
			SDL_version compiledVersion;
            SDL_version linkedVersion;

			Amara::GameProperties* properties;

			string name;
			bool quitted = false;
			bool dragged = false;

			SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

			int width = 0;
			int height = 0;

			Amara::IntRect* display = nullptr;
			Amara::IntRect* resolution = nullptr;
			Amara::IntRect* window = nullptr;

			bool lagging = false;
			int lagCounter = 0;

			bool windowFocused = false;
			bool isFullscreen = false;

			Amara::Loader* load = nullptr;
			Amara::AssetManager* assets = nullptr;
			Amara::SceneManager* scenes = nullptr;

			Amara::InputManager* input = nullptr;
			Amara::ControlScheme* controls = nullptr;
			bool controllerEnabled = true;

			Amara::EventManager* events = nullptr;


			Amara::TaskManager* taskManager = nullptr;

			bool vsync = false;
			int fps = 60;
			int tps = 1000 / fps;
			int lps = 60;
			LTimer fpsTimer;
			LTimer capTimer;

			int frameCounter = 0;
			int logicDelay = 0;

			SDL_Event e;

			Game(string givenName) {
				name = givenName;

				properties = new Amara::GameProperties();
				properties->game = this;
			}

			bool init(int startWidth, int startHeight) {
				width = startWidth;
				height = startHeight;

				// Getting version of SDL2
				SDL_VERSION(&compiledVersion);
           		SDL_GetVersion(&linkedVersion);

				printf("Compiled against SDL version %d.%d.%d ...\n",
                compiledVersion.major, compiledVersion.minor, compiledVersion.patch);
            	printf("Linking against SDL version %d.%d.%d.\n",
                linkedVersion.major, linkedVersion.minor, linkedVersion.patch);

				// Creating the video context
				if (SDL_Init(SDL_INIT_VIDEO) < 0) {
					printf("Game Error: Failed to initialize Video.");
					return false;
				}

				// Creating the audio context
				if (SDL_Init(SDL_INIT_AUDIO) < 0) {
					printf("Game Error: Failed to initialize Audio.");
					return false;
				}

				// Setting up controllers
				if (controllerEnabled && SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
					printf("Game Error: Failed to initialize Game Controller.");
					return false;
				}

				// Creating the window
				gWindow = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
				if (gWindow == NULL) {
					printf("Window could not be created. Game Error: %s\n", SDL_GetError());
					return false;
				}
				properties->gWindow = gWindow;

				// Get window surface
				gSurface = SDL_GetWindowSurface(gWindow);
				properties->gSurface = gSurface;

				// Fill the surface black
				// Background color
				SDL_FillRect(gSurface, NULL, SDL_MapRGB(gSurface->format, 0, 0, 0));

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);

				// Setting up the Renderer
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL) {
					printf("Game Error: Renderer failed to start. SDL Error: %s\n", SDL_GetError());
					return false;
				}
				properties->gRenderer = gRenderer;

				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("Game Error: Could not load assets. SDL_image Error: %s\n", IMG_GetError());
					return false;
				}

				 //Initialize SDL_ttf
				 if(TTF_Init() == -1) {
					 printf( "Game Error: SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					 return false;
				}

				//Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
					printf("Game Error: SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
					return false;
				}

				SDL_DisplayMode dm;
				if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
					printf("Game Error: Unable to detect display. Error: %s\n", SDL_GetError());
					return false;
				}

				// Setting game logical size
				SDL_RenderSetLogicalSize(gRenderer, width, height);

				display = new Amara::IntRect(0, 0, dm.w, dm.h);
				resolution = new Amara::IntRect(0, 0, width, height);
				window = new Amara::IntRect(0, 0, width, height);
				SDL_GetWindowPosition(gWindow, &window->x, &window->y);
				// printf("Game Info: Display width: %d, Display height: %d\n", dm.w, dm.h);

				load = new Amara::Loader(properties);
				properties->loader = load;

				assets = new Amara::AssetManager(properties);
				properties->assets = assets;

				input = new Amara::InputManager();
				input->keyboard = new Amara::Keyboard(properties);
				input->mouse = new Amara::Mouse(properties);
				properties->input = input;

				controls = new Amara::ControlScheme(properties);
				properties->controls = controls;

				scenes = new Amara::SceneManager(properties);
				properties->scenes = scenes;

				taskManager = new Amara::TaskManager(properties);
				properties->taskManager = taskManager;

				events = new Amara::EventManager(properties);
				properties->events = events;

				writeProperties();

				resizeWindow(width, height);
			}

			// For when the player closes the game
			void close() {
				// Quit Game
				quitted = true;

				//Destroy window
				SDL_FreeSurface(gSurface);
				gSurface = NULL;

				SDL_DestroyRenderer(gRenderer);
				SDL_DestroyWindow(gWindow);

				//Quit SDL subsystems
				Mix_CloseAudio();
				Mix_Quit();
				TTF_Quit();
				IMG_Quit();
				SDL_Quit();
			}

			void start() {
				// Game Loop
				while (!quitted) {
					manageFPSStart();

					writeProperties();

					// Draw Screen
					draw();

					// Manage frame catch up and slow down
					manageFPSEnd();
				}
				close();
			}

			void start(string startKey) {
				// Start a specific scene
				scenes->start(startKey);
				start();
			}

			void setFPS(int newFps, bool lockLogicSpeed) {
				fps = newFps;
				properties->fps = fps;
				tps = 1000 / fps;
				if (!lockLogicSpeed) {
					lps = newFps;
				}
			}

			void setFPS(int newFps) {
				setFPS(newFps, false);
			}

			void setFPS(int newFps, int newLps) {
				fps = newFps;
				lps = newLps;
				tps = 1000 / fps;
			}

			void setLogicTickRate(int newRate) {
				lps = newRate;
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
				SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
			}

			void resizeWindow(int neww, int newh) {
				if (gWindow != NULL) {
					SDL_SetWindowSize(gWindow, neww, newh);
					window->width = neww;
					window->height = newh;
				}
			}

			void setWindowPosition(int newx, int newy) {
				if (gWindow != NULL) {
					SDL_SetWindowPosition(gWindow, newx, newy);
					window->x = newx;
					window->y = newy;
				}
			}

			void setResolution(int neww, int newh) {
				if (gRenderer != NULL) {
					SDL_RenderSetLogicalSize(gRenderer, neww, newh);
				}
				resolution->width = neww;
				resolution->height = newh;
				width = neww;
				height = newh;
				writeProperties();
			}

			void resizeWindowAndResolution(int neww, int newh) {
				resizeWindow(neww, newh);
				setResolution(neww, newh);
			}

			void startFullscreen() {
				SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);
				isFullscreen = true;
			}

			void startWindowedFullscreen() {
				SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				isFullscreen = true;
			}

			void exitFullscreen() {
				SDL_SetWindowFullscreen(gWindow, 0);
				isFullscreen = false;
			}

		protected:

			void writeProperties() {
				properties->gWindow = gWindow;
				properties->gSurface = gSurface;
				properties->gRenderer = gRenderer;

				properties->width = width;
				properties->height = height;

				properties->display = display;
				properties->resolution = resolution;
				properties->window = window;

				properties->isFullscreen = isFullscreen;

				properties->loader = load;
				properties->assets = assets;
				properties->scenes = scenes;

				properties->input = input;
				properties->controls = controls;
				properties->taskManager = taskManager;

				properties->events = events;

				properties->lagging = lagging;
				properties->dragged = dragged;

				properties->fps = fps;
			}

			void draw() {
				// Clear the Renderer
				SDL_RenderClear(gRenderer);

				if (frameCounter >= logicDelay) {
					handleEvents();
					scenes->run();
					frameCounter = 0;
				}
				frameCounter += 1;

				scenes->draw();

				/// Draw to renderer
				SDL_RenderPresent(gRenderer);

				taskManager->run();
			}

			void manageFPSStart() {
				capTimer.start();
			}

			void manageFPSEnd() {
				// Check if frame finished early
				if (quitted) return;
				int totalWait = 0;
				logicDelay = 0;
				lagging = false;

				if (fps < lps) {
					for (int i = 1; i < lps/fps; i++) {
						handleEvents();
						scenes->run();
					}
				}
				else if (fps > lps) {
					logicDelay = fps/lps;
				}

				int frameTicks = capTimer.getTicks();
				if (frameTicks < tps) {
					// Wait remaining time
					totalWait += (tps - frameTicks);
				}
				// else if (frameTicks > tps) {
				// 	if (dragged) {
				// 		dragged = false;
				// 		properties->dragged = false;
				// 	}
				// 	else {
				// 		// Checking for lag
				// 		if (tps < (float)frameTicks * 0.5) {
				// 			lagging = true;
				// 			lagCounter += 1;
				// 		}

				// 		// Framerate catch up.
				// 		for (int i = 0; i < (frameTicks - tps); i++) {
				// 			if (frameCounter >= logicDelay) {
				// 				handleEvents();
				// 				scenes->run();
				// 				frameCounter = 0;
				// 				if (quitted) return;
				// 			}
				// 			frameCounter += 1;
				// 		}
				// 	}
				// }

				// Delay if game has not caught up
				if (totalWait > 0) {
					SDL_Delay(totalWait);
				}
			}

			void handleEvents() {
				// Handle events on queue
				input->keyboard->manage();
				input->mouse->manage();

				// manageControllers();

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quitted = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						input->keyboard->press(e.key.keysym.sym);
					}
					else if (e.type == SDL_KEYUP) {
						input->keyboard->release(e.key.keysym.sym);
					}
					else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						input->mouse->x = (mx * resolution->width/properties->width);
						input->mouse->y = (my * resolution->height/properties->height);

						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								input->mouse->left->press();
								events->addEvent(SCENELEFTCLICK);
								events->addEvent(OBJECTLEFTCLICK);
							}
							else if (e.button.button == SDL_BUTTON_RIGHT) {
								input->mouse->right->press();
								events->addEvent(SCENERIGHTCLICK);
								events->addEvent(OBJECTRIGHTCLICK);
							}
						}
						else if (e.type == SDL_MOUSEBUTTONUP) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								input->mouse->left->release();
								events->addEvent(SCENELEFTRELEASE);
								events->addEvent(OBJECTLEFTRELEASE);
							}
							else if (e.button.button == SDL_BUTTON_RIGHT) {
								input->mouse->right->release();
								events->addEvent(SCENERIGHTRELEASE);
								events->addEvent(OBJECTRIGHTRELEASE);
							}
						}
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_MOVED)) {
						dragged = true;
						properties->dragged = true;
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_LEAVE)) {
						windowFocused = false;
						properties->windowFocused = false;
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_ENTER)) {
						windowFocused = true;
						properties->windowFocused = true;
					}
					// else if (e.type == SDL_CONTROLLERDEVICEADDED) {
					// 	SDL_GameController* nsdlc = SDL_GameControllerOpen(e.cdevice.which);

					// 	Amara::Controller* newcontroller = new Amara::Controller(nsdlc);
					// 	printf("Game Info: Controller connected, Name: %s\n", SDL_GameControllerName(nsdlc));
					// 	controllers->push(newcontroller);
					// }
					// else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
					// 	SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
					// 	getController(controller)->press(e.cbutton.button);
					// }
					// else if (e.type == SDL_CONTROLLERBUTTONUP) {
					// 	SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
					// 	getController(controller)->release(e.cbutton.button);
					// }
				}

				controls->run();
				scenes->manageTasks();
			}
	};
}

#endif
