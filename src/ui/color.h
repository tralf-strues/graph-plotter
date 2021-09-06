//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file color.h
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include "SDL.h"

static const SDL_Color COLOR_WHITE  = {0xFF, 0xFF, 0xFF, 0xFF};
static const SDL_Color COLOR_BLACK  = {0x00, 0x00, 0x00, 0xFF};
static const SDL_Color COLOR_RED    = {0xFF, 0x00, 0x00, 0xFF};
static const SDL_Color COLOR_GREEN  = {0x00, 0xFF, 0x00, 0xFF};
static const SDL_Color COLOR_BLUE   = {0x00, 0x00, 0xFF, 0xFF};
static const SDL_Color COLOR_YELLOW = {0xFF, 0xFF, 0x00, 0xFF};

void setDrawColor(SDL_Renderer* renderer, SDL_Color color);

#endif // COLOR_H