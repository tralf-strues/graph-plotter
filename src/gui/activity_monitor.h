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

class GUIActivityMonitor : public GUIComponent
{
public:
    GUIActivityMonitor(const Viewport& viewport, size_t samplesCount);

    size_t getSamplesCount() const;

    const Viewport& getViewport() const;
    void setViewport(const Viewport& viewport);

    void addSample(float sample);

    /* GUIComponent */
    void render(Renderer& renderer) override;

private:
    size_t      m_SamplesCount;
    List<float> m_Samples;

    Viewport    m_Viewport;
};

#endif // ACTIVITY_MONITOR_H