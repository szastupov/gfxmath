#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec.h"
#include "matrix.h"

struct Quaternion {
    vec3f m_v;
    float m_w;

    Quaternion()
        : m_v(0.0f)
        , m_w(1.0f)
    {}

    Quaternion(float w, const vec3f &v)
        : m_v(v)
        , m_w(w)
    {
    }

    Quaternion operator + (const Quaternion &b) const
    {
        return Quaternion(m_w+b.m_w, m_v+b.m_v);
    }

    Quaternion operator - (const Quaternion &b) const
    {
        return Quaternion(m_w-b.m_w, m_v-b.m_v);
    }

    Quaternion operator * (const Quaternion &b) const
    {
        return Quaternion(m_w*b.m_w - dot(m_v, b.m_v),
                          b.m_v*m_w + m_v*b.m_w + cross(m_v, b.m_v));
    }

    Quaternion operator * (float s) const
    {
        return Quaternion(m_w*s, m_v*s);
    }

    Quaternion operator - () const
    {
        return Quaternion(-m_w, m_v);
    }

    bool operator == (const Quaternion &b) const
    {
        return m_w == b.m_w && m_v == b.m_v;
    }

    bool operator != (const Quaternion &b) const
    {
        return m_w != b.m_w || m_v != b.m_v;
    }

    float lengthSquared() const
    {
        return m_v.lengthSquared()+m_w*m_w;
    }

    float length() const
    {
        return sqrtf(lengthSquared());
    }

    // To rotation matrix
    Matrix4f toMatrix() const;

    // To vector of euler angles (degrees)
    vec3f toEuler() const;

    void toAngleAxis(float &angle, vec3f &axis) const;

    static Quaternion fromAngleAxis(float angle, const vec3f &axis);

    static Quaternion fromEuler(float rx, float ry, float rz);

    // From vector of euler angles
    static Quaternion fromEuler(const vec3f &v);

    static Quaternion fromDirection(const vec3f &dir);

    void loadIdentity();
};

std::ostream &operator<<(std::ostream &out, const Quaternion &q);

#endif
