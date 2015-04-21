#include <algorithm>
#include <cassert>
#include <iostream>
#include "ShaderProgram.hpp"
#include "GLDebug.hpp"

using cg::ShaderProgram;

ShaderProgram::ShaderProgram() : id(0), linked(false) {
    id = glCreateProgram();
}

ShaderProgram::ShaderProgram(ShaderProgram&& other)
    : id(other.id), linked(other.linked) {
    other.id = 0;
    other.linked = 0;
}

ShaderProgram::~ShaderProgram() {
  if (id != 0) {
    glDeleteProgram(id);
  }
}

ShaderProgram::operator GLuint() const {
    return id;
}

void ShaderProgram::bindFragDataLocation(GLuint colorNumber, const char* name) {
    glBindFragDataLocation(id, colorNumber, name);
    dbg::checkErrors();
}

void ShaderProgram::attachShader(GLuint shader) {
    glAttachShader(id, shader);
    dbg::checkErrors();
}

bool ShaderProgram::link() {
    glLinkProgram(id);

    auto status = GLint{GL_FALSE};
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        auto msgBuffer = std::array<char, 512>();
        glGetProgramInfoLog(id, msgBuffer.size(), nullptr, msgBuffer.data());
        std::cerr << msgBuffer.data() << std::endl;
    }
    linked = status == GL_TRUE;

    return linked;
}

void ShaderProgram::use() const {
    assert(linked && "Attempt to glUseProgram an unlinked ShaderProgram!");
    glUseProgram(id);
}

GLuint ShaderProgram::getUniformLocation(const char* name) const {
    return glGetUniformLocation(id, name);
}
