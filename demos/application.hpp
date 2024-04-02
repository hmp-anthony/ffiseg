#ifndef APPLICATION_H
#define APPLICATION_H

#include<cstdlib>

#include<ffiseg/ffiseg.hpp>

class application {
public:
    virtual const char* get_title();
    virtual void init_graphics();
    virtual void set_view();
    virtual void deinit();
    virtual void display();
    virtual void update();
    virtual void key(unsigned char key);
    virtual void resize(int width, int height);
    virtual void mouse(int button, int state, int x, int y);
    virtual void mouse_drag(int x, int y);
    void render_text(float x, float y, const char *text, void* font=NULL);
protected:
    int height;
    int width;
};

#endif
