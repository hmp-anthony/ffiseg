#include <ffiseg/particle_force_generator.hpp>
#include <ffiseg/ffiseg.hpp>

#include "gtest/gtest.h"

using namespace ffiseg;

TEST(particle_force_registry, registrations) {
    ffiseg::particle_force_registry pfr;
    ffiseg::particle p;
    ASSERT_EQ(1,1);
}

