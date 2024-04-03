#include <ffiseg/ffiseg.hpp>
#include <ffiseg/timer.hpp>

#include "../ogl_headers.hpp"
#include "../application.hpp"

#include <stdio.h>

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

};
