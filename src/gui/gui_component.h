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
    GUI_Component(const Vec2<int32_t>& pos);

    const Vec2<int32_t>& getPos() const;
    void setPos(const Vec2<int32_t>& pos);

    virtual void render(Renderer& renderer) = 0;

protected:
    Vec2<int32_t> m_Pos;
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