#include <QGLWidget>
#include <QPoint>
#include <QVector3D>

#include <GL/glu.h>

#include "Drawer.h"
#include "Rocket.h"


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

	private:
        // left/right
        GLfloat yaw_angle = -90;
        // up/down
        GLfloat pitch_angle = 0;
        QCursor cursor = QCursor(Qt::BlankCursor);
        QVector3D camera_pos = {0, 0.3, 2};
        Rocket rocket = Rocket();

		void icosahedron();
};
