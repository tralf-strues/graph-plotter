//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file phys_entity.cpp
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "phys_entity.h"

//----------------------------------PhysEntity----------------------------------
PhysEntity::PhysEntity(Type type, float mass, const Vec2<float>& pos, const Vec2<float>& velocity) :
                       type(type), mass(mass), pos(pos), velocity(velocity) {}

void PhysEntity::setPos(const Vec2<float> pos)
{
    this->pos = pos;
}

Vec2<float> PhysEntity::getPos() const
{
    return pos;
}

void PhysEntity::setVelocity(const Vec2<float> velocity)
{
    this->velocity = velocity;
}

Vec2<float> PhysEntity::getVelocity() const
{
    return velocity;
}

void PhysEntity::setMass(float mass)
{
    this->mass = mass;
}

float PhysEntity::getMass() const
{
    return mass;
}
//------------------------------------------------------------------------------

//-----------------------------------Electron-----------------------------------
Electron::Electron(float radius) : PhysEntity(ELECTRON), radius(radius)
{
    this->mass = ELECTRON_MASS;
}

float Electron::getEnergy() const
{
    float v = length(velocity);

    // FIXME:
    printf("Electron energy = %f\n", mass * v * v / 2 + ELECTRON_ENERGY);

    return mass * v * v / 2 + ELECTRON_ENERGY;
}

void Electron::move(float deltaTime)
{
    pos += velocity * deltaTime;
}

void Electron::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    renderer.setColor(COLOR_ELECTRON);

    Vec2<int32_t> circlePos    = viewport.toPixels(pos);
    int32_t       circleRadius = viewport.toPixels(radius);

    renderCircle(renderer, {circlePos, circleRadius});
}

void Electron::setRadius(float radius)
{
    this->radius = radius;
}

float Electron::getRadius() const
{
    return radius;
}
//------------------------------------------------------------------------------

//-------------------------------------Wall-------------------------------------
Wall::Wall(const Vec2<float>& direction) : 
           PhysEntity(WALL), direction(direction), electricField(0) {}

float Wall::getEnergy() const
{
    return WALL_ENERGY;
}

void Wall::move(float deltaTime)
{
    pos += velocity * deltaTime;
}

void Wall::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    renderer.setColor(COLOR_WALL);

    InfLine line;
    line.from      = viewport.toPixels(pos);
    line.direction = direction;

    renderInfLine(renderer, line);
}

void Wall::setDirection(const Vec2<float>& direction)
{
    this->direction = direction;
}

const Vec2<float>& Wall::getDirection() const
{
    return direction;
}

void Wall::setElectricField(float electricField)
{
    this->electricField = electricField;
}

float Wall::getElectricField() const
{
    return electricField;
}
//------------------------------------------------------------------------------

//-------------------------------------Atom-------------------------------------
Atom::Atom(float size, Charge charge) : PhysEntity(ATOM), size(size), charge(charge) {}

float Atom::getEnergy() const
{
    float v = length(velocity);

    // FIXME:
    printf("Atom energy = %lf\n", mass * v * v / 2 + abs(charge) * ELECTRON_ENERGY);

    return mass * v * v / 2 + abs(charge) * ELECTRON_ENERGY;
}

void Atom::move(float deltaTime)
{
    pos += velocity * deltaTime;
}

void Atom::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    Color color = COLOR_ATOM_NEUTRAL;
    if (charge > 0)
    {
        color = COLOR_ATOM_POSITIVE;
    }
    else if (charge < 0)
    {
        color = COLOR_ATOM_NEGATIVE;
    }

    renderer.setColor(color);

    int32_t       rectSize = viewport.toPixels(size);
    Vec2<int32_t> rectPos  = viewport.toPixels(pos) - 
                             (float) (rectSize / 2) * Vec2<float>{1, 1};

    Rectangle rect{rectPos, rectSize, rectSize};
    renderRect(renderer, rect);
}

void Atom::setSize(float size)
{
    this->size = size;
}

float Atom::getSize() const
{
    return size;
}

void Atom::setCharge(Charge charge)
{
    this->charge = charge;
}

Charge Atom::getCharge() const
{
    return charge;
}
//------------------------------------------------------------------------------