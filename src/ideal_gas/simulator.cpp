//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file simulator.cpp
//! @date 2021-09-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "simulator.h"

Vec2<float> toPixels(const Renderer& renderer, const Viewport& viewport, const Vec2<float>& point);

Molecule::Molecule(float radius, float mass, Vec2<float> pos, Vec2<float> velocity) :
                   radius(radius), mass(mass), pos(pos), velocity(velocity) {}

Vec2<float> Line::calculateNormal() const
{
    Vec2<float> normal = {0, 1};

    if (cmpFloat(direction.y, 0) != 0)
    {
        normal = {1, -direction.x / direction.y};
    }

    return normalize(normal);
}

float Viewport::getRelativeWidth() const
{
    return axesMax.x - axesMin.x;
}

float Viewport::getRelativeHeight() const
{
    return axesMax.y - axesMin.y;
}

void Simulator::processObjects()
{
    for (size_t i = 0; i < molecules.getSize(); i++)
    {
        for (size_t j = 0; j < lines.getSize(); j++)
        {
            float collisionTime = 0;
            bool  collided      = collide(*molecules[i], *lines[j], 0.001f, &collisionTime);

            if (collided)
            {
                
                collisionRespond(*molecules[i], *lines[j], collisionTime);
            }
        }
    }
}

bool collide(const Molecule& molecule, const Line& line, float deltaTime, float* collisionTime)
{
    assert(collisionTime);

    Vec2<float> normal = line.calculateNormal();

    float dotVelocityNormal = dotProduct(molecule.velocity, normal);
    float dotCenterNormal   = dotProduct(molecule.pos,      normal);
    float dotFromNormal     = dotProduct(line.from,         normal);
    float radiusSquared     = molecule.radius * molecule.radius;

    float a = dotVelocityNormal * dotVelocityNormal;
    float b = -2 * dotFromNormal * dotVelocityNormal;
    float c = dotFromNormal * dotFromNormal - 2 * dotFromNormal * dotCenterNormal - radiusSquared;

    float time1 = 0;
    float time2 = 0;

    int32_t solutions = solveQuadraticEquation(a, b, c, &time1, &time2);

    if (solutions != 1 && solutions != 2)
    {
        return false;
    }

    *collisionTime = (solutions == 2 && time2 < time1) ? time2 : time1;
    return cmpFloat(*collisionTime, deltaTime) <= 0;
}

void collisionRespond(Molecule& molecule, Line& line, float collisionTime)
{
    Vec2<float> normal = line.calculateNormal();

    float velocityPerpendicular = dotProduct(molecule.velocity, normal);
    float velocityAlong         = dotProduct(molecule.velocity, normalize(line.direction));

    molecule.pos      = collisionTime * molecule.velocity;
    molecule.velocity = velocityAlong * normalize(line.direction) - velocityPerpendicular * normal;
}

void drawMolecule(Renderer& renderer, const Viewport& viewport, const Molecule& molecule)
{
    Vec2<float> center = toPixels(renderer, viewport, molecule.pos);
    float       radius = molecule.radius * 
                         renderer.getWindow().getWidth() /
                         viewport.getRelativeWidth();

    Circle circle = {center, (int32_t) radius};
    renderCircle(renderer, circle);
}

Vec2<float> toPixels(const Renderer& renderer, const Viewport& viewport, const Vec2<float>& point)
{
    float relWidth  = viewport.getRelativeWidth();
    float relHeight = viewport.getRelativeHeight();

    float x =  (renderer.getWindow().getWidth() / relWidth)  * (point.x - viewport.axesMin.x); 
    float y = renderer.getWindow().getHeight() + (renderer.getWindow().getHeight() / relHeight * (point.y - viewport.axesMax.y)); 

    return {x, y};
}