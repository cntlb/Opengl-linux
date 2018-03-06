//
// Created by jmu on 18-3-2.
//

/*
 * 物体变换坐标:
 *          model               view                           projection            view port
 * 局部空间 --------> 世界空间 -------------> 视觉空间(观察空间) --------------> 裁切空间 --------> 屏幕空间
 *
 * 局部空间
 *      也称为物体空间, 模型中的所有顶点与物体的原点都是相对的. 即使最终绘制的不是在屏幕上的同一个位置,仍然可以指定所有的物体都有一个共同的原点
 *
 * 世界空间
 *      世界坐标中的顶点相对与世界原点. 物体的所有原点都设为(0,0,0)而绘制时却不一定都聚集在世界原点, 而是和世界原点有一个相对的位置,
 *      存在一个变换, 这个变换就是局部原点到世界原点的变换, 记作 M_model
 *
 * 观察空间
 *      就是OpenGL的摄像机, 也称作摄像机空间. 观察空间用于将世界坐标变换为观察者眼前所见的2D坐标, 这个变换记作 M_view
 *
 * 裁切空间
 *      使用正交变换或者投影变换处理物体的远近效果, 将超出范围的顶点裁切掉不显示, 变换记作: M_proj
 *
 * 于是从局部坐标到裁切坐标的变换:
 *      V_clip = M_proj * M_view * M_model * V_local
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
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    proj = glm::perspective(glm::radians(45.0f), wrapper.getWidth() / (float) wrapper.getHigh(), 0.1f, 100.0f);
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    glBindVertexArray(VAO);
    for(unsigned int i = 0; i < 10; i++) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float radian = 0;
        if(i%3 == 0){
            radian = time_escaps*glm::radians(20.0f * i);
        }
        if(i == 0){
            radian = time_escaps;
        }
        model = glm::rotate(model,  radian, glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glutSwapBuffers();
}

void onIdle(){
    time_escaps += 0.001;
    onDraw();
}

int main(int argc, char* argv[]){
    wrapper = GlutWrapper(onPreDraw, onDraw, onIdle);
    wrapper.init(&argc, argv);
    return 0;
}