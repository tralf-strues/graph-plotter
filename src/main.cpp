//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "ui/window.h"
#include "ui/elements/primitives.h"
#include "ui/elements/graph.h"

const size_t    WINDOW_WIDTH            = 800;
const size_t    WINDOW_HEIGHT           = 600;
const char*     WINDOW_TITLE            = "Graph plotter";
const size_t    MAX_WINDOW_TITLE_LENGTH = 128;
const ColorRGBA BACKGROUND_COLOR        = 0x1e'23'28'ff; 

void updateFpsTitle(Window* window, uint32_t frameTime);
float parabola(float x);
float sinus(float x);

int main()
{
    Window window = {};

    WindowError initStatus = createWindow(&window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (initStatus != WINDOW_NO_ERROR)
    {
        printf("Window failed to initialize! Exiting program with error code %d.\n", initStatus);
        return initStatus;
    }

    bool running = true;
    SDL_Event event = {};

    Graph graph = {};
    createGraph(&graph, nullptr, nullptr);

    SDL_Rect graphFrame1 = {50, 50, 300, 300};

    Vector original = {{1, 2}, {1.5, -0.5}};
    Vector vector   = original;
    addVector(&graph, &vector);
    float angle = 0;

    addFunction(&graph, parabola);
    addFunction(&graph, sinus);

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

        angle += 0.01f;
        vector.disp = rot(&original.disp, angle);

        /* ================ Update visuals ================ */
        setDrawColor(window.sdlRenderer, BACKGROUND_COLOR);
        SDL_RenderClear(window.sdlRenderer);

        renderGraph(window.sdlRenderer, &graph, &graphFrame1);

        SDL_RenderPresent(window.sdlRenderer);

        /* ================ Update fps title ================ */
        updateFpsTitle(&window, SDL_GetTicks() - frameStartTime);
    }

    closeWindow(&window);

    return 0;
}

void updateFpsTitle(Window* window, uint32_t frameTime)
{
    static char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    // Time is in milliseconds, that's why use 1e3 - to convert into seconds
    uint32_t fps = 1e3 / frameTime;
    snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

    updateTitle(window, windowTitle);
}

float parabola(float x)
{
    return 0.25 * x * x + 1;
}

float sinus(float x)
{
    return sinf(4 * x);
}