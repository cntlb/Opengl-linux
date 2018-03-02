/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>
#include "mylib/log.h"
#include "mylib/myglext.h"

#define LOG_TAG "triangle2.c"
const GLuint WIDTH = 800, HEIGHT = 600;
/*
 * opengl图形输送管线处理顶点数据:
 * 顶点 --> 顶点着色器 --> 基本图形组装 --> 几何着色器  --> 细分着色器 --> 栅格化 --> 像素着色器 --> 测试混合
 *
 * 顶点着色器    以单独的顶点作为输入, 主要目的是将3D坐标(笛卡尔坐标)转换为另一种3D坐标(透视坐标), 同事设置基本的顶点属性
 *              坐标转换通过顶点着色器小程序处理
 *              顶点属性通过 glVertexAttribPointer
 *
 * 顶点缓存对象(Vertex Buffer Object, VBO)允许将顶点数据直接拷贝到显存上, 渲染时直接从显存中读取而无需再从其他地方拷贝到显存.
 * 使用glVertexPointer/glNormalPointer将数据放到内存中, 渲染是需要拷贝到显存, 会消耗不少时间.
 * 创建VBO步骤:
 *      1. 创建VBO对象: void glGenBuffers(GLsizei n, GLuint *buffers)
 *      2. 绑定VBO: void glBindBuffer(GLenum target, GLuint buffer)
 *          target - 缓存对象要存储的数据类型，只有两个值： GL_ARRAY_BUFFER, 和 GL_ELEMENT_ARRAY_BUFFER。
 *          如果是顶点的相关属性，例如： 顶点坐标、纹理坐标、法线向量、颜色数组等，要使用 GL_ARRAY_BUFFER；
 *          索引数组，要使用 GL_ELEMENT_ARRAY_BUFFER，以便 glDrawElements() 使用。
 *      3. 绑定顶点数据: void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
 */

const GLchar* vertexShaderSource = "#version 320 es\n"
        "layout (location = 0) in vec3 position;\n"
        "out vec3 pos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "    pos = position;\n"
        "}";

const GLchar* fragmentShaderSource = "#version 320 es\n"
        "in highp vec3 pos;\n"
        "out highp vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4(pos, 1.0f);\n"
        "}";

//顶点缓冲对象
static GLuint VAO;
static GLuint program;

void display() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glViewport(0, 0, WIDTH, HEIGHT);

    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOGE("vertex shader compile failed:%s", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOGE("fragment shader compile failed:%s", infoLog);
    }

    //链接程序
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOGE("glLinkProgram failed:%s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

// Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, 1,2,3,// Left
            0.5f, -0.5f, 0.0f,  1,2,3,// Right
            0.0f,  0.5f, 0.0f,  1,2,3,// Top
    };
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    /* 注册OpenGL绘图函数 */
    glutDisplayFunc(display);

    /* 注册键盘响应函数 */
    glutKeyboardFunc(keyboard);
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop();
    return 0;
} 
