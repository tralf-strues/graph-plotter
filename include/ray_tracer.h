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

#include "sml/sml_graphics_wrapper.h"
#include "scene.h"
#include "zbuffer.h"

struct RayTracer
{
    RayTracer(Scene* scene = nullptr, BufferedTexture* targetTexture = nullptr, ZBuffer* zbuffer = nullptr);

    Scene*           scene;  // FIXME: make const (add const iterators)
    BufferedTexture* targetTexture;
    ZBuffer*         zbuffer;

    void renderScene();
};

#endif // RAY_TRACER_H