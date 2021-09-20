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
    for (size_t i = 0; i < lightSources.getSize(); i++)
    {
        lightSources[i]->toWorldSpace();
    }

    for (size_t i = 0; i < entities.getSize(); i++)
    {
        entities[i]->toWorldSpace();
    }
}

void Scene::updateCameraSpaceValues()
{
    for (size_t i = 0; i < lightSources.getSize(); i++)
    {
        lightSources[i]->toCameraSpace(camera);
    }

    for (size_t i = 0; i < entities.getSize(); i++)
    {
        entities[i]->toCameraSpace(camera);
    }
}