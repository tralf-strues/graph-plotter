#include <math.h>
#include "graph.h"

const float  STEP_PIXELS      = 0.1f;
const size_t LABEL_MAX_LENGTH = 32;

Vec2f32 toPixels(const Graph* graph, const SDL_Rect* rect, Vec2f32 point);
void renderVector(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, Vector vector);
void renderFunction(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, 
                    Function function);

void createGraph(Graph* graph, const Vec2f32* axesMin, const Vec2f32* axesMax)
{
    assert(graph);

    graph->axesMin = (axesMin != nullptr) ? *axesMin : GRAPH_DEFAULT_AXES_MIN;
    graph->axesMax = (axesMax != nullptr) ? *axesMax : GRAPH_DEFAULT_AXES_MAX;

    graph->functionsCount = 0;
    graph->vectorsCount = 0;

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

    // ================ Render frame ================
    setDrawColor(renderer, GRAPH_FRAME_COLOR);
    SDL_RenderDrawRect(renderer, rect);

    float relativeHeight = graph->axesMax.y - graph->axesMin.y;
    float relativeWidth  = graph->axesMax.x - graph->axesMin.x;

    // ================ Render axes ================
    setDrawColor(renderer, GRAPH_AXES_COLOR);

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

    // ================ Render vectors ================
    setDrawColor(renderer, GRAPH_VEC_COLOR);

    for (size_t vector = 0; vector < graph->vectorsCount; vector++)
    {
        renderVector(renderer, graph, rect, *graph->vectors[vector]);
    }
    
    // ================ Render functions ================
    setDrawColor(renderer, GRAPH_FUNC_COLOR);
    
    for (size_t function = 0; function < graph->functionsCount; function++)
    {
        renderFunction(renderer, graph, rect, *graph->functions[function]);
    }

    SDL_RenderSetClipRect(renderer, nullptr);
}

Vec2f32 toPixels(const Graph* graph, const SDL_Rect* rect, Vec2f32 point)
{
    assert(graph);
    assert(rect);

    float relativeWidth  = graph->axesMax.x - graph->axesMin.x;
    float relativeHeight = graph->axesMax.y - graph->axesMin.y;

    float x = rect->x + (float) rect->w / relativeWidth  * (point.x - graph->axesMin.x);
    float y = rect->y - (float) rect->h / relativeHeight * (point.y - graph->axesMax.y);

    return {x, y};
}

void renderVector(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, Vector vector)
{
    assert(renderer);
    assert(graph);
    assert(rect);

    float relativeHeight = graph->axesMax.y - graph->axesMin.y;
    float relativeWidth  = graph->axesMax.x - graph->axesMin.x;

    Vec2f32 from = toPixels(graph, rect, vector.origin);
    Vec2f32 to   = toPixels(graph, rect, add(&vector.origin, &vector.disp));

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

void renderFunction(SDL_Renderer* renderer, const Graph* graph, const SDL_Rect* rect, 
                    Function function)
{
    assert(renderer);
    assert(graph);
    assert(rect);
    assert(function);

    float   dx        = STEP_PIXELS * (graph->axesMax.x - graph->axesMin.x) / rect->w;
    Vec2f32 prevPoint = toPixels(graph, rect, {graph->axesMin.x, function(graph->axesMin.x)});

    for (float x = graph->axesMin.x; x <= graph->axesMax.x; x += dx)
    {
        Vec2f32 point = toPixels(graph, rect, {x, function(x)});
        SDL_RenderDrawLineF(renderer, prevPoint.x, prevPoint.y, point.x, point.y);
        prevPoint = point;
    }
}