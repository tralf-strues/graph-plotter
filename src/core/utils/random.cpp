//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file random.cpp
//! @date 2021-10-02
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "random.h"

bool randomTry(float probability)
{
    return ((rand() % (uint32_t) 1e6) / 1e6) <= probability;
}