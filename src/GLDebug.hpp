#pragma once

#include "GL/gl3w.h"
#include <iostream>

namespace cg {
namespace dbg {

inline void checkErrors(std::ostream& output = std::cerr) {
#ifndef NDEBUG
	auto err = GLenum { GL_NO_ERROR };
	while ((err = glGetError()) != GL_NO_ERROR) {
		output << "OpenGL error: " << err << std::endl;
	}
#endif
}

} //namespace dbg
} //namespace cg
