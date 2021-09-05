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

typedef double (*Function) (double x);

struct Graph
{
    //--------------------------------------------------------------------------
    //! @defgroup GRAPH_FRAME_SPECS Graph's frame specification
    //! @addtogroup GRAPH_FRAME_SPECS
    //! @{
    
    Window* window;
    Vec2u32 windowPos; ///< Position of the Graph's bottom-right corner 
    size_t  width;     ///< Width of the Graph
    size_t  height;    ///< Height of the Graph

    //! @}
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //! @defgroup GRAPH_AXES_SPECS Graph's axes specification
    //! @addtogroup GRAPH_AXES_SPECS
    //! @{

    Vec2f64 originOffset; ///< Origin's offset in the Graph's frame, must be in 
                          ///< the range [0, 1], where (0, 0) corresponds to the 
                          ///< bottom-left corner and (1, 1) corresponds to the 
                          ///< upper-right corner.

    Vec2f64 axesMax;      ///< Max values of x and y axes.

    //! @}
    //--------------------------------------------------------------------------
};

void createGraph(Window* window, Vec2u32 windowPos, size_t width, size_t height);
void setGraphOriginOffset(Graph* graph, Vec2f64 originOffset);
void setGraphAxesMax(Graph* graph, Vec2f64 axesMax);

void graphPlotFunction(Graph* graph, Function function, double xStep);

#endif // GRAPH_H