#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

#define BASE_MASS 10
#define SPRING_CONSTANT_1 2
#define DAMPER_CONSTANT_1 15
#define SPRING_LENGTH_1 1
#define SPRING_CONSTANT_2 3
#define DAMPER_CONSTANT_2 12
#define SPRING_LENGTH_2 1.414213
#define SPRING_CONSTANT_3 5
#define DAMPER_CONSTANT_3 50
#define SPRING_LENGTH_3 2
#define HEIGHT 10
#define CUBE_SIZE 100

/**
 * The main demo class definition.
 */
class cloth_demo : public mass_aggregate_application
{
    struct cloth {

        ffiseg::particle_spring* springs;
        ffiseg::particle_damper* dampers;

        ffiseg::vector get_wind_vector() {
            return ffiseg::vector(1, 1, 1);
        }

        class wind_force : public ffiseg::particle_force_generator {
            public:
                ffiseg::vector wind_vector;
                ffiseg::vector normal;
                void set_wind_vector(ffiseg::vector wv) { 
                    wind_vector = wv;
                }
                void set_normal(ffiseg::vector n) {
                    normal = n;
                }
                virtual void update_force(ffiseg::particle* part, ffiseg::real duration) {
                    auto p =  wind_vector - part->get_velocity();
                    auto q = p * normal;
                    part->add_force(wind_vector * q);
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
            particle_array[10 * j + i].set_position(i, 2.0 + i*j * 0.01, j);
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

    cloth::wind_force* wfs = new cloth::wind_force[100];

    // calculate the wind
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            auto o = particle_array[10 * j + i].get_position();
            auto p = particle_array[10 * j + (i + 1)].get_position();
            auto q = particle_array[10 * (j + 1) + i].get_position();
            auto P = p - o;
            auto Q = q - o;
            auto normal = P % Q;
            ffiseg::vector wv(2, 2, 2);
            wfs[10 * j + i].set_wind_vector(wv);
            wfs[10 * j + i].set_normal(normal);
            add_force_gen_to_registry(&particle_array[10 * j + i], &wfs[10 * j + i]);
        }
    }

