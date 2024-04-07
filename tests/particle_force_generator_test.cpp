#include <ffiseg/particle_force_generator.hpp>
#include <ffiseg/ffiseg.hpp>

#include "gtest/gtest.h"

using namespace ffiseg;

TEST(particle_force_registry, particle_spring) {
    ffiseg::particle_force_registry pfr;
    ffiseg::particle p(0, 0, 0);
    ffiseg::particle q(0, 5.0f, 0);
    p.set_mass(1000000.0f);
    q.set_mass(1.0f);

    // equal an opposite, and all that
    ffiseg::particle_spring ps_p(&p, 4.0, 1.0);
    pfr.add(&q, &ps_p);
    ffiseg::particle_spring ps_q(&q, 4.0, 1.0);
    pfr.add(&p, &ps_q);
   
    pfr.update_forces(1.0f);

    auto reg_q = pfr.get_registrations()[0];
    reg_q.part->integrate(1.0f);
    reg_q.part->integrate(1.0f);

    ASSERT_EQ(reg_q.part->get_mass(), 1);
    ASSERT_EQ(reg_q.part->get_position().get_y(), -11);
}

