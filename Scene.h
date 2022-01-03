#include <QGLWidget>
#include <QPoint>
#include <QVector3D>


class Scene: public QGLWidget {
    public:
        Scene();
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

	private:
        // left/right
        GLfloat yaw_angle = -90;
        // up/down
        GLfloat pitch_angle = 0;
        QCursor cursor = QCursor(Qt::BlankCursor);
        QVector3D camera_pos = {0, 3, 0};

		void icosahedron();
		void cube();
};
