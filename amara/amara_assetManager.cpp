namespace Amara {
    class AssetManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* loader = nullptr;

            std::unordered_map<std::string, std::vector<std::string>> textureGroups;

            std::unordered_map<std::string, std::unordered_map<std::string, Animation>> animations;

            AssetManager() {}
            AssetManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                loader = properties->loader;
            }

            Amara::Asset* get(std::string key) {
                return loader->get(key);
            }

			bool has(std::string key) {
				return loader->has(key);
			}

            nlohmann::json& getJSON(std::string key) {
                return loader->getJSON(key);
            }

            CSVFile* getCSV(std::string key) {
                return loader->getCSV(key);
            }

			std::string getString(std::string key) {
				return loader->getString(key);
			}

			SDL_Texture* getTexture(std::string key) {
				return loader->getTexture(key);
			}

			SDL_Surface* getSurface(std::string key) {
				return loader->getSurface(key);
			}

            SDL_Surface* getCachedSurface(std::string key) {
                return loader->getCachedSurface(key);
            }

            Amara::ImageTexture* getImageTexture(std::string key) {
                return (Amara::ImageTexture*)loader->get(key);
            }

			bool remove(std::string key) {
				return loader->remove(key);
			}

            bool add(std::string key, SDL_Texture* tx, bool replace) {
                return loader->add(key, tx, replace);
            }
            bool add(std::string key, SDL_Texture* tx) {
                return add(key, tx, true);
            }

            bool add(std::string key, SDL_Texture* tx, int frwidth, int frheight, bool replace) {
                return loader->add(key, tx, frwidth, frheight, replace);
            }
            bool add(std::string key, SDL_Texture* tx, int frwidth, int frheight) {
                return add(key, tx, frwidth, frheight, true);
            }

            bool add(std::string key, Amara::Asset* newAsset, bool replace) {
                return loader->add(key, newAsset, replace);
            }
            bool add(std::string key, Amara::Asset* newAsset) {
                return loader->add(key, newAsset, false);
            }

            Amara::Animation* addAnimation(std::string textureKey, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                auto tx = animations.find(textureKey);
                if (tx == animations.end()) {
                    animations[textureKey] = std::unordered_map<std::string, Animation>();
                    tx = animations.find(textureKey);
                }
                auto got = tx->second.find(animKey);
                if (got != tx->second.end()) {
                    Amara::Animation& anim = got->second;
                    anim.frames = frames;
                    anim.frameRate = frameRate;
                    anim.loop = loop;
                    return &anim;
                }
                tx->second[animKey] = Amara::Animation(textureKey, animKey, frames, frameRate, loop);
                return &tx->second[animKey];
            }
            Amara::Animation* addAnimation(std::string textureKey, std::string animKey, int frame) {
                return addAnimation(textureKey, animKey, {frame}, 1, false);
            }

            Amara::Animation* getAnimation(std::string textureKey, std::string animKey) {
                auto tx = animations.find(textureKey);
                if (tx != animations.end()) {
                    if (tx->second.find(animKey) != tx->second.end()) return &tx->second[animKey];
                }
                return nullptr;
            }

            void removeAnimation(std::string textureKey, std::string animKey) {
                auto tx = animations.find(textureKey);
                if (tx != animations.end()) {
                    auto got = tx->second.find(animKey);
                    if (got != tx->second.end()) tx->second.erase(animKey);
                }
            }

            void addAnimation(std::vector<std::string> textureKeys, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                for (std::string tKey : textureKeys) {
                    addAnimation(tKey, animKey, frames, frameRate, loop);
                }
            }

            void addAnimation(std::vector<std::string> textureKeys, std::string animKey, int frame) {
                addAnimation(textureKeys, animKey, {frame}, 1, false);
            }

            void addAnimation(std::string textureKey, std::string animKey, int startFrame, int numFrames, int frameRate, bool loop) {
                std::vector<int> frames;
                frames.clear();
                for (int i = 0; i < numFrames; i++) {
                    frames.push_back(startFrame + i);
                }
                addAnimation(textureKey, animKey, frames, frameRate, loop);
            }

            void configureAnimation(nlohmann::json& config) {
                std::string animKey = config["key"];
                bool loop = false;

                if (config.find("loop") != config.end()) {
                    loop = config["loop"];
                }

                if (config.find("textures") != config.end()) {
                    nlohmann::json& textures = config["textures"];
                    std::vector<std::string> textureKeys = textures;

                    if (config.find("frames") != config.end()) {
                        std::vector<int> frames = config["frames"];
                        int frameRate = config["frameRate"];
                        addAnimation(textureKeys, animKey, frames, frameRate, loop);
                    }
                    else if (config.find("frame") != config.end()) {
                        int frame = config["frame"];
                        addAnimation(textureKeys, animKey, frame);
                    }
                    else if (config.find("startFrame") != config.end()) {
                        if (config.find("numFrames") != config.end()) {
                            std::vector<int> frames;
                            frames.clear();
                            int startFrame = config["startFrame"];
                            int numFrames = config["numFrames"];
                            int frameRate = config["frameRate"];
                            for (int i = 0; i < numFrames; i++) {
                                frames.push_back(startFrame + i);
                            }
                            addAnimation(textureKeys, animKey, frames, frameRate, loop);
                        }
                        else {
                            SDL_Log("Number of frames for animation \"%s\" is not specified.", animKey.c_str());
                        }
                    }
                }
                if (config.find("texture") != config.end()) {
                    std::string textureKey = config["texture"];
                    if (config.find("frames") != config.end()) {
                        std::vector<int> frames = config["frames"];
                        int frameRate = config["frameRate"];
                        addAnimation(textureKey, animKey, frames, frameRate, loop);
                    }
                    else if (config.find("frame") != config.end()) {
                        int frame = config["frame"];
                        addAnimation(textureKey, animKey, frame);
                    }
                    else if (config.find("startFrame") != config.end()) {
                        if (config.find("numFrames") != config.end()) {
                            std::vector<int> frames;
                            frames.clear();
                            int startFrame = config["startFrame"];
                            int numFrames = config["numFrames"];
                            int frameRate = config["frameRate"];
                            for (int i = 0; i < numFrames; i++) {
                                frames.push_back(startFrame + i);
                            }
                            addAnimation(textureKey, animKey, frames, frameRate, loop);
                        }
                        else {
                            SDL_Log("Number of frames for animation \"%s\" is not specified.", animKey.c_str());
                        }
                    }
                }
                if (config.find("group") != config.end()) {
                    std::string groupKey = config["group"];
                    if (textureGroups.find(groupKey) != textureGroups.end()) {
                        std::vector<std::string> textureKeys = textureGroups[groupKey];
                        
                        if (config.find("frames") != config.end()) {
                            std::vector<int> frames = config["frames"];
                            int frameRate = config["frameRate"];
                            addAnimation(textureKeys, animKey, frames, frameRate, loop);
                        }
                        else if (config.find("frame") != config.end()) {
                            int frame = config["frame"];
                            addAnimation(textureKeys, animKey, frame);
                        }
                        else if (config.find("startFrame") != config.end()) {
                            if (config.find("numFrames") != config.end()) {
                                std::vector<int> frames;
                                frames.clear();
                                int startFrame = config["startFrame"];
                                int numFrames = config["numFrames"];
                                int frameRate = config["frameRate"];
                                for (int i = 0; i < numFrames; i++) {
                                    frames.push_back(startFrame + i);
                                }
                                addAnimation(textureKeys, animKey, frames, frameRate, loop);
                            }
                            else {
                                SDL_Log("Number of frames for animation \"%s\" is not specified.", animKey.c_str());
                            }
                        }
                    }
                }
                if (config.find("groups") != config.end()) {
                    std::vector<std::string> groups = config["groups"];
                    for (std::string groupKey: groups) {
                        if (textureGroups.find(groupKey) != textureGroups.end()) {
                            std::vector<std::string> textureKeys = textureGroups[groupKey];
                            
                            if (config.find("frames") != config.end()) {
                                std::vector<int> frames = config["frames"];
                                int frameRate = config["frameRate"];
                                addAnimation(textureKeys, animKey, frames, frameRate, loop);
                            }
                            else if (config.find("frame") != config.end()) {
                                int frame = config["frame"];
                                addAnimation(textureKeys, animKey, frame);
                            }
                            else if (config.find("startFrame") != config.end()) {
                                if (config.find("numFrames") != config.end()) {
                                    std::vector<int> frames;
                                    frames.clear();
                                    int startFrame = config["startFrame"];
                                    int numFrames = config["numFrames"];
                                    int frameRate = config["frameRate"];
                                    for (int i = 0; i < numFrames; i++) {
                                        frames.push_back(startFrame + i);
                                    }
                                    addAnimation(textureKeys, animKey, frames, frameRate, loop);
                                }
                                else {
                                    SDL_Log("Number of frames for animation \"%s\" is not specified.", animKey.c_str());
                                }
                            }
                        }
                    }
                }
            }

            void configureAnimations(nlohmann::json& config) {
                if (config.find("groups") != config.end()) {
                    nlohmann::json& groups = config["groups"];
                    for (nlohmann::json& group: groups) {
                        std::string key = group["key"];
                        std::vector<std::string> textures = group["textures"];
                        textureGroups[key] = textures;
                    }
                }

                if (config.find("animations") != config.end()) {
                    nlohmann::json& anims = config["animations"];
                    if (anims.is_array()) {
                        for (nlohmann::json& anim: anims) {
                            configureAnimation(anim);
                        }
                    }
                }
                else {
                    configureAnimation(config);
                }
            }

            void configureAnimations(std::string key) {
                Amara::JsonFile* configAsset = (Amara::JsonFile*)loader->get(key);
                if (configAsset != nullptr) {
                    configureAnimations(configAsset->getJSON());
                }
                else {
                    SDL_Log("Animations JSON file/asset \"%s\" was not found.", key.c_str());
                }
            }

            void configureAnimations(std::vector<std::string> keys) {
                for (std::string key: keys) {
                    configureAnimations(key);
                }
            }
    };
}