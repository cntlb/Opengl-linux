//
// Created by jmu on 18-3-7.
//

#include <mylib/common.h>
#include <mylib/Shader.h>
#include <mylib/GlutWrapper.h>
#include <mylib/camera.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mylib/camera.h>

#define LOG_TAG "2.1_light.cpp"

static GLuint VAO, lightVAO;
static Shader boxShader, lightShader;
static Camera camera;
static void onPreDraw(){
    const GLchar * box_vs = "#version 320 es\n"
            "layout(location=0) in vec3 aPos;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 view;\n"
            "uniform mat4 model;\n"
            "void main(){\n"
            "   gl_Position = projection*view*model*vec4(aPos, 1.0f);\n"
            "}";
    const GLchar* box_fs = "#version 320 es\n"
            "out highp vec4 color;\n"
            "uniform highp vec3 objectColor;\n"
            "uniform highp vec3 lightColor;\n"
            "void main(){\n"
            "   color = vec4(objectColor*lightColor, 1.0f);\n"
            "}";
    const GLchar* light_fs = "#version 320 es\n"
            "out highp vec4 color;\n"
            "void main(){\n"
            "   color = vec4(1.0f);\n"
            "}";
    boxShader = Shader(box_vs, box_fs);
    lightShader = Shader(box_vs, light_fs);

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

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    camera.Position = glm::vec3(0,2,5);
}

static void onDraw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    boxShader.use();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model, view, projection;
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600, 0.1f, 100.0f);

    boxShader.setMat4("model", glm::mat4());
    boxShader.setMat4("view", view);
    boxShader.setMat4("projection", projection);
    boxShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    boxShader.setVec3("lightColor",  0.5f, 1.0f, 0.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    lightShader.use();
    glm::mat4 M_light;
    M_light = glm::translate(M_light, glm::vec3(1, 1, -1));
    M_light = glm::scale(M_light, glm::vec3(0.2f));

    lightShader.setMat4("model", M_light);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glutSwapBuffers();

}
void onIdle(){
    onDraw();
}

void keyboard(unsigned char key, int x, int y){

    switch (key)
    {
        case 'a':
        case 'A':
            camera.ProcessKeyboard(Camera_Movement::LEFT, 1);
            break;
        case 'd':
        case 'D':
            camera.ProcessKeyboard(Camera_Movement::RIGHT, 1);
            break;
        case 'w':
        case 'W':
            camera.ProcessKeyboard(Camera_Movement::FORWARD, 1);
            break;
        case 's':
        case 'S':
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, 1);
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