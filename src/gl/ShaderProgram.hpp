#pragma once

#include "GL/gl3w.h"
#include <array>

namespace gl {

struct ShaderProgram {
    ShaderProgram();
    ShaderProgram(ShaderProgram&& other);
    ~ShaderProgram();
    //allows us to convert cast implicitly or explicity to GLuint
    operator GLuint() const;

    void bindFragDataLocation(GLuint colorNumber, const char* name);
    void attachShader(GLuint shader);
    bool link();
    void use() const;
    GLuint getUniformLocation(const char* name) const;
   private:
    GLuint id;
    bool linked;
};

}  // namespace gl
