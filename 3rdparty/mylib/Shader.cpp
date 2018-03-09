//
// Created by jmu on 18-3-2.
//

#include "Shader.h"
#include "common.h"

#define LOG_TAG "Shader.cpp"

Shader::Shader(const std::string vertex_path, const std::string fragment_path) {
}

Shader::Shader(const GLchar *vertex_src, const GLchar *fragment_src) {
    GLuint vertexShader, fragmentShader;
    GLchar log[1024];
    GLint success;

    auto compile_shader = [&](GLenum type, const GLchar *source) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(log), 0, log);
            LOGE("%s\n"
                 "========================\n"
                 "compile %s shader error:\n%s",
                 source, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", log)
        }
        return shader;
    };

    vertexShader = compile_shader(GL_VERTEX_SHADER, vertex_src);
    fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(log), 0, log);
        LOGE("link proram error:%s", log)
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    if (program)
        glUseProgram(program);
}

void Shader::unUse(){
    glUseProgram(0);
}

GLint Shader::getLocation(const std::string &name)const{
    return glGetUniformLocation(program, name.c_str());
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z)const{
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &v)const{
    glUniform3f(glGetUniformLocation(program, name.c_str()), v.x, v.y, v.z);
}

GLuint Shader::getProgram() const {
    return program;
}
