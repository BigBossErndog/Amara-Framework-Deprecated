namespace Amara {
    class Tween_Int: public Amara::Tween {
    public:
        int* number = nullptr;
        int start = 0;
        int target = 0;

        Tween_Int() {}
        
        Tween_Int(int* gNum, int gTarget, float gTime, Amara::Easing gEasing) {
            number = gNum;
            target = gTarget;
            time = gTime;
            easing = gEasing;
        }

        Tween_Int(int* gNum, int gTarget, float gTime): Tween_Int(gNum, gTarget, gTime, LINEAR) {}

        Tween_Int(int& gNum, float gTarget, float gTime, Amara::Easing gEasing): Tween_Int(&gNum, gTarget, gTime, gEasing) {}
        Tween_Int(int& gNum, float gTarget, float gTime): Tween_Int(&gNum, gTarget, gTime) {}

        void prepare() {
            start = *number;
        }

        void script() {
            Amara::Tween::progressFurther();
            *number = ease(start, target, progress, easing);
        }

        void finish() {
            Amara::Tween::finish();
            *number = target;
        }
    };

    class Tween_Float: public Amara::Tween {
    public:
        float* number = nullptr;
        float start = 0;
        float target = 0;

        Tween_Float() {}
        
        Tween_Float(float* gNum, float gTarget, float gTime, Amara::Easing gEasing) {
            number = gNum;
            start = *number;
            target = gTarget;
            time = gTime;
            easing = gEasing;
        }
        Tween_Float(float* gNum, float gTarget, float gTime): Tween_Float(gNum, gTarget, gTime, LINEAR) {}

        Tween_Float(float& gNum, float gTarget, float gTime, Amara::Easing gEasing): Tween_Float(&gNum, gTarget, gTime, gEasing) {}
        Tween_Float(float& gNum, float gTarget, float gTime): Tween_Float(&gNum, gTarget, gTime) {}

        void prepare() {
            start = *number;
        }

        void script() {
            Amara::Tween::finish();
            *number = ease(start, target, progress, easing);
        }

        void finish() {
            Amara::Tween::progressFurther();
            *number = target;
        }
    };

    class Tween_Wait: public Tween {
    public:
        Tween_Wait(double gt) {
            time = gt;
        }
        
        void script() {
            progressFurther();
        }
    };
}