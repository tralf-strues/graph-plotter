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
static const Viewport VIEWPORT                = {{0, 0}, {30, 20}};
static const float    DELTA_TIME              = 2e-3;

void updateFpsTitle(Window& window, uint32_t frameTime);

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    Simulator simulator;

    /* ================ Entities ================ */
    Molecule molecule1{1};
    molecule1.setPos(Vec2<float>{10, 10});
    molecule1.setVelocity(Vec2<float>{12, 24});

    Molecule molecule2{0.5};
    molecule2.setPos(Vec2<float>{20, 5});
    molecule2.setVelocity(Vec2<float>{3, -6});

    Wall wallTop;
    wallTop.setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMax.y});
    wallTop.setDirection(Vec2<float>{1, 0});

    Wall wallBottom;
    wallBottom.setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMin.y});
    wallBottom.setDirection(Vec2<float>{1, 0});

    Wall wallLeft;
    wallLeft.setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMin.y});
    wallLeft.setDirection(Vec2<float>{0, 1});

    Wall wallRight;
    wallRight.setPos(Vec2<float>{VIEWPORT.axesMax.x, VIEWPORT.axesMin.y});
    wallRight.setDirection(Vec2<float>{0, 1});

    simulator.entities.pushBack(&molecule1);
    simulator.entities.pushBack(&molecule2);
    simulator.entities.pushBack(&wallTop);
    simulator.entities.pushBack(&wallBottom);
    simulator.entities.pushBack(&wallLeft);
    simulator.entities.pushBack(&wallRight);

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
        simulator.simulate(DELTA_TIME);

        /* ================ Rendering ================ */
        renderer.setColor(COLOR_BLACK);
        renderer.clear();

        renderer.setColor(COLOR_RED);
        simulator.updateGraphics(renderer, VIEWPORT);

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