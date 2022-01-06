#include "Drawer.h"
#include "Rocket.h"


class SolarSystem: public Drawer {
    public:
        SolarSystem(): Drawer() {}

        void advance_movement();

        void draw_geometry();
        void gl_init();
    private:
        Rocket rocket = Rocket();
        GLfloat earth_rot = 0;
        GLfloat earth_orbit = 0;
        GLfloat orbit = 0;

        void saturn();
        void earth();
		void icosahedron();
};
