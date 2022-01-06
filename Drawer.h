#ifndef DRAWER_H
#define DRAWER_H

#include "Material.h"

#include <GL/glu.h>


class Drawer {
    public:
        Drawer();
		void sphere();
		void ring(GLfloat slope, GLfloat radius = 1);
		void disk(GLfloat hole = 0);
        void cube();
        virtual void draw_geometry() = 0;
        virtual void gl_init() = 0;

        struct material set_material(struct material &material);
        void load_texture(const char *file, GLuint &tex);
        void enable_texture(GLuint &tex);
        void disable_texture();

    private:
        GLUquadricObj *quad;
};

#endif
