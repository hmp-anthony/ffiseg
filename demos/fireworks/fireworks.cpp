#include <ffiseg/ffiseg.hpp>

#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>
#include <string>

static ffiseg::random_generator frandom;

class firework : public ffiseg::particle {
public:
    unsigned type;

    // Think of age as fuse-left.
    ffiseg::real age;

    bool update(ffiseg::real duration) {
        integrate(duration);
        age -= duration;
        return (age < 0) || (position.get_y() < 0);
    }
};

struct firework_rule {
    unsigned type;
    ffiseg::real min_age;
    ffiseg::real max_age;
    ffiseg::vector min_velocity;
    ffiseg::vector max_velocity;
    ffiseg::real damping;

    struct payload {
        unsigned type;
        unsigned count;
        void set(unsigned type, unsigned count) {
            payload::type = type;
            payload::count = count;
        }
    };

    unsigned payload_count;
    payload* payloads;

    firework_rule() : payload_count(0), payloads(NULL) {}

    ~firework_rule() {
        if(payloads != NULL) {
            delete[] payloads;
            payloads = NULL;
            payload_count = 0;
        }
    }

    void init(unsigned count) {
        payload_count = count;
        payloads = new payload[payload_count];
    }

    void set_parameters(unsigned type,
                        ffiseg::real min_age,
                        ffiseg::real max_age,
                        const ffiseg::vector &min_velocity,
                        const ffiseg::vector &max_velocity,
                        ffiseg::real damping)
    {
        firework_rule::type = type;
        firework_rule::min_age = min_age;
        firework_rule::max_age = max_age;
        firework_rule::min_velocity = min_velocity;
        firework_rule::max_velocity = max_velocity;
        firework_rule::damping = damping;
    }

    void create(firework* fwork, const firework* parent = NULL) const {
        fwork->type = type;
        fwork->age = frandom.random_real(min_age, max_age);

        ffiseg::vector vel;
        if(parent) {
            fwork->set_position(parent->get_position());
            vel += parent->get_velocity();
        } else {
            ffiseg::vector start;
            int x = (int)frandom.random_int(3) - 1;
            start.set_x(5.0f * ffiseg::real(x));
            fwork->set_position(start);
        }
    
        vel += frandom.random_vector(min_velocity, max_velocity);
        fwork->set_velocity(vel);
        
        fwork->set_mass(1);
        fwork->set_damping(damping);
        fwork->set_acceleration(ffiseg::vector::gravity);
    }
};

class fireworks_demo : public application
{
public:
    fireworks_demo();
    ~fireworks_demo();

    virtual void init_graphics();
    virtual const char* get_title();
    virtual void update();
    virtual void display();
    virtual void key(unsigned char key);
private:
    const static unsigned max_fireworks = 1024;
    firework fireworks[max_fireworks];
    unsigned next_firework;

    const static unsigned rule_count = 9;
    firework_rule rules[rule_count];

    /** Dispatches a firework from the origin. */
    void create(unsigned type, const firework *parent=NULL);

    /** Dispatches the given number of fireworks from the given parent. */
    void create(unsigned type, unsigned number, const firework *parent);

    /** Creates the rules. */
    void init_firework_rules();

};

fireworks_demo::fireworks_demo() : next_firework(0)
{
    // Make all shots unused
    for (firework* fwork = fireworks; fwork < fireworks + max_fireworks; fwork++) {
        fwork->type = 0;
    }

    // Create the firework types
    init_firework_rules();
}

fireworks_demo::~fireworks_demo()
{
}

