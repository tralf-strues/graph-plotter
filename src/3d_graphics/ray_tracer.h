//------------------------------------------------------------------------------
//! @brief Basic ray-tracer.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ray_tracer.h
//! @date 2021-09-16
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "scene.h"

void renderSceneRayTracing(Renderer& renderer, Scene& scene);

#endif // RAY_TRACER_H