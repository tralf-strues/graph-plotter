//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec2.h
//! @date 2021-09-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef VEC2_H
#define VEC2_H

#include <stdint.h>

// Vector 2D with type double
#define VEC2_NAME Vec2f64
#define VEC2_TYPE double
#include "vec2Builder.h"

// Vector 2D with type int32_t
#define VEC2_NAME Vec2i32
#define VEC2_TYPE int32_t
#include "vec2Builder.h"

// Vector 2D with type uint32_t
#define VEC2_NAME Vec2u32
#define VEC2_TYPE uint32_t
#include "vec2Builder.h"

#endif // VEC2_H