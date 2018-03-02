/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>

#define Pi M_PI

void draw_vertex() {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5);//点大小
    glBegin(GL_POINTS);
    {
        glColor3f(1, 0, 0);
        glVertex2f(0, 0);
        glVertex2f(0, 0.5);
    }
    glEnd();

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xcccc);//绘制虚线
    glLineWidth(2);//线条宽度
    glBegin(GL_LINES); {
        glColor3f(0, 0, 1);
        glVertex2f(0.5, 0);
        glVertex2f(0.5, 0.5);
    } glEnd();
    glFlush();          //更新窗口
}

void draw_star(void)
{
    GLfloat a = 1 / (2-2*cos(72*Pi/180));
    GLfloat bx = a * cos(18 * Pi/180);
    GLfloat by = a * sin(18 * Pi/180);
    GLfloat cy = -a * cos(18 * Pi/180);
    GLfloat
            PointA[2] = { 0, a },
            PointB[2] = { bx, by },
            PointC[2] = { 0.5, cy },
            PointD[2] = { -0.5f, cy },
            PointE[2] = { -bx, by };

    glClear(GL_COLOR_BUFFER_BIT);
    // 按照A->C->E->B->D->A的顺序，可以一笔将五角星画出
    glBegin(GL_LINE_LOOP); {
        glVertex2fv(PointA);
        glVertex2fv(PointC);
        glVertex2fv(PointE);
        glVertex2fv(PointB);
        glVertex2fv(PointD);
    } glEnd();
    glFlush();
}

//设置正反面
void draw_face(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
    glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式
    glFrontFace(GL_CCW);               // 设置逆时针方向为正面
    glBegin(GL_POLYGON);               // 按逆时针绘制一个正方形，在左下方
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.0f);
    glEnd();
    glFlush();
}


/* 定义对键盘的响应函数 */
void keyboard(unsigned char key, int x, int y) {
    /*按Esc键退出*/
    if (27 == key) {
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
//    glutDisplayFunc(draw_vertex);
//    glutDisplayFunc(draw_star);
    glutDisplayFunc(draw_face);

    /* 注册键盘响应函数 */
    glutKeyboardFunc(keyboard);
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
    return 0;
} 
