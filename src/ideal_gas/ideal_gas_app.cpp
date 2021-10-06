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

        updateAll();
        renderAll();

        /* Update fps title */
        frameTime = SDL_GetTicks() - frameStartTime;
        updateFpsTitle(m_Window, frameTime);

        // FIXME:
        // SDL_Delay(50);
    }
}

void IdealGasApp::updateAll()
{
    m_Simulator.simulate(DELTA_TIME);
    // m_Simulator.simulate(frameTime * TIME_SCALE); FIXME:

    int32_t atoms        = 0;
    int32_t electrons    = 0;
    int32_t positiveIons = 0;
    int32_t negativeIons = 0;
    int32_t maxValue     = 0;

    for (auto entity : m_Simulator.entities)
    {
        if (entity->type == PhysEntity::ELECTRON)
        {
            ++electrons;
            if (electrons > maxValue) { maxValue = electrons; }
        }
        else if (entity->type == PhysEntity::ATOM)
        {
            Atom* atom = static_cast<Atom*>(entity);

            if (atom->getCharge() == 0)
            {
                ++atoms;
                if (atoms > maxValue) { maxValue = atoms; }
            }
            else if (atom->getCharge() > 0)
            {
                ++positiveIons;
                if (positiveIons > maxValue) { maxValue = positiveIons; }
            }
            else
            {
                ++negativeIons;
                if (negativeIons > maxValue) { maxValue = negativeIons; }
            }
        }
    }

    if (static_cast<int32_t>(m_AtomsMonitor.getViewport().axesMax.y) <= maxValue)
    {
        float rangeMax = static_cast<int32_t>(maxValue * 1.5f);

        m_AtomsMonitor.setValueRange(0, rangeMax);
        m_AtomsMonitor.updateLabels(m_Renderer, m_Font);
        
        m_ElectronsMonitor.setValueRange(0, rangeMax);
        m_ElectronsMonitor.updateLabels(m_Renderer, m_Font);

        m_PIonsMonitor.setValueRange(0, rangeMax);
        m_PIonsMonitor.updateLabels(m_Renderer, m_Font);

        m_NIonsMonitor.setValueRange(0, rangeMax);
        m_NIonsMonitor.updateLabels(m_Renderer, m_Font);
    }

    m_AtomsMonitor.addSample(atoms);
    m_ElectronsMonitor.addSample(electrons);
    m_PIonsMonitor.addSample(positiveIons);
    m_NIonsMonitor.addSample(negativeIons);
}

void IdealGasApp::renderAll()
{
    m_Renderer.setColor(BACKGROUND_COLOR);
    m_Renderer.clear();

    m_Renderer.setClipRegion(SIMULATOR_VIEWPORT.windowArea);
    m_Simulator.updateGraphics(m_Renderer, SIMULATOR_VIEWPORT);
    m_Renderer.resetClipRegion();

    m_GuiManager.renderComponents(m_Renderer);
    m_Renderer.present();
}

IdealGasApp::IdealGasApp(int32_t argc, char* argv[])
    : Application(argc, argv),
      m_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE),
      m_Renderer(m_Window),
      m_Font(FONT_FILENAME, FONT_SIZE),
      m_Running(false),
      m_ButtonLeftElectrodeIncrease(Vec2<int32_t>{700, 10}, 60, 30, COLOR_GREEN),
      m_AtomsMonitor(ATOMS_MONITOR_VIEWPORT, ENTITIES_MONITOR_SAMPLES),
      m_ElectronsMonitor(ELECTRONS_MONITOR_VIEWPORT, ENTITIES_MONITOR_SAMPLES),
      m_PIonsMonitor(PIONS_MONITOR_VIEWPORT, ENTITIES_MONITOR_SAMPLES),
      m_NIonsMonitor(NIONS_MONITOR_VIEWPORT, ENTITIES_MONITOR_SAMPLES)
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
    wallLeft->setElectricField(5e3);

    Wall* wallRight = new Wall();
    wallRight->setPos(Vec2<float>{SIMULATOR_VIEWPORT.axesMax.x, SIMULATOR_VIEWPORT.axesMin.y});
    wallRight->setDirection(Vec2<float>{0, 1});
    wallRight->setElectricField(-5e3);

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
    /* Buttons */
    m_ButtonLeftElectrodeIncrease.setLabel(m_Renderer, "Hello!", m_Font);
    m_ButtonLeftElectrodeIncrease.attachToSystemEventManager(m_SystemEventManager);

    /* Monitors */
    m_AtomsMonitor.setColors(ENTITIES_MONITOR_FRAME_CLR, COLOR_ATOM_NEUTRAL, ENTITIES_MONITOR_TEXT_CLR);
    m_AtomsMonitor.setTitle(m_Renderer, m_Font, "Atoms");
    m_AtomsMonitor.updateLabels(m_Renderer, m_Font);

    m_ElectronsMonitor.setColors(ENTITIES_MONITOR_FRAME_CLR, COLOR_ELECTRON, ENTITIES_MONITOR_TEXT_CLR);
    m_ElectronsMonitor.setTitle(m_Renderer, m_Font, "Electrons");
    m_ElectronsMonitor.updateLabels(m_Renderer, m_Font);

    m_PIonsMonitor.setColors(ENTITIES_MONITOR_FRAME_CLR, COLOR_ATOM_POSITIVE, ENTITIES_MONITOR_TEXT_CLR);
    m_PIonsMonitor.setTitle(m_Renderer, m_Font, "Positive ions");
    m_PIonsMonitor.updateLabels(m_Renderer, m_Font);

    m_NIonsMonitor.setColors(ENTITIES_MONITOR_FRAME_CLR, COLOR_ATOM_NEGATIVE, ENTITIES_MONITOR_TEXT_CLR);
    m_NIonsMonitor.setTitle(m_Renderer, m_Font, "Negative ions");
    m_NIonsMonitor.updateLabels(m_Renderer, m_Font);

    m_GuiManager.addComponent(&m_ButtonLeftElectrodeIncrease);

    m_GuiManager.addComponent(&m_AtomsMonitor);
    m_GuiManager.addComponent(&m_ElectronsMonitor);
    m_GuiManager.addComponent(&m_PIonsMonitor);
    m_GuiManager.addComponent(&m_NIonsMonitor);
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