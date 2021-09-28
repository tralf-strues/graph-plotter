//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec3.h
//! @date 2021-09-13
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef VEC3_H
#define VEC3_H

#include <assert.h>
#include <stdio.h> // FIXME: remove
#include <math.h>
#include "vec2.h"

template <typename T>
struct Vec3
{
    T x;
    T y;
    T z;

    Vec3(const Vec2<T>& vector) : x(vector.x), y(vector.y), z(1) {}
    Vec3(const T& x = 0, const T& y = 0, const T& z = 0) : x(x), y(y), z(z) {}

    // T& getCoord(uint8_t i)
    // {
    //     return const_cast<T&>(getCoord(i));
    // }

    T& getCoord(uint8_t i)
    {
        assert(i < 3);

        switch (i)
        {
            case 0: { return x; }
            case 1: { return y; }
            case 2: { return z; }
        }

        return x;
    }

    const T& getCoord(uint8_t i) const
    {
        assert(i < 3);

        switch (i)
        {
            case 0: { return x; }
            case 1: { return y; }
            case 2: { return z; }
        }

        return x;
    }

    Vec3& operator+=(const Vec3& second)
    {
        x += second.x;
        y += second.y;
        z += second.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& second)
    {
        x -= second.x;
        y -= second.y;
        z -= second.z;

        return *this;
    }

    Vec3& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    Vec3& operator/=(T scalar)
    {
        assert(scalar != 0);

        x /= scalar;
        y /= scalar;
        z /= scalar;

        return *this;
    }
};

template <typename T>
Vec3<T> operator+(const Vec3<T>& first, const Vec3<T>& second)
{
    Vec3<T> sum(first);
    sum += second;

    return sum;
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& first, const Vec3<T>& second)
{
    Vec3<T> dif(first);
    dif -= second;

    return dif;
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& vector, T scalar)
{
    Vec3<T> mul(vector);
    mul *= scalar;

    return mul;
}

template <typename T>
Vec3<T> operator*(T scalar, const Vec3<T>& vector)
{
    return vector * scalar;
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& vector, T scalar)
{
    Vec3<T> ratio(vector);
    ratio /= scalar;

    return ratio;
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& vector)
{
    return vector * (-1.0f);
}

template <typename T>
Vec3<T> componentMultiply(const Vec3<T>& first, const Vec3<T>& second)
{
    return {first.x * second.x, first.y * second.y, first.z * second.z};
}

template <typename T>
float lengthSquare(const Vec3<T>& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

template <typename T>
float length(const Vec3<T>& vector)
{
    return sqrtf(lengthSquare(vector));
}

template <typename T>
Vec3<T> normalize(const Vec3<T>& vector)
{
    float len = length(vector);

    if (len == 0)
    {
        printf("WTF?\n");
    }

    assert(len != 0);

    return vector / len;
}

template <typename T>
T dotProduct(const Vec3<T>& first, const Vec3<T>& second)
{
    return first.x * second.x + 
           first.y * second.y +
           first.z * second.z;
}

template <typename T>
Vec3<T> crossProduct(const Vec3<T>& first, const Vec3<T>& second)
{
    Vec3<T> product;

	product.x = first.y * second.z - first.z * second.y;
	product.y = first.z * second.x - first.x * second.z;
	product.z = first.x * second.y - first.y * second.x;

	return product;
}

#endif // VEC3_H