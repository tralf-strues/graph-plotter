//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file float_compare.h
//! @date 2021-09-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef FLOAT_COMPARE_H
#define FLOAT_COMPARE_H

#include "inttypes.h"
#include <float.h>

int32_t cmpFloat(float first, float second, float epsilon = FLT_EPSILON);

#endif // FLOAT_COMPARE_H