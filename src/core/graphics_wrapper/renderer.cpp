//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file renderer.cpp
//! @date 2021-09-11
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "renderer.h"

Renderer::Renderer(Window& window) : window(window)
{
    nativeRenderer = SDL_CreateRenderer(window.nativeWindow, -1, SDL_RENDERER_ACCELERATED);
    if (nativeRenderer == nullptr)
    {
        setError(Renderer::CREATE_ERROR);
        return;
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(nativeRenderer);
}

Color Renderer::getColor() const
{
    return color;
}

void Renderer::setColor(Color color)
{
    this->color = color;
    SDL_SetRenderDrawColor(nativeRenderer, colorGetR(color), 
                                           colorGetG(color), 
                                           colorGetB(color), 
                                           colorGetA(color));
}

uint32_t Renderer::getError() const
{
    return errorStatus;  
}

Window& Renderer::getWindow() const
{
    return window;
}

void Renderer::setError(uint32_t error)
{
    errorStatus |= error;
}

void Renderer::present() const
{
    SDL_RenderPresent(nativeRenderer);
}

void Renderer::clear()
{
    SDL_RenderClear(nativeRenderer);
}

void Renderer::setClipRegion(const Rectangle& clipRegion) const
{
    SDL_Rect rect = {clipRegion.pos.x, clipRegion.pos.y, clipRegion.width, clipRegion.height};

    SDL_RenderSetClipRect(nativeRenderer, &rect);
}

void Renderer::resetClipRegion() const
{
    SDL_RenderSetClipRect(nativeRenderer, nullptr);
}

void Renderer::renderPoint(const Vec2<int32_t>& pos)
{
    SDL_RenderDrawPoint(nativeRenderer, pos.x, pos.y);
}

void Renderer::renderLine(const Vec2<int32_t>& start, const Vec2<int32_t>& end)
{
    SDL_RenderDrawLine(nativeRenderer, start.x, start.y, end.x, end.y);
}

void renderPoint(Renderer& renderer, const Vec2<int32_t>& pos)
{
    renderer.renderPoint(pos);
}

void renderLine(Renderer& renderer, const Vec2<int32_t>& start, const Vec2<int32_t>& end)
{
    renderer.renderLine(start, end);
}