//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file activity_monitor.cpp
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "activity_monitor.h"

GUI_ActivityMonitor::GUI_ActivityMonitor(Renderer& renderer,
                                         const Viewport& viewport,
                                         size_t samplesCount,
                                         Color frameColor,
                                         Color lineColor,
                                         Color textColor)
    : GUI_Component(renderer, viewport.windowArea.pos), 
      m_SamplesCount(samplesCount), 
      m_Samples(m_SamplesCount),
      m_FrameColor(frameColor),
      m_LineColor(lineColor),
      m_TextColor(textColor),
      m_GraphViewport(viewport)
{
    assert(m_SamplesCount != 0);

    m_GraphViewport.windowArea.pos.y += GUI_ACTIVITY_MONITOR_TITLE_HEIGHT;
}

size_t GUI_ActivityMonitor::getSamplesCount() const
{
    return m_SamplesCount;
}

void GUI_ActivityMonitor::setColors(Color frameColor, Color lineColor, Color textColor)
{
    setFrameColor(frameColor);
    setLineColor(lineColor);
    setTextColor(textColor);
}

Color GUI_ActivityMonitor::getFrameColor() const { return m_FrameColor; }
Color GUI_ActivityMonitor::getLineColor()  const { return m_LineColor;  }
Color GUI_ActivityMonitor::getTextColor()  const { return m_TextColor;  }

void GUI_ActivityMonitor::setFrameColor(Color color) { m_FrameColor = color; }
void GUI_ActivityMonitor::setLineColor(Color color)  { m_LineColor  = color; }
void GUI_ActivityMonitor::setTextColor(Color color)  { m_TextColor  = color; }

const Viewport& GUI_ActivityMonitor::getViewport() const
{
    return m_GraphViewport;
}

void GUI_ActivityMonitor::setViewport(const Viewport& viewport)
{
    m_GraphViewport = viewport;
}

void GUI_ActivityMonitor::setValueRange(float minY, float maxY)
{
    m_GraphViewport.axesMin.y = minY;
    m_GraphViewport.axesMax.y = maxY;
}

void GUI_ActivityMonitor::setTitle(const char* title, const Font& font)
{
    removeTitle();
    m_Title.load(m_Renderer, title, font, m_TextColor);

    m_TitleRenderRect.pos.x = m_GraphViewport.windowArea.pos.x +
                              (m_GraphViewport.windowArea.width - m_Title.getWidth()) / 2;

    m_TitleRenderRect.pos.y = m_GraphViewport.windowArea.pos.y - GUI_ACTIVITY_MONITOR_TITLE_HEIGHT +
                              (GUI_ACTIVITY_MONITOR_TITLE_HEIGHT - m_Title.getHeight()) / 2;

    m_TitleRenderRect.width  = m_Title.getWidth();
    m_TitleRenderRect.height = m_Title.getHeight();
}

const Text* GUI_ActivityMonitor::getTitle() const
{
    return &m_Title;
}

void GUI_ActivityMonitor::removeTitle()
{
    if (m_Title.getStr() != nullptr)
    {
        m_Title.destroy();
    }
}

void GUI_ActivityMonitor::updateLabels(const Font& font)
{
    m_LabelMin.destroy();
    m_LabelMax.destroy();

    static char labelStr[GUI_ACTIVITY_MONITOR_MAX_LABEL_SIZE];

    snprintf(labelStr, GUI_ACTIVITY_MONITOR_MAX_LABEL_SIZE, "%g", m_GraphViewport.axesMin.y);
    m_LabelMin.load(m_Renderer, labelStr, font, m_TextColor);

    snprintf(labelStr, GUI_ACTIVITY_MONITOR_MAX_LABEL_SIZE, "%g", m_GraphViewport.axesMax.y);
    m_LabelMax.load(m_Renderer, labelStr, font, m_TextColor);
}

void GUI_ActivityMonitor::addSample(float sample)
{
    if (m_Samples.getSize() == m_SamplesCount)
    {
        m_Samples.popFront();
    }

    m_Samples.pushBack(sample);
}

void GUI_ActivityMonitor::render()
{
    float xStep = m_GraphViewport.getRelativeWidth() / (m_SamplesCount - 1);
    Vec2<int32_t> prevPoint = m_GraphViewport.toPixels(Vec2<float>{0, *m_Samples.begin()});;

    ListIterator<float> it = m_Samples.begin();
    ++it;

    m_Renderer.setColor(m_LineColor);
    for (size_t sample = 1; sample <= m_SamplesCount && it != m_Samples.end(); ++sample, ++it)
    {
        Vec2<int32_t> nextPoint = m_GraphViewport.toPixels(Vec2<float>{xStep * sample, *(it)});
        renderLine(m_Renderer, prevPoint, nextPoint);

        prevPoint = nextPoint;
    }

    if (m_LabelMax.getStr() != nullptr)
    {
        m_LabelMax.render(m_Renderer, Vec2<int32_t>{m_GraphViewport.windowArea.pos.x +
                                                  m_GraphViewport.windowArea.width +
                                                  GUI_ACTIVITY_MONITOR_LABELS_MARGIN_X,
                                                  m_GraphViewport.windowArea.pos.y});
    }

    if (m_LabelMin.getStr() != nullptr)
    {
        m_LabelMin.render(m_Renderer, Vec2<int32_t>{m_GraphViewport.windowArea.pos.x +
                                                  m_GraphViewport.windowArea.width +
                                                  GUI_ACTIVITY_MONITOR_LABELS_MARGIN_X,
                                                  m_GraphViewport.windowArea.pos.y +
                                                  m_GraphViewport.windowArea.height -
                                                  m_LabelMax.getHeight()});
    }

    if (m_Title.getStr() != nullptr)
    {
        m_Title.render(m_Renderer, m_TitleRenderRect.pos);
    }

    m_Renderer.setColor(m_FrameColor);
    renderRect(m_Renderer, m_GraphViewport.windowArea);
}