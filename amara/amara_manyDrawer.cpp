namespace Amara {
    class ManyDrawer: public Amara::Layer {
    public:
        int drawRepetitions = 1;

        using Amara::Layer::Layer;

        virtual bool drawUpdate(int index) { return true; }
        
        virtual void draw(int vx, int vy, int vw, int vh) {
            if (!isVisible) return;
            for (int i = 0; i < drawRepetitions; i++) {
                if (drawUpdate(i)) Amara::Layer::draw(vx, vy, vw, vh);
            }
        }
    };
}