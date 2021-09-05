//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "ui/window.h"
#include "ui/elements/primitives.h"

const size_t WINDOW_WIDTH            = 800;
const size_t WINDOW_HEIGHT           = 600;
const char*  WINDOW_TITLE            = "Graph plotter";
const size_t MAX_WINDOW_TITLE_LENGTH = 128;

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

    char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    while (running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        // Process events
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

        // Update visuals
        setDrawColor(&window, COLOR_BLACK);
        clearWindow(&window);

        setDrawColor(&window, COLOR_YELLOW);
        drawLine(&window, {10u, 10u}, {100u, 460u});

        updateWindow(&window);

        // Update fps (time is in milliseconds, that's why use 1e3 - to convert into seconds)
        uint32_t fps = 1e3 / (SDL_GetTicks() - frameStartTime);
        snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

        updateTitle(&window, windowTitle);
    }

    return 0;
}