//
// Created by jmu on 18-3-12.
//
#include <mylib/GlutWrapper.h>
#include <mylib/Shader.h>
#include <mylib/common.h>
#include <mylib/FileSys.h>
#include <vector>

#define LOG_TAG "load_file_test.cpp"
using namespace std;
struct Vertex {
    glm::vec3 vertex;
    glm::vec3 color;
};

static void load(const char *pathName, vector<Vertex> &v);


static Shader shader;
static vector<Vertex> vertices;
static GLuint VAO;

static void onPreDraw() {
    load(Resource("example.obj"), vertices);

    const GLchar *vs = R"(
#version 320 es
layout(location = 0) in highp vec3 aPos;
layout(location = 1) in highp vec3 aColor;
out vec3 Color;
void main(){
    gl_Position = vec4(aPos, 1.0);
    Color = aColor;
}
)";

    const GLchar *fs = R"(
#version 320 es
in highp vec3 Color;
out highp vec4 color;

void main(){
    color = vec4(Color, 1.0);
}
)";
    shader = Shader(vs, fs);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0].vertex.x, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, vertex));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

}

static void onDraw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    glBindVertexArray(0);

    glutSwapBuffers();

}

int main(int argc, char *argv[]) {
    GlutWrapper(onPreDraw, onDraw)
            .init(&argc, argv);
    return 0;
}

void load(const char *pathName, vector<Vertex> &v) {
    FILE *f = fopen(pathName, "r");
    if (f == NULL) {
        LOGE("open file error!")
        return;
    }

    char line[128];
    vector<glm::vec3> vertices;
    vector<glm::vec3> colors;
    struct Index {
        GLuint vi;
        GLuint ci;
    };
    vector<Index> indices;
    while (true) {
        int res = fscanf(f, "%s", line);
        if (res == EOF) {
            break;
        }

        if (line[0] == 'v') {
            //parse vertex
            glm::vec3 v;
            fscanf(f, "%f %f %f", &v.x, &v.y, &v.z);
            vertices.push_back(v);
        } else if (line[0] == 'c') {
            //parse color
            glm::vec3 c;
            fscanf(f, "%f %f %f", &c.r, &c.g, &c.b);
            colors.push_back(c);
        } else if (line[0] == 'f') {
            //parse face indices
            for (int j = 0; j < 3; ++j) {
                Index i;
                fscanf(f, "%d/%d", &i.vi, &i.ci);
                indices.push_back(i);
            }
        } else {
            //LOGE("%s", line)
        }
    }

    //remove index
    for (auto i : indices) {
        v.push_back({vertices[i.vi - 1], colors[i.ci - 1]});
    }
}