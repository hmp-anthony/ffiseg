#include "ogl_headers.hpp"
#include "application.hpp"

#include<ffiseg/timer.hpp>

extern application* get_application();

application* app;

void create_window(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

void update() {
    ffiseg::timer::get().update();
    app->update();
}

void display() {
    app->display();
    glFlush();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    app->mouse(button, state, x, y);
}

void reshape(int width, int height) {
    app->resize(width, height);
}

void keyboard(unsigned char key, int x, int y) {
    app->key(key);
}

void motion(int x, int y) {
    app->mouse_drag(x, y);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    ffiseg::timer::init();

    app = get_application();
    create_window(app->get_title());

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    app->init_graphics();
    glutMainLoop();

    app->deinit();
    delete app;
    ffiseg::timer::deinit();
}
