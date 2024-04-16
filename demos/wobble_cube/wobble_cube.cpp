#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

/**
 * The main demo class definition.
 */
class wobble_cube_demo : public mass_aggregate_application
{
public:
    struct wobble_cube {
        ffiseg::particle* parts;
        ffiseg::particle_pseudo_spring* springs;
        ffiseg::particle_damper* dampers;
    };

    wobble_cube slg;

    /** Creates a new demo object. */
    wobble_cube_demo();
    virtual ~wobble_cube_demo();

    /** Returns the window title for the demo. */
    virtual const char* get_title();
    virtual void update();
    virtual void display();

};

// Method definitions
wobble_cube_demo::wobble_cube_demo() :
mass_aggregate_application(9) {

    slg.parts = particle_array;
    auto length = 4.0f;
    particle_array[0].set_position(0 ,2, 4);
    particle_array[1].set_position(4 ,1, 4);
    particle_array[2].set_position(4 ,1, 0);
    particle_array[3].set_position(0 ,1, 0);
    particle_array[4].set_position(0 ,4, 4);
    particle_array[5].set_position(4 ,5, 4);
    particle_array[6].set_position(4 ,5, 0);
    particle_array[7].set_position(0 ,5, 0);
    particle_array[8].set_position(2 ,2, 2);

    for(int i = 0; i < 8; ++i) {
        auto acc = ffiseg::vector(0,-10,0);
        particle_array[i].set_acceleration(acc);
        particle_array[i].set_mass(3);
        particle_array[i].set_damping(1.0f);
    }

    slg.springs = new ffiseg::particle_pseudo_spring[40];
    slg.dampers = new ffiseg::particle_damper[40];
    auto spring_constant = 300;
    auto damper_constant = 7;

    for(int i = 0; i < 4; ++i) {
        slg.springs[i].set_other(&slg.parts[i]);
        slg.springs[i].set_spring_constant(spring_constant);
        slg.springs[i].set_length(length);
        slg.dampers[i].set_other(&slg.parts[i]);
        slg.dampers[i].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[4 + i], &slg.springs[i]);
        add_force_gen_to_registry(&slg.parts[4 + i], &slg.dampers[i]);

        slg.springs[4 + i].set_other(&slg.parts[4 + i]);
        slg.springs[4 + i].set_spring_constant(spring_constant);
        slg.springs[4 + i].set_length(length);
        slg.dampers[4 + i].set_other(&slg.parts[4 + i]);
        slg.dampers[4 + i].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[i], &slg.springs[4 + i]);
        add_force_gen_to_registry(&slg.parts[i], &slg.dampers[4 + i]);
    }
    for(int i = 0; i < 8; ++i) {
        int j;
        if(i % 2 == 0) {
            j = i + 1;
        } else {
            j = i - 1;
        }
        slg.springs[i + 8].set_other(&slg.parts[i]);
        slg.springs[i + 8].set_spring_constant(spring_constant);
        slg.springs[i + 8].set_length(length);
        slg.dampers[i + 8].set_other(&slg.parts[i]);
        slg.dampers[i + 8].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[j], &slg.springs[i + 8]);
        add_force_gen_to_registry(&slg.parts[j], &slg.dampers[i + 8]);
    }

    for(int i = 0; i < 4; ++i) {
        int j  = 3 - i; 
        slg.springs[i + 16].set_other(&slg.parts[i]);
        slg.springs[i + 16].set_spring_constant(spring_constant);
        slg.springs[i + 16].set_length(length);
        slg.dampers[i + 16].set_other(&slg.parts[i]);
        slg.dampers[i + 16].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[j], &slg.springs[i + 16]);
        add_force_gen_to_registry(&slg.parts[j], &slg.dampers[i + 16]);
    }

    for(int k = 0; k < 4; ++k) {
        int i = 4 + k;
        int j  = 7 - k;
        slg.springs[k + 20].set_other(&slg.parts[i]);
        slg.springs[k + 20].set_spring_constant(spring_constant);
        slg.springs[k + 20].set_length(length);
        slg.dampers[k + 20].set_other(&slg.parts[i]);
        slg.dampers[k + 20].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[j], &slg.springs[k + 20]);
        add_force_gen_to_registry(&slg.parts[j], &slg.dampers[k + 20]);
    }

    for(int i = 0; i < 8; ++i) {
        slg.springs[i + 24].set_other(&slg.parts[i]);
        slg.springs[i + 24].set_spring_constant(spring_constant);
        slg.springs[i + 24].set_length(3.47);
        slg.dampers[i + 24].set_other(&slg.parts[i]);
        slg.dampers[i + 24].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[8], &slg.springs[i + 24]);
        add_force_gen_to_registry(&slg.parts[8], &slg.dampers[i + 24]);
    }

    for(int i = 0; i < 8; ++i) {
        slg.springs[i + 32].set_other(&slg.parts[8]);
        slg.springs[i + 32].set_spring_constant(spring_constant);
        slg.springs[i + 32].set_length(3.47);
        slg.dampers[i + 32].set_other(&slg.parts[8]);
        slg.dampers[i + 32].set_damper_constant(damper_constant);
        add_force_gen_to_registry(&slg.parts[i], &slg.springs[i + 32]);
        add_force_gen_to_registry(&slg.parts[i], &slg.dampers[i + 32]);
    }
}

wobble_cube_demo::~wobble_cube_demo() {}


const char* wobble_cube_demo::get_title()
{
    return "Ffiseg > wobble_cube_demo";
}

void wobble_cube_demo::update() {
    mass_aggregate_application::update();
}

void wobble_cube_demo::display() {
    mass_aggregate_application::display();

    auto fps = ffiseg::timer::get().fps;
    std::string fps_string = std::to_string(fps);

    glColor3f(0.0f, 0.0f, 0.0f);
    render_text(10.0f, 34.0f, fps_string.c_str());
}

application* get_application()
{
    return new wobble_cube_demo();
}
