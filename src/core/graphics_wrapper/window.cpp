//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file window.cpp
//! @date 2021-09-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "window.h"

Window::Window(size_t width, size_t height, const char* title) : width(width), height(height) 
{
    assert(width);
    assert(height);

    nativeWindow = SDL_CreateWindow(title, 
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    width, height, 0);

    if (nativeWindow == nullptr)
    {
        setError(CREATE_ERROR);
        return;
    }
}

Window::~Window()
{
    SDL_DestroyWindow(nativeWindow);
}

size_t Window::getWidth() const 
{ 
    return width;       
}

size_t Window::getHeight() const 
{ 
    return height;      
}

const char* Window::getTitle() const 
{ 
    return title;       
}

uint32_t Window::getError() const 
{ 
    return errorStatus; 
}

void Window::updateTitle(const char* title)
{
    this->title  = (title == nullptr) ? WINDOW_DEFAULT_TITLE : title;

    SDL_SetWindowTitle(nativeWindow, this->title);
}

void Window::resizeWindow(size_t width, size_t height)
{
    assert(width);
    assert(height);

    this->width  = width;
    this->height = height;

    SDL_SetWindowSize(nativeWindow, (int) width, (int) height);
}

void Window::setError(uint32_t error)
{
    errorStatus |= error;
}

bool initGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        return false;
    }

    return true;
}

void quitGraphics()
{
    SDL_Quit();
}

// FIXME:
// void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, Rectangle renderRect)
// {
//     assert(renderer);
//     assert(texture);

//     SDL_Rect renderQuad = {renderRect.x, renderRect.y, renderRect.width, renderRect.height};
// 	SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);   
// }