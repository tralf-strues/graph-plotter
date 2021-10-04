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
#include <SDL.h>

typedef uint32_t Color;

static const Color COLOR_WHITE  = 0xFF'FF'FF'FF;
static const Color COLOR_BLACK  = 0x00'00'00'FF;
static const Color COLOR_RED    = 0xFF'00'00'FF;
static const Color COLOR_GREEN  = 0x00'FF'00'FF;
static const Color COLOR_BLUE   = 0x00'00'FF'FF;
static const Color COLOR_YELLOW = 0xFF'FF'00'FF;

//------------------------------------------------------------------------------
//! @brief Creates Color from separate color chanels - rgba.
//! 
//! @param r Red value.
//! @param g Green value.
//! @param b Blue value.
//! @param a Alpha value.
//! 
//! @return Created rgba color. 
//------------------------------------------------------------------------------
Color rgbaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

//------------------------------------------------------------------------------
//! @brief Get the system level color object.
//! 
//! @param color
//! @return System level color. 
//------------------------------------------------------------------------------
SDL_Color getSystemColor(Color color);

//------------------------------------------------------------------------------
//! @param color
//! 
//! @return color's red chanel.
//------------------------------------------------------------------------------
uint8_t colorGetR(Color color);

//------------------------------------------------------------------------------
//! @param color
//! 
//! @return color's green chanel.
//------------------------------------------------------------------------------
uint8_t colorGetG(Color color);

//------------------------------------------------------------------------------
//! @param color
//! 
//! @return color's blue chanel.
//------------------------------------------------------------------------------
uint8_t colorGetB(Color color);

//------------------------------------------------------------------------------
//! @param color
//! 
//! @return color's alpha chanel.
//------------------------------------------------------------------------------
uint8_t colorGetA(Color color);

#endif // COLOR_H