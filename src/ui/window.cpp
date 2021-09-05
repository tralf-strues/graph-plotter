//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file window.cpp
//! @date 2021-09-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "window.h"

WindowError createTexture(Window* window); 

WindowError createWindow(Window* window, size_t width, size_t height, const char* title)
{
    assert(window);
    assert(width  != 0);
    assert(height != 0);
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return WINDOW_SDL_INIT_ERROR;
    }

    window->pixels = (uint32_t*) calloc(width * height, sizeof(uint32_t));
    if (window->pixels == nullptr)
    {
        return WINDOW_PIXELS_ALLOC_ERROR;
    }

    window->width  = width;
    window->height = height;
    updateTitle(window, title);

    window->sdlWindow = SDL_CreateWindow(window->title,
                                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         width, height,
                                         0);
    if (window->sdlWindow == nullptr)
    {
        return WINDOW_CREATE_ERROR;
    }

    window->sdlRenderer = SDL_CreateRenderer(window->sdlWindow, -1, 
                                             SDL_RENDERER_ACCELERATED);
    if (window->sdlRenderer == nullptr)
    {
        return WINDOW_CREATE_ERROR;
    }

    if (createTexture(window) != WINDOW_NO_ERROR)
    {
        return WINDOW_CREATE_ERROR;
    }

    return WINDOW_NO_ERROR;
}

WindowError createTexture(Window* window)
{
    assert(window);
    assert(window->sdlRenderer);
    assert(window->width);
    assert(window->height);

    window->sdlTexture = SDL_CreateTexture(window->sdlRenderer,
                                           SDL_PIXELFORMAT_RGBA8888, 
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window->width, 
                                           window->height);
    
    if (window->sdlTexture == nullptr)
    {
        return WINDOW_CREATE_ERROR;
    }

    return WINDOW_NO_ERROR;
}

void closeWindow(Window* window)
{
    ASSERT_WINDOW(window);

    window->width  = 0;
    window->height = 0;
    window->title  = nullptr;

    free(window->pixels);
    window->pixels = nullptr;

    SDL_DestroyTexture(window->sdlTexture);
    window->sdlTexture = nullptr;

    SDL_DestroyRenderer(window->sdlRenderer);
    window->sdlRenderer = nullptr;

    SDL_DestroyWindow(window->sdlWindow);
    window->sdlWindow = nullptr;

    SDL_Quit();
}

void updateTitle(Window* window, const char* title)
{
    assert(window);
    
    window->title  = (title == nullptr) ? WINDOW_DEFAULT_TITLE : title;

    if (window->sdlWindow != nullptr)
    {
        SDL_SetWindowTitle(window->sdlWindow, window->title);
    }
}

WindowError resizeWindow(Window* window, size_t width, size_t height)
{
    ASSERT_WINDOW(window);

    window->width  = width;
    window->height = height;

    if (window->pixels != nullptr)
    {
        uint32_t* newData = (uint32_t*) realloc(window->pixels, width * height * sizeof(uint32_t)); 
        if (newData == nullptr)
        {
            return WINDOW_PIXELS_ALLOC_ERROR;
        }

        window->pixels = newData;
    }

    if (window->sdlRenderer != nullptr)
    {
        SDL_DestroyTexture(window->sdlTexture);
        
        if (createTexture(window) != WINDOW_NO_ERROR)
        {
            return WINDOW_RESIZE_ERROR;
        }
    }

    SDL_SetWindowSize(window->sdlWindow, (int) width, (int) height);

    return WINDOW_NO_ERROR;
}

void setDrawColor(Window* window, ColorRGBA color)
{
    ASSERT_WINDOW(window);

    SDL_SetRenderDrawColor(window->sdlRenderer, getR(color), getG(color), getB(color), getA(color));
}

void clearWindow(Window* window)
{
    ASSERT_WINDOW(window);

    SDL_RenderClear(window->sdlRenderer);
}

void updateWindow(Window* window)
{
    ASSERT_WINDOW(window);

    // SDL_UpdateTexture(window->sdlTexture, 
    //                   nullptr, 
    //                   (void*) window->pixels, 
    //                   window->width * sizeof(uint32_t)); // TODO: sizeof(uint32_t) = 4 for ARGB

    // SDL_RenderCopy(window->sdlRenderer, window->sdlTexture, nullptr, nullptr);
    // SDL_RenderPresent(window->sdlRenderer);

    SDL_RenderPresent(window->sdlRenderer);
}