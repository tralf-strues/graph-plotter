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
static const size_t   WINDOW_WIDTH              = 1400;
static const size_t   WINDOW_HEIGHT             = 700;
static const char*    WINDOW_TITLE              = "Ideal gas simulation";
static const size_t   MAX_WINDOW_TITLE_LENGTH   = 128;

static const char*    FONT_FILENAME             = "../res/OpenSans-Bold.ttf";
static const size_t   FONT_SIZE                 = 16;

static const Color    BACKGROUND_COLOR          = 0x02162E'FF;
//------------------------------------------------------------------------------

//----------------------------------Simulation----------------------------------
static const float    DELTA_TIME                = 1e-6;
static const float    TIME_SCALE                = 1e-7; ///< simulation time passed after 1 real millisecond

static const size_t   ELECTRONS_COUNT           = 20;
static const size_t   ATOMS_COUNT               = 25;
//------------------------------------------------------------------------------

//--------------------------------GUI components--------------------------------
static const Viewport SIMULATOR_VIEWPORT        = Viewport{{0, 0}, {30, 20}, {{50, 50},   600, 400}};
static const Viewport ACTIVITY_MONITOR_VIEWPORT = Viewport{{0, 0}, {20, 10}, {{600, 500}, 400, 200}};
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
    GUI_ActivityMonitor     m_ActivityMonitor;

    QuitListener            m_Quit;

    ElectrodeButtonListener m_LeftElectrodeIncreaseListener;
    ElectrodeButtonListener m_RightElectrodeIncreaseListener;
    ElectrodeButtonListener m_LeftElectrodeDecreaseListener;
    ElectrodeButtonListener m_RightElectrodeDecreaseListener;

    void initSimulator();
    void initGUI();
    void initEventListeners();
};

#endif // IDEAL_GAS_APP_H