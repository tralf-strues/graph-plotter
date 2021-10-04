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

struct GUIComponent : protected IListener
{
    Vec2<int32_t> pos;

    GUIComponent(const Vec2<int32_t>& pos) : pos(pos) {}

    virtual void attachToSystemEventManager(SystemEventManager& manager) = 0;
    virtual void render(Renderer& renderer) = 0;

protected:
    
    /* IListener */
    virtual void onEvent(const Event& event) = 0;
};

#endif // GUI_COMPONENT_H