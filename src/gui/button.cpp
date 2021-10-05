//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "button.h"

EventButtonPressed::EventButtonPressed(GUI_Button* button)
    : Event(Event::GUI_BUTTON_PRESSED), button(button)
{
    assert(button);
}

GUI_Button::GUI_Button(const Vec2<int32_t>& pos, size_t width, size_t height, Color color)
    : GUI_Component(pos), m_Width(width), m_Height(height), m_Color(color), m_Label(nullptr)
{}

size_t GUI_Button::getWidth() const
{
    return m_Width;
}

void GUI_Button::setWidth(size_t width)
{
    m_Width = width;
}

size_t GUI_Button::getHeight() const
{
    return m_Height;
}

void GUI_Button::setHeight(size_t height)
{
    m_Height = height;
}

Color GUI_Button::getColor() const
{
    return m_Color;
}

void GUI_Button::setColor(Color color)
{
    m_Color = color;
}

const Text* GUI_Button::getLabel() const
{
    return m_Label;
}

void GUI_Button::setLabel(Renderer& renderer, const char* label, const Font& font, Color color)
{
    assert(label);
    assert(font.getNativeFont());

    if (m_Label != nullptr)
    {
        delete m_Label;
    }

    m_Label = new Text;

    m_Label->load(renderer, label, font, color);
}

void GUI_Button::onEvent(const Event& event)
{
    assert(event.type == Event::MOUSE_BUTTON_PRESSED);

    const EventMouseButton& mouseEvent = (const EventMouseButton&) event;

    if (mouseEvent.clickX >= m_Pos.x && mouseEvent.clickX < m_Pos.x + static_cast<int32_t>(m_Width) &&
        mouseEvent.clickY >= m_Pos.y && mouseEvent.clickY < m_Pos.y + static_cast<int32_t>(m_Height))
    {
        EventButtonPressed eventButtonPressed{this};
        notify(eventButtonPressed);
    }
}

void GUI_Button::attachToSystemEventManager(SystemEventManager& manager)
{
    manager.attachListener({Event::MOUSE_BUTTON_PRESSED}, this);
}

void GUI_Button::render(Renderer& renderer)
{
    Rectangle rect{m_Pos, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height)};

    renderer.setColor(m_Color);
    renderFilledRect(renderer, rect);

    if (m_Label != nullptr)
    {
        Vec2<int32_t> margin{(m_Width  - m_Label->getWidth())  / 2,
                             (m_Height - m_Label->getHeight()) / 2};

        m_Label->render(renderer, m_Pos + margin);
    }
}