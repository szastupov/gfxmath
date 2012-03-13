#include <iostream>
#include "quaternion.h"

std::ostream &operator<<(std::ostream &out, const Quaternion &q)
{
    out << "["
        << q.m_w << ", "
        << q.m_v
        << "]";

    return out;
}


Matrix4f Quaternion::toMatrix() const
{
    Matrix4f m;
    m.loadIdentity();

    vec3f v2 = m_v+m_v;
    vec3f vv = m_v*v2;
    vec3f v2w = v2*m_w;

    float x = m_v.x();
    float y = m_v.y();
    float xy = x * v2.y();
    float xz = x * v2.z();
    float yz = y * v2.z();

    float xx = vv.x();
    float yy = vv.y();
    float zz = vv.z();
    float wx = v2w.x();
    float wy = v2w.y();
    float wz = v2w.z();

    m[0][0]=1.0f-(yy+zz); m[1][0]=xy-wz;        m[2][0]=xz+wy;
    m[0][1]=xy+wz;        m[1][1]=1.0f-(xx+zz); m[2][1]=yz-wx;
    m[0][2]=xz-wy;        m[1][2]=yz+wx;        m[2][2]=1.0f-(xx+yy);

    return m;
}

vec3f Quaternion::toEuler() const
{
    float sqw = m_w*m_w;
    vec3f sqv = m_v*m_v;

    float z = atan2f(2.f * (m_v.x()*m_v.y() + m_v.z()*m_w), sqv.x() - sqv.y() - sqv.z() + sqw);
    float y = asinf(-2.f * (m_v.x()*m_v.z() - m_v.y()*m_w));
    float x = atan2f(2.f * (m_v.y()*m_v.z() + m_v.x()*m_w), -sqv.x() - sqv.y() + sqv.z() + sqw);

    return vec3f(x, y, z);
}

void Quaternion::toAngleAxis(float &angle, vec3f &axis) const
{
    float a = acosf(m_w);
    axis = m_v*(1.0f/sinf(a));
    angle = a*2;
}

Quaternion Quaternion::fromAngleAxis(float angle, const vec3f &axis)
{
    float a = angle*0.5f;
    return Quaternion(cosf(a), axis.normalized()*sinf(a));
}

Quaternion Quaternion::fromEuler(float rx, float ry, float rz)
{
    return fromEuler(vec3f(rx, ry, rz));
}

Quaternion Quaternion::fromEuler(const vec3f &rv)
{
    vec3f half = rv*0.5f;

    float cos_z_2 = cosf(half.z());
    float cos_y_2 = cosf(half.y());
    float cos_x_2 = cosf(half.x());

    float sin_z_2 = sinf(half.z());
    float sin_y_2 = sinf(half.y());
    float sin_x_2 = sinf(half.x());

    float w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
    float x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
    float y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
    float z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

    return Quaternion(w, vec3f(x, y, z));
}

Quaternion Quaternion::fromDirection(const vec3f &dir)
{
    static vec3f xaxis(1.0f, 0.0f, 0.0f);
    float angle = acosf(dot(xaxis, dir));
    vec3f axis = cross(xaxis, dir);
    return fromAngleAxis(angle, axis);
}

void Quaternion::loadIdentity()
{
    m_v.assign(0.0f);
    m_w = 1.0f;
}
