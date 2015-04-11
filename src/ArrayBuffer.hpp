 #pragma once

#include <array>
#include "GL/gl3w.h"

namespace cg {

struct ArrayBuffer {
	ArrayBuffer();
	template <class T>
	ArrayBuffer(const T& data);

	virtual ~ArrayBuffer();

	void bind();
	bool bound();
	//we'll probably need something more flexible for this later on
	template <class T>
	void bufferData(const T& data);
private:
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
}

} //namespace cg
