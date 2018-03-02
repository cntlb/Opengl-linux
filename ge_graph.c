/* ge_graph.c
 * 此程序利用GLUT绘制一个OpenGL窗口，并显示一个加以光照的球。
 * 由于头文件glut.h中已经包含了头文件gl.h和glu.h，所以只需要include 此文件
 * 编译:
 *  gcc ge_graph.c -lGL -lglut -lGLU -lm
 */
#include <GL/glut.h>
#include <math.h>

/*调用GLUT函数，绘制一个球*/
void draw_vertex() {
    glClearColor(1, 1, 1, 1);                     //设置刷新背景色
    glClear(GL_COLOR_BUFFER_BIT);              //刷新背景
    glBegin(GL_LINES);                //画个十字充当坐标系
        glColor3f(0, 0, 0);          //设置当前颜色
        glVertex3f(-1, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(0, -1, 0);
        glVertex3f(0, 1, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);         //画个y=sin(4x) 的函数图像
        glColor3f(1, 0, 0);
        for (float x = -1.00f; x <= 1.00; x += 0.01)
            glVertex2f(x, (GLfloat) sin(x * 4 * M_PI));
    glEnd();

    glBegin(GL_LINE_LOOP);          //画个蓝色的单位圆　
        glColor3f(0, 0, 1);
        for (float x = -1.00f; x <= 1.00; x += 0.01)
            glVertex2f(x, (GLfloat) sqrt(1 - x * x));
        for (float x = 1.00; x >= -1.00; x -= 0.01)
            glVertex2f(x, (GLfloat) -sqrt(1 - x * x));
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
