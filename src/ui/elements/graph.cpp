#include <math.h>
#include "graph.h"

void renderVector(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, Vector vector);

void createGraph(Graph* graph, const Vec2f32* axesMin, const Vec2f32* axesMax)
{
    assert(graph);

    graph->axesMin = (axesMin != nullptr) ? *axesMin : GRAPH_DEFAULT_AXES_MIN;
    graph->axesMax = (axesMax != nullptr) ? *axesMax : GRAPH_DEFAULT_AXES_MAX;

    assert(graph->axesMin.x < graph->axesMax.x);
    assert(graph->axesMin.y < graph->axesMax.y);
}

bool addFunction(Graph* graph, Function function)
{
    assert(graph);
    assert(function);
    
    if (graph->functionsCount >= GRAPH_MAX_FUNCTIONS)
    {
        return false;
    }

    graph->functions[graph->functionsCount++] = function;

    return true;
}

bool addVector(Graph* graph, Vector* vector)
{
    assert(graph);
    assert(vector);
    
    if (graph->vectorsCount >= GRAPH_MAX_VECTORS)
    {
        return false;
    }

    graph->vectors[graph->vectorsCount++] = vector;

    return true;
}

void renderGraph(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect)
{
    assert(renderer);
    assert(graph);
    assert(rect);

    SDL_RenderSetClipRect(renderer, rect);

    SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0x00, 0xff);
    SDL_RenderDrawRect(renderer, rect);

    float relativeHeight = graph->axesMax.y - graph->axesMin.y;
    float relativeWidth  = graph->axesMax.x - graph->axesMin.x;

    SDL_SetRenderDrawColor(renderer, 0xb0, 0xb0, 0x00, 0xff);

    // y axis
    if (graph->axesMin.x <= 0 && graph->axesMax.x >= 0)
    {
        renderVector(renderer, graph, rect, {{0, graph->axesMin.y}, {0, relativeHeight}});
    }

    // x axis
    if (graph->axesMin.y <= 0 && graph->axesMax.y >= 0)
    {
        renderVector(renderer, graph, rect, {{graph->axesMin.x, 0}, {relativeWidth, 0}});
    }

    // vectors
    for (size_t vector = 0; vector < graph->vectorsCount; vector++)
    {
        renderVector(renderer, graph, rect, *graph->vectors[vector]);
    }

    SDL_RenderSetClipRect(renderer, nullptr);
}

void renderVector(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, Vector vector)
{
    assert(renderer);
    assert(graph);

    float relativeHeight = graph->axesMax.y - graph->axesMin.y;
    float relativeWidth  = graph->axesMax.x - graph->axesMin.x;

    Vec2f32 from = {rect->x + (float) rect->w / relativeWidth  * (vector.origin.x - graph->axesMin.x), 
                    rect->y - (float) rect->h / relativeHeight * (vector.origin.y - graph->axesMax.y)};

    Vec2f32 to = {rect->x + (float) rect->w / relativeWidth  * (vector.origin.x + vector.disp.x - graph->axesMin.x), 
                  rect->y - (float) rect->h / relativeHeight * (vector.origin.y + vector.disp.y - graph->axesMax.y)};

    Vec2f32 vectorWindowCoords = sub(&to, &from);

    float xcos = vectorWindowCoords.x * GRAPH_ARROWHEAD_COS_ANGLE;
    float xsin = vectorWindowCoords.x * GRAPH_ARROWHEAD_SIN_ANGLE;
    float ycos = vectorWindowCoords.y * GRAPH_ARROWHEAD_COS_ANGLE;
    float ysin = vectorWindowCoords.y * GRAPH_ARROWHEAD_SIN_ANGLE;

    Vec2f32 head1 = {xcos - ysin, ycos + xsin};
    head1 = mul(&head1, -GRAPH_ARROWHEAD_LEN / len(&head1));

    Vec2f32 head2 = {xcos + ysin, ycos - xsin};
    head2 = mul(&head2, -GRAPH_ARROWHEAD_LEN / len(&head2));

    SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
    SDL_RenderDrawLineF(renderer, to.x, to.y, to.x + head1.x, to.y + head1.y);
    SDL_RenderDrawLineF(renderer, to.x, to.y, to.x + head2.x, to.y + head2.y);
}