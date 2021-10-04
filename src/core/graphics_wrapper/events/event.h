//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file event.h
//! @date 2021-10-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef EVENT_H
#define EVENT_H

#include <inttypes.h>
#include "scancode.h"

struct Event
{
    enum Type
    {
        INVALID,

        /* Window events */
        WINDOW_CLOSE,

        /* Mouse events */
        MOUSE_BUTTON_PRESSED,

        /* Keyboard events */
        KEYBOARD_PRESSED,

        /* GUI */
        GUI_BUTTON_PRESSED,

        TOTAL_TYPES
    };

    const Type type;

    Event(Type type = INVALID) : type(type) {}
};

struct EventMouseButton : public Event
{
    int32_t clickX;
    int32_t clickY;

    EventMouseButton(int32_t clickX = 0, int32_t clickY = 0, Type type = MOUSE_BUTTON_PRESSED) : 
                     Event(type), clickX(clickX), clickY(clickY) {}
};

struct EventKeyboardPressed : public Event
{
    Scancode scancode;

    EventKeyboardPressed(Scancode scancode = SCANCODE_INVALID, Type type = KEYBOARD_PRESSED) : 
                         Event(type), scancode(scancode) {}
};

#endif // EVENT_H