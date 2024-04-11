#include<ffiseg/ffiseg.hpp>

#include"../application.hpp"
#include"../ogl_headers.hpp"

#include <stdio.h>

class ballistic_demo : public application {
    enum shot_type {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    };

    struct ammo_round {
        ffiseg::particle part;
        shot_type type;
        unsigned start_time;

        void render() {
            ffiseg::vector pos;
            part.get_position(&pos);

            glColor3f(0, 0, 0);
            glPushMatrix();
            glTranslatef(pos.get_x(), pos.get_y(), pos.get_z());
            glutSolidSphere(0.3f, 5, 4);
            glPopMatrix();

            glColor3f(0.75, 0.75, 0.75);
            glPushMatrix();
            glTranslatef(pos.get_x(), 0, pos.get_z());
            glScalef(1.0f, 0.1f, 1.0f);
            glutSolidSphere(0.6f, 5, 4);
            glPopMatrix();
        }
    };

    const static unsigned ammo_rounds = 16;

    ammo_round ammo[ammo_rounds];
    shot_type current_shot_type;
    void fire();

public:
    ballistic_demo();
    virtual const char* get_title();
    virtual void update();
    virtual void display();
    virtual void mouse(int button, int state, int x, int y);
    virtual void key(unsigned char key);
};

ballistic_demo::ballistic_demo() : current_shot_type(LASER) {
    for (ammo_round* shot = ammo; shot < ammo + ammo_rounds; shot++) {
        shot->type = UNUSED;
    }
}

const char* ballistic_demo::get_title()
{
    return "ffiseg > Ballistic Demo";
}

void ballistic_demo::fire()
{
    // Find the first available round.
    ammo_round *shot;
    for (shot = ammo; shot < ammo + ammo_rounds; shot++)
    {
        if (shot->type == UNUSED) break;
    }

    // If we didn't find a round, then exit - we can't fire.
    if (shot >= ammo + ammo_rounds) return;

    // Set the properties of the particle
    switch(current_shot_type) {
    case PISTOL:
        shot->part.set_mass(2.0f); // 2.0kg
        shot->part.set_velocity(0.0f, 0.0f, 35.0f); // 35m/s
        shot->part.set_acceleration(0.0f, -1.0f, 0.0f);
        shot->part.set_damping(0.99f);
        break;

    case ARTILLERY:
        shot->part.set_mass(200.0f); // 200.0kg
        shot->part.set_velocity(0.0f, 30.0f, 40.0f); // 50m/s
        shot->part.set_acceleration(0.0f, -20.0f, 0.0f);
        shot->part.set_damping(0.99f);
        break;

    case FIREBALL:
        shot->part.set_mass(1.0f); // 1.0kg - mostly blast damage
        shot->part.set_velocity(0.0f, 0.0f, 10.0f); // 5m/s
        shot->part.set_acceleration(0.0f, 0.6f, 0.0f); // Floats up
        shot->part.set_damping(0.9f);
        break;

    case LASER:
        // Note that this is the kind of laser bolt seen in films,
        // not a realistic laser beam!
        shot->part.set_mass(0.1f); // 0.1kg - almost no weight
        shot->part.set_velocity(0.0f, 0.0f, 100.0f); // 100m/s
        shot->part.set_acceleration(0.0f, 0.0f, 0.0f); // No gravity
        shot->part.set_damping(0.99f);
        break;
    }

    // Set the data common to all particle types
    shot->part.set_position(0.0f, 1.5f, 0.0f);
    shot->start_time = ffiseg::timer::get().last_frame_timestamp;
    shot->type = current_shot_type;

    // Clear the force accumulators
    shot->part.clear_accumulator();
}

void ballistic_demo::update()
{
    // Find the duration of the last frame in seconds
    float duration = (float)ffiseg::timer::get().last_frame_duration * 0.001f;
    if (duration <= 0.0f) return;

    // Update the physics of each particle in turn
    for (ammo_round *shot = ammo; shot < ammo + ammo_rounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            // Run the physics
            shot->part.integrate(duration);

            // Check if the particle is now invalid
            if (shot->part.get_position().get_y() < 0.0f ||
                shot->start_time + 10000 < ffiseg::timer::get().last_frame_timestamp ||
                shot->part.get_position().get_z() > 500.0f)
            {
                // We simply set the shot type to be unused, so the
                // memory it occupies can be reused by another shot.
                shot->type = UNUSED;
            }
        }
    }

    application::update();
}

void ballistic_demo::display() {
    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-25.0, 8.0, 5.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

    // Draw a sphere at the firing point, and add a shadow projected
    // onto the ground plane.
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glutSolidSphere(0.1f, 5, 5);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidSphere(0.1f, 5, 5);
    glPopMatrix();

    // Draw some scale lines
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINES);
    for (unsigned i = 0; i < 200; i += 10) {
        glVertex3f(-5.0f, 0.0f, i);
        glVertex3f(5.0f, 0.0f, i);
    }
    glEnd();

    // Render each particle in turn
    for (ammo_round *shot = ammo; shot < ammo + ammo_rounds; shot++) {
        if (shot->type != UNUSED) {
            shot->render();
        }
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    render_text(10.0f, 34.0f, "Click: Fire\n1-4: Select Ammo");

    switch(current_shot_type) {
        case PISTOL: render_text(10.0f, 10.0f, "Current Ammo: Pistol"); break;
        case ARTILLERY: render_text(10.0f, 10.0f, "Current Ammo: Artillery"); break;
        case FIREBALL: render_text(10.0f, 10.0f, "Current Ammo: Fireball"); break;
        case LASER: render_text(10.0f, 10.0f, "Current Ammo: Laser"); break;
    }
}

void ballistic_demo::mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) fire();
}

void ballistic_demo::key(unsigned char key) {
    switch(key) {
        case '1': current_shot_type = PISTOL; break;
        case '2': current_shot_type = ARTILLERY; break;
        case '3': current_shot_type = FIREBALL; break;
        case '4': current_shot_type = LASER; break;
    }
}

application* get_application() {
    return new ballistic_demo();
}
