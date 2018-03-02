/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>

#define Pi M_PI

//为同一多边形的不同顶点指定不同的颜色
void draw_vertex() {
    glClearColor(0.5, 0.5,0,0.5);
    int i;
    glShadeModel(GL_FLAT);
//    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for(i=0; i<=8; ++i)
    {
        glColor3f(i&0x04, i&0x02, i&0x01);
        glVertex2f((GLfloat) cos(i * Pi / 4), (GLfloat) sin(i * Pi / 4));
    }
    glEnd();
    glFlush();          //更新窗口
}


/* 定义对键盘的响应函数 */
void keyboard(unsigned char key, int x, int y) {
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
    glutInitWindowSize(300, 300);
    /* 定义窗口位置 */
    glutInitWindowPosition(100, 100);
    /* 显示窗口，窗口标题为执行函数名 */
    glutCreateWindow(argv[0]);

    /* 注册OpenGL绘图函数 */
    glutDisplayFunc(draw_vertex);

    /* 注册键盘响应函数 */
    glutKeyboardFunc(keyboard);
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
    return 0;
} 
