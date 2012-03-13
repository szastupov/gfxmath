#include "vec.h"
#include "matrix.h"
#include "quaternion.h"
#include "plane.h"
#include "frustum.h"

#define BOOST_TEST_MODULE MathTest
#ifdef ANDROID
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

BOOST_AUTO_TEST_CASE(Vectors)
{
    vec3f a(1.0f, 2.0f, 2.0f);
    vec3f b(2.0f, 4.0f, 4.0f);
    vec3f c(1.0f, 2.0f, 2.0f);
    vec3f d(-1.0f, -2.0f, -2.0f);
    vec3f n1(1.f, 0.f, 0.f);
    vec3f n2(0.f, 1.f, 0.f);
    vec3f n3(0.f, 0.f, 1.f);

    BOOST_CHECK_NE(a, b);
    BOOST_CHECK_EQUAL(a, c);
    BOOST_CHECK_EQUAL(a+c, b);
    BOOST_CHECK_EQUAL(d, -a);
    BOOST_CHECK_EQUAL(a*2, b);
    BOOST_CHECK_EQUAL(b/2, a);
    BOOST_CHECK(a[0] == 1 && a[1] == 2);
    BOOST_CHECK(a[0] == a.x() && a[1] == a.y());
    BOOST_CHECK_NE(b.length(),  0);
    BOOST_CHECK_EQUAL(distance(a, b), (a-b).length());
    BOOST_CHECK_EQUAL(dot(a, b), 18);
    BOOST_CHECK_EQUAL(cross(n1, n2), n3);
    BOOST_CHECK_EQUAL(vec4f(n1, 1.0f), vec4f(n1[0], n1[1], n1[2], 1.0f));
}

