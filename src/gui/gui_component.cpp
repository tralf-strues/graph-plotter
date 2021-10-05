//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_component.cpp
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "gui_component.h"

GUI_Component::GUI_Component(const Vec2<int32_t>& pos) : m_Pos(pos) {}

const Vec2<int32_t>& GUI_Component::getPos() const
{
    return m_Pos;
}

void GUI_Component::setPos(const Vec2<int32_t>& pos)
{
    m_Pos = pos;
}