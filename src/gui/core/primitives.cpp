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