    cloth clth; 
    // set up the springs
    clth.springs = new ffiseg::particle_spring[12 * 100];
    // set up the dampers
    clth.dampers = new ffiseg::particle_damper[12 * 100];

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {

            if((i == 0) || (i == 9) || (j == 0) || (j == 9)) continue;  

            // first we complete the one-up, one-down, one-left and one-right
            clth.springs[10 * j + i].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i].set_spring_constant(SPRING_CONSTANT_1);
            clth.springs[10 * j + i].set_length(SPRING_LENGTH_1);
            clth.dampers[10 * j + i].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i].set_damper_constant(DAMPER_CONSTANT_1);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 1)], &clth.springs[10 * j + i]);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 1)], &clth.dampers[10 * j + i]);

            clth.springs[10 * j + i + 100].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 100].set_spring_constant(SPRING_CONSTANT_1);
            clth.springs[10 * j + i + 100].set_length(SPRING_LENGTH_1);
            clth.dampers[10 * j + i + 100].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 100].set_damper_constant(DAMPER_CONSTANT_1);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 1)], &clth.springs[10 * j + i + 100]);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 1)], &clth.dampers[10 * j + i + 100]);

            clth.springs[10 * j + i + 200].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 200].set_spring_constant(SPRING_CONSTANT_1);
            clth.springs[10 * j + i + 200].set_length(SPRING_LENGTH_1);
            clth.dampers[10 * j + i + 200].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 200].set_damper_constant(DAMPER_CONSTANT_1);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + i], &clth.springs[10 * j + i + 200]);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + i], &clth.dampers[10 * j + i + 200]);

            clth.springs[10 * j + i + 300].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 300].set_spring_constant(SPRING_CONSTANT_1);
            clth.springs[10 * j + i + 300].set_length(SPRING_LENGTH_1);
            clth.dampers[10 * j + i + 300].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 300].set_damper_constant(DAMPER_CONSTANT_1);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + i], &clth.springs[10 * j + i + 300]);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + i], &clth.dampers[10 * j + i + 300]);
        
            // now the diagonal links
            clth.springs[10 * j + i + 400].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 400].set_spring_constant(SPRING_CONSTANT_2);
            clth.springs[10 * j + i + 400].set_length(SPRING_LENGTH_2);
            clth.dampers[10 * j + i + 400].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 400].set_damper_constant(DAMPER_CONSTANT_2);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i - 1)], &clth.springs[10 * j + i + 400]);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i - 1)], &clth.dampers[10 * j + i + 400]);

            clth.springs[10 * j + i + 500].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 500].set_spring_constant(SPRING_CONSTANT_2);
            clth.springs[10 * j + i + 500].set_length(SPRING_LENGTH_2);
            clth.dampers[10 * j + i + 500].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 500].set_damper_constant(DAMPER_CONSTANT_2);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i + 1)], &clth.springs[10 * j + i + 500]);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i + 1)], &clth.dampers[10 * j + i + 500]);

            clth.springs[10 * j + i + 600].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 600].set_spring_constant(SPRING_CONSTANT_2);
            clth.springs[10 * j + i + 600].set_length(SPRING_LENGTH_2);
            clth.dampers[10 * j + i + 600].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 600].set_damper_constant(DAMPER_CONSTANT_2);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i + 1)], &clth.springs[10 * j + i + 600]);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i + 1)], &clth.dampers[10 * j + i + 600]);

            clth.springs[10 * j + i + 700].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 700].set_spring_constant(SPRING_CONSTANT_2);
            clth.springs[10 * j + i + 700].set_length(SPRING_LENGTH_2);
            clth.dampers[10 * j + i + 700].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 700].set_damper_constant(DAMPER_CONSTANT_2);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i - 1)], &clth.springs[10 * j + i + 700]);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i - 1)], &clth.dampers[10 * j + i + 700]);

            // 2-by-2 computational molecule
            if((i < 2) || (i > 7) || (j < 2) || (j > 7)) continue;  

            clth.springs[10 * j + i + 800].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 800].set_spring_constant(SPRING_CONSTANT_3);
            clth.springs[10 * j + i + 800].set_length(SPRING_LENGTH_3);
            clth.dampers[10 * j + i + 800].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 800].set_damper_constant(DAMPER_CONSTANT_3);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 2)], &clth.springs[10 * j + i + 800]);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 2)], &clth.dampers[10 * j + i + 800]);

            clth.springs[10 * j + i + 900].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 900].set_spring_constant(SPRING_CONSTANT_3);
            clth.springs[10 * j + i + 900].set_length(SPRING_LENGTH_3);
            clth.dampers[10 * j + i + 900].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 900].set_damper_constant(DAMPER_CONSTANT_3);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 2)], &clth.springs[10 * j + i + 900]);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 2)], &clth.dampers[10 * j + i + 900]);

            clth.springs[10 * j + i + 1000].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 1000].set_spring_constant(SPRING_CONSTANT_3);
            clth.springs[10 * j + i + 1000].set_length(SPRING_LENGTH_3);
            clth.dampers[10 * j + i + 1000].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 1000].set_damper_constant(DAMPER_CONSTANT_3);
            add_force_gen_to_registry(&particle_array[10 * (j - 2) + i], &clth.springs[10 * j + i + 1000]);
            add_force_gen_to_registry(&particle_array[10 * (j - 2) + i], &clth.dampers[10 * j + i + 1000]);

            clth.springs[10 * j + i + 1100].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 1100].set_spring_constant(SPRING_CONSTANT_3);
            clth.springs[10 * j + i + 1100].set_length(SPRING_LENGTH_3);
            clth.dampers[10 * j + i + 1100].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 1100].set_damper_constant(DAMPER_CONSTANT_3);
            add_force_gen_to_registry(&particle_array[10 * (j + 2) + i], &clth.springs[10 * j + i + 1100]);
            add_force_gen_to_registry(&particle_array[10 * (j + 2) + i], &clth.dampers[10 * j + i + 1100]);
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
