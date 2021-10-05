//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file viewport.h
//! @date 2021-09-27
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../graphics_wrapper/_core_graphics_wrapper.h"

struct Viewport
{
    Viewport(const Vec2<float>& axesMin, const Vec2<float>& axesMax, const Rectangle& windowArea);

    Vec2<float> axesMin;
    Vec2<float> axesMax;

    Rectangle   windowArea;

    float getRelativeWidth()  const;
    float getRelativeHeight() const;

    Vec2<float> toPixels(const Vec2<float>& point) const;
    float       toPixels(float distance) const;
};

#endif // VIEWPORT_H