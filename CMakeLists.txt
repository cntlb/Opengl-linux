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

function(gldemo name)
    project(${name})
    add_executable(${name} ${name}.c)
    target_link_libraries(${name} GL GLU glut m)
endfunction()

gldemo(test2)
gldemo(ge_graph)
gldemo(gl_vertex)
gldemo(gl_fly)
gldemo(gl_vertexcolor)
gldemo(gl_transform)
gldemo(gl_teapot)


function(leanopengl name)
    project(${name})
        include_directories(3rdparty)
    add_executable(${name} leanopengl/${name}.c)
    target_link_libraries(${name} GL GLU glut m)
endfunction()
leanopengl(opengl_info)
leanopengl(1.4_window)
leanopengl(1.5_triangle)
leanopengl(1.5_ractangle)
leanopengl(1.5_triangle_exercise)
leanopengl(1.6_shader_uniform)
leanopengl(1.6_shader_moreattr)
