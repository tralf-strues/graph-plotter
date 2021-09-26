//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.h
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../core/graphics_wrapper/_core_graphics_wrapper.h"
#include "../core/containers/dynamic_array.h"
#include "../core/math/_core_math.h"

struct Molecule
{
    Molecule(float radius = 1, float mass = 1, 
             Vec2<float> pos = {0, 0}, Vec2<float> velocity = {0, 0});

    float       radius;
    float       mass;
    Vec2<float> pos;
    Vec2<float> velocity;
};

//------------------------------------------------------------------------------
//! @brief Line given by the equation ax + by + c = 0.
//------------------------------------------------------------------------------
struct Line
{
    Vec2<float> from;
    Vec2<float> direction;

    Vec2<float> calculateNormal() const;
};

struct Viewport
{
    Vec2<float> axesMin;
    Vec2<float> axesMax;

    float getRelativeWidth()  const;
    float getRelativeHeight() const;
};

struct Simulator
{
    DynamicArray<Molecule*> molecules;
    DynamicArray<Line*>     lines;

    void processObjects();
};

bool collide(const Molecule& molecule, const Line& line, float deltaTime, float* collisionTime);
void collisionRespond(Molecule& molecule, Line& line, float collisionTime);

void drawMolecule(Renderer& renderer, const Viewport& viewport, const Molecule& molecule);

#endif // SIMULATOR_H