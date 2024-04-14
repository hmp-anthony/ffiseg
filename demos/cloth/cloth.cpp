#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

/**
 * The main demo class definition.
 */
class cloth_demo : public mass_aggregate_application
{
public:
    struct cloth {

        unsigned n;
        unsigned m;

        ffiseg::real mass;
        ffiseg::real length;
        ffiseg::real spring_constant_1;
        ffiseg::real damper_constant_1;
        ffiseg::real spring_constant_2;
        ffiseg::real damper_constant_2;
        ffiseg::real spring_constant_3;
        ffiseg::real damper_constant_3;

        ffiseg::particle* parts;
        ffiseg::vector* normals;
        ffiseg::particle_spring* springs;
        ffiseg::particle_damper* dampers;

        ffiseg::vector get_wind_vector() {
            return ffiseg::vector(0.0, 0.0, 0.0);
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

        wind_force* wind_forces;

        void update_normals() {

            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < m; ++j) {
                    auto o = parts[n * j + i].get_position();
                    auto p = parts[n * j + (i + 1)].get_position();
                    auto q = parts[n * (j + 1) + i].get_position();
                    auto P = p - o;
                    auto Q = q - o;
                    normals[n * j + i] = P % Q;
                }
            }

            for(int j = 0; j < m - 1; ++j) {
                auto o = parts[n * j + (n - 1)].get_position();
                auto p = parts[n * (j + 1) + (n - 1)].get_position();
                auto q = parts[n * j + (n - 2)].get_position();
                auto P = p - o;
                auto Q = q - o;
                normals[n * j + (n - 1)] = P % Q;
            }

            for(int i = 0; i < n - 1; ++i) {
                auto o = parts[n * (m - 1) + i].get_position();
                auto p = parts[n * (m - 1) + (i + 1)].get_position();
                auto q = parts[n * (m - 2) + i].get_position();
                auto P = p - o;
                auto Q = q - o;
                normals[n * (m - 1) + i] = Q % P;
            }

            auto o = parts[n * (m - 1) + (n - 1)].get_position();
            auto p = parts[n * (m - 1) + (n - 2)].get_position();
            auto q = parts[n * (m - 2) + (n - 1)].get_position();
            auto P = p - o;
            auto Q = q - o;
            normals[n * (m - 1) + (n - 1)] = P % Q;

        }

        // Call in update function!
        void update_wind_forces() {
            update_normals();
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < m; ++j) {
                    auto wind_vec = get_wind_vector();
                    wind_forces[n * j + i].set_normal(normals[n * j + i]);
                    wind_forces[n * j + i].set_wind_vector(wind_vec);
                }
            }
        }


    };
    
    cloth clth;

    /** Creates a new demo object. */
    cloth_demo();
    virtual ~cloth_demo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();
    virtual void update();

};

