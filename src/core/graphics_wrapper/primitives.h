//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file primitives.h
//! @date 2021-09-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "renderer.h"

struct Circle
{
    Vec2<int32_t> center;
    int32_t       radius;
};

struct InfLine
{
    Vec2<float> from;
    Vec2<float> direction;
};

void renderRect(Renderer& renderer, const Rectangle& rect);
void renderFilledRect(Renderer& renderer, const Rectangle& rect);
void renderCircle(Renderer& renderer, const Circle& circle);
void renderFilledCircle(Renderer& renderer, const Circle& circle);
void renderInfLine(Renderer& renderer, const InfLine& infLine);

#endif // PRIMITIVES_H