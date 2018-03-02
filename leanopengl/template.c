/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include "mylib/common.h"
#define LOG_TAG __FILE__
// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

static void display() {

    glFlush();          //更新窗口
}


/* 定义对键盘的响应函数 */
static void keyboard(unsigned char key, int x, int y) {
    /*按Esc键退出*/
    if(27 == key){
        exit(0);
    }
}

int main(int argc, char **argv) {
    /* GLUT环境初始化*/
    glutInit(&argc, argv);
    /* 显示模式初始化 */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    /* 定义窗口大小 */
    glutInitWindowSize(WIDTH, HEIGHT);
    /* 定义窗口位置 */
    glutInitWindowPosition(100, 100);
    /* 显示窗口，窗口标题为执行函数名 */
    glutCreateWindow(argv[0]);

    /* 注册OpenGL绘图函数 */
    glutDisplayFunc(display);

    /* 注册键盘响应函数 */
    glutKeyboardFunc(keyboard);
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
    return 0;
} 
