#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>


class spring_demo : public application {
public:
    spring_demo();
    virtual const char* get_title();
    virtual void update();
    virtual void display();
    virtual void mouse(int button, int state, int x, int y);
    virtual void key(unsigned char key);

private:
    struct weight {
        ffiseg::particle* part;

        void render() {
            ffiseg::vector pos;
            part->get_position(&pos);

            glColor3f(0, 0, 0);
            glPushMatrix();
            glTranslatef(pos.get_x(), pos.get_y(), pos.get_z());
            glutSolidSphere(0.3f, 5, 4);
            glPopMatrix();
        }
    };

    ffiseg::vector fixed_end;
    ffiseg::particle_anchored_spring spring;
    weight free_end;
};

spring_demo::spring_demo() : 
             fixed_end(1.0, 1.0, 1.0), 
             spring(fixed_end, 3.0, 3.0) {
    
    ffiseg::particle weight_particle(1.0, 1.0, 1.0);
    weight_particle.set_mass(10.0);
    free_end.part = &weight_particle;
}

const char* spring_demo::get_title() {
    return "ffsieg > Spring Demo";
}

void spring_demo::update() {
    float duration = (float)ffsieg::timer::get().last_frame_duration * 0.001f;
    if(duration <= 0.0f) return;
    spring.update(free_end.part, duration);

    application::update();
}

void spring_demo::dispay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-25.0, 8.0, 5.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

    free_end->render();
}

application* get_application() {
    return new spring_demo();
}

