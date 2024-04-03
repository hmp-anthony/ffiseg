add_library(particle particle.cpp ../include/ffiseg/particle.hpp)
target_include_directories(particle PUBLIC ../include)

add_library(timer timer.cpp ../include/ffiseg/timer.hpp)
target_include_directories(timer PUBLIC ../include)

add_library(random random.cpp ../include/ffiseg/random.hpp)
target_include_directories(random PUBLIC ../include)
