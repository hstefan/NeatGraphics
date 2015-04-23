#pragma once

#include <array>
#include "GL/gl3w.h"
#include "gl/GLDebug.hpp"
#include "util/NonCopyable.hpp"

namespace gl {

struct ArrayBuffer {
    ArrayBuffer();
    ArrayBuffer(ArrayBuffer&& other);
    template <class T>
    ArrayBuffer(const T& data);

    virtual ~ArrayBuffer();
    operator GLuint() const;

    void bind();
    bool bound();
    // we'll probably need something more flexible for this later on
    template <class T>
    void bufferData(const T& data);
   private:
    NONCOPYABLE(ArrayBuffer);
    GLuint id;
};

template <class T>
ArrayBuffer::ArrayBuffer(const T& data)
    : ArrayBuffer() {
    bufferData(data);
}

template <class T>
void ArrayBuffer::bufferData(const T& data) {
    if (!bound()) {
        bind();
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);
    dbg::checkErrors();
}

}  // namespace gl
