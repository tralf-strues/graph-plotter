//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../core/utils/random.h"
#include "simulator.h"

static const size_t   WINDOW_WIDTH            = 1200;
static const size_t   WINDOW_HEIGHT           = 800;
static const char*    WINDOW_TITLE            = "Ideal gas simulation";
static const size_t   MAX_WINDOW_TITLE_LENGTH = 128;
static const Color    BACKGROUND_COLOR        = 0x02162E'FF; 
static const Viewport VIEWPORT                = {{0, 0}, {30, 20}};
static const float    DELTA_TIME              = 3e-6;
static const size_t   ELECTRONS_COUNT         = 3;
static const size_t   ATOMS_COUNT             = 10;

void updateFpsTitle(Window& window, uint32_t frameTime);
void generateParticles(Simulator& simulator, size_t count, PhysEntity::Type type);

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    Simulator simulator;

    /* ================ Entities ================ */
    Wall* wallTop = new Wall();
    wallTop->setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMax.y});
    wallTop->setDirection(Vec2<float>{1, 0});

    Wall* wallBottom = new Wall();
    wallBottom->setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMin.y});
    wallBottom->setDirection(Vec2<float>{1, 0});

    Wall* wallLeft = new Wall();
    wallLeft->setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMin.y});
    wallLeft->setDirection(Vec2<float>{0, 1});

    Wall* wallRight = new Wall();
    wallRight->setPos(Vec2<float>{VIEWPORT.axesMax.x, VIEWPORT.axesMin.y});
    wallRight->setDirection(Vec2<float>{0, 1});

    simulator.entities.pushBack(wallTop);
    simulator.entities.pushBack(wallBottom);
    simulator.entities.pushBack(wallLeft);
    simulator.entities.pushBack(wallRight);

    generateParticles(simulator, ELECTRONS_COUNT, PhysEntity::ELECTRON);
    generateParticles(simulator, ATOMS_COUNT,     PhysEntity::ATOM);

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

                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        running = false;
                    }

                    break;
                }

                default: { break; }
            }
        }

        /* ================ Update objects ================ */
        simulator.simulate(DELTA_TIME);

        /* ================ Rendering ================ */
        renderer.setColor(BACKGROUND_COLOR);
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

void generateParticles(Simulator& simulator, size_t count, PhysEntity::Type type)
{
    if (type != PhysEntity::ELECTRON && type != PhysEntity::ATOM)
    {
        return;
    }

    for (size_t i = 0; i < count; ++i)
    {
        float radius = randomFromInterval<float>(0.4, 1.2);
        float volume = calculateSphereVolume(radius);

        Vec2<float> pos{randomFromInterval<float>(radius, VIEWPORT.getRelativeWidth()  - radius),
                        randomFromInterval<float>(radius, VIEWPORT.getRelativeHeight() - radius)};

        Vec2<float> velocity{randomFromInterval<float>(-3e4, 3e4),
                             randomFromInterval<float>(-3e4, 3e4)};

        if (type == PhysEntity::ELECTRON)
        {
            Electron* electron = new Electron{0.2f};
            electron->setPos(pos);
            electron->setVelocity(velocity);

            simulator.entities.pushBack(electron);
        } 
        else if (type == PhysEntity::ATOM)
        {
            Atom* atom = new Atom{radius};
            atom->setMass(volume * ATOM_DENSITY);
            atom->setPos(pos);
            atom->setVelocity(velocity);

            if (randomTry(0.1f))
            {
                atom->setCharge(-ELECTRON_CHARGE);
            }

            simulator.entities.pushBack(atom);
        }
    }
}