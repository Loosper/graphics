#include "Drawer.h"

Drawer::Drawer() {
    quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);
}

void Drawer::sphere() {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluSphere(quad, 1, 100, 100);
    glPopMatrix();
}

void Drawer::ring(GLfloat slope, GLfloat radius) {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, radius, slope, 1, 100, 100);
    glPopMatrix();
}

void Drawer::disk() {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluDisk(quad, 0, 1, 100, 1);
    glPopMatrix();
}
