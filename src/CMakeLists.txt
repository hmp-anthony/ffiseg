add_library(particle particle.cpp ../include/ffiseg/particle.hpp)
target_include_directories(particle PUBLIC ../include)

add_library(timer timer.cpp ../include/ffiseg/timer.hpp)
target_include_directories(timer PUBLIC ../include)

add_library(random_generator random_generator.cpp ../include/ffiseg/random_generator.hpp)
target_include_directories(random_generator PUBLIC ../include)

add_library(core core.cpp ../include/ffiseg/core.hpp)
target_include_directories(core PUBLIC ../include)

add_library(particle_force_generator particle_force_generator.cpp ../include/ffiseg/particle_force_generator.hpp)
target_include_directories(particle_force_generator PUBLIC ../include)

add_library(particle_contact particle_contact.cpp ../include/ffiseg/particle_contact.hpp)
target_include_directories(particle_contact PUBLIC ../include)

add_library(particle_link particle_link.cpp ../include/ffiseg/particle_link.hpp)
target_include_directories(particle_link PUBLIC ../include)

add_library(particle_world particle_world.cpp ../include/ffiseg/particle_world.hpp)
target_include_directories(particle_world PUBLIC ../include)
