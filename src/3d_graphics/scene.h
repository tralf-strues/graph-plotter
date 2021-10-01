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
#include "../core/containers/_core_containers.h"
#include "light.h"
#include "primitive3d.h"
#include "camera.h"

struct Scene
{
    Scene(Camera& camera) : camera(camera) {}

    Camera&               camera;
    DynamicArray<Light*>  lightSources;
    Vec3<float>           ambientColor; ///< Global ambient used in Blinn-Phong shading
    List<Primitive3d*>    primitives;
    // List<Mesh*>        meshes;

    void updateWorldSpaceValues();
    void updateCameraSpaceValues();
};

#endif // SCENE_H