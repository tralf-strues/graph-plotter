//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file phys_entity.h
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef PHYS_ENTITY_H
#define PHYS_ENTITY_H

#include "../core/utils/viewport.h"

typedef int8_t Charge;

static const Color COLOR_ELECTRON      = 0x7A'6D'C1'FF;

static const Color COLOR_WALL_NEUTRAL  = 0xF6'E4'92'FF;
static const Color COLOR_WALL_POSITIVE = 0xE6'96'A8'FF;
static const Color COLOR_WALL_NEGATIVE = 0xA2'99'D4'FF;

static const Color COLOR_ATOM_NEUTRAL  = 0xF6'E4'92'FF;
static const Color COLOR_ATOM_POSITIVE = 0xE6'96'A8'FF;
static const Color COLOR_ATOM_NEGATIVE = 0xA2'99'D4'FF;

static const Charge ELECTRON_CHARGE    = -1;
static const float  ELECTRON_ENERGY    = 1e-5;
static const float  ELECTRON_MASS      = 1e-4;

static const float  WALL_ENERGY        = 1e5;

//----------------------------------PhysEntity----------------------------------
class PhysEntity
{
public:
    enum Type
    {
        ELECTRON,
        WALL,
        ATOM,
        TOTAL_TYPES,
    };

public:
    const Type type;

    PhysEntity(Type type, float mass = 1, const Vec2<float>& pos = {0.0f, 0.0f}, 
               const Vec2<float>& velocity = {0.0f, 0.0f});

    void setMass(float mass);
    float getMass() const;

    void setPos(const Vec2<float> pos);
    Vec2<float> getPos() const;

    void setVelocity(const Vec2<float> velocity);
    Vec2<float> getVelocity() const;

    virtual float getEnergy() const = 0;
    virtual void move(float deltaTime);
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) = 0;

protected:
    float       m_Mass;
    Vec2<float> m_Pos;
    Vec2<float> m_Velocity;
};
//------------------------------------------------------------------------------

//-----------------------------------Electron-----------------------------------
class Electron : public PhysEntity
{
public:
    Electron(float radius = 1);

    virtual float getEnergy() const override;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) override;

    void setRadius(float radius);
    float getRadius() const;

private:
    float m_Radius;
};
//------------------------------------------------------------------------------

//-------------------------------------Wall-------------------------------------
class Wall : public PhysEntity
{
public:
    Wall(const Vec2<float>& direction = Vec2<float>{1, 1});

    virtual float getEnergy() const override;
    virtual void move(float deltaTime) override;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) override;

    void setDirection(const Vec2<float>& direction);
    const Vec2<float>& getDirection() const;

    void setElectricField(float electricField);
    float getElectricField() const;

private:
    Vec2<float> m_Direction;
    float       m_ElectricField;
};
//------------------------------------------------------------------------------

//-------------------------------------Atom-------------------------------------
class Atom : public PhysEntity
{
public:
    Atom(float size = 1, Charge charge = 0);

    virtual float getEnergy() const override;
    virtual void updateGraphics(Renderer& renderer, const Viewport& viewport) override;

    void setSize(float size);
    float getSize() const;

    void setCharge(Charge charge);
    Charge getCharge() const;

private:
    float  m_Size;
    Charge m_Charge;
};
//------------------------------------------------------------------------------

#endif // PHYS_ENTITY_H