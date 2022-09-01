#pragma once
#ifndef AMARA_TRANSITIONS
#define AMARA_TRANSITIONS



namespace Amara {
    class FillTransition: public Amara::SceneTransition {
        public:
            SDL_Color recColor;
            SDL_Color color = {0, 0, 0, 255};

            SDL_FRect drawnRect;
            SDL_Rect viewport;

            float fadeInSpeed = 0.1;
            float fadeOutSpeed = 0.1;
            float waitBetween = 0;

            FillTransition(): Amara::SceneTransition() {}
            FillTransition(float inSpeed, float outSpeed): FillTransition() {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
            }
            FillTransition(float inSpeed, float outSpeed, float gWaitBetween): FillTransition(inSpeed, outSpeed) {
                waitBetween = gWaitBetween;
            }
            FillTransition(float speed): FillTransition(speed, speed) {}

            FillTransition(std::string gNextScene): Amara::SceneTransition(gNextScene) {}

            FillTransition(std::string gNextScene, float inSpeed, float outSpeed): Amara::FillTransition(gNextScene) {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
            }
            FillTransition(std::string gNextScene, float inSpeed, float outSpeed, float gWaitBetween): Amara::FillTransition(gNextScene) {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
                waitBetween = gWaitBetween;
            }

            FillTransition(std::string gNextScene, float speed): FillTransition(gNextScene, speed, speed) {}

            void configure(nlohmann::json config) {
                Amara::SceneTransition::configure(config);
                if (config.find("fadeInSpeed") != config.end()) {
                    fadeInSpeed = config["fadeInSpeed"];
                }
                if (config.find("fadeOutSpeed") != config.end()) {
                    fadeOutSpeed = config["fadeOutSpeed"];
                }
                if (config.find("fadeSpeed") != config.end()) {
                    setSpeed(config["fadeSpeed"]);
                }
                if (config.find("waitBetween") != config.end()) {
                    waitBetween = config["waitBetween"];
                }
            }

            void setColor(int r, int g, int b, int a) {
                color.r = r;
                color.g = g;
                color.b = b;
                color.a = a;
            }
            void setColor(int r, int g, int b) {
                setColor(r, g, b, 255);
            }

            void setSpeed(float inSpeed, float outSpeed) {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
            }
            void setSpeed(float speed) {
                setSpeed(speed, speed);
            }

            void create() {
                alpha = 0;
            }

            void update() {
                start();
                if (evt()) {
                    alpha += fadeInSpeed;
                    if (alpha >= 1) {
                        alpha = 1;
                        nextEvt();
                    }
                }
                wait(waitBetween);
                startNextScene();
                waitForPermission();
                if (evt()) {
					alpha -= fadeOutSpeed;
                    if (alpha <= 0) {
                        alpha = 0;
                        nextEvt();
                    }
                }
                finishEvt();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::GameProperties* properties = Amara::Actor::properties;
                SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                drawnRect.x = -1;
                drawnRect.y = -1;
                drawnRect.w = vw+2;
                drawnRect.h = vh+2;

                int newAlpha = (float)color.a * alpha;
                SDL_SetRenderDrawBlendMode(properties->gRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(properties->gRenderer, color.r, color.g, color.b, newAlpha);
                SDL_RenderFillRectF(properties->gRenderer, &drawnRect);

                SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}


#endif