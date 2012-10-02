#include "frustum.h"

namespace math {

Frustum::Frustum()
    : m_up(0.0f, 1.0f, 0.0f)
    , m_dir(0.0f, 0.0f, -1.0f)
    , m_origin(0.0f)
{
}

void Frustum::reset()
{
    // Camera axes
    vec3f z = -m_dir;
    vec3f x = cross(m_up, z).normalized();
    vec3f y = cross(z, x);

    vec3f nc = m_origin - z*m_znear;
    vec3f fc = m_origin - z*m_zfar;

    vec3f aux;

    // near plane
    m_planes[0].set(nc, -z);
    // far plane
    m_planes[1].set(fc, z);

    // top
	aux = (nc + y*m_nh) - m_origin;
	aux.normalize();
	m_planes[2].set(nc+y*m_nh, cross(aux, x));

    // bottom
    aux = (nc - y*m_nh) - m_origin;
	aux.normalize();
	m_planes[3].set(nc-y*m_nh, cross(x, aux));

    // left
    aux = (nc - x*m_nw) - m_origin;
	aux.normalize();
	m_planes[4].set(nc-x*m_nw, cross(aux, y));

    // right
    aux = (nc + x*m_nw) - m_origin;
	aux.normalize();
	m_planes[5].set(nc+x*m_nw, cross(y, aux));
}

void Frustum::set(float fov, float aspectRatio,
                  float znear, float zfar)
{
    m_znear = znear;
    m_zfar = zfar;
    float tfov = tanf(fov*M_PI/180.0f/2.0f);
    m_nh = tfov*znear;
    m_nw = m_nh*aspectRatio;
    reset();
}

void Frustum::setPosition(const vec3f &pos)
{
    if (pos != m_origin) {
        m_origin = pos;
        reset();
    }
}

void Frustum::setOrientation(const Quaternion &q)
{
    Matrix4f mat = q.toMatrix();
    m_up = vec3(mat * vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_up.normalize();
    m_dir = vec3(mat * vec4f(0.0f, 0.0f, -1.0f, 1.0f));
    m_dir.normalize();
    reset();
}

bool Frustum::containsPoint(const vec3f &p) const
{
    for (int i = 0; i < 6; i++) {
        if (m_planes[i].distance(p) < 0)
            return false;
    }
    return true;
}

int Frustum::containsSphere(const vec3f &c, float r) const
{
    for (int i = 0; i < 6; i++) {
        float d = m_planes[i].distance(c);
        if (d < -r)
            return OUTSIDE;
        if (fabs(d) < r)
            return INTERSECT;
    }
    return INSIDE;
}

}; // namespace mats
