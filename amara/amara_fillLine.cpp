namespace Amara {
    class FillLine: public Amara::Actor {
    public:
        Amara::Color color = { 0, 0, 0, 255 };

        std::vector<FloatVector2> points;

        FillLine(std::vector<FloatVector2> gPoints) {
            points = gPoints;
        }
        FillLine(FloatVector2 p1, FloatVector2 p2): FillLine({ p1, p2 }) {}
        FillLine(Amara::Color gColor, std::vector<FloatVector2> gPoints): FillLine(gPoints) {
            color = gColor;
        }
        FillLine(Amara::Color gColor, FloatVector2 p1, FloatVector2 p2): FillLine(gColor, { p1, p2 }) {}

        void init() {
            Amara::Actor::init();
            entityType = "fillLine";
        }

        void configure(nlohmann::json config) {
            if (config.find("points") != config.end() && config["points"].is_array()) {
                nlohmann::json pointsData = config["points"];
                for (json point: pointsData) {
                    points.push_back({ point["x"], point["y"] });
                }
            }
        }

        Amara::FillRect* setColor(int r, int g, int b, int a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
			return this;
		}
		Amara::FillRect* setColor(int r, int g, int b) {
			return setColor(r, g, b, 255);
		}
		Amara::FillRect* setColor(Amara::Color gColor) {
			color = gColor;
			return this;
		}

		Amara::FillRect* setRenderOffset(float gx, float gy) {
			renderOffsetX = gx;
			renderOffsetY = gy;
			return this;
		}

		Amara::FillRect* setRenderOffset(float gi) {
			return setRenderOffset(gi, gi);
		}

        void draw(int vx, int vy, int vw, int vh) {
            
        }
    };
}