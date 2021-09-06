//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file color.cpp
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "color.h"

void setDrawColor(SDL_Renderer* renderer, SDL_Color color)
{
    assert(renderer);
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}