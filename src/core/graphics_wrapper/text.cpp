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
    if (filename != nullptr)
    {
        m_NativeFont = TTF_OpenFont(filename, size);
        assert(m_NativeFont);
    }
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

Text::~Text()
{
    destroy();
}

const Font& Text::getFont()   const { return m_Font;   }
const char* Text::getStr()    const { return m_Str;    }
Color       Text::getColor()  const { return m_Color;  }
size_t      Text::getWidth()  const { return m_Width;  }
size_t      Text::getHeight() const { return m_Height; }

void Text::load(Renderer& renderer, const char* str, const Font& font, Color color)
{
    assert(str);

    destroy();

    m_Font  = font;
    m_Color = color;
    m_Str   = str;
    SDL_Color sysColor = getSystemColor(m_Color);

    SDL_Surface* loadedSurface = TTF_RenderText_Blended(m_Font.getNativeFont(), 
                                                        m_Str, 
                                                        getSystemColor(m_Color));
    assert(loadedSurface);

    m_Texture = SDL_CreateTextureFromSurface(renderer.getNativeRenderer(), loadedSurface);
    assert(m_Texture);
    
    m_Width  = loadedSurface->w;
    m_Height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
}

void Text::destroy()
{
    if (m_Texture != nullptr)
    {
        SDL_DestroyTexture(m_Texture);
    }
}

void Text::render(Renderer& renderer, const Vec2<int32_t>& pos) const
{
    SDL_Rect renderQuad = {pos.x, pos.y, 
                           static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height)};

	SDL_RenderCopy(renderer.getNativeRenderer(), m_Texture, nullptr, &renderQuad); 
}