//
// Created by jmu on 18-3-2.
//

#ifndef INC_1_8_GLM_SHADER_H
#define INC_1_8_GLM_SHADER_H

#include <string>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint program;
public:
    Shader(){};
    Shader(const std::string vertex_src, const std::string fragment_src);
    Shader(const GLchar *vertex_src, const GLchar *fragment_src);

    GLuint getProgram() const;

    /*
     * use program
     * don't forget to activate/use the shader before setting uniforms!
     */
    void use();
    void unUse();
    GLint getLocation(const std::string &name)const;
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

};

#endif //INC_1_8_GLM_SHADER_H
