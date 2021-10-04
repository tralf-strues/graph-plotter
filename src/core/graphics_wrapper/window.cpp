//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file window.cpp
//! @date 2021-09-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include <SDL_ttf.h>
#include "window.h"

Window::Window(size_t width, size_t height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title) 
{
    assert(m_Width);
    assert(m_Height);

    m_NativeWindow = SDL_CreateWindow(title, 
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      m_Width, m_Height, 0);

    if (m_NativeWindow == nullptr)
    {
        setError(CREATE_ERROR);
        return;
    }
}

Window::~Window()
{
    SDL_DestroyWindow(m_NativeWindow);
}

size_t Window::getWidth() const 
{ 
    return m_Width;       
}

size_t Window::getHeight() const 
{ 
    return m_Height;      
}

const char* Window::getTitle() const 
{ 
    return m_Title;       
}

uint32_t Window::getError() const 
{ 
    return m_ErrorStatus; 
}

SDL_Window* Window::getNativeWindow() const
{
    return m_NativeWindow;
}

void Window::updateTitle(const char* title)
{
    m_Title  = (title == nullptr) ? WINDOW_DEFAULT_TITLE : title;

    SDL_SetWindowTitle(m_NativeWindow, m_Title);
}

void Window::resizeWindow(size_t width, size_t height)
{
    assert(width);
    assert(height);

    m_Width  = width;
    m_Height = height;

    SDL_SetWindowSize(m_NativeWindow, (int) m_Width, (int) m_Height);
}

void Window::setError(uint32_t error)
{
    m_ErrorStatus |= error;
}

bool initGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        return false;
    }

    if (TTF_Init() == -1)
    {
        return false;
    }

    return true;
}

void quitGraphics()
{
    SDL_Quit();
}