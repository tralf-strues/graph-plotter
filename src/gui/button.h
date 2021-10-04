//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.h
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "../core/graphics_wrapper/text.h"
#include "gui_component.h"

#ifndef BUTTON_H
#define BUTTON_H

struct Button;

struct EventButtonPressed : public Event
{
    Button* button;

    EventButtonPressed(Button* button);
};

struct Button : public GUIComponent, public Notifier
{
    size_t      width;
    size_t      height;
    Color       color;
    const Text* label;

    Button(const Vec2<int32_t>& pos,
           size_t width, size_t height,
           Color color = COLOR_BLACK);

    void setLabel(const Text* label);

    /* GUIComponent */
    void attachToSystemEventManager(SystemEventManager& manager) override;
    void render(Renderer& renderer) override;

protected:
    /* IListener */
    void onEvent(const Event& event) override;
};

#endif // BUTTON_H