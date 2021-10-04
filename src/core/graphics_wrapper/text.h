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
    Font(const char* filename, size_t size);
    Font();

    TTF_Font* getNativeFont() const;
    size_t getSize() const;

private:
    TTF_Font* m_NativeFont;
    size_t    m_FontSize;
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
    const char*  m_Str;
    Font         m_Font;
    Color        m_Color;

    size_t       m_Width;
    size_t       m_Height;

    SDL_Texture* m_Texture;
};

#endif // TEXT_H