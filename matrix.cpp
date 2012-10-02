#include <iomanip>
#include <iostream>
#include <cstring>

#include "matrix.h"

namespace math {

template <size_t N, typename T>
Matrix<N, T>::Matrix(const T *data)
{
    memcpy(m_data, data, sizeof(m_data));
}

template <size_t N, typename T>
void Matrix<N, T>::transpose()
{
    for (size_t j = 0; j < N-1; j++)
        for (size_t i = j+1; i < N; i++)
            std::swap(m_data[j][i], m_data[i][j]);
}

template <size_t N, typename T>
Matrix<N, T> Matrix<N, T>::transposed() const
{
    Matrix<N, T> ret(*this);
    ret.transpose();
    return ret;
}

template <size_t N, typename T>
T Matrix<N, T>::detMinor(int ox, int oy) const
{
    int x1 = ox % N;
    int y1 = oy % N;
    int x2 = (ox+1) % N;
    int y2 = (oy+1) % N;

    return (m_data[y1][x1] * m_data[y2][x2])
        -  (m_data[y1][x2] * m_data[y2][x1]);
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::loadZero()
{
    for (size_t j = 0; j < N; j++)
        for (size_t i = 0; i < N; i++)
            m_data[j][i] = 0;
    return *this;
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::loadIdentity()
{
    for (size_t j = 0; j < N; j++) {
        for (size_t i = 0; i < N; i++)
            m_data[j][i] = 0;
        m_data[j][j] = 1;
    }
    return *this;
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::setScale(T sx, T sy)
{
    m_data[0][0] = sx;
    m_data[1][1] = sy;
    return *this;
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::setTranslate(T tx, T ty)
{
    m_data[N-1][0] = tx;
    m_data[N-1][1] = ty;
    return  *this;
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::setScale(T sx, T sy, T sz)
{
    m_data[0][0] = sx;
    m_data[1][1] = sy;
    m_data[2][2] = sz;
    return *this;
}

template <size_t N, typename T>
Matrix<N, T>& Matrix<N, T>::setTranslate(T tx, T ty, T tz)
{
    m_data[N-1][0] = tx;
    m_data[N-1][1] = ty;
    m_data[N-1][2] = tz;
    return  *this;
}

template <size_t N, typename T>
bool Matrix<N, T>::isIdentinty() const
{
    for (size_t j = 0; j < N; j++) {
        vec<N, T> c(T(0));
        c[j] = 1;
        if (memcmp(m_data[j], c.data(), sizeof(m_data[j])) != 0)
            return false;
    }
    return true;
}

template <size_t N, typename T>
bool Matrix<N, T>::isScale() const
{
    for (size_t j = 0; j < N; j++) {
        vec<N, T> c(T(0));
        c[j] = m_data[j][j];
        if (memcmp(m_data[j], c.data(), sizeof(m_data[j])) != 0)
            return false;
    }
    return true;
}

template <size_t N, typename T>
bool Matrix<N, T>::operator == (const Matrix<N, T> &m) const
{
    for (size_t j = 0; j < N; j++)
        for (size_t i = 0; i < N; i++)
            if (m_data[j][i] != m.m_data[j][i])
                return false;
    return true;
}

template <size_t N, typename T>
Matrix<N, T> Matrix<N, T>::operator * (T s) const
{
    Matrix<N, T> ret;

    for (size_t j = 0; j < N; j++)
        for (size_t i = 0; i < N; i++)
            ret.m_data[j][i] = m_data[j][i]*s;

    return ret;
}

template <size_t N, typename T>
void mvmult(T dest[N], const T mat[N][N], const T vec[N])
{
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            dest[i] += mat[j][i]*vec[j];
}

#ifdef __SSE__
#include <xmmintrin.h>
template <>
void mvmult<4, float>(float dest[4], const float mat[4][4], const float vec[4])
{
    __m128 d;
    d = _mm_setzero_ps();
    for (size_t j = 0; j < 4; j++)
        d = _mm_add_ps(d, _mm_mul_ps(_mm_loadu_ps(mat[j]),
                                     _mm_set_ps1(vec[j])));
    _mm_storeu_ps(dest, d);
}
#endif // __SSE

template <size_t N, typename T>
Matrix<N, T> Matrix<N, T>::operator * (const Matrix<N, T> &m) const
{
    Matrix<N, T> ret;
    ret.loadZero();

    for (size_t j = 0; j < N; j++)
        mvmult<N, T>(ret.m_data[j], m_data, m[j]);

    return ret;
}

template <size_t N, typename T>
vec<N, T> Matrix<N, T>::operator * (const vec<N, T> &v) const
{
    vec<N, T> res(T(0));
    mvmult<N, T>(res.data(), m_data, v.data());
    return res;
}

template <size_t N, class T>
std::ostream &operator<<(std::ostream &out, const Matrix<N, T> &m)
{
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++)
            out << std::left << std::setw(5) << m[j][i] << " ";
        out << std::endl;
    }
    return out;
}

template class Matrix<2, float>;
template class Matrix<3, float>;
template class Matrix<4, float>;

template
std::ostream &operator<< <2, float>(std::ostream &out, const Matrix<2, float> &m);
template
std::ostream &operator<< <3, float>(std::ostream &out, const Matrix<3, float> &m);
template
std::ostream &operator<< <4, float>(std::ostream &out, const Matrix<4, float> &m);

template <>
float Matrix3f::det() const
{
    return m_data[0][0]*detMinor(1, 1)
        +  m_data[1][0]*detMinor(2, 1)
        +  m_data[2][0]*detMinor(0, 1);
}

template <>
float Matrix2f::det() const
{
    return detMinor(0, 0);
}

template <>
Matrix3f Matrix3f::inverse() const
{
    Matrix3f minors;

    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            minors.m_data[j][i] = detMinor(i+1, j+1);
    minors.transpose();

    return minors*(1.0f/det());
}

template <>
Matrix3f Matrix3f::inverseTransposed() const
{
    Matrix3f mat = inverse();
    mat.transpose();
    return mat;
}

}; // namespace math
