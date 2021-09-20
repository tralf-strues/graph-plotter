//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file scene.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SCENE_H
#define SCENE_H

#include "../core/math/_core_math.h"
#include "../core/containers/dynamic_array.h"
#include "light.h"
#include "entity.h"
#include "camera.h"

struct Scene
{
    Scene(Camera& camera) : camera(camera) {}

    Camera&               camera;
    DynamicArray<Light*>  lightSources;
    DynamicArray<Entity*> entities;
    Vec3<float>           ambientColor; ///< Global ambient used in Blinn-Phong shading

    void updateWorldSpaceValues();
    void updateCameraSpaceValues();
};

#endif // SCENE_H