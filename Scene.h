#include <QGLWidget>


class Scene: public QGLWidget {
    public:
        Scene();
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void keyPressEvent(QKeyEvent *event);

	private:
        int angle;
        GLfloat camera[2];
		void icosahedron();
};
