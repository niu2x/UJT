#ifndef MATH_VEC4_H
#define MATH_VEC4_H

namespace Math{

class Mat4;

class Vec4
{
public:
    float x;

    float y;

    float z;

    float w;

    Vec4();

    Vec4(float xx, float yy, float zz, float ww);

    Vec4(const float* array);

    Vec4(const Vec4& p1, const Vec4& p2);

    Vec4(const Vec4& copy);

    static Vec4 fromColor(unsigned int color);

    ~Vec4();

    bool isZero() const;

    bool isOne() const;

    static float angle(const Vec4& v1, const Vec4& v2);

    void add(const Vec4& v);

    static void add(const Vec4& v1, const Vec4& v2, Vec4* dst);

    void clamp(const Vec4& min, const Vec4& max);

    static void clamp(const Vec4& v, const Vec4& min, const Vec4& max, Vec4* dst);

    float distance(const Vec4& v) const;

    float distanceSquared(const Vec4& v) const;

    float dot(const Vec4& v) const;

    static float dot(const Vec4& v1, const Vec4& v2);

    float length() const;

    float lengthSquared() const;

    void negate();

    void normalize();

    Vec4 getNormalized() const;

    void scale(float scalar);

    void set(float xx, float yy, float zz, float ww);

    void set(const float* array);

    void set(const Vec4& v);

    void set(const Vec4& p1, const Vec4& p2);

    void subtract(const Vec4& v);

    static void subtract(const Vec4& v1, const Vec4& v2, Vec4* dst);

    inline const Vec4 operator+(const Vec4& v) const;

    inline Vec4& operator+=(const Vec4& v);

    inline const Vec4 operator-(const Vec4& v) const;

    inline Vec4& operator-=(const Vec4& v);

    inline const Vec4 operator-() const;

    inline const Vec4 operator*(float s) const;

    inline Vec4& operator*=(float s);

    inline const Vec4 operator/(float s) const;

    inline bool operator<(const Vec4& v) const;

    inline bool operator==(const Vec4& v) const;

    inline bool operator!=(const Vec4& v) const;
    
    static const Vec4 ZERO;
    static const Vec4 ONE;
    static const Vec4 UNIT_X;
    static const Vec4 UNIT_Y;
    static const Vec4 UNIT_Z;
    static const Vec4 UNIT_W;
};

/**
 * Calculates the scalar product of the given vector with the given value.
 * 
 * @param x The value to scale by.
 * @param v The vector to scale.
 * @return The scaled vector.
 */
inline const Vec4 operator*(float x, const Vec4& v);

}

#include "Vec4.inl"

#endif // MATH_VEC4_H
