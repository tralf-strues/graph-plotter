//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ideal_gas_app.h
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef IDEAL_GAS_APP_H
#define IDEAL_GAS_APP_H

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../core/utils/viewport.h"
#include "../core/app/app.h"
#include "../gui/gui_manager.h"
#include "ideal_gas_gui_logic.h"
#include "simulator.h"

//------------------------Window and system specification-----------------------
static const size_t        WINDOW_WIDTH               = 1400;
static const size_t        WINDOW_HEIGHT              = 700;
static const char*         WINDOW_TITLE               = "Ideal gas simulation";
static const size_t        MAX_WINDOW_TITLE_LENGTH    = 128;

static const char*         FONT_FILENAME              = "../res/OpenSans-Bold.ttf";
static const size_t        FONT_SIZE                  = 16;

static const Color         BACKGROUND_COLOR           = 0x000c1a'FF;
//------------------------------------------------------------------------------

//----------------------------------Simulation----------------------------------
static const float         DELTA_TIME                 = 1e-6;

static const size_t        ELECTRONS_COUNT            = 25;
static const size_t        ATOMS_COUNT                = 25;
//------------------------------------------------------------------------------

//--------------------------------GUI components--------------------------------
static const Viewport      SIMULATOR_VIEWPORT         = Viewport{{0, 0}, {55, 24}, {{20, 20},  1060, 480}};

static const size_t        ENTITIES_MONITOR_SAMPLES   = 1000;

static const Vec2<float>   ENTITIES_MONITOR_AXES_MIN  = {0,  0};
static const Vec2<float>   ENTITIES_MONITOR_AXES_MAX  = {(float) ENTITIES_MONITOR_SAMPLES, 32};

static const Color         ENTITIES_MONITOR_FRAME_CLR = COLOR_YELLOW; 
static const Color         ENTITIES_MONITOR_TEXT_CLR  = COLOR_WHITE; 
static const Vec2<int32_t> ENTITIES_MONITOR_POS       = {20, 520};
static const int32_t       ENTITIES_MONITOR_WIDTH     = 290;
static const int32_t       ENTITIES_MONITOR_HEIGHT    = 130;
static const int32_t       ENTITIES_MONITOR_MARGIN    = 60;

static const Viewport ATOMS_MONITOR_VIEWPORT = Viewport{ENTITIES_MONITOR_AXES_MIN,
                                                        ENTITIES_MONITOR_AXES_MAX,
                                                        {ENTITIES_MONITOR_POS, 
                                                         ENTITIES_MONITOR_WIDTH,
                                                         ENTITIES_MONITOR_HEIGHT}};

static const Viewport ELECTRONS_MONITOR_VIEWPORT = Viewport{ENTITIES_MONITOR_AXES_MIN,
                                                            ENTITIES_MONITOR_AXES_MAX,
                                                            {{ENTITIES_MONITOR_POS.x +
                                                              ENTITIES_MONITOR_WIDTH +
                                                              ENTITIES_MONITOR_MARGIN,
                                                              ENTITIES_MONITOR_POS.y}, 
                                                             ENTITIES_MONITOR_WIDTH,
                                                             ENTITIES_MONITOR_HEIGHT}};

static const Viewport PIONS_MONITOR_VIEWPORT = Viewport{ENTITIES_MONITOR_AXES_MIN,
                                                        ENTITIES_MONITOR_AXES_MAX,
                                                        {{ENTITIES_MONITOR_POS.x +
                                                          2 * ENTITIES_MONITOR_WIDTH +
                                                          2 * ENTITIES_MONITOR_MARGIN,
                                                          ENTITIES_MONITOR_POS.y}, 
                                                         ENTITIES_MONITOR_WIDTH,
                                                         ENTITIES_MONITOR_HEIGHT}};

static const Viewport NIONS_MONITOR_VIEWPORT = Viewport{ENTITIES_MONITOR_AXES_MIN,
                                                        ENTITIES_MONITOR_AXES_MAX,
                                                        {{ENTITIES_MONITOR_POS.x +
                                                          3 * ENTITIES_MONITOR_WIDTH +
                                                          3 * ENTITIES_MONITOR_MARGIN,
                                                          ENTITIES_MONITOR_POS.y}, 
                                                         ENTITIES_MONITOR_WIDTH,
                                                         ENTITIES_MONITOR_HEIGHT}};

static const Vec2<int32_t> LABEL_LEFT_ELECTRODE_POS  = {1100, 20};
static const Vec2<int32_t> LABEL_RIGHT_ELECTRODE_POS = {1100, 50};
static const Vec2<int32_t> LABEL_SPAWN_PARTICLE_POS  = {1100, 100};
//------------------------------------------------------------------------------

struct ButtonListener;

class IdealGasApp : Application
{
public:
    IdealGasApp(int32_t argc, char* argv[]);

    /* Application */
    void run() override;

private:
    Window                  m_Window;
    Renderer                m_Renderer;
    Font                    m_Font;
    SystemEventManager      m_SystemEventManager;
    bool                    m_Running;
    Simulator               m_Simulator;

    GUI_Manager             m_GuiManager;
    GUI_Button              m_ButtonLeftElectrodeIncrease;

    GUI_ActivityMonitor     m_AtomsMonitor;
    GUI_ActivityMonitor     m_ElectronsMonitor;
    GUI_ActivityMonitor     m_PIonsMonitor;
    GUI_ActivityMonitor     m_NIonsMonitor;

    GUI_TextLabel           m_LabelLeftElectrode;
    GUI_TextLabel           m_LabelRightElectrode;
    GUI_TextLabel           m_LabelSpawnParticle;

    QuitListener            m_Quit;

    ElectrodeButtonListener m_LeftElectrodeIncreaseListener;
    ElectrodeButtonListener m_RightElectrodeIncreaseListener;
    ElectrodeButtonListener m_LeftElectrodeDecreaseListener;
    ElectrodeButtonListener m_RightElectrodeDecreaseListener;

    void initSimulator();
    void initGUI();
    void initEventListeners();

    void updateAll();
    void renderAll();
};

#endif // IDEAL_GAS_APP_H