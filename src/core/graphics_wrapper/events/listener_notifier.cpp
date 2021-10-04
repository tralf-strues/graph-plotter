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
    assert(listener);

    m_Listeners.pushBack(ListenerInfo(events, listener));
}

void Notifier::detachListener(IListener* listener)
{
    assert(listener);

    for (ListIterator it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
    {
        if (it->listener == listener)
        {
            m_Listeners.remove(it);
            return;
        }
    }
}

void Notifier::notify(const Event& event)
{
    for (auto listenerInfo : m_Listeners)
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