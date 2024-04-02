add_library(particle particle.cpp ../include/ffiseg/particle.hpp)
target_include_directories(particle PUBLIC ../include)

add_library(timer timer.cpp ../include/ffiseg/timer.hpp)
target_include_directories(timer PUBLIC ../include)
