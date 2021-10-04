//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file listener_notifier.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "listener_notifier.h"

void Notifier::attachListener(const std::initializer_list<Event::Type>& events, IListener* listener)
{
    listeners.pushBack(ListenerInfo(events, listener));
}

void Notifier::detachListener(IListener* listener)
{
    for (ListIterator it = listeners.begin(); it != listeners.end(); ++it)
    {
        if (it->listener == listener)
        {
            listeners.remove(it);
            return;
        }
    }
}

void Notifier::notify(const Event& event)
{
    for (auto listenerInfo : listeners)
    {
        for (auto eventType : listenerInfo.events)
        {
            if (eventType == event.type)
            {
                listenerInfo.listener->onEvent(event);
            }
        }
    }
}