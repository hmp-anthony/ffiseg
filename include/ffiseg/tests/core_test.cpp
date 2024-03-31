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

TEST(vector, addition) {
    ffiseg::vector v1(1.0, 2.0, 3.0);
    ffiseg::vector v2(3.0, 2.0, 1.0);
    auto w = v1 + v2;

    ASSERT_EQ(w.get_x(), 4.0);
    ASSERT_EQ(w.get_y(), 4.0);
    ASSERT_EQ(w.get_z(), 4.0);

    ffiseg::vector v3(1.0, 1.0, 1.0);
    v1 += v3;
    ASSERT_EQ(v1.get_x(), 2.0);
    ASSERT_EQ(v1.get_y(), 3.0);
    ASSERT_EQ(v1.get_z(), 4.0);
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

TEST(vector, normalize) {
    ffiseg::vector v(1.0, 2.0, 3.0);
    v.normalize();
    ASSERT_NEAR(v.get_x(), 0.2673, 0.001); 
    ASSERT_NEAR(v.get_y(), 0.5345, 0.001); 
    ASSERT_NEAR(v.get_z(), 0.8018, 0.001); 
}

