/**
 Copyright 2013 BlackBerry Inc.
 Copyright (c) 2014-2015 Chukong Technologies

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Original file from GamePlay3D: http://gameplay3d.org

 This file was modified to fit the cocos2d-x project
 */

#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include <algorithm>
#include <functional>
#include <math.h>
namespace Math{

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

class Mat4;

class Vec2
{
public:

    float x;

    float y;

    Vec2();

    Vec2(float xx, float yy);

    Vec2(const float* array);

    Vec2(const Vec2& p1, const Vec2& p2);

    Vec2(const Vec2& copy);

    ~Vec2();

    inline bool isZero() const;

    inline bool isOne() const;

    static float angle(const Vec2& v1, const Vec2& v2);

    inline void add(const Vec2& v);

    static void add(const Vec2& v1, const Vec2& v2, Vec2* dst);

    void clamp(const Vec2& min, const Vec2& max);

    static void clamp(const Vec2& v, const Vec2& min, const Vec2& max, Vec2* dst);

    float distance(const Vec2& v) const;

    inline float distanceSquared(const Vec2& v) const;

    inline float dot(const Vec2& v) const;

    static float dot(const Vec2& v1, const Vec2& v2);

    float length() const;

    inline float lengthSquared() const;

    inline void negate();

    void normalize();

    Vec2 getNormalized() const;

    inline void scale(float scalar);

    inline void scale(const Vec2& scale);

    void rotate(const Vec2& point, float angle);

    inline void set(float xx, float yy);

    void set(const float* array);

    inline void set(const Vec2& v);

    inline void set(const Vec2& p1, const Vec2& p2);

    inline void setZero();

    inline void subtract(const Vec2& v);

    static void subtract(const Vec2& v1, const Vec2& v2, Vec2* dst);

    inline void smooth(const Vec2& target, float elapsedTime, float responseTime);

    inline const Vec2 operator+(const Vec2& v) const;

    inline Vec2& operator+=(const Vec2& v);

    inline const Vec2 operator-(const Vec2& v) const;

    inline Vec2& operator-=(const Vec2& v);

    inline const Vec2 operator-() const;

    inline const Vec2 operator*(float s) const;

    inline Vec2& operator*=(float s);
    
    inline const Vec2 operator/(float s) const;

    inline bool operator<(const Vec2& v) const;
    
    inline bool operator>(const Vec2& v) const;

    inline bool operator==(const Vec2& v) const;

    inline bool operator!=(const Vec2& v) const;

    //code added compatible for Point
public:

    inline void setPoint(float xx, float yy);

    bool equals(const Vec2& target) const;
    
    bool fuzzyEquals(const Vec2& target, float variance) const;

    inline float getLength() const {
        return sqrt(x*x + y*y);
    };

    inline float getLengthSq() const {
        return dot(*this); //x*x + y*y;
    };

    inline float getDistanceSq(const Vec2& other) const {
        return (*this - other).getLengthSq();
    };

    inline float getDistance(const Vec2& other) const {
        return (*this - other).getLength();
    };

    inline float getAngle() const {
        return atan2f(y, x);
    };

    float getAngle(const Vec2& other) const;

    inline float cross(const Vec2& other) const {
        return x*other.y - y*other.x;
    };

    inline Vec2 getPerp() const {
        return Vec2(-y, x);
    };
    
    inline Vec2 getMidpoint(const Vec2& other) const
    {
        return Vec2((x + other.x) / 2.0f, (y + other.y) / 2.0f);
    }
    
    inline Vec2 getClampPoint(const Vec2& min_inclusive, const Vec2& max_inclusive) const
    {
        return Vec2(clampf(x,min_inclusive.x,max_inclusive.x), clampf(y, min_inclusive.y, max_inclusive.y));
    }
    
    inline Vec2 compOp(std::function<float(float)> function) const
    {
        return Vec2(function(x), function(y));
    }

    inline Vec2 getRPerp() const {
        return Vec2(y, -x);
    };

    inline Vec2 project(const Vec2& other) const {
        return other * (dot(other)/other.dot(other));
    };

    /** Complex multiplication of two points ("rotates" two points).
     @return Vec2 vector with an angle of this.getAngle() + other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     * @js NA
     * @lua NA
     */
    inline Vec2 rotate(const Vec2& other) const {
        return Vec2(x*other.x - y*other.y, x*other.y + y*other.x);
    };

    /** Unrotates two points.
     @return Vec2 vector with an angle of this.getAngle() - other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     * @js NA
     * @lua NA
     */
    inline Vec2 unrotate(const Vec2& other) const {
        return Vec2(x*other.x + y*other.y, y*other.x - x*other.y);
    };

