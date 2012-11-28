#ifndef VEC_H
#define VEC_H

#include "tmath.h"

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iosfwd>

namespace math {

template <size_t N, typename T>
class vec {
public:
    vec()
    {}

    explicit vec(T v)
    {
        assign(v);
    }

    vec(T x, T y)
    {
        m_data[0] = x;
        m_data[1] = y;
    }

    vec(T x, T y, T z, T w)
    {
        assert(N >= 4);
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
        m_data[3] = w;
    }

    vec(T x, T y, T z)
    {
        assert(N >= 3);
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }

    vec(const vec<2, T> &src, T z)
    {
        assert(N == 3);
        m_data[0] = src[0];
        m_data[1] = src[1];
        m_data[2] = z;
    }

    vec(const vec<3, T> &src, T w)
    {
        assert(N == 4);
        m_data[0] = src[0];
        m_data[1] = src[1];
        m_data[2] = src[2];
        m_data[3] = w;
    }

    vec(const vec& src)
    {
        memcpy(m_data, src.m_data, sizeof(m_data));
    }

    explicit vec(const T* src)
    {
        memcpy(m_data, src, sizeof(m_data));
    }

    template <size_t Z, typename U>
    explicit vec(const vec<Z, U> &src)
    {
        assert(Z >= N);
        for (size_t i = 0; i < N; i++)
            m_data[i] = (T)src[i];
    }

    /// Assign the value to every component
    void assign(T v)
    {
        for (size_t i = 0; i < N; i++)
            m_data[i] = v;
    }

    T x() const { return m_data[0]; }
    T y() const { return m_data[1]; }
    T z() const { return m_data[2]; }
    T w() const { return m_data[3]; }

    T& x() { return m_data[0]; }
    T& y() { return m_data[1]; }
    T& z() { return m_data[2]; }
    T& w() { return m_data[3]; }

    T operator [](size_t i) const
    {
        return m_data[i];
    }

    T& operator [](size_t i)
    {
        return m_data[i];
    }

    /// Access raw vector data
    const T* data() const
    {
        return m_data;
    }

    T* data()
    {
        return m_data;
    }

    ///
    T lengthSquared() const
    {
        return dot(*this, *this);
    }

    /// Length (euclidian norm) of the vector
    T length() const
    {
        return sqrtf(lengthSquared());
    }

    /// Get normalized vector from this vector
    vec normalized() const
    {
        return *this / length();
    }

    /// Normalize this vector
    vec& normalize()
    {
        *this /= length();
        return *this;
    }

    T manhattanNorm() const
    {
        // TODO: use generic abs
        return abs(x())+abs(y());
    }

    /// Unary minus (invert vector)
    vec operator - () const
    {
        vec r;
        for (size_t i = 0; i < N; i++)
            r.m_data[i] = -m_data[i];
        return r;
    }

#define VEC_OP(op, on)                          \
    vec operator op (const vec &b) const        \
    {                                           \
        vec r(*this);                           \
        r op##= b;                              \
        return r;                               \
    }                                           \
    void operator op##= (const vec &b)          \
    {                                           \
        for (size_t i = 0; i < N; i++)          \
            m_data[i] op##= b.m_data[i];        \
    }

#define SCALAR_OP(op, on)                       \
    vec operator op (T b) const             \
    {                                           \
        vec r(*this);                           \
        r op##= b;                              \
        return r;                               \
    }                                           \
    void operator op##= (T b)               \
    {                                           \
        for (size_t i = 0; i < N; i++)          \
            m_data[i] op##= b;                  \
    }

    /// Vector addition
    VEC_OP(+, add)
    /// Vector substraction
    VEC_OP(-, sub)
    /// Per-component multiplication
    VEC_OP(*, mul)
    /// Per-component devision
    VEC_OP(/, div)
    /// Scalar multiplication
    SCALAR_OP(*, mul)
    /// Scalar division
    SCALAR_OP(/, div)

    vec map(T (*func)(T)) const
    {
        vec res;
        for (size_t i = 0; i < N; i++)
            res[i] = func(m_data[i]);
        return res;
    }

    bool operator == (const vec &b) const
    {
        for (size_t i = 0; i < N; i++)
            if (m_data[i] != b.m_data[i])
                return false;
        return true;
    }

    bool operator != (const vec &b) const
    {
        return !(*this == b);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        for (size_t i = 0; i < N; i++)
            ar & m_data[i];
    }

    void clamp(vec a, vec b)
    {
        for (size_t i = 0; i < N; i++)
            m_data[i] = math::clamp(m_data[i], a[i], b[i]);
    }

private:
    T m_data[N];
};

#undef SCALAR_OP
#undef VEC_OP

typedef vec<3, float> vec3f;
typedef vec<4, float> vec4f;
typedef vec<2, int> vec2i;
typedef vec<2, float> vec2f;
typedef vec<2, double> vec2d;

inline vec3f vec3(const vec4f &src)
{
    return vec3f(src.x(), src.y(), src.z());
}

/// Dot product
template <size_t N, typename T>
inline float dot(const vec<N, T> &a, const vec<N, T> &b)
{
    vec<N, T> p = a*b;
    float sum = 0;
    for (size_t i = 0; i < N; i++)
        sum += p[i];
    return sum;
}

/// Cross product
inline vec3f cross(const vec3f &a, const vec3f &b)
{
    return vec3f(a[1]*b[2]-a[2]*b[1],
                 a[2]*b[0]-a[0]*b[2],
                 a[0]*b[1]-a[1]*b[0]);
}

/// Distance between two vectors
template <size_t N, typename T>
inline float distance(const vec<N, T> &a, const vec<N, T> &b)
{
    return (a-b).length();
}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream &out, const vec<N, T> &v);

struct Ray {
    vec3f origin;
    vec3f dir;
};

std::ostream& operator<<(std::ostream &out, const Ray &ray);

}; // namespace math

#endif
