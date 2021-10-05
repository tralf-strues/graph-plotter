//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_manager.h
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "gui_component.h"

class GUI_Manager
{
public:
    void addComponent(GUI_Component* component);
    void removeComponent(GUI_Component* component);

    void renderComponents(Renderer& renderer);

private:
    List<GUI_Component*> m_Components;
};

#endif // GUI_MANAGER_H