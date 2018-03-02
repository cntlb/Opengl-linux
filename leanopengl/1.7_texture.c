/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include <GL/glut.h>
#include <math.h>
#include "mylib/myglext.h"
#include "mylib/log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LOG_TAG __FILE__
// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

void genShader(GLenum type, const GLchar ** source, GLuint*shader){
    static GLint success;
    static GLchar infolog[512];

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, source, 0);
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(*shader, sizeof(infolog), 0, infolog);
        LOGE("compile shader error[type=%d]:%s", type, infolog);
    }
}

static GLuint VAO, texture, program;
void display() {
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture);
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

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    //加载图像
    int x, y, channels_in_file;
    stbi_uc *image = stbi_load("../res/wall.jpg", &x, &y, &channels_in_file, 3);

    //创建纹理
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // (对当前绑定的纹理对象)设置纹理环绕过滤选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    const GLchar * vertexShaderSource = "#version 320 es\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "layout (location = 2) in vec2 texCoord;\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "void main(){\n"
            "   gl_Position = vec4(position, 1.0f);\n"
            "   ourColor = color;\n"
            "   TexCoord = texCoord;\n"
            "}";
    const GLchar * fragmentShaderSource = "#version 320 es\n"
            "in highp vec3 ourColor;\n"
            "in highp vec2 TexCoord;\n"
            "out highp vec4 color;\n"
            "uniform sampler2D ourTexture;\n"
            "void main()\n"
            "{\n"
            "    color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);\n"
            "}";
    GLuint vertexShader, fragmentShader;
    genShader(GL_VERTEX_SHADER, &vertexShaderSource, &vertexShader);
    genShader(GL_FRAGMENT_SHADER, &fragmentShaderSource, &fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    static GLint success;
    static GLchar infolog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, sizeof(infolog), 0, infolog);
        LOGE("link program error:%s", infolog);
    }

    GLfloat vertices[] = {
             // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(GLfloat), (const void *) ((3) * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3+3+2)*sizeof(GLfloat), (const void *) ((3 + 3) * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
} 
