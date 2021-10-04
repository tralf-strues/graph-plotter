//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file system_event_manager.cpp
//! @date 2021-10-04
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <SDL.h>
#include "system_event_manager.h"

void SystemEventManager::proccessEvents()
{
    SDL_Event nativeEvent = {};
    while (SDL_PollEvent(&nativeEvent))
    {
        switch (nativeEvent.type)
        {
            case SDL_QUIT: 
            { 
                Event event{Event::WINDOW_CLOSE};
                notify(event);
                break; 
            }

            case SDL_KEYDOWN:
            {
                EventKeyboardPressed event{(Scancode) nativeEvent.key.keysym.scancode};
                notify(event);
                break;
            }

            default: { break; }
        }
    }
}