#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "plane.h"
#include "matrix.h"
#include "quaternion.h"

namespace math {

typedef enum { OUTSIDE = 0, INSIDE = 1, INTERSECT = 2 } intersection_t;

class Frustum {
public:
    Frustum();

    void set(float fov, float aspectRatio,
             float znear, float zfar);

    void setPosition(const vec3f &tr);
    void setOrientation(const Quaternion &q);

    bool containsPoint(const vec3f &p) const;
    int containsSphere(const vec3f &c, float r) const;

private:
    vec3f m_up, m_dir, m_origin;
    Plane m_planes[6];
    float m_znear, m_zfar, m_nh, m_nw;

    void reset();
};

}; // namespace math

#endif
