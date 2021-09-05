//------------------------------------------------------------------------------
//! @brief Basic window management using SDL2.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file window.h
//! @date 2021-09-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H

#include <assert.h>
#include <stdio.h>
#include <SDL.h>

#include "color.h"

#define ASSERT_WINDOW(window) assert(window);              \
                              assert(window->width);       \
                              assert(window->height);      \
                              assert(window->title);       \
                              assert(window->pixels);      \
                              assert(window->sdlWindow);   \
                              assert(window->sdlRenderer); \
                              assert(window->sdlTexture);  

enum WindowError
{
    WINDOW_NO_ERROR,
    WINDOW_SDL_INIT_ERROR,
    WINDOW_PIXELS_ALLOC_ERROR,
    WINDOW_CREATE_ERROR,
    WINDOW_RESIZE_ERROR
};

struct Window
{
    size_t        width;
    size_t        height;
    const char*   title;

    uint32_t*     pixels; // TODO: make a typedef RGBA

    SDL_Window*   sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture*  sdlTexture;
};

static const char* WINDOW_DEFAULT_TITLE = "Untitled";

WindowError createWindow(Window* window, size_t width, size_t height, const char* title);

// NOTE: don't allow usage of all the functions below before a successful call of initWindow
// NOTE: exceptions: updateTitle, 
void closeWindow(Window* window);

void updateTitle(Window* window, const char* title);
WindowError resizeWindow(Window* window, size_t width, size_t height);

void setDrawColor(Window* window, ColorRGBA color);
void clearWindow(Window* window);
void updateWindow(Window* window);

#endif // WINDOW_H