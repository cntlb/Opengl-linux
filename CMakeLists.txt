cmake_minimum_required(VERSION 3.7)
project(OpenglDemo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(OpenglDemo ${SOURCE_FILES})


project(test1)
link_directories(/home/jmu/Desktop/test/opengl/lib)
include_directories(/home/jmu/Desktop/test/opengl/include)
add_executable(test1 test1.c)
target_link_libraries(test1 GLEW glfw3 GL X11 pthread Xrandr Xi Xxf86vm m dl)

