#pragma once

#include "mat.hpp"
#include <cmath>

namespace math {

typedef mat<2, 1> vec2;
typedef mat<3, 1> vec3;
typedef mat<4, 1> vec4;
typedef mat<2, 1, double> vecd2;
typedef mat<3, 1, double> vecd3;
typedef mat<4, 1, double> vecd4;

template <class T, unsigned D>
T norm(const mat<D, 1, T>& m) {
	T acc(0);
	for (int i = 0; i < D; ++i) {
		acc += m(i, 0) * m(i, 0);
	}
	return std::sqrt(acc);
}

template <class T, unsigned D>
mat<D, 1, T> normalize(const mat<D, 1, T>& v) {
	return T(1) / norm(v) * v;
}

vec3 cross(const vec3& u, const vec3& v);

} //namespace math
