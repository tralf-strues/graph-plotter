//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file window.cpp
//! @date 2021-09-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "window.h"

WindowError createWindow(Window* window, size_t width, size_t height, const char* title)
{
    assert(window);
    assert(width  != 0);
    assert(height != 0);
    
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("[ERROR] SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return WINDOW_SDL_INIT_ERROR;
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

    return WINDOW_NO_ERROR;
}

void closeWindow(Window* window)
{
    ASSERT_WINDOW(window);

    window->width  = 0;
    window->height = 0;
    window->title  = nullptr;

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

    SDL_SetWindowSize(window->sdlWindow, (int) width, (int) height);

    return WINDOW_NO_ERROR;
}

void setDrawColor(SDL_Renderer* renderer, ColorRGBA color)
{
    assert(renderer);

    SDL_SetRenderDrawColor(renderer, getR(color), getG(color), getB(color), getA(color));
}

void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, Vec2i32 pos, Vec2i32 size)
{
    assert(renderer);
    assert(texture);

    SDL_Rect renderQuad = {pos.x, pos.y, size.x, size.y};
	SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);   
}