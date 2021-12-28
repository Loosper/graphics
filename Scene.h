#include <QGLWidget>
#include <QPoint>


struct point {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class Scene: public QGLWidget {
    public:
        Scene();
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void enterEvent(QEvent *event);

	private:
        // left/right
        GLfloat yaw_angle = 90;
        // up/down
        GLfloat pitch_angle = 0;
        QCursor cursor = QCursor(Qt::BlankCursor);
        point camera_pos = {0, 3, 0};

		void icosahedron();
};
