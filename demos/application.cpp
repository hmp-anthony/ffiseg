#include<ffiseg/timer.hpp>

#include<cstring>

#include "application.hpp"
#include "ogl_headers.hpp"


void application::init_graphics()
{
    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    set_view();
}

void application::set_view()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width/(double)height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void application::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(639, 319);
    glEnd();
}

const char* application::get_title()
{
    return "Cyclone Demo";
}

void application::deinit()
{
}

void application::update()
{
    glutPostRedisplay();
}

void application::key(unsigned char key)
{
}


void application::resize(int width, int height)
{
    // Avoid the divide by zero.
    if (height <= 0) height = 1;

    // Set the internal variables and update the view
    application::width = width;
    application::height = height;
    glViewport(0, 0, width, height);
    set_view();
}

void application::mouse(int button, int state, int x, int y)
{
}

void application::mouse_drag(int x, int y)
{
}

// The following methods aren't intended to be overloaded
void application::render_text(float x, float y, const char *text, void *font)
{
    glDisable(GL_DEPTH_TEST);

    // Temporarily set up the view in orthographic projection.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

    // Move to modelview mode.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Ensure we have a font
    if (font == NULL) {
        font = GLUT_BITMAP_HELVETICA_10;
    }

    // Loop through characters displaying them.
    size_t len = strlen(text);

    glRasterPos2f(x, y);
    for (const char *letter = text; letter < text+len; letter++) {

        // If we meet a newline, then move down by the line-height
        // TODO: Make the line-height a function of the font
        if (*letter == '\n') {
            y -= 12.0f;
            glRasterPos2f(x, y);
        }
        glutBitmapCharacter(font, *letter);
    }

    // Pop the matrices to return to how we were before.
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

mass_aggregate_application::mass_aggregate_application(unsigned int particle_count)
                           : world(particle_count*10)
{
    particle_array = new ffiseg::particle[particle_count];
    for (unsigned i = 0; i < particle_count; i++)
    {
        world.get_particles().push_back(particle_array + i);
    }

    ground_contact_generator.init(&world.get_particles());
    world.get_contact_generators().push_back(&ground_contact_generator);
}

mass_aggregate_application::~mass_aggregate_application()
{
    delete[] particle_array;
}

void mass_aggregate_application::update()
{
    // Clear accumulators
    world.start_frame();
    // Find the duration of the last frame in seconds
    float duration = (float)ffiseg::timer::get().last_frame_duration * 0.001f;
    if (duration <= 0.0f) return;
    
    // Run the simulation
    world.run_physics(duration);
    application::update();
}

void mass_aggregate_application::init_graphics()
{
    // Call the superclass
    application::init_graphics();
}

void mass_aggregate_application::display()
{
    // Clear the view port and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 3.5, 8.0,  0.0, 3.5, 0.0,  0.0, 1.0, 0.0);
    glTranslatef(0, 0, -15);
    glColor3f(0,0,0);

    auto parts = world.get_particles();
    for (std::vector<ffiseg::particle*>::iterator p = parts.begin();
        p != parts.end();
        p++)
    {
        ffiseg::particle *part = *p;
        const ffiseg::vector &pos = part->get_position();
        glPushMatrix();
        glTranslatef(pos.get_x(), pos.get_y(), pos.get_z());
        glutSolidSphere(0.1f, 20, 10);
        glPopMatrix();
    }
}

void mass_aggregate_application::add_force_gen_to_registry(ffiseg::particle* p, ffiseg::particle_force_generator* pfgen) {
    world.add_force_gen_to_registry(p, pfgen);
}
