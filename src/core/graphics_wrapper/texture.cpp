//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file texture.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "texture.h"
#include "renderer.h"

Texture::Texture() : m_Width(0), m_Height(0), m_Pixels(nullptr), m_NativeTexture(nullptr) {}

Texture::Texture(Renderer& renderer, size_t width, size_t height)
    : m_Width(width), m_Height(height) 
{
    assert(m_Width);
    assert(m_Height);

    m_Pixels = new Color[m_Width * m_Height];
    m_NativeTexture = SDL_CreateTexture(renderer.getNativeRenderer(),
                                        SDL_PIXELFORMAT_RGBA8888, 
                                        SDL_TEXTUREACCESS_STREAMING,
                                        m_Width, m_Height);
}

Texture::~Texture()
{
    delete[] m_Pixels;
}

size_t Texture::getWidth() const
{
    return m_Width;
}

size_t Texture::getHeight() const
{
    return m_Height;
}

Color* Texture::getPixels() const
{
    return m_Pixels;
}

SDL_Texture* Texture::getNativeTexture() const
{
    return m_NativeTexture;
}

Color* Texture::operator[](size_t row)
{
    assert(row < m_Height);

    return &m_Pixels[row * m_Width];    
}

const Color* Texture::operator[](size_t row) const
{
    assert(row < m_Height);

    return &m_Pixels[row * m_Width];     
}

void Texture::clear(Color color)
{
    size_t pixelsCount = m_Height * m_Width;

    for (size_t i = 0; i < pixelsCount; i++)
    {
        m_Pixels[i] = color;
    }
}

void Texture::update()
{
    SDL_UpdateTexture(m_NativeTexture, nullptr, (void*) m_Pixels, m_Width * sizeof(Color));
}

bool Texture::writeToBMP(const char* filename) const
{
    assert(filename);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < m_Width * m_Height; i++)
    {
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 0] = colorGetB(m_Pixels[i]);
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 1] = colorGetG(m_Pixels[i]);
        ((uint8_t*)surface->pixels)[i * sizeof(Color) + 2] = colorGetR(m_Pixels[i]);
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

    if (m_Pixels != nullptr)
    {
        delete[] m_Pixels;
    }

    m_Width         = surface->w;
    m_Height        = surface->h;
    m_NativeTexture = texture;

    m_Pixels = new Color[m_Width * m_Height];

    for (size_t i = 0; i < m_Width * m_Height; i++)
    {
        m_Pixels[i] = rgbaColor(((uint8_t*)surface->pixels)[i * 3 + 2],
                                ((uint8_t*)surface->pixels)[i * 3 + 1],
                                ((uint8_t*)surface->pixels)[i * 3 + 0],
                                0xFF);
    }

    return true;
}