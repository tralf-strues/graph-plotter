//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file zbuffer.cpp
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "zbuffer.h"

ZBuffer::ZBuffer(size_t width, size_t height) : width(width), height(height)
{
    depths = new float[width * height]{0.0f};
}

ZBuffer::~ZBuffer()
{
    delete[] depths;
}

float ZBuffer::getDepth(size_t x, size_t y) const
{
    return depths[y * width + x];
}

bool ZBuffer::setDepth(size_t x, size_t y, float depth)
{
    size_t pos = y * width + x;

    if (depths[pos] > depth || depths[pos] == 0.0f)
    {
        depths[pos] = depth;
        return true;
    }

    return false;
}

void ZBuffer::reset()
{
    for (size_t i = 0; i < width * height; i++)
    {
        depths[i] = 0.0f;
    }
}

bool ZBuffer::isFilled(size_t x, size_t y) const
{
    return getDepth(x, y) != 0.0f;
}