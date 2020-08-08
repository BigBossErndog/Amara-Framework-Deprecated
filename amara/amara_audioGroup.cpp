#pragma once
#ifndef AMARA_AUDIOGROUP
#define AMARA_AUDIOGROUP

#include "amara.h"

namespace Amara {
    class AudioGroup: public Amara::AudioBase {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* load = nullptr;

            std::vector<Amara::AudioGroup*> groups;
            std::vector<Amara::AudioBase*> sounds;

			bool rootGroup = false;

            AudioGroup(std::string gKey) {
                key = gKey;
            }

            AudioGroup(Amara::GameProperties* gameProperties, std::string gKey): AudioGroup(gKey) {
                properties = gameProperties;
                load = properties->loader;
            }

            AudioGroup(std::string gKey, Amara::AssetType gType, void* asset): Amara::AudioBase(gKey, gType, asset) {}

            Amara::AudioGroup* add(Amara::AudioGroup* gGroup) {
                groups.push_back(gGroup);
                gGroup->properties = properties;
				if (properties) gGroup->load = properties->loader;
				gGroup->parent = this;
                return gGroup;
            }

            Amara::AudioBase* add(Amara::AudioBase* sound) {
                sounds.push_back(sound);
                sound->group = this;
				sound->parent = this;
                return sound;
            }

            Amara::AudioBase* add(std::string fKey) {
                Amara::AudioBase* sound = (Amara::AudioBase*)load->get(fKey);
                if (sound != nullptr) {
                    lastPlayed = sound;
                    currentlyPlaying = sound;

                    if (sound->group != nullptr) {
                        sound->group->lastPlayed = sound;
                        sound->group->currentlyPlaying = sound;
                    }
                    return add(sound);
                }
				SDL_Log("Audio not found: \"%s\"", fKey);
                return nullptr;
            }

            Amara::AudioGroup* getGroup(std::string fKey) {
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
				SDL_Log("AudioGroup not found: \"%s\"", fKey);
                return nullptr;
            }

            Amara::AudioBase* getSound(std::string fKey) {
                for (Amara::AudioBase* sound : sounds) {
                    if (sound->key.compare(fKey) == 0) {
                        return sound;
                    }
                }

                Amara::AudioBase* sound;
                for (Amara::AudioGroup* group : groups) {
                    sound = group->getSound(fKey);
                    if (sound != nullptr) {
                        return sound;
                    }
                }

				if (rootGroup) {
	                sound = (Amara::AudioBase*)load->get(fKey);
	                if (sound != nullptr) {
	                    return sound;
	                }
				}

                return nullptr;
            }

            Amara::AudioBase* get(std::string fKey) {
                Amara::AudioBase* audio;

                audio = getSound(fKey);
                if (audio != nullptr) {
                    return audio;
                }

                audio = getGroup(fKey);
                if (audio != nullptr) {
                    return audio;
                }

				for (Amara::AudioGroup* group: groups) {
					audio = group->get(fKey);
					if (audio != nullptr) {
						return audio;
					}
				}

				if (rootGroup) {
	                audio = (Amara::AudioBase*)load->get(fKey);
	                if (audio != nullptr) {
	                    return audio;
	                }
				}

                return nullptr;
            }

            Amara::AudioBase* play(std::string fKey) {
                Amara::AudioBase* audio = get(fKey);
                if (audio != nullptr) {
                    lastPlayed = audio;
					currentlyPlaying = audio;
                    audio->play();
                }

                return audio;
            }

            void run(float parentVolume) {
                Amara::AudioBase::run(parentVolume);

                for (Amara::AudioBase* audio : sounds) {
                    audio->run(volume * masterVolume * parentVolume);
                }

                for (Amara::AudioGroup* audio : groups) {
                    audio->run(volume * masterVolume * parentVolume);
                }

                if (currentlyPlaying != nullptr) {
                    if (!currentlyPlaying->isPlaying) {
                        currentlyPlaying = nullptr;
                    }
                }
            }

			void configure(nlohmann::json config) {
				if (config.find("audioGroups") != config.end()) {
					nlohmann::json& aGroups = config["audioGroups"];
					for (std::string gKey: aGroups) {
						add(new Amara::AudioGroup(gKey));
					}
				}
				if (config.find("music") != config.end()) {
					configure(config["music"]);
					return;
				}
				if (config.find("sound") != config.end()) {
					configure(config["sound"]);
					return;
				}
				for (nlohmann::json& m: config) {
					std::string mKey = m["key"];
					if (m.find("group") != m.end()) {
						std::string gKey = m["group"];
						Amara::AudioGroup* g = getGroup(gKey);
						if (g) {
							g->add(mKey);
						}
						else {
							SDL_Log("AudioGroup not found: \"%s\"", gKey.c_str());
						}
					}
				}
			}
    };
}

#endif
