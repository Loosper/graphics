#ifndef MATERIAL_H
#define MATERIAL_H
#include <GL/glu.h>

// no emissive
struct material {
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat ambient[4];
    GLfloat shininess;
};

#endif
