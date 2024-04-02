#include <ffiseg/timer.hpp>

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

using namespace ffiseg;

TEST(timer, frame_number) {
    timer::init();
    ASSERT_EQ(timer::get().frame_number, 0);
    timer::update();
    ASSERT_EQ(timer::get().frame_number, 1);
    timer::update();
    ASSERT_EQ(timer::get().frame_number, 2);
    timer::update();
    ASSERT_EQ(timer::get().frame_number, 3);
    timer::deinit();
}

TEST(timer, frame_duration) {
    timer::init();
    timer::update();
    using namespace std::this_thread; 
    using namespace std::chrono;

    sleep_until(system_clock::now() + seconds(1));
    timer::update();
    ASSERT_NEAR(timer::get().last_frame_duration, 1000, 1);
    timer::deinit();
}
