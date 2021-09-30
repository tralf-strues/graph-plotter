//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file mat4.cpp
//! @date 2021-09-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "mat4.h"

Mat4<float> createRotationMatrixXY(float pitchXY)
{
    float cosPitch = cosf(pitchXY);
    float sinPitch = sinf(pitchXY);

    return {{cosPitch, -sinPitch, 0, 0,
             sinPitch, cosPitch,  0, 0,
             0,        0,         1, 0,
             0,        0,         0, 1}};
}

Mat4<float> createRotationMatrixZX(float yawZX)
{
    float cosYaw = cosf(yawZX);
    float sinYaw = sinf(yawZX);

    return {{cosYaw,  0, sinYaw, 0,
             0,       1, 0,      0,
             -sinYaw, 0, cosYaw, 0,
             0,       0, 0,      1}};
}

Mat4<float> createRotationMatrixYZ(float rollYZ)
{
    float cosRoll = cosf(rollYZ);
    float sinRoll = sinf(rollYZ);

    return {{1, 0,       0,        0,
             0, cosRoll, -sinRoll, 0,
             0, sinRoll, cosRoll,  0,
             0, 0,       0,        1}};
}

Mat4<float> createRotationMatrix(float pitchXY, float yawZX, float rollYZ)
{
    return createRotationMatrixYZ(rollYZ) * 
           createRotationMatrixZX(yawZX)  * 
           createRotationMatrixXY(pitchXY);
}

Mat4<float> createScaleMatrix(Vec3<float> scale)
{
    return {{scale.x, 0,       0,       0,
             0,       scale.y, 0,       0,
             0,       0,       scale.z, 0,
             0,       0,       0,       1}};
}

Mat4<float> createTranslationMatrix(Vec3<float> translation)
{
    return {{1, 0, 0, translation.x,
             0, 1, 0, translation.y,
             0, 0, 1, translation.z,
             0, 0, 0, 1}};
}

Mat4<float> lookAt(const Vec3<float>& from, const Vec3<float>& direction)
{
    static const Vec3<float> tmp = {0, 1, 0};

    Vec3<float> forward = normalize(direction);
    Vec3<float> right   = crossProduct(tmp, forward);
    Vec3<float> up      = crossProduct(forward, right);

    return {{right.x,   right.y,   right.z,   -dotProduct(right,   from),
             up.x,      up.y,      up.z,      -dotProduct(up,      from),
             forward.x, forward.y, forward.z, -dotProduct(forward, from),
             0,         0,         0,         1                         }};
}