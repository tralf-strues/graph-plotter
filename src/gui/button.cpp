//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "button.h"

EventButtonPressed::EventButtonPressed(Button* button)
    : Event(Event::GUI_BUTTON_PRESSED), button(button)
{
    assert(button);
}

Button::Button(const Vec2<int32_t>& pos,
               size_t width, size_t height,
               Color color)
    : GUIComponent(pos), width(width), height(height), color(color), label(nullptr)
{}

void Button::setLabel(const Text* label)
{
    assert(label);

    this->label = label;
}

void Button::onEvent(const Event& event)
{
    assert(event.type == Event::MOUSE_BUTTON_PRESSED);

    const EventMouseButton& mouseEvent = (const EventMouseButton&) event;

    if (mouseEvent.clickX >= pos.x && mouseEvent.clickX < pos.x + static_cast<int32_t>(width) &&
        mouseEvent.clickY >= pos.y && mouseEvent.clickY < pos.y + static_cast<int32_t>(height))
    {
        EventButtonPressed eventButtonPressed{this};
        notify(eventButtonPressed);
    }
}

void Button::attachToSystemEventManager(SystemEventManager& manager)
{
    manager.attachListener({Event::MOUSE_BUTTON_PRESSED}, this);
}

void Button::render(Renderer& renderer)
{
    Rectangle rect{pos, static_cast<int32_t>(width), static_cast<int32_t>(height)};

    renderer.setColor(color);
    renderFilledRect(renderer, rect);

    if (label != nullptr)
    {
        Vec2<int32_t> margin{(width  - label->getWidth())  / 2,
                             (height - label->getHeight()) / 2};

        label->render(renderer, pos + margin);
    }
}