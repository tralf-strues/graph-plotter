//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file texture.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "texture.h"
#include "renderer.h"

Texture::Texture() : width(0), height(0), pixels(nullptr), nativeTexture(nullptr) {}

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
    assert(row < height);

    return &pixels[row * width];    
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

bool Texture::writeToBMP(const char* filename) const
{
    assert(filename);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < width * height; i++)
    {
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 0] = colorGetB(pixels[i]);
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 1] = colorGetG(pixels[i]);
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 2] = colorGetR(pixels[i]);
    }

    if (SDL_SaveBMP(surface, filename) != 0)
    {
        return false;
    }

    SDL_FreeSurface(surface);

    return true;
}

bool Texture::loadFromBMP(Renderer& renderer, const char* filename)
{
    assert(filename);

    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (surface == nullptr)
    {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getNativeRenderer(), surface);
    if (texture == nullptr)
    {
        return false;
    }

    if (pixels != nullptr)
    {
        delete[] pixels;
    }

    width         = surface->w;
    height        = surface->h;
    nativeTexture = texture;

    pixels = new Color[width * height];

    for (size_t i = 0; i < width * height; i++)
    {
        pixels[i] = rgbaColor(((uint8_t*)surface->pixels)[i * 3 + 2],
                              ((uint8_t*)surface->pixels)[i * 3 + 1],
                              ((uint8_t*)surface->pixels)[i * 3 + 0],
                              0xFF);
    }

    return true;
}