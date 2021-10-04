//------------------------------------------------------------------------------
//! @brief Basic texture management.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file texture.h
//! @date 2021-09-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include "color.h"
#include "../math/_core_math.h"

class Renderer;

class Texture
{
public:
    Texture();
    Texture(Renderer& renderer, size_t width, size_t height);
    ~Texture();

    size_t       getWidth()         const;
    size_t       getHeight()        const;
    Color*       getPixels()        const;
    SDL_Texture* getNativeTexture() const;

    Color* operator[](size_t row);
    const Color* operator[](size_t row) const;

    void clear(Color color);
    void update();

    bool writeToBMP(const char* filename) const;
    bool loadFromBMP(Renderer& renderer, const char* filename);

private:
    size_t       m_Width;
    size_t       m_Height;
    Color*       m_Pixels;

    SDL_Texture* m_NativeTexture;
};

#endif // TEXTURE_H