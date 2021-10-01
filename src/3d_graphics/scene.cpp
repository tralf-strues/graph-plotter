//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file scene.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "scene.h"

void Scene::updateWorldSpaceValues()
{
    for (auto light : lightSources)
    {
        light->toWorldSpace();
    }

    for (auto primitive : primitives)
    {
        primitive->toWorldSpace();
    }
}

void Scene::updateCameraSpaceValues()
{
    for (auto light : lightSources)
    {
        light->toCameraSpace(camera);
    }

    for (auto primitive : primitives)
    {
        primitive->toCameraSpace(camera);
    }
}