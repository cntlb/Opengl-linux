//
// Created by jmu on 18-2-27.
//

#include <GL/glut.h>
#include <stdio.h>
#include "mylib/log.h"
#define LOG_TAG "hello_window.c"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

void myDisplay(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void callback( unsigned char c, int i, int i2){
    LOGE("key:%d('%c')", c, c)
    if(c == 'q'){
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("LearnOpenGL");

    glutDisplayFunc(&myDisplay);
//    glutIdleFunc(&myIdle);     //空闲调用

    glutKeyboardUpFunc(callback);

    glutMainLoop();
    return 0;
}