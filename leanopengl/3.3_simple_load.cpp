//
// Created by jmu on 18-3-12.
//
#include <glm/glm.hpp>
#include <vector>
#include <stdio.h>
#include <mylib/log.h>
#include <mylib/GlutWrapper.h>
#include <mylib/camera.h>
#include <mylib/Shader.h>
#include <sstream>
#include <mylib/TextureUtil.h>

#define LOG_TAG "3.3_simple_load.cpp"
using namespace std;

struct Vertex;
struct Index;
struct GLData {
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> texCoords;
    vector<Index> indices;
};

struct Vertex {
    glm::vec3 v;
    glm::vec2 t;
    glm::vec3 n;
};

struct Index {
    GLuint v;
    GLuint t;
    GLuint n;
};
static GLData data;


static bool loadObj(const char *, GLData &);
static vector<Vertex> rmIndices(GLData &);

static GLuint VAO;
static GLuint texture1;
static Shader shader;
static Camera camera;

static void onPreDraw(){
    // https://github.com/wangdingqiao/noteForOpenGL/blob/master/resources/models/cube/cube.obj
    loadObj("/home/jmu/desktop/untitled.obj", data);
    const vector<Vertex> &vertices = rmIndices(data);

    glEnable(GL_DEPTH_TEST);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0].v, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, v));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, t));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, n));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    const GLchar * vs = R"(
#version 320 es
layout(location = 0) in highp vec3 aPos;
layout(location = 1) in highp vec2 aTexCoord;
layout(location = 2) in highp vec3 aNormal;

uniform highp mat4 model;
uniform highp mat4 view;
uniform highp mat4 projection;

out highp vec2 TexCoord;

void main(){
    gl_Position = projection*view*model*vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

)";
    const GLchar * fs = R"(
#version 320 es
out highp vec4 color;
uniform sampler2D texture1;
in highp vec2 TexCoord;
void main(){
    color = texture(texture1, TexCoord);
}
)";
    shader = Shader(vs, fs);

    shader.use();
    // dds纹理加载
    // https://github.com/wangdingqiao/noteForOpenGL/blob/master/modelLoading/simpleObjLoader/texture.h
    // https://www.cnblogs.com/crsky/p/6691611.html
    texture1 = TextureUtil::load("container2.png");
    shader.setInt("texture1", 0);
    shader.unUse();
}

static void onDraw(){
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE0, texture1);

    glm::mat4 model, view, projection;
    glm::scale(model, glm::vec3(0.5));
    projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600, 0.1f, 100.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.indices.size());
    glBindVertexArray(0);
    glutSwapBuffers();
}

void onIdle(){
//    time_elapse = glutGet(GLUT_ELAPSED_TIME);
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


int main(int argc, char *argv[]) {
    GlutWrapper wrapper = GlutWrapper(onPreDraw, onDraw, onIdle);
    wrapper.init2(&argc, argv);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    wrapper.startLoop();
    return 0;
}



bool loadObj(const char *pathName, GLData &data) {
    FILE *file = fopen(pathName, "r");
    if (!file) {
        LOGE("open file:%s error", pathName);
        return false;
    }
    char line[128];
    string start;
    while (true) {
        int res = fscanf(file, "%s", line);
        if (res == EOF) {
            break;
        }

        istringstream iss(line);
        start = line;
        if (start == "vn") {
            glm::vec3 vn;
            fscanf(file, "%f %f %f", &vn.x, &vn.y, &vn.z);
            data.normals.push_back(vn);
        } else if (start == "vt") {
            glm::vec2 vt;
            fscanf(file, "%f %f", &vt.x, &vt.y);
            data.texCoords.push_back(vt);
        } else if (start == "v") {
            glm::vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            data.vertices.push_back(v);
        } else if (start == "f") {
#define SET(idx) &idx.v, &idx.t, &idx.n
            Index index[3];
            // f v/t/n
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", SET(index[0]), SET(index[1]), SET(index[2]));
            data.indices.push_back(index[0]);
            data.indices.push_back(index[1]);
            data.indices.push_back(index[2]);
        }
    }

    fclose(file);
    return true;
}

vector<Vertex> rmIndices(GLData &data){
    vector<Vertex> res;
    for(auto i : data.indices){
        Vertex v; 
        v.v = data.vertices[i.v-1];
        v.t = data.texCoords[i.t-1];
        v.n = data.normals[i.n-1];
        res.push_back(v);
    }
    return res;
}
