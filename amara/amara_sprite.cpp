namespace Amara {
    class Sprite: public Amara::Image, public Amara::Animated {
        public:
            Sprite(float gx, float gy, std::string tx): Amara::Image(gx, gy, tx) {}
            Sprite(float gx, float gy): Amara::Image(gx, gy) {}
            Sprite(std::string tx): Amara::Image(tx) {}
            Sprite(): Amara::Image() {};

            using Amara::Image::init;
            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) override {
                anims = Amara::AnimationManager(gameProperties, this);
                Amara::Image::init(gameProperties, givenScene, givenParent);
                entityType = "sprite";
			}

            virtual bool play(std::string animKey) {
                return anims.play(texture, animKey);
            }

			virtual void play(Amara::Animation* anim) {
				return anims.play(texture, anim);
			}

            virtual void configure(nlohmann::json config) {
                Amara::Image::configure(config);
                if (config.find("animation") != config.end()) {
                    play(config["animation"]);
                    if (config.find("animationProgress") != config.end()) {
                        anims.setProgress(config["animationProgress"]);
                    }
                }
            }

            virtual nlohmann::json toData() {
                nlohmann::json config = Amara::Image::toData();
                if (anims.currentAnim) {
                    config["animation"] = anims.currentAnim->key;
                    config["animationProgress"] = anims.getProgress();
                }
                return config;
            }

            virtual void run() {
                Amara::Image::run();
                anims.run();
            }

            using Amara::Image::setTexture;
            bool setTexture(std::string gTextureKey, std::string gAnimKey) {
                if (setTexture(gTextureKey)) {
                    play(gAnimKey);
                    return true;
                }
                return false;
            }
    };
}
