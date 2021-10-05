//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ideal_gas_app.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/utils/random.h"
#include "ideal_gas_app.h"

void updateFpsTitle(Window& window, uint32_t frameTime);
void generateParticles(Simulator& simulator, size_t count, PhysEntity::Type type);

int main(int32_t argc, char* argv[])
{
    initGraphics();

    IdealGasApp app{argc, argv};
    app.run();

    quitGraphics();

    return 0;
}

void IdealGasApp::run()
{
    m_Running = true;

    uint32_t frameTime = 0;
    while (m_Running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        m_SystemEventManager.proccessEvents();

        m_Simulator.simulate(DELTA_TIME);
        // m_Simulator.simulate(frameTime * TIME_SCALE); FIXME:

        m_ActivityMonitor.addSample(randomFromInterval(0, 10));

        m_Renderer.setColor(BACKGROUND_COLOR);
        m_Renderer.clear();

        m_Renderer.setClipRegion(Rectangle{SIMULATOR_VIEWPORT.windowArea.pos, 
                                           SIMULATOR_VIEWPORT.windowArea.width  + 1,
                                           SIMULATOR_VIEWPORT.windowArea.height + 1});
        m_Simulator.updateGraphics(m_Renderer, SIMULATOR_VIEWPORT);
        m_Renderer.resetClipRegion();

        // button.render(m_Renderer);
        // activityMonitor.render(m_Renderer);
        m_GuiManager.renderComponents(m_Renderer);

        m_Renderer.present();

        /* Update fps title */
        frameTime = SDL_GetTicks() - frameStartTime;
        updateFpsTitle(m_Window, frameTime);

        // FIXME:
        // SDL_Delay(50);
    }
}

IdealGasApp::IdealGasApp(int32_t argc, char* argv[])
    : Application(argc, argv),
      m_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE),
      m_Renderer(m_Window),
      m_Font(FONT_FILENAME, FONT_SIZE),
      m_Running(false),
      m_ButtonLeftElectrodeIncrease(Vec2<int32_t>{700, 10}, 60, 30, COLOR_GREEN),
      m_ActivityMonitor(ACTIVITY_MONITOR_VIEWPORT, 50)
{
    initSimulator();
    initGUI();
    initEventListeners();
}

void IdealGasApp::initSimulator()
{
    generateParticles(m_Simulator, ELECTRONS_COUNT, PhysEntity::ELECTRON);
    generateParticles(m_Simulator, ATOMS_COUNT,     PhysEntity::ATOM);

    Wall* wallTop = new Wall();
    wallTop->setPos(Vec2<float>{SIMULATOR_VIEWPORT.axesMin.x, SIMULATOR_VIEWPORT.axesMax.y});
    wallTop->setDirection(Vec2<float>{1, 0});

    Wall* wallBottom = new Wall();
    wallBottom->setPos(Vec2<float>{SIMULATOR_VIEWPORT.axesMin.x, SIMULATOR_VIEWPORT.axesMin.y});
    wallBottom->setDirection(Vec2<float>{1, 0});

    Wall* wallLeft = new Wall();
    wallLeft->setPos(Vec2<float>{SIMULATOR_VIEWPORT.axesMin.x, SIMULATOR_VIEWPORT.axesMin.y});
    wallLeft->setDirection(Vec2<float>{0, 1});
    wallLeft->setElectricField(1e3);

    Wall* wallRight = new Wall();
    wallRight->setPos(Vec2<float>{SIMULATOR_VIEWPORT.axesMax.x, SIMULATOR_VIEWPORT.axesMin.y});
    wallRight->setDirection(Vec2<float>{0, 1});
    wallRight->setElectricField(-1e3);

    m_Simulator.entities.pushBack(wallTop);
    m_Simulator.entities.pushBack(wallBottom);
    m_Simulator.entities.pushBack(wallLeft);
    m_Simulator.entities.pushBack(wallRight);

    m_LeftElectrodeIncreaseListener.electrode  = wallLeft;
    m_RightElectrodeIncreaseListener.electrode = wallRight;
    m_LeftElectrodeDecreaseListener.electrode  = wallLeft;
    m_RightElectrodeDecreaseListener.electrode = wallRight;
}

void IdealGasApp::initGUI()
{
    // TODO:
    // Text buttonLabel{};
    // buttonLabel.load(m_Renderer, "Hello!", FONT, COLOR_WHITE);
    m_ButtonLeftElectrodeIncrease.setLabel(m_Renderer, "Hello!", m_Font);
    m_ButtonLeftElectrodeIncrease.attachToSystemEventManager(m_SystemEventManager);
    m_GuiManager.addComponent(&m_ButtonLeftElectrodeIncrease);

    // TODO:
    m_ActivityMonitor.updateLabels(m_Renderer, m_Font);
    m_GuiManager.addComponent(&m_ActivityMonitor);


}

void IdealGasApp::initEventListeners()
{
    m_Quit.running = &m_Running;
    m_SystemEventManager.attachListener({Event::WINDOW_CLOSE, Event::KEYBOARD_PRESSED}, &m_Quit);

    // FIXME:
    m_LeftElectrodeIncreaseListener.label = nullptr;
    m_LeftElectrodeIncreaseListener.type  = ElectrodeButtonListener::INCREASE_ELECTRIC_FIELD;
    m_ButtonLeftElectrodeIncrease.attachListener({Event::GUI_BUTTON_PRESSED}, 
                                                  &m_LeftElectrodeIncreaseListener);

    m_RightElectrodeIncreaseListener.label = nullptr;
    m_RightElectrodeIncreaseListener.type  = ElectrodeButtonListener::INCREASE_ELECTRIC_FIELD;

    m_LeftElectrodeDecreaseListener.label  = nullptr;
    m_LeftElectrodeDecreaseListener.type   = ElectrodeButtonListener::DECREASE_ELECTRIC_FIELD;

    m_RightElectrodeDecreaseListener.label = nullptr;
    m_RightElectrodeDecreaseListener.type  = ElectrodeButtonListener::DECREASE_ELECTRIC_FIELD;
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

        Vec2<float> pos{randomFromInterval<float>(2 * radius, SIMULATOR_VIEWPORT.getRelativeWidth()  - 2 * radius),
                        randomFromInterval<float>(2 * radius, SIMULATOR_VIEWPORT.getRelativeHeight() - 2 * radius)};

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

            if (randomTry(0.3f))
            {
                atom->setCharge(-ELECTRON_CHARGE);
            }

            simulator.entities.pushBack(atom);
        }
    }
}