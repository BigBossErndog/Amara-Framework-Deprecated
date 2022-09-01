#ifndef AMARA_TWEENER
#define AMARA_TWEENER



namespace Amara {
    class Tweener {
        public:
            Amara::GameProperties* properties = nullptr;

            std::vector<Amara::Tween*> tweens;

            Tweener() {}

            virtual void tween(Amara::Tween* tween) {
                tween->reset();
                tween->assign(this, properties);
                tweens.push_back(tween);
            }

            virtual void init(Amara::GameProperties* gProperties) {
                properties = gProperties;
                if (properties != nullptr) {
                    SDL_Log("FOUND PROPERTIES");
                    SDL_Log("FOUND PROPERTIES");
                    SDL_Log("FOUND PROPERTIES");
                    SDL_Log("FOUND PROPERTIES");
                    SDL_Log("FOUND PROPERTIES");
                }
                else {
                    SDL_Log("PROPERTIES?");
                    SDL_Log("PROPERTIES?");
                    SDL_Log("PROPERTIES?");
                    SDL_Log("PROPERTIES?");
                    SDL_Log("PROPERTIES?");
                }
                
            }

            virtual void run() {
                if (properties == nullptr) SDL_Log("PROBLEM");
                Amara::Tween* tween;
                for (auto it = tweens.begin(); it != tweens.end(); it++) {
                    tween = *it;
                    tween->run();
                    if (tween->finished) {
                        tweens.erase(it--);
                        if (tween->deleteOnFinish) {
                            delete tween;
                        }
                    }
                }
            }
    };
}

#endif