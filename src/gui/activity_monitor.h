//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file activity_monitor.h
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef ACTIVITY_MONITOR_H
#define ACTIVITY_MONITOR_H

#include "../core/utils/viewport.h"
#include "gui_component.h"

static const int32_t GUI_ACTIVITY_MONITOR_TITLE_HEIGHT    = 30;
static const size_t  GUI_ACTIVITY_MONITOR_MAX_LABEL_SIZE  = 32;
static const int32_t GUI_ACTIVITY_MONITOR_LABELS_MARGIN_X = 5; 

class GUI_ActivityMonitor : public GUI_Component
{
public:
    GUI_ActivityMonitor(const Viewport& viewport,
                        size_t samplesCount,
                        Color frameColor = COLOR_BLACK,
                        Color lineColor  = COLOR_BLACK,
                        Color textColor  = COLOR_BLACK);

    size_t getSamplesCount() const;

    void setColors(Color frameColor, Color lineColor, Color textColor);

    Color getFrameColor() const;
    void setFrameColor(Color color);

    Color getLineColor() const;
    void setLineColor(Color color);

    Color getTextColor() const;
    void setTextColor(Color color);

    const Viewport& getViewport() const;
    void setViewport(const Viewport& viewport);

    void setValueRange(float minY, float maxY);

    void setTitle(Renderer& renderer, const Font& font, const char* title);
    const Text* getTitle() const;
    void removeTitle();

    void updateLabels(Renderer& renderer, const Font& font);

    void addSample(float sample);

    /* GUI_Component */
    void render(Renderer& renderer) override;

private:
    size_t      m_SamplesCount = 0;
    List<float> m_Samples;

    Color       m_FrameColor;
    Color       m_LineColor;
    Color       m_TextColor;

    Rectangle   m_TitleRenderRect;
    Viewport    m_GraphViewport;

    Text        m_Title;
    Text        m_LabelMin;
    Text        m_LabelMax;
};

#endif // ACTIVITY_MONITOR_H