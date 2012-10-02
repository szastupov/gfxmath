#ifndef RECT_H
#define RECT_H
#include "vec.h"

namespace math {

struct Rect {
    float x, y, w, h;

    Rect()
        : x(0), y(0), w(1), h(1)
    {}

    Rect(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h)
    {}

    Rect(float x, float y)
        : x(x), y(y), w(1), h(1)
    {}

    template <typename T>
    explicit Rect(const vec<2, T> &pos, const vec<2, T> &scale)
        : x(pos.x()), y(pos.y()), w(scale.x()), h(scale.y())
    {}

    template <typename T>
    explicit Rect(const vec<2, T> &pos)
        : x(pos.x()), y(pos.y()), w(1), h(1)
    {}

    template <typename T>
    explicit Rect(const vec<4, T> &v)
        : x(v[0]), y(v[1]), w(v[2]), h(v[3])
    {}

    template <typename T>
    bool inRect(const vec<2, T> &pos) const
    {
        return (pos.x() >= x &&
                pos.x() < x+w &&
                pos.y() >= y &&
                pos.y() < y+h);
    }

    template <typename T>
    vec2f toRectUnit(const vec<2, T> &pos) const
    {
        return vec2f((pos.x()-x)/w, (pos.y()-y)/h);
    }

    vec2f getCenter() const
    {
        return vec2f(x+w*0.5f, y+h*0.5f);
    }

    void setCenter(const vec2f &pos)
    {
        x = pos.x()-w*0.5f;
        y = pos.y()-h*0.5f;
    }

    bool operator == (const Rect &b) const
    {
        return (x == b.x &&
                y == b.y &&
                w == b.w &&
                h == b.h);
    }

    bool operator != (const Rect &b) const
    {
        return (x != b.x ||
                y != b.y ||
                w != b.w ||
                h != b.h);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & x;
        ar & y;
        ar & w;
        ar & h;
    }
};

}; // namespace math

#endif
