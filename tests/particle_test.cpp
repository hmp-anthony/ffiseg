#include <ffiseg/particle.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "gtest/gtest.h"

using namespace ffiseg;

TEST(particle, integrate) {
    ffiseg::particle p;
    p.set_position(0.0, 0.0, 0.0);
    p.set_velocity(1.0, 2.0, 3.0);
    p.set_acceleration(0.0, 0.0, 0.0);

    p.integrate(1.0);
    auto pos = p.get_position();
    // inverse_mass has not been sent
    // so integrate returns early!
    ASSERT_NEAR(pos.get_x(), 0.0, 0.001);
    ASSERT_NEAR(pos.get_y(), 0.0, 0.001);
    ASSERT_NEAR(pos.get_z(), 0.0, 0.001);

    p.set_mass(10.0);
    p.integrate(1.0);
    pos = p.get_position();
    ASSERT_NEAR(pos.get_x(), 1.0, 0.001);
    ASSERT_NEAR(pos.get_y(), 2.0, 0.001);
    ASSERT_NEAR(pos.get_z(), 3.0, 0.001);

    ffiseg::particle q;
    q.set_mass(10.0);
    q.set_damping(0.995);
    q.set_position(1.0, 1.0, 1.0);
    q.set_velocity(1.0, 2.0, 3.0);
    q.set_acceleration(10.0, 1.0, 10.0);
    q.integrate(0.5);

    pos = q.get_position();
    auto vel = q.get_velocity();
    ASSERT_NEAR(pos.get_x(), 1.5, 0.001);
    ASSERT_NEAR(pos.get_y(), 2.0, 0.001);
    ASSERT_NEAR(pos.get_z(), 2.5, 0.001);
    ASSERT_NEAR(vel.get_x(), 5.9849, 0.001);
    ASSERT_NEAR(vel.get_y(), 2.4937, 0.001);
    ASSERT_NEAR(vel.get_z(), 7.9799, 0.001);
}

TEST(particle, mass) {
    ffiseg::particle p;
    p.set_mass(30.0);
    ASSERT_NEAR(p.get_mass(), 30.0, 0.01);
    ASSERT_NEAR(p.get_inverse_mass(), 0.033, 0.01);
}

TEST(particle, inverse_mass) {
    ffiseg::particle p;
    p.set_inverse_mass(30.0);
    ASSERT_NEAR(p.get_mass(), 0.033, 0.01);
    ASSERT_NEAR(p.get_inverse_mass(), 30.0, 0.01);

    p.set_inverse_mass(0.0);
    ASSERT_NEAR(p.get_mass(), REAL_MAX, 0.01);
    ASSERT_NEAR(p.get_inverse_mass(), 0.0, 0.01);
}

TEST(particle, damping) {
    ffiseg::particle p;
    p.set_damping(10.0);
    ASSERT_NEAR(p.get_damping(), 10.0, 0.01);
}

TEST(particle, position) {
    ffiseg::particle p;
    p.set_position(1.0, 2.0, 1.0);

    auto pos = p.get_position();
    ASSERT_NEAR(pos.get_x(), 1.0, 0.001);
    ASSERT_NEAR(pos.get_y(), 2.0, 0.001);
    ASSERT_NEAR(pos.get_z(), 1.0, 0.001);
}

TEST(particle, velocity) {
    ffiseg::particle p;
    p.set_velocity(1.0, 2.0, 3.0);

    auto vel = p.get_velocity();
    ASSERT_NEAR(vel.get_x(), 1.0, 0.001);
    ASSERT_NEAR(vel.get_y(), 2.0, 0.001);
    ASSERT_NEAR(vel.get_z(), 3.0, 0.001);
}
