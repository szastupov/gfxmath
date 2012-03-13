#ifndef MATRIX_H
#define MATRIX_H

#include "vec.h"

template <size_t N, typename T>
class Matrix {
public:
    Matrix() {}

    explicit Matrix(const T *data);

    template <size_t NN>
    explicit Matrix(const Matrix<NN, T> &src)
    {
        assert(N < NN);
        for (size_t j = 0; j < N; j++)
            memcpy(m_data[j], src[j], sizeof(m_data[j]));
    }

    const T* operator [] (int j) const
    {
        return m_data[j];
    }

    T* operator [] (int j)
    {
        return m_data[j];
    }

    Matrix operator * (T s) const;

    Matrix operator * (const Matrix &m) const;

    void operator *= (const Matrix &m)
    {
        *this = m * (*this);
    }

    vec<N, T> operator * (const vec<N, T> &v) const;

    bool operator == (const Matrix &m) const;

    const T* data() const
    {
        return m_data[0];
    }

    T det() const;

    T detMinor(int ox, int oy) const;

    Matrix inverse() const;

    void transpose();
    Matrix transposed() const;

    Matrix inverseTransposed() const;

    Matrix& loadZero();

    Matrix& loadIdentity();

    Matrix& setScale(T sx, T sy);
    Matrix& setTranslate(T tx, T ty);
    Matrix& setScale(T sx, T sy, T sz);
    Matrix& setTranslate(T tx, T ty, T tz);

    bool isIdentinty() const;
    bool isScale() const;
private:
    T m_data[N][N];
};

template <size_t N, class T>
std::ostream &operator<<(std::ostream &out, const Matrix<N, T> &m);

typedef Matrix<2, float> Matrix2f;
typedef Matrix<3, float> Matrix3f;
typedef Matrix<4, float> Matrix4f;

/////

inline Matrix4f identity()
{
    Matrix4f m;
    m.loadIdentity();
    return m;
}

inline Matrix4f scale(float sx, float sy, float sz)
{
    return identity().setScale(sx, sy, sz);
}

inline Matrix4f scale(const vec3f &s)
{
    return scale(s[0], s[1], s[2]);
}

inline Matrix4f translate(float tx, float ty, float tz)
{
    return identity().setTranslate(tx, ty, tz);
}

inline Matrix4f translate(const vec3f &s)
{
    return translate(s[0], s[1], s[2]);
}

#endif
