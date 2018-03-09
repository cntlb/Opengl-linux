//
// Created by jmu on 18-3-9.
//

#ifndef INC_2_4_TEXTURE_TEXTURE_H
#define INC_2_4_TEXTURE_TEXTURE_H

#include "common.h"
#define LOG_TAG "TextureUtil.h"

class TextureUtil{
public:
    static GLuint load(const char* path, void (*texParamFunc)()=TextureUtil::texFunc){
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int w,h,channels;
        stbi_uc* image = stbi_load(path, &w, &h, &channels, 0);
        GLint format;
        if(channels == 1){
            format = GL_RED;
        }else if(channels == 3){
            format = GL_RGB;
        }else if(channels == 4){
            format = GL_RGBA;
        }

        if(image){
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            texParamFunc();
            stbi_image_free(image);
        }else{
            LOGE("%s", "load texture data error!");
            stbi_image_free(image);
        }
        return texture;
    }

private:
    static void texFunc(){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
};

#endif //INC_2_4_TEXTURE_TEXTURE_H
