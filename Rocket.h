#include <GL/glu.h>

#include "Drawer.h"


class Rocket: public Drawer {
    public:
        Rocket(): Drawer() {}

        void draw_geometry();
    private:
        void nose_cone();
        void booster_bot(int height);
        void booster();
};
