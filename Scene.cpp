#include "Scene.h"

#include <math.h>
#include <iostream>
#include <algorithm>

#include <GL/glu.h>
#include <Qt>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>
#include <QApplication>

#include <QDebug>

#define PHI 1.618
#define RAD(x) (x * M_PI / 180)
// https://gamedev.stackexchange.com/questions/43588/how-to-rotate-camera-centered-around-the-cameras-position
// Based on these. I don't touch the up vector for now
#define X_OFF (cos(RAD(yaw_angle)))
#define Y_OFF (                      sin(RAD(pitch_angle)))
#define Z_OFF (sin(RAD(yaw_angle)) + cos(RAD(pitch_angle)))

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

// ^C ^V from stackoverflow cos c++17 is lowest required and idk if that's allowed on feng
template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
      return std::max(lower, std::min(n, upper));
}

Scene::Scene(): QGLWidget() {
    // force update at ~60 fps
    // so I don't have to manually repaint whenever i change shit
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::updateGL);
    timer->start(17);

}

void Scene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        camera_pos.x += X_OFF / 10;
        camera_pos.y += Y_OFF / 10;
        camera_pos.z += Z_OFF / 10;
    } else if (event->key() == Qt::Key_S) {
        camera_pos.x -= X_OFF / 10;
        camera_pos.y -= Y_OFF / 10;
        camera_pos.z -= Z_OFF / 10;
    } else if (event->key() == Qt::Key_Escape) {
        QApplication::restoreOverrideCursor();
        setMouseTracking(false);
    }

    // qDebug() << camera_pos.x << camera_pos.y << camera_pos.y;

    QGLWidget::keyPressEvent(event);
}

void Scene::enterEvent(QEvent *event) {
    event->accept();

    QApplication::setOverrideCursor(cursor);
    cursor.setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);
}

void Scene::mouseMoveEvent(QMouseEvent *event) {
    QPoint new_mouse = event->pos();
    event->accept();

    yaw_angle   += - (rect().center().x() - new_mouse.x()) / 10;
    pitch_angle +=   (rect().center().y() - new_mouse.y()) / 20;

    yaw_angle = clamp(yaw_angle, -360.f, 360.f);
    pitch_angle = clamp(pitch_angle, -360.f, 360.f);

    cursor.setPos(mapToGlobal(rect().center()));
}

void Scene::initializeGL() {
    glClearColor(0.1, 0.1, 0.1, 0);

    // GLfloat light_pos[] = {2., -2.,-2., 1.};
    // glEnable(GL_LIGHTING); // enable lighting in general
    // glEnable(GL_LIGHT0);   // each light source must also be enabled
    // glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glEnable(GL_DEPTH_TEST);
}

void Scene::resizeGL(int w, int h) {
    GLfloat ratio = (GLfloat)w / h;

    // doesn't relate to the projection matrix
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-10, 10, -10, 10, -10, 10);
    gluPerspective(60, ratio, 1, 10);
}

void Scene::paintGL() {
    point look_at;
    look_at.x = camera_pos.x + X_OFF;
    look_at.y = camera_pos.y + Y_OFF;
    look_at.z = camera_pos.z + Z_OFF;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 3, 10);
    this->icosahedron();
    glPopMatrix();

    glColor3f(1, 1, 1);
    draw_triangle(-100, -1, -100, -1, 0, 100, 100, -1, -100);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
        camera_pos.x, camera_pos.y, camera_pos.z,
        look_at.x, look_at.y, look_at.z,
        0, 1, 0
        // 0, look_at.z, -look_at.y
    );

    glScalef(0.5, 0.5, 0.5);
	glFlush();
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
