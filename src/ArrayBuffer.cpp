#include "ArrayBuffer.hpp"

using namespace cg;

ArrayBuffer::ArrayBuffer() 
	: id(GL_FALSE) {	
	glGenBuffers(1, &id);
}

ArrayBuffer::~ArrayBuffer() {
	glDeleteBuffers(1, &id);
}

void ArrayBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

bool ArrayBuffer::bound() {
	auto boundId = GLint { -1 };
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundId);
	return static_cast<GLint>(id) == boundId;
}
