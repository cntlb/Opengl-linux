//
// Created by jmu on 18-2-28.
//

#include <GL/glut.h>
#include "mylib/log.h"

#define LOG_TAG "opengl_info.c"

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(100, 100);
    glutCreateWindow("LearnOpenGL");

    LOGE("shading language version:%s", glGetString(GL_SHADING_LANGUAGE_VERSION))
    const GLubyte *name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte *biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte *OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    const GLubyte *shading_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte *gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本

    LOGE("OpenGL实现厂商的名字：%s", name);
    LOGE("渲染器标识符：%s", biaoshifu);
    LOGE("OpenGL实现的版本号：%s", OpenGLVersion);
    LOGE("GLU工具库版本：%s", gluVersion);
    LOGE("着色器语言版本：%s", shading_version);

    glutMainLoop();

    return 0;
}