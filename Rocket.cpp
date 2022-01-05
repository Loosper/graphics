#include "Rocket.h"

void Rocket::nose_cone() {
    glPushMatrix();
        ring(0.7);
        glTranslatef(0, 1, 0);
        glScalef(0.7, 0.7, 0.7);
        sphere();
    glPopMatrix();
}

void Rocket::booster_bot(int length) {
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        disk();
    glPopMatrix();
    ring(0.9);
    glPushMatrix();
        glTranslatef(0, 1, 0);
        glScalef(0.9, length, 0.9);
        ring(1);
    glPopMatrix();
}

void Rocket::booster() {
    booster_bot(10);
    glPushMatrix();
        glTranslatef(0, 11, 0);
        glScalef(0.9, 1, 0.9);
        nose_cone();
    glPopMatrix();
}

void Rocket::draw_geometry() {
    glTranslatef(0, -6, 0);

    glPushMatrix();
        glTranslatef(-2, 0, 0);
        booster();
        glTranslatef(4, 0, 0);
        booster();
    glPopMatrix();

    glPushMatrix();
        booster_bot(16);
        glTranslatef(0, 16, 0);

        ring(1.1, 0.9);
        glTranslatef(0, 1, 0);

        glPushMatrix();
            glScalef(1.1, 2, 1.1);
            ring(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, 2, 0);
            glScalef(1.1, 1, 1.1);
            nose_cone();
        glPopMatrix();
    glPopMatrix();

}
