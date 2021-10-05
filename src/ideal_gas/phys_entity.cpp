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
                       type(type), m_Mass(mass), m_Pos(pos), m_Velocity(velocity) {}

void PhysEntity::setPos(const Vec2<float> pos)
{
    m_Pos = pos;
}

Vec2<float> PhysEntity::getPos() const
{
    return m_Pos;
}

void PhysEntity::setVelocity(const Vec2<float> velocity)
{
    m_Velocity = velocity;
}

Vec2<float> PhysEntity::getVelocity() const
{
    return m_Velocity;
}

void PhysEntity::setMass(float mass)
{
    m_Mass = mass;
}

float PhysEntity::getMass() const
{
    return m_Mass;
}

void PhysEntity::move(float deltaTime)
{
    m_Pos += m_Velocity * deltaTime;
}
//------------------------------------------------------------------------------

//-----------------------------------Electron-----------------------------------
Electron::Electron(float radius) : PhysEntity(ELECTRON), m_Radius(radius)
{
    m_Mass = ELECTRON_MASS;
}

float Electron::getEnergy() const
{
    float v = length(m_Velocity);

    return m_Mass * v * v / 2 + ELECTRON_ENERGY;
}

void Electron::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    renderer.setColor(COLOR_ELECTRON);

    Vec2<int32_t> circlePos    = viewport.toPixels(m_Pos);
    int32_t       circleRadius = viewport.toPixels(m_Radius);

    renderFilledCircle(renderer, {circlePos, circleRadius});
}

void Electron::setRadius(float radius)
{
    m_Radius = radius;
}

float Electron::getRadius() const
{
    return m_Radius;
}
//------------------------------------------------------------------------------

//-------------------------------------Wall-------------------------------------
Wall::Wall(const Vec2<float>& direction) : 
           PhysEntity(WALL), m_Direction(direction), m_ElectricField(0) {}

float Wall::getEnergy() const
{
    return WALL_ENERGY;
}

void Wall::move(float deltaTime)
{
    
}

void Wall::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    Color color = COLOR_WALL_NEUTRAL;

    if (m_ElectricField > 0)
    {
        color = COLOR_WALL_POSITIVE;
    }
    else if (m_ElectricField < 0)
    {
        color = COLOR_WALL_NEGATIVE;
    }

    renderer.setColor(color);

    InfLine line;
    line.from      = viewport.toPixels(m_Pos);
    line.direction = m_Direction;

    renderInfLine(renderer, line);
}

void Wall::setDirection(const Vec2<float>& direction)
{
    m_Direction = direction;
}

const Vec2<float>& Wall::getDirection() const
{
    return m_Direction;
}

void Wall::setElectricField(float electricField)
{
    m_ElectricField = electricField;
}

float Wall::getElectricField() const
{
    return m_ElectricField;
}
//------------------------------------------------------------------------------

//-------------------------------------Atom-------------------------------------
Atom::Atom(float size, Charge charge) : PhysEntity(ATOM), m_Size(size), m_Charge(charge) {}

float Atom::getEnergy() const
{
    float v = length(m_Velocity);

    return m_Mass * v * v / 2 + abs(m_Charge) * ELECTRON_ENERGY;
}

void Atom::updateGraphics(Renderer& renderer, const Viewport& viewport)
{
    Color color = COLOR_ATOM_NEUTRAL;
    if (m_Charge > 0)
    {
        color = COLOR_ATOM_POSITIVE;
    }
    else if (m_Charge < 0)
    {
        color = COLOR_ATOM_NEGATIVE;
    }

    renderer.setColor(color);

    int32_t       rectSize = viewport.toPixels(m_Size);
    Vec2<int32_t> rectPos  = viewport.toPixels(m_Pos) - 
                             (float) (rectSize / 2) * Vec2<float>{1, 1};

    Rectangle rect{rectPos, rectSize, rectSize};
    renderFilledRect(renderer, rect);
}

void Atom::setSize(float size)
{
    m_Size = size;
}

float Atom::getSize() const
{
    return m_Size;
}

void Atom::setCharge(Charge charge)
{
    m_Charge = charge;
}

Charge Atom::getCharge() const
{
    return m_Charge;
}
//------------------------------------------------------------------------------