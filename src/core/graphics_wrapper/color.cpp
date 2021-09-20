//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file color.cpp
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "color.h"

uint8_t getByte(uint32_t number, uint8_t byteNumber);

Color rgbaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t) r << 24u) + 
           ((uint32_t) g << 16u) + 
           ((uint32_t) b << 8u ) + 
           (uint32_t) a;
}

uint8_t getByte(uint32_t number, uint8_t byteNumber)
{
    return (number & (0xFFu << (8u * byteNumber))) >> (8u * byteNumber);
}

uint8_t colorGetR(Color color)
{
    return getByte(color, 3u);
}

uint8_t colorGetG(Color color)
{
    return getByte(color, 2u);
}

uint8_t colorGetB(Color color)
{
    return getByte(color, 1u);
}

uint8_t colorGetA(Color color)
{
    return getByte(color, 0u);
}