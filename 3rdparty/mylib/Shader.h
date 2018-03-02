//
// Created by jmu on 18-3-2.
//

#ifndef INC_1_8_GLM_SHADER_H
#define INC_1_8_GLM_SHADER_H

#include <string>
#include <GL/glut.h>

class Shader {
private:
    GLuint program;
public:
    Shader(const std::string vertex_src, const std::string fragment_src);
    Shader(const GLchar *vertex_src, const GLchar *fragment_src);

    GLuint getProgram() const;

    /*
     * use program
     * don't forget to activate/use the shader before setting uniforms!
     */
    void use();
    void setUniform(const char* name, int value);
    void setUniform(const char* name, float value);
};

#endif //INC_1_8_GLM_SHADER_H
