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
class cloth_demo : public mass_aggregate_application
{
    struct cloth {

        ffiseg::particle* parts;
        ffiseg::vector* normals;
        ffiseg::particle_spring* springs;
        ffiseg::particle_damper* dampers;

        ffiseg::vector get_wind_vector() {
            return ffiseg::vector(1, 1, 1);
        }

        class wind_force : public ffiseg::particle_force_generator {
            public:
                ffiseg::vector* wind_vector;
                ffiseg::vector* normal;
                virtual void update_force(ffiseg::particle* part, ffiseg::real duration) {
                    auto p =  *wind_vector - part->get_velocity();
                    auto q = p * (*normal);
                }
        };
    };
    
public:
    /** Creates a new demo object. */
    cloth_demo();
    virtual ~cloth_demo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

};

// Method definitions
cloth_demo::cloth_demo() :
mass_aggregate_application(CUBE_SIZE) {
    // Create the masses and connections.
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            particle_array[10 * j + i].set_position(i, 2.0, j);
        }
    }
    
    cloth clth; 
        
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
    clth.springs = new ffiseg::particle_spring[4 * 100];

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {

            if((i == 0) || (i == 9) || (j == 0) || (j == 9)) continue;  

            clth.springs[10 * j + i].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i].set_spring_constant(SPRING_CONSTANT);
            clth.springs[10 * j + i].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 1)], &clth.springs[10 * j + i]);

            clth.springs[10 * j + i + 100].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 100].set_spring_constant(SPRING_CONSTANT);
            clth.springs[10 * j + i + 100].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 1)], &clth.springs[10 * j + i + 100]);

            clth.springs[10 * j + i + 200].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 200].set_spring_constant(SPRING_CONSTANT);
            clth.springs[10 * j + i + 200].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + i], &clth.springs[10 * j + i + 200]);

            clth.springs[10 * j + i + 300].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 300].set_spring_constant(SPRING_CONSTANT);
            clth.springs[10 * j + i + 300].set_length(SPRING_LENGTH);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + i], &clth.springs[10 * j + i + 300]);
        }
    }
}

cloth_demo::~cloth_demo() {}


const char* cloth_demo::getTitle()
{
    return "Cyclone > Platform Demo";
}

application* get_application()
{
    return new cloth_demo();
}
