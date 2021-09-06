//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file graph.h
//! @date 2021-09-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H

#include "../../math/vec2.h"
#include "../window.h"

typedef float (*Function) (float x);

static const size_t GRAPH_MAX_FUNCTIONS = 8u;
static const size_t GRAPH_MAX_VECTORS   = 8u;

static const Vec2f32 GRAPH_DEFAULT_AXES_MIN = {-1, -1};
static const Vec2f32 GRAPH_DEFAULT_AXES_MAX = { 5,  5};

static const ColorRGBA GRAPH_FRAME_COLOR = 0x74'72'6b'ff;
static const ColorRGBA GRAPH_AXES_COLOR  = 0xfe'fd'e5'ff;
static const ColorRGBA GRAPH_FUNC_COLOR  = 0xf4'e6'64'ff;
static const ColorRGBA GRAPH_VEC_COLOR   = 0xca'a9'71'ff;

static const float GRAPH_ARROWHEAD_ANGLE     = M_PI / 6;
static const float GRAPH_ARROWHEAD_COS_ANGLE = cosf(GRAPH_ARROWHEAD_ANGLE);
static const float GRAPH_ARROWHEAD_SIN_ANGLE = sinf(GRAPH_ARROWHEAD_ANGLE);
static const float GRAPH_ARROWHEAD_LEN       = 15;

struct Vector
{
    Vec2f32 origin;
    Vec2f32 disp;
};

struct Graph
{
    //--------------------------------------------------------------------------
    //! @defgroup GRAPH_AXES_SPECS Graph's axes specification
    //! @addtogroup GRAPH_AXES_SPECS
    //! @{

    Vec2f32 axesMin; ///< Min values of x and y axes. 
    Vec2f32 axesMax; ///< Max values of x and y axes.

    //! @}
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //! @defgroup GRAPH_ELEMENTS Graph's elements like functions and vectors.
    //! @addtogroup GRAPH_ELEMENTS
    //! @{

    Function functions[GRAPH_MAX_FUNCTIONS]; ///< Array of functions to plot.
    size_t   functionsCount;                 ///< Number of functions added.

    Vector*  vectors[GRAPH_MAX_VECTORS];     ///< Array of vectors   to plot.
    size_t   vectorsCount;                   ///< Number of vectors added.

    //! @}
    //--------------------------------------------------------------------------
};

void createGraph(Graph* graph, const Vec2f32* axesMin, const Vec2f32* axesMax);
bool addFunction(Graph* graph, Function function);
bool addVector(Graph* graph, Vector* vector);

void renderGraph(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect);

#endif // GRAPH_H