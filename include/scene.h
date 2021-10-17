//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file scene.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SCENE_H
#define SCENE_H

#include "sml/sml_math.h"
#include "sml/sml_containers.h"
#include "light.h"
#include "object3d.h"
#include "mesh.h"
#include "camera.h"

struct Scene
{
    Scene(Camera& camera) : camera(camera) {}

    Camera&              camera;
    DynamicArray<Light*> lightSources;
    Vec3<float>          ambientColor; ///< Global ambient used in Blinn-Phong shading
    List<Object3d*>      objects;
    List<Mesh*>          meshes;

    void updateWorldSpaceValues();
    void updateCameraSpaceValues();
};

#endif // SCENE_H