//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file primitives.cpp
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "primitives.h"

void drawLine(Window* window, Vec2u32 start, Vec2u32 end)
{
    ASSERT_WINDOW(window);
    assert(start.x < window->width);
    assert(start.y < window->height);
    assert(start.x < window->width);
    assert(start.y < window->height);

    SDL_RenderDrawLine(window->sdlRenderer, start.x, start.y, end.x, end.y);
}