#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <initializer_list>
#include <algorithm>
#include <assert.h>

// An MXN matrix has M Rows and N Columns
// Matrix is stored row-based: element N-1 is the last element of the first row.
template <std::size_t M, std::size_t N>
class Matrix {
    protected:
        std::array<float, M*N> arr;
    public:
        Matrix()
        {
            this->arr = {0};
        }

        Matrix(const std::array<float, M*N>& arr) : arr{arr} {}
        // Brace-enclosed initializer list constructor
        Matrix(std::initializer_list<float> l)
        {
            assert(l.size() == arr.size());
            std::copy(l.begin(), l.end(), arr.data());
        }

        bool operator==(const Matrix<M, N> &other) const
        {
            return this->arr == other.arr;
        }

        bool operator!=(const Matrix<M, N>& other) const
        {
            return !this->operator==(other);
        }

        Matrix<M, N>& operator*=(const float& f)
        {
            for (auto it = this->arr.begin(); it != this->arr.end(); ++it) {
                *it *= f;
            }
            return *this;
        }

        Matrix<M, N>& operator/=(const float& f)
        {
            for (auto it = this->arr.begin(); it != this->arr.end(); ++it) {
                *it /= f;
            }
            return *this;
        }

        float operator()(const std::size_t &m, const std::size_t &n) const
        {
            assert(m < M);
            assert(n < N);
            return this->arr[m*N + n];
        }

        float& operator()(const std::size_t &m, const std::size_t &n)
        {
            assert(m < M);
            assert(n < N);
            return this->arr[m*N + n];
        }

};

template <std::size_t M, std::size_t N>
Matrix<N,M> operator*(Matrix<M, N> mat, const float& f)
{
    return mat *= f;
}

template <std::size_t M, std::size_t N>
Matrix<N,M> operator/(Matrix<M, N> mat, const float& f)
{
    return mat /= f;
}

template <std::size_t K, std::size_t M, std::size_t N>
Matrix<M,K> operator*(const Matrix<M, N>& lhs, const Matrix<N, K>& rhs)
{
    Matrix<M, K> ret;
    for (std::size_t m = 0; m < M; ++m) {
        for (std::size_t k = 0; k < K; ++k) {
            for (std::size_t n = 0; n < N; ++n) {
                ret(m,k) += lhs(m,n) * rhs(n,k);
            }
        }
    }
    return ret;
}
#endif //MATRIX_HPP
