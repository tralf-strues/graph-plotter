//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text.h
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef TEXT_H
#define TEXT_H

#include <SDL_ttf.h>
#include "renderer.h"

class Font
{
public:
    Font(Renderer& renderer, const char* filename, size_t size);
    Font();

    TTF_Font* getNativeFont() const;
    size_t getSize() const;

private:
    TTF_Font* nativeFont;
    size_t    fontSize;
};

class Text
{
public:
    Text(Renderer& renderer, const char* str, Font font, Color color = COLOR_BLACK);

    const char* getStr() const;
    const Font& getFont() const;
    Color getColor() const;
    size_t getWidth() const;
    size_t getHeight() const;

    void render(Renderer& renderer, const Vec2<int32_t>& pos) const;

private:
    const char* str;
    Font        font;
    Color       color;

    size_t      width;
    size_t      height;

    SDL_Texture* texture;
};

#endif // TEXT_H