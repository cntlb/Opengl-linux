//
// Created by jmu on 18-3-7.
// 光照 -- 放射光纹理
//

#include <mylib/common.h>
#include <mylib/Shader.h>
#include <mylib/GlutWrapper.h>
#include <mylib/camera.h>
#include <mylib/TextureUtil.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mylib/camera.h>

#include <model/Mesh.h>
#include <model/model.h>


#define LOG_TAG "3.3_model_loading.cpp"

static GLuint cubeVAO, lightVAO;
static GLuint texture_diff, texture_spec, texture_emission;
static Shader shader;
static Camera camera;
static int time_elapse;
// lighting
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
static Model *ourModel;

unsigned int loadTexture(char const * path);

static void onPreDraw(){
    glEnable(GL_DEPTH_TEST);

    const GLchar * vs = R"(
#version 320 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";
    const GLchar * fs = R"(
#version 320 es
out highp vec4 FragColor;

in highp vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}
)";
    shader = Shader(vs, fs);

    // load models
    // -----------
    //https://learnopengl-cn.github.io/data/nanosuit.rar
    ourModel = new Model("/home/jmu/Desktop/nanosuit/nanosuit.obj");
}

static void onDraw(){
    // render
    // ------
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    shader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800.0f / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::rotate(model, glm::radians(time_elapse*0.02f), glm::vec3(0,1,0));
    model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);
    ourModel->Draw(shader);

    glutSwapBuffers();
}
void onIdle(){
    time_elapse = glutGet(GLUT_ELAPSED_TIME);
    onDraw();
}

#define Dx 0.2
void keyboard(unsigned char key, int x, int y){

    switch (key)
    {
        case 'a':
        case 'A':
            camera.ProcessKeyboard(Camera_Movement::LEFT, Dx);
            break;
        case 'd':
        case 'D':
            camera.ProcessKeyboard(Camera_Movement::RIGHT, Dx);
            break;
        case 'w':
        case 'W':
            camera.ProcessKeyboard(Camera_Movement::FORWARD, Dx);
            break;
        case 's':
        case 'S':
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, Dx);
            break;
        case 27:
            exit(0);
            break;
    }
}

static int downX, downY, downKey;
static bool mouseClick = false;

void  Mouse( int i1, int status, int x, int y){
//    LOGE("i1=%d, status=%d, x=%d, y=%d", i1,status,x,y);
    if(status == GLUT_DOWN){
        mouseClick = true;
        downKey = i1;
        downX = x;
        downY = y;
    }else{
        mouseClick = false;
    }

    if(i1 == 3){
        camera.ProcessMouseScroll(1);
    }else if(i1 == 4){
        camera.ProcessMouseScroll(-1);
    }
}

void Motion(int x, int y){
//    LOGE("x=%d, y=%d", x, y)
    if(!mouseClick){
        return;
    }

    if(downKey == GLUT_LEFT_BUTTON){
        //屏幕像素点的y轴从上到下增加
        camera.ProcessMouseMovement(x - downX, downY-y);
        downX = x;
        downY = y;
    }
}

int main(int argc, char* argv[]){
    GlutWrapper wrapper = GlutWrapper(onPreDraw, onDraw, onIdle);
    wrapper.init2(&argc, argv);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    wrapper.startLoop();
    return 0;
}