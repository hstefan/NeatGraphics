#include "gl/ArrayBuffer.hpp"

using gl::ArrayBuffer;

ArrayBuffer::ArrayBuffer() : id(GL_FALSE) {
    glGenBuffers(1, &id);
    dbg::checkErrors();
}

ArrayBuffer::~ArrayBuffer() {
    glDeleteBuffers(1, &id);
    dbg::checkErrors();
}

void ArrayBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    dbg::checkErrors();
}

bool ArrayBuffer::bound() {
    auto boundId = GLint{-1};
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundId);
    dbg::checkErrors();
    return static_cast<GLint>(id) == boundId;
}
