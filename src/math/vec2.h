//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec2.h
//! @date 2021-09-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef VEC2_H
#define VEC2_H

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
Vec2<T> operator*(const Vec2<T>& vector, float scalar)
{
    Vec2<T> mul(vector);
    mul *= scalar;

    return mul;
}

template <typename T>
float length(const Vec2<T>& vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

template <typename T>
Vec2<T> rotate(const Vec2<T>& vector, float angle)
{
    float sina = sinf(angle);
    float cosa = cosf(angle);

    float x = vector.x * cosa - vector.y * sina;
    float y = vector.x * sina + vector.y * cosa;

    return {x, y};
}

template <typename T>
Vec2<T> normalize(const Vec2<T>& vector)
{
    float len = length(vector);

    return {vector.x / len, vector.y / len};
}

#endif // VEC2_H