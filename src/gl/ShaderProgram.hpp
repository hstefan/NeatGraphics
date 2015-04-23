#pragma once

#include <array>
#include "GL/gl3w.h"
#include "util/NonCopyable.hpp"

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
    NONCOPYABLE(ShaderProgram);
    GLuint id;
    bool linked;
};

}  // namespace gl
