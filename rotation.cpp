#include "rotation.h"

namespace math {

Matrix4f rotateX(float a)
{
    float ca = cosf(a);
    float sa = sinf(a);
    Matrix4f m;
    m.loadIdentity();

    m[0][0] = 1.0f;  m[1][0] = 0.0f;  m[2][0] = 0.0f;
    m[0][1] = 0.0f;  m[1][1] = ca;    m[2][1] = -sa;
    m[0][2] = 0.0f;  m[1][2] = sa;    m[2][2] = ca;

    return m;
}

Matrix4f rotateY(float a)
{
    float ca = cosf(a);
    float sa = sinf(a);
    Matrix4f m;
    m.loadIdentity();

    m[0][0] = ca;    m[1][0] = 0.0f;  m[2][0] = sa;
    m[0][1] = 0.0f;  m[1][1] = 1.0f;  m[2][1] = 0.0f;
    m[0][2] = -sa;   m[1][2] = 0.0f;  m[2][2] = ca;

    return m;
}

Matrix4f rotateZ(float a)
{
    float ca = cosf(a);
    float sa = sinf(a);
    Matrix4f m;
    m.loadIdentity();

    m[0][0] = ca;    m[1][0] = -sa;   m[2][0] = 0.0f;
    m[0][1] = sa;    m[1][1] = ca;    m[2][1] = 0.0f;
    m[0][2] = 0.0f;  m[1][2] = 0.0f;  m[2][2] = 1.0f;

    return m;
}

Matrix3f rotate2D(float a)
{
    float ca = cosf(a);
    float sa = sinf(a);
    Matrix3f m;
    m.loadIdentity();

    m[0][0] = ca;    m[1][0] = -sa;
    m[0][1] = sa;    m[1][1] = ca;

    return m;
}

}; // namespace math