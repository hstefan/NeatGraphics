#pragma once

#include <array>
#include <ostream>

namespace math {

template <class T, unsigned M, unsigned N>
struct mat;

//handy typedefs
typedef mat<float, 2U, 2U> mat2;
typedef mat<float, 3U, 3U> mat3;
typedef mat<float, 4U, 4U> mat4;
typedef mat<double, 2U, 2U> matd2;
typedef mat<double, 3U, 3U> matd3;
typedef mat<double, 4U, 4U> matd4;

//overloaded operators
template<class T, unsigned M, unsigned N>
mat<T, M, N> operator+(const mat<T, M, N>& lhs, const mat<T, M, N>& rhs);

template<class T, unsigned M, unsigned N>
mat<T, M, N> operator-(const mat<T, M, N>& lhs, const mat<T, M, N>& rhs);

template<class T, unsigned M, unsigned N>
mat<T, M, N> operator*(const mat<T, M, N>& m, float scalar);

template<class T, unsigned M, unsigned N>
mat<T, M, N> operator*(float scalar, const mat<T, N, M>& m);

template<class T, unsigned M, unsigned N, unsigned P>
mat<T, N, P> operator*(const mat<T, M, N>& lhs, const mat<T, N, P>& rhs);

template<class T, unsigned M, unsigned N>
std::ostream& operator<<(std::ostream& stream, const mat<T, M, N>& mat);

//utility functions
template<class T, unsigned M, unsigned N>
mat<T, N, M> transpose(const mat<T, M, N>& m);

template <class T, unsigned M, unsigned N>
struct mat {
	//static definitions
	typedef T value_type;
	typedef std::array<T, N * M> container;
	static const auto width = N;
	static const auto height = M;

	//constructors
	template <class ...E>
	mat(E&&... d);

	mat();

	//access operator
	value_type& operator() (unsigned i, unsigned j);
	const value_type& operator() (unsigned i, unsigned j) const;
	//raw-indexed operator
	value_type& operator[] (unsigned e);
	const value_type& operator[] (unsigned e) const;

	decltype(auto) data();
private:
	container _data;
};

template <class T, unsigned M, unsigned N>
template <class ...E>
mat<T, M, N>::mat(E&&... d)
	: _data{ std::forward<E>(d)... } {
}

template <class T, unsigned M, unsigned N>
	mat<T, M, N>::mat() {
}

template <class T, unsigned M, unsigned N>
typename mat<T, M, N>::value_type& mat<T, M, N>::operator() (unsigned i, unsigned j) {
	return _data[i * N + j];
}


template <class T, unsigned M, unsigned N>
const typename mat<T, M, N>::value_type& mat<T, M, N>::operator() (unsigned i, unsigned j) const {
	return _data[i * N + j];
}


template <class T, unsigned M, unsigned N>
typename mat<T, M, N>::value_type& mat<T, M, N>::operator[] (unsigned e) {
	return _data[e];
}


template <class T, unsigned M, unsigned N>
const typename mat<T, M, N>::value_type& mat<T, M, N>::operator[] (unsigned e) const {
	return _data[e];
}

template <class T, unsigned M, unsigned N>
decltype(auto) mat<T, M, N>::data() { return _data.data(); }

template<class T, unsigned M, unsigned N>
mat<T, M, N> operator+(const mat<T, M, N>& lhs, const mat<T, M, N>& rhs) {
    mat<T, N, M> r;
    for (unsigned i = 0; i < N * M; ++i) {
	r[i] = lhs[i] + rhs[i];
    }
    return r;
}

template<class T, unsigned M, unsigned N>
mat<T, N, M> operator-(const mat<T, N, M>& lhs, const mat<T, N, M>& rhs) {
    mat<T, N, M> r;
    for (auto i = 0U; i < N * M; ++i) {
	r[i] = lhs[i] - rhs[i];
    }
    return r;
}

template<class T, unsigned M, unsigned N>
mat<T, N, M> operator*(const mat<T, N, M>& m, float scalar) {
    mat<T, N, M> r;
    for (auto i = 0U; i < N * M; ++i) {
	r[i] = m[i] * scalar;
    }
    return r;
}

template<class T, unsigned M, unsigned N>
mat<T, N, M> operator*(float scalar, const mat<T, N, M>& m) {
    return m * scalar;
}

template<class T, unsigned M, unsigned N, unsigned P>
mat<T, N, P> operator*(const mat<T, M, N>& lhs, const mat<T, N, P>& rhs) {
    mat<T, N, P> r;
    for (auto i = 0U; i < M; ++i) {
	for (auto j = 0U; j < P; ++j) {
	    r(i, j) = 0;
	    for (auto k = 0; k < N; ++k) {
		r(i, j) += lhs(i, k) * rhs(k, j);
	    }
	}
    }
    return r;
}

template<class T, unsigned M, unsigned N>
mat<T, N, M> transpose(const mat<T, M, N>& m) {
    mat<T, N, M> r;

    for (auto i = 0U; i < N; ++i) {
	for (auto j = 0U; j < M; ++j) {
	    r(i, j) = m(j, i);
	}
    }

    return r;
}

template<class T, unsigned M, unsigned N>
std::ostream& operator<<(std::ostream& stream, const mat<T, M, N>& mat) {
    stream << "mat<" << M << ',' <<  N << ">: " << std::endl;
    for (auto i = 0U; i < M; ++i) {
	for (auto j = 0U; j < N; ++j) {
	    stream << mat(i, j) << ' ';
	}
	stream << std::endl;
    }
    return stream;
}

}
