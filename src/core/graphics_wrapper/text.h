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
    TTF_Font* m_NativeFont = nullptr;
    size_t    m_FontSize   = 0;
};

class Text
{
public:
    ~Text();

    void load(Renderer& renderer, const char* str, Font font, Color color = COLOR_BLACK);
    void destroy();

    const char* getStr() const;
    const Font& getFont() const;
    Color getColor() const;
    size_t getWidth() const;
    size_t getHeight() const;

    void render(Renderer& renderer, const Vec2<int32_t>& pos) const;

private:
    const char*  m_Str     = nullptr;
    Font         m_Font    = {nullptr, 0};
    Color        m_Color   = 0;

    size_t       m_Width   = 0;
    size_t       m_Height  = 0;

    SDL_Texture* m_Texture = nullptr;
};

#endif // TEXT_H