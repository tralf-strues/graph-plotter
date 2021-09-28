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

//-----------------------------------Molecule-----------------------------------
Molecule::Molecule(float radius) : PhysEntity(MOLECULE), radius(radius) {}

void Molecule::move(float deltaTime)
{
    pos += velocity * deltaTime;
}

void Molecule::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    Vec2<int32_t> circlePos    = viewport.toPixels(renderer, pos);
    int32_t       circleRadius = viewport.toPixels(renderer, radius);

    renderCircle(renderer, {circlePos, circleRadius});
}

void Molecule::setRadius(float radius)
{
    this->radius = radius;
}

float Molecule::getRadius() const
{
    return radius;
}
//------------------------------------------------------------------------------

//-------------------------------------Wall-------------------------------------
Wall::Wall(const Vec2<float>& direction) : PhysEntity(WALL), direction(direction) {}

void Wall::move(float deltaTime)
{
    pos += velocity * deltaTime;
}

void Wall::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    InfLine line;
    line.from      = viewport.toPixels(renderer, pos);
    line.direction = viewport.toPixels(renderer, direction);

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
//------------------------------------------------------------------------------
