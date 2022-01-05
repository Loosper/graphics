#include <QGLWidget>
#include <QPoint>
#include <QVector3D>

#include <GL/glu.h>


// no emissive
struct material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
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
        void mousePressEvent(QMouseEvent *event);

	private:
        // left/right
        GLfloat yaw_angle = -90;
        // up/down
        GLfloat pitch_angle = 0;
        QCursor cursor = QCursor(Qt::BlankCursor);
        QVector3D camera_pos = {0, 0.3, 2};
        GLUquadricObj *quad;

		void icosahedron();
		void cube();
		void sphere();
		void ring(GLfloat slope, GLfloat radius = 1);
		void disk();

        void rocket();
        void top_cone();

        void load_texture(const char *file, GLuint *tex);
        void set_material(struct material &material);
};
