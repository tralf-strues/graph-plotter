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

class GUIButton;

struct EventButtonPressed : public Event
{
    GUIButton* button;

    EventButtonPressed(GUIButton* button);
};

class GUIButton : public GUIComponent, public SystemEventManagerListener, public Notifier
{
public:
    GUIButton(const Vec2<int32_t>& pos,
           size_t width, size_t height,
           Color color = COLOR_BLACK);

    size_t getWidth() const;
    void setWidth(size_t width);

    size_t getHeight() const;
    void setHeight(size_t height);

    Color getColor() const;
    void setColor(Color color);

    const Text* getLabe() const;
    void setLabel(const Text* label);

    /* SystemEventManagerListener */
    void attachToSystemEventManager(SystemEventManager& manager) override;

    /* GUIComponent */
    void render(Renderer& renderer) override;

protected:
    size_t      m_Width;
    size_t      m_Height;
    Color       m_Color;
    const Text* m_Label;

    /* SystemEventManagerListener */
    void onEvent(const Event& event) override;
};

#endif // BUTTON_H