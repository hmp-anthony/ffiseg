add_executable(cube cube.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(cube ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(cube application particle_link particle timer core)
