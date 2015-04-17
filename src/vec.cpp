#include "vec.hpp"

math::vec3 math::cross(const vec3& u, const vec3& v) {
    return vec3{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]};
}
