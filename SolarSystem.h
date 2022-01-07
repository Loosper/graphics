#include "Drawer.h"
#include "Rocket.h"

#include <QWidget>


class SolarSystem: public QWidget, public Drawer {
    public:
        SolarSystem(): QWidget(), Drawer() {}

        void advance_movement();

        void draw_geometry();
        void gl_init();
        void set_saturn_speed(int speed);
        void set_marc_speed(int speed);
        void set_marcus_speed(int speed);
        void set_earth_speed(int speed);
        void set_earth_tilt(int tilt);
        void set_earth_orbit_tilt(int tilt);
        void set_earth_spin(int spin);
        void pause();
    private:
        Rocket rocket = Rocket();
        GLfloat sun_rot = 0;
        GLfloat earth_rot = 0;
        GLfloat default_rot = 0;
        GLfloat earth_orbit = 0;
        GLfloat saturn_orbit = 0;
        GLfloat marc_orbit = 0;
        GLfloat marcus_orbit = 0;
        bool paused = false;

        GLint saturn_orbit_speed = 0;
        GLint marc_orbit_speed = 0;
        GLint marcus_orbit_speed = 0;
        GLint earth_orbit_speed  = 0;
        GLint earth_tilt_angle   = 0;
        GLint earth_orbit_tilt_angle = 0;
        GLint earth_spin_speed   = 0;

        void saturn();
        void earth();
		void icosahedron();
};
