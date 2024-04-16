#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

#define ROD_COUNT 12

#define BASE_MASS 1
#define EXTRA_MASS 10

/**
 * The main demo class definition.
 */
class cube_demo : public mass_aggregate_application
{
    ffiseg::particle_rod *rods;

    /**
     * Updates particle masses to take into account the mass
     * that's on the cube.
     */
    void updateAdditionalMass();

public:
    /** Creates a new demo object. */
    cube_demo();
    virtual ~cube_demo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

};

// Method definitions
cube_demo::cube_demo() :
mass_aggregate_application(8), rods(0)
{
    // Create the masses and connections.
    particle_array[0].set_position(0 , 0, 10);
    particle_array[1].set_position(10, 0, 10);
    particle_array[2].set_position(10, 0, 0);
    particle_array[3].set_position(0 , 0, 0);
    particle_array[4].set_position(0 ,10,10);
    particle_array[5].set_position(10,10,10);
    particle_array[6].set_position(10,10, 0);
    particle_array[7].set_position(0 ,10, 0);
    for (unsigned i = 0; i < 8; i++)
    {
        particle_array[i].set_mass(BASE_MASS);
        particle_array[i].set_velocity(0,0,0);
        particle_array[i].set_damping(0.9f);
        particle_array[i].set_acceleration(ffiseg::vector::gravity);
        particle_array[i].clear_accumulator();
    }

    rods = new ffiseg::particle_rod[ROD_COUNT];

    rods[0].parts[0] = &particle_array[0];
    rods[0].parts[1] = &particle_array[1];
    rods[0].length = 10;
    rods[1].parts[0] = &particle_array[1];
    rods[1].parts[1] = &particle_array[2];
    rods[1].length = 10;
    rods[2].parts[0] = &particle_array[2];
    rods[2].parts[1] = &particle_array[3];
    rods[2].length = 10;
    rods[3].parts[0] = &particle_array[3];
    rods[3].parts[1] = &particle_array[0];
    rods[3].length = 10;


    rods[4].parts[0] = &particle_array[4];
    rods[4].parts[1] = &particle_array[5];
    rods[4].length = 10;
    rods[5].parts[0] = &particle_array[5];
    rods[5].parts[1] = &particle_array[6];
    rods[5].length = 10;
    rods[6].parts[0] = &particle_array[6];
    rods[6].parts[1] = &particle_array[7];
    rods[6].length = 10;
    rods[7].parts[0] = &particle_array[7];
    rods[7].parts[1] = &particle_array[4];
    rods[7].length = 10;

    
    rods[8].parts[0] = &particle_array[0];
    rods[8].parts[1] = &particle_array[4];
    rods[8].length = 10;
    rods[9].parts[0] = &particle_array[1];
    rods[9].parts[1] = &particle_array[5];
    rods[9].length = 10;
    rods[10].parts[0] = &particle_array[2];
    rods[10].parts[1] = &particle_array[6];
    rods[10].length = 10;
    rods[11].parts[0] = &particle_array[3];
    rods[11].parts[1] = &particle_array[7];
    rods[11].length = 10;

    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.get_contact_generators().push_back(&rods[i]);
    }
}

cube_demo::~cube_demo()
{
    if (rods) delete[] rods;
}


void cube_demo::display()
{
    mass_aggregate_application::display();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        ffiseg::particle **particles = rods[i].parts;
        const ffiseg::vector &p0 = particles[0]->get_position();
        const ffiseg::vector &p1 = particles[1]->get_position();
        glVertex3f(p0.get_x(), p0.get_y(), p0.get_z());
        glVertex3f(p1.get_x(), p1.get_y(), p1.get_z());
    }
    glEnd();

}

const char* cube_demo::getTitle()
{
    return "Cyclone > Platform Demo";
}


/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
application* get_application()
{
    return new cube_demo();
}
