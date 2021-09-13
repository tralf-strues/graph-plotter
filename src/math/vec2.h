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

    float getLength() const
    {
        return sqrtf(x * x + y * y);
    }

    void rotate(float angle)
    {
        float sina = sinf(angle);
        float cosa = cosf(angle);

        float old_x = x;
        float old_y = y;

        x = old_x * cosa - old_y * sina;
        y = old_x * sina + old_y * cosa;
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
Vec2<T> rotate(const Vec2<T>& vector, float angle)
{
    Vec2<T> rotated(vector);
    rotated.rotate(angle);

    return rotated;
}

#endif // VEC2_H