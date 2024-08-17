namespace Amara {
    enum Easing {
        LINEAR,
        SINE_INOUT,
        SINE_IN,
        SINE_OUT,
        SINE_INTHENOUT, // Circular but sine in/out as it leaves/reaches start value.
        CIRCULAR
    };

    // Progress is between 0 and 1.
    double linearEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*progress;
    }   

    double sineInOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2.0 + progress*M_PI) + 1.0)/2.0;
    }

    double sineOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(progress*M_PI/2.0));
    }

    double sineInEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2.0 + progress*M_PI/2.0) + 1.0);
    }

    double sineInThenOutEase(float startVal, float endVal, double progress) {
        return startVal + (endVal - startVal)*(sin(-M_PI/2.0 + progress*M_PI*2.0) + 1.0)/2.0;
    }

    double circularEase(float startVal, float endVal, double progress) {
        return startVal + positionAtAngle(
            1, 0,
            linearEase(-M_PI/2.0, M_PI/2.0, progress),
            1
        ).y * (endVal - startVal);
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
            case SINE_INTHENOUT:
                return sineInThenOutEase(startVal, endVal, progress);
                break;
            case CIRCULAR:
                return circularEase(startVal, endVal, progress);
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