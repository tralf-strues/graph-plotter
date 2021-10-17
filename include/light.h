//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file light.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef LIGHT_H
#define LIGHT_H

#include "sml/sml_math.h"
#include "space_dep_value.h"

struct Light : public SpaceDependent
{
    SpaceDepValue<Vec3<float>> pos;

    float brightness;

    /* Blinn-Phong shading specification */
    Vec3<float> diffuse;
    Vec3<float> specular;

    /* SpaceDependent */
    virtual void toWorldSpace() override;
    virtual void toCameraSpace(const Camera& camera) override;
};

#endif // LIGHT_H