namespace Amara {
    class Tween_Volume: public Amara::Tween_Float {
    public:
        std::string audioKey;
        Amara::AudioBase* audioObj = nullptr;
        
        Tween_Volume(float gTarget, double gTime, Amara::Easing gEasing): Tween_Float() {
            targetNum = gTarget;
            time = gTime;
            easing = gEasing;
        }
        Tween_Volume(float gTarget, double gTime): Tween_Volume(gTarget, gTime, LINEAR) {}
        
        Tween_Volume(std::string gKey, float gTarget, double gTime, Amara::Easing gEasing): Tween_Volume(gTarget, gTime, gEasing) {
            audioKey = gKey;
        }
        Tween_Volume(std::string gKey, float gTarget, double gTime): Tween_Volume(gKey, gTarget, gTime, LINEAR) {}

        Tween_Volume(Amara::AudioBase* gAudio, float gTarget, double gTime, Amara::Easing gEasing): Tween_Volume(gTarget, gTime, gEasing) {
            audioObj = gAudio;
        }
        Tween_Volume(Amara::AudioBase* gAudio, float gTarget, double gTime): Tween_Volume(gAudio, gTarget, gTime, LINEAR) {}

        void prepare() {
            if (!audioObj && !audioKey.empty()) audioObj = audio->get(audioKey);
            num = &audioObj->volume;
            Tween_Float::prepare();
        }
    };

    class Tween_MasterVolume: public Amara::Tween_Volume {
    public:
        using Tween_Volume::Tween_Volume;

        void prepare() {
            if (!audioObj && !audioKey.empty()) audioObj = audio->get(audioKey);
            num = &audioObj->masterVolume;
            Tween_Float::prepare();
        }
    };
}