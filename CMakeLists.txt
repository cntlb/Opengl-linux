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
leanopengl(1.7_texture)
leanopengl(1.7_texture2)


function(leanopengl_cpp name)
    project(${name})
    set(CMAKE_CXX_FLAGS  -std=c++14)
    include_directories(3rdparty)
    add_executable(${name}
            leanopengl/${name}.cpp
            3rdparty/mylib/Shader.cpp
            3rdparty/stb_image.c
            3rdparty/mylib/GlutWrapper.cpp
            )
    target_link_libraries(${name} GL GLU glut m)
endfunction()
leanopengl_cpp(template)
leanopengl_cpp(1.7_texture2pp)
leanopengl_cpp(1.8_glm)
leanopengl_cpp(1.8_transform)
leanopengl_cpp(1.8_coordinate1)
leanopengl_cpp(1.8_coordinate2)
leanopengl_cpp(1.8_coordinate3)
leanopengl_cpp(1.10_camera)
leanopengl_cpp(1.10_camera2)
leanopengl_cpp(1.10_camera3)
leanopengl_cpp(1.10_camera4)

leanopengl_cpp(2.1_light)
leanopengl_cpp(2.2_light)
leanopengl_cpp(2.2_light2)
leanopengl_cpp(2.2_light3)
leanopengl_cpp(2.3_material)
leanopengl_cpp(2.3_material2)
leanopengl_cpp(2.4_texture)
leanopengl_cpp(2.4_texture2)
leanopengl_cpp(2.4_texture3)

function(leanopengl_assimp name)
    project(${name})
        set(CMAKE_CXX_FLAGS  -std=c++14)
        ADD_DEFINITIONS(-DROOT="${CMAKE_SOURCE_DIR}")
        include_directories(3rdparty)
        link_directories(libs)
        add_executable(${name}
            leanopengl/${name}.cpp
            3rdparty/mylib/Shader.cpp
            3rdparty/stb_image.c
            3rdparty/mylib/GlutWrapper.cpp
            )
        target_link_libraries(${name} GL GLU glut m assimp)
endfunction()
leanopengl_assimp(3.1_assimp)
leanopengl_assimp(3.3_model_loading)
leanopengl_assimp(3.2_load_file_test)
leanopengl_assimp(3.3_simple_load)
