//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file color.cpp
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "color.h"

uint8_t getByte(ColorRGBA number, uint8_t byteNumber);

ColorRGBA getRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t) r << 24u) + 
           ((uint32_t) g << 16u) + 
           ((uint32_t) b << 8u ) + 
           (uint32_t) a;
}

uint8_t getByte(ColorRGBA number, uint8_t byteNumber)
{
    return (number & (0xFFu << (8u * byteNumber))) >> (8u * byteNumber);
}

uint8_t getR(ColorRGBA color)
{
    return getByte(color, 3u);
}

uint8_t getG(ColorRGBA color)
{
    return getByte(color, 2u);
}

uint8_t getB(ColorRGBA color)
{
    return getByte(color, 1u);
}

uint8_t getA(ColorRGBA color)
{
    return getByte(color, 0u);
}