// Method definitions
cloth_demo::cloth_demo() :
mass_aggregate_application(10 * 20) {

    // declare and init cloth object
    clth.n = 2;
    clth.m = 2;
    clth.mass = 1;
    clth.length = 2;
    clth.spring_constant_1 = 10;
    clth.damper_constant_1 = 0;
    clth.spring_constant_2 = 0;
    clth.damper_constant_2 = 0;
    clth.spring_constant_3 = 0;
    clth.damper_constant_3 = 0;

    clth.parts = particle_array;
    clth.normals = new ffiseg::vector[clth.n * clth.m];
    clth.springs = new ffiseg::particle_spring[12 * clth.n * clth.m];
    clth.dampers = new ffiseg::particle_damper[12 * clth.n * clth.m];
    clth.wind_forces = new cloth::wind_force[clth.n * clth.m];

    auto n = clth.n;
    auto m = clth.m;
    auto spring_constant_1 = clth.spring_constant_1;
    auto damper_constant_1 = clth.damper_constant_1;
    auto spring_constant_2 = clth.spring_constant_2;
    auto damper_constant_2 = clth.damper_constant_2;
    auto spring_constant_3 = clth.spring_constant_3;
    auto damper_constant_3 = clth.damper_constant_3;
    auto length = clth.length;
    auto length_2 = 1.141 * length;
    auto length_3 = 2 * length;

    auto buf = n * m;
    
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            particle_array[n * j + i].set_position(i * clth.length, sin(0.1 * i * j), j * clth.length);
            particle_array[n * j + i].set_mass(clth.mass);
            particle_array[n * j + i].set_velocity(0, 0, 0);
            particle_array[n * j + i].set_damping(0.9f);
            auto acc = ffiseg::vector(0, 0, 0);
            particle_array[n * j + i].set_acceleration(acc);
            particle_array[n * j + i].clear_accumulator();
        }
    }

    // add wind forces
    clth.update_wind_forces();
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            add_force_gen_to_registry(&particle_array[n * j + i], &clth.wind_forces[n * j + i]);
        }
    }
    
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {

            if((i == 0) || (i == n - 1) || (j == 0) || (j == m - 1)) continue;  
        
                
            // first we complete the one-up, one-down, one-left and one-right
            clth.springs[n * j + i].set_other(&particle_array[n * j + i]);
            clth.springs[n * j + i].set_spring_constant(spring_constant_1);
            clth.springs[n * j + i].set_length(length);
            clth.dampers[n * j + i].set_other(&particle_array[n * j + i]);
            clth.dampers[n * j + i].set_damper_constant(damper_constant_1);
            add_force_gen_to_registry(&particle_array[n * j + (i - 1)], &clth.springs[n * j + i]);
            add_force_gen_to_registry(&particle_array[n * j + (i - 1)], &clth.dampers[n * j + i]);

            clth.springs[n * j + i + buf].set_other(&particle_array[n * j + i]);
            clth.springs[n * j + i + buf].set_spring_constant(spring_constant_1);
            clth.springs[n * j + i + buf].set_length(length);
            clth.dampers[n * j + i + buf].set_other(&particle_array[n * j + i]);
            clth.dampers[n * j + i + buf].set_damper_constant(damper_constant_1);
            add_force_gen_to_registry(&particle_array[n * j + (i + 1)], &clth.springs[n * j + i + buf]);
            add_force_gen_to_registry(&particle_array[n * j + (i + 1)], &clth.dampers[n * j + i + buf]);

            clth.springs[n * j + i + 2 * buf].set_other(&particle_array[n * j + i]);
            clth.springs[n * j + i + 2 * buf].set_spring_constant(spring_constant_1);
            clth.springs[n * j + i + 2 * buf].set_length(length);
            clth.dampers[n * j + i + 2 * buf].set_other(&particle_array[n * j + i]);
            clth.dampers[n * j + i + 2 * buf].set_damper_constant(damper_constant_1);
            add_force_gen_to_registry(&particle_array[n * (j - 1) + i], &clth.springs[n * j + i + 2 * buf]);
            add_force_gen_to_registry(&particle_array[n * (j - 1) + i], &clth.dampers[n * j + i + 2 * buf]);

            clth.springs[n * j + i + 3 * buf].set_other(&particle_array[n * j + i]);
            clth.springs[n * j + i + 3 * buf].set_spring_constant(spring_constant_1);
            clth.springs[n * j + i + 3 * buf].set_length(length);
            clth.dampers[n * j + i + 3 * buf].set_other(&particle_array[n * j + i]);
            clth.dampers[n * j + i + 3 * buf].set_damper_constant(damper_constant_1);
            add_force_gen_to_registry(&particle_array[n * (j + 1) + i], &clth.springs[n * j + i + 3 * buf]);
            add_force_gen_to_registry(&particle_array[n * (j + 1) + i], &clth.dampers[n * j + i + 3 * buf]);

            /* 
            // now the diagonal links
            clth.springs[10 * j + i + 400].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 400].set_spring_constant(spring_constant_2);
            clth.springs[10 * j + i + 400].set_length(length_2);
            clth.dampers[10 * j + i + 400].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 400].set_damper_constant(damper_constant_2);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i - 1)], &clth.springs[10 * j + i + 400]);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i - 1)], &clth.dampers[10 * j + i + 400]);

            clth.springs[10 * j + i + 500].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 500].set_spring_constant(spring_constant_2);
            clth.springs[10 * j + i + 500].set_length(length_2);
            clth.dampers[10 * j + i + 500].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 500].set_damper_constant(damper_constant_2);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i + 1)], &clth.springs[10 * j + i + 500]);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i + 1)], &clth.dampers[10 * j + i + 500]);

            clth.springs[10 * j + i + 600].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 600].set_spring_constant(spring_constant_2);
            clth.springs[10 * j + i + 600].set_length(length_2);
            clth.dampers[10 * j + i + 600].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 600].set_damper_constant(damper_constant_2);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i + 1)], &clth.springs[10 * j + i + 600]);
            add_force_gen_to_registry(&particle_array[10 * (j - 1) + (i + 1)], &clth.dampers[10 * j + i + 600]);

            clth.springs[10 * j + i + 700].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 700].set_spring_constant(spring_constant_2);
            clth.springs[10 * j + i + 700].set_length(length_2);
            clth.dampers[10 * j + i + 700].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 700].set_damper_constant(damper_constant_2);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i - 1)], &clth.springs[10 * j + i + 700]);
            add_force_gen_to_registry(&particle_array[10 * (j + 1) + (i - 1)], &clth.dampers[10 * j + i + 700]);

            // 2-by-2 computational molecule
            if((i < 2) || (i > 7) || (j < 2) || (j > 7)) continue;  

            clth.springs[10 * j + i + 800].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 800].set_spring_constant(spring_constant_3);
            clth.springs[10 * j + i + 800].set_length(length_3);
            clth.dampers[10 * j + i + 800].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 800].set_damper_constant(damper_constant_3);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 2)], &clth.springs[10 * j + i + 800]);
            add_force_gen_to_registry(&particle_array[10 * j + (i - 2)], &clth.dampers[10 * j + i + 800]);

            clth.springs[10 * j + i + 900].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 900].set_spring_constant(spring_constant_3);
            clth.springs[10 * j + i + 900].set_length(length_3);
            clth.dampers[10 * j + i + 900].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 900].set_damper_constant(damper_constant_3);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 2)], &clth.springs[10 * j + i + 900]);
            add_force_gen_to_registry(&particle_array[10 * j + (i + 2)], &clth.dampers[10 * j + i + 900]);

            clth.springs[10 * j + i + 1000].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 1000].set_spring_constant(spring_constant_3);
            clth.springs[10 * j + i + 1000].set_length(length_3);
            clth.dampers[10 * j + i + 1000].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 1000].set_damper_constant(damper_constant_3);
            add_force_gen_to_registry(&particle_array[10 * (j - 2) + i], &clth.springs[10 * j + i + 1000]);
            add_force_gen_to_registry(&particle_array[10 * (j - 2) + i], &clth.dampers[10 * j + i + 1000]);

            clth.springs[10 * j + i + 1100].set_other(&particle_array[10 * j + i]);
            clth.springs[10 * j + i + 1100].set_spring_constant(spring_constant_3);
            clth.springs[10 * j + i + 1100].set_length(length_3);
            clth.dampers[10 * j + i + 1100].set_other(&particle_array[10 * j + i]);
            clth.dampers[10 * j + i + 1100].set_damper_constant(damper_constant_3);
            add_force_gen_to_registry(&particle_array[10 * (j + 2) + i], &clth.springs[10 * j + i + 1100]);
            add_force_gen_to_registry(&particle_array[10 * (j + 2) + i], &clth.dampers[10 * j + i + 1100]);
            */
        }
    }
    // corner cases of the one-jump verical-horizontal links 
    // bottom left
    clth.springs[0].set_other(&particle_array[0]);
    clth.springs[0].set_spring_constant(spring_constant_1);
    clth.springs[0].set_length(length);
    clth.dampers[0].set_other(&particle_array[0]);
    clth.dampers[0].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n], &clth.springs[0]);
    add_force_gen_to_registry(&particle_array[n], &clth.dampers[0]);

    clth.springs[0 + buf].set_other(&particle_array[0]);
    clth.springs[0 + buf].set_spring_constant(spring_constant_1);
    clth.springs[0 + buf].set_length(length);
    clth.dampers[0 + buf].set_other(&particle_array[0]);
    clth.dampers[0 + buf].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[1], &clth.springs[0 + buf]);
    add_force_gen_to_registry(&particle_array[1], &clth.dampers[0 + buf]);
    
    // bottom right
    clth.springs[n - 1].set_other(&particle_array[n - 1]);
    clth.springs[n - 1].set_spring_constant(spring_constant_1);
    clth.springs[n - 1].set_length(length);
    clth.dampers[n - 1].set_other(&particle_array[n - 1]);
    clth.dampers[n - 1].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n - 2], &clth.springs[n - 1]);
    add_force_gen_to_registry(&particle_array[n - 2], &clth.dampers[n - 1]);

    clth.springs[n - 1 + buf].set_other(&particle_array[n - 1]);
    clth.springs[n - 1 + buf].set_spring_constant(spring_constant_1);
    clth.springs[n - 1 + buf].set_length(length);
    clth.dampers[n - 1 + buf].set_other(&particle_array[n - 1]);
    clth.dampers[n - 1 + buf].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[2 * n - 1], &clth.springs[n - 1 + buf]);
    add_force_gen_to_registry(&particle_array[2 * n - 1], &clth.dampers[n - 1 + buf]);

    // tep left
    clth.springs[n * (m - 1)].set_other(&particle_array[n * (m - 1)]);
    clth.springs[n * (m - 1)].set_spring_constant(spring_constant_1);
    clth.springs[n * (m - 1)].set_length(length);
    clth.dampers[n * (m - 1)].set_other(&particle_array[n * (m - 1)]);
    clth.dampers[n * (m - 1)].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n * (m - 2)], &clth.springs[n * (m - 1)]);
    add_force_gen_to_registry(&particle_array[n * (m - 2)], &clth.dampers[n * (m - 1)]);

    clth.springs[n * (m - 1) + buf].set_other(&particle_array[n * (m - 1)]);
    clth.springs[n * (m - 1) + buf].set_spring_constant(spring_constant_1);
    clth.springs[n * (m - 1) + buf].set_length(length);
    clth.dampers[n * (m - 1) + buf].set_other(&particle_array[n * (m - 1)]);
    clth.dampers[n * (m - 1) + buf].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n * (m - 1) + 1], &clth.springs[n * (m - 1) + buf]);
    add_force_gen_to_registry(&particle_array[n * (m - 1) + 1], &clth.dampers[n * (m - 1) + buf]);

    // top right
    clth.springs[n * m - 1].set_other(&particle_array[n * m - 1]);
    clth.springs[n * m - 1].set_spring_constant(spring_constant_1);
    clth.springs[n * m - 1].set_length(length);
    clth.dampers[n * m - 1].set_other(&particle_array[n * m - 1]);
    clth.dampers[n * m - 1].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n * m - 2], &clth.springs[n * m - 1]);
    add_force_gen_to_registry(&particle_array[n * m - 2], &clth.dampers[n * m - 1]);

    clth.springs[n * m - 1 + buf].set_other(&particle_array[n * m - 1]);
    clth.springs[n * m - 1 + buf].set_spring_constant(spring_constant_1);
    clth.springs[n * m - 1 + buf].set_length(length);
    clth.dampers[n * m - 1 + buf].set_other(&particle_array[n * m - 1]);
    clth.dampers[n * m - 1 + buf].set_damper_constant(damper_constant_1);
    add_force_gen_to_registry(&particle_array[n * (m - 1) - 1], &clth.springs[n * m - 1 + buf]);
    add_force_gen_to_registry(&particle_array[n * (m - 1) - 1], &clth.dampers[n * m - 1 + buf]);

    // now for the side lengths
    // j == 0, j == n - 1, i == 0, i == m - 1
    for(int i = 1; i < n - 1; ++i) {
        // bottom 
        clth.springs[i].set_other(&particle_array[i]);
        clth.springs[i].set_spring_constant(spring_constant_1);
        clth.springs[i].set_length(length);
        clth.dampers[i].set_other(&particle_array[i]);
        clth.dampers[i].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[i - 1], &clth.springs[i]);
        add_force_gen_to_registry(&particle_array[i - 1], &clth.dampers[i]);

        clth.springs[i + buf].set_other(&particle_array[i]);
        clth.springs[i + buf].set_spring_constant(spring_constant_1);
        clth.springs[i + buf].set_length(length);
        clth.dampers[i + buf].set_other(&particle_array[i]);
        clth.dampers[i + buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[i + 1], &clth.springs[i + buf]);
        add_force_gen_to_registry(&particle_array[i + 1], &clth.dampers[i + buf]);

        clth.springs[i + 2 * buf].set_other(&particle_array[i]);
        clth.springs[i + 2 * buf].set_spring_constant(spring_constant_1);
        clth.springs[i + 2 * buf].set_length(length);
        clth.dampers[i + 2 * buf].set_other(&particle_array[i]);
        clth.dampers[i + 2 * buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n + i], &clth.springs[i + 2 * buf]);
        add_force_gen_to_registry(&particle_array[n + i], &clth.dampers[i + 2 * buf]);

        // top
        clth.springs[(m - 1) * n + i].set_other(&particle_array[(m - 1) * n + i]);
        clth.springs[(m - 1) * n + i].set_spring_constant(spring_constant_1);
        clth.springs[(m - 1) * n + i].set_length(length);
        clth.dampers[(m - 1) * n + i].set_other(&particle_array[(m - 1) * n + i]);
        clth.dampers[(m - 1) * n + i].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + (i - 1)], &clth.springs[(m - 1) * n + i]);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + (i - 1)], &clth.dampers[(m - 1) * n + i]);

        clth.springs[(m - 1) * n + i + buf].set_other(&particle_array[(m - 1) * n + i]);
        clth.springs[(m - 1) * n + i + buf].set_spring_constant(spring_constant_1);
        clth.springs[(m - 1) * n + i + buf].set_length(length);
        clth.dampers[(m - 1) * n + i + buf].set_other(&particle_array[(m - 1) * n + i]);
        clth.dampers[(m - 1) * n + i + buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + i], &clth.springs[(m - 1) * n + i + buf]);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + i], &clth.dampers[(m - 1) * n + i + buf]);

        clth.springs[(m - 1) * n + i + 2 * buf].set_other(&particle_array[(m - 1) * n + i]);
        clth.springs[(m - 1) * n + i + 2 * buf].set_spring_constant(spring_constant_1);
        clth.springs[(m - 1) * n + i + 2 * buf].set_length(length);
        clth.dampers[(m - 1) * n + i + 2 * buf].set_other(&particle_array[(m - 1) * n + i]);
        clth.dampers[(m - 1) * n + i + 2 * buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + (i + 1)], &clth.springs[(m - 1) * n + i + 2 * buf]);
        add_force_gen_to_registry(&particle_array[(m - 1) * n + (i + 1)], &clth.dampers[(m - 1) * n + i + 2 * buf]);
    }

    for(int j = 1; j < m - 1; ++j) {
        // left 
        clth.springs[n * j].set_other(&particle_array[n * j]);
        clth.springs[n * j].set_spring_constant(spring_constant_1);
        clth.springs[n * j].set_length(length);
        clth.dampers[n * j].set_other(&particle_array[n * j]);
        clth.dampers[n * j].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * j + 1], &clth.springs[n * j]);
        add_force_gen_to_registry(&particle_array[n * j + 1], &clth.dampers[n * j]);

        clth.springs[n * j + buf].set_other(&particle_array[n * j]);
        clth.springs[n * j + buf].set_spring_constant(spring_constant_1);
        clth.springs[n * j + buf].set_length(length);
        clth.dampers[n * j + buf].set_other(&particle_array[n * j]);
        clth.dampers[n * j + buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * (j - 1)], &clth.springs[n * j + buf]);
        add_force_gen_to_registry(&particle_array[n * (j - 1)], &clth.dampers[n * j + buf]);

        clth.springs[n * j + 2 * buf].set_other(&particle_array[n * j]);
        clth.springs[n * j + 2 * buf].set_spring_constant(spring_constant_1);
        clth.springs[n * j + 2 * buf].set_length(length);
        clth.dampers[n * j + 2 * buf].set_other(&particle_array[n * j]);
        clth.dampers[n * j + 2 * buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * (j + 1)], &clth.springs[n * j + 2 * buf]);
        add_force_gen_to_registry(&particle_array[n * (j + 1)], &clth.dampers[n * j + 2 * buf]);
    
        // right
        clth.springs[n * (j + 1) - 1].set_other(&particle_array[n * (j + 1) - 1]);
        clth.springs[n * (j + 1) - 1].set_spring_constant(spring_constant_1);
        clth.springs[n * (j + 1) - 1].set_length(length);
        clth.dampers[n * (j + 1) - 1].set_other(&particle_array[n * (j + 1) - 1]);
        clth.dampers[n * (j + 1) - 1].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * j - 1], &clth.springs[n * (j + 1) - 1]);
        add_force_gen_to_registry(&particle_array[n * j - 1], &clth.dampers[n * (j + 1) - 1]);

        clth.springs[n * (j + 1) - 1 + buf].set_other(&particle_array[n * (j + 1) - 1]);
        clth.springs[n * (j + 1) - 1 + buf].set_spring_constant(spring_constant_1);
        clth.springs[n * (j + 1) - 1 + buf].set_length(length);
        clth.dampers[n * (j + 1) - 1 + buf].set_other(&particle_array[n * (j + 1) - 1]);
        clth.dampers[n * (j + 1) - 1 + buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * (j + 1) - 2], &clth.springs[n * (j + 1) - 1 + buf]);
        add_force_gen_to_registry(&particle_array[n * (j + 1) - 2], &clth.dampers[n * (j + 1) - 1 + buf]);

        clth.springs[n * (j + 1) - 1 + 2 * buf].set_other(&particle_array[n * (j + 1) - 1]);
        clth.springs[n * (j + 1) - 1 + 2 * buf].set_spring_constant(spring_constant_1);
        clth.springs[n * (j + 1) - 1 + 2 * buf].set_length(length);
        clth.dampers[n * (j + 1) - 1 + 2 * buf].set_other(&particle_array[n * (j + 1) - 1]);
        clth.dampers[n * (j + 1) - 1 + 2 * buf].set_damper_constant(damper_constant_1);
        add_force_gen_to_registry(&particle_array[n * (j + 2) - 1], &clth.springs[n * (j + 1) - 1 + 2 * buf]);
        add_force_gen_to_registry(&particle_array[n * (j + 2) - 1], &clth.dampers[n * (j + 1) - 1 + 2 * buf]);
    }
}

cloth_demo::~cloth_demo() {}


const char* cloth_demo::getTitle()
{
    return "Cyclone > Platform Demo";
}

void cloth_demo::update() {
    clth.update_wind_forces();
    mass_aggregate_application::update();
}

application* get_application()
{
    return new cloth_demo();
}
