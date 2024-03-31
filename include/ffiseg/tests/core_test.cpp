#include <ffiseg/core.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "gtest/gtest.h"

TEST(vector, scalar_multiplication) {
    ffiseg::vector v(1.0, 2.0, 3.0);
    v *= 5.0;
    ASSERT_EQ(v.get_x(), 5.0);
    ASSERT_EQ(v.get_y(), 10.0);
    ASSERT_EQ(v.get_z(), 15.0);

    auto w = v * 9.0;
    ASSERT_EQ(w.get_x(), 45.0);
    ASSERT_EQ(w.get_y(), 90.0);
    ASSERT_EQ(w.get_z(), 135.0);
}

TEST(vector, invert) {
    ffiseg::vector v(1.0, 2.0, 3.0);
    v.invert();
    ASSERT_EQ(-1.0, v.get_x());
    ASSERT_EQ(-2.0, v.get_y());
    ASSERT_EQ(-3.0, v.get_z());
}

TEST(vector, magnitude) {
    ffiseg::vector v(1.0, 2.0, 3.0);
    ASSERT_NEAR(v.magnitude(), 3.7416 , 0.001);
}

TEST(vector, square_magnitude) {
    ffiseg::vector v(1.0, 2.0, 3.0);
    ASSERT_NEAR(v.square_magnitude(), 14.0 , 0.001); 
}
