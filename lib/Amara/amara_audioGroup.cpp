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
                Amara::AudioGroup* group;
                for (size_t i = 0; i < groups.size(); i++) {
                    group = groups.at(i);
                    if (group->key.compare(fKey) == 0) {
                        return group;
                    }
                }
                for (size_t i = 0; i < groups.size(); i++) {
                    group = groups.at(i);
                    group = group->getGroup(fKey);
                    if (group != nullptr) {
                        return group;
                    }
                }
                return nullptr;
            }

            Amara::AudioBase* getSound(string fKey) {
                Amara::AudioBase* sound;
                for (size_t i = 0; i < sounds.size(); i++) {
                    sound = sounds.at(i);
                    if (sound->key.compare(fKey) == 0) {
                        return sound;
                    }
                }
                Amara::AudioGroup* group;
                for (size_t i = 0; i < groups.size(); i++) {
                    group = groups.at(i);
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
                Amara::AudioBase* audio;

                for (size_t i = 0; i < sounds.size(); i++) {
                    audio = sounds.at(i);
                    audio->run(volume * masterVolume * parentVolume);
                }

                for (size_t i = 0; i < groups.size(); i++) {
                    audio = groups.at(i);
                    audio->run(volume * masterVolume * parentVolume);
                }
            } 
    };
}

#endif