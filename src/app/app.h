//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file app.h
//! @date 2021-09-13
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef APP_H
#define APP_H

#include "../gui/core/core.h"
#include "../gui/elements/graph.h"

static const size_t        WINDOW_WIDTH            = 1200;
static const size_t        WINDOW_HEIGHT           = 800;
static const char*         WINDOW_TITLE            = "Graph plotter";
static const size_t        MAX_WINDOW_TITLE_LENGTH = 128;
static const Color         BACKGROUND_COLOR        = 0x1E'23'28'FF; 

static const Vec2<float>   GRAPH_AXES_MIN_1        = {-1, -1};
static const Vec2<float>   GRAPH_AXES_MAX_1        = { 5,  5};
static const Rectangle     GRAPH_FRAME_RECT_1      = {{50, 50}, 500, 500};

static const Vec2<float>   GRAPH_AXES_MIN_2        = {0, 0};
static const Vec2<float>   GRAPH_AXES_MAX_2        = {10, 10};
static const Rectangle     GRAPH_FRAME_RECT_2      = {{600, 50}, 400, 400};

static const Graph::Vector VECTOR_ORIGINAL         = {{1, 2}, {1.5, -0.5}};

class App
{
public:
    App();

    int run();

private:
    Window window;
    Renderer renderer;
    bool running;

    //--------------------------------------------------------------------------
    //! @defgroup APP_ELEMENTS Application's GUI elements.
    //! @addtogroup APP_ELEMENTS
    //! @{

    Graph graph1;
    Graph graph2;

    Graph::Vector vector;
    float         vectorAngle;

    //! @}
    //--------------------------------------------------------------------------
};

#endif // APP_H