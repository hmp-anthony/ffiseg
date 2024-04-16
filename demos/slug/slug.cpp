#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

/**
 * The main demo class definition.
 */
class slug_demo : public mass_aggregate_application
{
public:
    struct slug {
        ffiseg::particle* parts;
        ffiseg::particle_pseudo_spring* springs;
    };

    slug slg;

    /** Creates a new demo object. */
    slug_demo();
    virtual ~slug_demo();

    /** Returns the window title for the demo. */
    virtual const char* get_title();
    virtual void update();
    virtual void display();

};

// Method definitions
slug_demo::slug_demo() :
mass_aggregate_application(50 * 50) {

    slg.parts = particle_array;
    particle_array[0].set_position(0 ,5, 5);
    particle_array[1].set_position(5 ,5, 5);
    particle_array[2].set_position(5 ,5, 0);
    particle_array[3].set_position(0 ,5, 0);
    particle_array[4].set_position(0 ,10, 5);
    particle_array[5].set_position(5 ,10, 5);
    particle_array[6].set_position(5 ,10, 0);
    particle_array[7].set_position(0 ,10, 0);
    for(int i = 0; i < 8; ++i) {
        auto acc = ffiseg::vector(0,-10,0);
        particle_array[i].set_acceleration(acc);
        particle_array[i].set_mass(5);
    }

    slg.springs = new ffiseg::particle_pseudo_spring[24];
    auto spring_constant = 2;

    slg.springs[0].set_other(&slg.parts[0]);
    slg.springs[0].set_spring_constant(spring_constant);
    slg.springs[0].set_length(5);
    add_force_gen_to_registry(&slg.parts[1], &slg.springs[0]);
}

slug_demo::~slug_demo() {}


const char* slug_demo::get_title()
{
    return "Ffiseg > slug_demo";
}

void slug_demo::update() {
    mass_aggregate_application::update();
}

void slug_demo::display() {
    mass_aggregate_application::display();

    auto fps = ffiseg::timer::get().fps;
    std::string fps_string = std::to_string(fps);

    glColor3f(0.0f, 0.0f, 0.0f);
    render_text(10.0f, 34.0f, fps_string.c_str());
}

application* get_application()
{
    return new slug_demo();
}
