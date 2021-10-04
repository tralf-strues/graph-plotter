//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file listener_notifier.h
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef LISTENER_NOTIFIER_H
#define LISTENER_NOTIFIER_H

#include "../../containers/_core_containers.h"
#include "event.h"

static const size_t MAX_LISTENED_EVENTS = 3;

class IListener
{
public:
    virtual void onEvent(const Event& event) = 0;
};

class Notifier
{
public:
    virtual void attachListener(const std::initializer_list<Event::Type>& events, 
                                IListener* listener);
    virtual void detachListener(IListener* listener);

protected:
    struct ListenerInfo
    {
        Event::Type events[MAX_LISTENED_EVENTS];
        IListener*  listener;

        ListenerInfo(IListener* listener = nullptr) : listener(listener)
        {
            for (size_t i = 0; i < MAX_LISTENED_EVENTS; ++i)
            {
                this->events[i] = Event::INVALID;
            }
        }

        ListenerInfo(const std::initializer_list<Event::Type>& events, 
                     IListener* listener = nullptr)
            : ListenerInfo(listener) 
        {
            assert(events.size() <= MAX_LISTENED_EVENTS);

            auto initIt = events.begin();
            for (size_t i = 0; initIt != events.end(); ++initIt, ++i)
            {
                this->events[i] = *initIt;
            }
        }
    };

    List<ListenerInfo> m_Listeners;

    virtual void notify(const Event& event);
};

#endif // LISTENER_NOTIFIER_H