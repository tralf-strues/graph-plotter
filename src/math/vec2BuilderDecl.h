//------------------------------------------------------------------------------
//! @brief 2d vector builder.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec2Builder.h
//! @date 2021-09-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

struct VEC2_NAME
{
    VEC2_TYPE x;
    VEC2_TYPE y;
};

VEC2_NAME add(const VEC2_NAME* vector1, const VEC2_NAME* vector2);
VEC2_NAME sub(const VEC2_NAME* vector1, const VEC2_NAME* vector2);
VEC2_NAME mul(const VEC2_NAME* vector, VEC2_TYPE scalar);

VEC2_TYPE len(const VEC2_NAME* vector);
VEC2_NAME rot(const VEC2_NAME* vector, float angle);