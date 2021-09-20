#include <assert.h>
#include <math.h>
#include "graph.h"

const float STEP_PIXELS = 0.1f;

Vec2<float> toPixels(const Graph& graph, const Rectangle& region, Vec2<float> point);
void renderVector(Renderer& renderer, const Graph& graph, const Rectangle& region, 
                  const Graph::Vector& vector);
void renderFunction(Renderer& renderer, const Graph& graph, const Rectangle& region, 
                    Graph::Function function);

Graph::Graph(const Vec2<float>& axesMin, const Vec2<float>& axesMax) : axesMin(axesMin), 
                                                                       axesMax(axesMax), 
                                                                       functionsCount(0), 
                                                                       vectorsCount(0)
{
    assert(this->axesMin.x < this->axesMax.x);
    assert(this->axesMin.y < this->axesMax.y);
}

bool Graph::addFunction(Function function)
{
    assert(function);
    
    if (functionsCount >= GRAPH_MAX_FUNCTIONS)
    {
        return false;
    }

    functions[functionsCount++] = function;

    return true;
}

bool Graph::addVector(Vector* vector)
{
    assert(vector);
    
    if (vectorsCount >= GRAPH_MAX_VECTORS)
    {
        return false;
    }

    vectors[vectorsCount++] = vector;

    return true;
}

const Vec2<float>& Graph::getAxesMin() const
{
    return axesMin;
}

void Graph::setAxesMin(const Vec2<float>& axesMin)
{
    this->axesMin = axesMin;
}

const Vec2<float>& Graph::getAxesMax() const
{
    return axesMax;
}

void Graph::setAxesMax(const Vec2<float>& axesMax)
{
    this->axesMax = axesMax;
}

size_t Graph::getFunctionsCount() const
{
    return functionsCount;
}

size_t Graph::getVectorsCount() const
{
    return vectorsCount;
}

void Graph::render(Renderer& renderer, const Rectangle& region) const
{
    renderer.setClipRegion(region);

    float relHeight = axesMax.y - axesMin.y;
    float relWidth  = axesMax.x - axesMin.x;

    // ================ Render frame ================
    renderer.setColor(GRAPH_FRAME_COLOR);
    renderRect(renderer, region);

    // ================ Render axes ================
    renderer.setColor(GRAPH_AXES_COLOR);

    // y axis
    renderVector(renderer, *this, region, {{0, axesMin.y}, {0, relHeight}});
    
    // x axis
    renderVector(renderer, *this, region, {{axesMin.x, 0}, {relWidth, 0}});

    // ================ Render vectors ================
    renderer.setColor(GRAPH_VEC_COLOR);

    for (size_t vector = 0; vector < vectorsCount; vector++)
    {
        renderVector(renderer, *this, region, *vectors[vector]);
    }

    // ================ Render functions ================
    renderer.setColor(GRAPH_FUNC_COLOR);
    
    for (size_t function = 0; function < functionsCount; function++)
    {
        renderFunction(renderer, *this, region, functions[function]);
    }

    renderer.resetClipRegion();
}

Vec2<float> toPixels(const Graph& graph, const Rectangle& region, Vec2<float> point)
{
    float relHeight = graph.getAxesMax().y - graph.getAxesMin().y;
    float relWidth  = graph.getAxesMax().x - graph.getAxesMin().x;

    float x = region.pos.x + (float) region.width  / relWidth  * (point.x - graph.getAxesMin().x);
    float y = region.pos.y - (float) region.height / relHeight * (point.y - graph.getAxesMax().y);

    return {x, y};
}

void renderVector(Renderer& renderer, const Graph& graph, const Rectangle& region, 
                  const Graph::Vector& vector)
{
    Vec2<float> from = toPixels(graph, region, vector.origin);
    Vec2<float> to   = toPixels(graph, region, vector.origin + vector.disp);

    Vec2<float> vectorWindowCoords = to - from;

    float xcos = vectorWindowCoords.x * cosf(GRAPH_ARROWHEAD_ANGLE);
    float xsin = vectorWindowCoords.x * sinf(GRAPH_ARROWHEAD_ANGLE);
    float ycos = vectorWindowCoords.y * cosf(GRAPH_ARROWHEAD_ANGLE);
    float ysin = vectorWindowCoords.y * sinf(GRAPH_ARROWHEAD_ANGLE);

    Vec2<float> head1(xcos - ysin, ycos + xsin);
    head1 *= -GRAPH_ARROWHEAD_LEN / length(head1);

    Vec2<float> head2(xcos + ysin, ycos - xsin);
    head2 *= -GRAPH_ARROWHEAD_LEN / length(head2);

    renderLine(renderer, from, to);
    renderLine(renderer, to, to + head1);
    renderLine(renderer, to, to + head2);
}

void renderFunction(Renderer& renderer, const Graph& graph, const Rectangle& region, 
                    Graph::Function function)
{
    const Vec2<float>& axesMin = graph.getAxesMin();
    const Vec2<float>& axesMax = graph.getAxesMax();

    float dx = STEP_PIXELS * (axesMax.x - axesMin.x) / region.width;
    Vec2<float> prevPoint = toPixels(graph, region, {axesMin.x, function(axesMin.x)});

    for (float x = axesMin.x; x <= axesMax.x; x += dx)
    {
        Vec2<float> point = toPixels(graph, region, {x, function(x)});
        renderLine(renderer, prevPoint, point);

        prevPoint = point;
    }
}