    /** Linear Interpolation between two points a and b
     @returns
        alpha == 0 ? a
        alpha == 1 ? b
        otherwise a value between a..b
     @since v2.1.4
     * @js NA
     * @lua NA
     */
    inline Vec2 lerp(const Vec2& other, float alpha) const {
        return *this * (1.f - alpha) + other * alpha;
    };

    /** Rotates a point counter clockwise by the angle around a pivot
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation ccw in radians
     @returns the rotated point
     @since v2.1.4
     * @js NA
     * @lua NA
     */
    Vec2 rotateByAngle(const Vec2& pivot, float angle) const;

    /**
     * @js NA
     * @lua NA
     */
    static inline Vec2 forAngle(const float a)
    {
        return Vec2(cos(a), sin(a));
    }
    
    /** A general line-line intersection test
     @param A   the startpoint for the first line L1 = (A - B)
     @param B   the endpoint for the first line L1 = (A - B)
     @param C   the startpoint for the second line L2 = (C - D)
     @param D   the endpoint for the second line L2 = (C - D)
     @param S   the range for a hitpoint in L1 (p = A + S*(B - A))
     @param T   the range for a hitpoint in L2 (p = C + T*(D - C))
     @return    whether these two lines intersects.

     Note that to truly test intersection for segments we have to make
     sure that S & T lie within [0..1] and for rays, make sure S & T > 0
     the hit point is        C + T * (D - C);
     the hit point also is   A + S * (B - A);
     @since 3.0
     * @js NA
     * @lua NA
     */
    static bool isLineIntersect(const Vec2& A, const Vec2& B,
                                 const Vec2& C, const Vec2& D,
                                 float *S = nullptr, float *T = nullptr);
    
    /**
     returns true if Line A-B overlap with segment C-D
     @since v3.0
     * @js NA
     * @lua NA
     */
    static bool isLineOverlap(const Vec2& A, const Vec2& B,
                                const Vec2& C, const Vec2& D);
    
    /**
     returns true if Line A-B parallel with segment C-D
     @since v3.0
     * @js NA
     * @lua NA
     */
    static bool isLineParallel(const Vec2& A, const Vec2& B,
                   const Vec2& C, const Vec2& D);
    
    /**
     returns true if Segment A-B overlap with segment C-D
     @since v3.0
     * @js NA
     * @lua NA
     */
    static bool isSegmentOverlap(const Vec2& A, const Vec2& B,
                                 const Vec2& C, const Vec2& D,
                                 Vec2* S = nullptr, Vec2* E = nullptr);
    
    /**
     returns true if Segment A-B intersects with segment C-D
     @since v3.0
     * @js NA
     * @lua NA
     */
    static bool isSegmentIntersect(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D);
    
    /**
     returns the intersection point of line A-B, C-D
     @since v3.0
     * @js NA
     * @lua NA
     */
    static Vec2 getIntersectPoint(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D);
    
    /** equals to Vec2(0,0) */
    static const Vec2 ZERO;
    /** equals to Vec2(1,1) */
    static const Vec2 ONE;
    /** equals to Vec2(1,0) */
    static const Vec2 UNIT_X;
    /** equals to Vec2(0,1) */
    static const Vec2 UNIT_Y;
    /** equals to Vec2(0.5, 0.5) */
    static const Vec2 ANCHOR_MIDDLE;
    /** equals to Vec2(0, 0) */
    static const Vec2 ANCHOR_BOTTOM_LEFT;
    /** equals to Vec2(0, 1) */
    static const Vec2 ANCHOR_TOP_LEFT;
    /** equals to Vec2(1, 0) */
    static const Vec2 ANCHOR_BOTTOM_RIGHT;
    /** equals to Vec2(1, 1) */
    static const Vec2 ANCHOR_TOP_RIGHT;
    /** equals to Vec2(1, 0.5) */
    static const Vec2 ANCHOR_MIDDLE_RIGHT;
    /** equals to Vec2(0, 0.5) */
    static const Vec2 ANCHOR_MIDDLE_LEFT;
    /** equals to Vec2(0.5, 1) */
    static const Vec2 ANCHOR_MIDDLE_TOP;
    /** equals to Vec2(0.5, 0) */
    static const Vec2 ANCHOR_MIDDLE_BOTTOM;
};

/**
 * Calculates the scalar product of the given vector with the given value.
 * 
 * @param x The value to scale by.
 * @param v The vector to scale.
 * @return The scaled vector.
 */
inline const Vec2 operator*(float x, const Vec2& v);

typedef Vec2 Point;

}

/**
 end of base group
 @}
 */

#include "Vec2.inl"

#endif
