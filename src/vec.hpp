#pragma once

#include "mat.hpp"
#include <cmath>

namespace math {

typedef mat<float, 2, 1> vec2;
typedef mat<float, 3, 1> vec3;
typedef mat<float, 4, 1> vec4;
typedef mat<double, 2, 1> vecd2;
typedef mat<double, 3, 1> vecd3;
typedef mat<double, 4, 1> vecd4;

template <class T, unsigned D>
T norm(const mat<T, D, 1>& m) {
	T acc(0);
	for (int i = 0; i < D; ++i) {
		acc += m(i, 0) * m(i, 0);
	}
	return std::sqrt(acc);
}

template <class T, unsigned D>
mat<T, D, 1> normalize(const mat<T, D, 1>& v) {
	return T(1) / norm(v) * v;
}

vec3 cross(const vec3& v, const vec3& w);

}
