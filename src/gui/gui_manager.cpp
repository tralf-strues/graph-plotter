//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_manager.cpp
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "gui_manager.h"

void GUI_Manager::addComponent(GUI_Component* component)
{
    m_Components.pushBack(component);
}

void GUI_Manager::removeComponent(GUI_Component* component)
{
    auto it = m_Components.find(component);

    if (it != m_Components.end())
    {
        m_Components.remove(it);
    }
}

void GUI_Manager::renderComponents()
{
    for (auto component : m_Components)
    {
        component->render();
    }
}