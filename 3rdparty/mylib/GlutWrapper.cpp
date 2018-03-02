//
// Created by jmu on 18-3-2.
//

#include "GlutWrapper.h"
#include <GL/glut.h>
// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

void GlutWrapper::init(int *argc, char **argv) {

    /* GLUT环境初始化*/
    glutInit(argc, argv);
    /* 显示模式初始化 */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    /* 定义窗口大小 */
    glutInitWindowSize(WIDTH, HEIGHT);
    /* 定义窗口位置 */
    glutInitWindowPosition(100, 100);
    /* 显示窗口，窗口标题为执行函数名 */
    glutCreateWindow(argv[0]);

    if(onPreDraw)
        onPreDraw();

    /* 注册OpenGL绘图函数 */
    if(onDraw)
        glutDisplayFunc(onDraw);

    if(onIdle)
        glutIdleFunc(onIdle);

    /* 注册键盘响应函数 */
    if(onKeyboard)
        glutKeyboardFunc(onKeyboard);

    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
}
