#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

#define ROD_COUNT 0

#define BASE_MASS 30
#define SPRING_CONSTANT 10
#define SPRING_LENGTH 30
#define HEIGHT 10
#define CUBE_SIZE 1000

/**
 * The main demo class definition.
 */
class cloth : public mass_aggregate_application
{
    ffiseg::particle_rod *rods;
    ffiseg::particle_spring* springs;

public:
    /** Creates a new demo object. */
    cloth();
    virtual ~cloth();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

};

// Method definitions
cloth::cloth() :
mass_aggregate_application(CUBE_SIZE), rods(0) {
    // Create the masses and connections.
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            particle_array[10*j + i].set_position(i, 2.0, j);
        }
    }
    for (unsigned i = 0; i < CUBE_SIZE; i++)
    {
        particle_array[i].set_mass(BASE_MASS);
        particle_array[i].set_velocity(0, 0, 0);
        particle_array[i].set_damping(1.0f);
        auto acc = ffiseg::vector(0, 0, 0);
        particle_array[i].set_acceleration(acc);
        particle_array[i].clear_accumulator();
    }

    rods = new ffiseg::particle_rod[ROD_COUNT];

    /*
    springs = new ffiseg::particle_spring[4];

    springs[0].set_other(&particle_array[0]);
    springs[0].set_spring_constant(SPRING_CONSTANT);
    springs[0].set_length(SPRING_LENGTH);
    springs[1].set_other(&particle_array[1]);
    springs[1].set_spring_constant(SPRING_CONSTANT);
    springs[1].set_length(SPRING_LENGTH);
    springs[2].set_other(&particle_array[2]);
    springs[2].set_spring_constant(SPRING_CONSTANT);
    springs[2].set_length(SPRING_LENGTH);
    springs[3].set_other(&particle_array[3]);
    springs[3].set_spring_constant(SPRING_CONSTANT);
    springs[3].set_length(SPRING_LENGTH);

    add_force_gen_to_registry(&particle_array[4], &springs[0]);
    add_force_gen_to_registry(&particle_array[5], &springs[1]);
    add_force_gen_to_registry(&particle_array[6], &springs[2]);
    add_force_gen_to_registry(&particle_array[7], &springs[3]);

    */

    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.get_contact_generators().push_back(&rods[i]);
    }

}

cloth::~cloth()
{
    if (rods) delete[] rods;
}


const char* cloth::getTitle()
{
    return "Cyclone > Platform Demo";
}

application* get_application()
{
    return new cloth();
}
