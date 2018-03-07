//
// Created by jmu on 18-3-2.
//

/*
 * 通过键盘控制相机
 *
 */
#include <mylib/common.h>
#include <mylib/Shader.h>
#include <mylib/GlutWrapper.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define LOG_TAG __FILE__

static Shader *shader;
static GlutWrapper wrapper;

static GLuint VAO, texture1, texture2;
static float deltaX = 0, deltaZ = 0;
static float fov = 45.0f;
static const float D = 1.0f;
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static float pitch = 0, yaw = 0;

static glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

static void onPreDraw(){
    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glEnable(GL_DEPTH_TEST);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    const GLchar* vs_src = "#version 320 es\n"
            "layout(location = 0) in vec3 pos;\n"
            "layout(location = 1) in vec2 aTexture;\n"
            "out vec2 oTextureCoor;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 proj;\n"
            "void main(){\n"
            "   gl_Position = proj*view*model*vec4(pos, 1.0);\n"
            "   oTextureCoor = vec2(aTexture.x, 1.0-aTexture.y);\n"
            "}\n";
    const GLchar* fs_src = "#version 320 es\n"
            "out highp vec4 color;\n"
            "in highp vec2 oTextureCoor;\n"
            "uniform sampler2D texture1;\n"
            "uniform sampler2D texture2;\n"
            "void main(){\n"
            "   color = mix(texture(texture2, oTextureCoor),texture(texture1, oTextureCoor),0.2);\n"
            "   //color = texture(texture2, oTextureCoor);\n"
            "}\n";
    shader = new Shader(vs_src, fs_src);
    
    int width, high,channels;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_uc *image = stbi_load("../res/face.png", &width, &high, &channels, 0);
    if(!image){
        LOGE("load face.png texture error")
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, high, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = stbi_load("../res/wall.jpg", &width, &high, &channels, 0);
    if(!image){
        LOGE("load wall.jpg texture error")
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, high, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    shader->unUse();
}

static float time_escaps = 0;

static void onDraw(){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 view, proj;
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    float radius = 10.0f;
    float x = radius*sin(time_escaps);
    float z = radius*cos(time_escaps);
    glm::vec3 eye_pos = glm::vec3(x, 0, z) + glm::vec3(deltaX, 0, deltaZ);

    glm::vec3 front , up;
    front.y = sin(glm::radians(pitch));
    front.x = -cos(glm::radians(pitch))*cos(glm::radians(yaw));
    front.z = -cos(glm::radians(pitch))*sin(glm::radians(yaw));
    front = glm::normalize(front);

    glm::vec3 right = glm::cross(front, glm::vec3(0,1,0));
    up = glm::cross(right, front);
    up = glm::normalize(up);
    eye_pos = glm::vec3(deltaX, 0, deltaZ);
    view = glm::lookAt(eye_pos,
                       eye_pos + front,
                       up
    );
    proj = glm::perspective(glm::radians(fov), wrapper.getWidth() / (float) wrapper.getHigh(), 0.1f, 100.0f);
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    glBindVertexArray(VAO);
    for(unsigned int i = 0; i < 10; i++) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model,  glm::radians(20.0f*i), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glutSwapBuffers();
}

void onIdle(){
//    time_escaps += 0.001;
    onDraw();
}

void keyboard(unsigned char key, int x, int y){

    switch (key)
    {
        case 'a':
        case 'A':
            deltaX -= D;
            break;
        case 'd':
        case 'D':
            deltaX += D;
            break;
        case 'w':
        case 'W':
            deltaZ -= D;
            break;
        case 's':
        case 'S':
            deltaZ += D;
            break;
        case 27:
            exit(0);
            break;
    }
}

static int downX, downY, downKey;
static bool mouseClick = false;

void scroll_callback(double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov += yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void  Mouse( int i1, int status, int x, int y){
    LOGE("i1=%d, status=%d, x=%d, y=%d", i1,status,x,y);
    if(status == GLUT_DOWN){
        mouseClick = true;
        downKey = i1;
        downX = x;
        downY = y;
    }else{
        mouseClick = false;
    }

    if(i1 == 3){
        LOGE("roll up")
        scroll_callback(1);
    }else if(i1 == 4){
        LOGE("roll down")
        scroll_callback(-1);
    }
}

void Motion(int x, int y){
    LOGE("x=%d, y=%d", x, y)
    if(!mouseClick){
        return;
    }

    if(downKey == GLUT_LEFT_BUTTON){
        // 计算俯仰角
        pitch += 0.1*(y-downY);
        // 计算偏航角
        yaw += 0.1*(x-downX);
    }
}

int main(int argc, char* argv[]){
    wrapper = GlutWrapper(onPreDraw, onDraw, onIdle);
    wrapper.onKeyboard = keyboard;
    wrapper.init2(&argc, argv);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    wrapper.startLoop();
    return 0;
}