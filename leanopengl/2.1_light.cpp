//
// Created by jmu on 18-3-7.
//

#include <mylib/common.h>
#include <mylib/Shader.h>
#include <mylib/GlutWrapper.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define LOG_TAG __LINE__

static GLuint VAO, lightVAO;
static Shader boxShader, lightShader;

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
            "void main(){\n"
            "   color = vec4(1.0f, 0.5f, 0.31f, 1.0f);\n"
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

}

static void onDraw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    boxShader.use();
    glm::mat4 model, view, projection;
    view = glm::lookAt(glm::vec3(0, 3, -3.0f),
        glm::vec3(0,0,0),
        glm::vec3(1,0,0)
    );
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600, 0.1f, 100.0f);
    boxShader.setMat4("model", model);
    boxShader.setMat4("view", view);
    boxShader.setMat4("projection", projection);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    lightShader.use();
    glm::mat4 M_light;
    M_light = glm::translate(M_light, glm::vec3(1, 1, -1));
    lightShader.setMat4("model", M_light);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glutSwapBuffers();

}

int main(int argc, char* argv[]){
    GlutWrapper(onPreDraw, onDraw)
            .init(&argc, argv);
    return 0;
}