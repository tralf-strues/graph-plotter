//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file activity_monitor.cpp
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "activity_monitor.h"

GUIActivityMonitor::GUIActivityMonitor(const Viewport& viewport, size_t samplesCount)
    : GUIComponent(viewport.windowArea.pos), m_SamplesCount(samplesCount), 
      m_Samples(m_SamplesCount), m_Viewport(viewport)
{
    assert(m_SamplesCount != 0);
}

size_t GUIActivityMonitor::getSamplesCount() const
{
    return m_SamplesCount;
}

const Viewport& GUIActivityMonitor::getViewport() const
{
    return m_Viewport;
}

void GUIActivityMonitor::setViewport(const Viewport& viewport)
{
    m_Viewport = viewport;
}

void GUIActivityMonitor::addSample(float sample)
{
    if (m_Samples.getSize() == m_SamplesCount)
    {
        m_Samples.popFront();
    }

    m_Samples.pushBack(sample);
}

void GUIActivityMonitor::render(Renderer& renderer)
{
    // FIXME: parameter
    renderer.setColor(COLOR_YELLOW);

    renderRect(renderer, m_Viewport.windowArea);

    float xStep = m_Viewport.getRelativeWidth() / (m_SamplesCount - 1);

    Vec2<int32_t> prevPoint = m_Viewport.toPixels(Vec2<float>{0, *m_Samples.begin()});;

    ListIterator<float> it = m_Samples.begin();
    ++it;

    for (size_t sample = 1; sample <= m_SamplesCount && it != m_Samples.end(); ++sample, ++it)
    {
        Vec2<int32_t> nextPoint = m_Viewport.toPixels(Vec2<float>{xStep * sample, *(it)});
        renderLine(renderer, prevPoint, nextPoint);

        prevPoint = nextPoint;
    }
}