//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_component.h
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"

class GUI_Component
{
public:
    GUI_Component(Renderer& renderer, const Vec2<int32_t>& pos)
        : m_Renderer(renderer), m_Pos(pos) {}

    const Vec2<int32_t>& getPos() const   { return m_Pos; }
    void setPos(const Vec2<int32_t>& pos) { m_Pos = pos;  }

    virtual void render() = 0;

protected:
    Vec2<int32_t> m_Pos;
    Renderer&     m_Renderer;
};

class SystemEventManagerListener : protected IListener
{
public:
    virtual void attachToSystemEventManager(SystemEventManager& manager) = 0;

protected:
    /* IListener */
    virtual void onEvent(const Event& event) = 0;
};

#endif // GUI_COMPONENT_H