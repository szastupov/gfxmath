#ifndef ROTATION_H
#define ROTATION_H

#include "matrix.h"
#include "quaternion.h"

Matrix4f rotateX(float angle);
Matrix4f rotateY(float angle);
Matrix4f rotateZ(float angle);

Matrix3f rotate2D(float a);

inline Matrix4f rotateAxis(float angle, const vec3f &axis)
{
    return Quaternion::fromAngleAxis(angle, axis).toMatrix();
}

inline Matrix4f rotateTo(const vec3f &dir)
{
    return Quaternion::fromDirection(dir).toMatrix();
}

#endif
