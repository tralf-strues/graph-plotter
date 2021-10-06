//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file viewport.cpp
//! @date 2021-09-27
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "viewport.h"

Viewport::Viewport(const Vec2<float>& axesMin, 
                   const Vec2<float>& axesMax,
                   const Rectangle& windowArea) :
                   axesMin(axesMin),
                   axesMax(axesMax),
                   windowArea(windowArea)
{}

float Viewport::getRelativeWidth() const
{
    return axesMax.x - axesMin.x;
}

float Viewport::getRelativeHeight() const
{
    return axesMax.y - axesMin.y;
}

Vec2<float> Viewport::toPixels(const Vec2<float>& point) const
{
    float relWidth  = getRelativeWidth();
    float relHeight = getRelativeHeight();

    float x = windowArea.pos.x + ((windowArea.width - 1) / relWidth)  * (point.x - axesMin.x); 
    float y = windowArea.pos.y + 
              ((windowArea.height - 1) / relHeight) * (axesMax.y - point.y); 

    return {x, y};
}

float Viewport::toPixels(float distance) const
{
    return (float) windowArea.width * distance / getRelativeWidth();
}