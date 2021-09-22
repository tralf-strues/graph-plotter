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

private:
    size_t       width;
    size_t       height;
    Color*       pixels;

    SDL_Texture* nativeTexture;
};

#endif // TEXTURE_H