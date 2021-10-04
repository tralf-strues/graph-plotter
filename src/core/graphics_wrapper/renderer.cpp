//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file renderer.cpp
//! @date 2021-09-11
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "renderer.h"

Renderer::Renderer(Window& window) : m_Window(window)
{
    m_NativeRenderer = SDL_CreateRenderer(m_Window.getNativeWindow(), -1, SDL_RENDERER_ACCELERATED);
    if (m_NativeRenderer == nullptr)
    {
        setError(Renderer::CREATE_ERROR);
        return;
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_NativeRenderer);
}

Color Renderer::getColor() const
{
    return m_Color;
}

void Renderer::setColor(Color color)
{
    m_Color = color;
    SDL_SetRenderDrawColor(m_NativeRenderer, colorGetR(color), 
                                             colorGetG(color), 
                                             colorGetB(color), 
                                             colorGetA(color));
}

uint32_t Renderer::getError() const
{
    return m_ErrorStatus;  
}

Window& Renderer::getWindow() const
{
    return m_Window;
}

SDL_Renderer* Renderer::getNativeRenderer() const
{
    return m_NativeRenderer;
}

void Renderer::setError(uint32_t error)
{
    m_ErrorStatus |= error;
}

void Renderer::present() const
{
    SDL_RenderPresent(m_NativeRenderer);
}

void Renderer::clear()
{
    SDL_RenderClear(m_NativeRenderer);
}

void Renderer::setClipRegion(const Rectangle& clipRegion) const
{
    SDL_Rect rect = {clipRegion.pos.x, clipRegion.pos.y, clipRegion.width, clipRegion.height};

    SDL_RenderSetClipRect(m_NativeRenderer, &rect);
}

void Renderer::resetClipRegion() const
{
    SDL_RenderSetClipRect(m_NativeRenderer, nullptr);
}

void Renderer::renderPoint(const Vec2<int32_t>& pos)
{
    SDL_RenderDrawPoint(m_NativeRenderer, pos.x, pos.y);
}

void Renderer::renderLine(const Vec2<int32_t>& start, const Vec2<int32_t>& end)
{
    SDL_RenderDrawLine(m_NativeRenderer, start.x, start.y, end.x, end.y);
}

void Renderer::renderTexture(const Texture& texture, const Vec2<int32_t>& pos)
{
    SDL_Rect destRect = {pos.x, pos.y, (int) texture.getWidth(), (int) texture.getHeight()}; 
    SDL_RenderCopy(m_NativeRenderer, texture.getNativeTexture(), &destRect, nullptr);
}

void renderPoint(Renderer& renderer, const Vec2<int32_t>& pos)
{
    renderer.renderPoint(pos);
}

void renderLine(Renderer& renderer, const Vec2<int32_t>& start, const Vec2<int32_t>& end)
{
    renderer.renderLine(start, end);
}

void renderTexture(Renderer& renderer, const Texture& texture, const Vec2<int32_t>& pos)
{
    renderer.renderTexture(texture, pos);
}