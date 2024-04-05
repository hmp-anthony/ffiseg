find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
add_library(application application.cpp application.hpp)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(application ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

add_subdirectory(ballistic)
add_subdirectory(fireworks)
