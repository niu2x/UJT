/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __MATH_GEOMETRY_H__
#define __MATH_GEOMETRY_H__

#include "Vec2.h"

namespace Math{

class Size
{
public:
    float width;
    float height;

public:
    operator Vec2() const
    {
        return Vec2(width, height);
    }

public:

    Size();
    Size(float width, float height);
    Size(const Size& other);
    explicit Size(const Vec2& point);
   
    Size& operator= (const Size& other);

    Size& operator= (const Vec2& point);

    Size operator+(const Size& right) const;

    Size operator-(const Size& right) const;
   
    Size operator*(float a) const;

    Size operator/(float a) const;
    
    void setSize(float width, float height);
   
    bool equals(const Size& target) const;
    
    static const Size ZERO;
};

/**Rectangle area.*/
class Rect
{
public:

    Vec2 origin;

    Size  size;

public:
    Rect();
    Rect(float x, float y, float width, float height);
    Rect(const Vec2& pos, const Size& dimension);
    Rect(const Rect& other);
    Rect& operator= (const Rect& other);
    void setRect(float x, float y, float width, float height);
    float getMinX() const; /// return the leftmost x-value of current rect
    float getMidX() const; /// return the midpoint x-value of current rect
    float getMaxX() const; /// return the rightmost x-value of current rect
    float getMinY() const; /// return the bottommost y-value of current rect
    float getMidY() const; /// return the midpoint y-value of current rect
    float getMaxY() const; /// return the topmost y-value of current rect
    bool equals(const Rect& rect) const;
    bool containsPoint(const Vec2& point) const;
    bool intersectsRect(const Rect& rect) const;
    bool intersectsCircle(const Vec2& center, float radius) const;
    Rect unionWithRect(const Rect & rect) const;
    void merge(const Rect& rect);
    static const Rect ZERO;
};

}

#endif // __MATH_CCGEOMETRY_H__
