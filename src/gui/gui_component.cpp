//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_component.cpp
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "gui_component.h"

GUIComponent::GUIComponent(const Vec2<int32_t>& pos) : m_Pos(pos) {}

const Vec2<int32_t>& GUIComponent::getPos() const
{
    return m_Pos;
}

void GUIComponent::setPos(const Vec2<int32_t>& pos)
{
    m_Pos = pos;
}