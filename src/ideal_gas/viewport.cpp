//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file viewport.cpp
//! @date 2021-09-27
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "viewport.h"

float Viewport::getRelativeWidth() const
{
    return axesMax.x - axesMin.x;
}

float Viewport::getRelativeHeight() const
{
    return axesMax.y - axesMin.y;
}

Vec2<float> Viewport::toPixels(const Renderer& renderer, const Vec2<float>& point) const
{
    float relWidth  = getRelativeWidth();
    float relHeight = getRelativeHeight();

    float x = (renderer.getWindow().getWidth() / relWidth)  * (point.x - axesMin.x); 
    float y = renderer.getWindow().getHeight() + 
              (renderer.getWindow().getHeight() / relHeight) * (point.y - axesMax.y); 

    return {x, y};
}

float Viewport::toPixels(const Renderer& renderer, float distance) const
{
    return length(toPixels(renderer, Vec2<float>{1, 0} * distance));
}