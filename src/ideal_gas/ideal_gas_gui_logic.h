//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file ideal_gas_gui_logic.h
//! @date 2021-10-05
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef IDEAL_GAS_GUI_LOGIC_H
#define IDEAL_GAS_GUI_LOGIC_H

#include "../gui/activity_monitor.h"
#include "../gui/button.h"
#include "../gui/text_label.h"
#include "simulator.h"

static const float ELECTRIC_FIELD_CHANGE_STEP = 500;

struct QuitListener : public IListener
{
    bool* running;

    void onEvent(const Event& event) override
    {
        switch (event.type)
        {
            case Event::WINDOW_CLOSE:
            { 
                *running = false;
                break;
            }

            case Event::KEYBOARD_PRESSED:
            {
                if (((const EventKeyboardPressed&) event).scancode == SCANCODE_ESCAPE)
                {
                    *running = false;
                }
                break;
            }

            default: { break; }
        }
    }
};

struct ElectrodeButtonListener : public IListener
{
    enum Type
    {
        INVALID,
        
        INCREASE_ELECTRIC_FIELD,
        DECREASE_ELECTRIC_FIELD,
    };

    Type           type;
    GUI_TextLabel* label;
    Wall*          electrode;

    void onEvent(const Event& event) override
    {
        assert(event.type == Event::GUI_BUTTON_PRESSED);

        float electricField = electrode->getElectricField();

        switch (type)
        {
            case INCREASE_ELECTRIC_FIELD: 
            {
                electrode->setElectricField(electricField + ELECTRIC_FIELD_CHANGE_STEP);
                break;
            }
            
            case DECREASE_ELECTRIC_FIELD:
            {
                electrode->setElectricField(electricField - ELECTRIC_FIELD_CHANGE_STEP);
                break;
            }

            default: { assert("Invalid behavior type!"); break; }
        };

        static char labelText[32];
        snprintf(labelText, sizeof(labelText), "%g", electrode->getElectricField());

        label->updateText(labelText);
    }
};

struct SpawnButtonListener : public IListener
{
    enum Type
    {
        INVALID,
        SPAWN_ATOM,
        SPAWN_ELECTRON,
        SPAWN_PION,
        SPAWN_NION
    };

    Simulator*      simulator;
    const Viewport* viewport;
    Type            spawnEntityType;

    SpawnButtonListener(Simulator* simulator, const Viewport* viewport, Type spawnEntityType)
        : simulator(simulator),
          viewport(viewport),
          spawnEntityType(spawnEntityType) 
    {}

    void onEvent(const Event& event) override
    {
        assert(event.type == Event::GUI_BUTTON_PRESSED);

        printf("SpawnButtonListener\n");

        float radius = randomFromInterval<float>(0.4, 1.2);
        float volume = calculateSphereVolume(radius);

        Vec2<float> pos{randomFromInterval<float>(2 * radius, viewport->getRelativeWidth()  - 2 * radius),
                        randomFromInterval<float>(2 * radius, viewport->getRelativeHeight() - 2 * radius)};

        Vec2<float> velocity{randomFromInterval<float>(-3e4, 3e4),
                             randomFromInterval<float>(-3e4, 3e4)};

        if (spawnEntityType == SPAWN_ELECTRON)
        {
            Electron* electron = new Electron{0.2f};
            electron->setPos(pos);
            electron->setVelocity(velocity);

            simulator->entities.pushBack(electron);
        }
        else
        {
            Atom* atom = new Atom{radius};
            atom->setMass(volume * ATOM_DENSITY);
            atom->setPos(pos);
            atom->setVelocity(velocity);

            if (spawnEntityType == SPAWN_PION)
            {
                atom->setCharge(-ELECTRON_CHARGE);
            }
            else if (spawnEntityType == SPAWN_NION)
            {
                atom->setCharge(ELECTRON_CHARGE);
            }

            simulator->entities.pushBack(atom);
        }
    }
};

#endif // IDEAL_GAS_GUI_LOGIC_H