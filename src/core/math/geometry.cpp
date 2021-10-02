//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file geometry.cpp
//! @date 2021-10-02
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <math.h>
#include "geometry.h"

float calculateSphereVolume(float radius)
{
    return 4.0f / 3.0f * M_PI * radius * radius * radius;
}

float calculateSphereRadiusFromVolume(float volume)
{
    return powf(3 * volume / (4 * M_PI), 1.0f / 3.0f);
}