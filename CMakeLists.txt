cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
project(ffiseg CXX)

enable_testing()

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)
