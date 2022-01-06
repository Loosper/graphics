#include "Scene.h"

#include <math.h>
#include <iostream>
#include <algorithm>

#include <Qt>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>
#include <QApplication>

#include <QDebug>

#define RAD(x) (x * M_PI / 180)
// https://gamedev.stackexchange.com/questions/43588/how-to-rotate-camera-centered-around-the-cameras-position
// Based on these. I don't touch the up vector for now. Works only in a single axis
// #define X_OFF (cos(RAD(yaw_angle)))
// #define Y_OFF (                      sin(RAD(pitch_angle)))
// #define Z_OFF (sin(RAD(yaw_angle)) + cos(RAD(pitch_angle)))
#define X_OFF (cos(RAD(yaw_angle)))
#define Y_OFF (                  0)
#define Z_OFF (sin(RAD(yaw_angle)))


extern struct material smooth_material;
extern struct material light1;
GLfloat light_pos[] = {0, 0, 0, 1};


Scene::Scene(): QGLWidget(), Drawer() {
    QTimer *render_timer = new QTimer(this);
    QTimer *movement_timer = new QTimer(this);

    connect(render_timer, &QTimer::timeout, this, &Scene::updateGL);
    connect(movement_timer, &QTimer::timeout, this, &Scene::tick_movement);
    // force update at ~60 fps
    // so I don't have to manually repaint whenever i change shit
    render_timer->start(17);
    // so that it's evently spaced and easy to think about
    movement_timer->start(10);
}
void Scene::tick_movement() {
    solar.advance_movement();
}

void Scene::keyPressEvent(QKeyEvent *event) {
    QVector3D direction(X_OFF / 10, 0, Z_OFF / 10);

    if (event->key() == Qt::Key_W) {
        camera_pos += direction;
    } else if (event->key() == Qt::Key_S) {
        camera_pos -= direction;
    } else if (event->key() == Qt::Key_Escape) {
        QApplication::restoreOverrideCursor();
        setMouseTracking(false);
    } else if (event->key() == Qt::Key_Space) {
        camera_pos.setY(camera_pos.y() + 0.1);
    } else if (event->key() == Qt::Key_C) {
        camera_pos.setY(camera_pos.y() - 0.1);
    } else if (event->key() == Qt::Key_Shift) {
        if (glIsEnabled(GL_LIGHTING)) {
            glDisable(GL_LIGHTING);
        } else {
            glEnable(GL_LIGHTING);
        }
    }

    QGLWidget::keyPressEvent(event);
}

void Scene::mousePressEvent(QMouseEvent *event) {
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

    cursor.setPos(mapToGlobal(rect().center()));
}

void Scene::initializeGL() {
    glClearColor(0, 0, 0, 0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1.ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1.diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1.specular);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    solar.gl_init();
}

void Scene::resizeGL(int w, int h) {
    GLfloat ratio = (GLfloat)w / h;

    // doesn't relate to the projection matrix
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 100);
}


void Scene::draw_geometry() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_material(smooth_material);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
    glEnd();
    glColor3f(1, 1, 1);

    solar.draw_geometry();
}

void Scene::paintGL() {
    // TODO: this for mulitaxis rotation. IDK what tf it does
    // QQuaternion rot = QQuaternion::fromAxisAndAngle(0, 0, 1, yaw_angle) * QQuaternion::fromAxisAndAngle(0, 1, 0, pitch_angle);
    // QVector3D look_at = rot.rotatedVector(QVector3D(0, 1, 0));
    // QVector3D look_at(0, 0, 1);
    // up_v = QVector3D::crossProduct(look_at, up_v);

    QVector3D look_at(X_OFF, Y_OFF, Z_OFF);
    QVector3D up_v(0, 1, 0);
    look_at += camera_pos;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // why does this not work?
    // glRotatef(yaw_angle, 0, 1, 0);
    // glRotatef(pitch_angle, 0, 0, 1);

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
	gluLookAt(
        camera_pos.x(), camera_pos.y(), camera_pos.z(),
        look_at.x(), look_at.y(), look_at.z(),
        up_v.x(), up_v.y(), up_v.z()
    );

    glTranslatef(0, 0, -20);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    draw_geometry();

	glFlush();
}

