//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec2.h
//! @date 2021-09-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef VEC2_H
#define VEC2_H

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include "float_compare.h"

template <typename T>
struct Vec2
{
    T x;
    T y;

    Vec2() = default;
    Vec2(const T& x, const T& y) : x(x), y(y) {}

    template<typename U = float>
    Vec2<int32_t>(Vec2<U> vector) : x(vector.x), y(vector.y) {}

    Vec2& operator+=(const Vec2& second)
    {
        x += second.x;
        y += second.y;

        return *this;
    }

    Vec2& operator-=(const Vec2& second)
    {
        x -= second.x;
        y -= second.y;

        return *this;
    }

    Vec2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    Vec2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;

        return *this;
    }
};

template <typename T>
Vec2<T> operator+(const Vec2<T>& first, const Vec2<T>& second)
{
    Vec2<T> sum(first);
    sum += second;

    return sum;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& first, const Vec2<T>& second)
{
    Vec2<T> dif(first);
    dif -= second;

    return dif;
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& vector, T scalar)
{
    Vec2<T> mul(vector);
    mul *= scalar;

    return mul;
}

template <typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& vector)
{
    return vector * scalar;
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& vector, T scalar)
{
    Vec2<T> ratio(vector);
    ratio /= scalar;

    return ratio;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& vector)
{
    return vector * (-1.0f);
}

template <typename T>
Vec2<T> componentMultiply(const Vec2<T>& first, const Vec2<T>& second)
{
    return {first.x * second.x, first.y * second.y};
}

template <typename T>
float lengthSquare(const Vec2<T>& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

template <typename T>
float length(const Vec2<T>& vector)
{
    return sqrtf(lengthSquare(vector));
}

template <typename T>
void rotate(Vec2<T>& vector, float angle)
{
    float sina = sinf(angle);
    float cosa = cosf(angle);

    float x = vector.x;
    float y = vector.y;

    vector.x = x * cosa - y * sina;
    vector.y = x * sina + y * cosa;
}

template <typename T>
Vec2<T> getNormal(const Vec2<T>& vector)
{
    float length = length(vector);
    if (cmpFloat(length, 0) == 0)
    {
        return Vec2<T>{0, 0};
    }

    return normalize(Vec2<T>{-vector.y / length, vector.x / length});
}

template <typename T>
Vec2<T> normalize(const Vec2<T>& vector)
{
    float len = length(vector);
    assert(len != 0);

    return vector / len;
}

template <typename T>
T dotProduct(const Vec2<T>& first, const Vec2<T>& second)
{
    return first.x * second.x + 
           first.y * second.y;
}

#endif // VEC2_H