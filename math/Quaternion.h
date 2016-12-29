#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Vec3.h"

namespace Math{

class Mat4;

class Quaternion
{
    friend class Curve;
    friend class Transform;

public:
    float x;
    float y;
    float z;
    float w;

    Quaternion();

    Quaternion(float xx, float yy, float zz, float ww);

    Quaternion(float* array);

    Quaternion(const Mat4& m);

    Quaternion(const Vec3& axis, float angle);

    Quaternion(const Quaternion& copy);

    ~Quaternion();

    static const Quaternion& identity();

    static const Quaternion& zero();

    bool isIdentity() const;

    bool isZero() const;

    static void createFromRotationMatrix(const Mat4& m, Quaternion* dst);

    static void createFromAxisAngle(const Vec3& axis, float angle, Quaternion* dst);

    void conjugate();

    Quaternion getConjugated() const;

    bool inverse();

    Quaternion getInversed() const;

    void multiply(const Quaternion& q);

    static void multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst);

    void normalize();

    Quaternion getNormalized() const;

    void set(float xx, float yy, float zz, float ww);

    void set(float* array);

    void set(const Mat4& m);

    void set(const Vec3& axis, float angle);

    void set(const Quaternion& q);

    void setIdentity();

    float toAxisAngle(Vec3* e) const;

    static void lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    
    static void slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
    
    static void squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t, Quaternion* dst);

    inline Quaternion operator*(const Quaternion& q) const;

    inline Vec3 operator*(const Vec3& v) const;

    inline Quaternion& operator*=(const Quaternion& q);
    
    static const Quaternion ZERO;

private:

    static void slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, float q2z, float q2w, float t, float* dstx, float* dsty, float* dstz, float* dstw);

    static void slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
};

}

#include "Quaternion.inl"

#endif
