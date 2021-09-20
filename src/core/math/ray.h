//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ray.h
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray
{
    Vec3<float> from;
    Vec3<float> direction;

    Vec3<float> at(float rayParameter) const;
};

#endif // RAY_H