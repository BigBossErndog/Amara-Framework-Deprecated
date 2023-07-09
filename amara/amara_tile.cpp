namespace Amara {
    const unsigned long TILED_FLIPPEDHORIZONTALLY = 0x80000000;
    const unsigned long TILED_FLIPPEDVERTICALLY = 0x40000000;
    const unsigned long TILED_FLIPPEDANTIDIAGONALLY = 0x20000000;

    struct TileFrame {
        int tileId;
        int duration;
    };

    class TileAnimation {
    public:
        Amara::GameProperties* properties = nullptr;

        int id = 0;

        std::vector<TileFrame> frames;
        int numFrames = 0;
        int frame = 0;
        int currentTileId = 0;

        float frameCounter = 0;

        TileAnimation(Amara::GameProperties* gameProperties, nlohmann::json& animData) {
            properties = gameProperties;

            id = animData["id"];
            frames.clear();
            frame = 0;

            numFrames = animData["animation"].size();
            for (nlohmann::json& frameData: animData["animation"]) {
                TileFrame tf;
                tf.tileId = frameData["tileid"];
                tf.duration = frameData["duration"];
                frames.push_back(tf);
            }

            currentTileId = (frames.empty()) ? id : frames[0].tileId;
        }

        void update() {
            if (frames.empty()) return;
            double factor = 1000.0/properties->lps;
            int duration = frames[frame].duration;

            frameCounter += (1000.0/properties->lps);

            if (frameCounter >= duration) {
                frameCounter -= duration;
                frame += 1;
                frame = frame % numFrames;
                currentTileId = frames[frame].tileId;
            }
        }
    };

    struct Tile {
        int id = -1;
        int x = 0;
        int y = 0;

        bool fhorizontal = false;
        bool fvertical = false;
        bool fdiagonal = false;

        float tileWidth = 0;
        float tileHeight = 0;
        int widthInPixels = 0;
        int heightInPixels = 0;

        bool animated  = false;

        float alpha = 1;
    };
}