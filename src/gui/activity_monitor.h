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

static const size_t  GUI_ACTIVITY_MONITOR_MAX_LABEL_SIZE  = 32;
static const Color   GUI_ACTIVITY_MONITOR_LABEL_COLOR     = COLOR_WHITE; // FIXME: parameter
static const int32_t GUI_ACTIVITY_MONITOR_LABELS_MARGIN_X = 5; 

class GUI_ActivityMonitor : public GUI_Component
{
public:
    GUI_ActivityMonitor(const Viewport& viewport, size_t samplesCount);

    size_t getSamplesCount() const;

    const Viewport& getViewport() const;
    void setViewport(const Viewport& viewport);

    void updateLabels(Renderer& renderer, Font font);

    void addSample(float sample);

    /* GUI_Component */
    void render(Renderer& renderer) override;

private:
    size_t      m_SamplesCount;
    List<float> m_Samples;
    Viewport    m_Viewport;

    Text        m_LabelMin;
    Text        m_LabelMax;
};

#endif // ACTIVITY_MONITOR_H