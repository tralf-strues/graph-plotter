//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file mat4.h
//! @date 2021-09-13
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"

const size_t MAT4_SIZE = 4;

template <typename T>
struct Mat4
{
    T elements[MAT4_SIZE * MAT4_SIZE];

    T* operator[](size_t row)
    {
        assert(row < MAT4_SIZE);

        return &elements[row * MAT4_SIZE];
    }

    const T* operator[](size_t row) const
    {
        assert(row < MAT4_SIZE);

        return &elements[row * MAT4_SIZE];
    }

    Mat4& operator+=(const Mat4& second)
    {
        // for (size_t row = 0; row < MAT4_SIZE; row++)
        // {
        //     for (size_t col = 0; col < MAT4_SIZE; col++)
        //     {
        //         elements[row][col] += second[row][col];
        //     }
        // }

        for (size_t i = 0; i < MAT4_SIZE * MAT4_SIZE; i++)
        {
            elements[i] += second.elements[i];
        }

        return *this;
    }

    Mat4& operator-=(const Mat4& second)
    {
        // for (size_t row = 0; row < MAT4_SIZE; row++)
        // {
        //     for (size_t col = 0; col < MAT4_SIZE; col++)
        //     {
        //         elements[row][col] -= second[row][col];
        //     }
        // }

        for (size_t i = 0; i < MAT4_SIZE * MAT4_SIZE; i++)
        {
            elements[i] -= second.elements[i];
        }

        return *this;
    }

    Mat4& operator*=(const Mat4& second)
    {
        for (size_t row = 0; row < MAT4_SIZE; row++)
        {
            for (size_t col = 0; col < MAT4_SIZE; col++)
            {
                T result = 0;

                for (size_t i = 0; i < MAT4_SIZE; i++)
                {
                    result += elements[row * MAT4_SIZE + i] * second.elements[i * MAT4_SIZE + col];
                }

                elements[row * MAT4_SIZE + col] *= result;
            }
        }

        return *this;
    }

    Mat4& operator*=(T scalar)
    {
        // for (size_t row = 0; row < MAT4_SIZE; row++)
        // {
        //     for (size_t col = 0; col < MAT4_SIZE; col++)
        //     {
        //         elements[row][col] *= scalar;
        //     }
        // }

        for (size_t i = 0; i < MAT4_SIZE * MAT4_SIZE; i++)
        {
            elements[i] *= scalar;
        }
    }
};

template <typename T>
Mat4<T> operator+(const Mat4<T>& first, const Mat4<T>& second)
{
    Mat4<T> sum(first);
    sum += second;

    return sum;
}

template <typename T>
Mat4<T> operator-(const Mat4<T>& first, const Mat4<T>& second)
{
    Mat4<T> dif(first);
    dif -= second;

    return dif;
}

template <typename T>
Mat4<T> operator*(const Mat4<T>& first, const Mat4<T>& second)
{
    Mat4<T> product(first);
    product *= second;

    return product;
}

template <typename T>
Vec3<T> operator*(const Mat4<T>& matrix, const Vec3<T>& vector)
{
    Vec3<T> product;
    
    for (size_t row = 0; row < MAT4_SIZE - 1; row++)
    {
        for (size_t col = 0; col < MAT4_SIZE - 1; col++)
        {
            product.getCoord(row) += matrix[row][col] * vector.getCoord(col);
        }
    }

    T w = matrix[MAT4_SIZE - 1][MAT4_SIZE - 1];
    for (size_t col = 0; col < MAT4_SIZE - 1; col++)
    {
        w += matrix[MAT4_SIZE - 1][col] * vector.getCoord(col);
    }

    product /= w;

    return product;
}

template <typename T>
Mat4<T> operator*(const Mat4<T>& matrix, T scalar)
{
    Mat4<T> product(matrix);
    product *= scalar;

    return product;
}

template <typename T>
Mat4<T> createIdentityMatrix(T diagonal)
{
    Mat4<T> matrix;
    
    for (size_t row = 0; row < MAT4_SIZE; row++)
    {
        for (size_t col = 0; col < MAT4_SIZE; col++)
        {
            matrix.elements[row][col] = diagonal;
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
//! @brief Create a rotation matrix around Z axis using Euler's angles.
//! 
//! @param pitchXY Angle around Z axis (in the X-Y plane).
//!
//! @return Rotation matrix.
//------------------------------------------------------------------------------
Mat4<float> createRotationMatrixXY(float pitchXY);

//------------------------------------------------------------------------------
//! @brief Create a rotation matrix around Y axis using Euler's angles.
//! 
//! @param yawZX Angle around Y axis (in the Z-X plane).
//!
//! @return Rotation matrix.
//------------------------------------------------------------------------------
Mat4<float> createRotationMatrixZX(float yawZX);

//------------------------------------------------------------------------------
//! @brief Create a rotation matrix around X axis using Euler's angles.
//! 
//! @param rollYZ Angle around X axis (in the Y-Z plane).
//!
//! @return Rotation matrix.
//------------------------------------------------------------------------------
Mat4<float> createRotationMatrixYZ(float rollYZ);

//------------------------------------------------------------------------------
//! @brief Create a rotation matrix using Euler's angles.
//! 
//! @param pitchXY Angle around Z axis (in the X-Y plane).
//! @param yawZX   Angle around Y axis (in the Z-X plane).
//! @param rollYZ  Angle around X axis (in the Y-Z plane).
//!
//! @return Rotation matrix.
//------------------------------------------------------------------------------
Mat4<float> createRotationMatrix(float pitchXY, float yawZX, float rollYZ);

//------------------------------------------------------------------------------
//! @brief Creates a view matrix for translating from world to camera space.
//! 
//! @param from      Camera's position.
//! @param direction Camera's view direction.
//!
//! @return View matrix.
//------------------------------------------------------------------------------
Mat4<float> lookAt(const Vec3<float>& from, const Vec3<float>& direction);

#endif // MAT4_H