BOOST_AUTO_TEST_CASE(MatrixMult)
{
    Matrix4f mscale = scale(1.0f, 2.0f, 3.0f);
    vec4f r = mscale*vec4f(2.0f, 2.0f, 2.0f, 1.0f);
    BOOST_CHECK_EQUAL(r, vec4f(2.0f, 4.0f, 6.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(MatrixTransforms)
{
    Matrix4f mid = identity();
    Matrix4f mscale = scale(1.0f, 2.0f, 3.0f);
    Matrix4f mtranslate = translate(4.0f, 5.0f, 6.0f);
    Matrix4f mts;
    mts.loadIdentity();
    mts.setScale(1.0f, 2.0f, 3.0f);
    mts.setTranslate(4.0f, 5.0f, 6.0f);

    BOOST_REQUIRE(mid.isIdentinty());
    BOOST_REQUIRE(mscale.isScale());

    BOOST_CHECK_EQUAL(mid * mscale, mscale);
    BOOST_CHECK_EQUAL(mid * mtranslate, mtranslate);
    BOOST_CHECK_EQUAL(mtranslate * mscale, mts);
}

BOOST_AUTO_TEST_CASE(MatrixTranspose)
{
    Matrix4f m1, m2;
    int c = 0;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++) {
            m1[j][i] = c;
            m2[i][j] = c;
            c++;
        }

    BOOST_CHECK_EQUAL(m1.transposed(), m2);
}

BOOST_AUTO_TEST_CASE(QuaternionAlgebra)
{
    Quaternion a(2.0f, vec3f(1.0f, 2.0f, 3.0f));
    Quaternion b(3.0f, vec3f(4.0f, 5.0f, 6.0f));
    Quaternion c(5.0f, vec3f(5.0f, 7.0f, 9.0f));
    BOOST_CHECK_EQUAL(a+b, c);
    BOOST_CHECK_EQUAL(c-a, b);
    BOOST_CHECK_EQUAL(c-b, a);
    BOOST_CHECK_NE(a*b, b*a);
}

BOOST_AUTO_TEST_CASE(QuaternionConversions)
{
    Quaternion id;
    BOOST_CHECK(id.toMatrix().isIdentinty());

    ///
    float a1 = 3.14f;
    vec3f axis1(1.0f, 0.0f, 0.0f);
    Quaternion qaa = Quaternion::fromAngleAxis(a1, axis1);

    float a2;
    vec3f axis2;
    qaa.toAngleAxis(a2, axis2);
    BOOST_CHECK_EQUAL(a1, a2);
    BOOST_CHECK_EQUAL(axis1, axis2);
    ///

    ///
    vec3f angles1(rad(10.0f), rad(20.0f), rad(30.0f));
    Quaternion qe = Quaternion::fromEuler(angles1);
    vec3f angles2 = qe.toEuler();
    BOOST_CHECK_CLOSE(angles1[0], angles2[0], 0.0001);
    BOOST_CHECK_CLOSE(angles1[1], angles2[1], 0.0001);
    BOOST_CHECK_CLOSE(angles1[2], angles2[2], 0.0001);
    ///

    // TODO complete it
}

BOOST_AUTO_TEST_CASE(Planes)
{
    Plane plane;
    plane.set(vec3f(3.0f, 2.0f, 0.0f),
              vec3f(0.0f, 1.0f, 0.0f));

    vec3f p1(4.0f, 2.0f, 0.0f);
    vec3f p2(4.0f, 4.0f, 0.0f);
    Ray ray1 = { vec3f(3.0f, 5.0f, 0.0f), vec3f(0.0f, -1.0f, 0.0f) };
    Ray ray2 = { vec3f(3.0f, 0.0f, 0.0f), vec3f(0.0f, -1.0f, 0.0f) };
    Ray ray3 = { vec3f(3.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f) };

    BOOST_CHECK_EQUAL(plane.distance(p1), 0.0f);
    BOOST_CHECK_EQUAL(plane.distance(p2), 2.0f);
    BOOST_CHECK_EQUAL(plane.distance(ray1), 3.0f);
    BOOST_CHECK_EQUAL(plane.distance(ray2), -2.0f);
    BOOST_CHECK_EQUAL(plane.distance(ray3), 2.0f);
}

BOOST_AUTO_TEST_CASE(FrustumCulling)
{
    Frustum frustum;
    frustum.set(45.0f, 1.0f, 1.0f, 100.0f);
    BOOST_CHECK(frustum.containsPoint(vec3f(0.0f, 0.0f, -2.0f)));
    BOOST_CHECK(!frustum.containsPoint(vec3f(10.0f, 0.0f, -2.0f)));
    BOOST_CHECK_EQUAL(frustum.containsSphere(vec3f(10.0f, 0.0f, -2.0f), 10.0f), INTERSECT);
    BOOST_CHECK_EQUAL(frustum.containsSphere(vec3f(10.0f, 0.0f, -2.0f), 1.0f), INTERSECT);
    BOOST_CHECK_EQUAL(frustum.containsSphere(vec3f(0.0f, 0.0f, 0.0f), 1.0f), INTERSECT);

    // Check z-axis moves
    frustum.setPosition(vec3f(0.0f, 0.0f, 2.0f));
    BOOST_CHECK(frustum.containsPoint(vec3f(0.0f, 0.0f, -2.0f)));
    BOOST_CHECK(!frustum.containsPoint(vec3f(10.0f, 0.0f, -2.0f)));

    // Check x-axis moves
    frustum.setPosition(vec3f(-10.0f, 0.0f, 0.0f));
    BOOST_CHECK(frustum.containsPoint(vec3f(-10.0f, 0.0f, -2.0f)));
    BOOST_CHECK(!frustum.containsPoint(vec3f(0.0f, 0.0f, -2.0f)));
    BOOST_CHECK(frustum.containsSphere(vec3f(0.0f, 0.0f, -2.0f), 10.0f));

    // TODO add rotate test
}

BOOST_AUTO_TEST_CASE(Matrix3)
{
    float zero[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    Matrix3f mzero(zero);
    BOOST_CHECK_EQUAL(mzero.det(), 0.0f);

    float scale[] = {
        1, 0, 0,
        0, 2, 0,
        0, 0, 3
    };
    Matrix3f mscale(scale);
    BOOST_CHECK_EQUAL(mscale.det(), 6.0f);

    Matrix3f m1, m2;
    int c = 0;
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++) {
            m1[j][i] = c;
            m2[i][j] = c;
            c++;
        }

    m1.transpose();
    BOOST_REQUIRE_EQUAL(m1, m2);

    float identity[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    Matrix3f inv = mscale.inverse();
    Matrix3f id = inv*mscale;

    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            BOOST_CHECK_EQUAL(id[j][i], identity[j][i]);
}

BOOST_AUTO_TEST_CASE(Matrix2)
{
    float src[] = {
        1, 3,
        2, 4
    };
    float expected[] = {
        1, 2,
        3, 4
    };

    Matrix2f mat(src);
    mat.transpose();
    BOOST_CHECK_EQUAL_COLLECTIONS(mat.data(), mat.data()+4,
                                  expected, expected+4);

    Matrix2f mat2(src);
    vec2f v(5, 6);
    BOOST_CHECK_EQUAL(mat2*v, vec2f(17, 39));

    Matrix2f mat3(src);
    float det = mat3[0][0]*mat3[1][1] - mat3[1][0]*mat3[0][1];
    BOOST_CHECK_EQUAL(mat3.det(), det);
}
