#include "Scene.h"

#include <math.h>
#include <iostream>
#include <algorithm>

#include <Qt>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>
#include <QApplication>
#include <QImage>

#include <QDebug>

#define PHI 1.618
#define RAD(x) (x * M_PI / 180)
// https://gamedev.stackexchange.com/questions/43588/how-to-rotate-camera-centered-around-the-cameras-position
// Based on these. I don't touch the up vector for now. Works only in a single axis
// #define X_OFF (cos(RAD(yaw_angle)))
// #define Y_OFF (                      sin(RAD(pitch_angle)))
// #define Z_OFF (sin(RAD(yaw_angle)) + cos(RAD(pitch_angle)))
#define X_OFF (cos(RAD(yaw_angle)))
#define Y_OFF (                  0)
#define Z_OFF (sin(RAD(yaw_angle)))

using namespace std;


static struct material smooth_material = {
    {0.5, 0.5, 0.5, 1.0},
    {0.5, 0.5, 0.5, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    3.0
};

GLfloat light_pos[] = {10, 3, 50, 1};

GLuint markus_tex;
GLuint mark_tex;
GLuint earth_tex;

Scene::Scene(): QGLWidget(), Drawer() {
    // force update at ~60 fps
    // so I don't have to manually repaint whenever i change shit
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::updateGL);
    timer->start(17);
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
    glClearColor(0.3, 0.3, 0.3, 0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    // VERY useful for debug
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    load_texture("markus.ppm", &markus_tex);
    load_texture("Marc_Dekamps.ppm", &mark_tex);
    load_texture("Mercator-projection.ppm", &earth_tex);

}


void Scene::load_texture(const char *file, GLuint *tex) {
    QImage image(file, "ppm");

    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA,
        GL_UNSIGNED_BYTE, QGLWidget::convertToGLFormat(image).bits()
    );
}

void Scene::set_material(struct material &material) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,  material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
}

void Scene::resizeGL(int w, int h) {
    GLfloat ratio = (GLfloat)w / h;

    // doesn't relate to the projection matrix
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-10, 10, -10, 10, -10, 10);
    gluPerspective(60, ratio, 0.1, 30);
}

void draw_triangle(QVector3D v1, QVector3D v2, QVector3D v3) {
    QVector3D normal = QVector3D::normal(v1, v2, v3);
    // qDebug() << normal;
	glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(v1.x(), v1.y(), v1.z());
        glVertex3f(v2.x(), v2.y(), v2.z());
        glVertex3f(v3.x(), v3.y(), v3.z());
    glEnd();
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

    // glColor3f(1, 1, 1);
    // glPushMatrix();
    //     glTranslatef(0, 5, -5);
    //     this->icosahedron();
    //     glTranslatef(3, 0, 0);
    //     glEnable(GL_TEXTURE_2D);
    //     glBindTexture(GL_TEXTURE_2D, markus_tex);
    //     glColor3f(1, 1, 1);
    //     this->cube();
    //     glTranslatef(3, 0, 0);
    //     glBindTexture(GL_TEXTURE_2D, earth_tex);
    //     sphere();
    //     glTranslatef(3, 0, 0);
    //     cylinder();
    //     glDisable(GL_TEXTURE_2D);
    // glPopMatrix();


    // draw_triangle(QVector3D(-10, 0, -10), QVector3D(0, 0, 10), QVector3D(10, 0, -10));
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

    draw_geometry();
    rocket.draw_geometry();

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // why does this not work?
    // glRotatef(yaw_angle, 0, 1, 0);
    // glRotatef(pitch_angle, 0, 0, 1);

	gluLookAt(
        camera_pos.x(), camera_pos.y(), camera_pos.z(),
        look_at.x(), look_at.y(), look_at.z(),
        up_v.x(), up_v.y(), up_v.z()
    );

    glScalef(0.5, 0.5, 0.5);
	glFlush();
}


void Scene::icosahedron() {
    // naming scheme is by y. Lvl 1 is at the bottom and we go up. l/r are left/right, f/b are forward/backward
    QVector3D
        l1_l(-1, -PHI, 0), l1_r(1, -PHI, 0),
        l2_f(0, -1, PHI), l2_b(0, -1, -PHI),
        l3_lf(-PHI, 0, 1), l3_lb(-PHI, 0, -1), l3_rf(PHI, 0, 1), l3_rb(PHI, 0, -1),
        l4_f(0, 1, PHI), l4_b(0, 1, -PHI),
        l5_l(-1, PHI, 0), l5_r(1, PHI, 0);

    // // guides
    // glBegin(GL_LINES);
    // // green
    // glColor3f(0, 1.0, 0.5);
    // glVertex3f(l5_r.x(), l5_r.y(), l5_r.z());
    // glVertex3f(l5_l.x(), l5_l.y(), l5_l.z());
    // glVertex3f(l1_r.x(), l1_r.y(), l1_r.z());
    // glVertex3f(l1_l.x(), l1_l.y(), l1_l.z());
    // // cyan
    // glColor3f(0, 0.5, 1.0);
    // glVertex3f(l3_lf.x(), l3_lf.y(), l3_lf.z());
    // glVertex3f(l3_lb.x(), l3_lb.y(), l3_lb.z());
    // glVertex3f(l3_rf.x(), l3_rf.y(), l3_rf.z());
    // glVertex3f(l3_rb.x(), l3_rb.y(), l3_rb.z());
    // // orange
    // glColor3f(1, 0.5, 0.0);
    // glVertex3f(l4_f.x(), l4_f.y(), l4_f.z());
    // glVertex3f(l4_b.x(), l4_b.y(), l4_b.z());
    // glVertex3f(l2_f.x(), l2_f.y(), l2_f.z());
    // glVertex3f(l2_b.x(), l2_b.y(), l2_b.z());
    // glEnd();

    // glColor3f(1, 0.5, 0.0); // orange
    draw_triangle(l5_r, l5_l, l4_f);
    draw_triangle(l1_r, l2_f, l1_l);
    draw_triangle(l5_r, l5_l, l4_b);
    draw_triangle(l1_l, l1_r, l2_b);
    // glColor3f(0, 0, 1.0); // blue
    draw_triangle(l2_f, l1_r, l3_rf);
    draw_triangle(l1_l, l2_f, l3_lf);
    draw_triangle(l2_b, l1_r, l3_rb);
    draw_triangle(l1_l, l2_b, l3_lb);
    // glColor3f(1, 0, 0); // red
    draw_triangle(l3_lf, l2_f, l4_f);
    draw_triangle(l3_rf, l4_f, l2_f);
    draw_triangle(l3_lb, l2_b, l4_b);
    draw_triangle(l3_rb, l4_b, l2_b);
    // glColor3f(0, 1, 0); // green
    draw_triangle(l5_r, l4_f,  l3_rf);
    draw_triangle(l3_lf, l4_f,  l5_l);
    draw_triangle(l3_rb,  l5_r, l4_b);
    draw_triangle(l5_l,  l3_lb, l4_b);
    // glColor3f(1, 1, 0); // yellow
    draw_triangle(l3_lf, l3_lb, l1_l);
    draw_triangle(l3_lb, l3_lf, l5_l);
    draw_triangle(l3_rb, l3_rf, l1_r);
    draw_triangle(l3_rf, l3_rb, l5_r);
}

void Scene::cube() {
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
