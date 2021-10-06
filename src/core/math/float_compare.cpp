//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file float_compare.cpp
//! @date 2021-09-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <math.h>
#include "float_compare.h"

int32_t cmpFloat(float first, float second, float epsilon)
{
    if (fabsf(first - second) < epsilon)
    {
        return 0;
    }

    return (first < second) ? -1 : 1;
}