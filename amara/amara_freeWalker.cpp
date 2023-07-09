namespace Amara {
    class FreeWalker: public Amara::Sprite {
    public:
        Direction direction = Down;
        Direction walkDirection = NoDir;
        int walkDirections = 0;

        float movementSpeed = 1;
        float walkSpeed = 1;
        float runSpeed = 4;
		bool accumulative = false;

        bool isRunning = false;

        bool justFinishedWalking = true;

        bool controlsEnabled = true;

        Amara::PathFindingTask* pathTask = nullptr;
        Amara::PathTile currentPathTile;

        FreeWalker(int gx, int gy): Amara::Sprite(gx, gy) {}
        FreeWalker(int gx, int gy, std::string tKey): Amara::Sprite(gx, gy, tKey) {}
        FreeWalker(): Amara::Sprite() {}

        void init() {
            Amara::Sprite::init();
            entityType = "actor";
			justFinishedWalking = true;
        }

        virtual void configure(nlohmann::json config) {
            Amara::Sprite::configure(config);
            if (config.find("direction") != config.end()) {
                direction = config["direction"];
                face(direction);
            }
            if (config.find("walkSpeed") != config.end()) {
                walkSpeed = config["walkSpeed"];
            }
            if (config.find("runSpeed") != config.end()) {
                runSpeed = config["runSpeed"];
            }
            if (config.find("controlsEnabled") != config.end()) {
                controlsEnabled = config["controlsEnabled"];
            }
			if (config.find("accumulative") != config.end()) {
				accumulative = config["accumulative"];
			}
        }

        virtual nlohmann::json toData() {
            nlohmann::json config = Amara::Sprite::toData();
            config["direction"] = direction;
            config["walkSpeed"] = walkSpeed;
            config["runSpeed"] = runSpeed;
            config["controlsEnabled"] = controlsEnabled;
			config["accumulative"] = accumulative;
            return config;
        }

        bool isWalking() {
            return (bool)walkDirection; 
        }

        bool isBusy() {
            if (isWalking()) {
                return true;
            }
            return false;
        }

        virtual void face(Direction dir) {
            direction = dir;
            walkDirection = NoDir;
            walkDirections = 0;
        }

        virtual void walk(Direction dir, bool shouldRun) {
            if (dir == NoDir) {
                face(direction);
                return;
            }

            movementSpeed = shouldRun ? runSpeed : walkSpeed;
            isRunning = shouldRun;
			
            float ox, oy;
			bool stickMode = false;
			if (input->lastMode == InputMode_Gamepad) {
				Gamepad* gamepad = input->gamepads->get(0);
				Stick* stick = gamepad->getStick(BUTTON_LEFTSTICK);

				if (abs(stick->xvalue) > JOYSTICK_DEADZONE || abs(stick->yvalue) > JOYSTICK_DEADZONE) {
					ox = sin(stick->angle) * movementSpeed;
					oy = cos(stick->angle) * movementSpeed;
					stickMode = true;
				}
			}
			if (!stickMode) {
				ox = Amara::getOffsetX(dir) * movementSpeed;
				oy = Amara::getOffsetY(dir) * movementSpeed;
				if (abs(ox) && abs(oy)) {
					ox = ox/abs(ox) * sqrt(pow(abs(movementSpeed),2)/2);
					oy = oy/abs(oy) * sqrt(pow(abs(movementSpeed),2)/2);
				}
			}
            
            if (physics) {
				if (accumulative) {
					physics->velocityX += ox;
					physics->velocityY += oy;
				}
				else {
					physics->velocityX = ox;
					physics->velocityY = oy;
				}
            }
            else {
                x += ox;
                y += oy;
            }

            walkDirection = dir;
            walkDirections |= dir;
            switch (dir) {
                case UpLeft:
                    walkDirections |= Up;
                    walkDirections |= Left;
                    break;
                case UpRight:
                    walkDirections |= Up;
                    walkDirections |= Right;
                    break;
                case DownLeft:
                    walkDirections |= Down;
                    walkDirections |= Left;
                    break;
                case DownRight:
                    walkDirections |= Down;
                    walkDirections |= Right;
                    break;
            }
            direction = dir;

			justFinishedWalking = false;
        }

        virtual void walk(Direction dir) {
            walk(dir, false);
        }

        virtual void run(Direction dir) {
            walk(dir, true);
        }

        virtual bool walkTo(int gx, int gy, bool shouldRun) {
            Direction toWalk = NoDir;

            movementSpeed = shouldRun ? runSpeed : walkSpeed;
            isRunning = shouldRun;

            if (abs(gx - x) < movementSpeed) {
                x = gx;
            }
            else {
                if (x < gx) {
                    toWalk = Right;
                }
                else {
                    toWalk = Left;
                }
            }

            if (abs(gy - y) < movementSpeed) {
                y = gy;
            }
            else {
                if (y < gy) {
                    if (toWalk == Right) toWalk = DownRight;
                    else if (toWalk == Left) toWalk = DownLeft;
                    else toWalk = Down;
                }
                else {
                    if (toWalk == Right) toWalk = UpRight;
                    else if (toWalk == Left) toWalk = UpLeft;
                    else toWalk = Up;
                }
            }

            walk(toWalk);

            return (toWalk == NoDir);
        }
        virtual bool walkTo(int gx, int gy) {
            return walkTo(gx, gy, false);
        }
        virtual bool runTo(int gx, int gy) {
            return walkTo(gx, gy, true);
        }

        void run() {
            if (controlsEnabled) {
                movementSpeed = (controls->isDown("run")) ? runSpeed : walkSpeed;

                if (controls->isDown("up") && !controls->isDown("down")) {
                    walkDirection = Up;
                    walkDirections |= Up;
                }
                if (controls->isDown("down") && !controls->isDown("up")) {
                    walkDirection = Down;
                    walkDirections |= Down;
                }
                if (controls->isDown("left") && !controls->isDown("right")) {
                    walkDirections |= Left;
                    if (walkDirection == Up) {
                        walkDirection = UpLeft;
                        walkDirections |= UpLeft;
                    }
                    else if (walkDirection == Down) {
                        walkDirection = DownLeft;
                        walkDirections |= DownLeft;
                    }
                    else walkDirection = Left;
                }
                if (controls->isDown("right") && !controls->isDown("left")) {
                    walkDirections |= Right;
                    if (walkDirection == Up) {
                        walkDirection = UpRight;
                        walkDirections |= UpRight;
                    }
                    else if (walkDirection == Down) {
                        walkDirection = DownRight;
                        walkDirections |= DownRight;
                    }
                    else walkDirection = Right;
                }

                if (walkDirection) {
                    walk(walkDirection);
                }
                else {
                    face(direction);
                }
            }
            else if (!walkDirection) {
                face(direction);
            }
			
            Amara::Sprite::run();

			if (walkDirection) {
                justFinishedWalking = true;
            }
			else {
				justFinishedWalking = false;
			}

			walkDirection = NoDir;
            walkDirections = 0;
			movementSpeed = walkSpeed;
        }
    };
}