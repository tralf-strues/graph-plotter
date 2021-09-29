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
static const float    DELTA_TIME              = 1e-2;
static const size_t   ELECTRONS_COUNT         = 4;

void updateFpsTitle(Window& window, uint32_t frameTime);
void generateParticles(Simulator& simulator, size_t count);

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    Simulator simulator;

    /* ================ Entities ================ */
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

    simulator.entities.pushBack(&wallTop);
    simulator.entities.pushBack(&wallBottom);
    simulator.entities.pushBack(&wallLeft);
    simulator.entities.pushBack(&wallRight);

    generateParticles(simulator, ELECTRONS_COUNT);

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

void generateParticles(Simulator& simulator, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        float radius = (50.0f + (rand() % 100)) / 100.0f;
        float volume = 4 / 3 * 3.14f * radius * radius * radius;
        Vec2<float> pos{radius + (VIEWPORT.getRelativeWidth()  - radius) * (rand() % 100) / 100.0f,
                        radius + (VIEWPORT.getRelativeHeight() - radius) * (rand() % 100) / 100.0f};

        Vec2<float> velocity{30.0f * (rand() % 100) / 100.0f,
                             30.0f * (rand() % 100) / 100.0f};

        Electron* electron = new Electron{radius};
        electron->setMass(volume * 100.0f);
        electron->setPos(pos);
        electron->setVelocity(velocity);

        simulator.entities.pushBack(electron);
    }
}