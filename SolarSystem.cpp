#include "SolarSystem.h"

#include <math.h>
#include <QVector3D>

#define PHI 1.618

GLuint earth_tex;
GLuint saturn_tex;
GLuint rings_tex;
GLuint markus_tex;
GLuint marc_tex;
extern struct material sun_material;

void SolarSystem::advance_movement() {
    earth_rot -= 0.2;
    earth_orbit += 0.01;
    orbit -= 0.003;
}

void SolarSystem::gl_init() {
    rocket.gl_init();
    load_texture("earth.ppm", earth_tex);
    load_texture("saturn.jpg", saturn_tex);
    load_texture("jellys.jpg", rings_tex);
    load_texture("markus.ppm", markus_tex);
    load_texture("marc.ppm", marc_tex);
}

void SolarSystem::saturn() {
    glPushMatrix();
        // this is saturn after all
        glRotatef(26.73, 0, 0, 1);
        enable_texture(saturn_tex);
        sphere();
        disable_texture();

        glScalef(2, 1, 2);
        enable_texture(rings_tex);
        disk(0.75);
        glRotatef(180, 1, 0, 0);
        disk(0.75);
        disable_texture();
    glPopMatrix();
}

void SolarSystem::earth() {
    glPushMatrix();
        glRotatef(earth_rot, 0, 1, 0);
        enable_texture(earth_tex);
        sphere();
        disable_texture();
    glPopMatrix();
}


void SolarSystem::draw_geometry() {
    struct material old;

    glPushMatrix();
        glPushMatrix();
            old = set_material(sun_material);
            glRotatef(earth_rot, 0, 1, 0);
            icosahedron();
            set_material(old);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(sin(orbit) * 5, 0, cos(orbit) * 5);
            enable_texture(markus_tex);
            sphere();
            disable_texture();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(sin(orbit) * 25, 0, cos(orbit) * 25);
            enable_texture(marc_tex);
            sphere();
            disable_texture();
        glPopMatrix();

        glTranslatef(sin(orbit) * 10, 0, cos(orbit) * 10);
        glPushMatrix();
            glScalef(4, 4, 4);
            saturn();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(sin(earth_orbit) * 13, 0, cos(earth_orbit) * 13);
            earth();
            glTranslatef(0, 0.95, 0);
            glScalef(0.3, 0.3, 0.3);
            glRotatef(180, 0, 1, 0);
            rocket.draw_geometry();
        glPopMatrix();
    glPopMatrix();
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

void SolarSystem::icosahedron() {
    // naming scheme is by y. Lvl 1 is at the bottom and we go up. l/r are left/right, f/b are forward/backward
    QVector3D
        l1_l(-1, -PHI, 0), l1_r(1, -PHI, 0),
        l2_f(0, -1, PHI), l2_b(0, -1, -PHI),
        l3_lf(-PHI, 0, 1), l3_lb(-PHI, 0, -1), l3_rf(PHI, 0, 1), l3_rb(PHI, 0, -1),
        l4_f(0, 1, PHI), l4_b(0, 1, -PHI),
        l5_l(-1, PHI, 0), l5_r(1, PHI, 0);

    // glColor3f(1, 0.5, 0.0); // orange
    draw_triangle(l5_r, l5_l, l4_f);
    draw_triangle(l1_r, l2_f, l1_l);
    draw_triangle(l4_b, l5_l, l5_r);
    draw_triangle(l2_b, l1_r, l1_l);
    // glColor3f(0, 0, 1.0); // blue
    draw_triangle(l2_f, l1_r, l3_rf);
    draw_triangle(l1_l, l2_f, l3_lf);
    draw_triangle(l3_rb, l1_r, l2_b);
    draw_triangle(l3_lb, l2_b, l1_l);
    // glColor3f(1, 0, 0); // red
    draw_triangle(l3_lf, l2_f, l4_f);
    draw_triangle(l3_rf, l4_f, l2_f);
    draw_triangle(l4_b, l2_b, l3_lb);
    draw_triangle(l2_b, l4_b, l3_rb);
    // glColor3f(0, 1, 0); // green
    draw_triangle(l5_r,  l4_f, l3_rf);
    draw_triangle(l3_lf, l4_f, l5_l);
    draw_triangle(l3_rb, l4_b, l5_r);
    draw_triangle(l5_l,  l4_b, l3_lb);
    // glColor3f(1, 1, 0); // yellow
    draw_triangle(l3_lf, l3_lb, l1_l);
    draw_triangle(l3_lb, l3_lf, l5_l);
    draw_triangle(l3_rb, l3_rf, l1_r);
    draw_triangle(l3_rf, l3_rb, l5_r);
}

