/*
 * 此程序利用GLUT绘制一个OpenGL窗口模板
 */
#include "mylib/common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LOG_TAG __FILE__
// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

static void genShader(GLenum type, const GLchar ** source, GLuint*shader){
    static GLint success;
    static GLchar infolog[512];

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, source, 0);
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(*shader, sizeof(infolog), 0, infolog);
        LOGE("compile shader error:\n%s", infolog);
    }
}

static GLuint VAO, texture1, texture2, program;
static void display() {
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    // don't forget to activate/use the shader before setting uniforms!
    glUseProgram(program);

    //active textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(program, "texture2"), 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glutSwapBuffers();
}


/* 定义对键盘的响应函数 */
static void keyboard(unsigned char key, int x, int y) {
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
    stbi_uc *image;

    //创建纹理
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    //texture1
    glBindTexture(GL_TEXTURE_2D, texture1);
    image = stbi_load("../res/wall.jpg", &x, &y, &channels_in_file, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    //texture2
    glBindTexture(GL_TEXTURE_2D, texture2);
    image = stbi_load("../res/face.png", &x, &y, &channels_in_file, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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
            //TODO 此处改为1-texCoord.y报错:cannot construct `vec2' from a non-numeric data type
            //可能和glsl版本有关
            "   TexCoord = vec2(2.0f*texCoord.x, 2.0f*(1.0f-texCoord.y));\n"
            "}";
    const GLchar * fragmentShaderSource = "#version 320 es\n"
            "in highp vec3 ourColor;\n"
            "in highp vec2 TexCoord;\n"
            "out highp vec4 color;\n"
            "uniform sampler2D texture1;\n"
            "uniform sampler2D texture2;\n"
            "void main()\n"
            "{\n"
            "    color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
            "    //color = texture(texture2, TexCoord);\n"
            "}";
    GLuint vertexShader, fragmentShader;
    genShader(GL_VERTEX_SHADER, &vertexShaderSource, &vertexShader);
    genShader(GL_FRAGMENT_SHADER, &fragmentShaderSource, &fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    static GLint success;
    static GLchar infolog[1024];
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
