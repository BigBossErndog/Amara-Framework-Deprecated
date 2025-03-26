namespace Amara {
    class Script_PlayAudio: public Script {
    public:
        float delay = 0;
        std::string audioKey;

        Script_PlayAudio(std::string gKey) {
            audioKey = gKey;
        }
        Script_PlayAudio(std::string gKey, float gDelay): Script_PlayAudio(gKey) {
            delay = gDelay;
        }

        void prepare() {}

        void script() {
            start();
            if (delay == 0) {
                finish();
                return;
            }

            wait(delay);
            finishEvt();
        }

        void finish() {
            audio->play(audioKey);
            Script::finish();
        }
    };

    class Tween_Volume: public Amara::Tween_Float {
    public:
        std::string audioKey;
        Amara::AudioBase* audioObj = nullptr;
        
        Tween_Volume(float gTarget, double gTime, Amara::Easing gEasing): Tween_Float() {
            target = gTarget;
            time = gTime;
            easing = gEasing;
        }
        Tween_Volume(float gTarget, double gTime): Tween_Volume(gTarget, gTime, EASE_LINEAR) {}
        
        Tween_Volume(std::string gKey, float gTarget, double gTime, Amara::Easing gEasing): Tween_Volume(gTarget, gTime, gEasing) {
            audioKey = gKey;
        }
        Tween_Volume(std::string gKey, float gTarget, double gTime): Tween_Volume(gKey, gTarget, gTime, EASE_LINEAR) {}

        Tween_Volume(Amara::AudioBase* gAudio, float gTarget, double gTime, Amara::Easing gEasing): Tween_Volume(gTarget, gTime, gEasing) {
            audioObj = gAudio;
        }
        Tween_Volume(Amara::AudioBase* gAudio, float gTarget, double gTime): Tween_Volume(gAudio, gTarget, gTime, EASE_LINEAR) {}

        void prepare() {
            if (!audioObj && !audioKey.empty()) audioObj = audio->get(audioKey);
            number = &audioObj->volume;
            Tween_Float::prepare();
        }
    };

    class Tween_MasterVolume: public Amara::Tween_Volume {
    public:
        using Tween_Volume::Tween_Volume;

        void prepare() {
            if (!audioObj && !audioKey.empty()) audioObj = audio->get(audioKey);
            number = &audioObj->masterVolume;
            Tween_Float::prepare();
        }
    };
}