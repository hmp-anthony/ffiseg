#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

#define BASE_MASS 10
#define SPRING_CONSTANT 8
#define SPRING_LENGTH 1
#define HEIGHT 10
#define CUBE_SIZE 100

/**
 * The main demo class definition.
 */
class cloth : public mass_aggregate_application
{
    ffiseg::particle_spring* springs;
    ffiseg::particle_rod* rods;

public:
    /** Creates a new demo object. */
    cloth();
    virtual ~cloth();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

};

// Method definitions
cloth::cloth() :
mass_aggregate_application(CUBE_SIZE) {
    // Create the masses and connections.
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            particle_array[10 * j + i].set_position(i, 2.0, j);
        }
    }

    for (unsigned i = 0; i < CUBE_SIZE; i++)
    {
        particle_array[i].set_mass(BASE_MASS);
        particle_array[i].set_velocity(0, 0, 0);
        particle_array[i].set_damping(0.3f);
        auto acc = ffiseg::vector(0, 0, 0);
        particle_array[i].set_acceleration(acc);
        particle_array[i].clear_accumulator();
    }

    auto pos = particle_array[43].get_position();
    pos.set_y(pos.get_y() + 10);
    particle_array[43].set_position(pos);

    // set up the springs
    springs = new ffiseg::particle_spring[4 * 100];

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {

            if((i == 0) || (i == 9) || (j == 0) || (j == 9)) continue;  

            springs[10 * j + i].set_other(&particle_array[10 * j + i]);
            springs[10 * j + i].set_spring_constant(SPRING_CONSTANT);
            springs[10 * j + i].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 1)], &springs[10 * j + i]);

            springs[10 * j + i + 100].set_other(&particle_array[10 * j + i]);
            springs[10 * j + i + 100].set_spring_constant(SPRING_CONSTANT);
            springs[10 * j + i + 100].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 1)], &springs[10 * j + i + 100]);

            springs[10 * j + i + 200].set_other(&particle_array[10 * j + i]);
            springs[10 * j + i + 200].set_spring_constant(SPRING_CONSTANT);
            springs[10 * j + i + 200].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + i], &springs[10 * j + i + 200]);

            springs[10 * j + i + 300].set_other(&particle_array[10 * j + i]);
            springs[10 * j + i + 300].set_spring_constant(SPRING_CONSTANT);
            springs[10 * j + i + 300].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + i], &springs[10 * j + i + 300]);
        }
    }

    // set up the rods
    rods = new ffiseg::particle_rod[4 * 100];

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {

            if((i < 2) || (i > 7) || (j < 2) || (j > 7)) continue;  
            
            rods[10 * j + i].parts[0] = &particle_array[10 * j + i];
            rods[10 * j + i].parts[1] = &particle_array[10 * j + (i - 2)];
            rods[10 * j + i].length = 2;

            rods[10 * j + i + 100].parts[0] = &particle_array[10 * j + i];
            rods[10 * j + i + 100].parts[1] = &particle_array[10 * j + (i + 2)];
            rods[10 * j + i + 100].length = 2;

            rods[10 * j + i + 200].parts[0] = &particle_array[10 * j + i];
            rods[10 * j + i + 200].parts[1] = &particle_array[10 * (j + 2) + i];
            rods[10 * j + i + 200].length = 2;

            rods[10 * j + i + 300].parts[0] = &particle_array[10 * j + i];
            rods[10 * j + i + 300].parts[1] = &particle_array[10 * (j - 2) + i];
            rods[10 * j + i + 300].length = 2;

        }

    }

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {

            if((i < 2) || (i > 7) || (j < 2) || (j > 7)) continue;  
            
            world.get_contact_generators().push_back(&rods[10 * j + i]);
            world.get_contact_generators().push_back(&rods[10 * j + i + 100]);
            world.get_contact_generators().push_back(&rods[10 * j + i + 200]);
            world.get_contact_generators().push_back(&rods[10 * j + i + 300]);
        }

    }
}

cloth::~cloth() {}


const char* cloth::getTitle()
{
    return "Cyclone > Platform Demo";
}

application* get_application()
{
    return new cloth();
}
