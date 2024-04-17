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
mass_aggregate_application(1000) {

    slg.parts = particle_array;
    auto length = 1.0f;

    unsigned n = 10;
    slg.springs = new ffiseg::particle_pseudo_spring[6 * 1000];
    slg.dampers = new ffiseg::particle_damper[6 * 1000];
    auto spring_constant = 2;
    auto damper_constant = 10;

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; ++k) {
                auto pos = ffiseg::vector(i * length ,j * length, k * length);
                particle_array[(n * n * i) + (n * j) + k].set_position(pos);
                particle_array[(n * n * i) + (n * j) + k].set_mass(5);
                particle_array[(n * n * i) + (n * j) + k].set_damping(1.0f);
            }
        }
    }


    for(int i = 0; i < n - 1; ++i) {
        for(int j = 0; j < n - 1; ++j) {
            for(int k = 0; k < n - 1; ++k) {
                int F_index_1 = (n * n * i) + (n * j) + k;
                int F_index_2 = (n * n * i) + (n * j) + (k + 1);
                int F_index_3 = (n * n * i) + (n * (j + 1)) + k;
                int F_index_4 = (n * n * (i + 1)) + (n * j) + k;

                slg.springs[F_index_1].set_other(&slg.parts[F_index_1]);
                slg.springs[F_index_1].set_spring_constant(spring_constant);
                slg.springs[F_index_1].set_length(length);
                slg.dampers[F_index_1].set_other(&slg.parts[F_index_1]);
                slg.dampers[F_index_1].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_2], &slg.springs[F_index_1]);
                add_force_gen_to_registry(&slg.parts[F_index_2], &slg.dampers[F_index_1]);

                slg.springs[F_index_1 + n * n].set_other(&slg.parts[F_index_1]);
                slg.springs[F_index_1 + n * n].set_spring_constant(spring_constant);
                slg.springs[F_index_1 + n * n].set_length(length);
                slg.dampers[F_index_1 + n * n].set_other(&slg.parts[F_index_1]);
                slg.dampers[F_index_1 + n * n].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_3], &slg.springs[F_index_1 + n * n]);
                add_force_gen_to_registry(&slg.parts[F_index_3], &slg.dampers[F_index_1 + n * n]);

                slg.springs[F_index_1 + 2 * n * n].set_other(&slg.parts[F_index_1]);
                slg.springs[F_index_1 + 2 * n * n].set_spring_constant(spring_constant);
                slg.springs[F_index_1 + 2 * n * n].set_length(length);
                slg.dampers[F_index_1 + 2 * n * n].set_other(&slg.parts[F_index_1]);
                slg.dampers[F_index_1 + 2 * n * n].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_4], &slg.springs[F_index_1 + 2 * n * n]);
                add_force_gen_to_registry(&slg.parts[F_index_4], &slg.dampers[F_index_1 + 2 * n * n]);

                slg.springs[F_index_2 + 3 * n * n].set_other(&slg.parts[F_index_2]);
                slg.springs[F_index_2 + 3 * n * n].set_spring_constant(spring_constant);
                slg.springs[F_index_2 + 3 * n * n].set_length(length);
                slg.dampers[F_index_2 + 3 * n * n].set_other(&slg.parts[F_index_2]);
                slg.dampers[F_index_2 + 3 * n * n].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.springs[F_index_2 + 3 * n * n]);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.dampers[F_index_2 + 3 * n * n]);

                slg.springs[F_index_3 + 4 * n * n].set_other(&slg.parts[F_index_3]);
                slg.springs[F_index_3 + 4 * n * n].set_spring_constant(spring_constant);
                slg.springs[F_index_3 + 4 * n * n].set_length(length);
                slg.dampers[F_index_3 + 4 * n * n].set_other(&slg.parts[F_index_3]);
                slg.dampers[F_index_3 + 4 * n * n].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.springs[F_index_3 + 4 * n * n]);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.dampers[F_index_3 + 4 * n * n]);

                slg.springs[F_index_4 + 5 * n * n].set_other(&slg.parts[F_index_4]);
                slg.springs[F_index_4 + 5 * n * n].set_spring_constant(spring_constant);
                slg.springs[F_index_4 + 5 * n * n].set_length(length);
                slg.dampers[F_index_4 + 5 * n * n].set_other(&slg.parts[F_index_4]);
                slg.dampers[F_index_4 + 5 * n * n].set_damper_constant(damper_constant);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.springs[F_index_4 + 5 * n * n]);
                add_force_gen_to_registry(&slg.parts[F_index_1], &slg.dampers[F_index_4 + 5 * n * n]);
            }
        }
    }

    for(int i = 0; i < n - 1; ++i) {
        for(int j = 0; j < n - 1; ++j) {
            int F_index_1 = (n * n * i) + (n * j) + (n - 1);
            int F_index_3 = (n * n * i) + (n * (j + 1)) + (n - 1);
            int F_index_4 = (n * n * (i + 1)) + (n * j) + (n - 1);
        }
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
