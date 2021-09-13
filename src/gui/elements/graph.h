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
#include "../core/core.h"

static const size_t GRAPH_MAX_FUNCTIONS = 8u;
static const size_t GRAPH_MAX_VECTORS   = 8u;

static const Vec2<float> GRAPH_DEFAULT_AXES_MIN = {-1.0f, -1.0f};
static const Vec2<float> GRAPH_DEFAULT_AXES_MAX = { 5.0f,  5.0f};

static const Color GRAPH_FRAME_COLOR = 0x74'72'6B'FF;
static const Color GRAPH_AXES_COLOR  = 0xFE'FD'E5'FF;
static const Color GRAPH_FUNC_COLOR  = 0xF4'E6'64'FF;
static const Color GRAPH_VEC_COLOR   = 0xCA'A9'71'FF;

static const float GRAPH_ARROWHEAD_ANGLE = M_PI / 6.0f;
static const float GRAPH_ARROWHEAD_LEN   = 15.0f;

class Graph
{
public:
    typedef float (*Function) (float x);

    struct Vector
    {
        Vec2<float> origin;
        Vec2<float> disp;
    };

public:
    Graph(const Vec2<float>& axesMin = GRAPH_DEFAULT_AXES_MIN, 
          const Vec2<float>& axesMax = GRAPH_DEFAULT_AXES_MAX);

    bool addFunction(Function function);
    bool addVector(Vector* vector);

    const Vec2<float>& getAxesMin() const;
    void setAxesMin(const Vec2<float>& axesMin);

    const Vec2<float>& getAxesMax() const;
    void setAxesMax(const Vec2<float>& axesMax);

    size_t getFunctionsCount() const;
    size_t getVectorsCount() const;

    void render(Renderer& renderer, const Rectangle& region) const;

private:
    Vec2<float> axesMin;                     ///< Min values of x and y axes. 
    Vec2<float> axesMax;                     ///< Max values of x and y axes.

    Function functions[GRAPH_MAX_FUNCTIONS]; ///< Array of functions to plot.
    size_t   functionsCount;                 ///< Number of functions added.

    Vector*  vectors[GRAPH_MAX_VECTORS];     ///< Array of vectors to plot.
    size_t   vectorsCount;                   ///< Number of vectors added.
};

#endif // GRAPH_H