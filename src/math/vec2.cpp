//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file vec2.cpp
//! @date 2021-09-06
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include "vec2.h"

// Vector 2D with type float
#define VEC2_NAME Vec2f32
#define VEC2_TYPE float
#include "vec2BuilderDef.h"
#undef VEC2_NAME
#undef VEC2_TYPE

// Vector 2D with type int32_t
#define VEC2_NAME Vec2i32
#define VEC2_TYPE int32_t
#include "vec2BuilderDef.h"
#undef VEC2_NAME
#undef VEC2_TYPE

// Vector 2D with type uint32_t
#define VEC2_NAME Vec2u32
#define VEC2_TYPE uint32_t
#include "vec2BuilderDef.h"
#undef VEC2_NAME
#undef VEC2_TYPE
