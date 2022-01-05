#ifndef DRAWER_H
#define DRAWER_H
#include <GL/glu.h>

class Drawer {
    public:
        Drawer();
		void sphere();
		void ring(GLfloat slope, GLfloat radius = 1);
		void disk();
        virtual void draw_geometry() = 0;

    private:
        GLUquadricObj *quad;
};

#endif
