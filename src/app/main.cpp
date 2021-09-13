//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file main.cpp
//! @date 2021-09-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "app.h"

int main()
{   
    initGraphics();

    App app;
    app.run();

    quitGraphics();

    return 0;
}