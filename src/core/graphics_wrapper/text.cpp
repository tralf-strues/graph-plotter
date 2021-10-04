//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "text.h"

Font::Font(Renderer& renderer, const char* filename, size_t size) : fontSize(size)
{
    assert(filename);
    assert(size);

    nativeFont = TTF_OpenFont(filename, size);
    assert(nativeFont);
}

Font::Font() : nativeFont(nullptr), fontSize(0) {}

TTF_Font* Font::getNativeFont() const
{
    return nativeFont;
}

size_t Font::getSize() const
{
    return fontSize;
}

const char* Text::getStr() const
{
    return str;
}

const Font& Text::getFont() const
{
    return font;
}

Color Text::getColor() const
{
    return color;
}

size_t Text::getWidth() const
{
    return width;
}

size_t Text::getHeight() const
{
    return height;
}

Text::Text(Renderer& renderer, const char* str, Font font, Color color)
    : str(str), font(font), color(color)
{
    assert(str);

    SDL_Surface* loadedSurface = TTF_RenderText_Blended(font.getNativeFont(), 
                                                        str, 
                                                        getSystemColor(color));

    assert(loadedSurface);

    texture = SDL_CreateTextureFromSurface(renderer.getNativeRenderer(), loadedSurface);
    assert(texture);
    
    width  = loadedSurface->w;
    height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
}

void Text::render(Renderer& renderer, const Vec2<int32_t>& pos) const
{
    SDL_Rect renderQuad = {pos.x, pos.y, static_cast<int32_t>(width), static_cast<int32_t>(height)};
	SDL_RenderCopy(renderer.getNativeRenderer(), texture, nullptr, &renderQuad); 
}