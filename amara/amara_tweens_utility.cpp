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

        Tween_Int(int* gNum, int gTarget, float gTime): Tween_Int(gNum, gTarget, gTime, EASE_LINEAR) {}

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
        Tween_Float(float* gNum, float gTarget, float gTime): Tween_Float(gNum, gTarget, gTime, EASE_LINEAR) {}

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

    class Tween_Color: public Tween {
    public:
        SDL_Color startColor;
        SDL_Color endColor;

        SDL_Color* affectColor;

        Tween_Color(SDL_Color* gAffect, SDL_Color gEnd, float tt, Amara::Easing gEasing) {
            startColor = *gAffect;
            affectColor = gAffect;
            endColor = gEnd;
            time = tt;
            easing = gEasing;
        }
        Tween_Color(SDL_Color& gAffect, SDL_Color gEnd, float tt, Amara::Easing gEasing): Tween_Color(&gAffect, gEnd, tt, gEasing) {}
        Tween_Color(SDL_Color& gAffect, SDL_Color gEnd, float tt): Tween_Color(gAffect, gEnd, tt, EASE_LINEAR) {}
        Tween_Color(FillRect* rect, SDL_Color gEnd, float tt, Amara::Easing gEasing): Tween_Color(rect->color,gEnd, tt, gEasing) {}
        Tween_Color(FillRect* rect, SDL_Color gEnd, float tt): Tween_Color(rect, gEnd, tt, EASE_LINEAR) {}

        void script() {
            Amara::Tween::progressFurther();
            affectColor->r = ease(startColor.r, endColor.r, progress, easing);
            affectColor->g = ease(startColor.g, endColor.g, progress, easing);
            affectColor->b = ease(startColor.b, endColor.b, progress, easing);
            affectColor->a = ease(startColor.a, endColor.a, progress, easing);
        }

        void finish() {
            Tween::finish();
            affectColor->r = endColor.r;
            affectColor->g = endColor.g;
            affectColor->b = endColor.b;
            affectColor->a = endColor.a;
        }
    };
}