void fireworks_demo::init_firework_rules()
{
    // Go through the firework types and create their rules.
    rules[0].init(2);
    rules[0].set_parameters(
        1, // type
        0.5f, 1.4f, // age range
        ffiseg::vector(-5, 25, -5), // min velocity
        ffiseg::vector(5, 28, 5), // max velocity
        0.1 // damping
        );
    rules[0].payloads[0].set(3, 5);
    rules[0].payloads[1].set(5, 5);

    rules[1].init(1);
    rules[1].set_parameters(
        2, // type
        0.5f, 1.0f, // age range
        ffiseg::vector(-5, 10, -5), // min velocity
        ffiseg::vector(5, 20, 5), // max velocity
        0.8 // damping
        );
    rules[1].payloads[0].set(4, 2);

    rules[2].init(0);
    rules[2].set_parameters(
        3, // type
        0.5f, 1.5f, // age range
        ffiseg::vector(-5, -5, -5), // min velocity
        ffiseg::vector(5, 5, 5), // max velocity
        0.1 // damping
        );

    rules[3].init(0);
    rules[3].set_parameters(
        4, // type
        0.25f, 0.5f, // age range
        ffiseg::vector(-20, 5, -5), // min velocity
        ffiseg::vector(20, 5, 5), // max velocity
        0.2 // damping
        );

    rules[4].init(1);
    rules[4].set_parameters(
        5, // type
        0.5f, 1.0f, // age range
        ffiseg::vector(-20, 2, -5), // min velocity
        ffiseg::vector(20, 18, 5), // max velocity
        0.01 // damping
        );
    rules[4].payloads[0].set(3, 5);

    rules[5].init(0);
    rules[5].set_parameters(
        6, // type
        3, 5, // age range
        ffiseg::vector(-5, 5, -5), // min velocity
        ffiseg::vector(5, 10, 5), // max velocity
        0.95 // damping
        );

    rules[6].init(1);
    rules[6].set_parameters(
        7, // type
        4, 5, // age range
        ffiseg::vector(-5, 50, -5), // min velocity
        ffiseg::vector(5, 60, 5), // max velocity
        0.01 // damping
        );
    rules[6].payloads[0].set(8, 10);

    rules[7].init(0);
    rules[7].set_parameters(
        8, // type
        0.25f, 0.5f, // age range
        ffiseg::vector(-1, -1, -1), // min velocity
        ffiseg::vector(1, 1, 1), // max velocity
        0.01 // damping
        );

    rules[8].init(0);
    rules[8].set_parameters(
        9, // type
        3, 5, // age range
        ffiseg::vector(-15, 10, -5), // min velocity
        ffiseg::vector(15, 15, 5), // max velocity
        0.95 // damping
        );
}

void fireworks_demo::init_graphics()
{
    application::init_graphics();
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
}

const char* fireworks_demo::get_title()
{
    return "ffisg > Fireworks Demo";
}

void fireworks_demo::create(unsigned type, const firework *parent)
{
    // Get the rule needed to create this firework
    firework_rule* rule = rules + (type - 1);

    // Create the firework
    rule->create(fireworks + next_firework, parent);

    // Increment the index for the next firework
    next_firework = (next_firework + 1) % max_fireworks;
}

void fireworks_demo::create(unsigned type, unsigned number, const firework *parent)
{
    for (unsigned i = 0; i < number; i++)
    {
        create(type, parent);
    }
}

void fireworks_demo::update() {
    // Find the duration of the last frame in seconds
    float duration = (float)ffiseg::timer::get().last_frame_duration * 0.001f;
    if (duration <= 0.0f) return;

    for (firework* fwork = fireworks; fwork < fireworks + max_fireworks; fwork++)
    {
        // Check if we need to process this firework.
        if (fwork->type > 0)
        {
            // Does it need removing?
            if (fwork->update(duration))
            {
                // Find the appropriate rule
                firework_rule *rule = rules + (fwork->type-1);

                // Delete the current firework (this doesn't affect its
                // position and velocity for passing to the create function,
                // just whether or not it is processed for rendering or
                // physics.
                fwork->type = 0;

                // Add the payload
                for (unsigned i = 0; i < rule->payload_count; i++)
                {
                    firework_rule::payload* p = rule->payloads + i;
                    create(p->type, p->count, fwork);
                }
            }
        }
    }

    application::update();
}

