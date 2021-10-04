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
static const Viewport VIEWPORT                = Viewport{{0, 0}, {30, 20}, {{50, 50}, 600, 400}};
static const float    DELTA_TIME              = 3e-6;
static const size_t   ELECTRONS_COUNT         = 15;
static const size_t   ATOMS_COUNT             = 25;

void updateFpsTitle(Window& window, uint32_t frameTime);
void generateParticles(Simulator& simulator, size_t count, PhysEntity::Type type);

struct QuitListener : public IListener
{
    bool& running;
    QuitListener(bool& running) : running(running) {}

    void onEvent(const Event& event) override
    {
        switch (event.type)
        {
            case Event::WINDOW_CLOSE:
            { 
                running = false;
                break;
            }

            case Event::KEYBOARD_PRESSED:
            {
                if (((const EventKeyboardPressed&) event).scancode == SCANCODE_ESCAPE)
                {
                    running = false;
                }
                break;
            }

            default: { break; }
        }
    }
};

int main()
{
    initGraphics();

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Renderer renderer(window);

    bool running = true;

    Simulator simulator;

    /* ================ Entities ================ */
    Wall* wallTop = new Wall();
    wallTop->setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMax.y - 1});
    wallTop->setDirection(Vec2<float>{1, 0});

    Wall* wallBottom = new Wall();
    wallBottom->setPos(Vec2<float>{VIEWPORT.axesMin.x, VIEWPORT.axesMin.y + 1});
    wallBottom->setDirection(Vec2<float>{1, 0});

    Wall* wallLeft = new Wall();
    wallLeft->setPos(Vec2<float>{VIEWPORT.axesMin.x + 1, VIEWPORT.axesMin.y});
    wallLeft->setDirection(Vec2<float>{0, 1});
    wallLeft->setElectricField(5e2);

    Wall* wallRight = new Wall();
    wallRight->setPos(Vec2<float>{VIEWPORT.axesMax.x - 1, VIEWPORT.axesMin.y});
    wallRight->setDirection(Vec2<float>{0, 1});
    wallRight->setElectricField(-5e2);

    simulator.entities.pushBack(wallTop);
    simulator.entities.pushBack(wallBottom);
    simulator.entities.pushBack(wallLeft);
    simulator.entities.pushBack(wallRight);

    generateParticles(simulator, ELECTRONS_COUNT, PhysEntity::ELECTRON);
    generateParticles(simulator, ATOMS_COUNT,     PhysEntity::ATOM);

    /* ================ Events handling ================ */
    SystemEventManager eventManager{};

    QuitListener quitListener{running};
    eventManager.attachListener({Event::WINDOW_CLOSE, Event::KEYBOARD_PRESSED}, &quitListener);

    /* ================ Main loop ================ */
    while (running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        /* ================ Process events ================ */
        eventManager.proccessEvents();

        /* ================ Update objects ================ */
        simulator.simulate(DELTA_TIME);

        /* ================ Rendering ================ */
        renderer.setColor(BACKGROUND_COLOR);
        renderer.clear();

        renderer.setColor(COLOR_RED);

        renderer.setClipRegion(VIEWPORT.windowArea);
        simulator.updateGraphics(renderer, VIEWPORT);
        renderer.resetClipRegion();

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

        Vec2<float> pos{randomFromInterval<float>(2 * radius, VIEWPORT.getRelativeWidth()  - 2 * radius),
                        randomFromInterval<float>(2 * radius, VIEWPORT.getRelativeHeight() - 2 * radius)};

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