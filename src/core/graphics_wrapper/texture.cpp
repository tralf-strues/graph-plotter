//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file texture.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "texture.h"
#include "renderer.h"

Texture::Texture(Renderer& renderer, size_t width, size_t height) : width(width), height(height) 
{
    assert(width);
    assert(height);

    pixels = new Color[width * height];
    nativeTexture = SDL_CreateTexture(renderer.getNativeRenderer(),
                                      SDL_PIXELFORMAT_RGBA8888, 
                                      SDL_TEXTUREACCESS_STREAMING,
                                      width, height);
}

Texture::~Texture()
{
    delete[] pixels;
}

size_t Texture::getWidth() const
{
    return width;
}

size_t Texture::getHeight() const
{
    return height;
}

Color* Texture::getPixels() const
{
    return pixels;
}

SDL_Texture* Texture::getNativeTexture() const
{
    return nativeTexture;
}

Color* Texture::operator[](size_t row)
{
    if (row >= height)
    {
        assert(row < height);
    }
    return &pixels[row * width];    
    // return const_cast<Color*>((*(const Texture*)this)[row]); 
}

const Color* Texture::operator[](size_t row) const
{
    assert(row < height);

    return &pixels[row * width];     
}

void Texture::clear(Color color)
{
    size_t pixelsCount = height * width;

    for (size_t i = 0; i < pixelsCount; i++)
    {
        pixels[i] = color;
    }
}

void Texture::update()
{
    SDL_UpdateTexture(nativeTexture, nullptr, (void*) pixels, width * sizeof(Color));
}