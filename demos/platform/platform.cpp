#include <ffiseg/ffiseg.hpp>
#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <cassert>
#include <iostream>

#define ROD_COUNT 15

#define BASE_MASS 1
#define EXTRA_MASS 10

/**
 * The main demo class definition.
 */
class platform_demo : public mass_aggregate_application
{
    ffiseg::particle_rod *rods;

    ffiseg::vector massPos;
    ffiseg::vector massDisplayPos;

    /**
     * Updates particle masses to take into account the mass
     * that's on the platform.
     */
    void updateAdditionalMass();

public:
    /** Creates a new demo object. */
    platform_demo();
    virtual ~platform_demo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);
};

// Method definitions
platform_demo::platform_demo() :
mass_aggregate_application(6), rods(0), massPos(0,0,0.5f)
{

    std::cout << "constructor begin" << std::endl;
    // Create the masses and connections.
    particle_array[0].set_position(0,0,1);
    particle_array[1].set_position(0,0,-1);
    particle_array[2].set_position(-3,2,1);
    particle_array[3].set_position(-3,2,-1);
    particle_array[4].set_position(4,2,1);
    particle_array[5].set_position(4,2,-1);
    for (unsigned i = 0; i < 6; i++)
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
    rods[0].length = 2;
    rods[1].parts[0] = &particle_array[2];
    rods[1].parts[1] = &particle_array[3];
    rods[1].length = 2;
    rods[2].parts[0] = &particle_array[4];
    rods[2].parts[1] = &particle_array[5];
    rods[2].length = 2;

    rods[3].parts[0] = &particle_array[2];
    rods[3].parts[1] = &particle_array[4];
    rods[3].length = 7;
    rods[4].parts[0] = &particle_array[3];
    rods[4].parts[1] = &particle_array[5];
    rods[4].length = 7;

    rods[5].parts[0] = &particle_array[0];
    rods[5].parts[1] = &particle_array[2];
    rods[5].length = 3.606;
    rods[6].parts[0] = &particle_array[1];
    rods[6].parts[1] = &particle_array[3];
    rods[6].length = 3.606;

    rods[7].parts[0] = &particle_array[0];
    rods[7].parts[1] = &particle_array[4];
    rods[7].length = 4.472;
    rods[8].parts[0] = &particle_array[1];
    rods[8].parts[1] = &particle_array[5];
    rods[8].length = 4.472;

    rods[9].parts[0] = &particle_array[0];
    rods[9].parts[1] = &particle_array[3];
    rods[9].length = 4.123;
    rods[10].parts[0] = &particle_array[2];
    rods[10].parts[1] = &particle_array[5];
    rods[10].length = 7.28;
    rods[11].parts[0] = &particle_array[4];
    rods[11].parts[1] = &particle_array[1];
    rods[11].length = 4.899;
    rods[12].parts[0] = &particle_array[1];
    rods[12].parts[1] = &particle_array[2];
    rods[12].length = 4.123;
    rods[13].parts[0] = &particle_array[3];
    rods[13].parts[1] = &particle_array[4];
    rods[13].length = 7.28;
    rods[14].parts[0] = &particle_array[5];
    rods[14].parts[1] = &particle_array[0];
    rods[14].length = 4.899;

    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.get_contact_generators().push_back(&rods[i]);
    }

    updateAdditionalMass();
    std::cout << "end" << std::endl;
}

platform_demo::~platform_demo()
{
    if (rods) delete[] rods;
}

void platform_demo::updateAdditionalMass()
{

    std::cout << "updateAdditional Mass" << std::endl;
    for (unsigned i = 2; i < 6; i++)
    {
        particle_array[i].set_mass(BASE_MASS);
    }

    // Find the coordinates of the mass as an index and proportion
    ffiseg::real xp = massPos.get_x();
    if (xp < 0) xp = 0;
    if (xp > 1) xp = 1;

    ffiseg::real zp = massPos.get_z();
    if (zp < 0) zp = 0;
    if (zp > 1) zp = 1;

    // Calculate where to draw the mass
    massDisplayPos.clear();

    // Add the proportion to the correct masses
    particle_array[2].set_mass(BASE_MASS + EXTRA_MASS*(1-xp)*(1-zp));
    massDisplayPos.add_scaled_vector(
        particle_array[2].get_position(), (1-xp)*(1-zp)
        );

    if (xp > 0)
    {
        particle_array[4].set_mass(BASE_MASS + EXTRA_MASS*xp*(1-zp));
        massDisplayPos.add_scaled_vector(
            particle_array[4].get_position(), xp*(1-zp)
            );

        if (zp > 0)
        {
            particle_array[5].set_mass(BASE_MASS + EXTRA_MASS*xp*zp);
            massDisplayPos.add_scaled_vector(
                particle_array[5].get_position(), xp*zp
                );
        }
    }
    if (zp > 0)
    {
        particle_array[3].set_mass(BASE_MASS + EXTRA_MASS*(1-xp)*zp);
        massDisplayPos.add_scaled_vector(
            particle_array[3].get_position(), (1-xp)*zp
            );
    }

    std::cout << "update additional mass end" << std::endl;
}

void platform_demo::display()
{
    std::cout << "display begin" << std::endl;
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

    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(massDisplayPos.get_x(), massDisplayPos.get_y() + 0.25f, massDisplayPos.get_z());
    glutSolidSphere(0.25f, 20, 10);
    glPopMatrix();
    std::cout << "display end" << std::endl;
}

void platform_demo::update()
{
    mass_aggregate_application::update();

    updateAdditionalMass();
}

const char* platform_demo::getTitle()
{
    return "Cyclone > Platform Demo";
}

void platform_demo::key(unsigned char key)
{ 
    switch(key)
    {
    case 'w': case 'W': {
        auto mpz = massPos.get_z();
        massPos.set_z(mpz + 0.1f);
        if (massPos.get_z() > 1.0f) massPos.set_z(1.0f);
        break;}
    case 's': case 'S': {
        auto mpz = massPos.get_z();
        massPos.set_z(mpz - 0.1f);
        if (massPos.get_z() < 0.0f) massPos.set_z(0.0f);
        break;}
    case 'a': case 'A': {
        auto mpx = massPos.get_x();
        massPos.set_x(mpx + 0.1f);
        if (massPos.get_x() > 1.0f) massPos.set_x(1.0f);
        break;}
    case 'd': case 'D': {
        auto mpx = massPos.get_x();
        massPos.set_x(mpx - 0.1f);
        if (massPos.get_x() < 0.0f) massPos.set_x(0.0f);
        break;}

    default:
        mass_aggregate_application::key(key);
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
application* get_application()
{
    return new platform_demo();
}
