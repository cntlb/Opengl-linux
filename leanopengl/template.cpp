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

void onPreDraw(){

}

void onDraw(){

}

int main(int argc, char* argv[]){
    GlutWrapper wrapper;
    wrapper.onDraw = onDraw;
    wrapper.onPreDraw = onPreDraw;
    wrapper.onKeyboard = &GlutWrapper::keyboard;
    wrapper.init(&argc, argv);
    return 0;
}