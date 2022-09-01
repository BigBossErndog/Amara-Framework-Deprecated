#ifndef AMARA_TWEENBASE
#define AMARA_TWEENBASE



namespace Amara {
    class Tweener;

    class Tween: public Amara::StateManager {
        public:
            Amara::GameProperties* properties = nullptr;
            bool finished = false; 
            bool deleteOnFinish = true;

            double progress = 0;
            float time = 0;

            virtual void assign(Amara::Tweener* gTweener, Amara::GameProperties* gProperties) {
                properties = gProperties;
            }

            virtual void run() {
                progress += (time/properties->lps);
                if (progress >= 1) {
                    finished = true;
                    progress = 1;
                }
            }
            
            virtual void reset() {
                progress = 0;
            }
    };
}

#endif