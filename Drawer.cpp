#include <QGLWidget>
#include <QImage>
#include <QVector3D>

#include "Drawer.h"


#define FACES 100

using namespace std;


Drawer::Drawer() {
    quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);
}


struct material Drawer::set_material(struct material &material) {
    struct material old;

    glGetMaterialfv(GL_FRONT, GL_AMBIENT,  old.ambient);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE,  old.diffuse);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, old.specular);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &old.shininess);
    glGetMaterialfv(GL_FRONT, GL_EMISSION, old.emission);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emission);

    return old;
}

void Drawer::load_texture(const char *file, GLuint &tex) {
    QImage image(file);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA,
        GL_UNSIGNED_BYTE, QGLWidget::convertToGLFormat(image).bits()
    );
}

void Drawer::enable_texture(GLuint &tex) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void Drawer::disable_texture() {
    glDisable(GL_TEXTURE_2D);
}

void Drawer::sphere() {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluSphere(quad, 1, FACES, FACES);
    glPopMatrix();
}

void Drawer::ring(GLfloat slope, GLfloat radius) {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quad, radius, slope, 1, FACES, FACES);
    glPopMatrix();
}

void Drawer::disk(GLfloat hole) {
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluDisk(quad, hole, 1, FACES, 1);
    glPopMatrix();
}

void Drawer::cube() {
    vector<QVector3D> vertices = {
        {-0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},
        {-0.5, 0.5, -0.5},
        {0.5, 0.5, -0.5},
        {-0.5, -0.5, -0.5},
        {0.5, -0.5, -0.5},
    };

    vector<array<GLfloat, 2>> textues = {
        {0, 1},
        {1, 1},
        {0, 0},
        {1, 0},
        {0, 2},
        {1, 2},
        {0, -2},
        {1, -2},
    };

    vector<array<GLfloat, 4>> faces = {
        {0, 2, 3, 1},
        {4, 5, 7, 6},
        {1, 3, 7, 5},
        {0, 4, 6, 2},
        {0, 1, 5, 4},
        {3, 2, 6, 7}
    };

    for (array<GLfloat, 4> face: faces) {
        QVector3D normal = QVector3D::normal(
            vertices[face[0]],
            vertices[face[1]],
            vertices[face[2]]
        );

        glBegin(GL_POLYGON);
            glNormal3f(normal.x(), normal.y(), normal.z());
            for (int vertex: face) {
                glTexCoord2f(textues[vertex][0], textues[vertex][1]);
                glVertex3f(vertices[vertex].x(), vertices[vertex].y(), vertices[vertex].z());
            }
        glEnd();
    }
}
