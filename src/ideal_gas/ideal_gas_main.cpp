//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "simulator.h"

static const size_t   WINDOW_WIDTH            = 1200;
static const size_t   WINDOW_HEIGHT           = 800;
static const char*    WINDOW_TITLE            = "Ideal gas simulation";
static const size_t   MAX_WINDOW_TITLE_LENGTH = 128;
static const Color    BACKGROUND_COLOR        = 0x2F'69'AA'FF; 
static const Viewport VIEWPORT                = {{0, 0}, {10, 10}};

void updateFpsTitle(Window& window, uint32_t frameTime);

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    /* ================ Entities ================ */
    Molecule molecule{1, 1, {3, 3}, {1, 1}};

    /* ================ Main loop ================ */
    SDL_Event event   = {};
    bool      running = true;

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

        /* ================ Update objects ================ */
        

        /* ================ Rendering ================ */
        renderer.setColor(COLOR_BLACK);
        renderer.clear();

        renderer.setColor(COLOR_RED);
        drawMolecule(renderer, VIEWPORT, molecule);

        renderer.present();

        /* ================ Update fps title ================ */
        updateFpsTitle(window, SDL_GetTicks() - frameStartTime);
    }

    quitGraphics();

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