//
// Created by jmu on 18-3-5.
//
#include <mylib/common.h>
#include <mylib/Shader.h>
#include <mylib/GlutWrapper.h>

//glm
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

static void onPreDraw(){

}

static void onDraw(){

}

int main(int argc, char* argv[]){
    GlutWrapper(onPreDraw, onDraw)
            .init(&argc, argv);
    return 0;
}