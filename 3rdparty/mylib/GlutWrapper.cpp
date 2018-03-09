//
// Created by jmu on 18-3-2.
//

#include "GlutWrapper.h"
#include <GL/glut.h>

// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

/* 定义对键盘的响应函数 */
static void keyboard(unsigned char key, int x, int y) {
    /*按Esc键退出*/
    if(27 == key){
        exit(0);
    }
}

GlutWrapper::GlutWrapper() : onPreDraw(0), onDraw(0), onIdle(0), onKeyboard(keyboard) {}

GlutWrapper::GlutWrapper(FUNC_VV onPreDraw, FUNC_VV onDraw, FUNC_VV onIdle):
        onPreDraw(onPreDraw),
        onDraw(onDraw),
        onIdle(onIdle),
        onKeyboard(keyboard) {

}

void GlutWrapper::init(int *argc, char **argv) {
    init2(argc, argv);
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
}

void GlutWrapper::init2(int *argc, char **argv) {
    /* GLUT环境初始化*/
    glutInit(argc, argv);
    /* 显示模式初始化 */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    /* 定义窗口大小 */
    glutInitWindowSize(getWidth(), getHigh());
    /* 定义窗口位置 */
    glutInitWindowPosition(100, 100);
    /* 显示窗口，窗口标题为执行函数名 */
    glutCreateWindow(argv[0]);

    if (onPreDraw)
        onPreDraw();

    /* 注册OpenGL绘图函数 */
    if (onDraw)
        glutDisplayFunc(onDraw);

    if (onIdle)
        glutIdleFunc(onIdle);

    /* 注册键盘响应函数 */
    if (onKeyboard)
        glutKeyboardFunc(onKeyboard);
}

void GlutWrapper::startLoop(){
    glutMainLoop();
}

uint GlutWrapper::getWidth() const {
    return WIDTH;
}

uint GlutWrapper::getHigh() const {
    return HEIGHT;
}
