//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file hit.h
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef HIT_H
#define HIT_H

#include "sml/sml_math.h"
#include "material.h"

struct Hit
{
    Vec3<float>     pos;          ///< Hit's position in camera space.
    float           rayParameter; ///< pos = ray.from + ray.direction * rayParameter
    Vec3<float>     normal;       ///< Normal to the surface at pos in camera space.
    const Material* material;     ///< Material of the surface having been hit.
};

class Hittable
{
public:
    virtual bool intersect(const Ray& ray, Hit* hit) = 0;
};

#endif // HIT_H