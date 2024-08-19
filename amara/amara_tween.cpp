namespace Amara {
    class Tween: public Script {
    public:
        Amara::Easing easing;
        double progress = 0;
        double time = 0;
        bool useDeltaTime = true;

        using Script::init;
        void init() {
            Script::init();
            progress = 0;
        }

        virtual void progressFurther() {
            progress += 1/(time*properties->lps);
            if (progress >= 1) {
                progress = 1;
                finish();
            }
        }
    };

    class Tween_Int: public Tween {
    public:
        int startNum = 0;
        int targetNum = 0;
        int* num = nullptr;

        Tween_Int(int& gNum, int gTarget, double gTime, Amara::Easing gEasing) {
            num = &gNum;
            targetNum = gTarget;
            easing = gEasing;
            time = gTime;
        }
        Tween_Int(int& gNum, int gTarget, double gTime): Tween_Int(gNum, gTarget, gTime, LINEAR) {}

        void prepare() {
            startNum = *num;
        }

        void script() {
            progressFurther();
            *num = floor(ease(startNum, targetNum, progress, easing));
        }

        void finish() {
            Tween::finish();
        }
    };

    class Tween_Float: public Tween {
    public:
        float startNum = 0;
        float targetNum = 0;
        float* num = nullptr;

        Tween_Float() {}
        Tween_Float(float& gNum, float gTarget, double gTime, Amara::Easing gEasing) {
            num = &gNum;
            targetNum = gTarget;
            easing = gEasing;
            time = gTime;
        }
        Tween_Float(float& gNum, float gTarget, double gTime): Tween_Float(gNum, gTarget, gTime, LINEAR) {}

        void prepare() {
            startNum = *num;
        }

        void script() {
            progressFurther();
            *num = ease(startNum, targetNum, progress, easing);
        }

        void finish() {
            Tween::finish();
        }
    };
}
