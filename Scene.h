#include <QGLWidget>
#include <QPoint>
#include <QVector3D>
#include <QSlider>

#include <GL/glu.h>

#include "Drawer.h"
#include "SolarSystem.h"


class Scene: public QGLWidget, public Drawer {
    public:
        Scene();
        void draw_geometry();
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

        void gl_init() {}
        void tick_movement();

	private:
        QSlider *tilt_slider(void (SolarSystem::*method)(int));
        QSlider *speed_slider(void (SolarSystem::*method)(int));
        // left/right
        GLfloat yaw_angle = -90;
        // up/down
        GLfloat pitch_angle = 0;
        QCursor cursor = QCursor(Qt::BlankCursor);
        QVector3D camera_pos = {0, 0.3, 2};
        SolarSystem solar;
};
