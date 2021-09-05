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

typedef uint32_t ColorRGBA;

static const ColorRGBA COLOR_WHITE  = 0xFF'FF'FF'FF;
static const ColorRGBA COLOR_BLACK  = 0x00'00'00'FF;
static const ColorRGBA COLOR_RED    = 0xFF'00'00'FF;
static const ColorRGBA COLOR_GREEN  = 0x00'FF'00'FF;
static const ColorRGBA COLOR_BLUE   = 0x00'00'FF'FF;
static const ColorRGBA COLOR_YELLOW = 0xFF'FF'00'FF;

ColorRGBA getRGBA (uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint8_t   getR    (ColorRGBA color);
uint8_t   getG    (ColorRGBA color);
uint8_t   getB    (ColorRGBA color);
uint8_t   getA    (ColorRGBA color);

#endif // COLOR_H