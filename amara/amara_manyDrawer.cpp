namespace Amara {
    class ManyDrawer: public Amara::Actor {
    public:
        int drawRepetitions = 1;
        bool skipDraw = false;

        virtual bool drawUpdate() { return true; }

        virtual void draw(int vx, int vy, int vw, int vh) {
            if (!isVisible) return;
            for (int i = 0; i < drawRepetitions; i++) {
                skipDraw = false;
                if (drawUpdate()) Amara::Actor::draw(vx, vy, vw, vh);
            }
        }
    };
}