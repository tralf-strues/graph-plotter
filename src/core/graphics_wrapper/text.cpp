//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "text.h"

Font::Font(const char* filename, size_t size) : m_FontSize(size)
{
    assert(filename);
    assert(size);

    m_NativeFont = TTF_OpenFont(filename, size);
    assert(m_NativeFont);
}

Font::Font() : m_NativeFont(nullptr), m_FontSize(0) {}

TTF_Font* Font::getNativeFont() const
{
    return m_NativeFont;
}

size_t Font::getSize() const
{
    return m_FontSize;
}

const char* Text::getStr() const
{
    return m_Str;
}

const Font& Text::getFont() const
{
    return m_Font;
}

Color Text::getColor() const
{
    return m_Color;
}

size_t Text::getWidth() const
{
    return m_Width;
}

size_t Text::getHeight() const
{
    return m_Height;
}

Text::Text(Renderer& renderer, const char* str, Font font, Color color)
    : m_Str(str), m_Font(font), m_Color(color)
{
    assert(m_Str);

    SDL_Surface* loadedSurface = TTF_RenderText_Blended(m_Font.getNativeFont(), 
                                                        m_Str, 
                                                        getSystemColor(color));

    assert(loadedSurface);

    m_Texture = SDL_CreateTextureFromSurface(renderer.getNativeRenderer(), loadedSurface);
    assert(m_Texture);
    
    m_Width  = loadedSurface->w;
    m_Height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
}

void Text::render(Renderer& renderer, const Vec2<int32_t>& pos) const
{
    SDL_Rect renderQuad = {pos.x, pos.y, 
                           static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height)};

	SDL_RenderCopy(renderer.getNativeRenderer(), m_Texture, nullptr, &renderQuad); 
}