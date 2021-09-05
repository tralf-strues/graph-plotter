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

VEC2_NAME add(VEC2_NAME vector1, VEC2_NAME vector2);
VEC2_NAME sub(VEC2_NAME vector1, VEC2_NAME vector2);