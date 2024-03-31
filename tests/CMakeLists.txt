
add_executable(core_test core_test.cpp)
target_link_libraries(core_test gtest_main)
add_test(NAME core_test COMMAND core_test)

add_executable(particle_test particle_test.cpp)
target_link_libraries(particle_test gtest_main)
add_test(NAME particle_test COMMAND particle_test)
