#include "Scene.h"

#include <math.h>
#include <iostream>

#include <GL/glu.h>
#include <Qt>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>

#define PI 3.1415
#define PHI 1.618


Scene::Scene(): QGLWidget(), angle(0), camera{0, 0} {
    // force update at ~60 fps
    // so I don't have to manually repaint whenever i change shit
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::updateGL);
    timer->start(17);
}

void Scene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left) {
        angle -= 2;
    } else if (event->key() == Qt::Key_Right) {
        angle += 2;
    } else if (event->key() == Qt::Key_Up) {
        // TODO: does this even make sense
        camera[0] += cos(angle * PI / 100);
        camera[1] += sin(angle * PI / 100);
    } else if (event->key() == Qt::Key_Down) {
        camera[0] -= cos(angle * PI / 100);
        camera[1] -= sin(angle * PI / 100);
    }

    QGLWidget::keyPressEvent(event);
}


void Scene::initializeGL() {
    glClearColor(0, 0, 0, 0);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
	// glOrtho(-10, 10, -10, 10, -10, 10);

    // GLfloat light_pos[] = {2., -2.,-2., 1.};
    // glEnable(GL_LIGHTING); // enable lighting in general
    // glEnable(GL_LIGHT0);   // each light source must also be enabled
    // glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glEnable(GL_DEPTH_TEST);
}

void Scene::resizeGL(int w, int h) {
    // all this to preserve the screen's aspect ratio and to not distort
    GLfloat ratio = (float)h / w;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // TODO: I'm pretty sure I'm putting this into the wrong matrix.
    // In lectures it's before the projection matrix is selected
    glViewport(0, 0, w, h);
	glOrtho(-10, 10, -10 * ratio, 10 * ratio, -10, 10);
}

void Scene::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->icosahedron();

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
        camera[0], 0, camera[1],
        cos(angle * PI / 180) + camera[0], 0, sin(angle * PI / 180) + camera[1],
        0, 1, 0
    );

	glFlush();
}



void draw_triangle(
    GLfloat x1, GLfloat y1, GLfloat z1,
    GLfloat x2, GLfloat y2, GLfloat z2,
    GLfloat x3, GLfloat y3, GLfloat z3
) {

	glBegin(GL_TRIANGLES);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    glEnd();
}

void Scene::icosahedron() {
    // these are the vertices, connected by lines in pairs as a visual guide
    // glBegin(GL_LINES);
    // // orange
    // glColor3f(1, 0.5, 0.0);
    // glVertex3f(   0,  1.0,  PHI);
    // glVertex3f(   0,  1.0, -PHI);
    // glVertex3f(   0, -1.0,  PHI);
    // glVertex3f(   0, -1.0, -PHI);
    // // green
    // glColor3f(0, 1.0, 0.5);
    // glVertex3f(   1,  PHI,    0);
    // glVertex3f(   1, -PHI,    0);
    // glVertex3f(  -1,  PHI,    0);
    // glVertex3f(  -1, -PHI,    0);
    // // cyan
    // glColor3f(0, 0.5, 1.0);
    // glVertex3f( PHI,    0,    1);
    // glVertex3f( PHI,    0,    -1);
    // glVertex3f(-PHI,    0,    1);
    // glVertex3f(-PHI,    0,   -1);
    // glEnd();

    // IDK how to document this, I got it by pairing up vertices from above
    // comment out a colour group to see my logic
    // it would have been REALLY cool if I could generate these on the fly
    // but that's harder maths
    glColor3f(1, 0.5, 0.0);
    draw_triangle( PHI, 0, 1,  PHI, 0, -1,  1, -PHI, 0);
    draw_triangle( PHI, 0, 1,  PHI, 0, -1,  1,  PHI, 0);
    draw_triangle(-PHI, 0, 1, -PHI, 0, -1, -1, -PHI, 0);
    draw_triangle(-PHI, 0, 1, -PHI, 0, -1, -1,  PHI, 0);

    glColor3f(0, 0, 1.0);
    draw_triangle(-PHI, 0,  1, -1, PHI, 0, 0, 1,  PHI);
    draw_triangle(-PHI, 0, -1, -1, PHI, 0, 0, 1, -PHI);
    draw_triangle( PHI, 0, -1,  1, PHI, 0, 0, 1, -PHI);
    draw_triangle( PHI, 0,  1,  1, PHI, 0, 0, 1,  PHI);

    glColor3f(1, 0, 0);
    draw_triangle(-PHI, 0,  1, -1, -PHI, 0, 0, -1,  PHI);
    draw_triangle(-PHI, 0, -1, -1, -PHI, 0, 0, -1, -PHI);
    draw_triangle( PHI, 0, -1,  1, -PHI, 0, 0, -1, -PHI);
    draw_triangle( PHI, 0,  1,  1, -PHI, 0, 0, -1,  PHI);

    glColor3f(0, 1, 0);
    draw_triangle( PHI, 0,  1, 0, 1,  PHI, 0, -1,  PHI);
    draw_triangle(-PHI, 0,  1, 0, 1,  PHI, 0, -1,  PHI);
    draw_triangle( PHI, 0, -1, 0, 1, -PHI, 0, -1, -PHI);
    draw_triangle(-PHI, 0, -1, 0, 1, -PHI, 0, -1, -PHI);

    glColor3f(1, 1, 0);
    draw_triangle(1,  PHI, 0, -1,  PHI, 0, 0,  1,  PHI);
    draw_triangle(1,  PHI, 0, -1,  PHI, 0, 0,  1, -PHI);
    draw_triangle(1, -PHI, 0, -1, -PHI, 0, 0, -1,  PHI);
    draw_triangle(1, -PHI, 0, -1, -PHI, 0, 0, -1, -PHI);
}
