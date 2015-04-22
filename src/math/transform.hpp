#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"

namespace math {

mat4 translate(float tx, float ty, float tz) {
    // clang-format off
    return mat4 {
        0.0f, 0.0f, 0.0f, tx,
        0.0f, 0.0f, 0.0f, ty,
        0.0f, 0.0f, 0.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
}

mat4 scale(float sx, float sy, float sz) {
    // clang-format off
    return mat4 {
        sx,   0.0f, 0.0f, 0.0f,
        0.0f, sy,   0.0f, 0.0f,
        0.0f, 0.0f, sz,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
}

mat4 identity() {
    return scale(1.0f, 1.0f, 1.0f);
}

//TODO: couldn't we try some "explode" thing with variadic templates?
mat4 translate(vec3 t) { return translate(t[0], t[1], t[2]); }

mat4 scale(vec3 s) { return scale(s[0], s[1], s[2]); }

} //namespace math
