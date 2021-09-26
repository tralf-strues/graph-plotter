//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file primitives.cpp
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "primitives.h"

void renderRect(Renderer& renderer, const Rectangle& rect)
{
    Vec2<int32_t> upperLeftCorner(rect.pos.x, rect.pos.y);
    Vec2<int32_t> upperRightCorner(rect.pos.x + rect.width - 1, rect.pos.y);
    
    Vec2<int32_t> bottomLeftCorner(rect.pos.x, rect.pos.y + rect.height - 1);
    Vec2<int32_t> bottomRightCorner(rect.pos.x + rect.width - 1, rect.pos.y + rect.height - 1);

    // Top line
    renderLine(renderer, upperLeftCorner, upperRightCorner);

    // Bottom line
    renderLine(renderer, bottomLeftCorner, bottomRightCorner);

    // Left line
    renderLine(renderer, upperLeftCorner, bottomLeftCorner);

    // Right line
    renderLine(renderer, upperRightCorner, bottomRightCorner);
}

void renderCircle(Renderer& renderer, const Circle& circle)
{
    int32_t diameter = 2 * circle.radius;

    int32_t x     = (circle.radius - 1);
    int32_t y     = 0;
    int32_t tx    = 1;
    int32_t ty    = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        renderPoint(renderer, {circle.center.x + x, circle.center.y - y});
        renderPoint(renderer, {circle.center.x + x, circle.center.y + y});
        renderPoint(renderer, {circle.center.x - x, circle.center.y - y});
        renderPoint(renderer, {circle.center.x - x, circle.center.y + y});
        renderPoint(renderer, {circle.center.x + y, circle.center.y - x});
        renderPoint(renderer, {circle.center.x + y, circle.center.y + x});
        renderPoint(renderer, {circle.center.x - y, circle.center.y - x});
        renderPoint(renderer, {circle.center.x - y, circle.center.y + x});

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty    += 2;
        }

        if (error > 0)
        {
            --x;
            tx    += 2;
            error += (tx - diameter);
        }
    }
}