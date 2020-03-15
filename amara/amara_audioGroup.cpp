#ifndef AMARA_AUDIOGROUP
#define AMARA_AUDIOGROUP

#include "amara.h"

namespace Amara {
    class AudioGroup: public Amara::AudioBase {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* load = nullptr;

            vector<Amara::AudioGroup*> groups;
            vector<Amara::AudioBase*> sounds;

            AudioGroup(string gKey) {
                key = gKey;
            }

            AudioGroup(Amara::GameProperties* gameProperties, string gKey): AudioGroup(gKey) {
                properties = gameProperties;
                load = properties->loader;
            }

            AudioGroup(string gKey, Amara::AssetType gType, void* asset): Amara::AudioBase(gKey, gType, asset) {}

            Amara::AudioGroup* add(Amara::AudioGroup* gGroup) {
                groups.push_back(gGroup);
                gGroup->properties = properties;
                return gGroup;
            }

            Amara::AudioBase* add(Amara::AudioBase* sound) {
                sounds.push_back(sound);
                return sound;
            }

            Amara::AudioBase* add(string fKey) {
                Amara::AudioBase* sound = (Amara::AudioBase*)load->get(fKey);
                if (sound != nullptr) {
                    return add(sound);
                }
                return nullptr;
            }

            Amara::AudioGroup* getGroup(string fKey) {
                for (Amara::AudioGroup* group : groups) {
                    if (group->key.compare(fKey) == 0) {
                        return group;
                    }
                }
                for (Amara::AudioGroup* group : groups) {
                    group = group->getGroup(fKey);
                    if (group != nullptr) {
                        return group;
                    }
                }
                return nullptr;
            }

            Amara::AudioBase* getSound(string fKey) {
                Amara::AudioBase* sound;
                for (Amara::AudioBase* audio : sounds) {
                    if (sound->key.compare(fKey) == 0) {
                        return sound;
                    }
                }
                for (Amara::AudioGroup* group : groups) {
                    sound = group->getSound(fKey);
                    if (sound != nullptr) {
                        return sound;
                    }
                }
                return nullptr;
            }

            Amara::AudioBase* get(string fKey) {
                Amara::AudioBase* audio;

                audio = getSound(fKey);
                if (audio != nullptr) {
                    return audio;
                }

                audio = getGroup(fKey);
                if (audio != nullptr) {
                    return audio;
                }

                return nullptr;
            }

            void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

                for (Amara::AudioBase* audio : sounds) {
                    audio->run(volume * masterVolume * parentVolume);
                }

                for (Amara::AudioGroup* audio : groups) {
                    audio->run(volume * masterVolume * parentVolume);
                }
            } 
    };
}

#endif