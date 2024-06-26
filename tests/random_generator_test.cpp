#include <ffiseg/random_generator.hpp>

#include "gtest/gtest.h"

using namespace ffiseg;


TEST(random_generator, get_buffer) {
    random_generator r(10);
    auto buf = r.get_buffer();
    ASSERT_EQ(buf[0], 3143560755);
    ASSERT_EQ(buf[1], 3250952192);
    ASSERT_EQ(buf[2], 3160668161);
    ASSERT_EQ(buf[3], 4092732166);
    ASSERT_EQ(buf[4], 2147578143);
    ASSERT_EQ(buf[5], 3231009436);
    ASSERT_EQ(buf[6], 1613637901);
    ASSERT_EQ(buf[7], 3981155394);
    ASSERT_EQ(buf[8], 2674746187);
    ASSERT_EQ(buf[9], 3899386232);
    ASSERT_EQ(buf[10], 902936409);
    ASSERT_EQ(buf[11], 3786778558);
    ASSERT_EQ(buf[12], 2726621367);
    ASSERT_EQ(buf[13], 4036513940);
    ASSERT_EQ(buf[14], 3561811685);
    ASSERT_EQ(buf[15], 3429613946);
    ASSERT_EQ(buf[16], 2261091683);
}

TEST(random_generator, random_real) {
    random_generator r1(10);
    ASSERT_NEAR(r1.random_real(), 0.95617, 0.01);
    random_generator r2(100);
    ASSERT_NEAR(r2.random_real(), 0.74995, 0.01);

    auto rr1 = r1.random_real(1.0, 2.0);
    ASSERT_TRUE(rr1 <= 2.0);
    ASSERT_TRUE(rr1 >= 1.0);
}


TEST(random_generator, random_vector) {
    random_generator r(5);
    vector min(13.0, 15.0, 17.0);
    vector max(17.0, 16.0, 18.0);

    vector rvector = r.random_vector(min, max);    
    ASSERT_TRUE(rvector.get_x() <= 17.0);
    ASSERT_TRUE(rvector.get_x() >= 13.0);
    ASSERT_TRUE(rvector.get_y() <= 16.0);
    ASSERT_TRUE(rvector.get_y() >= 15.0);
    ASSERT_TRUE(rvector.get_z() <= 18.0);
    ASSERT_TRUE(rvector.get_z() >= 17.0);
    rvector = r.random_vector(min, max);    
    ASSERT_TRUE(rvector.get_x() <= 17.0);
    ASSERT_TRUE(rvector.get_x() >= 13.0);
    ASSERT_TRUE(rvector.get_y() <= 16.0);
    ASSERT_TRUE(rvector.get_y() >= 15.0);
    ASSERT_TRUE(rvector.get_z() <= 18.0);
    ASSERT_TRUE(rvector.get_z() >= 17.0);
}
