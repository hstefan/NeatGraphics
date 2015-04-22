#pragma once

#include <string>
#include "GL/gl3w.h"

namespace gl {

struct Shader {
    Shader(GLenum shaderType, const std::string& contents);
    Shader(Shader&& other);
    ~Shader();
    operator GLuint() const;

    bool compile();

   private:
    GLuint id;
};

}  // namespace gl
