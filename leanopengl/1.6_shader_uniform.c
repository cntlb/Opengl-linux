/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>
#include "mylib/myglext.h"
#include "mylib/log.h"
#define LOG_TAG "1.5_triangle_exercise.c"
// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

//链接程序
static GLuint program;
//顶点缓冲, 索引, 数组对象
static GLuint VBO, EBO, VAO;
GLfloat time = 0;
void display() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    //todo 设置uniform的值
    GLint location = glGetUniformLocation(program, "color3f");
    glUseProgram(program);
    glUniform3f(location, 0, (GLfloat) (cos(time) / 2 + 0.5f), (GLfloat) (sin(time) / 2 + 0.5f));
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glutSwapBuffers();
}


/* 定义对键盘的响应函数 */
void keyboard(unsigned char key, int x, int y) {
    /*按Esc键退出*/
    if(27 == key){
        exit(0);
    }
}

void idle(){
    time += 0.001f;
    display();
}

__attribute__((destructor)) void onDestroy(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    int success;
    GLchar logInfo[512];

    //顶点着色器源码
    const GLchar * vertexShaderSource = "#version 320 es\n"
            "layout(location = 0) in vec3 vertex_position;\n"
            "void main(){\n"
            "   gl_Position = vec4(vertex_position, 1.0f);\n"
            "}\n";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, sizeof(logInfo), 0, logInfo);
        LOGE("compile vertex shader error:%s", logInfo)
    }else{
        LOGE("compile vertex shader OK!")
    }

    //像素着色器
    const GLchar * fragmentShaderSource = "#version 320 es\n"
            "out highp vec4 color;\n"
            "uniform highp vec3 color3f;\n"//todo uniform变量
            "void main(){\n"
            "   color = vec4(color3f, 1.0f);\n"
            "}\n";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, sizeof(logInfo), 0, logInfo);
        LOGE("compile fragment shader error:%s", logInfo)
    }else{
        LOGE("compile fragment shader OK!")
    }
    
    //着色器程序
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, sizeof(logInfo), 0, logInfo);
        LOGE("link program error:%s", logInfo)
    }else{
        LOGE("link program OK!")
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //顶点数据
    GLfloat vertexes[] = {
            0.5f, 0.5f, 0,  //rt
            -0.5f, 0.5f, 0, //lt
            -0.5f, -0.5f, 0,//lb
            0.5f, -0.5f, 0, //rb
    };
    //索引数组
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
} 
