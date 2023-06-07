namespace Amara {
    class Timer: public Amara::Actor {
    public:
        int milliseconds = 0;
        int seconds = 0;
        int minutes = 0;
        int hours = 0;

        bool isRunning = false;

        LTimer ticker;
        Uint32 lastTick = 0;

        Timer() {}

        void configure(nlohmann::json config) {
            Actor::configure(config);
            if (config.find("milliseconds") != config.end()) milliseconds = config["milliseconds"];
            if (config.find("seconds") != config.end()) seconds = config["seconds"];
            if (config.find("minutes") != config.end()) minutes = config["minutes"];
            if (config.find("hours") != config.end()) hours = config["hours"];
        }

        nlohmann::json toData() {
            nlohmann::json config = Actor::toData();
            
            config["milliseconds"] = milliseconds;
            config["seconds"] = seconds;
            config["minutes"] = minutes;
            config["hours"] = hours;

            return config;
        }

        void init() {
            reset();
        }

        void reset() {
            milliseconds = 0;
            seconds = 0;
            minutes = 0;
            hours = 0;

            ticker.stop();
        }

        void start() {
            reset();
            ticker.start();
            tick();
            isRunning = true;
        }

        void resume() {
            tick();
            isRunning = true;
        }

        void stop() {
            tick();
            isRunning = false;
        }

        void tick() {
            milliseconds += ticker.getTicks();
            while (milliseconds >= 1000) {
                milliseconds -= 1000;
                seconds += 1;
            }
            while (seconds >= 60) {
                seconds -= 60;  
                minutes += 1;
            }
            while (minutes >= 60) {
                minutes -= 60;
                hours += 1;
            }
            ticker.start();
        }

        void run() {
            tick();
            Actor::run();
        }
    };
}