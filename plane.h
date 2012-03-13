#ifndef PLANE_H
#define PLANE_H
#include "vec.h"

class Plane {
public:
    Plane()
    {}

    Plane(const vec3f &p0, const vec3f &n)
        : m_n(n)
        , m_p0(p0)
    {}

    float distance(const vec3f &p) const
    {
        return dot(m_n, p-m_p0);
    }

    float distance(const Ray &ray) const
    {
        return dot(m_p0-ray.origin, m_n)
            /  dot(ray.dir, m_n);
    }

    // set from normal and origin
    void set(const vec3f &p0, const vec3f &n)
    {
        assert(fabs(n.length() - 1.0f) < 0.00001);
        m_n = n;
        m_p0 = p0;
    }

private:
    vec3f m_n;                  // normal
    vec3f m_p0;
};

#endif
