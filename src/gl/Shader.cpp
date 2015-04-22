#include <array>
#include "gl/Shader.hpp"
#include "gl/GLDebug.hpp"

using gl::Shader;
using gl::dbg::checkErrors;

Shader::Shader(GLenum shaderType, const std::string& contents) : id(0) {
    id = glCreateShader(shaderType);
    const auto shaderContent = contents.c_str();
    glShaderSource(id, 1, &shaderContent, nullptr);
    checkErrors();
    compile();
}

Shader::Shader(Shader&& other) : id(other.id) { other.id = 0; }

Shader::~Shader() {
    glDeleteShader(id);
    checkErrors();
}

bool Shader::compile() {
    glCompileShader(id);
    auto status = GLint{-1};
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        auto MsgBuffer = std::array<char, 512>();
        glGetShaderInfoLog(id, MsgBuffer.size(), nullptr, MsgBuffer.data());
        std::cerr << MsgBuffer.data() << std::endl;
    }
    return status = GL_TRUE;
}

Shader::operator GLuint() const { return id; }
