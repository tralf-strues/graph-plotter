//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file system_event_manager.h
//! @date 2021-10-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SYSTEM_EVENT_MANAGER_H
#define SYSTEM_EVENT_MANAGER_H

#include "../../containers/list.h"
#include "listener_notifier.h"

class SystemEventManager : public Notifier
{
public:
    void proccessEvents();
};

#endif // SYSTEM_EVENT_MANAGER_H