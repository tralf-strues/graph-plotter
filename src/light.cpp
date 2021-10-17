//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file light.cpp
//! @date 2021-09-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "light.h"
#include "camera.h"

void Light::toWorldSpace() {}

void Light::toCameraSpace(const Camera& camera)
{
    pos.cameraSpace = camera.getViewMatrix() * pos.worldSpace;
}