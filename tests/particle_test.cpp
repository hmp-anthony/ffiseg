#include <ffiseg/particle.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "gtest/gtest.h"

using namespace ffiseg;

TEST(particle, init) {
    ffiseg::particle p;
    ASSERT_EQ(1,1);
}


