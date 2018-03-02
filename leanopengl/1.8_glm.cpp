//
// Created by jmu on 18-3-2.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "mylib/Shader.h"

float * addr(glm::mat4 mat){
    return glm::value_ptr(mat);
}

//glm test
int main(){

    glm::mat4 trans;
    trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    float *ptr = glm::value_ptr(trans);

    GLfloat trans2[] = {
            0, 1, 0, 0,
            -1, 0, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    return 0;
}