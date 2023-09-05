namespace Amara {
    enum Easing {
        LINEAR,
        SINE_INOUT,
        SINE_IN,
        SINE_OUT,
        SINE_INOUT_BACKROUND
    };

    double linearEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*progress;
    }   

    double sineInOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI) + 1)/2;
    }

    double sineOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(progress*M_PI/2));
    }

    double sineInEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI/2) + 1);
    }

    double sineInOutBackRoundEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2 + progress*M_PI*2) + 1)/2;
    }

    double sineHardInOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(M_PI * progress));
    }

    double ease(float startVal, float endVal, double progress, Amara::Easing easing) {
        switch (easing) {
            case LINEAR:
                return linearEase(startVal, endVal, progress);
                break;
            case SINE_INOUT:
                return sineInOutEase(startVal, endVal, progress);
                break;
            case SINE_IN:
                return sineInEase(startVal, endVal, progress);
                break;
             case SINE_OUT:
                return sineOutEase(startVal, endVal, progress);
                break;
        }
        return linearEase(startVal, endVal, progress);
    }

    SDL_Color ease(SDL_Color startColor, SDL_Color endColor, double progress, Amara::Easing easing) {
        return {
            ease(startColor.r, endColor.r, progress, easing),
            ease(startColor.g, endColor.g, progress, easing),
            ease(startColor.b, endColor.b, progress, easing),
            ease(startColor.a, endColor.a, progress, easing)
        };
    }
    SDL_Color ease(SDL_Color startColor, SDL_Color endColor, double progress) {
        return ease(startColor, endColor, progress, LINEAR);
    }
}