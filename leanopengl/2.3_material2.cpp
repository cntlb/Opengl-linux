//
// Created by jmu on 18-3-7.
// 光照 -- 材质, 增加光照控制
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

#define LOG_TAG "2.3_material.cpp"

static GLuint cubeVAO, lightVAO;
static Shader boxShader, lightShader;
static Camera camera;
static int time_elapse;
// lighting
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
static void onPreDraw(){
    const GLchar * box_vs = R"(
#version 320 es
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec3 Normal;
out vec3 FragPos;
void main(){
   gl_Position = projection*view*model*vec4(aPos, 1.0f);
   Normal = mat3(transpose(inverse(model)))*aNormal;
   FragPos = vec3(model*vec4(aPos, 1.0f));
}
    )";
    const GLchar *box_fs = R"(
#version 320 es

uniform struct{
    highp vec3 ambient;
    highp vec3 diffuse;
    highp vec3 specular;
    highp float shininess;
} material;

uniform struct{
    highp vec3 ambient;
    highp vec3 diffuse;
    highp vec3 specular;
    highp vec3 position;
} light;

out highp vec4 FragColor;

in highp vec3 Normal;
in highp vec3 FragPos;

uniform highp vec3 viewPos;

void main()
{
   // ambient
   highp vec3 ambient = material.ambient * light.ambient;          //环境光(光源颜色x环境光因子)

   // diffuse
   highp vec3 norm = normalize(Normal);                             //规范化法向量
   highp vec3 lightDir = normalize(light.position - FragPos);       //计算光的投射方向(光源-物体顶点)
   highp float diff = max(dot(norm, lightDir), 0.0);                //计算点乘, 结果代表反射光的强度
   highp vec3 diffuse = material.diffuse * diff * light.diffuse;    //计算反射光

   //specular                                                       //镜面光
   highp vec3 viewDir = normalize(viewPos-FragPos);                 //视线向量
   highp vec3 reflectDir = reflect(-lightDir, norm);                //反射光向量
   highp float spec = pow(max(dot(viewDir, reflectDir), 0.0),
                material.shininess);                                //计算镜面光光强
   highp vec3 specular = material.specular * spec * light.specular; //光照之镜面光分量

   //light result
   highp vec3 result = ambient + diffuse + specular;  //环境光+反射光
   FragColor = vec4(result, 1.0);
}
    )";
    boxShader = Shader(box_vs, box_fs);

    const GLchar* light_vs = R"(
        #version 320 es
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
           gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    const GLchar* light_fs = R"(
        #version 320 es
        out highp vec4 color;
        uniform highp vec3 lightColor;
        void main(){
           color = vec4(lightColor,1.0f);
        }
    )";
    lightShader = Shader(light_vs, light_fs);



    float vertices[] = {
            //顶点                 //顶点法向量
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glEnable(GL_DEPTH_TEST);

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //顶点
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    //顶点法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    camera.Position = glm::vec3(2,2,5);
}

static void onDraw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    boxShader.use();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model, view, projection;
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600, 0.1f, 100.0f);
//    model = glm::rotate(model, glm::radians(time_elapse*0.1f), glm::vec3(1,0,0));

    boxShader.setMat4("model", model);
    boxShader.setMat4("view", view);
    boxShader.setMat4("projection", projection);

    boxShader.setVec3("light.position", lightPos);
    boxShader.setVec3("viewPos", camera.Position);

    //光源颜色
    glm::vec3 lightColor;
    lightColor.x = sin(0.001f*time_elapse * 2.0f);
    lightColor.y = sin(0.001f*time_elapse * 0.7f);
    lightColor.z = sin(0.001f*time_elapse * 1.3f);
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    boxShader.setVec3("light.ambient", ambientColor);
    boxShader.setVec3("light.diffuse", diffuseColor);
    boxShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    boxShader.setVec3("material.ambient",  1.0f, 0.5f, 0.31f);
    boxShader.setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
    boxShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    boxShader.setFloat("material.shininess", 32.0f);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    lightShader.use();
    glm::mat4 M_light;
    M_light = glm::translate(M_light, lightPos);
    M_light = glm::scale(M_light, glm::vec3(0.2f));

    lightShader.setMat4("model", M_light);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    lightShader.setVec3("lightColor", lightColor);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

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