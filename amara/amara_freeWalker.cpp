#pragma once

#include "amara.h"

namespace Amara {
    class FreeWalker: public Amara::Sprite {
    public:
        Direction direction = Down;
        Direction walkDirection = NoDir;
        int walkDirections = 0;

        float movementSpeed = 1;
        float walkSpeed = 1;
        float runSpeed = 4;

        bool justFinishedWalking = false;

        bool controlsEnabled = false;

        Amara::PathFindingTask* pathTask = nullptr;
        Amara::PathTile currentPathTile;

        FreeWalker(int gx, int gy): Amara::Sprite(gx, gy) {}
        FreeWalker(int gx, int gy, std::string tKey): Amara::Sprite(gx, gy, tKey) {}
        FreeWalker(): Amara::Sprite() {}

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
        }

        virtual nlohmann::json toData() {
            nlohmann::json config = Amara::Sprite::toData();
            config["direction"] = direction;
            config["walkSpeed"] = walkSpeed;
            config["runSpeed"] = runSpeed;
            config["controlsEnabled"] = controlsEnabled;
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
            movementSpeed = shouldRun ? runSpeed : walkSpeed;
            float ox = Amara::getOffsetX(dir) * movementSpeed;
            float oy = Amara::getOffsetY(dir) * movementSpeed;
            if (abs(ox) && abs(oy)) {
                ox = ox/abs(ox) * sqrt(pow(abs(movementSpeed),2)/2);
                oy = oy/abs(oy) * sqrt(pow(abs(movementSpeed),2)/2);
            }
            
            if (physics) {
                physics->velocityX += ox;
                physics->velocityY += oy;
            }
            else {
                x += ox;
                y += oy;
            }

            walkDirection = dir;
            walkDirections |= dir;
            direction = dir;
        }

        virtual void walk(Direction dir) {
            walk(dir, false);
        }

        virtual void run(Direction dir) {
            walk(dir, true);
        }

        void run() {
            if (walkDirection) {
                justFinishedWalking = true;
            }

            if (controlsEnabled) {
                walkDirection = NoDir;
                walkDirections = 0;
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
                    justFinishedWalking = false;
                }
                else {
                    face(direction);
                }
            }
            else if (!walkDirection) {
                face(direction);
            }

            Amara::Sprite::run();
            if (!controlsEnabled) {
                walkDirection = NoDir;
                walkDirections = 0;
            }
        }
    };
}