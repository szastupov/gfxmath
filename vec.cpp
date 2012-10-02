#include <iostream>
#include "vec.h"

namespace math {

std::ostream& operator<<(std::ostream &out, const Ray &ray)
{
    out << "Ray {"
        << ray.origin << ", "
        << ray.dir
        << "}";

    return out;
}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream &out, const vec<N, T> &v)
{
    out << "(";
    for (size_t i = 0; i < N; i++) {
        out << v[i];
        if (i+1 != N)
            out << ", ";
    }
    out << ")";

    return out;
}

template
std::ostream& operator<< <3, float>(std::ostream &out, const vec<3, float> &v);

template
std::ostream& operator<< <4, float>(std::ostream &out, const vec<4, float> &v);

template
std::ostream& operator<< <2, float>(std::ostream &out, const vec<2, float> &v);

template
std::ostream& operator<< <2, int>(std::ostream &out, const vec<2, int> &v);

}; // namespace math