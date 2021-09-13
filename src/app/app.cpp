//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file app.cpp
//! @date 2021-09-13
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "app.h"

float parabola(float x);
float sinus(float x);
float hyperbola(float x);

void updateFpsTitle(Window& window, uint32_t frameTime);

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE),
             renderer(window),
             graph1(GRAPH_AXES_MIN_1, GRAPH_AXES_MAX_1),
             graph2(GRAPH_AXES_MIN_2, GRAPH_AXES_MAX_2),
             vector(VECTOR_ORIGINAL)

{
    assert(window.getError() != Window::NO_ERROR);

    graph1.addVector(&vector);

    graph1.addFunction(parabola);
    graph1.addFunction(sinus);

    graph2.addFunction(hyperbola);
    graph2.addFunction(logf);
}

// FIXME: get rid of sdl_event usage
int App::run()
{
    SDL_Event event = {};

    while (running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        /* ================ Process events ================ */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: 
                { 
                    running = false; 
                    break; 
                }

                default: { break; }
            }
        }

        vectorAngle += 0.01f;
        vector.disp = rotate(VECTOR_ORIGINAL.disp, vectorAngle);

        /* ================ Rendering ================ */
        renderer.setColor(BACKGROUND_COLOR);
        renderer.clear();

        graph1.render(renderer, GRAPH_FRAME_RECT_1);
        graph2.render(renderer, GRAPH_FRAME_RECT_2);

        renderer.present();

        /* ================ Update fps title ================ */
        updateFpsTitle(window, SDL_GetTicks() - frameStartTime);
    }

    return 0;
}

void updateFpsTitle(Window& window, uint32_t frameTime)
{
    static char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    // Time is in milliseconds, that's why use 1e3 - to convert into seconds
    uint32_t fps = 1e3 / frameTime;
    snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

    window.updateTitle(windowTitle);
}

float parabola(float x)
{
    return 0.25 * x * x + 1;
}

float sinus(float x)
{
    return sinf(4 * x);
}

float hyperbola(float x)
{
    return 4 / x;
}