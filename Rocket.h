#ifndef ROCKET_H
#define ROCKET_H
#include <cstddef>
#include "Drawer.h"


class Rocket: public Drawer {
    public:
        Rocket(): Drawer() {}

        void draw_geometry();
        void gl_init();
    private:
        void nose_cone();
        void pipe();
        void engine();
        void booster_bot(int height, GLuint *tex = NULL);
        void booster();
        void struts();
};

#endif
