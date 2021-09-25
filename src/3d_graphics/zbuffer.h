//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file zbuffer.h
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef ZBUFFER_H
#define ZBUFFER_H

#include <stdlib.h>

struct ZBuffer
{
    const size_t width;
    const size_t height;
    float*       depths;

    ZBuffer(size_t width, size_t height);
    ~ZBuffer();

    float getDepth(size_t x, size_t y) const;
    bool  setDepth(size_t x, size_t y, float depth);

    void  reset();
    bool  isFilled(size_t x, size_t y) const;
};

#endif // ZBUFFER_H