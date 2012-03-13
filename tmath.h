#ifndef TMATH_H
#define TMATH_H

#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

template <typename T>
T lerp(const T &a, const T &b, float t)
{
    return a+(b-a)*t;
}

inline float rad(float deg)
{
    return deg*(M_PI/180.0f);
}

#endif
