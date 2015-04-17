#pragma once

#include <array>
#include <ostream>

namespace math {

template <unsigned M, unsigned N = M, class T = float>
struct mat;

// handy typedefs
typedef mat<2U> mat2;
typedef mat<3U> mat3;
typedef mat<4U> mat4;
typedef mat<2U, 2U, double> matd2;
typedef mat<3U, 3U, double> matd3;
typedef mat<4U, 4U, double> matd4;

// utility functions
template <unsigned M, unsigned N = M, class T = float>
mat<N, M, T> transpose(const mat<M, N, T>& m);

template <unsigned M, unsigned N, class T>
struct mat {
    // static definitions
    typedef T value_type;
    typedef std::array<T, N * M> container;
    static const auto width = N;
    static const auto height = M;

    // constructors
    template <class... E>
    mat(E&&... d);

    mat();

    // access operator
    value_type& operator()(unsigned i, unsigned j);
    const value_type& operator()(unsigned i, unsigned j) const;
    // raw-indexed operator
    value_type& operator[](unsigned e);
    const value_type& operator[](unsigned e) const;

    decltype(auto) data();

   private:
    container _data;
};

template <unsigned M, unsigned N, class T>
template <class... E>
mat<M, N, T>::mat(E&&... d)
    : _data{std::forward<E>(d)...} {}

template <unsigned M, unsigned N, class T>
mat<M, N, T>::mat() {}

template <unsigned M, unsigned N, class T>
typename mat<M, N, T>::value_type& mat<M, N, T>::operator()(unsigned i,
                                                            unsigned j) {
    return _data[i * N + j];
}

template <unsigned M, unsigned N, class T>
const typename mat<M, N, T>::value_type& mat<M, N, T>::operator()(
    unsigned i, unsigned j) const {
    return _data[i * N + j];
}

template <unsigned M, unsigned N, class T>
typename mat<M, N, T>::value_type& mat<M, N, T>::operator[](unsigned e) {
    return _data[e];
}

template <unsigned M, unsigned N, class T>
const typename mat<M, N, T>::value_type& mat<M, N, T>::operator[](
    unsigned e) const {
    return _data[e];
}

template <unsigned M, unsigned N, class T>
decltype(auto) mat<M, N, T>::data() {
    return _data.data();
}

template <unsigned M, unsigned N, class T>
mat<M, N, T> operator+(const mat<M, N, T>& lhs, const mat<M, N, T>& rhs) {
    mat<M, N, T> r;
    for (unsigned i = 0; i < N * M; ++i) {
        r[i] = lhs[i] + rhs[i];
    }
    return r;
}

template <unsigned M, unsigned N, class T>
mat<M, N, T> operator-(const mat<M, N, T>& lhs, const mat<M, N, T>& rhs) {
    mat<M, N, T> r;
    for (auto i = 0U; i < N * M; ++i) {
        r[i] = lhs[i] - rhs[i];
    }
    return r;
}

template <unsigned M, unsigned N, class T>
mat<M, N, T> operator*(const mat<M, N, T>& m, float scalar) {
    mat<M, N, T> r;
    for (auto i = 0U; i < N * M; ++i) {
        r[i] = m[i] * scalar;
    }
    return r;
}

template <unsigned M, unsigned N, class T>
mat<M, N, T> operator*(float scalar, const mat<M, N, T>& m) {
    return m * scalar;
}

template <unsigned M, unsigned N, unsigned P, class T>
mat<N, P, T> operator*(const mat<M, N, T>& lhs, const mat<N, P, T>& rhs) {
    mat<N, P, T> r;
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

template <unsigned M, unsigned N, class T>
mat<N, M, T> transpose(const mat<M, N, T>& m) {
    mat<N, M, T> r;
    for (auto i = 0U; i < N; ++i) {
        for (auto j = 0U; j < M; ++j) {
            r(i, j) = m(j, i);
        }
    }
    return r;
}

template <unsigned M, unsigned N, class T>
std::ostream& operator<<(std::ostream& stream, const mat<M, N, T>& mat) {
    stream << "mat<" << M << ',' << N << ">: " << std::endl;
    for (auto i = 0U; i < M; ++i) {
        for (auto j = 0U; j < N; ++j) {
            stream << mat(i, j) << ' ';
        }
        stream << std::endl;
    }
    return stream;
}

}  // namespace math
