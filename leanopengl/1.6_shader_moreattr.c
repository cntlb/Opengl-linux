/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>
#include "mylib/myglext.h"
#include "mylib/log.h"
#define LOG_TAG "1.6_shader_moreattr.c"
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//链接程序
static GLuint program;
//顶点缓冲, 索引, 数组对象
static GLuint VBO, EBO, VAO;
void display() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
            "layout(location = 1) in vec3 vertex_color;\n"
            "out highp vec3 out_color;\n"
            "void main(){\n"
            "   gl_Position = vec4(vertex_position, 1.0f);\n"
            "   out_color = vertex_color;\n"
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
            "in highp vec3 out_color;\n"
            "void main(){\n"
            "   color = vec4(out_color, 1.0f);\n"
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

    //顶点数据, todo 添加了颜色信息
    GLfloat vertexes[] = {
            //vertex          //color
            0.5f,  0.5f,  0,  1.0f, 0.0f, 0.0f, //rt red
            -0.5f, 0.5f,  0,  0.0f, 1.0f, 0.0f, //lt green
            -0.5f, -0.5f, 0,  0.0f, 0.0f, 1.0f, //lb blue
            0.5f,  -0.5f, 0,  1.0f, 1.0f, 0.0f, //rb yellow
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

    //vertex_position (location=0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    //vertex_color (location=1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
} 
