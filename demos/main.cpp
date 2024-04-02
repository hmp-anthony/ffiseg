#include "ogl_headers.hpp"
#include "application.hpp"

#include<ffiseg/timer.hpp>

// Forward declaration of the function that will return the
// application object for this particular demo. This should be
// implemented in the demo's .cpp file.
extern application* get_application();

// Store the global application object.
application* app;

/**
 * Creates a window in which to display the scene.
 */
void create_window(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

/**
 * Called each frame to update the 3D scene. Delegates to
 * the application.
 */
void update()
{
    // Update the timing.
    ffiseg::timer::get().update();

    // Delegate to the application.
    app->update();
}

/**
 * Called each frame to display the 3D scene. Delegates to
 * the application.
 */
void display()
{
    app->display();

    // Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Called when a mouse button is pressed. Delegates to the
 * application.
 */
void mouse(int button, int state, int x, int y)
{
    app->mouse(button, state, x, y);
}

/**
 * Called when the display window changes size.
 */
void reshape(int width, int height)
{
    app->resize(width, height);
}

/**
 * Called when a key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    // Note we omit passing on the x and y: they are rarely needed.
    app->key(key);
}

/**
 * Called when the mouse is dragged.
 */
void motion(int x, int y)
{
    app->mouse_drag(x, y);
}

/**
 * The main entry point. We pass arguments onto GLUT.
 */
int main(int argc, char** argv)
{
    // Set up GLUT and the timers
    glutInit(&argc, argv);
    ffiseg::timer::init();

    // Create the application and its window
    app = get_application();
    create_window(app->get_title());

    // Set up the appropriate handler functions
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Run the application
    app->init_graphics();
    glutMainLoop();

    // Clean up the application
    app->deinit();
    delete app;
    ffiseg::timer::deinit();
}
