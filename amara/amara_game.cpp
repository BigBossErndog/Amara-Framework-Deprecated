namespace Amara {
	class Game: public Amara::DataObject {
		public:
			SDL_version compiledVersion;
            SDL_version linkedVersion;

			Amara::GameProperties properties;

			RNG rng;

			Amara::MessageQueue messages;

			std::string name;
			bool quit = false;

			SDL_Window* gWindow = NULL;
			SDL_Renderer* gRenderer = NULL;

			int width = 0;
			int height = 0;

			int displayIndex = 0;
			Amara::IntRect display;
			Amara::IntRect resolution;
			Amara::IntRect window;

			bool lagging = false;
			int lagCounter = 0;

			bool windowMoved = false;
			bool windowFocused = false;
			bool isFullscreen = false;
			bool isWindowed = true;

			bool renderTargetsReset = false;
			bool renderDeviceReset = false;

			Amara::Loader load;
			Amara::AssetManager assets;
			Amara::SceneManager scenes;

			Amara::InputManager input;
			Amara::ControlScheme controls;
			bool controllerEnabled = true;

			SDL_Color backgroundColor = {255, 255, 255, 255};

			Amara::AudioGroup audio;
			Amara::EventManager events;
			Amara::TaskManager taskManager;

			Amara::FileManager files;
			
			int fps = 60;
			int tps = 1000 / fps;
			int lps = fps;
			double realFPS = fps;
			double deltaTime = 1;
			LTimer capTimer;
			
			int frameCounter = 0;
			int logicDelay = 0;

			bool reloadAssets = false;

			SDL_Event e;

			bool testing = true;
			bool hardwareRendering = true;

			Game() {}

			Game(std::string givenName, bool gRendering) {
				name = givenName;

				properties = Amara::GameProperties();
				properties.game = this;

				hardwareRendering = gRendering;
			}

			Game(std::string givenName): Game(givenName, true) {}

			bool init(int startWidth, int startHeight) {
				SDL_SetMainReady();

				width = startWidth;
				height = startHeight;

				// Getting version of SDL2
				SDL_VERSION(&compiledVersion);
           		SDL_GetVersion(&linkedVersion);

				SDL_Log("Compiled against SDL version %d.%d.%d ...\n",
                compiledVersion.major, compiledVersion.minor, compiledVersion.patch);
            	SDL_Log("Linking against SDL version %d.%d.%d.\n",
                linkedVersion.major, linkedVersion.minor, linkedVersion.patch);
				
				// Creating the video context
				if (SDL_Init(SDL_INIT_VIDEO) < 0) {
					SDL_Log("Game Error: Failed to initialize Video.");
					return false;
				}

				// Creating the audio context
				if (SDL_Init(SDL_INIT_AUDIO) < 0) {
					SDL_Log("Game Error: Failed to initialize Audio.");
					return false;
				}

				// Setting up joy sticks
				if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
					SDL_Log("Game Error: Failed to initialize Joystick.");
				}

				// Setting up controllers
				if (controllerEnabled && SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
					SDL_Log("Game Error: Failed to initialize Game Controller.");
				}

				// Creating the window
				gWindow = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
				if (gWindow == NULL) {
					SDL_Log("Window could not be created. Game Error: %s\n", SDL_GetError());
					return false;
				}
				properties.gWindow = gWindow;

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);

				// Setting up the Renderer
				if (hardwareRendering) {
					gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
					SDL_Log("Started on Hardware Accelerated Rendering.");
				}
				else {
					gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
					SDL_Log("Started on Software Rendering.");
				}
				if (gRenderer == NULL) {
					SDL_Log("Game Error: Renderer failed to start. SDL Error: %s\n", SDL_GetError());
					return false;
				}
				properties.gRenderer = gRenderer;

				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					SDL_Log("Game Error: Could not load assets. SDL_image Error: %s\n", IMG_GetError());
					return false;
				}

				 //Initialize SDL_ttf
				 if(TTF_Init() == -1) {
					 SDL_Log( "Game Error: SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					 return false;
				}

				//Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
					SDL_Log("Game Error: SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
					return false;
				}

				SDL_DisplayMode dm;
				if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
					SDL_Log("Game Error: Unable to detect display. Error: %s\n", SDL_GetError());
					return false;
				}

				// Setting game logical size
				SDL_RenderSetLogicalSize(gRenderer, width, height);
				
				displayIndex = SDL_GetWindowDisplayIndex(gWindow);
				SDL_Rect displayRect;
				SDL_GetDisplayBounds(displayIndex, &displayRect);
				display = Amara::IntRect{ displayRect.x, displayRect.y, displayRect.w, displayRect.h };
				properties.display = &display;

				resolution = Amara::IntRect{ 0, 0, width, height };
				properties.resolution = &resolution;

				window = Amara::IntRect{ 0, 0, width, height };
				properties.window = &window;

				// Update window position.
				SDL_GetWindowPosition(gWindow, &window.x, &window.y);

				load = Amara::Loader(&properties);
				properties.loader = &load;

				assets = Amara::AssetManager(&properties);
				properties.assets = &assets;

				input = Amara::InputManager(&properties);
				properties.input = &input;

				messages = MessageQueue(&properties);
				messages.clear();

				properties.messages = &messages;

				files = FileManager();

				data.clear();
				rng.randomize();


				// Check connected gamepads
				for (int i = 0; i < SDL_NumJoysticks(); i++) {
					if (SDL_IsGameController(i)) {
						SDL_GameController* controller = SDL_GameControllerOpen(i);
						input.gamepads.connectGamepad(controller);
					}
				}

				controls = Amara::ControlScheme(&properties);
				properties.controls = &controls;

				scenes = Amara::SceneManager(&properties);
				properties.scenes = &scenes;

				taskManager = Amara::TaskManager(&properties);
				properties.taskManager = &taskManager;

				events = Amara::EventManager(&properties);
				properties.events = &events;

				audio = Amara::AudioGroup(&properties, "root");
				properties.audio = &audio;
				audio.rootGroup = true;

				properties.rng = &rng;

				writeProperties();

				setWindowSize(width, height);

				return true;
			}

			// For when the player closes the game
			void close() {
				// Quit Game
				quit = true;

				//Quit SDL subsystems
				Mix_CloseAudio();
				Mix_Quit();
				TTF_Quit();
				IMG_Quit();

				SDL_DestroyRenderer(gRenderer);
				SDL_DestroyWindow(gWindow);

				SDL_Quit();
			}

			void start() {
				// Game Loop
				while (!quit) {
					gameLoop();
				}
				close();
			}

			void start(std::string startKey) {
				// Start a specific scene
				scenes.start(startKey);
				start();
			}

			void gameLoop() {
				renderTargetsReset = false;
				renderDeviceReset = false;
				manageFPSStart();
				writeProperties();
				// Draw Screen
				draw();
				// Manage frame catch up and slow down
				manageFPSEnd();
				taskManager.run();
				if (renderTargetsReset || renderDeviceReset) {
					reloadAssets = true;
				}
				else if (reloadAssets && (isWindowed || windowFocused)) {
					reloadAssets = false;
					load.regenerateAssets();
				}
			}

			void setFPS(int newFps, bool lockLogicSpeed) {
				fps = newFps;
				properties.fps = fps;
				tps = 1000 / fps;
				if (!lockLogicSpeed) {
					lps = newFps;
					properties.lps = lps;
				}
			}

			void setFPS(int newFps) {
				setFPS(newFps, false);
			}

			void setFPS(int newFps, int newLps) {
				fps = newFps;
				lps = newLps;
				tps = 1000 / fps;
				properties.fps = fps;
				properties.lps = lps;
			}

			void setLogicTickRate(int newRate) {
				lps = newRate;
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				backgroundColor.r = r;
				backgroundColor.g = g;
				backgroundColor.b = b;
				backgroundColor.a = a;
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
				setBackgroundColor(r, g, b, 255);
			}

			void setWindowSize(int neww, int newh) {
				if (gWindow != NULL) {
					SDL_SetWindowSize(gWindow, neww, newh);
					window.width = neww;
					window.height = newh;
				}
			}

			void setWindowPosition(int newx, int newy) {
				if (gWindow != NULL) {
					SDL_SetWindowPosition(gWindow, newx, newy);
					window.x = newx;
					window.y = newy;
				}
			}

			void setWindowRelativePosition(int newx, int newy) {
				int displayIndex = SDL_GetWindowDisplayIndex(gWindow);
				SDL_Rect rect;
				SDL_GetDisplayBounds(displayIndex, &rect);
				setWindowPosition(rect.x + newx, rect.y + newy);
			}

			void centerWindowPosition() {
				int displayIndex = SDL_GetWindowDisplayIndex(gWindow);
				SDL_Rect rect;
				SDL_GetDisplayBounds(displayIndex, &rect);
				setWindowPosition(
					rect.x + rect.w/2.0 - window.width/2.0, 
					rect.y + rect.h/2.0 - window.height/2.0
				);
			}

			void setResolution(int neww, int newh) {
				if (gRenderer != NULL) {
					SDL_RenderSetLogicalSize(gRenderer, neww, newh);
				}
				resolution.width = neww;
				resolution.height = newh;
				width = neww;
				height = newh;
				writeProperties();
			}

			void setWindowAndResolution(int neww, int newh) {
				setWindowSize(neww, newh);
				setResolution(neww, newh);
			}

			void startFullscreen() {
				SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);
				isFullscreen = true;
				isWindowed = false;
			}

			void startWindowedFullscreen() {
				SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				isFullscreen = true;
				isWindowed = true;
			}

			void exitFullscreen() {
				SDL_SetWindowFullscreen(gWindow, 0);
				isFullscreen = false;
				isWindowed = true;
			}

			void setWindowIcon(SDL_Surface* sf) {
				SDL_SetWindowIcon(gWindow, sf);
			}
			void setWindowIcon(std::string key) {
				setWindowIcon(assets.getSurface(key));
			}

			void writeProperties() {
				properties.gWindow = gWindow;
				properties.gRenderer = gRenderer;

				properties.testing = testing;

				properties.width = width;
				properties.height = height;

				properties.isFullscreen = isFullscreen;
				properties.isWindowed = isWindowed;

				properties.renderTargetsReset = renderTargetsReset;
				properties.renderDeviceReset = renderDeviceReset;

				properties.lagging = lagging;
				properties.windowMoved = windowMoved;
				properties.windowFocused = windowFocused;

				properties.fps = fps;
				properties.lps = lps;
				properties.realFPS = realFPS;
				properties.deltaTime = deltaTime;

				properties.backgroundColor = backgroundColor;
			}

			void update() {
				if (quit) return;
				handleEvents();
				writeProperties();
				if (quit) return;
				messages.update();
				events.manage();
				scenes.run();
				scenes.manageTasks();
				audio.run(1);
			}

			void draw() {
				// Clear the Renderer
				SDL_SetRenderDrawColor(gRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
				SDL_RenderClear(gRenderer);

				if (frameCounter >= logicDelay) {
					update();
					frameCounter = 0;
				}
				frameCounter += 1;
				if (isWindowed || windowFocused) scenes.draw();
				events.manageInteracts();
				/// Draw to renderer
				SDL_RenderPresent(gRenderer);
			}

			void manageFPSStart() {
				capTimer.start();
			}

			void manageFPSEnd() {
				// Check if frame finished early
				if (quit) return;
				int totalWait = 0;
				logicDelay = 0;
				lagging = false;

				if (fps < lps) {
					for (int i = 1; i < lps/fps; i++) {
						update();
					}
				}
				else if (fps > lps) {
					logicDelay = floor(fps/(float)lps);
				}

				int frameTicks = capTimer.getTicks();
				if (frameTicks < tps) {
					// Wait remaining time
					totalWait += (tps - frameTicks);
				}
				realFPS = fps / (frameTicks / 1000.f);
				deltaTime = fps / realFPS;

				// Delay if game has not caught up
				if (totalWait > 0) {
					SDL_Delay(totalWait);
				}
			}

			void handleEvents() {
				// Handle events on queue
				input.keyboard.manage();
				input.mouse.manage();
				input.gamepads.manage();
				input.touches.manage();

				windowMoved = false;

				// manageControllers();

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
						properties.quit = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						input.keyboard.press(e.key.keysym.sym);
						input.keyboard.isActivated = true;
					}
					else if (e.type == SDL_KEYUP) {
						input.keyboard.release(e.key.keysym.sym);
						input.keyboard.isActivated = true;
					}
					else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						input.mouse.dx = (mx * (float)resolution.width/(float)window.width);
						input.mouse.dy = (my * (float)resolution.height/(float)window.height);

						input.mouse.x = input.mouse.dx;
						input.mouse.y = input.mouse.dy;

						float offset, upScale;
						int vx, vy = 0;
						float ratioRes = ((float)resolution.width / (float)resolution.height);
						float ratioWin = ((float)window.width / (float)window.height);

						input.mouse.isActivated = true;
						if (e.type == SDL_MOUSEMOTION) input.mouse.moved = true;

						if (ratioRes < ratioWin) {
							upScale = ((float)window.height/(float)resolution.height);
							offset = ((float)window.width - ((float)resolution.width * upScale))/2;
							input.mouse.dx = mx/upScale;
							input.mouse.x = (mx - offset)/upScale;
						}
						else if (ratioRes > ratioWin) {
							upScale = ((float)window.width/(float)resolution.width);
							offset = ((float)window.height - ((float)resolution.height * upScale))/2;
							input.mouse.dy = my/upScale;
							input.mouse.y = (my - offset)/upScale;
						}

						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								input.mouse.left.press();
							}
							else if (e.button.button == SDL_BUTTON_RIGHT) {
								input.mouse.right.press();
							}
							else if (e.button.button == SDL_BUTTON_MIDDLE) {
								input.mouse.middle.press();
							}
						}
						else if (e.type == SDL_MOUSEBUTTONUP) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								input.mouse.left.release();
							}
							else if (e.button.button == SDL_BUTTON_RIGHT) {
								input.mouse.right.release();
							}
							else if (e.button.button == SDL_BUTTON_MIDDLE) {
								input.mouse.middle.release();
							}
						}
					}
					else if (e.type == SDL_MOUSEWHEEL) {
						int mul = (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -1 : 1;
						input.mouse.scrollX = e.wheel.x * mul;
						input.mouse.scrollY = e.wheel.y * mul;

						input.mouse.isActivated = true;
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_MOVED)) {
						windowMoved = true;
						properties.windowMoved = true;
						window.x = e.window.data1;
						window.y = e.window.data2;

						displayIndex = SDL_GetWindowDisplayIndex(gWindow);
						SDL_Rect displayRect;
						SDL_GetDisplayBounds(displayIndex, &displayRect);
						display = Amara::IntRect{ displayRect.x, displayRect.y, displayRect.w, displayRect.h };
					}
					else if (e.type == SDL_RENDER_TARGETS_RESET) {
						renderTargetsReset = true;
					}
					else if (e.type == SDL_RENDER_DEVICE_RESET) {
						renderDeviceReset = true;
					}
					else if (e.type == SDL_CONTROLLERDEVICEADDED) {
						SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
						input.gamepads.connectGamepad(controller);
					}
					else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
						input.gamepads.disconnectGamepad(controller);
					}
					else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
						Amara::Gamepad* gamepad = input.gamepads.get(controller);
						if (gamepad != nullptr) gamepad->press(e.cbutton.button);
						input.gamepads.isActivated = true;
					}
					else if (e.type == SDL_CONTROLLERBUTTONUP) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
						Amara::Gamepad* gamepad = input.gamepads.get(controller);
						if (gamepad != nullptr) gamepad->release(e.cbutton.button);
						input.gamepads.isActivated = true;
					}
					else if (e.type == SDL_CONTROLLERAXISMOTION) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.caxis.which);
						Amara::Gamepad* gamepad = input.gamepads.get(controller);
						if (gamepad != nullptr) gamepad->push(e.caxis.axis, e.caxis.value);
						input.gamepads.isActivated = true;
					}
					if (e.type == SDL_FINGERDOWN) {
						Amara::TouchPointer* pointer = input.touches.newPointer(e.tfinger.fingerId);
						if (pointer) {
							pointer->press();
							pointer->virtualizeXY(e);
						}

						input.touches.isActivated = true;
					}
					else if (e.type == SDL_FINGERMOTION) {
						Amara::TouchPointer* pointer = input.touches.getPointer(e.tfinger.fingerId);
						if (pointer) {
							pointer->virtualizeXY(e);
						}

						input.touches.isActivated = true;
					}
					else if (e.type == SDL_FINGERUP) {
						Amara::TouchPointer* pointer = input.touches.getPointer(e.tfinger.fingerId);
						if (pointer) {
							pointer->release();
							pointer->virtualizeXY(e);

							input.touches.removePointer(pointer->id);
						}

						input.touches.isActivated = true;
					}
					else if( e.type == SDL_TEXTINPUT ) {
                        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ) {
                            input.inputText += e.text.text;
                        }
                    }
				}

				controls.run();
				input.mouse.afterManage();
				
				input.mode = InputMode_None;
				if (input.keyboard.isActivated) {
					input.mode |= InputMode_Keyboard;
					input.lastMode = InputMode_Keyboard;
				}
				if (input.mouse.isActivated) {
					input.mode |= InputMode_Mouse;
					input.lastMode = InputMode_Mouse;
				}
				if (input.gamepads.isActivated) {
					input.mode |= InputMode_Gamepad;
					input.lastMode = InputMode_Gamepad;
				}
				if (input.touches.isActivated) {
					input.mode |= InputMode_Touch;
					input.lastMode = InputMode_Touch;
				}

				Uint32 winFlags = SDL_GetWindowFlags(gWindow);
				if (winFlags & SDL_WINDOW_MOUSE_FOCUS) {
					windowFocused = true;
				}
				else windowFocused = false;
			}
	};
}