void fireworks_demo::display()
{
    const static ffiseg::real size = 0.1f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 4.0, 10.0,  0.0, 4.0, 0.0,  0.0, 1.0, 0.0);

    std::string info_string = "Press the number keys to fire fireworks"; 
    glColor3f(0.1f, 1.0f, 1.0f);
    render_text(10.0f, 34.0f, info_string.c_str());

    glBegin(GL_QUADS);
    for (firework* fwork = fireworks; fwork < fireworks + max_fireworks; fwork++) {
        if (fwork->type > 0) {
            switch (fwork->type) {
                case 1: glColor3f(1,0,0); break;
                case 2: glColor3f(1,0.5f,0); break;
                case 3: glColor3f(1,1,0); break;
                case 4: glColor3f(0,1,0); break;
                case 5: glColor3f(0,1,1); break;
                case 6: glColor3f(0.4f,0.4f,1); break;
                case 7: glColor3f(1,0,1); break;
                case 8: glColor3f(1,1,1); break;
                case 9: glColor3f(1,0.5f,0.5f); break;
            };

            const ffiseg::vector &pos = fwork->get_position();
            
            auto c = ffiseg::vector(pos.get_x(), pos.get_y(), pos.get_z());
            auto x1 = ffiseg::vector(pos.get_x() - size, pos.get_y() - size, pos.get_z());
            auto x2 = ffiseg::vector(pos.get_x() + size, pos.get_y() - size, pos.get_z());
            auto x3 = ffiseg::vector(pos.get_x() + size, pos.get_y() + size, pos.get_z());
            auto x4 = ffiseg::vector(pos.get_x() - size, pos.get_y() + size, pos.get_z());

            // translate to the origin
            auto x1o = x1 - c;
            auto x2o = x2 - c;
            auto x3o = x3 - c;
            auto x4o = x4 - c;

            // rotate
            auto theta = ffiseg::timer::get().get_time() * 0.01;
            auto y1o = ffiseg::vector(cos(theta) * x1o.get_x() - sin(theta) * x1o.get_y(),
                                      sin(theta) * x1o.get_x() + cos(theta) * x1o.get_y(), 
                                      x1o.get_z());
            auto y2o = ffiseg::vector(cos(theta) * x2o.get_x() - sin(theta) * x2o.get_y(),
                                      sin(theta) * x2o.get_x() + cos(theta) * x2o.get_y(),
                                      x2o.get_z());
            auto y3o = ffiseg::vector(cos(theta) * x3o.get_x() - sin(theta) * x3o.get_y(),
                                      sin(theta) * x3o.get_x() + cos(theta) * x3o.get_y(),
                                      x3o.get_z());
            auto y4o = ffiseg::vector(cos(theta) * x4o.get_x() - sin(theta) * x4o.get_y(),
                                      sin(theta) * x4o.get_x() + cos(theta) * x4o.get_y(),
                                      x4o.get_z());

            // translate back
            auto y1 = y1o + c;
            auto y2 = y2o + c;
            auto y3 = y3o + c;
            auto y4 = y4o + c;

            glVertex3f(y1.get_x(), y1.get_y(), y1.get_z());
            glVertex3f(y2.get_x(), y2.get_y(), y2.get_z());
            glVertex3f(y3.get_x(), y3.get_y(), y3.get_z());
            glVertex3f(y4.get_x(), y4.get_y(), y4.get_z());

            // Render the firework's reflection
            glVertex3f(y1.get_x(), -y1.get_y(), y1.get_z());
            glVertex3f(y2.get_x(), -y2.get_y(), y2.get_z());
            glVertex3f(y3.get_x(), -y3.get_y(), y3.get_z());
            glVertex3f(y4.get_x(), -y4.get_y(), y4.get_z());
        }
    }
    
    glEnd();
}

void fireworks_demo::key(unsigned char key)
{
    switch (key) {
        case '1': create(1, 1, NULL); break;
        case '2': create(2, 1, NULL); break;
        case '3': create(3, 1, NULL); break;
        case '4': create(4, 1, NULL); break;
        case '5': create(5, 1, NULL); break;
        case '6': create(6, 1, NULL); break;
        case '7': create(7, 1, NULL); break;
        case '8': create(8, 1, NULL); break;
        case '9': create(9, 1, NULL); break;
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
application* get_application()
{
    return new fireworks_demo();
}
