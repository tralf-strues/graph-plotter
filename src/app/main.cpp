//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../ui/window.h"
#include "../ui/elements/primitives.h"
#include "../ui/elements/graph.h"

const size_t    WINDOW_WIDTH            = 1200;
const size_t    WINDOW_HEIGHT           = 800;
const char*     WINDOW_TITLE            = "Graph plotter";
const size_t    MAX_WINDOW_TITLE_LENGTH = 128;
const SDL_Color BACKGROUND_COLOR        = {0x1e, 0x23, 0x28, 0xff}; 

const Vec2f32   GRAPH_AXES_MIN_1        = {-1, -1};
const Vec2f32   GRAPH_AXES_MAX_1        = { 5,  5};
const SDL_Rect  GRAPH_FRAME_RECT_1      = {50, 50, 500, 500};

const Vec2f32   GRAPH_AXES_MIN_2        = {0, 0};
const Vec2f32   GRAPH_AXES_MAX_2        = {10, 10};
const SDL_Rect  GRAPH_FRAME_RECT_2      = {600, 50, 400, 400};

const Vector    VECTOR_ORIGINAL         = {{1, 2}, {1.5, -0.5}};

struct App
{
    Window window;
    bool   running;

    //--------------------------------------------------------------------------
    //! @defgroup APP_ELEMENTS Application's GUI elements.
    //! @addtogroup APP_ELEMENTS
    //! @{

    Graph graph1;
    Graph graph2;

    Vector vector;
    float  vectorAngle;

    //! @}
    //--------------------------------------------------------------------------
};

int initApp(App* app);
int runApp(App* app);
int processEvent(App* app, SDL_Event event);
int closeApp(App* app);

void updateFpsTitle(Window* window, uint32_t frameTime);
float parabola(float x);
float sinus(float x);
float hyperbola(float x);

int main()
{
    App app;
    initApp(&app);

    runApp(&app);

    closeApp(&app);

    return 0;
}

int initApp(App* app)
{
    assert(app);

    WindowError initStatus = createWindow(&app->window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (initStatus != WINDOW_NO_ERROR)
    {
        printf("Window failed to initialize! Exiting program with error code %d.\n", initStatus);
        return initStatus;
    }

    createGraph(&app->graph1, &GRAPH_AXES_MIN_1, &GRAPH_AXES_MAX_1);
    createGraph(&app->graph2, &GRAPH_AXES_MIN_2, &GRAPH_AXES_MAX_2);

    app->vector = VECTOR_ORIGINAL;
    addVector(&app->graph1, &app->vector);

    addFunction(&app->graph1, parabola);
    addFunction(&app->graph1, sinus);

    addFunction(&app->graph2, hyperbola);
    addFunction(&app->graph2, logf);

    return 0;
}

int runApp(App* app)
{
    assert(app);

    SDL_Event event = {};

    while (app->running)
    {
        uint32_t frameStartTime = SDL_GetTicks();

        /* ================ Process events ================ */
        while (SDL_PollEvent(&event))
        {
            processEvent(app, event);
        }

        app->vectorAngle += 0.01f;
        app->vector.disp = rot(&VECTOR_ORIGINAL.disp, app->vectorAngle);

        /* ================ Rendering ================ */
        setDrawColor(app->window.sdlRenderer, BACKGROUND_COLOR);
        SDL_RenderClear(app->window.sdlRenderer);

        renderGraph(app->window.sdlRenderer, &app->graph1, &GRAPH_FRAME_RECT_1);
        renderGraph(app->window.sdlRenderer, &app->graph2, &GRAPH_FRAME_RECT_2);

        SDL_RenderPresent(app->window.sdlRenderer);

        /* ================ Update fps title ================ */
        updateFpsTitle(&app->window, SDL_GetTicks() - frameStartTime);
    }

    return 0;
}

int processEvent(App* app, SDL_Event event)
{
    assert(app);

    switch (event.type)
    {
        case SDL_QUIT: 
        { 
            app->running = false; 
            break; 
        }

        default: { break; }
    }

    return 0;
}

int closeApp(App* app)
{
    assert(app);

    closeWindow(&app->window);

    return 0;
}

void updateFpsTitle(Window* window, uint32_t frameTime)
{
    static char windowTitle[MAX_WINDOW_TITLE_LENGTH] = {};

    // Time is in milliseconds, that's why use 1e3 - to convert into seconds
    uint32_t fps = 1e3 / frameTime;
    snprintf(windowTitle, MAX_WINDOW_TITLE_LENGTH, "%s [%" PRIu32 " fps]", WINDOW_TITLE, fps);

    updateTitle(window, windowTitle);
}

float parabola(float x)
{
    return 0.25 * x * x + 1;
}

float sinus(float x)
{
    return sinf(4 * x);
}

float hyperbola(float x)
{
    return 4 / x;
}
