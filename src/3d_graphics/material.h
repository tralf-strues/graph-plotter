//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file material.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/math/_core_math.h"

struct Material
{
    Vec3<float> ambient;
    Vec3<float> diffuse;
    Vec3<float> specular;

    float       shiness;
};

#endif // MATERIAL_H