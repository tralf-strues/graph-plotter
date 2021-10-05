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

class GUI_Button;

struct EventButtonPressed : public Event
{
    GUI_Button* button;

    EventButtonPressed(GUI_Button* button);
};

class GUI_Button : public GUI_Component, public SystemEventManagerListener, public Notifier
{
public:
    GUI_Button(const Vec2<int32_t>& pos,
           size_t width, size_t height,
           Color color = COLOR_BLACK);

    size_t getWidth() const;
    void setWidth(size_t width);

    size_t getHeight() const;
    void setHeight(size_t height);

    Color getColor() const;
    void setColor(Color color);

    const Text* getLabel() const;
    void setLabel(Renderer& renderer, const char* label, const Font& font, Color color = COLOR_WHITE);

    /* SystemEventManagerListener */
    void attachToSystemEventManager(SystemEventManager& manager) override;

    /* GUI_Component */
    void render(Renderer& renderer) override;

protected:
    size_t m_Width;
    size_t m_Height;
    Color  m_Color;
    Text*  m_Label;

    /* SystemEventManagerListener */
    void onEvent(const Event& event) override;
};

#endif // BUTTON_H